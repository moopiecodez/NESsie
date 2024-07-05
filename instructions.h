#include <stdint.h>
#define BYTE uint8_t

void BRK();
void RTI();
void ORA(BYTE compared);
//may need to change to take memory address to update
BYTE ASL(BYTE byte);
void PHP(); 
void push_SP_to_stack();

//clear flag instructions
void CLC();
void CLD();
void CLI();
void CLV();

void CMP(BYTE memory);