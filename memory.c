#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach/error.h>
#include <mach/mach_vm.h>

vm_mem_t* vm_mem_alloc(
    task_t task, 
    size_t size) 
{
    vm_mem_t* mem = malloc(sizeof(vm_mem_t));
    mem->task = task;
    mem->size = size;

    if (mach_vm_allocate(task, 
            &mem->addr, 
            size, 
            VM_FLAGS_ANYWHERE) != KERN_SUCCESS) 
    {
        panic("Unable to allocate VM memory\n");
    }

    return mem;
}

void vm_mem_free(vm_mem_t* mem) {
    // TODO: find out how to deallocate vm memory
    free(mem);
}

void* vm_mem_read(
    vm_mem_t* mem,
    void* offset,
    size_t count)
{
    uint32_t size;
    void* vm_ptr;
    if (vm_read(
            mem->task, 
            (vm_offset_t)offset, 
            count, 
            (vm_offset_t*)&vm_ptr, 
            &size) != KERN_SUCCESS) {
        panic("vm_read failed\n");
    }

    uint8_t* buffer = malloc(size);
    if (buffer == NULL) {
        panic("malloc failed\n");
    }

    memcpy(buffer, vm_ptr, size);
    return buffer;
}

void vm_mem_write(
    vm_mem_t* mem, 
    void* dest,
    void* source, 
    size_t count) 
{
    if (vm_write(mem->task, 
            (mach_vm_address_t)dest, 
            (vm_offset_t)source, 
            count) != KERN_SUCCESS)
    {
        panic("vm_write failed\n");
    }
}

void vm_mem_mark_execute(vm_mem_t* mem) {
    if (vm_protect(mem->task, 
            mem->addr, 
            mem->size, 
            FALSE, 
            VM_PROT_READ | VM_PROT_EXECUTE) != KERN_SUCCESS) {
        panic("vm_protect (executable) failed");
    }
}

void vm_mem_mark_write(vm_mem_t* mem) {
    /* Mark stack as writable  - not really necessary */
    if (vm_protect(mem->task, 
            mem->addr, 
            mem->size, 
            TRUE, 
            VM_PROT_READ | VM_PROT_WRITE) != KERN_SUCCESS) {
        panic("vm_protect (writable) failed");
    }

