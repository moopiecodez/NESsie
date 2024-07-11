#include <stdint.h>
#define BYTE uint8_t

void ADC(BYTE memory);

void BRK();
void RTI();
void ORA(BYTE compared);
//may need to change to take memory address to update
BYTE ASL(BYTE byte);
void PHP(); 

//helper functions
void push_to_stack(BYTE reg);
void compare_set_flags(BYTE reg, BYTE memory);

//clear flag instructions
void CLC();
void CLD();
void CLI();
void CLV();

//comparison instructions
void CMP(BYTE memory);
void CPX(BYTE memory);
void CPY(BYTE memory);