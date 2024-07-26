#include <stdint.h>

#define BYTE uint8_t

//constant for flag bitwise operations
#define FLAG_MASK ~(~0 << 1)
#define LEFT_BIT 7
#define RIGHT_BIT 0

//IRQ interrupt vector locations
#define IRQ_HIGH 0xFFFFu
#define IRQ_LOW 0xFFFEu

//constant for base stack location in memory
#define STACK_BASE 0x0100u

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

void power_cpu(CPU *cpu);

void setFlag(CPU *cpu, int position);
void resetFlag(CPU *cpu, int position);
BYTE getBit(BYTE source, int position);

void increment(CPU *cpu, BYTE *memory);
void INC(CPU *cpu, BYTE *memory);
void INX(CPU *cpu);
void INY(CPU *cpu);
void decrement(CPU *cpu, BYTE *memory);
void DEC(CPU *cpu, BYTE *memory);
void DEX(CPU *cpu);
void DEY(CPU *cpu);

void LDA(CPU *cpu, BYTE *memory);
void LDX(CPU *cpu, BYTE *memory);
void LDY(CPU *cpu, BYTE *memory);

void ASL(CPU *cpu, BYTE *memory);
void LSR(CPU *cpu, BYTE *memory);
void ROL(CPU *cpu, BYTE *memory);
void ROR(CPU *cpu, BYTE *memory);

void STA(CPU *cpu, BYTE *memory);
void STX(CPU *cpu, BYTE *memory);
void STY(CPU *cpu, BYTE *memory);

void TAX(CPU *cpu);
void TAY(CPU *cpu);
void TSX(CPU *cpu);
void TXA(CPU *cpu);
void TXS(CPU *cpu);
void TYA(CPU *cpu);

void AND(CPU *cpu, BYTE *memory);
void EOR(CPU *cpu, BYTE *memory);
void ORA(CPU *cpu, BYTE *memory);
void BIT(CPU *cpu, BYTE *memory);

void push_to_stack(CPU *cpu, BYTE *memory, BYTE reg);
void PHA(CPU *cpu, BYTE *memory);
void PHP(CPU *cpu, BYTE *memory);

void pull_from_stack(CPU *cpu, BYTE *memory, BYTE *reg);
void PLA(CPU *cpu, BYTE *memory);
void PLP(CPU *cpu, BYTE *memory);

void ADC(CPU *cpu, BYTE *memory);
void SBC(CPU *cpu, BYTE *memory);

void set_flags_on_compare(CPU *cpu, BYTE reg, BYTE *memory);
void CMP(CPU *cpu, BYTE *memory);
void CPX(CPU *cpu, BYTE *memory);
void CPY(CPU *cpu, BYTE *memory);

void CLC(CPU *cpu);
void CLD(CPU *cpu);
void CLI(CPU *cpu);
void CLV(CPU *cpu);
void SEC(CPU *cpu);
void SED(CPU *cpu);
void SEI(CPU *cpu);

void BCC(CPU *cpu, BYTE displacement);
void BCS(CPU *cpu, BYTE displacement);
void BEQ(CPU *cpu, BYTE displacement);
void BMI(CPU *cpu, BYTE displacement);
void BNE(CPU *cpu, BYTE displacement);
void BPL(CPU *cpu, BYTE displacement);
void BVC(CPU *cpu, BYTE displacement);
void BVS(CPU *cpu, BYTE displacement);

void JMP(CPU *cpu, BYTE low, BYTE high);
void JSR(CPU *cpu, BYTE *memory, BYTE low, BYTE high);
void RTS(CPU *cpu, BYTE *memory);

void BRK(CPU *cpu, BYTE *memor);
void NOP();
void RTI(CPU *cpu, BYTE *memory);