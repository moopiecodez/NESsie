#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <stdint.h>
#include "device.h"

typedef struct cartridge *Cartridge;

Device cartridge_load(char *);

#endif