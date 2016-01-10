#ifndef MEMORY_H
#define MEMORY_H

struct vm_mem_t {
    mach_vm_address_t addr; // remote copy
    void* local; // local copy
};

#endif
