
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "server.h"
#include "sort_server.h"


int test_gtest() {
    return 5;
}

void sort_numbers(int* array, int array_len) {
    for(int i = 1; i < array_len; i++) {
        int j = i;
        while(j > 0 && array[j-1] > array[j]) {
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
        while(j> 0 && (strcmp(array[j - 1], array[j])>0)) {
            char* temp = array[j - 1];
            array[j - 1] = array[j];
            array[j] = temp;
            j--;
        }
    }
}

char* make_null_term_string(int length) {
    char* string = (char*)malloc((length + 1)*sizeof(char));
    string[length] = '\0';
    return string;
}

char** parse_strings(const cJSON* items, int array_len) {
    // allocate memory for char** array
    char** array = (char**)malloc(array_len * sizeof(char*));
    if(array == NULL) {
        fprintf(stderr, "Failed to allocate memory for array\n");
        exit(EXIT_FAILURE);
    }
    //place the values in the array
    for(int i =0; i < array_len; i++) {
        cJSON* item = cJSON_GetArrayItem(items, i);
        if (item == NULL) {
            fprintf(stderr, "Failed to parse JSON\n");
        }
        if (!cJSON_IsString(item)) {
            fprintf(stderr, "Failed to parse JSON\n");
        }
        int string_len = strlen(item->valuestring);
        array[i] = make_null_term_string(string_len);
        char* source = item->valuestring;
        strncpy(array[i], source, string_len);
    }
    return array;
}

int* parse_integers(const cJSON* items, int array_len) {
    // allocate memory for int* array
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
    return array;
}

char* build_str_from_int_arr(int* arr, int size) {
    int max_digits = 10;
    int buffer_size = size * (max_digits + 1) + 1;
    char* str = (char*)malloc(buffer_size * sizeof(char));
    str[0] = '\0';
    strcat(str, "{\"numbers\" : [");

    for(int i=0; i < size; i++) {
        char temp[max_digits + 1];
        sprintf(temp, "%d", arr[i]);
        strcat(str, temp);
        if(i < size - 1) {
            strcat(str, ", ");
        } else if(i == size - 1) {
            strcat(str, "]}");
        }
    }
    return str;
}

char* build_str_from_str(char** arr, int size) {
    int max_characters = 100;
    int buffer_size = size * (max_characters + 1) + 1;
    char* str = (char*)malloc(buffer_size * sizeof(char));
    str[0] = '\0';
    strcat(str, "{\"strings\" : [");

    for(int i=0; i < size; i++) {
        char temp[max_characters + 1];
        sprintf(temp, "\"%s\"", arr[i]);
        strcat(str, temp);
        if(i < size - 1) {
            strcat(str, ", ");
        }else if(i == size - 1) {
            strcat(str, "]}");
        }
    }
    return str;
}

char* parse(const char* body) {

    // get the request body
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
        int* array = parse_integers(items, array_len);
        sort_numbers(array, array_len);
        cJSON_Delete(body_json);
        char* response = build_str_from_int_arr(array, array_len);
        return response;

    }

    if(strcmp("ALPHA", sort_type) == 0 || strcmp("CHRONO", sort_type) == 0) {
        char** array = parse_strings(items, array_len);
        //call sort_strings
        sort_alpha(array, array_len);
        cJSON_Delete(body_json);
        char* response = build_str_from_str(array, array_len);
        return response;
    }


    else {
        fprintf(stderr, "sorting type not understood");
        exit(EXIT_FAILURE);
    }
}

void sort(Server server, Request request) {
    char* message = parse(request.content);
    printf("%s\n", message);
    respond(&server, &request, 200, message);
    free(message);
}


