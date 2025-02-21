cmake_minimum_required(VERSION 3.29)
project(sort C)

set(CMAKE_C_STANDARD 11)

add_executable(sort files_server.c
        server.c
        cJSON.c)
