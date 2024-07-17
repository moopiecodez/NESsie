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
    Z and N flags set depending on result
*/
void INC(BYTE *memory) {
    increment(memory);
}

/*
    INX - increment X register by 1
    Z and N flags set depending on result
*/
void INX() {
    increment(&cpu.X);
}

/*
    INY - increment Y register by 1
    Z and N flags set depending on result
*/
void INY() {
    increment(&cpu.Y);
}

/*
    LDA - load a byte of memory into accumulator
    Z and N flags set depending on result
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
    Z and N flags set depending on result
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
    Z and N flags set depending on result
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