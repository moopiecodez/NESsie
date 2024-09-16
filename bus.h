#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include "device.h"

#define ADDR_RANGE_END      0x10000
#define PRGROM_HIGH_START   0xC000
#define PRGROM_LOW_START    0x8000
#define CARTRIDGE_START     0x4020
#define IO_REGISTERS_START  0x2000

struct bus;
typedef struct bus *Bus;

Bus bus_create(Device *, Device *);

uint8_t bus_read(Bus, uint16_t);
void bus_write(Bus, uint16_t, uint8_t);

#endif
