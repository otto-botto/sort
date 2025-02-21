
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "cJSON.h"
#include "server.h"
#include "files_server.h"

#define DIRECTORY "/home/lora/Documents/server"

int parse(const char* body) {
    int return_val;
    cJSON* body_json = cJSON_Parse(body);
    if (body_json == NULL) {
        fprintf(stderr, "Failed to parse JSON\n");
        exit(EXIT_FAILURE);
    }
    const cJSON* type = NULL;
    const cJSON* items = NULL;
    int array_len;

    type = cJSON_GetObjectItemCaseSensitive(body_json, "type");
    items = cJSON_GetObjectItemCaseSensitive(body_json, "items");
    array_len = cJSON_GetArraySize(items);
    return_val = array_len;
    char* sort_type = type->valuestring;
    void* array[array_len];

    if(strcmp("NUMBER", sort_type) == 0){
        // allocate an array of integers
        int* array = (int*)malloc(array_len * sizeof(int));;
        if (array == NULL) {
            fprintf(stderr, "Failed to allocate memory for array\n");
        }

        for(int i =0; i < array_len; i++) {
            cJSON* item = cJSON_GetArrayItem(items, i);
            if (item == NULL) {
                fprintf(stderr, "Failed to parse JSON\n");
            }
            if (!cJSON_IsNumber(item)) {
                fprintf(stderr, "Failed to parse JSON\n");
            }
            array[i] = item->valueint;
        }

        goto end;
    }
    if(strcmp("ALPHA", sort_type) == 0) {
        return_val =  ALPHA;
    }

    if(strcmp("CHRONO", sort_type) == 0) {
        return_val =  CHRONO;
    }

    else {
        fprintf(stderr, "sorting type not understood");
        exit(EXIT_FAILURE);
    }


end:
    cJSON_Delete(body_json);
    return return_val;
}

void welcome(Server server, Request request) {
    char* message = "Welcome to the files server.\n";
    respond(&server, &request, 200, message);
}


void sort(Server server, Request request) {
    int type = parse(request.content);
    printf("%d\n", type);
    char* message = "Request successfully sent.\n";
    respond(&server, &request, 200, message);
}


int main(int argc, char* argv[]) {
    Server server = create_server(5527);
    while (1) {
        Request request = next_request(&server);
        switch(request.method) {
            case GET:
                welcome(server, request);
                break;
            case POST:
                sort(server, request);
                break;
            case DELETE:
                break;
            default:
                fprintf(stderr, "request method not understood");
        }
    }

    return 0;
}