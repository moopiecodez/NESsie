#include <stdint.h>
#define BYTE uint8_t

//constants for Flag positions in Processor Status register
#define FLAG_N 7
#define FLAG_V 6
#define FLAG_B 4 
#define FLAG_D 3
#define FLAG_I 2
#define FLAG_Z 1
#define FLAG_C 0

//constant for base stack location in memory
#define STACK_BASE 0x0100

/*
Processor Status (P) register, ensure 8 bits
7   -   N (negative flag)
6   -   V (overflow flag)
5   -   not used
4   -   B (break command flag)
3   -   D (decimal mode flag)   - not implemented in NES version
2   -   I (interrupt disable flag)
1   -   Z (zero flag)
0   -   C (carry flag)
*/
BYTE P = 0x00u;

/* Accumulator Register: 8-bit register used to store results of arithmetic
and logical operations or hold a value from memory.
*/
BYTE A = 0x00u;

/*  Index Register X (X): 8-bit register used as counter/offset. Can be set
    to a value from memory. Can get or set value of stack pointer.
*/
BYTE X = 0x00u;

/*  Index Register Y (Y): 8-bit register used as counter/offset. Cannot affect
    stack pointer.
*/
BYTE Y = 0x00u;

/*
  Stack Pointer - 8 bit register acts as offset from $0100, (Stack is
  $0100-$01FF) so starts at $FF.
  When byte pushed onto stack, S decremented and when byte pulled
  S is incremented.
  No concept of overflow and will just wrap around from $00 to $FF
  
  In an empty stack, the stack pointer points to the element where the next 
  value will be stored. It is moved after pushing and before pulling. ARM 
  traditionally uses a full stack, but 6502 and 65816 use an empty stack. 
  For example, on a 6502, if S = $FD, and the program pushes something, 
  it'll be written to $01FD and then S becomes $FC to show that $01FC is 
  available to store the next value. It is common practice on a 6502 to 
  initialize the stack pointer to $FF at reset time. 
  */
BYTE S = 0xFFu;

//Program Counter is a 16 bit register
uint16_t PC = 0xFFu; 

/*
    array to hold NES memory addresses from $0000-$FFFF
    each page is 0xFF
    will need to do memory mirroring $0000-$07FF mapped to $0800-$1FFF
*/
BYTE memory[0xFFFF];