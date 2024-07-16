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

    //action
    printf("Testing INC instruction\n");
    INC(mem);
    int actualVal = memory[0xFFFE];
    unsigned int actualNflag = getBit(*mem, FLAG_N);
    //assert
    assert(actualVal == expectedVal);
    assert( actualNflag == 1);
    printf("Test passed\n");
    return 0;
}
