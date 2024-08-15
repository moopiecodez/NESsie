#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(test_BCC) {
    setFlag(&cpu, FLAG_C);
    BCC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag, expected set");
    resetFlag(&cpu, FLAG_C);
    BCC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag, expected clear");


}
END_TEST

START_TEST(test_BCS) {
    setFlag(&cpu, FLAG_C);
    BCS(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag, expected set");
    resetFlag(&cpu, FLAG_C);
    BCS(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag, expected clear");
}
END_TEST

START_TEST(test_BEQ) {
    setFlag(&cpu, FLAG_Z);
    BEQ(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag, expected set");
    resetFlag(&cpu, FLAG_Z);
    BEQ(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag, expected clear");
}
END_TEST

START_TEST(test_BMI) {
    setFlag(&cpu, FLAG_N);
    BMI(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag, expected set");
    resetFlag(&cpu, FLAG_N);
    BMI(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag, expected clear");
}
END_TEST

START_TEST(test_BNE) {
    setFlag(&cpu, FLAG_Z);
    BNE(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag, expected set");
    resetFlag(&cpu, FLAG_Z);
    BNE(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag, expected clear");
}
END_TEST

START_TEST(test_BPL) {
    setFlag(&cpu, FLAG_N);
    BPL(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag, expected set");
    resetFlag(&cpu, FLAG_N);
    BPL(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag, expected clear");
}
END_TEST

START_TEST(test_BVC) {
    setFlag(&cpu, FLAG_V);
    BVC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 1, "incorrect V flag, expected set");
    resetFlag(&cpu, FLAG_V);
    BVC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag, expected clear");
}
END_TEST

START_TEST(test_BVS) {
    setFlag(&cpu, FLAG_V);
    BVS(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 1, "incorrect V flag, expected set");
    resetFlag(&cpu, FLAG_V);
    BVS(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag, expected clear");
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
    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
