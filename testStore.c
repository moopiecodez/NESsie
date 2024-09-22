#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_STA) {
    cpu.A = 0xC9;
    STA(&cpu);
    ck_assert_msg(cpu.DB == cpu.A, "incorrect value on DB for storage");
}
END_TEST

START_TEST(test_STX) {
    cpu.X = 0xC8;
    STX(&cpu);
    ck_assert_msg(cpu.DB == cpu.X, "incorrect value on DB for storage");
}
END_TEST

START_TEST(test_STY) {
    cpu.Y = 0xC7;
    STY(&cpu);
    ck_assert_msg(cpu.DB == cpu.Y, "incorrect value stored");
}
END_TEST

Suite *Store_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Store Instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_STA);
    tcase_add_test(tc_core, test_STX);
    tcase_add_test(tc_core, test_STY);
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
