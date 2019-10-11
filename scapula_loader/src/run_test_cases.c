#include "run_test_cases.h"
#include "print.h"
#include "panic.h"
#include "init.h"
#include <stdint.h>
#include <libfdt.h>
#include <aligned_malloc.h>
#include "bfelf_loader.h"

// Memory allocator for bfelf_loader
void * loader_malloc(size_t size)
{
    return aligned_malloc(0x1000, size);
}

void run_test_cases(void * image)
{
    char * path = "/images/aarch64_testcases";
    int elf_node = 0;
    const void * elf_buffer;
    int elf_buffer_len = 0;
    struct bfelf_file_t ef = {0};
    status_t ret = 0;
    struct scapula_os_config * cfg;
    void * test_case_heap;
    uint32_t test_case_heap_size = 0x200000;

    SCAPULA_INFO("Loading test cases from image: 0x%x", image);

    // Find the aarch64 testcase ELF file in the .fit image
    elf_node = fdt_path_offset(image, path);
    if (elf_node < 0) {
        SCAPULA_ERROR("Couldn't find testcases in image tree (%s)", path);
        panic();
    }

    // Locate the node property (buffer) that contains the ELF executable
    elf_buffer = fdt_getprop(image, elf_node, "data", &elf_buffer_len);
    if(elf_buffer_len <= 0) {
        SCAPULA_ERROR("Couldn't find the data to load! (%d)", elf_buffer_len);
        panic();
    }

    // Initialize and allocate memory for the ELF executable to be loaded to
    ret = bfelf_file_init(elf_buffer, elf_buffer_len, &ef);
    if(ret != BFSUCCESS) {
        SCAPULA_ERROR("Failed to intialize ELF Loader");
        panic();
    } 

    void * exec = bfelf_file_alloc(&ef, loader_malloc);
    if(exec == NULL) {
        SCAPULA_ERROR("Failed to allocate RWE memory");
        panic();
    }

    // Load the ELF file into memory
    bfelf_file_load(exec, 0x0, &ef, NULL);

    // Initialize a configuration argument to be passed to main()
    test_case_heap = loader_malloc(test_case_heap_size);
    if (!test_case_heap) {
        SCAPULA_ERROR("out of memory");
        panic();
    }

    cfg = loader_malloc(sizeof(struct scapula_os_config));
    if (!cfg) {
        SCAPULA_ERROR("out of memory");
        panic();
    }

    cfg->heap_address = test_case_heap;
    cfg->heap_size = test_case_heap_size;

    // Execute the ELF file
    uintptr_t entry_point = ef.entry;
    SCAPULA_INFO("Executing ELF file at entry point: 0x%x", entry_point);
    ((void (*)(struct scapula_os_config *))(entry_point))(cfg);
}
