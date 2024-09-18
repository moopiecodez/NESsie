#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

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

struct cpu {
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
};

CPU * cpu_create(){
    CPU *cpu = malloc(sizeof(struct cpu));
    return cpu;
}

void setFlag(CPU *, int);
void clearFlag(CPU *, int);
BYTE getBit(BYTE, int);

//instruction and addressing mode prototypes
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

void increment(CPU *, BYTE);
void decrement(CPU *, BYTE);

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

//flag instructions 
Instruction CLC;
Instruction CLD;
Instruction CLI;
Instruction CLV;
Instruction CPY;
Instruction CPX;

Instruction SEC;
Instruction SED;
Instruction SEI;

//branch instructions
Instruction BCC;
Instruction BCS;
Instruction BEQ;
Instruction BMI;
Instruction BNE;
Instruction BPL;
Instruction BVC;
Instruction BVS;

//jump instructions
Instruction JMP;
Instruction JSR;
//stack instructions
Instruction PHA;
Instruction PHP;
Instruction PLA;
Instruction PLP;

//interrupts
Instruction BRK;
Instruction RTI;
Instruction RTS;

Instruction NOT;


typedef void addr_mode_step(CPU *, Bus, Instruction *);
addr_mode_step fetch_opcode;
addr_mode_step fetch_throw;
addr_mode_step fetch_throw_accumulator;
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
    char *name;
} Operation;

Operation decode(CPU *cpu);

void execute(CPU *, Operation, Bus);

void print_cpu(CPU *cpu) {
    char flags[8];
    for (int i = 7; i >= 0; i--) {
        flags[7-i] = cpu->P & (1<<i) ? '1' : '0';
    }
    printf("PC: %04x, AB: %04x, DB: %02x, IR: %02x, A: %02x, X: %02x, Y: %02x, P: %s, T: %u, DL: %02x, ALU: %02x, ACR: %02x\n",
            cpu->PC, cpu->AB, cpu->DB, cpu->IR, cpu->A, cpu->X, cpu->Y,
              flags, cpu->T, cpu->DL, cpu->ALU, cpu->ACR_FLAG);
}

void clocktick(CPU *cpu, Bus bus) {
    Operation operation;
    operation = decode(cpu);
    execute(cpu, operation, bus);
    printf("%s | ", operation.name);
    print_cpu(cpu);
    cpu->T++;
}

/*
macros mean that calling:
    addressingmode(addr_x, {fetch_opcode, ...});
expands to:
    addr_mode_step *addr_x_[] = {
        fetch_opcode,
        ...
    };
    AddressingMode addr_x = {
        (int)(sizeof addr_x_ / sizeof addr_x_[0]),
        addr_x_
    };
*/
#define len(X) (int)(sizeof X / sizeof X[0])
#define passarray(...) __VA_ARGS__
#define addressingmode_(X, Y, Z) addr_mode_step *Y[] = Z; AddressingMode X = {len(Y), Y}
#define addressingmode(name, array) addressingmode_(name, name##_, passarray(array))


addressingmode(addr_imp_brk, passarray({
    fetch_opcode,
    fetch_throw_brk,
    stack_push_PCH,
    stack_push_PCL,
    stack_push_register,
    fetch_PCL,
    fetch_PCH
}));

addressingmode(addr_imp_rts, passarray({
    fetch_opcode,
    fetch_throw,
    increment_S,
    stack_pull_PCL,
    stack_pull_PCH,
    increment_PC
}));

addressingmode(addr_imp_jsr, passarray({
    fetch_opcode,
    fetch_ADL,
    hold_ADL,
    stack_push_PCH,
    stack_push_PCL,
    set_PC_to_JSR
}));

addressingmode(addr_jmp_abs, passarray({
    fetch_opcode,
    fetch_ADL,
    set_PC_to_JMP
}));

addressingmode(addr_imp_rti, passarray({
    fetch_opcode,
    fetch_throw,
    increment_S,
    stack_pull_register,
    stack_pull_PCL,
    stack_pull_PCH
}));

addressingmode(addr_immediate, passarray({
    fetch_opcode,
    imm_fetch_operand
}));

addressingmode(addr_implied, passarray({
    fetch_opcode,
    fetch_throw     //unlike in BRK, PC not incremented
}));

addressingmode(addr_accumulator, passarray({
    fetch_opcode,
    fetch_throw_accumulator //unlike in BRK, PC not incremented
}));
/*
    implied push
    addressing mode for instructions pushing to stack
*/
addressingmode(addr_imp_push, passarray ({
    fetch_opcode,
    fetch_throw,
    stack_push_register
}));

/*
    implied pull
    addressing mode for instructions pulling from stack
*/
addressingmode(addr_imp_pull, passarray ({
    fetch_opcode,
    fetch_throw,
    increment_S,
    stack_pull_register
}));

addressingmode(addr_relative, passarray({
    fetch_opcode,
    imm_fetch_operand,
    branch_PCL,
    branch_fixPCH
}));

/*
absolute addressing - read
*/
addressingmode(addr_abs_r, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr_exe   //executes operation
}));

/*
absolute addressing - read, modify, write
*/
addressingmode(addr_abs_rmw, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr,
    modify, //zimmers describes as write value back and then do operation
    write_addr
}));

/*
absolute addressing - write
*/
addressingmode(addr_abs_w, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    write_register
}));

/*
zeropage addressing - read
*/
addressingmode(addr_zeropage_r, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_exe   //executes operation
}));

/*
zeropage addressing - read, modify, write
*/
addressingmode(addr_zeropage_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_addr,
    modify,
    write_addr
}));

/*
zeropage addressing - write
*/
addressingmode(addr_zeropage_w, passarray({
    fetch_opcode,
    fetch_address,
    write_register
}));

/*
zeropage indexed X addressing - read
*/
addressingmode(addr_zp_X_r, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_X,
    read_addr
}));

/*
zeropage indexed Y addressing - read
*/
addressingmode(addr_zp_Y_r, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_Y,
    read_addr
}));

/*
zeropage indexed X addressing - read, modify, write
*/
addressingmode(addr_zp_X_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_X,
    read_addr,
    modify,
    write_addr
}));

/*
zeropage indexed Y addressing - read, modify, write
*/
addressingmode(addr_zp_Y_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_Y,
    read_addr,
    modify,
    write_addr
}));

/*
zeropage indexed X addressing - write
*/
addressingmode(addr_zp_X_w, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_X,
    write_register
}));

/*
zeropage indexed Y addressing - write
*/
addressingmode(addr_zp_Y_w, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_Y,
    write_register
}));

/*
absolute X addressing - read
*/
addressingmode(addr_abs_X_r, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_X,
    read_addr_fixADH,
    read_addr_updated_exe
}));

/*
absolute Y addressing - read
*/
addressingmode(addr_abs_Y_r, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_Y,
    read_addr_fixADH_exe,
    read_addr_updated_exe
}));

/*
absolute X addressing - read, modify, write
*/
addressingmode(addr_abs_X_rmw, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_X,
    read_addr_fixADH,
    read_addr_updated,
    modify,
    write_addr
}));

/*
absolute Y addressing - read, modify, write
*/
addressingmode(addr_abs_Y_rmw, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_Y,
    read_addr_fixADH,
    read_addr_updated,
    modify,
    write_addr
}));

/*
absolute X addressing - write
*/
addressingmode(addr_abs_X_w, passarray ({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_X,
    read_addr,
    write_register_fixedADH
}));

/*
absolute Y addressing - write
*/
addressingmode(addr_abs_Y_w, passarray ({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_Y,
    read_addr,
    write_register_fixedADH //trace not quite accurate as accumulator should be dealt with before
}));

/*
indexed indirect addressing - read
*/
addressingmode(addr_indX_r, passarray ({
    fetch_opcode,
    fetch_address,
    read_ptr_add_X,
    fetch_ptrX_ADL,
    fetch_ptrX_ADH,
    read_addr_exe
}));

/*
indexed indirect addressing - read, modify, write
*/
addressingmode(addr_indX_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_ptr_add_X,
    fetch_ptrX_ADL,
    fetch_ptrX_ADH,
    read_addr,
    modify, //zimmers describes as write value back and then do operation
    write_addr
}));

/*
indexed indirect addressing - write
*/
addressingmode(addr_indX_w, passarray({
    fetch_opcode,
    fetch_address,
    read_ptr_add_X,
    fetch_ptrX_ADL,
    fetch_ptrX_ADH,
    write_register
}));

/*
indirect indexed addressing - read
*/
addressingmode(addr_indY_r, passarray({
    fetch_opcode,
    fetch_address,
    fetch_ptr_ADL,
    fetch_ptr_ADH_add_Y,
    read_addr_fixADH_exe,
    read_addr_updated_exe
}));

/*
indirect indexed addressing - read, modify, write
*/
addressingmode(addr_indY_rmw, passarray ({
    fetch_opcode,
    fetch_address,
    fetch_ptr_ADL,
    fetch_ptr_ADH_add_Y,
    read_addr_fixADH,
    read_addr_updated,
    modify,
    write_addr
}));

/*
indirect indexed addressing - write
*/
addressingmode(addr_indY_w, passarray({
    fetch_opcode,
    fetch_address,
    fetch_ptr_ADL,
    fetch_ptr_ADH_add_Y,
    read_addr_fixADH,
    write_register_fixedADH
}));

/*
absolute indirect addressing - used by jump
*/
addressingmode(addr_jmp_abs_X, passarray({ //doesn't handle page boundary crossing between PCL and PCH locations
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr,
    read_PCH
}));

// Read instructions (LDA, LDX, LDY, EOR, AND, ORA, ADC, SBC, CMP, BIT, LAX, NOP)
//Read-Modify-Write instructions (ASL, LSR, ROL, ROR, INC, DEC, SLO, SRE, RLA, RRA, ISB, DCP)
//Write instructions (STA, STX, STY, SAX)

#define op(instruction, mode) { instruction, &addr_##mode , #instruction }

Operation operations[] = {
    op(BRK, imp_brk), op(ORA, indX_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(ORA, zeropage_r), op(ASL, zeropage_rmw), op(NOT, implied), op(PHP, imp_push), op(ORA, immediate), op(ASL, accumulator), op(NOT, implied), op(NOT, implied), op(ORA, abs_r), op(ASL, abs_rmw), op(NOT, implied),
    op(BPL, relative), op(ORA, indY_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(ORA, zp_X_r), op(ASL, zp_X_rmw), op(NOT, implied), op(CLC, implied), op(ORA, abs_Y_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(ORA, abs_X_r), op(ASL, abs_X_rmw), op(NOT, implied),
    op(JSR, imp_jsr), op(AND, indX_r), op(NOT, implied), op(NOT, implied), op(BIT, zeropage_r), op(AND, zeropage_r), op(ROL, zeropage_rmw), op(NOT, implied), op(PLP, imp_pull), op(AND, immediate), op(ROL, accumulator), op(NOT, implied), op(BIT, abs_r), op(AND, abs_r), op(ROL, abs_rmw), op(NOT, implied),
    op(BMI, relative), op(AND, indY_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(AND, zp_X_r), op(ROL, zp_X_rmw), op(NOT, implied), op(SEC, implied), op(AND, abs_Y_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(AND, abs_X_r), op(ROL, abs_X_rmw), op(NOT, implied),
    op(RTI, imp_rti), op(EOR, indX_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(EOR, zeropage_r), op(LSR, zeropage_rmw), op(NOT, implied), op(PHA, imp_push), op(EOR, immediate), op(LSR, accumulator), op(NOT, implied), op(JMP, jmp_abs), op(EOR, abs_r), op(LSR, abs_rmw), op(NOT, implied),
    op(BVC, relative), op(EOR, indY_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(EOR, zp_X_r), op(LSR, zp_X_rmw), op(NOT, implied), op(CLI, implied), op(EOR, abs_Y_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(EOR, abs_X_r), op(LSR, abs_X_rmw), op(NOT, implied),
    op(RTS, imp_rts), op(ADC, indX_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(ADC, zeropage_r), op(ROR, zeropage_rmw), op(NOT, implied), op(PLA, imp_pull), op(ADC, immediate), op(ROR, accumulator), op(NOT, implied), op(JMP, jmp_abs_X), op(ADC, abs_r), op(ROR, abs_rmw), op(NOT, implied),
    op(BVS, relative), op(ADC, indY_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(ADC, zp_X_r), op(ROR, zp_X_rmw), op(NOT, implied), op(SEI, implied), op(ADC, abs_Y_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(ADC, abs_X_r), op(ROR, abs_X_rmw), op(NOT, implied),
    op(NOT, implied), op(STA, indX_w), op(NOT, implied), op(NOT, implied), op(STY, zeropage_w), op(STA, zeropage_w), op(STX, zeropage_w), op(NOT, implied), op(DEY, implied), op(NOT, implied), op(TXA, implied), op(NOT, implied), op(STY, abs_w), op(STA, abs_w), op(STX, abs_w), op(NOT, implied),
    op(BCC, relative), op(STA, indY_w), op(NOT, implied), op(NOT, implied), op(STY, zp_X_w), op(STA, zp_X_w), op(STX, zp_Y_w), op(NOT, implied), op(TYA, implied), op(STA, abs_Y_w), op(TXS, implied), op(NOT, implied), op(NOT, implied), op(STA, abs_X_w), op(NOT, implied), op(NOT, implied),
    op(LDY, immediate), op(LDA, indX_r), op(LDX, immediate), op(NOT, implied), op(LDY, zeropage_r), op(LDA, zeropage_r), op(LDX, zeropage_r), op(NOT, implied), op(TAY, implied), op(LDA, immediate), op(TAX, implied), op(NOT, implied), op(LDY, abs_r), op(LDA, abs_r), op(LDX, abs_r), op(NOT, implied),
    op(BCS, relative), op(LDA, indY_r), op(NOT, implied), op(NOT, implied), op(LDY, zp_X_r), op(LDA, zp_X_r), op(LDX, zp_Y_r), op(NOT, implied), op(CLV, implied), op(LDA, abs_Y_r), op(TSX, implied), op(NOT, implied), op(LDY, abs_X_r), op(LDA, abs_X_r), op(LDX, abs_Y_r), op(NOT, implied),
    op(CPY, immediate), op(CMP, indX_r), op(NOT, implied), op(NOT, implied), op(CPY, zeropage_r), op(CMP, zeropage_r), op(DEC, zeropage_rmw), op(NOT, implied), op(INY, implied), op(CMP, immediate), op(DEX, implied), op(NOT, implied), op(CPY, abs_r), op(CMP, abs_r), op(DEC, abs_rmw), op(NOT, implied),
    op(BNE, relative), op(CMP, indY_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(CMP, zp_X_r), op(DEC, zp_X_rmw), op(NOT, implied), op(CLD, implied), op(CMP, abs_Y_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(CMP, abs_X_r), op(DEC, abs_X_rmw), op(NOT, implied),
    op(CPX, immediate), op(SBC, indX_r), op(NOT, implied), op(NOT, implied), op(CPX, zeropage_r), op(SBC, zeropage_r), op(INC, zeropage_rmw), op(NOT, implied), op(INX, implied), op(SBC, immediate), op(NOP, implied), op(NOT, implied),  op(CPX, abs_r), op(SBC, abs_r), op(INC, abs_rmw), op(NOT, implied),
    op(BEQ, relative), op(SBC, indY_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(SBC, zp_X_r), op(INC, zp_X_rmw), op(NOT, implied), op(SED, implied), op(SBC, abs_Y_r), op(NOT, implied), op(NOT, implied), op(NOT, implied), op(SBC, abs_X_r), op(INC, abs_X_r), op(NOT, implied)
};

Operation decode(CPU *cpu) {
    Operation operation = operations[cpu->IR];
    if(cpu->T >= operation.mode->numsteps) {
        cpu->T = 0;
    }
    return operation;
}

void execute(CPU *cpu, Operation operation, Bus bus){
    operation.mode->step[cpu->T](cpu, bus, operation.ins);
}

/*
    Fetches opcode and loads it into Instruction Register.
    Increments Program Counter.
*/
void fetch_opcode(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->IR = cpu->DL;
    cpu->PC++;
}

void increment_PC(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->PC = (cpu->DL << 8) + cpu->ALU;
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB; //reflect visual6502
    ins(cpu); //RTS doesn't actually do anything but includes so ins used
    cpu->PC++;
}

/*
    Reads next byte and throws it away.
    Used for implied and accumulator addressing.
    Does not increment Program Counter.
    Note BRK would increment PC
*/
void fetch_throw(CPU *cpu, Bus bus, Instruction *ins) {
    //emulate reading memory but doing nothing with it
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    ins(cpu);
}

void fetch_throw_accumulator(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->DL = cpu->A; //abstraction to ensure instruction executes on accumulator value
    ins(cpu);
    cpu->A = cpu->ALU; //update accumulator with modified value
    //emulate reading memory but doing nothing with it
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
}

/*
    Reads next byte and throws it away.
    Used for implied accumulator addressing BRK instruction.
    Increments Program Counter.
*/
void fetch_throw_brk(CPU *cpu, Bus bus, Instruction *ins) {
    //emulate reading memory but doing nothing with it
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->PC++;
}

/*
    Fetches operand and holds it in CPU predecode register.
    Increments Program Counter.
*/
void imm_fetch_operand(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->PC++;
    ins(cpu);
    if(cpu->ACR_FLAG == 0) { //for branch instructions check if branch triggered
        cpu->T++; //skip branch next step
        cpu->T++; //skip potential second next step to fix PCH if page boundary crossed
    }
}

void branch_PCL(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->ALU = (BYTE)(cpu->PC & 0xFF) + cpu->DL;
    if((cpu->DL < 0x80) && !(cpu->ALU < cpu->DL)){ // operand < 0x80 means positive offset 0x80 so check for overflow
        cpu->ACR_FLAG = 0; //only need to adjust to 0 as carry flag already set if you get to this cycle
    }
    if((cpu->DL >= 0x80) && !(cpu->ALU > (BYTE)(cpu->PC & 0xFF))) { // need to check condition as offset can be negative
        cpu->ACR_FLAG = 0;
    }
    if(cpu->ACR_FLAG == 0){
        cpu->T++;
    }
    cpu->PC = (cpu->PC & 0xFF00) + cpu->ALU;

}

void branch_fixPCH(CPU *cpu, Bus bus, Instruction *ins) {
    if(cpu->DL < 0x80) {
        cpu->PC += 0x0100; //correct for positive offset page cross
    } else {
        cpu->PC -= 0x0100; //correct for negative offset page cross
    }
}

void stack_push_PCH(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->DB = cpu->PC >> 8;
    cpu->DL = cpu->DB;
    cpu->AB = STACK_BASE + cpu->S;
    bus_write(bus, cpu->AB, cpu->DL);
    cpu->S--;
}

void stack_push_PCL(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->DB = cpu->PC;
    cpu->DL = cpu->DB;
    cpu->AB = STACK_BASE + cpu->S;
    bus_write(bus, cpu->AB, cpu->DL);
    cpu->S--;
}

void stack_pull_PCH(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->ALU = cpu->DL; //PCL obtained in previous cycle
    cpu->AB = STACK_BASE + cpu->S;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
}

void stack_pull_PCL(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = STACK_BASE + cpu->S;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->S++;
}

void stack_push_register(CPU *cpu, Bus bus, Instruction *ins) {
    ins(cpu);
    cpu->DL = cpu->DB;
    cpu->AB = STACK_BASE + cpu->S;
    bus_write(bus, cpu->AB, cpu->DL);
    cpu->S--;
}

void stack_pull_register(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = STACK_BASE + cpu->S;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    ins(cpu);
}

void increment_S(CPU *cpu, Bus bus, Instruction *ins) {
    //AB set to stack and read but nothing done with it (see visual6502)
    cpu->AB = STACK_BASE + cpu->S;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->S++;
}

void fetch_PCL(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = IRQ_LOW;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->PC = cpu->DL;
    setFlag(cpu, FLAG_I);
}

void fetch_PCH(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = IRQ_HIGH;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->PC += cpu->DL << 8;
}

void set_PC_to_JSR(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB); //get PCH
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->PD is ADH
    cpu->PC = cpu->ALU; //ALU holds target PCL from earlier in JSR routine
    cpu->PC += cpu->DL << 8;
}

void set_PC_to_JMP(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB); //get PCH
    cpu->PC = cpu->DL; //DL still holds target PCL at start
    cpu->PC += cpu->DB << 8;
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->PD is ADH
}

void hold_ADL(CPU *cpu, Bus bus, Instruction *ins){
    cpu->ALU = cpu->DL; //holds ADL for jump later in JSR
    ins(cpu);
}


void fetch_ADL(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    cpu->PC++;
}

void fetch_ADH(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->ALU = cpu->DL; //start of cycle cpu->DL is ADL which is stored in ALU
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->PD is ADH
    cpu->PC++;
}

void fetch_address(CPU *cpu, Bus bus, Instruction *ins){ //***check as this reads address held in PC
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB; //at end of cycle DL holds contents of address stored in PC to be used as address in next cycle
    cpu->PC++;
}

void read_addr_exe(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    ins(cpu);
}

void read_zp_addr_exe(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->DL;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    ins(cpu);
}

void read_zp_addr(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->DL; //in previous cycle effective address stored in DL
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
}

void read_addr(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU; //in previous cycle ADL address stored in ALU and DL holds ADH
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB; //DL holds target PCL
}

void read_PCH(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->AB + 1;
    cpu->DB = bus_read(bus, cpu->AB); //DB holds PCH
    cpu->PC = (cpu->DB << 8) + cpu->DL; //DL holds target PCL from previous cycle
    cpu->DL = cpu->DB;
}

void modify(CPU *cpu, Bus bus, Instruction *ins) {
    //memory[cpu->AB] = cpu->DB writes unmodified value again first
    ins(cpu); 
}

void write_addr(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->DB = cpu->ALU;
    cpu->DL = cpu->DB;
    bus_write(bus, cpu->AB, cpu->DL); //write modified value
}

void write_register(CPU *cpu, Bus bus, Instruction *ins) {
    ins(cpu);
    cpu->DL = cpu->DB;
    bus_write(bus, cpu->AB, cpu->DL); //write register value returned by instruction
}

void read_addr_add_X(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->DB = cpu->X;
    cpu->ALU = cpu->DL + cpu->DB; //page boundary crossings not handled
    cpu->DL = cpu->DB;
    cpu->AB = cpu->ALU; //check this is happening at right point
}

void read_addr_add_Y(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->DB = cpu->Y;
    cpu->ALU = cpu->DL + cpu->DB; //page boundary crossings not handled
    cpu->DL = cpu->DB;
    cpu->AB = cpu->ALU;
}

void fetch_ADH_add_X(CPU *cpu, Bus bus, Instruction *ins){
    //at the start of cycle cpu->DL holds ADL
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB); //DB is ADH
    cpu->ALU = cpu->DL + cpu->X; // add ADL and X
    if(cpu->ALU < cpu->X) { //set ALU carry flag if page boundary crossed
        cpu->ACR_FLAG = 1;
    } else {
        cpu->ACR_FLAG = 0;
    }
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->DL is ADH
    cpu->PC++;
}
void fetch_ADH_add_Y(CPU *cpu, Bus bus, Instruction *ins){
    //start of cycle cpu->DL is ADL
    cpu->AB = cpu->PC;
    cpu->DB = bus_read(bus, cpu->AB); //DB is ADH
    cpu->ALU = cpu->DL + cpu->Y; // add ADL and X
    if(cpu->ALU < cpu->Y) { //set ALU carry flag if page boundary crossed
        cpu->ACR_FLAG = 1;
    } else {
        cpu->ACR_FLAG = 0;
    }
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->DL is ADH
    cpu->PC++;
}

void read_addr_fixADH_exe(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU;
    cpu->DB = bus_read(bus, cpu->AB);
    if(cpu->ACR_FLAG != 0) {
        cpu->ALU = cpu->DL + 1; //fixed ADH value including page boundary cross
        cpu->ACR_FLAG = 0;
    } else {
        cpu->DL = cpu->DB; //check if ins use DB or DL
        ins(cpu);
        cpu->T++; //skip next step if additional cycle not needed
    }
    cpu->DL = cpu->DB;
}

void read_addr_updated_exe(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = (cpu->ALU << 8) + (cpu->AB & 0xff); //updated address
    cpu->ALU = 0x00;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
    ins(cpu);
}

void read_addr_fixADH(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU;
    cpu->DB = bus_read(bus, cpu->AB);
    if(cpu->ACR_FLAG != 0) {
        cpu->ALU = cpu->DL + 1; //fixed ADH value including page boundary cross
        cpu->ACR_FLAG = 0;
    } else {
        cpu->T++; //skip next step if additional cycle not needed
    }
    cpu->DL = cpu->DB;
}

void read_addr_updated(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = (cpu->ALU << 8) + (cpu->AB & 0xff); //updated address
    cpu->ALU = 0x00;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->DL = cpu->DB;
}

void write_register_fixedADH(CPU *cpu, Bus bus, Instruction *ins) {
    if(cpu->ACR_FLAG != 0) { //technically ALU and flag should be reset by time write starts
        cpu->ALU = (cpu->AB >> 8) + 1;
        cpu->ACR_FLAG = 0;
        cpu->AB = (cpu->ALU << 8) + (cpu->AB & 0xff);
        //cpu->AB = cpu->AB + 0x0100; alternative
    }
    ins(cpu);
    cpu->DL = cpu->DB;
    bus_write(bus, cpu->AB, cpu->DL); //write register value returned by instruction
}

void read_ptr_add_X(CPU *cpu, Bus bus, Instruction *ins) {
    cpu->AB = cpu->DL;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->ALU = cpu->DL + cpu->X; //ALU holds address (ptr fetched in previous cycle + X) doesn't deal with crossing page boundaries
    cpu->DL = cpu->DB;
}

void fetch_ptrX_ADL(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->ALU; //ptr + X
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->ALU = cpu->ALU + 1; //ptr + X + 1
    cpu->DL = cpu->DB; //DL is ADL
}

void fetch_ptrX_ADH(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->ALU; //ALU holds ptr + X + 1
    cpu->DB = bus_read(bus, cpu->AB); //DB is AHL, DL still holds ADL
    cpu->ALU = cpu->DB; //store ADL in ALU
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->DL is ADH
}
void fetch_ptr_ADL(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->DL;
    cpu->DB = bus_read(bus, cpu->AB);
    cpu->ALU = cpu->DL + 1;
    cpu->DL = cpu->DB; //DL is ADL
}

void fetch_ptr_ADH_add_Y(CPU *cpu, Bus bus, Instruction *ins){
    cpu->AB = cpu->ALU;
    cpu->DB = bus_read(bus, cpu->AB); //DB is AHL
    cpu->ALU = cpu->DB + cpu->Y; //store ADL in ALU add Y
    if(cpu->ALU < cpu->Y) {
        cpu->ACR_FLAG = 1;
    } else {
        cpu->ACR_FLAG = 0;
    }
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->DL is ADH
}

/*
    BRK - Break
    Assumes Opcode process has already incremented PC by 2.
    B flag set.
    PC and P pushed to stack.
    I flag set.
    Interrupt Pointer ($FFFF and $FFFE) loaded into PC.
*/
void BRK(CPU *cpu) {
    setFlag(cpu, FLAG_B);
    cpu->DB = cpu->P;
    //interrupt flag set after status register pushed
}

/*
    Clear carry flag
    Sets C flag to 0
*/
void CLC(CPU *cpu) {
    clearFlag(cpu, FLAG_C);
}

void power_cpu(CPU *cpu) {
    cpu->P = cpu->A = cpu->X = cpu->Y = 0x00u;
    cpu->PC = 0xFFFCu;
    cpu->S = 0xFFu;
    cpu->IR = 0x00u;
    cpu->DB = 0x00u;
    setFlag(cpu, FLAG_I);
    //clear internal RAM except high scores $0000-$07FF
}

void setFlag(CPU *cpu, int position) {
    cpu->P = cpu->P | (FLAG_MASK << position);
}

void clearFlag(CPU *cpu, int position) {
    cpu->P = cpu->P & ~(FLAG_MASK << position);
}

BYTE getBit(BYTE source, int position) {
    unsigned int bit = (source >> position) & FLAG_MASK;
    return bit;
}

void increment(CPU *cpu, BYTE target) {
    cpu->ALU = target + 1;
    if (cpu->ALU == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->ALU, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    INC - increment memory by 1
    Z and N flags set depending on result.
*/
void INC(CPU *cpu) {
    increment(cpu, cpu->DL);
}

/*
    INX - increment X register by 1
    Z and N flags set depending on result.
*/
void INX(CPU *cpu) {
    increment(cpu, cpu->X);
    cpu->X = cpu->ALU; //in actual hardware register updates in second cycle of next instruction
}

/*
    INY - increment Y register by 1
    Z and N flags set depending on result.
*/
void INY(CPU *cpu) {
    increment(cpu, cpu->Y);
    cpu->Y = cpu->ALU;
}

void decrement(CPU *cpu, BYTE target) {
    cpu->ALU = target - 1;
    if (cpu->ALU == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->ALU, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    DEC - decrement memory by 1
    Z and N flags set depending on result.
*/
void DEC(CPU *cpu) {
    decrement(cpu, cpu->DL);
}

/*
    DEX - decrement X register by 1
    Z and N flags set depending on result.
*/
void DEX(CPU *cpu) {
    decrement(cpu, cpu->X);
    cpu->X = cpu->ALU;
}

/*
    DEY - decrement Y register by 1
    Z and N flags set depending on result.
*/
void DEY(CPU *cpu) {
    decrement(cpu, cpu->Y);
    cpu->Y = cpu->ALU;
}
/*
    LDA - load a byte of memory into accumulator
    Z and N flags set depending on result.
*/
void LDA(CPU *cpu) {
    cpu->A = cpu->DL;
    if (cpu->DL == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->DL, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    LDX - load a byte of memory into X register
    Z and N flags set depending on result.
*/
void LDX(CPU *cpu) {
    cpu->X = cpu->DL;
    if (cpu->DL == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->DL, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    LDY - load a byte of memory into Y register
    Z and N flags set depending on result.
*/
void LDY(CPU *cpu) {
    cpu->Y = cpu->DL;
    if (cpu->DL == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->DL, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    ASL - Arithmetic Shift Left
    Shifts bits in A or memory left by one place.
    Carry flag set to contents of old 7 bit.
    Sets Zero and Negative flag based on result.
*/
void ASL(CPU *cpu) {
    if (getBit(cpu->DL, 7) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        clearFlag(cpu, FLAG_C);
    }
    cpu->ALU = cpu->DL << 1; //ALU holds result before it is transferred on buses
    if (cpu->ALU == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->ALU, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    LSR - logical shift right
    Shifts bits in A or memory right by one place. 
    Carry flag set to contents of old 0 bit.
    Sets Zero and Negative flag based on result.
*/
void LSR(CPU *cpu) {
    if (getBit(cpu->DL, 0) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        clearFlag(cpu, FLAG_C);
    }
    cpu->ALU = cpu->DL >> 1; //ALU holds result before it is transferred on buses
    if (cpu->ALU == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->ALU, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }

}

/*
    ROL - Rotate Left
    Shifts bits in A or memory left by one place.
    Bit 0 is set to contents of Carry flag.
    Carry flag set to contents of old 7 bit.
    Sets Zero and Negative flag based on result.
*/
void ROL(CPU *cpu) {
    BYTE mask;
    if(getBit(cpu->P, FLAG_C) != 0) {
        mask = FLAG_MASK;
    } else {
        mask = 0x0;
    }
    if (getBit(cpu->DL, LEFT_BIT) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        clearFlag(cpu, FLAG_C);
    }
    cpu->ALU = cpu->DL << 1;
    //sets 0 bit
    cpu->ALU = cpu->ALU | mask;
    if (cpu->ALU == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->ALU, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    ROR - Rotate Right
    Shifts bits in A or memory right by one place.
    Bit 7 is set to contents of Carry flag.
    Carry flag set to contents of old 0 bit.
    Sets Zero and Negative flag based on result.
*/
void ROR(CPU *cpu) {
    BYTE mask;
    if(getBit(cpu->P, FLAG_C) != 0) {
        mask = FLAG_MASK << LEFT_BIT;
    } else {
        mask = 0x0;
    }
    if (getBit(cpu->DL, RIGHT_BIT) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        clearFlag(cpu, FLAG_C);
    }
    cpu->ALU = cpu->DL >> 1;
    //sets 0 bit
    cpu->ALU = cpu->ALU | mask;
    if (cpu->ALU == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->ALU, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    STA - Store Accumulator
    Put contents of A onto DB to store into memory.
*/
void STA(CPU *cpu) {
    cpu->DB = cpu->A;
}

/*
    STX - Store X Register
    Put contents of X onto DB to store into memory.
*/
void STX(CPU *cpu) {
    cpu->DB = cpu->X;
}

/*
    STY - Store Y Register
    Put contents of Y onto DB to store into memory.
*/
void STY(CPU *cpu) {
    cpu->DB = cpu->Y;
}

/*
    TAX - Transfer Accumulator to X
    Copies accumulator contents into X.
    Sets Zero and Negative flags as appropriate.
*/
void TAX(CPU *cpu) {
    cpu->X = cpu->A;
    if (cpu->X == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->X, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    TAY - Transfer Accumulator to Y
    Copies accumulator contents into Y.
    Sets Zero and Negative flags as appropriate.
*/
void TAY(CPU *cpu) {
    cpu->Y = cpu->A;
    if (cpu->Y == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->Y, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    TSX - Transfer Stack Pointer to X
    Copies Stack Pointer value into X.
    Sets Zero and Negative flags as appropriate.
*/
void TSX(CPU *cpu) {
    cpu->X = cpu->S;
    if (cpu->X == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->X, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    TXA - Transfer X to Accumulator
    Copies X contents into Accumulator.
    Sets Zero and Negative flags as appropriate.
*/
void TXA(CPU *cpu) {
    cpu->A = cpu->X;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    TXS - Transfer X to Stack Pointer
    Copies X contents into Stack Pointer.
    Does not affect status flags.
*/
void TXS(CPU *cpu) {
    cpu->S = cpu->X;
}

/*
    TYA - Transfer Y to Accumulator
    Copies Y contents into Accumulator.
    Sets Zero and Negative flags as appropriate.
*/
void TYA(CPU *cpu) {
    cpu->A = cpu->Y;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    AND - AND memory and Accumulator
    Bitwise AND performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void AND(CPU *cpu) {
    cpu->A = cpu->A & cpu->DL;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    EOR - Exclusive OR
    Bitwise Exclusive Or performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void EOR(CPU *cpu) {
    cpu->A = cpu->A ^ cpu->DL;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    ORA - Inclusive OR
    Bitwise OR performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void ORA(CPU *cpu) {
    cpu->A = cpu->A | cpu->DL;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
}

/*
    BIT - Bit Test
    Tests if bits set in memory location based on Accumulator.
    ANDs Accumulator and memory location but does not store results.
    Condition flags are set according to AND result (z)
    and memory contents (N and V).
*/
void BIT(CPU *cpu) {
    if((cpu->A & cpu->DL) == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->DL, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
    if (getBit(cpu->DL, FLAG_V) != 0) {
        setFlag(cpu, FLAG_V);
    } else {
        clearFlag(cpu, FLAG_V);
    }
}

/*
    Push - Accumulator
    Puts Accumulator contents on cpu data bus to store on stack.
    No other registers/statuses are affected.
*/
void PHA(CPU *cpu) {
    cpu->DB = cpu->A;
}

/*
    Push - Processor Status
    Puts Processor Status contents on cpu data bus to store on stack.
    No other registers/statuses are affected.
*/
void PHP(CPU *cpu) {
    cpu->DB = cpu->P;
}

/*
    Pull accumulator from stack
    Load accumulator with value pulled from top of stack.
    Sets Zero flag and negative flag based on A.
*/
void PLA(CPU *cpu) {
    cpu->A = cpu->DL;
    if (getBit(cpu->A, FLAG_N)) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
}

/*
    Pull processor status from stack
    Processor loaded with value pulled from top of stack.
*/
void PLP(CPU *cpu) {
    cpu->P = cpu->DL;
}

/*
    ADC - Add with Carry
    Adds contents of memory to accumulator with carry value.
    Carry flag is set if overflow occurs.
    Z flag set if result is 0.
    N flag set if result is negative.
    V flag set if sign bit is incorrect.
*/
void ADC(CPU *cpu) {
    BYTE HIGH_BIT_MASK = 0x80;
    int sum = cpu->DL + cpu->A + getBit(cpu->P, FLAG_C);
    if((cpu->A ^ sum) & (cpu->DL ^ sum) & HIGH_BIT_MASK) {
        setFlag(cpu, FLAG_V);
    } else {
        clearFlag(cpu, FLAG_V);
    }
    cpu->A = sum;
    //check if unsigned sum larger than 255 to determine if carry set
    if(sum > 0xFF) {
        setFlag(cpu, FLAG_C);
    } else {
        clearFlag(cpu, FLAG_C);
    }
    if (getBit(cpu->A, FLAG_N)) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
     if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }

}

/*
    SBC - Subtract with Carry
    Subtracts contents of memory from accumulator with not of carry value.
    Carry flag is clear if overflow occurs.
    Z flag set if result is 0.
    N flag set if result is negative.
    V flag set if sign bit is incorrect.
*/
void SBC(CPU *cpu) {
   cpu->DL = ~(cpu->DL);
   ADC(cpu);
}

/*
    Compare helper function
    Compares given register with memory value and sets flags accordingly.
*/
void set_flags_on_compare(CPU *cpu, BYTE reg) {
    cpu->ALU = reg - cpu->DL;
    if (getBit(cpu->ALU, FLAG_N)) {
        setFlag(cpu, FLAG_N);
    } else {
        clearFlag(cpu, FLAG_N);
    }
    if (reg == cpu->DL) {
        setFlag(cpu, FLAG_Z);
        setFlag(cpu, FLAG_C);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (reg > cpu->DL) {
        setFlag(cpu, FLAG_C);
    } else if (reg < cpu->DL) {
        clearFlag(cpu, FLAG_C);
    }
}

/*
    Compare memory and accumulator
    Subtracts contents of memory from accumulator.
    Z Flag is set if A = M otherwise reset.
    N Flag is set by result bit 7.
    C when memory less than or equal to accumulator, reset if M greater than A

*/
void CMP(CPU *cpu) {
    set_flags_on_compare(cpu, cpu->A);
}

/*
    Compare memory and X register
    Subtracts contents of memory from X register.
    Z is set if X = M otherwise reset.
    N is set by result bit 7.
    C when memory less than or equal to X, reset if M greater than X.

*/
void CPX(CPU *cpu) {
    set_flags_on_compare(cpu, cpu->X);
}

/*
    Compare memory and Y register
    Subtracts contents of memory from Y register.
    Z is set if Y = M otherwise reset.
    N is set by result bit 7.
    C when memory less than or equal to Y, reset if M greater than Y.

*/
void CPY(CPU *cpu) {
    set_flags_on_compare(cpu, cpu->Y);
}

/*
    Clear decimal mode
    Sets D flag to 0
    Not used in NES (NES doesn't use decimal mode)
*/
void CLD(CPU *cpu) {
    clearFlag(cpu, FLAG_D);
}

/*
    Clear interrupt disable bit
    Sets I flag to 0
*/
void CLI(CPU *cpu) {
    clearFlag(cpu, FLAG_I);
}

/*
    Clear overflow flag
    Sets V flag to 0
*/
void CLV(CPU *cpu) {
    clearFlag(cpu, FLAG_V);
}

/*
    Set carry flag
    Sets C flag to 1
*/
void SEC(CPU *cpu) {
    setFlag(cpu, FLAG_C);
}

/*
    Set decimal mode flag
    Sets D flag to 1
*/
void SED(CPU *cpu) {
    setFlag(cpu, FLAG_D);
}

/*
    Set interrupt disable flag
    Sets I flag to 1
*/
void SEI(CPU *cpu) {
    setFlag(cpu, FLAG_I);
}

/*
    BCC - Branch if Carry Flag Clear
    If C flag clear relative displacement added to PC to cause branch.
*/
void BCC(CPU *cpu) {
    if(getBit(cpu->P, FLAG_C) == 0) {
        cpu->ACR_FLAG = 1;
    }
}

/*
    BCS - Branch if Carry Flag Set
    If C flag set relative displacement added to PC to cause branch.
*/
void BCS(CPU *cpu){
    if(getBit(cpu->P, FLAG_C) != 0) {
        cpu->ACR_FLAG = 1;
    }
}

/*
    BEQ - Branch if Equal
    If Z flag set relative displacement added to PC to cause branch.
*/
void BEQ(CPU *cpu) {
    if(getBit(cpu->P, FLAG_Z) != 0) {
        cpu->ACR_FLAG = 1;
    }
}

/*
    BMI - Branch if Minus
    If N flag set relative displacement added to PC to cause branch.
*/
void BMI(CPU *cpu) {
    if(getBit(cpu->P, FLAG_N) != 0) {
        cpu->ACR_FLAG = 1;
    }
}

/*
    BNE - Branch if Not Equal
    If Z flag clear relative displacement added to PC to cause branch.
*/
void BNE(CPU *cpu) {
    if(getBit(cpu->P, FLAG_Z) == 0) {
        cpu->ACR_FLAG = 1;
    }
}

/*
    BPL - Branch if Positive
    If N flag clear relative displacement added to PC to cause branch.
*/
void BPL(CPU *cpu) {
    if(getBit(cpu->P, FLAG_N) == 0) {
        cpu->ACR_FLAG = 1;
    }
}

/*
    BVC - Branch if Overflow Clear
    If V flag clear relative displacement added to PC to cause branch.
*/
void BVC(CPU *cpu) {
    if(getBit(cpu->P, FLAG_V) == 0) {
        cpu->ACR_FLAG = 1;
    }
}

/*
    BVS - Branch if Overflow Set
    If V flag set relative displacement added to PC to cause branch.
*/
void BVS(CPU *cpu) {
    if(getBit(cpu->P, FLAG_V) != 0) {
        cpu->ACR_FLAG = 1;;
    }
}

/*
    JMP - Jumps
    Sets PC to target address set in instruction operand.
*/
void JMP(CPU *cpu) {
    ;
}

/*
    JSR - Jump to Subroutine
    Pushes PC onto the Stack. Stack Pointer adjusted as part of this.
    High byte is stored first.
    Relies on Opcode process calling this function to have incremented PC by 2
    before this function is called.
    Address part of instruction then stored in PC.
*/
void JSR(CPU *cpu) {
    ;//operation implemented in cycle functions
}

/*
    RTS - Return from Subroutine
    Pulls the Program Counter from the stack and increments by 1.
*/
void RTS(CPU *cpu) {
    ; //operation implemented in cycle functions
}

/*
    RTI - Return from Interrupt
    Pulls the Status (P) register and Program Counter off top of the Stack.
    Increments S.
    PC being pulled is handled by cycle functions.
*/
void RTI(CPU *cpu) {
    cpu->P = cpu->DL;
    cpu->S++;
}

/*
    NOP - No operation
    Does nothing except increment PC which is done in opcode processing.
*/
void NOP(CPU *cpu) {
    ; //intention is that it does nothing
}

/*
    NOT
    Unimplemented opcode
*/
void NOT(CPU *cpu) {
    printf("ERROR: this instruction is not implemented\n");
}

