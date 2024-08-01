#include <check.h>
#include <stdlib.h>
#include "cpu.h"

START_TEST(test_fetch) {
    BYTE memory[10] = {0xA4, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    BYTE expected_op = 0xA4;
    //LDX is A4
    //LDA is A1
    BYTE actual_op = fetch(&cpu, memory);
    ck_assert_msg(actual_op == expected_op, "operand not properly fetched");
}
END_TEST

START_TEST(test_incrementPC) {
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    BYTE expectedInitialPC = 0x0;
    BYTE expectedIncrementedPC = 0x1;
    ck_assert_msg(cpu->PC == expectedInitialPC, "initial PC not set to 0");
    incrementPC(cpu);
    ck_assert_msg(cpu->PC == expectedIncrementedPC, "PC not correctly incremented");
}
END_TEST


START_TEST(test_accumulator_addressing) {
    BYTE memory[10] = {0xA4, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    BYTE *expectedPtr = &cpu->A;
    BYTE *actualPtr = A_addressing(cpu, memory);
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return accumulator address");
}
END_TEST

Suite *fetch_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Fetch");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_fetch);
    tcase_add_test(tc_core, test_incrementPC);
    tcase_add_test(tc_core, test_accumulator_addressing);
    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
    SRunner *sr;
    Suite *s;
    int number_failed;

    s = fetch_suite();
    sr = srunner_create(s);
   
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
