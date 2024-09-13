#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include "testAll.h"

START_TEST(test_JMP) {
    u_int16_t expectedPC = 0x0101;
    cpu.PC = expectedPC;
    NOP(&cpu);
    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC value");
    ck_assert_msg(cpu.S == 0xFF, "incorrect Stack Pointer value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) != 0, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");

}
END_TEST

START_TEST(test_JSR) {
    u_int16_t expectedPC = 0x0101;
    cpu.PC = expectedPC;
    NOP(&cpu);
    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC value");
    ck_assert_msg(cpu.S == 0xFF, "incorrect Stack Pointer value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) != 0, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");

}
END_TEST

START_TEST(test_RTS) {
    u_int16_t expectedPC = 0x0101;
    cpu.PC = expectedPC;
    RTS(&cpu);
    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC value");
    ck_assert_msg(cpu.S == 0xFF, "incorrect Stack Pointer value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) != 0, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
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
