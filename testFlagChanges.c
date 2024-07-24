#include <check.h>
#include <stdlib.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_CLC) {
    power_cpu();
    setFlag(FLAG_C);
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "initial C flag not set");

    CLC();
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CLD) {
    power_cpu();
    setFlag(FLAG_D);
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 1, "initial D flag not set");

    CLD();

    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CLI) {
    power_cpu();
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "initial I flag not set");

    CLI();
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CLV) {
    power_cpu();
    setFlag(FLAG_V);
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 1, "initial V flag not set");

    CLV();
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_SEC) {
    power_cpu();
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "initial C flag incorrectly set");

    SEC();

    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_SED) {
    power_cpu();
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "initial D flag incorrectly set");

    SED();

    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 1, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_SEI) {
    power_cpu();
    resetFlag(FLAG_I);
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0, "initial I flag incorrectly set");

    SEI();

    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

Suite *Flag_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Flag status change instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_CLC);
    tcase_add_test(tc_core, test_CLD);
    tcase_add_test(tc_core, test_CLI);
    tcase_add_test(tc_core, test_CLV);
    tcase_add_test(tc_core, test_SEC);
    tcase_add_test(tc_core, test_SED);
    tcase_add_test(tc_core, test_SEI);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = Flag_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
