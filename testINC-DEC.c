#include <stdio.h>
#include <assert.h>
#include "cpu.h"

BYTE memory[0xFFFF];

int main(void) {
    //arrange
    power_cpu();
    BYTE expectedNeg = 1;
    BYTE expectedZero = 0;
    BYTE val = -3;
    BYTE expectedVal = val + 1;
    memory[0xFFFE] = val;
    BYTE *mem = &memory[0xFFFE];
    

    //action
    printf("Testing INC instruction\n");
    INC(mem);
    int actualVal = memory[0xFFFE];
    BYTE actualNflag = getBit(cpu.P, FLAG_N);
    BYTE actualZflag = getBit(cpu.P, FLAG_Z);
    
    //assert
    assert(actualVal == expectedVal);
    assert(actualNflag == expectedNeg);
    assert(actualZflag == expectedZero);
    
    printf("Test passed\n");
    return 0;
}
