#include <check.h>
#include <stdlib.h>
#include "cpu.h"

START_TEST(test_fetchOpcode) {
    BYTE memory[10] = {0xA4, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    BYTE expected_op = 0xA4;
    //LDX is A4
    //LDA is A1
    fetchOpcode(&cpu, memory);
    ck_assert_msg(cpu.IR == expected_op, "operand not properly fetched");
}
END_TEST

START_TEST(test_incrementPC) {
    CPU cpu = {0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    BYTE expected_op = 0x3;
    incrementPC(&cpu);
    ck_assert_msg(cpu.PC == expected_op, "PC incorrect");

}
END_TEST

START_TEST(test_read_throw) {
    BYTE memory[10] = {0xA4, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU cpu = {0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    BYTE expected_op = 0x3;
    readThrow(&cpu, memory, op_0A);
    ck_assert_msg(cpu.PC == expected_op, "PC incorrect");

}
END_TEST

START_TEST(test_addr_accumulator) {

}
END_TEST

START_TEST(test_addr_imm) {

}
END_TEST

START_TEST(test_addr_absolute) {

}
END_TEST

START_TEST(test_addr_zp) {

}
END_TEST

START_TEST(test_addr_zpX) {

}
END_TEST

START_TEST(test_addr_zpY) {

}
END_TEST

START_TEST(test_addr_absX) {

}
END_TEST

START_TEST(test_addr_absY) {

}
END_TEST

START_TEST(test_addr_imp) {

}
END_TEST

START_TEST(test_addr_rel_true) {

}
END_TEST
START_TEST(test_addr_indirectX) {

}
END_TEST

START_TEST(test_addr_indirectY) {

}
END_TEST

START_TEST(test_addr_indirectY_with_carry) {

}
END_TEST

Suite *cycle_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Cycle Functions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_fetchOpcode);
    tcase_add_test(tc_core, test_incrementPC);
    tcase_add_test(tc_core, test_read_throw);    
    tcase_add_test(tc_core, test_addr_accumulator);
    tcase_add_test(tc_core, test_addr_imm);
    tcase_add_test(tc_core, test_addr_absolute);
    tcase_add_test(tc_core, test_addr_zp);
    tcase_add_test(tc_core, test_addr_zpX);
    tcase_add_test(tc_core, test_addr_zpY);
    tcase_add_test(tc_core, test_addr_absX);
    tcase_add_test(tc_core, test_addr_absY);
    tcase_add_test(tc_core, test_addr_imp);
    tcase_add_test(tc_core, test_addr_rel_true);
    tcase_add_test(tc_core, test_addr_indirectX);
    tcase_add_test(tc_core, test_addr_indirectY);
    tcase_add_test(tc_core, test_addr_indirectY_with_carry);


    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
    SRunner *sr;
    Suite *s;
    int number_failed;

    s = cycle_suite();
    sr = srunner_create(s);
   
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
