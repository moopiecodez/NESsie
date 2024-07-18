#include <check.h>
#include <stdlib.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_STA) {
    power_cpu();
    memory[0xFFFC] = 0x00;
    cpu.A = 0xC9;
    STA(&memory[0xFFFC]);
    ck_assert_msg(memory[0xFFFC] == cpu.A, "incorrect value stored");
    ck_assert_msg(memory[0xFFFC] != 0x00, "value not stored");
}
END_TEST

START_TEST(test_STX) {
    power_cpu();
    memory[0xFFFB] = 0x00;
    cpu.X = 0xC8;
    STX(&memory[0xFFFB]);;
    ck_assert_msg(memory[0xFFFB] == cpu.X, "incorrect value stored");
    ck_assert_msg(memory[0xFFFB] != 0x00, "value not stored");
}
END_TEST

START_TEST(test_STY) {
    power_cpu();
    memory[0xFFFA] = 0x00;
    cpu.Y = 0xC7;
    STY(&memory[0xFFFA]);;
    ck_assert_msg(memory[0xFFFA] == cpu.Y, "incorrect value stored");
    ck_assert_msg(memory[0xFFFA] != 0x00, "value not stored");
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

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = Store_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;

}