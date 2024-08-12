#include <stdio.h>
#include <stddef.h>
#include "cpu.h"

/*
    array to hold NES memory addresses from $0000-$FFFF
    each page is 0xFF
    will need to do memory mirroring $0000-$07FF mapped to $0800-$1FFF
*/
// BYTE memory[0xFFFF];
BYTE memory[0xFFFF] = {
    0x04, 0x0C, 0x00, 0x07, 0x0D, 0x00, 0x01, 0x00, 0x03, 0x44, 0x01, 0x05, 0x24, 0x18, 0xCC, 0x1E
};

void print_cpu(CPU *cpu) {
    char flags[8];
    for (int i = 7; i >= 0; i--) {
        flags[7-i] = cpu->P & (1<<i) ? '1' : '0';
    }
    printf("A: %02x, X: %02x, Y: %02x, PC: %04x, IR: %02x, AB: %04x, DB: %02x, P: %s, T: %u\n", 
            cpu->A, cpu->X, cpu->Y, cpu->PC, 
            cpu->IR, cpu->AB, cpu->DB, flags, cpu->T);
}

int main(void) {
    printf("setting up new main function for nessie\n");

    CPU cpu;
    Operation operation;
    cpu.A  = 0x00;
    cpu.PC = 0x0000;
    cpu.IR = 0x00;
    cpu.P = 0x00;
    //needed for BRK
    memory[IRQ_HIGH] = 0x2E;
    memory[IRQ_LOW] = 0x1C;
    setFlag(&cpu, FLAG_I);
    setFlag(&cpu, FLAG_C);

    print_cpu(&cpu);
    //--------------------------------------------------
    for (int step_num = 0; step_num < 12; step_num++) {
        operation = decode(&cpu);
        operation.mode->step[cpu.T](&cpu, memory, operation.ins);
        cpu.T++;
        print_cpu(&cpu);
    }
    cpu.DB = 0x18;
    LSR(&cpu);
    printf("DB: %02x\n", cpu.DB);

    return 0;
}
