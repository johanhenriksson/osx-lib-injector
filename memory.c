#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach/error.h>
#include <mach/mach_vm.h>

mach_vm_address_t remote_alloc(task_t task, size_t size) {
    mach_vm_address_t addr;
    if (mach_vm_allocate(task, 
            &addr, 
            size, 
            VM_FLAGS_ANYWHERE) != KERN_SUCCESS) 
    {
        panic("Unable to allocate VM memory\n");
    }
    return addr;
}

void remote_write(
    task_t task, 
    mach_vm_address_t offset,
    void* ptr, 
    size_t count) 
{
    if (vm_write(task, 
            offset, 
            (vm_offset_t)ptr, 
            count) != KERN_SUCCESS)
    {
        panic("vm_write failed\n");
    }
}
