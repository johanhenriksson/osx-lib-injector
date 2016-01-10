#ifndef PAYLOAD_H
#define PAYLOAD_H
#include <stdlib.h>

typedef struct payload_t payload_t;

struct payload_t {
    void* code;
    void* path;
    size_t code_len;
    size_t path_len;
};

payload_t* prepare_payload(char* lib_path);

int find_pattern(char* pattern, 
    int length, 
    void* start, 
    int scan);

int code_patch( void* code, 
    size_t code_len, 
    char* pattern, 
    char* patch, 
    size_t patch_len);

size_t payload_size();
size_t payload_code_size();
size_t payload_path_size();

#endif
