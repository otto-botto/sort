#include "cJSON.h"
#include "server.h"
#ifndef FILES_SERVER_H
#define FILES_SERVER_H

typedef enum {
    ALPHA,
    CHRONO,
    NUMBER
}Type;

void sort(Server server, Request request);

int test_gtest();


#endif //FILES_SERVER_H
