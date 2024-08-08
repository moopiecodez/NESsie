#include <stdio.h>
#include "cpu.h"

/*
    array to hold NES memory addresses from $0000-$FFFF
    each page is 0xFF
    will need to do memory mirroring $0000-$07FF mapped to $0800-$1FFF
*/
// BYTE memory[0xFFFF];
BYTE memory[] = {
    0x01, 0x12
};

void print_cpu(CPU *cpu) {
    printf("A: %02x, PC: %04x, IR: %02x\n", cpu->A, cpu->PC, cpu->IR);
}

int main(void) {
    printf("setting up new main function for nessie\n");
    CPU cpu;
    instruction *ins;
    addr_mode_step *step;
    addr_mode *mode;
    cpu.A  = 0x00;
    cpu.PC = 0x0000;
    cpu.IR = 0x00;
    instruction *ins_set[] = {
        BRK,
        LDA
    };
    addr_mode *addressing_modes[] = {
        &implied,
        &immediate
    };
    print_cpu(&cpu);
    //--------------------------------------------------
    for (int step_num = 0; step_num < 2; step_num++) {
        mode = addressing_modes[cpu.IR];
        ins = ins_set[cpu.IR];
        (*mode)[step_num](&cpu, memory, ins);
        print_cpu(&cpu);
    }
  
    return 0;
}
