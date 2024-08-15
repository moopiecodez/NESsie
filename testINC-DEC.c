#include <stdlib.h>
#include <check.h>
#include "testAll.h"

START_TEST(test_INC) {
    BYTE val = 0xFD;
    BYTE expectedVal = val + 0x01;
    cpu.DB = val;
    
    INC(&cpu);

    ck_assert_msg(cpu.ALU == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_INX) {
    BYTE val = 0x02;
    cpu.X = 0x03;
    BYTE expectedVal = cpu.X + val;
    cpu.DB = val;

    INX(&cpu);
    ck_assert_msg(cpu.ALU == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_INY) {
    BYTE val = 0x01;
    cpu.Y = 0xC2;
    BYTE expectedVal = cpu.Y + val;
    cpu.DB = val;

    INY(&cpu);
    ck_assert_msg(cpu.ALU == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEC) {
    BYTE val = 0xFD;
    BYTE expectedVal = val - 0x01;
    cpu.DB = val;
    
    DEC(&cpu);

    ck_assert_msg(cpu.ALU == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEX) {
    BYTE val = 0x01;
    cpu.X = 0x02;
    BYTE expectedVal = cpu.X - val;
    
    DEX(&cpu);

    ck_assert_msg(cpu.ALU == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEY) {
    BYTE val = 0x01;
    cpu.Y = 0x00;
    BYTE expectedVal = cpu.Y - val;

    DEY(&cpu);

    ck_assert_msg(cpu.ALU == expectedVal, "incorrect value");
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
