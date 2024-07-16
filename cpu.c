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

unsigned int getBit(BYTE source, int position) {
    unsigned int bit = (source >> position) & FLAG_MASK;
    return bit;
}
/*
    INC - increment memory by 1
    Z and N flags set depending on result
*/
void INC(BYTE *memory) {
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
