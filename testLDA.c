#include <stdio.h>
#include <check.h>
#include "testAll.h"

START_TEST(test_LDA) {
    BYTE expectedA = 0x2C;
    memory[0xFFF1] = expectedA;

    LDA(&cpu, &memory[0xFFF1]);

    ck_assert_msg(cpu.A == expectedA, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");

    BYTE expectedFlagset_N = 1;
    BYTE expectedFlagclear_N = 0;
    BYTE expectedFlagset_Z = 1;
    BYTE expectedFlagclear_Z = 0;
}
END_TEST

START_TEST(test_LDX) {
    BYTE expectedX = 0x00; //zero
    memory[0xFFF2] = expectedX;

    LDX(&cpu, &memory[0xFFF2]);

    ck_assert_msg(cpu.X == expectedX, "incorrect X register value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) != 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_LDY) {
    BYTE expectedY = 0x85; //negative
    memory[0xFFF3] = expectedY;

    LDY(&cpu, &memory[0xFFF3]);
    ck_assert_msg(cpu.Y == expectedY, "incorrect Y register value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) != 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

Suite *Load_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Load Instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_LDA);
    tcase_add_test(tc_core, test_LDX);
    tcase_add_test(tc_core, test_LDY);
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
