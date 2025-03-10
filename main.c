#include <stdio.h>
#include <string.h>
#include "server.h"
#include "sort_server.h"

int main(int argc, char* argv[]) {
    Server server = create_server(5527);
    while (1) {
        Request request = next_request(&server);
        switch(request.method) {
            case GET:
                char* message = "HTTP/1.1 200 OK\r\n";
                respond(&server, &request, 200, message);
                break;
            case POST:
                if(strcmp(request.path, "/sort") == 0) {
                    sort(server, request);
                }

                break;
            default:
                fprintf(stderr, "request method not understood");
        }
    }
    return 0;
}