#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include "testAll.h"

START_TEST(test_BRK) {
    //already includes the 2 increment to PC
    cpu.PC = 0xFF00;
    //sets I flag to 0 to test BRK sets it correctly after push
    resetFlag(&cpu, FLAG_I);
    u_int16_t expectedPC = 0xAF03;
    memory[IRQ_HIGH] = 0xAF;
    memory[IRQ_LOW] = 0x03;
    
    BRK(&cpu, memory);
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
    BYTE high = 0x7C;
    BYTE low = 0x08;
    push_to_stack(&cpu, memory, high);
    push_to_stack(&cpu, memory, low);
    push_to_stack(&cpu, memory, cpu.P);
    uint16_t expectedPC = 0x7C08;

    setFlag(&cpu, FLAG_N);
    ck_assert_msg(getBit(cpu.P, FLAG_N) == 1, "N flag not set");

    RTI(&cpu, memory);
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
    tcase_add_checked_fixture(tc_core, setup, teardown);
    
    suite_add_tcase(s, tc_core);

    return s;
}
