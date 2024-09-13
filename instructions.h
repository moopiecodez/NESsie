#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define BYTE uint8_t


typedef void Instruction(CPU *);
//Read instructions:
Instruction LDA;
Instruction LDX;
Instruction LDY;
Instruction EOR;
Instruction AND;
Instruction ORA;
Instruction ADC;
Instruction SBC;
Instruction CMP;
Instruction BIT;
Instruction NOP;


//RMW instructions:
Instruction ASL;
Instruction LSR;
Instruction ROL;
Instruction ROR;
Instruction INC;
Instruction INX;
Instruction INY;
Instruction DEC;
Instruction DEX;
Instruction DEY;

void increment(CPU *cpu, BYTE memory);
void decrement(CPU *cpu, BYTE *memory);

//transfer instructions
Instruction TAX;
Instruction TAY;
Instruction TSX;
Instruction TXA;
Instruction TXS;
Instruction TYA;

// Write instructions:
Instruction STX;
Instruction STA;
Instruction STY;

Instruction CLC;
Instruction CLD;
Instruction CLI;
Instruction CLV;
Instruction CPY;
Instruction CPX;

//branch instructions
Instruction BCC;
Instruction BCS;
Instruction BEQ;
Instruction BMI;
Instruction BNE;
Instruction BPL;
Instruction BVC;
Instruction BVS;

//stack instructions
Instruction PHA;
Instruction PHP;
Instruction PLA;
Instruction PLP;

//interrupts
Instruction BRK;
Instruction RTI;

typedef void addr_mode_step(CPU *, BYTE *, Instruction *);
addr_mode_step fetch_opcode;
addr_mode_step fetch_throw;
addr_mode_step fetch_throw_brk;
addr_mode_step imm_fetch_operand;
addr_mode_step stack_push_PCH;
addr_mode_step stack_push_PCL;
addr_mode_step stack_push_register;
addr_mode_step increment_S;
addr_mode_step increment_PC;
addr_mode_step stack_pull_PCH;
addr_mode_step stack_pull_PCL;
addr_mode_step stack_pull_register;
addr_mode_step fetch_PCL;
addr_mode_step fetch_PCH;
addr_mode_step fetch_ADL;
addr_mode_step fetch_ADH;
addr_mode_step fetch_address;
addr_mode_step read_addr_exe;
addr_mode_step read_addr;
addr_mode_step read_zp_addr_exe;
addr_mode_step read_zp_addr;
addr_mode_step modify;
addr_mode_step write_addr;
addr_mode_step write_register;
addr_mode_step read_addr_add_X;
addr_mode_step read_addr_add_Y;
addr_mode_step fetch_ADH_add_X;
addr_mode_step fetch_ADH_add_Y;
addr_mode_step read_addr_fixADH_exe;
addr_mode_step read_addr_updated_exe;
addr_mode_step read_addr_fixADH;
addr_mode_step read_addr_updated;
addr_mode_step write_register_fixedADH;
addr_mode_step read_ptr_add_X;
addr_mode_step fetch_ptr_ADL;
addr_mode_step fetch_ptr_ADH_add_Y;
addr_mode_step fetch_ptrX_ADL;
addr_mode_step fetch_ptrX_ADH;
addr_mode_step branch_PCL;
addr_mode_step branch_fixPCH;
addr_mode_step read_PCH;
addr_mode_step hold_ADL;
addr_mode_step set_PC_to_JSR;
addr_mode_step set_PC_to_JMP;


typedef struct addr_mode {
    int numsteps;
    addr_mode_step **step;
} AddressingMode;

typedef struct op {
    Instruction *ins;
    AddressingMode *mode;
} Operation;

#endif
