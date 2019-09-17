#include "init.h"
#include "print.h"
#include "run_test_cases.h"

// Address of the .fit image that contains the test cases to be loaded.
// The .fit image is statically linked to this binary as test_case_image.a
extern char test_case_image;

void scapula_loader_main()
{
    print_banner();
    init_scapula_os();
    run_test_cases(&test_case_image);
    print_success();
    while(1);
}
