#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "testAll.h"

START_TEST(test_ASL) {
    memory[0xFFF1] = 0x2e;
    BYTE expected = 0x5c;

    ASL(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ASL_neg) {
    memory[0xFFF1] = 0xEC;
    BYTE expected = 0xD8;

    ASL(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ASL_car) {
    memory[0xFFF1] = 0x95;
    BYTE expected = 0x2A;

    ASL(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ASL_zer) {
    memory[0xFFF1] = 0x80;
    BYTE expected = 0x00;

    ASL(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR) {
    memory[0xFFF1] = 0x5c;
    BYTE expected = 0x2e;

    LSR(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR_neg) {
    memory[0xFFF1] = 0xec;
    BYTE expected = 0x76;

    LSR(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR_car) {
    memory[0xFFF1] = 0x3b;
    BYTE expected = 0x1d;

    LSR(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR_zer) {
    memory[0xFFF1] = 0x01;
    BYTE expected = 0x00;

    LSR(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect zero flag");
}
END_TEST

START_TEST(test_ROL) {
    setFlag(&cpu, FLAG_C);
    memory[0xFFF1] = 0x2e;
    BYTE expected = 0x5D;

    ROL(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ROR) {
    setFlag(&cpu, FLAG_C);
    memory[0xFFF1] = 0x5D;
    BYTE expected = 0xAE;

    ROR(&cpu, &memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

Suite * Shift_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Shift Instructions");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_LSR);
    tcase_add_test(tc_core, test_ASL);
    tcase_add_test(tc_core, test_ROL);
    tcase_add_test(tc_core, test_ROR);
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    /*Limits test case*/
    tc_limits = tcase_create("Limits");
    tcase_add_test(tc_limits, test_ASL_neg);
    tcase_add_test(tc_limits, test_ASL_car);
    tcase_add_test(tc_limits, test_ASL_zer);
    tcase_add_test(tc_limits, test_LSR_neg);
    tcase_add_test(tc_limits, test_LSR_car);
    tcase_add_test(tc_limits, test_LSR_zer);
    tcase_add_checked_fixture(tc_limits, setup, teardown);
    suite_add_tcase(s, tc_limits);

    return s;
}
