#include "cpu.h"

CPU cpu = {0x0000u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

void power_cpu() {
    cpu.P = cpu.A = cpu.X = cpu.Y = 0x00u;
    cpu.PC = 0xFFFCu;
    cpu.S = 0xFFu;
    setFlag(FLAG_I);
    //clear internal RAM except high scores $0000-$07FF
}

void setFlag(int position) {
    cpu.P = cpu.P | (FLAG_MASK << position);
}

void resetFlag(int position) {
    cpu.P = cpu.P & ~(FLAG_MASK << position);
}

BYTE getBit(BYTE source, int position) {
    unsigned int bit = (source >> position) & FLAG_MASK;
    return bit;
}

void increment(BYTE *memory) {
    *memory = *memory + 1;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    INC - increment memory by 1
    Z and N flags set depending on result.
*/
void INC(BYTE *memory) {
    increment(memory);
}

/*
    INX - increment X register by 1
    Z and N flags set depending on result.
*/
void INX() {
    increment(&cpu.X);
}

/*
    INY - increment Y register by 1
    Z and N flags set depending on result.
*/
void INY() {
    increment(&cpu.Y);
}

void decrement(BYTE *memory) {
    *memory = *memory - 1;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    DEC - decrement memory by 1
    Z and N flags set depending on result.
*/
void DEC(BYTE *memory) {
    decrement(memory);
}

/*
    DEX - decrement X register by 1
    Z and N flags set depending on result.
*/
void DEX() {
    decrement(&cpu.X);
}

/*
    DEY - decrement Y register by 1
    Z and N flags set depending on result.
*/
void DEY() {
    decrement(&cpu.Y);
}
/*
    LDA - load a byte of memory into accumulator
    Z and N flags set depending on result.
*/
void LDA(BYTE *memory) {
    cpu.A = *memory;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    LDX - load a byte of memory into X register
    Z and N flags set depending on result.
*/
void LDX(BYTE *memory) {
    cpu.X = *memory;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    LDY - load a byte of memory into Y register
    Z and N flags set depending on result.
*/
void LDY(BYTE *memory) {
    cpu.Y = *memory;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    ASL - Arithmetic Shift Left
    Shifts bits in A or memory left by one place.
    Carry flag set to contents of old 7 bit.
    Sets Zero and Negative flag based on result.
*/
void ASL(BYTE *memory) {
    if (getBit(*memory, 7) != 0) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    *memory = *memory << 1;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    LSR - logical shift right
    Shifts bits in A or memory right by one place. 
    Carry flag set to contents of old 0 bit.
    Sets Zero and Negative flag based on result.
*/
void LSR(BYTE *memory) {
    if (getBit(*memory, 0) != 0) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    *memory = *memory >> 1;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }

}

/*
    ROL - Rotate Left
    Shifts bits in A or memory left by one place.
    Bit 0 is set to contents of Carry flag.
    Carry flag set to contents of old 7 bit.
    Sets Zero and Negative flag based on result.
*/
void ROL(BYTE *memory) {
    BYTE mask;
    if(getBit(cpu.P, FLAG_C) != 0) {
        mask = FLAG_MASK;
    } else {
        mask = 0x0;
    }
    if (getBit(*memory, LEFT_BIT) != 0) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    *memory = *memory << 1;
    //sets 0 bit
    *memory = *memory | mask;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    ROR - Rotate Right
    Shifts bits in A or memory right by one place.
    Bit 7 is set to contents of Carry flag.
    Carry flag set to contents of old 0 bit.
    Sets Zero and Negative flag based on result.
*/
void ROR(BYTE *memory) {
    BYTE mask;
    if(getBit(cpu.P, FLAG_C) != 0) {
        mask = FLAG_MASK << LEFT_BIT;
    } else {
        mask = 0x0;
    }
    if (getBit(*memory, RIGHT_BIT) != 0) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    *memory = *memory >> 1;
    //sets 0 bit
    *memory = *memory | mask;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) == 1) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    STA - Store Accumulator
    Store contents of A into memory.
*/
void STA(BYTE *memory) {
    *memory = cpu.A;
}

/*
    STX - Store X Register
    Store contents of X into memory.
*/
void STX(BYTE *memory) {
    *memory = cpu.X;
}

/*
    STY - Store Y Register
    Store contents of Y into memory.
*/
void STY(BYTE *memory) {
    *memory = cpu.Y;
}

/*
    TAX - Transfer Accumulator to X
    Copies accumulator contents into X.
    Sets Zero and Negative flags as appropriate.
*/
void TAX() {
    cpu.X = cpu.A;
    if (cpu.X == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.X, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    TAY - Transfer Accumulator to Y
    Copies accumulator contents into Y.
    Sets Zero and Negative flags as appropriate.
*/
void TAY() {
    cpu.Y = cpu.A;
    if (cpu.Y == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.Y, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    TSX - Transfer Stack Pointer to X
    Copies Stack Pointer value into X.
    Sets Zero and Negative flags as appropriate.
*/
void TSX() {
    cpu.X = cpu.S;
    if (cpu.X == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.X, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    TXA - Transfer X to Accumulator
    Copies X contents into Accumulator.
    Sets Zero and Negative flags as appropriate.
*/
void TXA() {
    cpu.A = cpu.X;
    if (cpu.A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.A, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    TXS - Transfer X to Stack Pointer
    Copies X contents into Stack Pointer.
    Does not affect status flags.
*/
void TXS() {
    cpu.S = cpu.X;
}

/*
    TYA - Transfer Y to Accumulator
    Copies Y contents into Accumulator.
    Sets Zero and Negative flags as appropriate.
*/
void TYA() {
    cpu.A = cpu.Y;
    if (cpu.A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.A, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    AND - AND memory and Accumulator
    Bitwise AND performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void AND(BYTE *memory) {
    cpu.A = cpu.A & *memory;
    if (cpu.A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.A, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    EOR - Exclusive OR
    Bitwise Exclusive Or performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void EOR(BYTE *memory) {
    cpu.A = cpu.A ^ *memory;
    if (cpu.A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.A, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    ORA - Inclusive OR
    Bitwise OR performed on contents of memory and Accumulator.
    Result stored in Accumulator.
    Sets Z and N flags according to result.
*/
void ORA(BYTE*memory) {
    cpu.A = cpu.A | *memory;
    if (cpu.A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(cpu.A, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    BIT - Bit Test
    Tests if bits set in memory location based on Accumulator.
    ANDs Accumulator and memory location but does not store results.
    Condition flags are set according to AND result (z)
    and memory contents (N and V).
*/
void BIT(BYTE *memory) {
    if((cpu.A & *memory) == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (getBit(*memory, FLAG_N) != 0) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    if (getBit(*memory, FLAG_V) != 0) {
        setFlag(FLAG_V);
    } else {
        resetFlag(FLAG_V);
    }
}

/*
    Helper function - pushes register to stack and decrements Stack Pointer
    by 1 to next position on Stack
*/
void push_to_stack(BYTE *memory, BYTE reg) {
    memory[STACK_BASE + cpu.S] = reg;
    cpu.S--;
}


/*
    Push - Accumulator
    Stores contents of the Accumulator on top of the stack.
    Then decrements Stack Pointer by 1.
    No other registers/statuses are affected.
*/
void PHA(BYTE *memory) {
    push_to_stack(memory, cpu.A);
}

/*
    Push - Processor Status
    Stores contents of Processor Status on stack.
    Then decrements Stack Pointer by 1.
    No other registers/statuses are affected.
*/
void PHP(BYTE *memory) {
    push_to_stack(memory, cpu.P);
}

/*
    Helper function - pulls register from stack
    Increments stack pointer before pulling contents
*/
void pull_from_stack(BYTE *memory, BYTE *reg) {
    cpu.S++;
    *reg = memory[STACK_BASE + cpu.S];
}

/*
    Pull accumulator from stack
    Increments Stack Pointer by 1.
    Then load accumulator from Top of stack.
    Sets Zero flag and negative flag based on A.
*/
void PLA(BYTE *memory) {
    pull_from_stack(memory, &cpu.A);
    if (getBit(cpu.A, FLAG_N)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    if (cpu.A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
}

/*
    Pull processor status from stack
    Increments Stack Pointer by 1.
    Then load processor status from top of stack.
*/
void PLP(BYTE *memory) {
    pull_from_stack(memory, &cpu.P);
}

/*
    ADC - Add with Carry
    Adds contents of memory to accumulator with carry value.
    Carry flag is set if overflow occurs.
    Z flag set if result is 0.
    N flag set if result is negative.
    V flag set if sign bit is incorrect.
*/
void ADC(BYTE *memory) {
    BYTE HIGH_BIT_MASK = 0x80;
    int sum = *memory + cpu.A + getBit(cpu.P, FLAG_C);
    if((cpu.A ^ sum) & (*memory ^ sum) & HIGH_BIT_MASK) {
        setFlag(FLAG_V);
    } else {
        resetFlag(FLAG_V);
    }
    cpu.A = sum;
    //check if unsigned sum larger than 255 to determine if carry set
    if((sum >> 9) & ~(~0 << 1)) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    if (getBit(cpu.A, FLAG_N)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
     if (cpu.A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
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
void SBC(BYTE *memory) {
   BYTE mem_complement = ~(*memory);
   ADC(&mem_complement);
}

/*
    Compare helper function
    Compares given register with memory value and sets flags accordingly.
*/
void set_flags_on_compare(BYTE reg, BYTE *memory) {
    BYTE result = reg - *memory;
    if (getBit(result, FLAG_N)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    if (reg == *memory) {
        setFlag(FLAG_Z);
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_Z);
    }
    if (reg > *memory) {
        setFlag(FLAG_C);
    } else if (reg < *memory) {
        resetFlag(FLAG_C);
    }
}

/*
    Compare memory and accumulator
    Subtracts contents of memory from accumulator.
    Z Flag is set if A = M otherwise reset.
    N Flag is set by result bit 7.
    C when memory less than or equal to accumulator, reset if M greater than A

*/
void CMP(BYTE *memory) {
    set_flags_on_compare(cpu.A, memory);
}

/*
    Compare memory and X register
    Subtracts contents of memory from X register.
    Z is set if X = M otherwise reset.
    N is set by result bit 7.
    C when memory less than or equal to X, reset if M greater than X.

*/
void CPX(BYTE *memory) {
    set_flags_on_compare(cpu.X, memory);
}

/*
    Compare memory and Y register
    Subtracts contents of memory from Y register.
    Z is set if Y = M otherwise reset.
    N is set by result bit 7.
    C when memory less than or equal to Y, reset if M greater than Y.

*/
void CPY(BYTE *memory) {
    set_flags_on_compare(cpu.Y, memory);
}
