#include <stdlib.h>
#include "bus.h"

struct bus {
    Device *cartridge;
};

Bus bus_create(Device *cartridge) {
    Bus bus = malloc(sizeof(struct bus));
    bus->cartridge = cartridge;
    return bus;
}

uint8_t bus_read(Bus bus, uint16_t address) {
    uint8_t byte;
    if(address >= CARTRIDGE_START && address < ADDR_RANGE_END) {
        byte = bus->cartridge->read(bus->cartridge->data, address);
    }
    return byte;
}


/*
uint8_t cpu_read (uint16_t address) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        byte = *(cpumemory + (address & CPU_RAM_MIRROR));
    }

    else if (address >= 0x2000 && address <= 0x3FFF) {
        byte = *(cpumemory + (address & PPU_REG_MIRROR));
    }

    //0x4000 - 0x401F APU and I/O to be implemented

    else if (address >= 0x4020 && address <= 0xFFFF) {
        byte = mapper_read(address); //needs to be mapper.read
    }
x}

#define CPU_RAM_MIRROR 0x07FF
#define PPU_REG_MIRROR 0x0007

//would need to implement other I/O for DMA, controllers APU
uint8_t cpumemory[0xFFFF];

uint8_t ppumemory[0x3FFF];

struct bus {
    struct Device *cpudevice;
    void *ppudevice;
    void *cartridge;
};

void cpu_write(uint16_t address, uint8_t byte) {
    cpumemory[address] = byte;
    if (address >= 0x0000 && address <= 0x1FFF) {
        *(cpumemory + (address & CPU_RAM_MIRROR)) = byte;
    }

    else if (address >= 0x2000 && address <= 0x3FFF) {
        *(cpumemory + (address & PPU_REG_MIRROR)) = byte;
    }

    //0x4000 - 0x401F APU and I/O to be implemented

    else if (address >= 0x4020 && address <= 0xFFFF) {
        mapper_write(address, byte); // needs to be mapper.read
    }
}

*/