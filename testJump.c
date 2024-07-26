#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include "testAll.h"

START_TEST(test_JMP) {
    cpu.PC = 0xFF00;
    u_int16_t operand = 0xAF03;
    BYTE Low = 0x03;
    BYTE High = 0xAF;
    JMP(&cpu, Low, High);

    ck_assert_msg(cpu.PC == operand, "incorrect PC value");
}
END_TEST

START_TEST(test_JSR) {
    cpu.PC = 0xE34D;
    u_int16_t operand = 0xE102;
    BYTE Low = 0x02;
    BYTE High = 0xE1;
    //set up to reflect incrementing of PC by 2 on any instruction executing
    cpu.PC += 2;
    JSR(&cpu, memory, Low, High);
    ck_assert_msg(cpu.PC == operand, "incorrect PC value");
    ck_assert_msg(memory[0x01FF] == 0xE3, "high byte of PC incorrectly stored");
    ck_assert_msg(memory[0x01FE] == 0x4F, "low byte of PC incorrectly stored");

}
END_TEST

START_TEST(test_RTS) {
    BYTE high = 0x7C;
    BYTE low = 0x08;
    push_to_stack(&cpu, memory, high);
    push_to_stack(&cpu, memory, low);
    uint16_t expectedPC = 0x7C09;

    RTS(&cpu, memory);
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
    tcase_add_checked_fixture(tc_core, setup, teardown);
    
    suite_add_tcase(s, tc_core);

    return s;
}
