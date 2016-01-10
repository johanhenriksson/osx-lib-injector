#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "payload.h"

/*
mach_vm_address_t remote_alloc(task_t task, size_t size) {
    mach_error_t result = 0;
    mach_vm_address_t addr;

    result = mach_vm_allocate(task, &addr, size, VM_FLAGS_ANYWHERE);
    if (result != KERN_SUCCESS) {
        fprintf(stderr, "Unable to allocate VM memory\n");
        exit(-1);
    }

    return addr;
}
*/

int main() {
    void* code = prepare_payload("test path goes here");
    FILE* f = fopen("output","w");
    fwrite(code, 1, payload_size(), f);
    fclose(f);
    return 0;
}
