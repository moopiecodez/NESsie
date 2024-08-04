#include "cpu.h"

void oc_00(CPU *cpu, BYTE *memory) {
    imp();
    BRK(cpu, memory);
}

void oc_01(CPU *cpu, BYTE *memory) {
    //(zp,X)
    BYTE *data = zpX(cpu, memory);
    ORA(cpu, data);
}

void oc_05(CPU *cpu, BYTE *memory) {
    //zp
    BYTE *data = zp(cpu, memory);
    ORA(cpu, data);
}

void oc_06(CPU *cpu, BYTE *memory) {
    //zp aka addr
    BYTE *data = zp(cpu, memory);
    ASL(cpu, data);
}

void oc_08(CPU *cpu, BYTE *memory) {
    imp();
    PHP(cpu, memory);
}

void oc_09(CPU *cpu, BYTE *memory) {
    //# aka data
    ORA(cpu, memory);
}

void oc_0A(CPU *cpu, BYTE *memory) {
    //A
    ASL(cpu, memory);
}

void oc_0D(CPU *cpu, BYTE *memory) {
    //abs aka addr16
    ORA(cpu, memory);
}

void oc_0E(CPU *cpu, BYTE *memory) {
    //addr16
    ASL(cpu, memory);
}

void oc_11(CPU *cpu, BYTE *memory) {
    //(zp),Y aka (addr)Y
    ORA(cpu, memory);
}

void oc_15(CPU *cpu, BYTE *memory) {
    //zp,X aka addr,X
    ORA(cpu, memory);
}

void oc_16(CPU *cpu, BYTE *memory) {
    //addr,X
    ASL(cpu, memory);
}

void oc_18(CPU *cpu, BYTE *memory) {
    //single Byte
    CLC(cpu);
}

void oc_19(CPU *cpu, BYTE *memory) {
    //abs,Y aka addr16,Y
    ORA(cpu, memory);
}
void oc_1D(CPU *cpu, BYTE *memory) {
    //abs,X aka addr16,X
    ORA(cpu, memory);
}

void oc_1E(CPU *cpu, BYTE *memory) {
    //addr16,X
    ASL(cpu, memory);
}

void oc_20(CPU *cpu, BYTE *memory) {
    //label
    BYTE low;
    BYTE high;
    JSR(cpu, memory, low, high);
}

void oc_A1(CPU *cpu, BYTE *memory) {
    BYTE mem;
    mem = zpX(cpu, memory);
    LDA(cpu, mem);
}

void oc_A4(CPU *cpu, BYTE *memory) {
    
}

void oc_4C(CPU *cpu, BYTE *memory) {
    //JMP PC changes need to be different
}