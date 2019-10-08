#ifndef SCAPULA_OS_H
#define SCAPULA_OS_H

#include <stdint.h>

struct scapula_os_config {
    void * heap_address;
    uint32_t heap_size;
};

// init_scapula_os
//
// Initilize Scapula OS, verifying the current excecution environment and
// setting up global state
//
void init_scapula_os(struct scapula_os_config * cfg);

#endif
