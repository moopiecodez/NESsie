#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_AND) {
    memory[0xFFF1] = 0x60;
    cpu.A = 0x65;
    BYTE expected = 0x60;

    AND(&cpu, &memory[0xFFF1]);

    ck_assert_msg(cpu.A == expected, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "Z flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "N flag incorrect");
}
END_TEST

START_TEST(test_EOR) {
    memory[0xFFF2] = 0x65;
    cpu.A = 0x60;
    BYTE expected = 0x05;

    EOR(&cpu, &memory[0xFFF2]);
    ck_assert_msg(cpu.A == expected, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "Z flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "N flag incorrect");
}
END_TEST

START_TEST(test_ORA) {
    memory[0xFFF3] = 0x65;
    cpu.A = 0x60;
    BYTE expected = 0x65;

    ORA(&cpu, &memory[0xFFF3]);
    ck_assert_msg(cpu.A == expected, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "Z flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "N flag incorrect");
}
END_TEST

START_TEST(test_BIT) {
    memory[0xFFF3] = 0x82;
    cpu.A = 0x60;
    BYTE expected = 0x60;

    BIT(&cpu, &memory[0xFFF3]);

    ck_assert_msg(cpu.A == expected, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "Z flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "N flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "V flag incorrect");
}
END_TEST

Suite *Logical_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Logical Instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_AND);
    tcase_add_test(tc_core, test_EOR);
    tcase_add_test(tc_core, test_ORA);
    tcase_add_test(tc_core, test_BIT);
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
