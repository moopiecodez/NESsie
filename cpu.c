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
