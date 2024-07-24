#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_ASL) {
    power_cpu();
    memory[0xFFF1] = 0x2e;
    BYTE expected = 0x5c;

    ASL(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ASL_neg) {
    power_cpu();
    memory[0xFFF1] = 0xEC;
    BYTE expected = 0xD8;

    ASL(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ASL_car) {
    power_cpu();
    memory[0xFFF1] = 0x95;
    BYTE expected = 0x2A;

    ASL(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ASL_zer) {
    power_cpu();
    memory[0xFFF1] = 0x80;
    BYTE expected = 0x00;

    ASL(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR) {
    power_cpu();
    memory[0xFFF1] = 0x5c;
    BYTE expected = 0x2e;

    LSR(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR_neg) {
    power_cpu();
    memory[0xFFF1] = 0xec;
    BYTE expected = 0x76;

    LSR(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR_car) {
    power_cpu();
    memory[0xFFF1] = 0x3b;
    BYTE expected = 0x1d;

    LSR(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_LSR_zer) {
    power_cpu();
    memory[0xFFF1] = 0x01;
    BYTE expected = 0x00;

    LSR(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect zero flag");
}
END_TEST

START_TEST(test_ROL) {
    power_cpu();
    setFlag(FLAG_C);
    memory[0xFFF1] = 0x2e;
    BYTE expected = 0x5D;

    ROL(&memory[0xFFF1]);
    ck_assert_msg(memory[0xFFF1] == expected, "resulting byte is wrong");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect negative flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect carry flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect zero flag");
}
END_TEST

START_TEST(test_ROR) {
    power_cpu();
    setFlag(FLAG_C);
    memory[0xFFF1] = 0x5D;
    BYTE expected = 0xAE;

    ROR(&memory[0xFFF1]);
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

    suite_add_tcase(s, tc_core);

    /*Limits test case*/
    tc_limits = tcase_create("Limits");
    tcase_add_test(tc_limits, test_ASL_neg);
    tcase_add_test(tc_limits, test_ASL_car);
    tcase_add_test(tc_limits, test_ASL_zer);
    tcase_add_test(tc_limits, test_LSR_neg);
    tcase_add_test(tc_limits, test_LSR_car);
    tcase_add_test(tc_limits, test_LSR_zer);
    suite_add_tcase(s, tc_limits);

    return s;
}

int main(void) {
    printf("I'm saying something\n");
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = Shift_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE ;
    
    return 0;
}