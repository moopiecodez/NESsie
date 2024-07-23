#include <check.h>
#include <stdlib.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_ADC) {
    power_cpu();
    setFlag(FLAG_C);
    memory[0xFF0C] = 0xCE;
    cpu.A = 0x23;
    BYTE expected = 0xF2;

    ADC(&memory[0xFF0C]);

    ck_assert_msg(cpu.A == expected, "incorrect result");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_SBC){
power_cpu();
    setFlag(FLAG_C);
    memory[0xFF0C] = 0xCE;
    cpu.A = 0x23;
    BYTE expected = 0x55;

    SBC(&memory[0xFF0C]);

    ck_assert_msg(cpu.A == expected, "incorrect result");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP){
    power_cpu();
    cpu.A = 0xF6;
    memory[0xFFEC] = 0x18;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0xF6, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_eq){
    //compares equal values
    power_cpu();
    cpu.A = 0xF6;
    memory[0xFFEC] = 0xF6;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0xF6, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_zero_positive){
    //compares zero with positive number
    power_cpu();
    cpu.A = 0x00;
    memory[0xFFEC] = 0x18;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0x00, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_zero_negative){
    //compares zero with negative number
    power_cpu();
    cpu.A = 0x00;
    memory[0xFFEC] = 0xF6;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0x00, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_pos_neg){
    //compares positive with negative number
    power_cpu();
    cpu.A = 0x0C;
    memory[0xFFEC] = 0xF6;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0x0C, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_neg_neg){
    //compares negative with negative number
    power_cpu();
    cpu.A = 0x81;
    memory[0xFFEC] = 0xF6;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0x81, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_pos_pos){
    //compares positive with positive number
    power_cpu();
    cpu.A = 0x0C;
    memory[0xFFEC] = 0x2C;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0x0C, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_neg_pos){
    //compares negative with positive number
    power_cpu();
    cpu.A = 0xF6;
    memory[0xFFEC] = 0x0C;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0xF6, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_neg_smallneg){
    //compares negative with smaller negative number
    power_cpu();
    cpu.A = 0xF6;
    memory[0xFFEC] = 0x81;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0xF6, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_CMP_pos_smallpos){
    //compares positive with smaller positive number
    power_cpu();
    cpu.A = 0x2C;
    memory[0xFFEC] = 0x0C;

    CMP(&memory[0xFFEC]);

    ck_assert_msg(cpu.A == 0x2C, "incorrect Accumulator value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_CPX) {
    power_cpu();
    cpu.X = 0xF6;
    memory[0xFFEC] = 0x18;

    CPX(&memory[0xFFEC]);

    ck_assert_msg(cpu.X == 0xF6, "incorrect X reg value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_CPY) {
    power_cpu();
    cpu.Y = 0xF6;
    memory[0xFFEC] = 0x18;

    CPY(&memory[0xFFEC]);

    ck_assert_msg(cpu.Y == 0xF6, "incorrect Y reg value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

Suite *Arithmetic_suite(void) {
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Arithmetic instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_ADC);
    tcase_add_test(tc_core, test_SBC);
    tcase_add_test(tc_core, test_CMP);
    tcase_add_test(tc_core, test_CPX);
    tcase_add_test(tc_core, test_CPY);

    suite_add_tcase(s, tc_core);

    /*Limits test case*/
    tc_limits = tcase_create("Limits");
    tcase_add_test(tc_limits, test_CMP_eq);
    tcase_add_test(tc_limits, test_CMP_zero_positive);
    tcase_add_test(tc_limits, test_CMP_zero_negative);
    tcase_add_test(tc_limits, test_CMP_pos_neg);
    tcase_add_test(tc_limits, test_CMP_neg_neg);
    tcase_add_test(tc_limits, test_CMP_pos_pos);
    tcase_add_test(tc_limits, test_CMP_neg_pos);
    tcase_add_test(tc_limits, test_CMP_neg_smallneg);
    tcase_add_test(tc_limits, test_CMP_pos_smallpos);
    suite_add_tcase(s, tc_limits);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = Arithmetic_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
