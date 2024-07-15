#include <stdio.h>
#include <stdint.h>
#include "cpu.h"
#include "instructions.h"

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
ensures Flag is 0
perform AND bitwise operation on mask with all bits except Flag to 
be set to 1, so clears intended position
*/
void resetFlag(int position) {
    BYTE resetMask = ~(~0 << 1);
    P = P & ~(resetMask << position);
}


void IRQ_Interrupt() {
    uint16_t InterruptPointer = (memory[0xFFFF] << 8) + memory[0xFFFE];
    PC = InterruptPointer;
}

void power_CPU() {
    A, X, Y = 0x00u;
    PC = 0xFFFCu;
    S = 0xFFu;
    //is S $FF or $FD?
    P = 0x00u;
    setFlag(FLAG_I);
    //clear internal RAM except high scores $0000-$07FF
}

void reset_CPU() {
    //S -= 3;
    //is it FF or -=3?
    S = 0xFFu;
    PC = 0xFFFCu;
    setFlag(FLAG_I);
    //clear internal RAM except high scores $0000-$07FF
}

int main() {
    power_CPU();
    printf("Hello I'm a CPU\n");
    printf("I'm a 6502!\n");
    printf("Ok... I'm actually a virtual RP2A07.. but it still counts!\n");
    printf("...\n");
    printf("My first instruction is BRK - break.\n");

    //load ROM
    //interpret ROM
    //initialise global variables
    printf("The initial PC is %u\n", PC);
    
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");
    printf("The status of the break flag is: %u\n", getFlag(FLAG_B));
    printf("The PC is %u\n", PC);

    //set values for interrupt pointer
    memory[0xFFFE] = 2;
    memory[0xFFFF] = 48;
    printf("The PC is %u\n", PC);

    BRK();
    printf("The PC is %u\n", PC);

    printf("The status of the break flag is: %u\n", getFlag(FLAG_B));
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");

    printf("The PC is: %u\n", PC);

    RTI();
    printf("The Processor Status register after RTI is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");
    printf("The PC is: %u\n", PC);


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
    ASL(&A);
    check_memory(A);
    printf("memory is: ");
    check_memory(memory[0xFFFF]);
    ASL(&memory[0xFFFF]);
    //memory[0xFFFF] = ASL(memory[0xFFFF]);
    check_memory(memory[0xFFFF]);
    memory[0xFFFF] = 48;
    printf("%d\n", A);
    ADC(memory[0xFFFF]);
    check_memory(A);
    printf("The Processor Status register is: 0x%x\n", P);
    for(int i = 7; i >= 0; i--) {
        printf("%u", getFlag(i));
    }
    printf("\n");


}

/*
    BRK for break. PC incremented by 2 and B flag set. PC and P pushed to stack.
    I flag set. Interrupt Pointer (FFFF and FFFE) loaded into PC.
*/
void BRK() {
    PC += 2;
        printf("The initial PC before shift is %u\n", PC);
    setFlag(FLAG_B);
   
    printf("%u\n", S);
    printf("testing %u\n", PC >> 8);
    printf("testing base %u\n", STACK_BASE + S);
    printf("stack holds %u\n", memory[STACK_BASE + S]);
    
    push_to_stack(PC >> 8); //high byte of PC
    
    printf("the high byte of PC is %d\n", memory[STACK_BASE + S + 1]);
    printf("%u\n", S);
    push_to_stack(PC); // low byte of PC
    printf("the low byte of PC is %d\n", memory[STACK_BASE + S + 1]);
    printf("%u\n", S);
    push_to_stack(P);

    setFlag(FLAG_I);
    //Interrupt pointer $FFFE and $FFFF loaded into PC
    IRQ_Interrupt();
 
}

/*
    Return from Interrupt. Pulls the Status (P) register and PC off top of the 
    Stack.
    Relies on Stack Pointer pointing to correct position in stack
*/
void RTI() {
    //printf("%u\n", S);
    P = 0;
    printf("the status register was %x\n", P);
    P = pull_from_stack();
    
    //note loses interrupt mask set during BRK instruction
    printf("the status register is %x\n", P);
    
    BYTE lowByte = pull_from_stack();
    printf("the lowbyte is: %u \n", lowByte);
    
    PC = pull_from_stack() << 8;
    printf("the PC highbyte is: %u \n", PC);
    PC = PC + lowByte;
    printf("%u\n", PC);
    
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
void ASL(BYTE *byte) {
    if (getBit(*byte, 7)) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    *byte = *byte << 1;
    if (check_neg(*byte)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    //question if 0 after carry as overall wasn't 0 so is flag still set????
    if (*byte != 0) {
        resetFlag(FLAG_Z);
    } else {
        setFlag(FLAG_Z);
    }
    //return byte;
}

/* Push processor status
    Stores contents of the Status (P) register on top of the stack.
    Then decrements Stack Pointer by 1.
    No other registers/statuses are affected.
*/
void PHP() {
    push_to_stack(P);
}

/* Push accumulator
    Stores contents of the Accumulator on top of the stack.
    Then decrements Stack Pointer by 1.
    No other registers/statuses are affected.
*/
void PHA() {
    push_to_stack(A);
}

//function for pushing register to stack, used by various instructions
void push_to_stack(BYTE reg) {
    memory[STACK_BASE + S] = reg;
    S--;
}

//function for pulling register from stack, used by various instructions
BYTE pull_from_stack() {
    S++;
    return memory[STACK_BASE + S]; 
}


/* Pull accumulator from stack
    Increments Stack Pointer by 1.
    Then load accumulator from Top of stack
    Sets Zero flag and negative flag based on A.    
*/
void PLA() {
    A = pull_from_stack();
    if (check_neg(A)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    if (A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
}

/* Pull processor status from stack
    Increments Stack Pointer by 1.
    Load processor status register from top of stack
    Sets flags based on stack
*/
void PLP() {
    P = pull_from_stack();
    //flags automatically set as held in bits
}

/*
    Clear carry flag
    Initialises carry flag to 0
    Single byte instruction, addressing mode implied
    2 cycles
*/
void CLC() {
    resetFlag(FLAG_C);
}

/*
    Clear decimal mode
    Sets decimal flag to 0
    single byte instruction, addressing mode implied
    2 cycles
    not strictly necessary - NES doesn't use decimal
*/
void CLD() {
    resetFlag(FLAG_D);
}

/*
    Clear interrupt disable bit
    Sets interrupt disable flag to 0
    single byte instruction, addressing mode implied
    2 cycles
*/
void CLI() {
    resetFlag(FLAG_I);
}

/*
    Clear overflow flag
    Sets overflow flag to 0
    single byte instruction, addressing mode implied
    2 cycles
*/
void CLV() {
    resetFlag(FLAG_V);
}

/*
    Compare memory and accumulator
    subtracts contents of memory from accumulator
    Z is set if A = M otherwise reset
    N is set by result bit 7
    C when memory less than or equal to accumulator, reset if M greater than A

*/
void CMP(BYTE memory) {
    //check if negative flag set on result or memory value
    compare_flags(A, memory);
}

void compare_flags(BYTE reg, BYTE memory) {
    BYTE result = reg - memory;

    if (check_neg(result)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    if (result = 0) {
        setFlag(FLAG_Z);
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_Z);
    }
    if (result > 0) {
        setFlag(FLAG_C);
    } else if (result < 0) {
        resetFlag(FLAG_C);
    }
}

/*
    Compare memory and X register
    subtracts contents of memory from X register
    Z is set if X = M otherwise reset
    N is set by result bit 7
    C when memory less than or equal to X, reset if M greater than X

*/
void CPX(BYTE memory) {
    compare_flags(X, memory);
}

/*
    Compare memory and Y register
    subtracts contents of memory from Y register
    Z is set if Y = M otherwise reset
    N is set by result bit 7
    C when memory less than or equal to Y, reset if M greater than Y

*/
void CPY(BYTE memory) {
    compare_flags(Y, memory);
}

/*
    ADC add with carry - adds contents of memory to accumulator with carry
    carry bit is set if overflow occurs
*/
void ADC(BYTE memory) {
    int sum = memory + A + getFlag(FLAG_C);
    if((A & sum) && (memory & sum) && 0x80) {
        setFlag(FLAG_V);
    } else {
        resetFlag(FLAG_V);
    }
    A = sum;
    //check if unsigned sum larger than 255 to determine if carry set
    if((sum >> 9) & ~(~0 << 1)) {
        setFlag(FLAG_C);
    } else {
        resetFlag(FLAG_C);
    }
    if (check_neg(A)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
     if (A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }

}

/*
 AND - ands memory with Accumulator
*/
void AND(BYTE memory) {
    A = A & memory;
    if (check_neg(A)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
     if (A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }   
}

/*
    BCC - Branch if Carry Clear
    If C flag clear add relative displacement to PC to branch.
    i.e. add second object code byte contents (signed 8 bit displacement) to
    PC + 2. Result is memory address for next instruction
    otherwise just PC + 2 is next address
*/
void BCC(BYTE rdisplacement) {
    if (getFlag(FLAG_C) == 0) {
        //check when addressing ***
        PC += 2 + rdisplacement;
    }
}

/*
    BCS - Branch if Carry Set
    Same as BCC except branch triggered if C is set.
*/
void BCS(BYTE rdisplacement) {
    if (getFlag(FLAG_C) != 0) {
        PC += 2 + rdisplacement;
    }
}

/*
    BEQ - Branch if Equal to Zero
    Same as BCC except branch triggered if Z is set.
*/
void BEQ(BYTE rdisplacement) {
    if (getFlag(FLAG_Z) != 0) {
        PC += 2 + rdisplacement;
    }
}

/*
    BMI - Branch if Minus
    Same as BMI except branch triggered if N is set.
*/
void BMI(BYTE rdisplacement) {
    if (getFlag(FLAG_N) != 0) {
        PC += 2 + rdisplacement;
    }
}

/*
    BNE - Branch if not equal to Zero
    Same as BMI except branch triggered if Z is clear.
*/
void BNE(BYTE rdisplacement) {
    if (getFlag(FLAG_Z) == 0) {
        PC += 2 + rdisplacement;
    }
}

/*
    BPL - Branch if Plus
    Same as BMI except branch triggered if N is clear.
*/
void BPL(BYTE rdisplacement) {
    if (getFlag(FLAG_N) == 0) {
        PC += 2 + rdisplacement;
    }
}

/*
    BVC - Branch if Overflow clear
    Same as BMI except branch triggered if V is clear.
*/
void BVC(BYTE rdisplacement) {
    if (getFlag(FLAG_V) == 0) {
        PC += 2 + rdisplacement;
    }
}

/*
    BVS - Branch if Overflow set
    Same as BMI except branch triggered if V is set.
*/
void BVS(BYTE rdisplacement) {
    if (getFlag(FLAG_V) != 0) {
        PC += 2 + rdisplacement;
    }
}
/*
    BIT - BIT test
    logically AND A and memory contents, leaves A and memory unchanged.
    Z flag set depending on result of AND operation
    N flag set based on 7th bit of memory byte
    V flag set based on 6th bit of memory byte
*/
void BIT(BYTE memory) {
    BYTE result = A ^ memory;
    if (result == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(memory)) {
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
    if (getBit(memory, 6)) {
        setFlag(FLAG_V);
    } else {
        resetFlag(FLAG_V);
    }
}

/*
    DEC - decrement memory
    decrements contents of memory contents by one
    sets Z and N flags accordingly
*/
void DEC(BYTE *memory) {
    *memory = *memory - 1;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(*memory)){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    DEX - decrement X register
    decrements contents of X register by one
    sets Z and N flags accordingly
*/
void DEX() {
    X--;
    if (X == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(X)){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    } 
}

/*
    DEY - decrement Y register
    decrements contents of Y register by one
    sets Z and N flags accordingly
*/
void DEY() {
    Y--;
    if (Y == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(Y)){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    } 
}

/*
    EOR - exclusive OR
    exclusively or contents of Accumulator and memory location
    Set N and Z flags based on result
*/
void EOR(BYTE memory) {
    A = A ^ memory;
    if (A == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(A)){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    } 
}

/*
    INC - increment memory by 1
    Z and N flags set depending on result
*/
void INC(BYTE *memory) {
    *memory = *memory + 1;
    if (*memory == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(*memory)){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    INX - increment X register by 1
    Z and N flags set depending on result
*/
void INX() {
    X++;
    if (X == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(X)){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}

/*
    INY - increment Y register by 1
    Z and N flags set depending on result
*/
void INY() {
    Y++;
    if (Y == 0) {
        setFlag(FLAG_Z);
    } else {
        resetFlag(FLAG_Z);
    }
    if (check_neg(Y)){
        setFlag(FLAG_N);
    } else {
        resetFlag(FLAG_N);
    }
}