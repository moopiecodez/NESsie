#include <stdio.h>
#include <assert.h>
#include "cpu.h"

BYTE memory[0xFFFF];

int main(void) {
    //arrange
    power_cpu();
    BYTE expectedNeg = 1;
    BYTE val = -3;
    BYTE expectedVal = val + 1;
    memory[0xFFFE] = val;
    BYTE *mem = &memory[0xFFFE];
    BYTE expectedValX = 1;
    BYTE expectedValY = 1;
    BYTE expectedNegX = 0;
    BYTE expectedNegY = 0;


    //action
    printf("Testing INC instruction\n");
    INC(mem);
    INX();
    INY();
    int actualVal = memory[0xFFFE];
    unsigned int actualNflagMem = getBit(*mem, FLAG_N);
    unsigned int actualNflagX = getBit(cpu.X, FLAG_N);
    unsigned int actualNflagY = getBit(cpu.Y, FLAG_N);
    //assert
    assert(actualVal == expectedVal);
    assert(actualNflagMem == expectedNeg);
    assert(cpu.X == expectedValX);
    assert(cpu.Y == expectedValY);
    assert(actualNflagX == expectedNegX);
    assert(actualNflagY == expectedNegY);

    printf("Test passed\n");
    return 0;
}
