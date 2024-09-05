#define BYTE uint8_t

typedef void Instruction(CPU *);
Instruction BRK;
//Read instructions:
Instruction LDA;
Instruction LDX;
Instruction LDY;
Instruction AND;


//RMW instructions:
Instruction ASL;
Instruction LSR;

// Write instructions:
Instruction STX;
Instruction STA;
Instruction STY;

Instruction CLC;
