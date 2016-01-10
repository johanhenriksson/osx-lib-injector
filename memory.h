#ifndef MEMORY_H
#define MEMORY_H

#include <mach/mach.h>
#include <mach/mach_vm.h>

typedef struct vm_mem_t vm_mem_t;

struct vm_mem_t {
    task_t task;
    mach_vm_address_t addr; // remote copy
    size_t size;
};

vm_mem_t* vm_mem_alloc(task_t, size_t);
void vm_mem_free(vm_mem_t*);

void vm_mem_write(vm_mem_t*, void*, void*, size_t);
void* vm_mem_read(vm_mem_t*, void*, size_t);
void vm_mem_mark_execute(vm_mem_t* mem) {
void vm_mem_mark_write(vm_mem_t* mem) {

#endif
