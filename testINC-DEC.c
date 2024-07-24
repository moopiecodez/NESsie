#include <stdlib.h>
#include <check.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_INC) {
    power_cpu();
    BYTE val = 0xFD;
    BYTE expectedVal = val + 0x01;
    memory[0xFFFE] = val;
    
    INC(&memory[0xFFFE]);

    ck_assert_msg(memory[0xFFFE] == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_INX) {
    power_cpu();
    cpu.X = 0x02;
    BYTE expectedVal = cpu.X + 0x01;

    INX();
    ck_assert_msg(cpu.X == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_INY) {
    power_cpu();
    cpu.Y = 0xC2;
    BYTE expectedVal = cpu.Y + 0x01;

    INY();
    ck_assert_msg(cpu.Y == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEC) {
    power_cpu();
    BYTE val = 0xFD;
    BYTE expectedVal = val - 0x01;
    memory[0xFFFE] = val;
    
    DEC(&memory[0xFFFE]);

    ck_assert_msg(memory[0xFFFE] == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEX) {
    power_cpu();
    cpu.X = 0x02;
    BYTE expectedVal = cpu.X - 0x01;
    
    DEX();

    ck_assert_msg(cpu.X == expectedVal, "incorrect value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_DEY) {
    power_cpu();
    cpu.Y = 0x00;
    BYTE expectedVal = cpu.Y - 0x01;

    DEY();

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

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = INC_DEC_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
