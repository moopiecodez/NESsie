#include <stdio.h>
#include <stdint.h>
#include "cpu.h"

/* 
    Addressing modes:
    Memory - immediate - 2 bytes second operand in second byte
    Memory - absolute or direct non-zero-page - 2-3 bytes
    Memory - zero page (direct)
    Implied or inherent - 1 byte no further operand required
    Accumulator - 1 byte operates on data in accumulator
    Pre-indexed indirect - 2 bytes 2nd byte added to X register to access memory
    Post-indexed indirect - location in 2nd byte contains address added to y to find full address
    Zero page, indexed (aka base page, indexed)
    Absolute indexed - contains full 16 bits address
    Relative - contains a signed 8 bit offset for branch if condition true

    Indirect - JMP contains 16 bit address identifying least sig byte of another
        16 bit address address which is target

*/

/*
    Accumulator - acts on the accumulator. One byte instruction
*/
BYTE *A_addressing(CPU *cpu, BYTE *memory) {
    //memory not used but done for consistency
    incrementPC(cpu);
    return &cpu->A;
}

/*
    # - Immediate addressing: operand is contained in the second byte 
    of the instruction. No additional memory addressing required.
*/
BYTE *imm(CPU *cpu, BYTE *memory) {
    //do stuff to extract constant;
    incrementPC(cpu);
    BYTE byte = fetch(cpu, memory);
    incrementPC(cpu);
    //note this is not pointer to operand's location in memory
    //converted to pointer for function return type consistency
    BYTE *operand = &byte;
    return operand;
}

/*
    Absolute - load contents of an absolute address
    Second Byte specifies low order bits and third specifies high order bits.
    Function returns pointer to absolute address
*/
BYTE *absolute(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE lowBits = fetch(cpu, memory);
    incrementPC(cpu);
    BYTE highBits = fetch(cpu,memory);
    incrementPC(cpu);
    uint16_t effectiveAddress = (highBits << 8) + lowBits;
    BYTE *address = &memory[effectiveAddress];
    return address;
}

/*
    Zero page - second byte contains lower order bits of address on zero page.
    Contents of second byte added to $00 as high order bits to give address 
    between $0000-00FF.
    Returns a pointer to the effective address.
*/
BYTE *zp(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE lowBits = fetch(cpu, memory);
    incrementPC(cpu);
    BYTE *address = &memory[ZERO_PAGE + lowBits];
    return address;
}

/*
    zp,X - indexed zero page addressing X
    Form of zero page addressing. Address calculated by adding second byte of
    instruction to contents of the X register. References a location on zero 
    page ($00).
    Crossing of page boundaries does not occur.
*/
BYTE *zpX(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE lowBits = fetch(cpu, memory);
    incrementPC(cpu);
    lowBits += cpu->X;
    BYTE *address = &memory[ZERO_PAGE + lowBits];
    return address;
}

/*
    zp,Y - indexed zero page addressing Y
    Form of zero page addressing. Address calculated by adding second byte of
    instruction to contents of the Y register. References a location on zero 
    page ($00).
    Crossing of page boundaries does not occur.
*/
BYTE *zpY(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE lowBits = fetch(cpu, memory);
    incrementPC(cpu);
    lowBits += cpu->Y;
    BYTE *address = &memory[ZERO_PAGE + lowBits];
    return address;
}

/*
    a,x - indexed absolute addressing X
    Effective address formed by adding contents of X to address contained in
    second and third bytes of the instruction.
*/
BYTE *absX(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE lowBits = fetch(cpu, memory);
    incrementPC(cpu);
    BYTE highBits = fetch(cpu,memory);
    uint16_t effectiveAddress = (highBits << 8) + lowBits;
    effectiveAddress += cpu->X;
    incrementPC(cpu);
    BYTE *address = &memory[effectiveAddress];
    return address;
}

/*
    a,y - indexed absolute addressing Y
    Effective address formed by adding contents of Y to address contained in
    second and third bytes of the instruction.
*/
BYTE *absY(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE lowBits = fetch(cpu, memory);
    incrementPC(cpu);
    BYTE highBits = fetch(cpu,memory);
    uint16_t effectiveAddress = (highBits << 8) + lowBits;
    effectiveAddress += cpu->Y;
    incrementPC(cpu);
    BYTE *address = &memory[effectiveAddress];
    return address;
}

/*
    Implied - aka inherent, implied addressing
    Address containing operand is implicitly stated in operation code of the
    instruction.
    Second parameter added for consistency in function declarations.
    Returns null pointer for consistency in function declarations.
*/
BYTE *imp(CPU *cpu, BYTE *memory) {
    //increment PC after fetching opcode
    incrementPC(cpu);
    BYTE *address = NULL;
    return address;
}

/*
    Relative - label relative addressing
    Only used in branch instructions.
    Second byte of the instruction is a signed offset added to lower bits of PC 
    when PC is set at next instruction.
    Returns null pointer for consistency in function declarations.
*/
BYTE *rel(CPU *cpu, BYTE *memory) {
    //test to check that offset works correctly on unsigned register
    incrementPC(cpu);
    int8_t offset = (int8_t)fetch(cpu, memory);
    incrementPC(cpu);
    cpu->PC = cpu->PC + offset;
    BYTE *address = NULL;
    return address;
}

/*
    (zp,X) - Indexed indirect addressing
    Second byte of instruction is added to contents of the X register
    discarding the carry.
    Result points to a memory location on page zero whose contents is the low
    order eight bits of the effective address.
    Next memory location in page zero contains high order eight bits of effective
    address. Both memory locations specifying high and low order eight bits must
    be in page zero.
*/
BYTE *indirectX(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE zpLocation = fetch(cpu, memory);
    incrementPC(cpu);
    zpLocation += cpu->X;
    BYTE lowBits = memory[ZERO_PAGE + zpLocation];
    BYTE highBits = memory[ZERO_PAGE + zpLocation + 1];
    uint16_t effectiveAddress = (highBits << 8) + lowBits;
    BYTE *address = &memory[effectiveAddress];
    return address;
}

/*
    (zp),Y - Indirect indexed addressing
    Second byte of instruction points to memory location in page zero. The
    contents of this location is added to Y register giving the low order eight
    bits of effective address.
    Carry from the addition is added to contents of the next zero page memory
    location. The result is the high order eight bits of effective address.
*/
BYTE *indirectY(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE zpLocation = fetch(cpu, memory);
    incrementPC(cpu);
    BYTE lowBits = memory[ZERO_PAGE + zpLocation];
    uint16_t sum = (uint16_t)lowBits + cpu->Y;
    BYTE carry = (sum >> 8) & FLAG_MASK;
    lowBits = (BYTE)sum;
    BYTE highBits = memory[ZERO_PAGE + zpLocation + 1];
    highBits += carry;
    uint16_t effectiveAddress = (highBits << 8) + lowBits;
    BYTE *address = &memory[effectiveAddress];
    return address;
}

/*
    Absolute Indirect
    Second byte of instruction contains low order bits to memory location.
    Third byte of instruction contains high order bits to memory location.
    Contents of fully specified location is low order bits of effective address
    Memory location +1 contains high order bits of effective address.
    Effective address loaded into PC.
*/
void absIndirect(CPU *cpu, BYTE *memory) {
    incrementPC(cpu);
    BYTE addressLowBits = fetch(cpu, memory);
    incrementPC(cpu);
    BYTE addressHighBits = fetch(cpu, memory);
    incrementPC(cpu);
    uint16_t address = (addressHighBits << 8) + addressLowBits;
    BYTE lowBits = memory[address];
    BYTE highBits = memory[address + 1];
    uint16_t PCupdate = (highBits << 8) + lowBits;
    cpu->PC = PCupdate;
}


