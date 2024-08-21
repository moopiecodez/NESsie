#include <check.h>
#include <stdlib.h>
#include "cpu.h"

CPU cpu;
BYTE memory[0xFFFF] = {
    0xA4, 0x23, 0x08, 0xfe, 0x00, 0x02, 0x01, 0x00, 0x03, 0x44, 0x01, 0x05, 0x24, 0x18, 0xCC, 0x1E,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x11, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x12, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup(void) {
     cpu.PC = cpu.P = cpu.A = cpu.X = cpu.Y = cpu.S = cpu.IR = cpu.DB
     = cpu.AB = cpu.T = cpu.DL = cpu.ALU = cpu.ACR_FLAG = 0;
}
void teardown(void) {
    ;
}


START_TEST(fetchOpcode_cpu_trace) {
    BYTE expected = 0xA4;

    fetch_opcode(&cpu, memory, CLI);
    ck_assert_msg(cpu.IR == expected, "operand not properly fetched");
    ck_assert_msg(cpu.PC == 0x01, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == expected, "DB incorrect");
    ck_assert_msg(cpu.DL == expected, "DL incorrect");
}
END_TEST

START_TEST(fetch_throw_cpu_trace) {
    BYTE expected = 0xA4;
    fetch_throw(&cpu, memory, CLI);
    ck_assert_msg(cpu.PC == 0x00, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == expected, "DB incorrect");
    ck_assert_msg(cpu.DL == expected, "DL incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0, "Instruction execution incorrect");
}
END_TEST

START_TEST(fetch_throw_brk_cpu_trace) {
    BYTE expected = 0xA4;
    fetch_throw_brk(&cpu, memory, CLI);
    ck_assert_msg(cpu.PC == 0x01, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == expected, "DB incorrect");
    ck_assert_msg(cpu.DL == expected, "DL incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0, "Instruction execution incorrect");
}
END_TEST

START_TEST(imm_fetch_operand_branch_F_cpu_trace) {
    BYTE expected = 0xA4;
    imm_fetch_operand(&cpu, memory, CLI);
    ck_assert_msg(cpu.PC == 0x01, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == expected, "DB incorrect");
    ck_assert_msg(cpu.DL == expected, "DL incorrect");
    ck_assert_msg(cpu.T == 0x02, "T incorrect"); //testing incremented by 2
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
}
END_TEST

START_TEST(imm_fetch_operand_branch_T_cpu_trace) {
    BYTE expected = 0xA4;
    imm_fetch_operand(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x00, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == expected, "DB incorrect");
    ck_assert_msg(cpu.DL == expected, "DL incorrect");
    ck_assert_msg(cpu.T == 0x00, "T incorrect"); //testing T not incremented
    ck_assert_msg(cpu.ACR_FLAG == 0x01, "Expected ACR_FLAG to be set");
}
END_TEST

START_TEST(branch_PCL_cpu_trace_overflow_F) {
    BYTE expected_calc_value = 0x02;
    cpu.DL = expected_calc_value;
    cpu.ACR_FLAG = 1; //to reflect branch triggered in previous cycle
    branch_PCL(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x0002, "PC incorrect: %04x", cpu.PC);
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x00, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_calc_value, "DL incorrect");
    ck_assert_msg(cpu.ALU == expected_calc_value, "ALU value incorrect");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(cpu.T == 0x01, "T incorrect"); //testing T incremented
}
END_TEST


START_TEST(branch_PCL_cpu_trace_overflow_T) {
    cpu.PC = 0xFE;
    cpu.DL = 0x03;
    cpu.ACR_FLAG = 1; //to reflect branch triggered in previous cycle
    branch_PCL(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x0001, "PC incorrect: %04x", cpu.PC);
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x00, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x03, "DL incorrect");
    ck_assert_msg(cpu.T == 0x00, "T incorrect"); //testing T not incremented
    ck_assert_msg(cpu.ALU == 0x01, "ALU value incorrect");
    ck_assert_msg(cpu.ACR_FLAG == 0x01, "Expected ACR_FLAG to be set");
}
END_TEST

START_TEST(branch_fixPCH_positive_offset) {
    cpu.PC = 0x0001;
    cpu.DL = 0x03;
    cpu.ACR_FLAG = 1; //to reflect branch triggered in previous cycle
    branch_fixPCH(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x0101, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x00, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x03, "DL incorrect");
    ck_assert_msg(cpu.T == 0x00, "T incorrect"); //testing T not incremented
}
END_TEST

START_TEST(branch_fixPCH_negative_offset) {
    cpu.PC = 0x01FA;
    cpu.DL = 0xFC;
    cpu.ACR_FLAG = 1; //to reflect branch triggered in previous cycle
    branch_fixPCH(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x00FA, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x00, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x00, "DB incorrect");
    ck_assert_msg(cpu.DL == 0xFC, "DL incorrect");
    ck_assert_msg(cpu.T == 0x00, "T incorrect"); //testing T not incremented
}
END_TEST

START_TEST(stack_push_PCH_cpu_trace) {
    cpu.PC = 0x012C;
    cpu.S = 0xFF;
    stack_push_PCH(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x012C, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x01FF, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x01, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x01, "DL incorrect");
    ck_assert_msg(memory[cpu.AB] == 0x01, "memory contents incorrect");
    ck_assert_msg(cpu.S == 0xFE, "S incorrect");
}
END_TEST

START_TEST(stack_push_PCL_cpu_trace) {
    cpu.PC = 0x012C;
    cpu.S = 0xFE;
    stack_push_PCL(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x012C, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x01FE, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x2C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x2C, "DL incorrect");
    ck_assert_msg(memory[cpu.AB] == 0x2C, "memory contents incorrect");
    ck_assert_msg(cpu.S == 0xFD, "S incorrect");
}
END_TEST

START_TEST(stack_push_register_PHP_cpu_trace) {
    cpu.P = 0x3C;
    cpu.S = 0xFD;
    stack_push_register(&cpu, memory, PHP);
    ck_assert_msg(cpu.PC == 0x0000, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x01FD, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x3C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x3C, "DL incorrect");
    ck_assert_msg(memory[cpu.AB] == 0x3C, "memory contents incorrect");
    ck_assert_msg(cpu.S == 0xFC, "S incorrect");
}
END_TEST

START_TEST(stack_push_register_PHA_cpu_trace) {
    cpu.A = 0x3C;
    cpu.S = 0xFD;
    stack_push_register(&cpu, memory, PHA);
    ck_assert_msg(cpu.PC == 0x0000, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x01FD, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x3C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x3C, "DL incorrect");
    ck_assert_msg(memory[cpu.AB] == 0x3C, "memory contents incorrect");
    ck_assert_msg(cpu.S == 0xFC, "S incorrect");
}
END_TEST

START_TEST(stack_push_register_BRK_cpu_trace) {
    cpu.DB = 0x00;
    cpu.DL = 0x00;
    cpu.P = 0x2C; //value will change to reflect setting of B flag
    cpu.S = 0xFD;
    stack_push_register(&cpu, memory, BRK);
    ck_assert_msg(cpu.PC == 0x0000, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x01FD, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x3C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x3C, "DL incorrect");
    ck_assert_msg(memory[cpu.AB] == 0x3C, "memory contents incorrect");
    ck_assert_msg(cpu.S == 0xFC, "S incorrect");
}
END_TEST

START_TEST(increment_s_cpu_trace) {
    cpu.S = 0xFD;
    increment_s(&cpu, memory, PHP);
    ck_assert_msg(cpu.S == 0xFE, "S incorrect");
}
END_TEST

START_TEST(stack_pull_register_PLP_cpu_trace) {
    //doing
    cpu.S = 0xFD;
    memory[STACK_BASE + cpu.S] = 0x3C;
    cpu.P = 0x00;
    stack_pull_register(&cpu, memory, PLP);
    ck_assert_msg(cpu.PC == 0x0000, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x01FD, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x3C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x3C, "DL incorrect");
    ck_assert_msg(cpu.P == 0x3C, "P incorrect");
    ck_assert_msg(cpu.S == 0xFD, "S incorrect");
}
END_TEST

START_TEST(stack_pull_register_PLA_cpu_trace) {
    cpu.S = 0xFD;
    memory[STACK_BASE + cpu.S] = 0x3C;
    cpu.A = 0x00;
    stack_pull_register(&cpu, memory, PLA);
    ck_assert_msg(cpu.PC == 0x0000, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x01FD, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x3C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x3C, "DL incorrect");
    ck_assert_msg(cpu.A == 0x3C, "A incorrect");
    ck_assert_msg(cpu.S == 0xFD, "S incorrect");
}
END_TEST

START_TEST(fetch_PCL_from_interrupt_vector_cpu_trace) {
    memory[IRQ_LOW] = 0x1D;
    memory[IRQ_HIGH] = 0x2C;
    fetch_PCL(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x001D, "PC incorrect");
    ck_assert_msg(cpu.AB == IRQ_LOW, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x1D, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x1D, "DL incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0x01, "ExpectedI Flag to be set");
}
END_TEST

START_TEST(fetch_PCH_from_interrupt_vector_cpu_trace) {
    memory[IRQ_LOW] = 0x1D;
    memory[IRQ_HIGH] = 0x2C;
    fetch_PCH(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x2C00, "PC incorrect");
    ck_assert_msg(cpu.AB == IRQ_HIGH, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x2C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x2C, "DL incorrect");
    ck_assert_msg(getBit(cpu.P, FLAG_I) == 0x00, "ExpectedI Flag to be set");
}
END_TEST

START_TEST(fetch_ADL_cpu_trace) {
    cpu.PC = 0x02;
    memory[0x02] = 0x1D;
    fetch_ADL(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x0003, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x02, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x1D, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x1D, "DL incorrect");
}
END_TEST

START_TEST(fetch_ADH_cpu_trace) {
    cpu.PC = 0x03;
    cpu.DL = 0x1D;
    memory[0x03] = 0x2C;
    fetch_ADH(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x0004, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x03, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x2C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x2C, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x1D, "ALU incorrect");
}
END_TEST

START_TEST(fetch_address_cpu_trace) {
    cpu.PC = 0x03;
    cpu.DL = 0x1D;
    memory[0x03] = 0x2C;
    fetch_address(&cpu, memory, BCC);
    ck_assert_msg(cpu.PC == 0x0004, "PC incorrect");
    ck_assert_msg(cpu.AB == 0x03, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x2C, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x2C, "DL incorrect");
}
END_TEST

START_TEST(read_addr_exe_cpu_trace) {
    BYTE expected_val = 0x03;
    cpu.ALU = 0x1D;
    cpu.DL = 0x2C;
    memory[0x2C1D] = expected_val;
    read_addr_exe(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x2C1D, "AB incorrect");
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(cpu.A == expected_val, "Accumulator incorrect, expected to be loaded");
}
END_TEST

START_TEST(read_addr_cpu_trace) {
    BYTE expected_val = 0x03;
    cpu.ALU = 0x1D;
    cpu.DL = 0x2C;
    memory[0x2C1D] = expected_val;
    read_addr(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x2C1D, "AB incorrect");
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(cpu.A == 0x00, "Accumulator incorrect, no operation expected");
}
END_TEST

START_TEST(read_zp_addr_exe_cpu_trace) {
    BYTE expected_val = 0x03;
    cpu.DL = 0x2C;
    memory[0x002C] = expected_val;
    read_zp_addr_exe(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x002C, "AB incorrect");
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(cpu.A == expected_val, "Accumulator incorrect, expected to be loaded");
}
END_TEST

START_TEST(read_zp_addr_cpu_trace) {
    BYTE expected_val = 0x03;
    cpu.DL = 0x2C;
    memory[0x002C] = expected_val;
    read_zp_addr(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x002C, "AB incorrect");
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(cpu.A == 0x00, "Accumulator incorrect, no operation expected");
}
END_TEST

START_TEST(read_PCH_cpu_trace) {
    BYTE expected_val = 0x03;
    cpu.AB = 0x002B;
    cpu.DL = 0x2D;
    memory[0x002C] = expected_val;
    read_PCH(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x002C, "AB incorrect");
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(cpu.PC == 0x032D, "PC incorrect");
    ck_assert_msg(cpu.A == 0x00, "Accumulator incorrect, no operation expected");
}
END_TEST

//variants of ASL tested in instruction specific tests
START_TEST(modify_ASLinstruction_cpu_trace) {
    BYTE expected_val = 0x08;
    cpu.DL = 0x04;
    modify(&cpu, memory, ASL); //result goes to ALU not DB
    ck_assert_msg(cpu.DB == 0x00, "DB incorrect");
    ck_assert_msg(cpu.ALU == expected_val, "ALU incorrect");
    ck_assert_msg(cpu.DL == 0x04, "DL incorrect");
}
END_TEST

START_TEST(write_addr_cpu_trace) {
    BYTE expected_val = 0x08;
    cpu.ALU = expected_val;
    cpu.AB = 0x002C;
    write_addr(&cpu, memory, ASL);
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(memory[cpu.AB] == expected_val, "Memory contents incorrect");
}
END_TEST

START_TEST(write_register_cpu_trace) {
    BYTE expected_val = 0x08;
    cpu.A = expected_val; //register to be stored is accumulator
    cpu.AB = 0x002C;
    write_register(&cpu, memory, STA); //result goes to ALU not DB
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(memory[cpu.AB] == expected_val, "Memory contents incorrect");
}
END_TEST

START_TEST(read_addr_add_X_cpu_trace) {
    cpu.X = 0x03;
    cpu.DL = 0x2D;
    read_addr_add_X(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0030, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x03, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x03, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x0030, "ALU incorrect");
}
END_TEST

START_TEST(read_addr_add_Y_cpu_trace) {
    cpu.Y = 0x03;
    cpu.DL = 0x2D;
    read_addr_add_Y(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0030, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x03, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x03, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x0030, "ALU incorrect");
}
END_TEST

START_TEST(fetch_ADH_add_X_overflow_F) {
    cpu.X = 0x03;
    cpu.DL = 0x2D; //holds ADL
    cpu.PC = 0x0123;
    memory[0x0123] = 0x02;
    fetch_ADH_add_X(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0123, "AB incorrect");
    ck_assert_msg(cpu.PC == 0x0124, "PC incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x30, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
}
END_TEST

START_TEST(fetch_ADH_add_X_overflow_T) {
    cpu.X = 0x03;
    cpu.DL = 0xFD; //holds ADL
    cpu.PC = 0x0123;
    memory[0x0123] = 0x02;
    fetch_ADH_add_X(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0123, "AB incorrect");
    ck_assert_msg(cpu.PC == 0x0124, "PC incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x00, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x01, "Expected ACR_FLAG to be clear");
}
END_TEST

START_TEST(fetch_ADH_add_Y_overflow_F) {
    cpu.Y = 0x03;
    cpu.DL = 0x2D; //holds ADL
    cpu.PC = 0x0123;
    memory[0x0123] = 0x02;
    fetch_ADH_add_Y(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0123, "AB incorrect");
    ck_assert_msg(cpu.PC == 0x0124, "PC incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x30, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
}
END_TEST

START_TEST(fetch_ADH_add_Y_overflow_T) {
    cpu.Y = 0x03;
    cpu.DL = 0xFD; //holds ADL
    cpu.PC = 0x0123;
    memory[0x0123] = 0x02;
    fetch_ADH_add_Y(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0123, "AB incorrect");
    ck_assert_msg(cpu.PC == 0x0124, "PC incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x00, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x01, "Expected ACR_FLAG to be set");
}
END_TEST

START_TEST(read_addr_fixADH_exe_overflow_F) {
    cpu.ACR_FLAG = 0x00;
    cpu.ALU = 0x3C;
    cpu.DL = 0x1D;
    memory[0x1D3C] = 0x02;
    read_addr_fixADH_exe(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x1D3C, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x3C, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(cpu.T == 0x01, "T incorrect");
    ck_assert_msg(cpu.A == 0x02, "A incorrect");
}
END_TEST

START_TEST(read_addr_fixADH_exe_overflow_T) {
    cpu.ACR_FLAG = 0x01;
    cpu.ALU = 0x3C;
    cpu.DL = 0x1D;
    memory[0x1D3C] = 0x02;
    read_addr_fixADH_exe(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x1D3C, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x1E, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(cpu.T == 0x00, "T incorrect");
    ck_assert_msg(cpu.A == 0x00, "A incorrect");
}
END_TEST

START_TEST(read_addr_updated_exe_cpu_trace) {
    cpu.ALU = 0x1E;
    cpu.DL = 0x02;
    cpu.AB = 0x1D3C;
    memory[0x1E3C] = 0x05;
    read_addr_updated_exe(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x1E3C, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x05, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x05, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x00, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(cpu.T == 0x00, "T incorrect");
    ck_assert_msg(cpu.A == 0x05, "A incorrect");
}
END_TEST


START_TEST(read_addr_fixADH_overflow_F) {
    cpu.ACR_FLAG = 0x00;
    cpu.ALU = 0x3C;
    cpu.DL = 0x1D;
    memory[0x1D3C] = 0x02;
    read_addr_fixADH_exe(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x1D3C, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x3C, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(cpu.T == 0x01, "T incorrect");
}
END_TEST

START_TEST(read_addr_fixADH_overflow_T) {
    cpu.ACR_FLAG = 0x01;
    cpu.ALU = 0x3C;
    cpu.DL = 0x1D;
    memory[0x1D3C] = 0x02;
    read_addr_fixADH(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x1D3C, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x02, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x02, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x1E, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(cpu.T == 0x00, "T incorrect");
}
END_TEST

START_TEST(read_addr_updated_cpu_trace) {
    cpu.ALU = 0x1E;
    cpu.DL = 0x02;
    cpu.AB = 0x1D3C;
    memory[0x1E3C] = 0x05;
    read_addr_updated(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x1E3C, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x05, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x05, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x00, "ALU incorrect ");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(cpu.T == 0x00, "T incorrect");
}
END_TEST


START_TEST(write_register_fixedADH_overflow_T) {
    BYTE expected_val = 0x08;
    cpu.ACR_FLAG = 0x01;
    cpu.A = expected_val; //register to be stored is accumulator
    cpu.AB = 0x1E2C;
    write_register_fixedADH(&cpu, memory, STA); 
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(cpu.AB == 0x1F2C, "AB incorrect");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(memory[0x1F2C] == expected_val, "Memory contents incorrect");
}
END_TEST

START_TEST(write_register_fixedADH_overflow_F) {
    BYTE expected_val = 0x08;
    cpu.ACR_FLAG = 0;
    cpu.A = expected_val; //register to be stored is accumulator
    cpu.AB = 0x1E2C;
    write_register_fixedADH(&cpu, memory, STA); 
    ck_assert_msg(cpu.DB == expected_val, "DB incorrect");
    ck_assert_msg(cpu.DL == expected_val, "DL incorrect");
    ck_assert_msg(cpu.AB == 0x1E2C, "AB incorrect");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be clear");
    ck_assert_msg(memory[cpu.AB] == expected_val, "Memory contents incorrect");
}
END_TEST

START_TEST(read_ptr_add_X_cpu_trace) {
    cpu.X = 0x03;
    cpu.DL = 0x2D;
    memory[0x002D] = 0x05;
    read_ptr_add_X(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x002D, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x05, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x05, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x30, "ALU incorrect");
}
END_TEST

START_TEST(fetch_ptrX_ADL_cpu_trace) {
    cpu.ALU = 0x30;
    cpu.DL = 0x2D;
    memory[0x0030] = 0x05;
    fetch_ptrX_ADL(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0030, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x05, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x05, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x31, "ALU incorrect");
}
END_TEST

START_TEST(fetch_ptrX_ADH_cpu_trace) {
    cpu.ALU = 0x31;
    cpu.DL = 0x2D;
    memory[0x0031] = 0x05;
    fetch_ptrX_ADH(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x0031, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x05, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x05, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x05, "ALU incorrect");
}
END_TEST

START_TEST(fetch_ptr_ADL_cpu_trace) {
    cpu.DL = 0x2D;
    memory[0x002D] = 0x05;
    fetch_ptr_ADL(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x002D, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x05, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x05, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x2E, "ALU incorrect");
}
END_TEST

START_TEST(fetch_ptr_add_Y_overflow_T) {
    cpu.Y = 0x0A;
    cpu.ALU = 0xFA;
    cpu.DL = 0x2D;
    memory[0x00FA] = 0xF6;
    fetch_ptr_ADH_add_Y(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x00FA, "AB incorrect");
    ck_assert_msg(cpu.DB == 0xF6, "DB incorrect");
    ck_assert_msg(cpu.DL == 0xF6, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x00, "ALU incorrect");
    ck_assert_msg(cpu.ACR_FLAG == 0x01, "Expected ACR_FLAG to be set");

}
END_TEST

START_TEST(fetch_ptr_add_Y_overflow_F) {
    cpu.Y = 0x02;
    cpu.ALU = 0xFA;
    cpu.DL = 0x2D;
    memory[0x00FA] = 0x05;
    fetch_ptr_ADH_add_Y(&cpu, memory, LDA);
    ck_assert_msg(cpu.AB == 0x00FA, "AB incorrect");
    ck_assert_msg(cpu.DB == 0x05, "DB incorrect");
    ck_assert_msg(cpu.DL == 0x05, "DL incorrect");
    ck_assert_msg(cpu.ALU == 0x07, "ALU incorrect");
    ck_assert_msg(cpu.ACR_FLAG == 0x00, "Expected ACR_FLAG to be set");

}
END_TEST

Suite *cycle_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Cycle Functions");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, fetchOpcode_cpu_trace);
    tcase_add_test(tc_core, fetch_throw_cpu_trace);
    tcase_add_test(tc_core, fetch_throw_brk_cpu_trace);
    tcase_add_test(tc_core, imm_fetch_operand_branch_F_cpu_trace);
    tcase_add_test(tc_core, imm_fetch_operand_branch_T_cpu_trace);
    tcase_add_test(tc_core, branch_PCL_cpu_trace_overflow_F);
    tcase_add_test(tc_core, branch_PCL_cpu_trace_overflow_T);
    tcase_add_test(tc_core, branch_fixPCH_positive_offset);
    tcase_add_test(tc_core, branch_fixPCH_negative_offset);
    tcase_add_test(tc_core, stack_push_PCH_cpu_trace);
    tcase_add_test(tc_core, stack_push_PCL_cpu_trace);
    tcase_add_test(tc_core, stack_push_register_PHP_cpu_trace);
    tcase_add_test(tc_core, stack_push_register_PHA_cpu_trace);
    tcase_add_test(tc_core, stack_push_register_BRK_cpu_trace);
    tcase_add_test(tc_core, increment_s_cpu_trace);
    tcase_add_test(tc_core, stack_pull_register_PLP_cpu_trace);
    tcase_add_test(tc_core, stack_pull_register_PLA_cpu_trace);
    tcase_add_test(tc_core, fetch_PCL_from_interrupt_vector_cpu_trace);
    tcase_add_test(tc_core, fetch_PCH_from_interrupt_vector_cpu_trace);
    tcase_add_test(tc_core, fetch_ADL_cpu_trace);
    tcase_add_test(tc_core, fetch_ADH_cpu_trace);
    tcase_add_test(tc_core, fetch_address_cpu_trace);
    tcase_add_test(tc_core, read_addr_exe_cpu_trace);
    tcase_add_test(tc_core, read_addr_cpu_trace);
    tcase_add_test(tc_core, read_zp_addr_exe_cpu_trace);
    tcase_add_test(tc_core, read_zp_addr_cpu_trace);
    tcase_add_test(tc_core, read_PCH_cpu_trace);
    tcase_add_test(tc_core, modify_ASLinstruction_cpu_trace);
    tcase_add_test(tc_core, write_addr_cpu_trace);
    tcase_add_test(tc_core, write_register_cpu_trace);
    tcase_add_test(tc_core, read_addr_add_X_cpu_trace);
    tcase_add_test(tc_core, read_addr_add_Y_cpu_trace);
    tcase_add_test(tc_core, fetch_ADH_add_X_overflow_F);
    tcase_add_test(tc_core, fetch_ADH_add_X_overflow_T);
    tcase_add_test(tc_core, fetch_ADH_add_Y_overflow_F);
    tcase_add_test(tc_core, fetch_ADH_add_Y_overflow_T);
    tcase_add_test(tc_core, read_addr_fixADH_exe_overflow_F);
    tcase_add_test(tc_core, read_addr_fixADH_exe_overflow_T);
    tcase_add_test(tc_core, read_addr_updated_exe_cpu_trace);
    tcase_add_test(tc_core, read_addr_fixADH_overflow_F);
    tcase_add_test(tc_core, read_addr_fixADH_overflow_T);
    tcase_add_test(tc_core, read_addr_updated_cpu_trace);
    tcase_add_test(tc_core, write_register_fixedADH_overflow_T);
    tcase_add_test(tc_core, write_register_fixedADH_overflow_F);
    tcase_add_test(tc_core, read_ptr_add_X_cpu_trace);
    tcase_add_test(tc_core, fetch_ptrX_ADL_cpu_trace);
    tcase_add_test(tc_core, fetch_ptrX_ADH_cpu_trace);
    tcase_add_test(tc_core, fetch_ptr_ADL_cpu_trace);
    tcase_add_test(tc_core, fetch_ptr_add_Y_overflow_T);
    tcase_add_test(tc_core, fetch_ptr_add_Y_overflow_F);
    
    tcase_add_checked_fixture(tc_core, setup, teardown);

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
