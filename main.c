#include <stdio.h>
#include "server.h"
#include "files_server.h"

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
                sort(server, request);
                break;
            default:
                fprintf(stderr, "request method not understood");
        }
    }
    return 0;
}