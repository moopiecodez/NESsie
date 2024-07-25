#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_TAX) {
    power_cpu();
    cpu.A = 0xCE;

    TAX();

    ck_assert_msg(cpu.X == cpu.A, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "negative flag incorrect");
}
END_TEST

START_TEST(test_TAY) {
    power_cpu();
    cpu.A = 0x7E;

    TAY();

    ck_assert_msg(cpu.Y == cpu.A, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "negative flag incorrect");
}
END_TEST

START_TEST(test_TSX) {
    power_cpu();
    cpu.S = 0xED;

    TSX();

    ck_assert_msg(cpu.X == cpu.S, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "negative flag incorrect");
}
END_TEST

START_TEST(test_TXA) {
    power_cpu();
    cpu.X = 0x00;

    TXA();

    ck_assert_msg(cpu.A == cpu.X, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "negative flag incorrect");
}
END_TEST

START_TEST(test_TXS) {
    power_cpu();
    cpu.X = 0xCC;

    TXS();

    ck_assert_msg(cpu.S == cpu.X, "value not transferred");
}
END_TEST

START_TEST(test_TYA) {
    power_cpu();
    cpu.Y = 0x2E;

    TYA();

    ck_assert_msg(cpu.A == cpu.Y, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "negative flag incorrect");
}
END_TEST

Suite *Transfer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Transfer Instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_TAX);
    tcase_add_test(tc_core, test_TAY);
    tcase_add_test(tc_core, test_TSX);
    tcase_add_test(tc_core, test_TXA);
    tcase_add_test(tc_core, test_TXS);
    tcase_add_test(tc_core, test_TYA);

    suite_add_tcase(s, tc_core);

    return s;
}
