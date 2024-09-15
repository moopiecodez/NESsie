#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <stdio.h>
#include <stdint.h>

typedef struct cartridge *Cartridge;

Cartridge cartridge_load(char *);
uint8_t cartridge_read(Cartridge, uint16_t);

#endif