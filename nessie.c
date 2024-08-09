#include <stdio.h>
#include "cpu.h"

/*
    array to hold NES memory addresses from $0000-$FFFF
    each page is 0xFF
    will need to do memory mirroring $0000-$07FF mapped to $0800-$1FFF
*/
// BYTE memory[0xFFFF];
BYTE memory[0xFFFF] = {
    0x01, 0x12
};

void print_cpu(CPU *cpu) {
    char string[8];
    for (int i = 7; i >= 0; i--) {
        string[7-i] = cpu->P & (1<<i) ? '1' : '0';
    }
    printf("A: %02x, PC: %04x, IR: %02x, P:%s\n", cpu->A, cpu->PC, cpu->IR, string);
}

int main(void) {
    printf("setting up new main function for nessie\n");

    CPU cpu;
    Operation operation;
    addr_mode_step *step;
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
    for (int step_num = 0; step_num < 2; step_num++) {
        operation = decode(cpu.IR);
        (*operation.mode)[step_num](&cpu, memory, operation.ins);
        print_cpu(&cpu);
    }
    //how to get to next IR
    return 0;
}
