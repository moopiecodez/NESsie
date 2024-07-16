#include <stdio.h>
#include <assert.h>
#include "cpu.h"

BYTE memory[0xFFFF];

int main(void) {
    //arrange
    // registers set to 0x12u to check power_cpu correctly sets them
    cpu.P = cpu.A = cpu.X = cpu.Y = 0x12u;
    BYTE expectedP, expectedA, expectedX, expectedY;
    expectedP = 0x04u;
    expectedA = expectedX = expectedY = 0x00u;
    uint16_t expectedPC = 0xFFFCu;
    BYTE expectedS = 0xFFu;    
    
    //action
    power_cpu();
    BYTE actualP = cpu.P;
    BYTE actualA = cpu.A;
    BYTE actualX = cpu.X;
    BYTE actualY = cpu.Y;
    uint16_t actualPC = cpu.PC;
    BYTE actualS = cpu.S;   
    
    //assert
    printf("Testing power_cpu()\n");
    assert(cpu.P == expectedP);
    assert(cpu.A == expectedA);
    assert(cpu.X == expectedX);
    assert(cpu.Y == expectedY);
    assert(cpu.PC == expectedPC);
    assert(cpu.S == expectedS);
    printf("Test passed\n");

    return 0;
}
