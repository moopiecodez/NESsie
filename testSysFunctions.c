#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include "testAll.h"

START_TEST(test_BRK) {
    //clear all flags to test BRK sets B flag correctly
    cpu.P = 0x00;
    cpu.S = 0xFC;
    cpu.DB = 0;
    BRK(&cpu);
    ck_assert_msg(cpu.S == 0xFC, "incorrect Stack Pointer value");
    ck_assert_msg(cpu.P == 0x10, "incorrect Processor Status register");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) != 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_NOP) {
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

START_TEST(test_RTI) {
    cpu.DL = 0x86;
    cpu.S = 0xFE;
    RTI(&cpu);
    ck_assert_msg(cpu.P == 0x86, "incorrect Processor Status register value");
    ck_assert_msg(cpu.S == 0xFF, "incorrect Stack Pointer value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) != 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) != 0, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) != 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");    
}
END_TEST

Suite *System_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("System function instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_BRK);
    tcase_add_test(tc_core, test_NOP);
    tcase_add_test(tc_core, test_RTI);
    tcase_add_checked_fixture(tc_core, setup, teardown);
    
    suite_add_tcase(s, tc_core);

    return s;
}
