#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_PHA) {
    power_cpu();
    cpu.A = 0xCC;

    PHA(memory);
    ck_assert_msg(memory[cpu.S + STACK_BASE + 1] == 0xCC,
    "incorrect stack value");
}
END_TEST

START_TEST(test_PHP) {
    power_cpu();
    setFlag(FLAG_N);
    setFlag(FLAG_Z);
    BYTE expected = 0x86;

    PHP(memory);
    ck_assert_msg(memory[cpu.S + STACK_BASE + 1] == expected,
    "incorrect stack value");
}
END_TEST

START_TEST(test_PLA) {
    power_cpu();
    BYTE expected = 0xCC;
    memory[0x01FF] = expected;
    cpu.S--;
    cpu.A = 0x02;

    PLA(memory);
    ck_assert_msg(cpu.A == expected, "incorrect stack value is %d", cpu.A);
}
END_TEST

START_TEST(test_PLP) {
    power_cpu();
    BYTE expected = 0x86;
    memory[0x01FF] = expected;
    cpu.S--;

    PLP(memory);
    ck_assert_msg(cpu.P == expected, "incorrect stack value is %d", cpu.A);
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

    suite_add_tcase(s, tc_core);

    return s;
}
