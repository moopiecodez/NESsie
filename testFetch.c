#include <check.h>
#include <stdlib.h>
#include "cpu.h"

START_TEST(test_fetch) {
    BYTE memory[10] = {0xA4, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    BYTE expected_op = 0xA4;
    //LDX is A4
    //LDA is A1
    BYTE actual_op = fetch(&cpu, memory);
    ck_assert_msg(actual_op == expected_op, "operand not properly fetched");
}
END_TEST

START_TEST(test_incrementPC) {
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    BYTE expectedInitialPC = 0x0;
    BYTE expectedIncrementedPC = 0x1;
    ck_assert_msg(cpu->PC == expectedInitialPC, "initial PC not set to 0");
    incrementPC(cpu);
    ck_assert_msg(cpu->PC == expectedIncrementedPC, "PC not correctly incremented");
}
END_TEST


START_TEST(test_accumulator_addressing) {
    BYTE memory[10] = {0xA4, 0xA1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    BYTE *expectedPtr = &cpu->A;
    BYTE *actualPtr = A_addressing(cpu, memory);
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return accumulator address");
}
END_TEST

START_TEST(test_imm_addressing) {
    //set up memory to include AND instruction with immediate operand
    BYTE memory[10] = {0x29, 0x07, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    BYTE expectedOperand = memory[1];
    BYTE actualOperand = *(imm(cpu, memory));
    /*can't test pointer for immediate addressing as returns pointer to value of
      operand, not memory location
    */
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_absolute) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    memory[0x01] = 0x24;
    memory[0x02] = 0x1C;
    memory[0x1C24] = 0x77;
    BYTE expectedOperand = memory[0x1C24];
    BYTE *expectedPtr = &memory[0x1C24];
    BYTE *actualPtr = absolute(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_zp) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    memory[0x01] = 0x24;
    memory[0x02] = 0x1C;
    memory[ZERO_PAGE + memory[0x01]] = 0x77;
    BYTE expectedOperand = memory[0x0024];
    BYTE *expectedPtr = &memory[0x0024];
    BYTE *actualPtr = zp(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_zpX) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    cpu->X = 0x02;
    memory[0x01] = 0x24;
    memory[ZERO_PAGE + memory[0x01] + cpu->X] = 0x77;
    BYTE expectedOperand = memory[0x0026];
    BYTE *expectedPtr = &memory[0x0026];
    BYTE *actualPtr = zpX(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_zpY) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    cpu->Y = 0x02;
    memory[0x01] = 0x24;
    memory[ZERO_PAGE + memory[0x01] + cpu->Y] = 0x77;
    BYTE expectedOperand = memory[0x0026];
    BYTE *expectedPtr = &memory[0x0026];
    BYTE *actualPtr = zpY(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_absX) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    cpu->X = 0x02;
    memory[0x01] = 0x24;
    memory[0x02] = 0x1C;
    memory[0x1C24 + cpu->X] = 0x77;
    BYTE expectedOperand = memory[0x1C26];
    BYTE *expectedPtr = &memory[0x1C26];
    BYTE *actualPtr = absX(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_absY) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    cpu->Y = 0x02;
    memory[0x01] = 0x24;
    memory[0x02] = 0x1C;
    memory[0x1C24 + cpu->Y] = 0x77;
    BYTE expectedOperand = memory[0x1C26];
    BYTE *expectedPtr = &memory[0x1C26];
    BYTE *actualPtr = absY(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_imp) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    memory[0x01] = 0x24;
    BYTE *expectedPtr = NULL;
    BYTE *actualPtr = imp(cpu, memory);
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
}
END_TEST

START_TEST(test_rel_condition_true) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    //initial CPU where opcode stored
    cpu->PC = 0x1B21;
    //offset stored in memory location PC + 1
    memory[0x1B22] = 0x24;
    //PC incremented by 2 in instruction + offset
    uint16_t expectedPC = 0x1B47;
    BYTE *expectedPtr = NULL;
    BYTE *actualPtr = rel(cpu, memory);
    uint16_t actualPC = cpu->PC;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualPC == expectedPC, "Incorrect data, value is %x", actualPC);
}
END_TEST

START_TEST(test_indirectX) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    cpu->X = 0x02;
    memory[0x01] = 0x24;
    memory[0x0026] = 0x1C;
    memory[0x0027] = 0x2B;
    memory[0x2B1C] = 0x77;
    BYTE expectedOperand = memory[0x2B1C];
    BYTE *expectedPtr = &memory[0x2B1C];
    BYTE *actualPtr = indirectX(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST


/*
    (zp),Y - Indirect indexed addressing
    Second byte of instruction points to memory location in page zero. The
    contents of this location is added to Y register giving the low order eight
    bits of effective address.
    Carry from the addition is added to contents of the next zero page memory
    location. The result is the high order eight bits of effective address.
    need to do with carry
*/
START_TEST(test_indirectY) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    cpu->Y = 0x02;
    memory[0x01] = 0x24;
    memory[0x0024] = 0x1C;
    //0x1C + 0x02 = 0x1E
    memory[0x0025] = 0x2B;
    memory[0x2B1E] = 0x77;
    BYTE expectedOperand = memory[0x2B1E];
    BYTE *expectedPtr = &memory[0x2B1E];
    BYTE *actualPtr = indirectY(cpu, memory);
    BYTE actualOperand = *actualPtr;
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
}
END_TEST

START_TEST(test_indirectY_with_carry) {
    BYTE memory[0xFFFF];
    CPU test_cpu = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    CPU *cpu = &test_cpu;
    uint16_t expectedEffectiveAddress = 0x2C00;
    cpu->Y = 0x02;
    memory[0x01] = 0x24;
    //location pointed to by second byte
    memory[0x0024] = 0xFE;
    memory[0x0025] = 0x2B;
    memory[expectedEffectiveAddress] = 0x77;

    //sum of contents of location pointed to by second byte and Y register
    //0xFE + 0x02 = 0x00 + carry
    uint16_t sum = memory[0x0024] + cpu->Y;
    BYTE lowBits = (BYTE)sum;
    BYTE carry = (sum >> 8) & FLAG_MASK;
    BYTE highBits = memory[0x0025] + carry;
    uint16_t actualEffectiveAddress = (highBits << 8) + lowBits;
    BYTE expectedOperand = memory[actualEffectiveAddress];
    BYTE *expectedPtr = &memory[actualEffectiveAddress];
    BYTE *actualPtr = indirectY(cpu, memory);
    BYTE actualOperand = *actualPtr;

    ck_assert_msg(carry == 1, "Carry not correct: %x", carry);
    ck_assert_msg(actualEffectiveAddress == expectedEffectiveAddress, "Effective address incorrect: %x", actualEffectiveAddress);
    ck_assert_msg(actualOperand == expectedOperand, "Incorrect data, value is %x", actualOperand);
    ck_assert_msg(actualPtr == expectedPtr, "Addressing doesn't return correct address");
}
END_TEST

Suite *fetch_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Fetch");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_fetch);
    tcase_add_test(tc_core, test_incrementPC);
    tcase_add_test(tc_core, test_accumulator_addressing);
    tcase_add_test(tc_core, test_imm_addressing);
    tcase_add_test(tc_core, test_absolute);
    tcase_add_test(tc_core, test_zp);
    tcase_add_test(tc_core, test_zpX);
    tcase_add_test(tc_core, test_zpY);
    tcase_add_test(tc_core, test_absX);
    tcase_add_test(tc_core, test_absY);
    tcase_add_test(tc_core, test_imp);
    tcase_add_test(tc_core, test_rel_condition_true);
    tcase_add_test(tc_core, test_indirectX);
    tcase_add_test(tc_core, test_indirectY);
    tcase_add_test(tc_core, test_indirectY_with_carry);


    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
    SRunner *sr;
    Suite *s;
    int number_failed;

    s = fetch_suite();
    sr = srunner_create(s);
   
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
