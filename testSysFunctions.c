#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"

BYTE memory[0xFFFF];

START_TEST(test_BRK) {
    power_cpu();
    //already includes the 2 increment to PC
    cpu.PC = 0xFF00;
    //sets I flag to 0 to test BRK sets it correctly after push
    resetFlag(FLAG_I);
    u_int16_t expectedPC = 0xAF03;
    memory[IRQ_HIGH] = 0xAF;
    memory[IRQ_LOW] = 0x03;
    
    BRK(memory);
    BYTE P_on_stack = memory[0x01FD];

    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC value");
    ck_assert_msg(getBit(P_on_stack, FLAG_B) != 0, "incorrect B flag on stack");
    ck_assert_msg(getBit(P_on_stack, FLAG_I) == 0, "incorrect I flag on stack");
    ck_assert_msg(memory[0x01FE] == 0x00, "incorrect low bit on stack");
    ck_assert_msg(memory[0x01FF] == 0xFF, "incorrect high bit on stack");
    ck_assert_msg(cpu.S == 0xFC, "incorrect Stack Pointer value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) != 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag"); 
}
END_TEST

START_TEST(test_NOP) {
    power_cpu();

    u_int16_t expectedPC = 0xFFFCu;

    NOP();
    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC value");
    ck_assert_msg(cpu.S == 0xFF, "incorrect Stack Pointer value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");   

}
END_TEST

START_TEST(test_RTI) {
    power_cpu();
    BYTE high = 0x7C;
    BYTE low = 0x08;
    push_to_stack(memory, high);
    push_to_stack(memory, low);
    push_to_stack(memory, cpu.P);
    uint16_t expectedPC = 0x7C08;

    setFlag(FLAG_N);
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "N flag not set");

    RTI(memory);
    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC value");
    ck_assert_msg(cpu.S == 0xFF, "incorrect Stack Pointer value");
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 0, "incorrect N flag");
    ck_assert_msg(getBit(cpu.P, FLAG_V) == 0, "incorrect V flag");
    ck_assert_msg(getBit(cpu.P, FLAG_B) == 0, "incorrect B flag");
    ck_assert_msg(getBit(cpu.P, FLAG_D) == 0, "incorrect D flag");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 1, "incorrect I flag");
    ck_assert_msg(getBit(cpu.P, FLAG_Z) == 0, "incorrect Z flag");
    ck_assert_msg(getBit(cpu.P, FLAG_C) == 0, "incorrect C flag");    
}
END_TEST

Suite *System_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("System function instructions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_BRK);
    tcase_add_test(tc_core, test_NOP);
    tcase_add_test(tc_core, test_RTI);
    
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s;
    SRunner *sr;
    int number_failed;

    s = System_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
