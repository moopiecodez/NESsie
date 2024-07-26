#include <stdlib.h>
#include <check.h>
#include "testAll.h"

START_TEST(test_INC) {
    BYTE val = 0xFD;
    BYTE expectedVal = val + 0x01;
    memory[0xFFFE] = val;
    
    INC(&cpu, &memory[0xFFFE]);

    ck_assert_msg(memory[0xFFFE] == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_INX) {
    cpu.X = 0x02;
    BYTE expectedVal = cpu.X + 0x01;

    INX(&cpu);
    ck_assert_msg(cpu.X == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_INY) {
    cpu.Y = 0xC2;
    BYTE expectedVal = cpu.Y + 0x01;

    INY(&cpu);
    ck_assert_msg(cpu.Y == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEC) {
    BYTE val = 0xFD;
    BYTE expectedVal = val - 0x01;
    memory[0xFFFE] = val;
    
    DEC(&cpu, &memory[0xFFFE]);

    ck_assert_msg(memory[0xFFFE] == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEX) {
    cpu.X = 0x02;
    BYTE expectedVal = cpu.X - 0x01;
    
    DEX(&cpu);

    ck_assert_msg(cpu.X == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEY) {
    cpu.Y = 0x00;
    BYTE expectedVal = cpu.Y - 0x01;

    DEY(&cpu);

    ck_assert_msg(cpu.Y == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

Suite *INC_DEC_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Increment and Decrement Instructions");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_INC);
    tcase_add_test(tc_core, test_INX);
    tcase_add_test(tc_core, test_INY);
    tcase_add_test(tc_core, test_DEC);
    tcase_add_test(tc_core, test_DEX);
    tcase_add_test(tc_core, test_DEY);
    tcase_add_checked_fixture(tc_core, setup, teardown);
    suite_add_tcase(s, tc_core);

    return s;
}
