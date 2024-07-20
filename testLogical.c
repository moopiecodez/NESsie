#include <check.h>
#include <stdlib.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_AND) {
    power_cpu();
    memory[0xFFF1] = 0x60;
    cpu.A = 0x65;
    BYTE expected = 0x60;

    AND(&memory[0xFFF1]);

    ck_assert_msg(cpu.A == expected, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "Z flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "N flag incorrect");
}
END_TEST

START_TEST(test_EOR) {
    power_cpu();
    memory[0xFFF2] = 0x65;
    cpu.A = 0x60;
    BYTE expected = 0x05;

    EOR(&memory[0xFFF2]);
    ck_assert_msg(cpu.A == expected, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "Z flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "N flag incorrect");
}
END_TEST

START_TEST(test_ORA) {
    power_cpu();
    memory[0xFFF3] = 0x65;
    cpu.A = 0x60;
    BYTE expected = 0x65;

    ORA(&memory[0xFFF3]);
    ck_assert_msg(cpu.A == expected, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "Z flag incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "N flag incorrect");
}
END_TEST

START_TEST(test_BIT) {
    power_cpu();
    memory[0xFFF3] = 0x82;
    cpu.A = 0x60;
    BYTE expected = 0x60;

    BIT(&memory[0xFFF3]);

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

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = Logical_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
