#include "cpu.h"

BYTE fetch(CPU *cpu, BYTE (*memory)[]) {
    BYTE operand = (*memory)[cpu->PC];
    return operand;
}

void power_cpu(CPU *cpu) {
    cpu->P = cpu->A = cpu->X = cpu->Y = 0x00u;
    cpu->PC = 0xFFFCu;
    cpu->S = 0xFFu;
    setFlag(cpu, FLAG_I);
    //clear internal RAM except high scores $0000-$07FF
}

void setFlag(CPU *cpu, int position) {
    cpu->P = cpu->P | (FLAG_MASK << position);
}

void resetFlag(CPU *cpu, int position) {
    cpu->P = cpu->P & ~(FLAG_MASK << position);
}

BYTE getBit(BYTE source, int position) {
    unsigned int bit = (source >> position) & FLAG_MASK;
    return bit;
}

void increment(CPU *cpu, BYTE *memory) {
    *memory = *memory + 1;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    INC - increment memory by 1
    Z and N flags set depending on result.
*/
void INC(CPU *cpu, BYTE *memory) {
    increment(cpu, memory);
}

/*
    INX - increment X register by 1
    Z and N flags set depending on result.
*/
void INX(CPU *cpu) {
    increment(cpu, &cpu->X);
}

/*
    INY - increment Y register by 1
    Z and N flags set depending on result.
*/
void INY(CPU *cpu) {
    increment(cpu, &cpu->Y);
}

void decrement(CPU *cpu, BYTE *memory) {
    *memory = *memory - 1;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    DEC - decrement memory by 1
    Z and N flags set depending on result.
*/
void DEC(CPU *cpu, BYTE *memory) {
    decrement(cpu, memory);
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
void LDA(CPU *cpu, BYTE *memory) {
    cpu->A = *memory;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    LDX - load a byte of memory into X register
    Z and N flags set depending on result.
*/
void LDX(CPU *cpu, BYTE *memory) {
    cpu->X = *memory;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    LDY - load a byte of memory into Y register
    Z and N flags set depending on result.
*/
void LDY(CPU *cpu, BYTE *memory) {
    cpu->Y = *memory;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    ASL - Arithmetic Shift Left
    Shifts bits in A or memory left by one place.
    Carry flag set to contents of old 7 bit.
    Sets Zero and Negative flag based on result.
*/
void ASL(CPU *cpu, BYTE *memory) {
    if (getBit(*memory, 7) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        resetFlag(cpu, FLAG_C);
    }
    *memory = *memory << 1;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    LSR - logical shift right
    Shifts bits in A or memory right by one place. 
    Carry flag set to contents of old 0 bit.
    Sets Zero and Negative flag based on result.
*/
void LSR(CPU *cpu, BYTE *memory) {
    if (getBit(*memory, 0) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        resetFlag(cpu, FLAG_C);
    }
    *memory = *memory >> 1;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0){
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }

}

/*
    ROL - Rotate Left
    Shifts bits in A or memory left by one place.
    Bit 0 is set to contents of Carry flag.
    Carry flag set to contents of old 7 bit.
    Sets Zero and Negative flag based on result.
*/
void ROL(CPU *cpu, BYTE *memory) {
    BYTE mask;
    if(getBit(cpu->P, FLAG_C) != 0) {
        mask = FLAG_MASK;
    } else {
        mask = 0x0;
    }
    if (getBit(*memory, LEFT_BIT) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        resetFlag(cpu, FLAG_C);
    }
    *memory = *memory << 1;
    //sets 0 bit
    *memory = *memory | mask;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    ROR - Rotate Right
    Shifts bits in A or memory right by one place.
    Bit 7 is set to contents of Carry flag.
    Carry flag set to contents of old 0 bit.
    Sets Zero and Negative flag based on result.
*/
void ROR(CPU *cpu, BYTE *memory) {
    BYTE mask;
    if(getBit(cpu->P, FLAG_C) != 0) {
        mask = FLAG_MASK << LEFT_BIT;
    } else {
        mask = 0x0;
    }
    if (getBit(*memory, RIGHT_BIT) != 0) {
        setFlag(cpu, FLAG_C);
    } else {
        resetFlag(cpu, FLAG_C);
    }
    *memory = *memory >> 1;
    //sets 0 bit
    *memory = *memory | mask;
    if (*memory == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    STA - Store Accumulator
    Store contents of A into memory.
*/
void STA(CPU *cpu, BYTE *memory) {
    *memory = cpu->A;
}

/*
    STX - Store X Register
    Store contents of X into memory.
*/
void STX(CPU *cpu, BYTE *memory) {
    *memory = cpu->X;
}

/*
    STY - Store Y Register
    Store contents of Y into memory.
*/
void STY(CPU *cpu, BYTE *memory) {
    *memory = cpu->Y;
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
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->X, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
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
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->Y, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
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
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->X, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
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
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
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
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    AND - AND memory and Accumulator
    Bitwise AND performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void AND(CPU *cpu, BYTE *memory) {
    cpu->A = cpu->A & *memory;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    EOR - Exclusive OR
    Bitwise Exclusive Or performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void EOR(CPU *cpu, BYTE *memory) {
    cpu->A = cpu->A ^ *memory;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    ORA - Inclusive OR
    Bitwise OR performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void ORA(CPU *cpu, BYTE*memory) {
    cpu->A = cpu->A | *memory;
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(cpu->A, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
}

/*
    BIT - Bit Test
    Tests if bits set in memory location based on Accumulator.
    ANDs Accumulator and memory location but does not store results.
    Condition flags are set according to AND result (z)
    and memory contents (N and V).
*/
void BIT(CPU *cpu, BYTE *memory) {
    if((cpu->A & *memory) == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
    if (getBit(*memory, FLAG_V) != 0) {
        setFlag(cpu, FLAG_V);
    } else {
        resetFlag(cpu, FLAG_V);
    }
}

/*
    Helper function - pushes register to stack and decrements Stack Pointer
    by 1 to next position on Stack
*/
void push_to_stack(CPU *cpu, BYTE *memory, BYTE reg) {
    memory[STACK_BASE + cpu->S] = reg;
    cpu->S--;
}


/*
    Push - Accumulator
    Stores contents of the Accumulator on top of the stack.
    Then decrements Stack Pointer by 1.
    No other registers/statuses are affected.
*/
void PHA(CPU *cpu, BYTE *memory) {
    push_to_stack(cpu, memory, cpu->A);
}

/*
    Push - Processor Status
    Stores contents of Processor Status on stack.
    Then decrements Stack Pointer by 1.
    No other registers/statuses are affected.
*/
void PHP(CPU *cpu, BYTE *memory) {
    push_to_stack(cpu, memory, cpu->P);
}

/*
    Helper function - pulls register from stack
    Increments stack pointer before pulling contents
*/
void pull_from_stack(CPU *cpu, BYTE *memory, BYTE *reg) {
    cpu->S++;
    *reg = memory[STACK_BASE + cpu->S];
}

/*
    Pull accumulator from stack
    Increments Stack Pointer by 1.
    Then load accumulator from Top of stack.
    Sets Zero flag and negative flag based on A.
*/
void PLA(CPU *cpu, BYTE *memory) {
    pull_from_stack(cpu, memory, &cpu->A);
    if (getBit(cpu->A, FLAG_N)) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
    if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
}

/*
    Pull processor status from stack
    Increments Stack Pointer by 1.
    Then load processor status from top of stack.
*/
void PLP(CPU *cpu, BYTE *memory) {
    pull_from_stack(cpu, memory, &cpu->P);
}

/*
    ADC - Add with Carry
    Adds contents of memory to accumulator with carry value.
    Carry flag is set if overflow occurs.
    Z flag set if result is 0.
    N flag set if result is negative.
    V flag set if sign bit is incorrect.
*/
void ADC(CPU *cpu, BYTE *memory) {
    BYTE HIGH_BIT_MASK = 0x80;
    int sum = *memory + cpu->A + getBit(cpu->P, FLAG_C);
    if((cpu->A ^ sum) & (*memory ^ sum) & HIGH_BIT_MASK) {
        setFlag(cpu, FLAG_V);
    } else {
        resetFlag(cpu, FLAG_V);
    }
    cpu->A = sum;
    //check if unsigned sum larger than 255 to determine if carry set
    if(sum > 0xFF) {
        setFlag(cpu, FLAG_C);
    } else {
        resetFlag(cpu, FLAG_C);
    }
    if (getBit(cpu->A, FLAG_N)) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
     if (cpu->A == 0) {
        setFlag(cpu, FLAG_Z);
    } else {
        resetFlag(cpu, FLAG_Z);
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
void SBC(CPU *cpu, BYTE *memory) {
   BYTE mem_complement = ~(*memory);
   ADC(cpu, &mem_complement);
}

/*
    Compare helper function
    Compares given register with memory value and sets flags accordingly.
*/
void set_flags_on_compare(CPU *cpu, BYTE reg, BYTE *memory) {
    BYTE result = reg - *memory;
    if (getBit(result, FLAG_N)) {
        setFlag(cpu, FLAG_N);
    } else {
        resetFlag(cpu, FLAG_N);
    }
    if (reg == *memory) {
        setFlag(cpu, FLAG_Z);
        setFlag(cpu, FLAG_C);
    } else {
        resetFlag(cpu, FLAG_Z);
    }
    if (reg > *memory) {
        setFlag(cpu, FLAG_C);
    } else if (reg < *memory) {
        resetFlag(cpu, FLAG_C);
    }
}

/*
    Compare memory and accumulator
    Subtracts contents of memory from accumulator.
    Z Flag is set if A = M otherwise reset.
    N Flag is set by result bit 7.
    C when memory less than or equal to accumulator, reset if M greater than A

*/
void CMP(CPU *cpu, BYTE *memory) {
    set_flags_on_compare(cpu, cpu->A, memory);
}

/*
    Compare memory and X register
    Subtracts contents of memory from X register.
    Z is set if X = M otherwise reset.
    N is set by result bit 7.
    C when memory less than or equal to X, reset if M greater than X.

*/
void CPX(CPU *cpu, BYTE *memory) {
    set_flags_on_compare(cpu, cpu->X, memory);
}

/*
    Compare memory and Y register
    Subtracts contents of memory from Y register.
    Z is set if Y = M otherwise reset.
    N is set by result bit 7.
    C when memory less than or equal to Y, reset if M greater than Y.

*/
void CPY(CPU *cpu, BYTE *memory) {
    set_flags_on_compare(cpu, cpu->Y, memory);
}

/*
    Clear carry flag
    Sets C flag to 0
*/
void CLC(CPU *cpu) {
    resetFlag(cpu, FLAG_C);
}

/*
    Clear decimal mode
    Sets D flag to 0
    Not used in NES (NES doesn't use decimal mode)
*/
void CLD(CPU *cpu) {
    resetFlag(cpu, FLAG_D);
}

/*
    Clear interrupt disable bit
    Sets I flag to 0
*/
void CLI(CPU *cpu) {
    resetFlag(cpu, FLAG_I);
}

/*
    Clear overflow flag
    Sets V flag to 0
*/
void CLV(CPU *cpu) {
    resetFlag(cpu, FLAG_V);
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
void BCC(CPU *cpu, BYTE displacement) {
    if(getBit(cpu->P, FLAG_C) == 0) {
        cpu->PC += displacement;
    }
}

/*
    BCS - Branch if Carry Flag Set
    If C flag set relative displacement added to PC to cause branch.
*/
void BCS(CPU *cpu, BYTE displacement){
    if(getBit(cpu->P, FLAG_C) != 0) {
        cpu->PC += displacement;
    }
}

/*
    BEQ - Branch if Equal
    If Z flag set relative displacement added to PC to cause branch.
*/
void BEQ(CPU *cpu, BYTE displacement) {
    if(getBit(cpu->P, FLAG_Z) != 0) {
        cpu->PC += displacement;
    }
}

/*
    BMI - Branch if Minus
    If N flag set relative displacement added to PC to cause branch.
*/
void BMI(CPU *cpu, BYTE displacement) {
    if(getBit(cpu->P, FLAG_N) != 0) {
        cpu->PC += displacement;
    }
}

/*
    BNE - Branch if Not Equal
    If Z flag clear relative displacement added to PC to cause branch.
*/
void BNE(CPU *cpu, BYTE displacement) {
    if(getBit(cpu->P, FLAG_Z) == 0) {
        cpu->PC += displacement;
    }
}

/*
    BPL - Branch if Positive
    If N flag clear relative displacement added to PC to cause branch.
*/
void BPL(CPU *cpu, BYTE displacement) {
    if(getBit(cpu->P, FLAG_N) == 0) {
        cpu->PC += displacement;
    }
}

/*
    BVC - Branch if Overflow Clear
    If V flag clear relative displacement added to PC to cause branch.
*/
void BVC(CPU *cpu, BYTE displacement) {
    if(getBit(cpu->P, FLAG_V) == 0) {
        cpu->PC += displacement;
    }
}

/*
    BVS - Branch if Overflow Set
    If V flag set relative displacement added to PC to cause branch.
*/
void BVS(CPU *cpu, BYTE displacement) {
    if(getBit(cpu->P, FLAG_V) != 0) {
        cpu->PC += displacement;
    }
}

/*
    JMP - Jumps
    Sets PC to target address set in instruction operand.
*/
void JMP(CPU *cpu, BYTE low, BYTE high) {
    uint16_t address = high << 8;
    address += low;
    cpu->PC = address;
}

/*
    JSR - Jump to Subroutine
    Pushes PC onto the Stack. Stack Pointer adjusted as part of this.
    High byte is stored first.
    Relies on Opcode process calling this function to have incremented PC by 2
    before this function is called.
    Address part of instruction then stored in PC.    
*/
void JSR(CPU *cpu, BYTE *memory, BYTE low, BYTE high) {
    push_to_stack(cpu, memory, (cpu->PC >> 8));
    push_to_stack(cpu, memory, cpu->PC);
    uint16_t address = high << 8;
    address += low;
    cpu->PC = address;
}

/*
    RTS - Return from Subroutine
    Pulls the Program Counter from the stack and increments by 1.
*/
void RTS(CPU *cpu, BYTE *memory) {
    BYTE lowByte;
    BYTE highByte;
    pull_from_stack(cpu, memory, &lowByte);
    pull_from_stack(cpu, memory, &highByte);
    cpu->PC = highByte << 8;
    cpu->PC = cpu->PC + lowByte + 1;
}

/*
    RTI - Return from Interrupt
    Pulls the Status (P) register and Program Counter off top of the Stack.
    Relies on Stack Pointer pointing to correct position in stack.
*/
void RTI(CPU *cpu, BYTE *memory) {
    pull_from_stack(cpu, memory, &cpu->P);
    BYTE lowByte;
    BYTE highByte;
    pull_from_stack(cpu, memory, &lowByte);
    pull_from_stack(cpu, memory, &highByte);
    cpu->PC = highByte << 8;
    cpu->PC = cpu->PC + lowByte;
}

/*
    BRK - Break
    Assumes Opcode process has already incremented PC by 2.
    B flag set.
    PC and P pushed to stack.
    I flag set.
    Interrupt Pointer ($FFFF and $FFFE) loaded into PC.
*/
void BRK(CPU *cpu, BYTE *memory) {
    setFlag(cpu, FLAG_B);
    BYTE lowByte;
    BYTE highByte;
    highByte = cpu->PC >> 8;
    lowByte = cpu->PC;
    push_to_stack(cpu, memory, highByte);
    push_to_stack(cpu, memory, lowByte);
    push_to_stack(cpu, memory, cpu->P);

    setFlag(cpu, FLAG_I);
    //Interrupt pointer $FFFE and $FFFF loaded into PC
    cpu->PC = (memory[IRQ_HIGH] << 8) + memory[IRQ_LOW];
}

/*
    NOP - No operation
    Does nothing except increment PC which is done in opcode processing.
*/
void NOP() {
    ; //intention is that it does nothing
}
