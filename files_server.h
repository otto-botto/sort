#include "server.h"
#ifndef FILES_SERVER_H
#define FILES_SERVER_H

typedef enum {
    ALPHA,
    CHRONO,
    NUMBER
}Type;

void sort(Server server, Request request);
void sort_numbers(int* array, int array_len);
void sort_alpha(char** array, int array_len);
int test_gtest();


#endif //FILES_SERVER_H
