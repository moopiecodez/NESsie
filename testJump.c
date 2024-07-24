#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_JMP) {
    power_cpu();
    cpu.PC = 0xFF00;
    u_int16_t operand = 0xAF03;
    BYTE Low = 0x03;
    BYTE High = 0xAF;
    JMP(Low, High);

    ck_assert_msg(cpu.PC == operand, "incorrect PC value");
}
END_TEST

START_TEST(test_JSR) {
    power_cpu();
    cpu.PC = 0xE34D;
    u_int16_t operand = 0xE102;
    BYTE Low = 0x02;
    BYTE High = 0xE1;
    //set up to reflect incrementing of PC by 2 on any instruction executing
    cpu.PC += 2;
    JSR(memory, Low, High);
    ck_assert_msg(cpu.PC == operand, "incorrect PC value");
    ck_assert_msg(memory[0x01FF] == 0xE3, "high byte of PC incorrectly stored");
    ck_assert_msg(memory[0x01FE] == 0x4F, "low byte of PC incorrectly stored");

}
END_TEST

START_TEST(test_RTS) {
    power_cpu();
    BYTE high = 0x7C;
    BYTE low = 0x08;
    push_to_stack(memory, high);
    push_to_stack(memory, low);
    uint16_t expectedPC = 0x7C09;

    RTS(memory);
    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC value");
    ck_assert_msg(cpu.S == 0xFF, "incorrect Stack Pointer value");    
}
END_TEST

Suite *Jump_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Jump instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_JMP);
    tcase_add_test(tc_core, test_JSR);
    tcase_add_test(tc_core, test_RTS);
    
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = Jump_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}