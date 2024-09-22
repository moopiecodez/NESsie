#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include "device.h"

#define ADDR_RANGE_END      0x10000
#define PRGROM_HIGH_START   0xC000
#define PRGROM_LOW_START    0x8000
#define CARTRIDGE_START     0x4020
#define IO_REGISTERS_START  0x4000
#define PPU_REGISTERS_START 0x2000
#define RAM_MASK            0x07FF
#define PPU_REG_MASK        0x0007


struct bus;
typedef struct bus *Bus;

Bus bus_create(Device *, Device *);

uint8_t bus_read(Bus, uint16_t);
void bus_write(Bus, uint16_t, uint8_t);
uint8_t read_ram(Bus, uint16_t);
void write_ram(Bus, uint16_t, uint8_t);
uint8_t read_io(void *, uint16_t);
void write_io(void *, uint16_t, uint8_t);
uint8_t read_ppu(void *, uint16_t);
void write_ppu(void *, uint16_t, uint8_t);
#endif
