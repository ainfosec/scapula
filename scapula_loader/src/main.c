#include "init.h"
#include "print.h"
#include "exception_level.h"
#include "run_test_cases.h"

#include <stdint.h>
#include <malloc.h>

// Address of the .fit image that contains the test cases to be loaded.
// The .fit image is statically linked to this binary as test_case_image.a
extern char test_case_image;

// Available heap memory is currently compiled into the scapula_loader binary,
// with these symbols defined by the scapula_loader's linker script. 
extern uint64_t scapula_os_heap_start;
extern uint64_t scapula_os_heap_end;

void scapula_loader_main()
{
    print_banner();
    size_t heap_size = (uintptr_t)&scapula_os_heap_end - (uintptr_t)&scapula_os_heap_start;
    malloc_addblock(&scapula_os_heap_start, heap_size);
    malloc_init();
    run_test_cases(&test_case_image);
    print_success();
    while(1);
}
