#include <check.h>
#include <stdlib.h>
#include "testAll.h"

BYTE memory[0xFFFF];
CPU cpu = {0x0000u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

void setup(void) {
    power_cpu(&cpu);
}
void teardown(void) {
    ;
}

int main() {
    SRunner *sr;
    Suite *s;
    int number_failed;

    s = suite_create("Master Suite");
    sr = srunner_create(s);
    srunner_add_suite(sr, Arithmetic_suite());
    srunner_add_suite(sr, Branch_suite());
    srunner_add_suite(sr, Shift_suite());
    srunner_add_suite(sr, Flag_suite());
    srunner_add_suite(sr, INC_DEC_suite());
    srunner_add_suite(sr, Jump_suite());
    srunner_add_suite(sr, Logical_suite());
    srunner_add_suite(sr, Stack_suite());
    srunner_add_suite(sr, Store_suite());
    srunner_add_suite(sr, System_suite());
    srunner_add_suite(sr, Transfer_suite());
    srunner_add_suite(sr, Load_suite());
    srunner_add_suite(sr, Power_suite());

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
