#include <stdio.h>
#include <stdint.h>

//define constants for Flag positions in Processor Status register
#define FLAG_N 7
#define FLAG_V 6
#define FLAG_B 4
#define FLAG_D 3
#define FLAG_I 2
#define FLAG_Z 1
#define FLAG_C 0

//define constant for one Byte of memory
#define BYTE uint8_t

void BRK();
void RTI();
void ORA(BYTE compared);
//may need to change to take memory address to update
BYTE ASL(BYTE byte);
void PHP(); 
void push_SP_to_stack();

//ensure Program Counter is an int that takes 2 bytes/16 bits of memory
uint16_t PC = 0xFFu; 

//Processor Status (P) register, ensure 8 bits
/*
7   -   N (negative flag)
6   -   V (overflow flag)
5   -   not used
4   -   B (break command flag)
3   -   D (decimal mode flag)
2   -   I (interrupt disable flag)
1   -   Z (zero flag)
0   -   C (carry flag)

can use SET_ON constants to turn on
*/
BYTE P = 0x00u;

/*
  Stack $0100-$01FF
  Stack Pointer - 8 bit register acts as offset from $0100, so starts at $FF
  When byte pushed onto stack, SP decremented and when byte pulled
  SP is incremented.
  No concept of overflow and will just wrap around from $00 to $FF
  
  In an empty stack, the stack pointer points to the element where the next 
  value will be stored. It is moved after pushing and before pulling. ARM 
  traditionally uses a full stack, but 6502 and 65816 use an empty stack. 
  For example, on a 6502, if S = $FD, and the program pushes something, 
  it'll be written to $01FD and then S becomes $FC to show that $01FC is 
  available to store the next value. It is common practice on a 6502 to 
  initialize the stack pointer to $FF at reset time. 
  */

  
BYTE SP = 0xFFu;

/* Accumulator Register: 8-bit register used to store results of arithmetic
and logical operations or hold a value from memory.
*/
BYTE A = 0x00u;

//declare an array to hold NES memory addresses from $0000-$FFFF
//each page is 0xFF
//will need to do memory mirroring $0000-$07FF mapped to $0800-$1FFF
char memory[0xFFFF];

//helper function to check memory value bits
void check_memory(BYTE mem) {
    BYTE mask = ~(~0 << 1);
    for(int i = 7; i >= 0; i--) {
        printf("%u", (mem >> i) & mask);
    }
    printf("\n");
}

//returns 0 if not negative and 128 if it is because c has no bool
unsigned int check_neg(BYTE byte) {
    BYTE neg_mask = (~0 << 7);
    return neg_mask & byte;
}

unsigned int getBit(BYTE byte, int position) {
    BYTE mask = ~(~0 << 1);
    return (byte >> position) & mask;
}

unsigned int getFlag(int position) {
    unsigned int flag;
    flag = getBit(P, position);
    return flag;
}

void setFlag(int position) {
    BYTE setMask = ~(~0 << 1);
    P = P | (setMask << position);
}

/* 
works by checking if Flag to be reset is 0, in which case unchanged
otherwise perform AND bitwise operation on mask with all bits except Flag to 
be reset to 1
*/
void resetFlag(int position) {
    BYTE resetMask = ~(~0 << 1);
    P = P & ~(resetMask << position);
}


void IRQ_Interrupt() {
    uint16_t InterruptPointer = (memory[0xFFFF] << 8) + memory[0xFFFE];
    PC = InterruptPointer;
}

int main() {
    printf("Hello I'm a CPU\n");
    printf("I'm a 6502!\n");
    printf("Ok... I'm actually a virtual RP2A07.. but it still counts!\n");
    printf("...\n");
    printf("My first instruction is BRK - break.\n");

    //load ROM
    //interpret ROM
    //initialise global variables
    printf("%u\n", PC);
    
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");
    printf("The status of the break flag is: %u\n", getFlag(FLAG_B));
    
    //set values for interrupt pointer
    memory[0xFFFE] = 2;
    memory[0xFFFF] = 48;
    BRK();
    printf("The status of the break flag is: %u\n", getFlag(FLAG_B));
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");

    printf("%u\n", PC);

    RTI();

    A = 20;
    check_memory(A);
    ORA(27);
    check_memory(27);
    check_memory(A);
    printf("%u\n", check_neg(A));
    printf("%u\n", check_neg(-2));

    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");

    resetFlag(FLAG_B);
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");
    resetFlag(FLAG_B);
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");

    check_memory(A);
    A = -27;
    check_memory(A);
    A = ASL(A);
    check_memory(A);

}

//function for break instruction
// takes 7 cycles?
void BRK() {
    //increment PC by 2
    PC += 2;
    //set B flag
    setFlag(FLAG_B);

    //PC and status register (P) pushed to stack
    //if PC is 16 bits 2 memory locations?
    //if used elsewhere make helper function
    
    printf("%u\n", SP);
    memory[SP] = PC >> 8; //high byte of PC
    SP--;
    printf("%u\n", SP);
    memory[SP] = PC; // low byte of PC
    SP--;
    printf("%u\n", SP);
    push_SP_to_stack();

    //Interrupt Mask bit is set to 1
    //note described as disables maskable interrupt? 
    setFlag(FLAG_I);
    //Interrupt pointer $FFFE and $FFFF loaded into PC
    IRQ_Interrupt();
  
    /*The BRK instruction forces the generation of an interrupt request. 
    The program counter and processor status are pushed on the stack then 
    the IRQ interrupt vector at $FFFE/F is loaded into the PC and the break 
    flag in the status set to one.
    
    OR

    BRK causes a non-maskable interrupt and increments the program counter by 
    one. Therefore an RTI will go to the address of the BRK +2 so that BRK may 
    be used to replace a two-byte instruction for debugging and the subsequent 
    RTI will be correct. */
}

/*
    Return from Interrupt. Pulls the Status (P) register and PC off top of the 
    Stack.
    Relies on Stack Pointer pointing to correct position in stack
*/
void RTI() {
    printf("%u\n", SP);
    SP++;
    printf("%u\n", SP);
    printf("%u\n", P);
    P = memory[SP]; //status register
    //note loses interrupt mask set during BRK instruction
    printf("%u\n", P);
    SP++;
    printf("%u\n", SP);
    BYTE lowByte = memory[SP];
    SP++;
    printf("%u\n", SP);
    PC = memory[SP] << 8;
    printf("%u\n", PC);
    PC = PC + lowByte;
    printf("%u\n", PC);
    //check if meant to be +2 ***
}

/*
Logically OR Memory with Accumulator
Can be immediate, Zero Page, Absolute or Indirect addressing
*/
//immediate addressing
/*note takes mem loc to be compared as argument so parser will need to extract
it from instruction and pass it to the function. Alternatively can have
different function for each addressing type.
*/
void ORA(BYTE compared_mem) {
    //differing clock cycle time
    //is Len for PC?
    A = (A | compared_mem);
    //sets zero flag if A is 0
    if (A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    //sets negative flag if A negative (bit 7 set)
    if (check_neg(A)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
Shift accumulator or memory byte left
This shifts all bits in accumulator or memory byte left by 1 bit.
Bit 0 is set to 0 and bit 7 is placed in the carry flag.
Effectively multiplies memory contents by 2.
Accumulator or memory byte is determined by addressing mode.
*/
//either needs to return a Byte or update memory as a side effect
BYTE ASL(BYTE byte) {
    if (getBit(byte, 7)) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    byte = byte << 1;
    if (check_neg(byte)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    //question if 0 after carry as overall wasn't 0 so is flag still set????
    if (byte != 0) {
        resetFlag(FLAG_Z);
    } else {
        setFlag(FLAG_Z);
    }
    return byte;
}

/* Push processor status
    Stores contents of the Status (P) register on top of the stack.
    Then decrements Stack Pointer by 1.
    No other registers/statuses are affected.
*/
void PHP() {
    push_SP_to_stack();
}

//function for pushing status register to stack, used by various instructions
void push_SP_to_stack() {
    memory[SP] = P;
    SP--;
}

