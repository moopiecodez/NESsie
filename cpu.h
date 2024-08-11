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
    BYTE ABL;       /* Address Bus Low Register*/ //check if ADL or ABL
    BYTE ABH;       /* Address Bus High Byte*/
    BYTE T;         /* Instruction step number*/
} CPU;

typedef void Instruction(CPU *, BYTE);
Instruction BRK;
Instruction LDA;
Instruction LDX;
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
addr_mode_step read_addr_exe;
addr_mode_step read_addr;

typedef addr_mode_step *addr_mode[];
//accumulator addressing is the same cycle wise
static addr_mode immediate = {
    fetch_opcode,
    imm_fetch_operand
};

static addr_mode implied_brk = {
    fetch_opcode,
    fetch_throw_brk,
    stack_push_PCH,
    stack_push_PCL,
    stack_push_P,
    fetch_PCL,
    fetch_PCH
};

static addr_mode implied = {
    fetch_opcode,
    //unlike in BRK, PC not incremented
    fetch_throw
};

static addr_mode absolute_r = {
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr_exe
};

static addr_mode absolute_rmw = {
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    read_addr, //problem is this has ins() and here ins() done next step
    //modify, //zimmers describes as write value back and then do operation
    //write_new_value
};

static addr_mode absolute_w = {
    fetch_opcode,
    fetch_ADL,
    fetch_ADH,
    //write_register
};

struct op {
    int steps;
    Instruction *ins;
    addr_mode *mode;
};

typedef struct op Operation;

Operation decode(CPU *cpu);

// typedef struct instruction_opcode {
//     //instruction ins;
//     //void (*instruction)(CPU *, BYTE *);
//     //flexible array member
//     //void (*addrmode[])(CPU *, BYTE *);
// } OPCODE;

// typedef OPCODE *instructions[];
// instructions *get_instruction_set();



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

// instruction LDY;

// instruction ASL;
// instruction LSR;
// instruction ROL;
// instruction ROR;

// instruction STA;
// instruction STX;
// instruction STY;

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
BYTE *A_addressing(CPU *cpu, BYTE *memory);
BYTE *imm(CPU *cpu, BYTE *memory);
BYTE *absolute(CPU *cpu, BYTE *memory);
BYTE *zp(CPU *cpu, BYTE *memory);
BYTE *zpX(CPU *cpu, BYTE *memory);
BYTE *zpY(CPU *cpu, BYTE *memory);
BYTE *absX(CPU *cpu, BYTE *memory);
BYTE *absY(CPU *cpu, BYTE *memory);
BYTE *imp(CPU *cpu, BYTE *memory);
BYTE *rel(CPU *cpu, BYTE *memory);
BYTE *indirectX(CPU *cpu, BYTE *memory);
BYTE *indirectY(CPU *cpu, BYTE *memory);
void absIndirect(CPU *cpu, BYTE *memory);
*/