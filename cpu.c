#include "cpu.h"
#include <stdio.h>

void print_cpu(CPU *cpu) {
    char flags[8];
    for (int i = 7; i >= 0; i--) {
        flags[7-i] = cpu->P & (1<<i) ? '1' : '0';
    }
    printf("PC: %04x, AB: %04x, DB: %02x, IR: %02x, A: %02x, X: %02x, Y: %02x, P: %s, T: %u, DL: %02x, ALU: %02x, ACR: %02x\n",
            cpu->PC, cpu->AB, cpu->DB, cpu->IR, cpu->A, cpu->X, cpu->Y,
              flags, cpu->T, cpu->DL, cpu->ALU, cpu->ACR_FLAG);
}

void clocktick(CPU *cpu, BYTE *memory) {
    Operation operation;
    operation = decode(cpu);
    execute(cpu, operation, memory);
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


addressingmode(addr_brk, passarray({
    fetch_opcode,
    fetch_throw_brk,
    stack_push_PCH,
    stack_push_PCL,
    stack_push_register,
    fetch_PCL,
    fetch_PCH
}));

addressingmode(addr_immediate, passarray({
    fetch_opcode,
    imm_fetch_operand
}));

/*
    Same cycle functions for Accumulator addressing
*/
addressingmode(addr_implied, passarray({
    fetch_opcode,
    fetch_throw     //unlike in BRK, PC not incremented
}));

/*
    addressing mode for instructions pushing to stack
*/
addressingmode(addr_implied_push, passarray ({
    fetch_opcode,
    fetch_throw,
    stack_push_register
}));

/*
    addressing mode for instructions pulling from stack
*/
addressingmode(addr_implied_pull, passarray ({
    fetch_opcode,
    fetch_throw,
    increment_s,
    stack_pull_register
}));

addressingmode(addr_absolute_r, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr_exe   //executes operation
}));

addressingmode(addr_absolute_rmw, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr,
    modify, //zimmers describes as write value back and then do operation
    write_addr
}));

addressingmode(addr_absolute_w, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    write_register
}));

addressingmode(addr_zeropage_r, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_exe   //executes operation
}));

addressingmode(addr_zeropage_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_addr,
    modify,
    write_addr
}));

addressingmode(addr_zeropage_w, passarray({
    fetch_opcode,
    fetch_address,
    write_register
}));

addressingmode(addr_zpindex_X_r, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_X,
    read_addr
}));

addressingmode(addr_zpindex_Y_r, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_Y,
    read_addr
}));

addressingmode(addr_zpindex_X_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_X,
    read_addr,
    modify,
    write_addr
}));

addressingmode(addr_zpindex_Y_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_Y,
    read_addr,
    modify,
    write_addr
}));

addressingmode(addr_zpindex_X_w, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_X,
    write_register
}));

addressingmode(addr_zpindex_Y_w, passarray({
    fetch_opcode,
    fetch_address,
    read_addr_add_Y,
    write_register
}));

addressingmode(addr_absolute_X_r, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_X,
    read_addr_fixADH,
    read_addr_updated_exe
}));

addressingmode(addr_absolute_Y_r, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_Y,
    read_addr_fixADH_exe,
    read_addr_updated_exe
}));

addressingmode(addr_absolute_X_rmw, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_X,
    read_addr_fixADH,
    read_addr_updated,
    modify,
    write_addr
}));

addressingmode(addr_absolute_Y_rmw, passarray({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_Y,
    read_addr_fixADH,
    read_addr_updated,
    modify,
    write_addr
}));

addressingmode(addr_absolute_X_w, passarray ({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_X,
    read_addr,
    write_register_fixedADH
}));

addressingmode(addr_absolute_Y_w, passarray ({
    fetch_opcode,
    fetch_ADL,
    fetch_ADH_add_Y,
    read_addr,
    write_register_fixedADH //trace not quite accurate as accumulator should be dealt with before
}));

addressingmode(addr_relative, passarray({
    fetch_opcode,
    imm_fetch_operand,
    branch_PCL,
    branch_fixPCH
}));

addressingmode(addr_indexed_indirect_r, passarray ({
    fetch_opcode,
    fetch_address,
    read_ptr_add_X,
    fetch_ptrX_ADL,
    fetch_ptrX_ADH,
    read_addr_exe
}));

addressingmode(addr_indexed_indirect_rmw, passarray({
    fetch_opcode,
    fetch_address,
    read_ptr_add_X,
    fetch_ptrX_ADL,
    fetch_ptrX_ADH,
    read_addr,
    modify, //zimmers describes as write value back and then do operation
    write_addr
}));

addressingmode(addr_indexed_indirect_w, passarray({
    fetch_opcode,
    fetch_address,
    read_ptr_add_X,
    fetch_ptrX_ADL,
    fetch_ptrX_ADH,
    write_register
}));

addressingmode(addr_indirect_indexed_r, passarray({
    fetch_opcode,
    fetch_address,
    fetch_ptr_ADL,
    fetch_ptr_ADH_add_Y,
    read_addr_fixADH_exe,
    read_addr_updated_exe
}));

addressingmode(addr_indirect_indexed_rmw, passarray ({
    fetch_opcode,
    fetch_address,
    fetch_ptr_ADL,
    fetch_ptr_ADH_add_Y,
    read_addr_fixADH,
    read_addr_updated,
    modify,
    write_addr
}));

addressingmode(addr_indirect_indexed_w, passarray({
    fetch_opcode,
    fetch_address,
    fetch_ptr_ADL,
    fetch_ptr_ADH_add_Y,
    read_addr_fixADH,
    write_register_fixedADH
}));

addressingmode(absolute_indirect, passarray({ //doesn't handle page boundary crossing between PCL and PCH locations
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr,
    read_PCH
}));

Operation operations[] = {
    { BRK, &addr_brk },
    { CLC, &addr_implied },
    { LDX, &addr_immediate},
    { LDX, &addr_absolute_r},
    { ASL, &addr_absolute_rmw},
    { STA, &addr_absolute_w},
    { LDY, &addr_zeropage_r},
    { LSR, &addr_zeropage_rmw},
    { AND, &addr_absolute_X_r}
};


Operation decode(CPU *cpu) {
    Operation operation = operations[cpu->IR];
    if(cpu->T >= operation.mode->numsteps) {
        cpu->T = 0;
    }
    return operation;
}

void execute(CPU *cpu, Operation operation, BYTE *memory){
    operation.mode->step[cpu->T](cpu, memory, operation.ins);
}

/*
    Fetches opcode and loads it into Instruction Register.
    Increments Program Counter.
*/
void fetch_opcode(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB];
    // cpu->DB = memory[cpu->PC];
    cpu->DL = cpu->DB;
    cpu->IR = cpu->DL;
    cpu->PC++;
}

/*
    Reads next byte and throws it away.
    Used for implied and accumulator addressing.
    Does not increment Program Counter.
    Note BRK would increment PC
*/
void fetch_throw(CPU *cpu, BYTE *memory, Instruction *ins) {
    //emulate reading memory but doing nothing with it
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    ins(cpu);
    if(cpu->IR == 0xE8 || cpu->IR == 0xCA){ // update X reg after INX or DEX
        cpu->X = cpu->ALU;
    }
    if(cpu->IR == 0xC8 || cpu->IR == 0x88) {// update Y reg after INX or DEX
        cpu->Y = cpu->ALU;
    }
}

/*
    Reads next byte and throws it away.
    Used for implied accumulator addressing BRK instruction.
    Increments Program Counter.
*/
void fetch_throw_brk(CPU *cpu, BYTE *memory, Instruction *ins) {
    //emulate reading memory but doing nothing with it
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    cpu->PC++;
}

/*
    Fetches operand and holds it in CPU predecode register.
    Increments Program Counter.
*/
void imm_fetch_operand(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    ins(cpu);
    if(cpu->ACR_FLAG == 0) { //for branch instructions check if branch triggered
        cpu->T++; //skip branch next step
        cpu->T++; //skip potential second next step to fix PCH if page boundary crossed
        cpu->PC++; //if branch not triggered or not a branch instruction just increment PC
    }
}

void branch_PCL(CPU *cpu, BYTE *memory, Instruction *ins) {
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

void branch_fixPCH(CPU *cpu, BYTE *memory, Instruction *ins) {
    if(cpu->DL < 0x80) {
        cpu->PC += 0x0100; //correct for positive offset page cross
    } else {
        cpu->PC -= 0x0100; //correct for negative offset page cross
    }
}

void stack_push_PCH(CPU *cpu, BYTE *memory, Instruction *ins) {
    // BYTE PCH = cpu->PC >> 8;
    cpu->DB = cpu->PC >> 8;
    cpu->DL = cpu->DB;
    // memory[STACK_BASE + cpu->S] = cpu->DB;
    cpu->AB = STACK_BASE + cpu->S;
    memory[cpu->AB] = cpu->DL;
    cpu->S--;
}

void stack_push_PCL(CPU *cpu, BYTE *memory, Instruction *ins) {
    // BYTE PCL = cpu->PC;
    cpu->DB = cpu->PC;
    cpu->DL = cpu->DB;
    cpu->AB = STACK_BASE + cpu->S;
    memory[cpu->AB] = cpu->DL;
    // cpu->AB = STACK_BASE + cpu->S;
    // memory[cpu->AB] = cpu->DB;
    cpu->S--;
}

void stack_push_register(CPU *cpu, BYTE *memory, Instruction *ins) {
    ins(cpu);
    cpu->DL = cpu->DB;
    cpu->AB = STACK_BASE + cpu->S;
    memory[cpu->AB] = cpu->DL;
    cpu->S--;
}

void stack_pull_register(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->AB = STACK_BASE + cpu->S;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    ins(cpu);
}

void increment_s(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->S++;
}

void fetch_PCL(CPU *cpu, BYTE *memory, Instruction *ins) {
    // cpu->PC = memory[IRQ_LOW];
    cpu->AB = IRQ_LOW;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    cpu->PC = cpu->DL;
    //check which cycle this is set
    setFlag(cpu, FLAG_I);
}

void fetch_PCH(CPU *cpu, BYTE *memory, Instruction *ins) {
    // cpu->PC += (memory[IRQ_HIGH] << 8);
    cpu->AB = IRQ_HIGH;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    cpu->PC += cpu->DL << 8;
}

void fetch_ADL(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    cpu->PC++;
}

void fetch_ADH(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB];
    cpu->ALU = cpu->DL; //start of cycle cpu->DL is ADL which is stored in ALU
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->PD is ADH
    cpu->PC++;
}

void fetch_address(CPU *cpu, BYTE *memory, Instruction *ins){ //***check as this reads address held in PC
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB; //at end of cycle DL holds contents of address stored in PC to be used as address in next cycle
    cpu->PC++;
}

void read_addr_exe(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    ins(cpu);
}

void read_zp_addr_exe(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->DL;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    ins(cpu);
}

void read_zp_addr(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->DL; //in previous cycle effective address stored in DL
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
}

void read_addr(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU; //in previous cycle address stored in ALU
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
}

void read_PCH(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->AB + 1;
    cpu->DB = memory[cpu->AB];
    cpu->PC = (cpu->DB << 8) + cpu->DL;
    cpu->DL = cpu->DB;
}

void modify(CPU *cpu, BYTE *memory, Instruction *ins) {
    //memory[cpu->AB] = cpu->DB writes unmodified value again first
    ins(cpu); 
}

void write_addr(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->DB = cpu->ALU;
    cpu->DL = cpu->DB;
    memory[cpu->AB] = cpu->DL; //write modified value
}

void write_register(CPU *cpu, BYTE *memory, Instruction *ins) {
    ins(cpu);
    cpu->DL = cpu->DB;
    memory[cpu->AB] = cpu->DL; //write register value returned by instruction
}

void read_addr_add_X(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->DB = cpu->X;
    cpu->ALU = cpu->DL + cpu->DB; //page boundary crossings not handled
    cpu->DL = cpu->DB;
    cpu->AB = cpu->ALU; //check this is happening at right point
}

void read_addr_add_Y(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->DB = cpu->Y;
    cpu->ALU = cpu->DL + cpu->DB; //page boundary crossings not handled
    cpu->DL = cpu->DB;
    cpu->AB = cpu->ALU;
}

void fetch_ADH_add_X(CPU *cpu, BYTE *memory, Instruction *ins){
    //at the start of cycle cpu->DL holds ADL
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB]; //DB is ADH
    cpu->ALU = cpu->DL + cpu->X; // add ADL and X
    if(cpu->ALU < cpu->X) { //set ALU carry flag if page boundary crossed
        cpu->ACR_FLAG = 1;
    } else {
        cpu->ACR_FLAG = 0;
    }
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->DL is ADH
    cpu->PC++;
}
void fetch_ADH_add_Y(CPU *cpu, BYTE *memory, Instruction *ins){
    //start of cycle cpu->DL is ADL
    cpu->AB = cpu->PC;
    cpu->DB = memory[cpu->AB]; //DB is ADH
    cpu->ALU = cpu->DL + cpu->Y; // add ADL and X
    if(cpu->ALU < cpu->Y) { //set ALU carry flag if page boundary crossed
        cpu->ACR_FLAG = 1;
    } else {
        cpu->ACR_FLAG = 0;
    }
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->DL is ADH
    cpu->PC++;
}

void read_addr_fixADH_exe(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU;
    cpu->DB = memory[cpu->AB];
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

void read_addr_updated_exe(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->AB = (cpu->ALU << 8) + (cpu->AB & 0xff); //updated address
    cpu->ALU = 0x00;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
    ins(cpu);
}

void read_addr_fixADH(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = (cpu->DL << 8) + cpu->ALU;
    cpu->DB = memory[cpu->AB];
    if(cpu->ACR_FLAG != 0) {
        cpu->ALU = cpu->DL + 1; //fixed ADH value including page boundary cross
        cpu->ACR_FLAG = 0;
    } else {
        cpu->T++; //skip next step if additional cycle not needed
    }
    cpu->DL = cpu->DB;
}

void read_addr_updated(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->AB = (cpu->ALU << 8) + (cpu->AB & 0xff); //updated address
    cpu->ALU = 0x00;
    cpu->DB = memory[cpu->AB];
    cpu->DL = cpu->DB;
}

// void read_addr_fixADH_w(CPU *cpu, BYTE *memory, Instruction *ins){
//     cpu->AB = (cpu->DB << 8) + cpu->ALU;
//     cpu->DB = memory[cpu->AB];
//     if(cpu->ACR_FLAG != 0) {
//         cpu->ALU = cpu->DL + 1; //fixed ADH value including page boundary cross
//         cpu->ACR_FLAG = 0;
//         cpu->AB = (cpu->ALU << 8) + (cpu->AB & 0xff); //updated address for write - trace will only display updated address
//     }
//     cpu->DL = cpu->DB;
// }

void write_register_fixedADH(CPU *cpu, BYTE *memory, Instruction *ins) {
    if(cpu->ACR_FLAG != 0) { //technically ALU and flag should be reset by time write starts
        cpu->ALU = (cpu->AB >> 8) + 1;
        cpu->ACR_FLAG = 0;
        cpu->AB = (cpu->ALU << 8) + (cpu->AB & 0xff);
        //cpu->AB = cpu->AB + 0x0100; alternative
    }
    ins(cpu);
    cpu->DL = cpu->DB;
    memory[cpu->AB] = cpu->DL; //write register value returned by instruction
}

void read_ptr_add_X(CPU *cpu, BYTE *memory, Instruction *ins) {
    cpu->AB = cpu->DL;
    cpu->DB = memory[cpu->AB];
    cpu->ALU = cpu->DL + cpu->X; //ALU holds address (ptr fetched in previous cycle + X) doesn't deal with crossing page boundaries
    cpu->DL = cpu->DB;
}

void fetch_ptrX_ADL(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->ALU; //ptr + X
    cpu->DB = memory[cpu->AB];
    cpu->ALU = cpu->ALU + 1; //ptr + X + 1
    cpu->DL = cpu->DB; //DL is ADL
}

void fetch_ptrX_ADH(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->ALU; //ALU holds ptr + X + 1
    cpu->DB = memory[cpu->AB]; //DB is AHL, DL still holds ADL
    cpu->ALU = cpu->DB; //store ADL in ALU
    cpu->DL = cpu->DB; //end of cycle/start of next cycle cpu->DL is ADH
}
void fetch_ptr_ADL(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->DL;
    cpu->DB = memory[cpu->AB];
    cpu->ALU = cpu->DL + 1;
    cpu->DL = cpu->DB; //DL is ADL
}

void fetch_ptr_ADH_add_Y(CPU *cpu, BYTE *memory, Instruction *ins){
    cpu->AB = cpu->ALU;
    cpu->DB = memory[cpu->AB]; //DB is AHL
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
}

/*
    INY - increment Y register by 1
    Z and N flags set depending on result.
*/
void INY(CPU *cpu) {
    increment(cpu, cpu->Y);
}

void decrement(CPU *cpu, BYTE *target) {
    cpu->ALU = *target - 1;
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
    decrement(cpu, &cpu->DL);
}

/*
    DEX - decrement X register by 1
    Z and N flags set depending on result.
*/
void DEX(CPU *cpu) {
    decrement(cpu, &cpu->X);
}

/*
    DEY - decrement Y register by 1
    Z and N flags set depending on result.
*/
void DEY(CPU *cpu) {
    decrement(cpu, &cpu->Y);
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
    cpu->DB = cpu->DL << 1;
    //sets 0 bit
    cpu->DB = cpu->DB | mask;
    if (cpu->DB == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->DB, FLAG_N) != 0) {
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
    cpu->DB = cpu->DL >> 1;
    //sets 0 bit
    cpu->DB = cpu->DB | mask;
    if (cpu->DB == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        clearFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->DB, FLAG_N) != 0) {
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

// /*
//     Helper function - pulls register from stack
//     Increments stack pointer before pulling contents
// */
// void pull_from_stack(CPU *cpu, BYTE *memory, BYTE *reg) {
//     cpu->S++;
//     *reg = memory[STACK_BASE + cpu->S];
// }

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
void JSR(CPU *cpu, BYTE *memory) {
    // implemented in cycle functions
    // push_to_stack(cpu, memory, (cpu->PC >> 8));
    // push_to_stack(cpu, memory, cpu->PC);
    // BYTE high = memory[cpu->PC + 1];
    // BYTE low = memory[cpu->PC + 2];;
    // uint16_t address = high << 8;
    // address += low;
    // cpu->PC = address;
}

/*
    RTS - Return from Subroutine
    Pulls the Program Counter from the stack and increments by 1.
*/
// void RTS(CPU *cpu, BYTE *memory) {
//     BYTE lowByte;
//     BYTE highByte;
//     pull_from_stack(cpu, memory, &lowByte);
//     pull_from_stack(cpu, memory, &highByte);
//     cpu->PC = highByte << 8;
//     cpu->PC = cpu->PC + lowByte + 1;
// }

/*
    RTI - Return from Interrupt
    Pulls the Status (P) register and Program Counter off top of the Stack.
    Relies on Stack Pointer pointing to correct position in stack.
*/
// void RTI(CPU *cpu, BYTE *memory) {
//     pull_from_stack(cpu, memory, &cpu->P);
//     BYTE lowByte;
//     BYTE highByte;
//     pull_from_stack(cpu, memory, &lowByte);
//     pull_from_stack(cpu, memory, &highByte);
//     cpu->PC = highByte << 8;
//     cpu->PC = cpu->PC + lowByte;
// }

/*
    NOP - No operation
    Does nothing except increment PC which is done in opcode processing.
*/
void NOP(CPU *cpu) {
    ; //intention is that it does nothing
}
