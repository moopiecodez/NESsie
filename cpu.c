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

void BRK();

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
uint8_t P = 0x00u;

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

  
uint8_t SP = 0xFFu;

//declare an array to hold NES memory addresses from $0000-$FFFF
//each page is 0xFF
//will need to do memory mirroring $0000-$07FF mapped to $0800-$1FFF
char memory[0xFFFF];

unsigned int getFlag(int position) {
    unsigned int flag;
    uint8_t mask = ~(~0 << 1);
    flag = (P >> position) & mask;
    return flag;
}

void setFlag(int position) {
    uint8_t setFlag = ~(~0 << 1);
    P = P | (setFlag << position);
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
    memory[SP] = P; // status register
    
    /*
    check it worked
    for(int i = 0; i < 3; i++) {
        printf("%u\n", memory[SP]);
        SP++;
    }
    */

    //Interrupt Mask bit is set to 1
    setFlag(FLAG_I);
    //Interrupt pointer $FFFE and $FFFF loaded into PC
    IRQ_Interrupt();

    //does RTI need to be in BRK? seems like separate instruction
    
    


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

void ORA() {
    //affects flags N and Z
    //differing clock cycle time
    //is Len for PC?
}

