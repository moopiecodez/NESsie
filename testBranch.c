#include <check.h>
#include <stdlib.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_BCC) {
    power_cpu();
    cpu.PC = 0xFF00;
    BYTE displacement = 0x03;
    BCC(displacement);

    ck_assert_msg(cpu.PC == 0xFF03, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");
}
END_TEST

START_TEST(test_BCS) {
    power_cpu();
    cpu.PC = 0xFF00;
    BYTE displacement = 0x03;
    setFlag(FLAG_C);

    BCS(displacement);
    ck_assert_msg(cpu.PC == 0xFF03, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag");
}
END_TEST

START_TEST(test_BEQ) {
    power_cpu();
    cpu.PC = 0xFF00;
    BYTE displacement = 0x04;
    setFlag(FLAG_Z);

    BEQ(displacement);
    ck_assert_msg(cpu.PC == 0xFF04, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag");
}
END_TEST

START_TEST(test_BMI) {
    power_cpu();
    cpu.PC = 0xFF00;
    BYTE displacement = 0x05;
    setFlag(FLAG_N);

    BMI(displacement);
    ck_assert_msg(cpu.PC == 0xFF05, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag");
}
END_TEST

START_TEST(test_BNE) {
    power_cpu();
    cpu.PC = 0xFF00;
    BYTE displacement = 0x03;

    BNE(displacement);
    ck_assert_msg(cpu.PC == 0xFF03, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
}
END_TEST

START_TEST(test_BPL) {
    power_cpu();
    cpu.PC = 0xFF00;
    BYTE displacement = 0x06;
    BPL(displacement);
    ck_assert_msg(cpu.PC == 0xFF06, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
}
END_TEST

START_TEST(test_BVC) {
    power_cpu();
    cpu.PC = 0xFF00;
    BYTE displacement = 0x04;

    BVC(displacement);
    ck_assert_msg(cpu.PC == 0xFF04, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
}
END_TEST

START_TEST(test_BVS) {
    power_cpu();
    setFlag(FLAG_V);
    cpu.PC = 0xFF00;
    BYTE displacement = 0x01;

    BVS(displacement);
    ck_assert_msg(cpu.PC == 0xFF01, "incorrect PC value");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 1, "incorrect N flag");
}
END_TEST

Suite *Branch_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Branch instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_BCC);
    tcase_add_test(tc_core, test_BCS);
    tcase_add_test(tc_core, test_BEQ);
    tcase_add_test(tc_core, test_BMI);
    tcase_add_test(tc_core, test_BNE);
    tcase_add_test(tc_core, test_BPL);
    tcase_add_test(tc_core, test_BVC);
    tcase_add_test(tc_core, test_BVS);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = Branch_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
