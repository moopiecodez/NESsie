#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_PHA) {
    BYTE expected = 0xCC;
    cpu.A = expected;
    PHA(&cpu);
    ck_assert_msg(cpu.DB == expected, "incorrect value to be pushed to stack");
}
END_TEST

START_TEST(test_PHP) {
    setFlag(&cpu, FLAG_N);
    setFlag(&cpu, FLAG_Z);
    setFlag(&cpu, FLAG_I);
    BYTE expected = 0x86;
    PHP(&cpu);
    ck_assert_msg(cpu.DB == expected, "incorrect value to be pushed to stack");
}
END_TEST

START_TEST(test_PLA) {
    BYTE expected = 0xCC;
    cpu.P = 0x00; // reset status register to test PLA function effect on flags
    cpu.DL = expected;
    cpu.A = 0x02;
    PLA(&cpu);
    ck_assert_msg(cpu.A == expected, "incorrect accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_PLP) {
    BYTE expected = 0x86;
    cpu.DL = expected;
    cpu.P = 0x01;
    PLP(&cpu);
    ck_assert_msg(cpu.P == expected, "incorrect status register value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

Suite *Stack_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Stack instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_PHA);
    tcase_add_test(tc_core, test_PHP);
    tcase_add_test(tc_core, test_PLA);
    tcase_add_test(tc_core, test_PLP);
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
