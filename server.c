#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>

#include "server.h"

const static int MAX_HTTP_REQUEST_SIZE = 10 * 1024 * 1024;

// Whether the server should keep running. A signal
// handler is responsible for updating this.
static volatile int keepRunning = 1;

// Signal handler that captures termination events
// and indicates that the server should be shutdown.
void sigHandler(int unused) {
    keepRunning = 0;
}

// Appender helps with building strings.
typedef struct {
    char *buffer;
    int curr;
    int capacity;
} Appender;

Appender create_appender(const int n) {
    Appender appender;
    appender.buffer = malloc(sizeof(char) * n);
    appender.capacity = n;
    appender.curr = 0;
    return appender;
}

void free_appender(const Appender *appender) {
    free(appender->buffer);
}

void append(Appender *appender, char *str) {
    int new_curr = appender->curr + strlen(str);
    if (new_curr >= appender->capacity) {
        fprintf(stderr, "append overflowed capacity");
        exit(EXIT_FAILURE);
    }
    memcpy(appender->buffer + appender->curr, str, strlen(str));
    appender->curr = new_curr;
}

// Returns the minimum of two comparable objects.
#define min(a,b) (((a) < (b)) ? (a) : (b))

Server create_server(const int port) {
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    Server server;
    server.server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server.server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("failed to set reuse addr");
        exit(EXIT_FAILURE);
    }

    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(port);

    if (bind(server.server_fd, (struct sockaddr *) &server.address, sizeof(server.address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(server.server_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);

    if (listen(server.server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return server;
}

Request next_request(const Server *server) {
    int addrlen = sizeof(server->address);
    int client_socket = -1;
    while (keepRunning) {
        client_socket = accept(server->server_fd, (struct sockaddr *) &server->address, (socklen_t *) &addrlen);
        if (client_socket >= 0) {
            break;
        }
    }
    if (!keepRunning) {
        printf("Server disconnected\n");
        close(server->server_fd);
        exit(EXIT_SUCCESS);
    }
    char *request_buffer = malloc(MAX_HTTP_REQUEST_SIZE);
    int read_bytes = recv(client_socket, request_buffer, MAX_HTTP_REQUEST_SIZE, 0);
    if (read_bytes < 0) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
    while (1) {
        int next_read = recv(client_socket,
            request_buffer + read_bytes,
            MAX_HTTP_REQUEST_SIZE - read_bytes, 0);
        read_bytes += next_read;
        if (next_read < 0) {
          break;
        }
    }
    if (read_bytes == MAX_HTTP_REQUEST_SIZE) {
        printf("Request size exceeded MAX_HTTP_REQUEST_SIZE\n");
        exit(EXIT_FAILURE);
    }

    Request request;
    int p = 0;
    if (strncmp(request_buffer, "GET ", min(4, read_bytes)) == 0) {
        request.method = GET;
        p += 4;
    } else if (strncmp(request_buffer, "POST ", min(5, read_bytes)) == 0) {
        request.method = POST;
        p += 5;
    } else if (strncmp(request_buffer, "PUT ", min(4, read_bytes)) == 0) {
        request.method = PUT;
        p += 4;
    } else if (strncmp(request_buffer, "DELETE ", min(7, read_bytes)) == 0) {
        request.method = DELETE;
        p += 7;
    } else {
        fprintf(stderr, "request method not understood");
        exit(EXIT_FAILURE);
    }

    int path_end = -1;
    for (int i = p; i < read_bytes; i++) {
        if (request_buffer[i] == ' ') {
            path_end = i;
            break;
        }
    }
    if (path_end == -1) {
        fprintf(stderr, "path not found");
        exit(EXIT_FAILURE);
    }

    char *end_of_headers = strstr(request_buffer, "\r\n\r\n");
    int start_of_body = (end_of_headers - request_buffer) + 4;
    int body_size = read_bytes - start_of_body + 1;
    if (body_size > 0) {
        request.content_length = body_size;
        request.content = malloc(body_size);
        memcpy(request.content, request_buffer + start_of_body, body_size);
    } else if (body_size == 0) {
        request.content_length = 0;
        request.content = NULL;
    }
    request.path = calloc(path_end - p + 1, sizeof(char));
    strncpy(request.path, request_buffer + p, path_end - p);
    request.client_socket = client_socket;
    free(request_buffer);
    return request;
}

void respond(const Server *server,
             Request *request,
             int http_status,
             char *body) {
    Appender appender = create_appender((int) strlen(body) * 1024);
    append(&appender, "HTTP/1.1 ");
    switch (http_status) {
        case 200:
            append(&appender, "200 OK\r\n");
            break;
        case 403:
            append(&appender, "403 Forbidden\r\n");
            break;
        case 404:
            append(&appender, "404 Not Found\r\n");
            break;
        default: {
            perror("unrecognized HTTP status");
            exit(EXIT_FAILURE);
        }
    }
    append(&appender, "Content-Type: text/plain\r\n");
    char *content_length = calloc(1024, 1);
    snprintf(content_length, 1024, "Content-Length: %d\r\n\r\n", (int) strlen(body));
    append(&appender, content_length);
    free(content_length);
    append(&appender, body);
    send(request->client_socket, appender.buffer, appender.curr, 0);
    close(request->client_socket);
    free(request->path);
    free_appender(&appender);
}
