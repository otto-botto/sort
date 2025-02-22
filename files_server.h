//
// Created by lora on 2/8/25.
//

#ifndef FILES_SERVER_H
#define FILES_SERVER_H

typedef enum {
    ALPHA,
    CHRONO,
    NUMBER
}Type;

void welcome(Server server, Request request);
void sort(Server server, Request request);
// int sort_number();
void delete_file(Server server, Request request);

int test_gtest();


#endif //FILES_SERVER_H
