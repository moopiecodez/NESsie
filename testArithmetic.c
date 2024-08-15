#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_ADC) {
    setFlag(&cpu, FLAG_C);
    cpu.DL = 0xCE;
    cpu.A = 0x23;
    BYTE expected = 0xF2;

    ADC(&cpu);
    getBit(cpu.P, FLAG_B);

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
    setFlag(&cpu, FLAG_C);
    cpu.DL = 0xCE;
    cpu.A = 0x23;
    BYTE expected = 0x55;

    SBC(&cpu);

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
    BYTE expected = 0xF6;
    cpu.A = expected;
    cpu.DL = 0x18;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0xF6;
    cpu.A = expected;
    cpu.DL = 0xF6;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0x00;
    cpu.A = expected;
    cpu.DL = 0x18;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0x00;
    cpu.A = expected;
    cpu.DL = 0xF6;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0x0C;
    cpu.A = expected;
    cpu.DL = 0xF6;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0x81;
    cpu.A = expected;
    cpu.DL = 0xF6;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0x0C;
    cpu.A = expected;
    cpu.DL = 0x2C;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0xF6;
    cpu.A = expected;
    cpu.DL = 0x0C;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0xF6;
    cpu.A = expected;
    cpu.DL = 0x81;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0x2C;
    cpu.A = expected;
    cpu.DL = 0x0C;

    CMP(&cpu);

    ck_assert_msg(cpu.A == expected, "incorrect Accumulator value");
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
    BYTE expected = 0xF6;
    cpu.X = expected;
    cpu.DL = 0x18;

    CPX(&cpu);

    ck_assert_msg(cpu.X == expected, "incorrect X reg value");
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
    BYTE expected = 0xF6;
    cpu.Y = expected;
    cpu.DL = 0x18;

    CPY(&cpu);

    ck_assert_msg(cpu.Y == expected, "incorrect Y reg value");
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
    tcase_add_checked_fixture(tc_core, setup, teardown);

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
    tcase_add_checked_fixture(tc_limits, setup, teardown);
    suite_add_tcase(s, tc_limits);

    return s;
}
