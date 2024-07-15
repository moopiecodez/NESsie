#include <stdio.h>
#include <assert.h>
#include "cpu.h"
#include "instructions.h"

int main(void) {
    printf("Testing INC instruction\n");
    int expected = 4;
    memory[0xFFFE] = 3;
    BYTE *mem = &memory[0xFFFE];
    INC(mem);
    int actual = memory[0xFFFE];
    //any expression resolving to 0 triggers error
    assert(actual == expected);
    return 0;
}