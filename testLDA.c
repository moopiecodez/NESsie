#include <stdio.h>
#include <assert.h>
#include "cpu.h"

BYTE memory[0xFFFF];

int main(void) {
    //arrange
    power_cpu();
    BYTE expectedA = 0x2c;
    BYTE expectedX = 0x00; //zero
    BYTE expectedY = 0x85; //negative
    memory[0xFFF1] = expectedA;
    memory[0xFFF2] = expectedX;
    memory[0xFFF3] = expectedY;
    BYTE expectedFlagset_N = 1;
    BYTE expectedFlagclear_N = 0;
    BYTE expectedFlagset_Z = 1;
    BYTE expectedFlagclear_Z = 0;

    //action
    LDA(&memory[0xFFF1]);
    BYTE actualFlagN_A = getBit(cpu.P, FLAG_N);
    BYTE actualFlagZ_A = getBit(cpu.P, FLAG_Z);
    LDX(&memory[0xFFF2]);
    BYTE actualFlagN_X = getBit(cpu.P, FLAG_N); 
    BYTE actualFlagZ_X = getBit(cpu.P, FLAG_Z);
    LDY(&memory[0xFFF3]);
    BYTE actualFlagN_Y = getBit(cpu.P, FLAG_N);
    BYTE actualFlagZ_Y = getBit(cpu.P, FLAG_Z);

    //assert
    printf("Testing LDA().\n");
    assert(cpu.A == expectedA);
    assert(actualFlagN_A == expectedFlagclear_N);
    assert(actualFlagZ_A == expectedFlagclear_Z);
    printf("Testing LDX().\n");
    assert(cpu.X == expectedX);
    assert(actualFlagN_X == expectedFlagclear_N);
    assert(actualFlagZ_X == expectedFlagset_Z);
    
    printf("Testing LDY().\n");
    assert(cpu.Y == expectedY);
    assert(actualFlagN_Y == expectedFlagset_N);
    assert(actualFlagZ_Y == expectedFlagclear_Z);
    printf("Test passed.\n");
    return 0;
}
