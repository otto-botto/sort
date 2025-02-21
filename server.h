#pragma once

#include <netinet/in.h>

typedef enum
{
    GET,
    POST,
    DELETE,
    PUT
} Method;

typedef struct
{
    struct sockaddr_in address;
    int server_fd;
} Server;

typedef struct
{
    char* path;
    char* content;
    int content_length;
    int client_socket;
    Method method;
} Request;

// Create a new server and listen on `port`.
Server create_server(int port);

// Waits for the next request. If the program is terminated
// with Ctrl-C, then this will capture the termination and
// cleanup the server.
Request next_request(const Server* server);

// Writes a response to the client. This method will free the
// state associated with `request`.
void respond(const Server* server,
             Request* request,
             int http_status,
             char* body);
