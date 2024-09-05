#include <check.h>
#include <stdlib.h>
#include "testAll.h"

START_TEST(flag_C_T_BCC_branch_F) {
    setFlag(&cpu, FLAG_C);
    BCC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag, expected set");
}
END_TEST

START_TEST(flag_C_F_BCC_branch_T) {
    clearFlag(&cpu, FLAG_C);
    BCC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag, expected clear");
}
END_TEST

START_TEST(flag_C_T_BCS_branch_T) {
    setFlag(&cpu, FLAG_C);
    cpu.ACR_FLAG = 0;
    BCS(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 1, "incorrect C flag, expected set");
}
END_TEST

START_TEST(flag_C_F_BCS_branch_F) {
    clearFlag(&cpu, FLAG_C);
    cpu.ACR_FLAG = 0;
    BCS(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag, expected clear");
}
END_TEST

START_TEST(flag_Z_T_BEQ_branch_T) {
    setFlag(&cpu, FLAG_Z);
    cpu.ACR_FLAG = 0;
    BEQ(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag, expected set");
}
END_TEST

START_TEST(flag_Z_F_BEQ_branch_F) {
    clearFlag(&cpu, FLAG_Z);
    cpu.ACR_FLAG = 0;
    BEQ(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag, expected clear");
}
END_TEST
START_TEST(flag_N_T_BMI_branch_T) {
    setFlag(&cpu, FLAG_N);
    cpu.ACR_FLAG = 0;
    BMI(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag, expected set");
}
END_TEST

START_TEST(flag_N_F_BMI_branch_F) {
    clearFlag(&cpu, FLAG_N);
    cpu.ACR_FLAG = 0;
    BMI(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag, expected clear");
}
END_TEST

START_TEST(flag_Z_T_BNE_branch_F) {
    setFlag(&cpu, FLAG_Z);
    cpu.ACR_FLAG = 0;
    BNE(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 1, "incorrect Z flag, expected set");
}
END_TEST

START_TEST(flag_Z_F_BNE_branch_T) {
    cpu.ACR_FLAG = 0;
    BNE(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag, expected clear");
}
END_TEST

START_TEST(flag_N_T_BPL_branch_F) {
    setFlag(&cpu, FLAG_N);
    cpu.ACR_FLAG = 0;
    BPL(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 0, "incorrect FLAG, expected no branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "incorrect N flag, expected set");
}
END_TEST

START_TEST(flag_N_F_BPL_branch_T) {
    clearFlag(&cpu, FLAG_N);
    cpu.ACR_FLAG = 0;
    BPL(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag, expected clear");
}
END_TEST


START_TEST(flag_V_T_BVC_branch_F) {
    clearFlag(&cpu, FLAG_V);
    cpu.ACR_FLAG = 0;
    BVC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag, expected clear");
}
END_TEST

START_TEST(flag_V_F_BVC_branch_T) {
    clearFlag(&cpu, FLAG_V);
    cpu.ACR_FLAG = 0;
    BVC(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag, expected clear");
}
END_TEST

START_TEST(flag_V_T_BVS_branch_T) {
    setFlag(&cpu, FLAG_V);
    cpu.ACR_FLAG = 0;
    BVS(&cpu);
    ck_assert_msg(cpu.ACR_FLAG == 1, "incorrect FLAG, expected branch");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 1, "incorrect V flag, expected set");
}
END_TEST

START_TEST(flag_V_F_BVS_branch_F) {
    clearFlag(&cpu, FLAG_V);
    cpu.ACR_FLAG = 0;
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
    tcase_add_test(tc_core, flag_C_T_BCC_branch_F);
    tcase_add_test(tc_core, flag_C_F_BCC_branch_T);
    tcase_add_test(tc_core, flag_C_T_BCS_branch_T);
    tcase_add_test(tc_core, flag_C_F_BCS_branch_F);
    tcase_add_test(tc_core, flag_Z_T_BEQ_branch_T);
    tcase_add_test(tc_core, flag_Z_F_BEQ_branch_F);
    tcase_add_test(tc_core, flag_N_T_BMI_branch_T);
    tcase_add_test(tc_core, flag_N_F_BMI_branch_F);
    tcase_add_test(tc_core, flag_Z_T_BNE_branch_F);
    tcase_add_test(tc_core, flag_Z_F_BNE_branch_T);
    tcase_add_test(tc_core, flag_N_T_BPL_branch_F);
    tcase_add_test(tc_core, flag_N_F_BPL_branch_T);
    tcase_add_test(tc_core, flag_V_F_BVC_branch_T);
    tcase_add_test(tc_core, flag_V_T_BVC_branch_F);
    tcase_add_test(tc_core, flag_V_T_BVS_branch_T);
    tcase_add_test(tc_core, flag_V_F_BVS_branch_F);

    tcase_add_checked_fixture(tc_core, setup, teardown);

    suite_add_tcase(s, tc_core);

    return s;
}
