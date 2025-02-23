
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "server.h"
#include "files_server.h"
#define DIRECTORY "/home/lora/Documents/server"

int test_gtest() {
    return 5;
}

void sort_numbers(int* array, int array_len) {
    for(int i = 1; i < array_len; i++) {
        int j = i;
        while(array[j-1] > array[j] && j > 0) {
            int temp = array[j-1];
            array[j - 1] = array[j];
            array[j] = temp;
            j --;
        }
    }
}

void sort_alpha(char** array, int array_len) {
    for(int i = 1; i < array_len; i++) {
        int j = i ;
        while((strcmp(array[j - 1], array[j])>0) && j > 0) {
            char* temp = array[j - 1];
            array[j - 1] = array[j];
            array[j] = temp;
            j--;
        }
    }
}



void parse(const char* body) {
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
    char* sort_type = type->valuestring;

    if(strcmp("NUMBER", sort_type) == 0){
        // allocate an array of integers
        int* array = (int*)malloc(array_len * sizeof(int));
        if (array == NULL) {
            fprintf(stderr, "Failed to allocate memory for array\n");
            exit(EXIT_FAILURE);
        }
        //place the values in the array
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
        // call sort_numbers
        sort_numbers(array, array_len);
        for(int i = 0; i < array_len; i++) {
            printf("%d\n", array[i]);
        }
        free(array);
        goto end;
    }

    if(strcmp("ALPHA", sort_type) == 0) {
        // char* array = (char*)malloc(array_len * sizeof(char));
        return;
    }

    if(strcmp("CHRONO", sort_type) == 0) {
        // char* array = (char*)malloc(array_len * sizeof(char));
        return;
    }

    else {
        fprintf(stderr, "sorting type not understood");
        exit(EXIT_FAILURE);
    }


end:
    cJSON_Delete(body_json);
}

void sort(Server server, Request request) {
    parse(request.content);

    char* message = "Request successfully sent.\n";
    respond(&server, &request, 200, message);
}


