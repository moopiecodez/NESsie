#include <stdio.h>
#include "cpu.h"
#include "instructions.h"

//check test seting global variables not a problem

int test_BRK() {
    /*
    increments PC by 2
    sets B flag
    puts stuff on stack
    sets I flag
    changes PC to interrupt pointer
    */
    PC = 2;
    P = 1;
    printStatus();
    return 0;
   
}
void printStatus() {
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");
}

void main() {
    printf("tests running...");
    test_BRK();
}