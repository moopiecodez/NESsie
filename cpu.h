#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "bus.h"

#define BYTE uint8_t

//constant for flag bitwise operations
#define FLAG_MASK ~(~0 << 1)
#define LEFT_BIT 7
#define RIGHT_BIT 0

//IRQ interrupt vector locations
#define IRQ_HIGH 0xFFFFu
#define IRQ_LOW 0xFFFEu

//constant for base stack location in memory
#define STACK_BASE 0x0100u

//constant for zero page location
#define ZERO_PAGE 0x0000


//constants for Processor Status register flags position, 5 not used
#define FLAG_N 7    /* N (negative flag) */
#define FLAG_V 6    /* V (overflow flag) */
#define FLAG_B 4    /* B (break command flag) */
#define FLAG_D 3    /* D (decimal mode flag) - not used in NES CPU */
#define FLAG_I 2    /* I (interrupt disable flag) */
#define FLAG_Z 1    /* Z (zero flag) */
#define FLAG_C 0    /* C (carry flag)*/

typedef struct cpu_registers {
    uint16_t PC;    /* Program Counter */
    BYTE P;         /* Processor status register */
    BYTE A;         /* Accumulator register */  
    BYTE X;         /* Index Register X */
    BYTE Y;         /* Index Register Y */
    BYTE S;         /* Stack Pointer/offset from $0100, initialised at $FF*/
    BYTE IR;        /* Instruction Register, holds opcode initialised to 0*/
    BYTE DB;        /* Data Bus*/
    uint16_t AB;    /* Address Bus*/
    BYTE T;         /* Instruction step number*/
    BYTE DL;        /*Input Data Latch duplicated in Predecode register loaded with each read bus cycle*/
    BYTE ALU;       /*Arithmetic Logic Unit*/
    BYTE ACR_FLAG;  /*ALU carry register used as a flag if ALU operation resulted in a carry*/
} CPU;

// typedef void Instruction(CPU *);
// //Read instructions:
// Instruction LDA;
// Instruction LDX;
// Instruction LDY;
// Instruction EOR;
// Instruction AND;
// Instruction ORA;
// Instruction ADC;
// Instruction SBC;
// Instruction CMP;
// Instruction BIT;
// Instruction NOP;


// //RMW instructions:
// Instruction ASL;
// Instruction LSR;
// Instruction ROL;
// Instruction ROR;
// Instruction INC;
// Instruction INX;
// Instruction INY;
// Instruction DEC;
// Instruction DEX;
// Instruction DEY;

// void increment(CPU *cpu, BYTE memory);
// void decrement(CPU *cpu, BYTE *memory);

// //transfer instructions
// Instruction TAX;
// Instruction TAY;
// Instruction TSX;
// Instruction TXA;
// Instruction TXS;
// Instruction TYA;

// // Write instructions:
// Instruction STX;
// Instruction STA;
// Instruction STY;

// Instruction CLC;
// Instruction CLD;
// Instruction CLI;
// Instruction CLV;
// Instruction CPY;
// Instruction CPX;

// //branch instructions
// Instruction BCC;
// Instruction BCS;
// Instruction BEQ;
// Instruction BMI;
// Instruction BNE;
// Instruction BPL;
// Instruction BVC;
// Instruction BVS;

// //stack instructions
// Instruction PHA;
// Instruction PHP;
// Instruction PLA;
// Instruction PLP;

// //interrupts
// Instruction BRK;
// Instruction RTI;

// typedef void addr_mode_step(CPU *, BYTE *, Instruction *);
// addr_mode_step fetch_opcode;
// addr_mode_step fetch_throw;
// addr_mode_step fetch_throw_brk;
// addr_mode_step imm_fetch_operand;
// addr_mode_step stack_push_PCH;
// addr_mode_step stack_push_PCL;
// addr_mode_step stack_push_register;
// addr_mode_step increment_S;
// addr_mode_step increment_PC;
// addr_mode_step stack_pull_PCH;
// addr_mode_step stack_pull_PCL;
// addr_mode_step stack_pull_register;
// addr_mode_step fetch_PCL;
// addr_mode_step fetch_PCH;
// addr_mode_step fetch_ADL;
// addr_mode_step fetch_ADH;
// addr_mode_step fetch_address;
// addr_mode_step read_addr_exe;
// addr_mode_step read_addr;
// addr_mode_step read_zp_addr_exe;
// addr_mode_step read_zp_addr;
// addr_mode_step modify;
// addr_mode_step write_addr;
// addr_mode_step write_register;
// addr_mode_step read_addr_add_X;
// addr_mode_step read_addr_add_Y;
// addr_mode_step fetch_ADH_add_X;
// addr_mode_step fetch_ADH_add_Y;
// addr_mode_step read_addr_fixADH_exe;
// addr_mode_step read_addr_updated_exe;
// addr_mode_step read_addr_fixADH;
// addr_mode_step read_addr_updated;
// addr_mode_step write_register_fixedADH;
// addr_mode_step read_ptr_add_X;
// addr_mode_step fetch_ptr_ADL;
// addr_mode_step fetch_ptr_ADH_add_Y;
// addr_mode_step fetch_ptrX_ADL;
// addr_mode_step fetch_ptrX_ADH;
// addr_mode_step branch_PCL;
// addr_mode_step branch_fixPCH;
// addr_mode_step read_PCH;
// addr_mode_step hold_ADL;
// addr_mode_step set_PC_to_JSR;
// addr_mode_step set_PC_to_JMP;


// typedef struct addr_mode {
//     int numsteps;
//     addr_mode_step **step;
// } AddressingMode;

// typedef struct op {
//     Instruction *ins;
//     AddressingMode *mode;
// } Operation;

// Operation decode(CPU *cpu);

// void execute(CPU *cpu, Operation operation, BYTE *memory);

void clocktick(CPU *, Bus);
void print_cpu(CPU *);

void power_cpu(CPU *);

void setFlag(CPU *, int);
void clearFlag(CPU *, int);
BYTE getBit(BYTE, int);
#endif