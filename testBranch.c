#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_BCC) {
    cpu.PC = 0xFF00;
    BYTE displacement = 0x03;
    BCC(&cpu, displacement);

    ck_assert_msg(cpu.PC == 0xFF03, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_BCS) {
    cpu.PC = 0xFF00;
    BYTE displacement = 0x03;
    setFlag(&cpu, FLAG_C);

    BCS(&cpu, displacement);
    ck_assert_msg(cpu.PC == 0xFF03, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_BEQ) {
    cpu.PC = 0xFF00;
    BYTE displacement = 0x04;
    setFlag(&cpu, FLAG_Z);

    BEQ(&cpu, displacement);
    ck_assert_msg(cpu.PC == 0xFF04, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag");
}
END_TEST

START_TEST(test_BMI) {
    cpu.PC = 0xFF00;
    BYTE displacement = 0x05;
    setFlag(&cpu, FLAG_N);

    BMI(&cpu, displacement);
    ck_assert_msg(cpu.PC == 0xFF05, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
}
END_TEST

START_TEST(test_BNE) {
    cpu.PC = 0xFF00;
    BYTE displacement = 0x03;

    BNE(&cpu, displacement);
    ck_assert_msg(cpu.PC == 0xFF03, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_BPL) {
    cpu.PC = 0xFF00;
    BYTE displacement = 0x06;
    BPL(&cpu, displacement);
    ck_assert_msg(cpu.PC == 0xFF06, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
}
END_TEST

START_TEST(test_BVC) {
    cpu.PC = 0xFF00;
    BYTE displacement = 0x04;

    BVC(&cpu, displacement);
    ck_assert_msg(cpu.PC == 0xFF04, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
}
END_TEST

START_TEST(test_BVS) {
    setFlag(&cpu, FLAG_V);
    cpu.PC = 0xFF00;
    BYTE displacement = 0x01;

    BVS(&cpu, displacement);
    ck_assert_msg(cpu.PC == 0xFF01, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 1, "incorrect N flag");
}
END_TEST

Suite *Branch_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Branch instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_BCC);
    tcase_add_test(tc_core, test_BCS);
    tcase_add_test(tc_core, test_BEQ);
    tcase_add_test(tc_core, test_BMI);
    tcase_add_test(tc_core, test_BNE);
    tcase_add_test(tc_core, test_BPL);
    tcase_add_test(tc_core, test_BVC);
    tcase_add_test(tc_core, test_BVS);
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
