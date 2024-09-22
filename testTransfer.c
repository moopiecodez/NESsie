#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_TAX) {
    cpu.A = 0xCE;

    TAX(&cpu);

    ck_assert_msg(cpu.X == cpu.A, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "negative flag incorrect");
}
END_TEST

START_TEST(test_TAY) {
    cpu.A = 0x7E;

    TAY(&cpu);

    ck_assert_msg(cpu.Y == cpu.A, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "negative flag incorrect");
}
END_TEST

START_TEST(test_TSX) {
    cpu.S = 0xED;

    TSX(&cpu);

    ck_assert_msg(cpu.X == cpu.S, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "negative flag incorrect");
}
END_TEST

START_TEST(test_TXA) {
    cpu.X = 0x00;

    TXA(&cpu);

    ck_assert_msg(cpu.A == cpu.X, "value not transferred");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "zero flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "negative flag incorrect");
}
END_TEST

START_TEST(test_TXS) {
    cpu.X = 0xCC;

    TXS(&cpu);

    ck_assert_msg(cpu.S == cpu.X, "value not transferred");
}
END_TEST

START_TEST(test_TYA) {
    cpu.Y = 0x2E;

    TYA(&cpu);

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
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
