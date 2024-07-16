#include <stdio.h>
#include "cpu.h"

/*
    array to hold NES memory addresses from $0000-$FFFF
    each page is 0xFF
    will need to do memory mirroring $0000-$07FF mapped to $0800-$1FFF
*/
BYTE memory[0xFFFF];

int main(void) {
    printf("setting up new main function for nessie");
    
    return 0;
}
