#include <stdint.h>

#define BYTE uint8_t

//constant for flag bitwise operations
#define FLAG_MASK ~(~0 << 1)

//constant for base stack location in memory
#define STACK_BASE 0x0100

//constants for Processor Status register flags position, 5 not used
#define FLAG_N 7    /* N (negative flag) */
#define FLAG_V 6    /* V (overflow flag) */
#define FLAG_B 4    /* B (break command flag) */
#define FLAG_D 3    /* D (decimal mode flag) - not used in NES CPU */
#define FLAG_I 2    /* I (interrupt disable flag) */
#define FLAG_Z 1    /* Z (zero flag) */
#define FLAG_C 0    /* C (carry flag)*/

typedef struct cpu_registers {
    uint16_t PC;    /* Program Counter */
    BYTE P;         /* Processor status register */
    BYTE A;         /* Accumulator register */  
    BYTE X;         /* Index Register X */
    BYTE Y;         /* Index Register Y */
    BYTE S;         /* Stack Pointer/offset from $0100, initialised at $FF*/
} CPU;

extern CPU cpu;

void power_cpu();

void setFlag();
BYTE getBit(BYTE source, int position);

void increment(BYTE *memory);
void INC(BYTE *memory);
void INX();
void INY();

void LDA(BYTE *memory);
void LDX(BYTE *memory);
void LDY(BYTE *memory);

void ASL(BYTE *memory);
void LSR(BYTE *memory);

void STA(BYTE *memory);
void STX(BYTE *memory);
void STY(BYTE *memory);

void TAX();
void TAY();
void TSX();
void TXA();
void TXS();
void TYA();

void AND(BYTE *memory);
void EOR(BYTE *memory);
void ORA(BYTE *memory);
void BIT(BYTE *memory);

void push_to_stack(BYTE *memory, BYTE reg);
void PHA(BYTE *memory);
void PHP(BYTE *memory);

void pull_from_stack(BYTE *memory, BYTE *reg);
void PLA(BYTE *memory);
void PLP(BYTE *memory);