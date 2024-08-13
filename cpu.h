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

//constant for zero page location
#define ZERO_PAGE 0x0000


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
    BYTE IR;        /* Instruction Register, holds opcode initialised to 0*/
    BYTE DB;        /* Data Bus*/
    uint16_t AB;    /* Address Bus*/
    BYTE T;         /* Instruction step number*/
    BYTE DL;        /*Input Data Latch duplicated in Predecode register loaded with each read bus cycle*/
    BYTE ALU;       /*Arithmetic Logic Unit*/
    BYTE ACR_FLAG;  /*ALU carry register used as a flag if ALU operation resulted in a carry*/
} CPU;

typedef void Instruction(CPU *);
Instruction BRK;
//Read instructions:
Instruction LDA;
Instruction LDX;
Instruction LDY;

//RMW instructions:
Instruction ASL;
Instruction LSR;

// Write instructions:
Instruction STX;
Instruction STA;
Instruction STY;

Instruction CLC;


void incrementPC(CPU *cpu);

typedef void addr_mode_step(CPU *, BYTE *, Instruction *);
addr_mode_step fetch_opcode;
addr_mode_step fetch_throw;
addr_mode_step fetch_throw_brk;
addr_mode_step imm_fetch_operand;
addr_mode_step stack_push_PCH;
addr_mode_step stack_push_PCL;
addr_mode_step stack_push_P;
addr_mode_step fetch_PCL;
addr_mode_step fetch_PCH;
addr_mode_step fetch_ADL;
addr_mode_step fetch_ADH;
addr_mode_step fetch_address;
addr_mode_step read_addr_exe;
addr_mode_step read_addr;
addr_mode_step modify;
addr_mode_step write_addr;
addr_mode_step write_register;
addr_mode_step read_addr_add_X;
addr_mode_step read_addr_add_Y;
addr_mode_step fetch_ADH_add_X;
addr_mode_step fetch_ADH_add_Y;
addr_mode_step read_addr_fixADH_X;
addr_mode_step read_addr_fixADH_Y;

typedef struct addr_mode {
    int numsteps;
    addr_mode_step **step;
} AddressingMode;

typedef struct op {
    Instruction *ins;
    AddressingMode *mode;
} Operation;

Operation decode(CPU *cpu);

void power_cpu(CPU *cpu);

void setFlag(CPU *cpu, int position);
void resetFlag(CPU *cpu, int position);
BYTE getBit(BYTE source, int position);

// void increment(CPU *cpu, BYTE *memory);

// instruction INC;
// instruction INX;
// instruction INY;
// void decrement(CPU *cpu, BYTE *memory);
// instruction DEC;
// instruction DEX;
// instruction DEY;

// instruction ROL;
// instruction ROR;



// instruction TAX;
// instruction TAY;
// instruction TSX;
// instruction TXA;
// instruction TXS;
// instruction TYA;

// instruction AND;
// instruction EOR;
// instruction ORA;
// instruction BIT;

// void push_to_stack(CPU *cpu, BYTE *memory, BYTE reg);
// instruction PHA;
// instruction PHP;

// void pull_from_stack(CPU *cpu, BYTE *memory, BYTE *reg);
// instruction PLA;
// instruction PLP;

// instruction ADC;
// instruction SBC;

// void set_flags_on_compare(CPU *cpu, BYTE reg, BYTE *memory);
// instruction CMP;
// instruction CPX;
// instruction CPY;

// instruction CLD;
// instruction CLI;
// instruction CLV;
// instruction SEC;
// instruction SED;
// instruction SEI;

// instruction BCC;
// instruction BCS;
// instruction BEQ;
// instruction BMI;
// instruction BNE;
// instruction BPL;
// instruction BVC;
// instruction BVS;

// instruction JMP;
// instruction JSR;
// instruction RTS;

// instruction BRK;
// instruction NOP;
// instruction RTI;

//addressing modes
/*
BYTE *absolute(CPU *cpu, BYTE *memory);
BYTE *zp(CPU *cpu, BYTE *memory);
BYTE *zpX(CPU *cpu, BYTE *memory);
BYTE *zpY(CPU *cpu, BYTE *memory);
BYTE *absX(CPU *cpu, BYTE *memory);
BYTE *absY(CPU *cpu, BYTE *memory);
BYTE *rel(CPU *cpu, BYTE *memory);
BYTE *indirectX(CPU *cpu, BYTE *memory);
BYTE *indirectY(CPU *cpu, BYTE *memory);
void absIndirect(CPU *cpu, BYTE *memory);
*/