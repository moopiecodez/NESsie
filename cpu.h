#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "bus.h"

#define BYTE uint8_t

typedef struct cpu CPU;
CPU *cpu_create();

void clocktick(CPU *, Bus);
void print_cpu(CPU *);
void power_cpu(CPU *);

#endif