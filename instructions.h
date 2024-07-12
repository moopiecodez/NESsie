#include <stdint.h>
#define BYTE uint8_t

void ADC(BYTE memory);

void BRK();
void RTI();
void ORA(BYTE compared);
//may need to change to take memory address to update
void ASL(BYTE *byte);
void PHP(); 
void PHA();
void PLP();
void PLA();

//helper functions
void push_to_stack(BYTE reg);
BYTE pull_from_stack();
void compare_flags(BYTE reg, BYTE memory);

//clear flag instructions
void CLC();
void CLD();
void CLI();
void CLV();

//comparison instructions
void CMP(BYTE memory);
void CPX(BYTE memory);
void CPY(BYTE memory);

void AND(BYTE memory);

//branch instructions
void BCC(BYTE rdisplacement);
void BCS(BYTE rdisplacement);
void BEQ(BYTE rdisplacement);
void BMI(BYTE rdisplacement);
void BNE(BYTE rdisplacement);
void BPL(BYTE rdisplacement);
void BVC(BYTE rdisplacement);
void BVS(BYTE rdisplacement);

void BIT(BYTE memory);

//decrement instructions
void DEC(BYTE *memory);
void DEX();
void DEY();

void EOR(BYTE memory);

//increment instructions
void INC(BYTE *memory);
void INX();
void INY();

