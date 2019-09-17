#include "run_test_cases.h"
#include "print.h"
#include "panic.h"
#include <stdint.h>
#include <libfdt.h>
#include <aligned_malloc.h>
#include "bfelf_loader.h"

// libmemory_freelist can't find this symbol, add here as a hack to allow linking
void __assert_fail(const char* expr, const char* file,
        unsigned int line, const char* function)
{ return; }

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

    bfelf_file_load(exec, 0x0, &ef, NULL);

    uintptr_t entry_point = ef.entry;
    SCAPULA_INFO("Executing ELF file at entry point: 0x%x", entry_point);
    ((void (*)(void))(entry_point))();
}
