#include <stdlib.h>
#include "bus.h"

struct bus {
    Device *cartridge;
    //Device *io;
    //Device *ppu;
    Device *ram;
};

/*
uint8_t io_mem [(CARTRIDGE_START-IO_REGISTERS_START+1)];
uint8_t ppu_reg [(IO_REGISTERS_START-PPU_REGISTERS_START +1)]; //needs to be updated to be PPU registers

Device io = {&read_io, &write_io, io_mem};
*/
Bus bus_create(Device *cartridge, Device *ram) {
    Bus bus = malloc(sizeof(struct bus));
    bus->cartridge = cartridge;
    //bus->io = &io;
    //bus->ppu = ppu; //how to sync to ppu reg
    bus->ram = ram;
    return bus;
}


uint8_t bus_read(Bus bus, uint16_t address) {
    uint8_t byte;
    if(address >= CARTRIDGE_START && address < ADDR_RANGE_END) {
        byte = bus->cartridge->read(bus->cartridge->data, address);
    } else if(address < CARTRIDGE_START) {
        byte = bus->ram->read(bus->ram->data, address);
    }
    return byte;
}

void bus_write(Bus bus, uint16_t address, uint8_t byte) {
    if(address >= CARTRIDGE_START && address < ADDR_RANGE_END) {
        bus->cartridge->write(bus->cartridge->data, address, byte);
    } else if(address < CARTRIDGE_START) {
        bus->ram->write(bus->ram->data, address, byte);
    }
}

/*
void bus_write(Bus bus, uint16_t address, uint8_t byte) {
    if(address >= CARTRIDGE_START && address < ADDR_RANGE_END) {
        bus->cartridge->write(bus->cartridge->data, address, byte);
    } else if(address >= IO_REGISTERS_START && address < CARTRIDGE_START) {
        bus->io->write(bus->io->data, address, byte);
    } else if(address >= PPU_REGISTERS_START && address < IO_REGISTERS_START) {
        bus->ppu->write(bus->ppu->data, address, byte);
    }else if(address < PPU_REGISTERS_START) {
        bus->ram->write(bus->ram->data, address, byte);
    }
}

uint8_t read_ram(void *data, uint16_t address) {
    uint8_t *memory = (uint8_t *) data;
    uint8_t byte = memory[address & RAM_MASK];
    return byte;
}

void write_ram(void *data, uint16_t address, uint8_t byte) {
    uint8_t *memory = (uint8_t *) data;
    memory[address & RAM_MASK] = byte;
}

uint8_t read_io(void *data, uint16_t address) {
    uint8_t *memory = (uint8_t *) data;
    uint8_t byte = memory[address];
    return byte;
}

void write_io(void *data, uint16_t address, uint8_t byte) {
    uint8_t *memory = (uint8_t *) data;
    memory[address] = byte;
}

uint8_t read_ppu(void *data, uint16_t address) {
    uint8_t *memory = (uint8_t *) data;
    uint8_t byte = memory[address & PPU_REG_MASK];
    return byte;
}

void write_ppu(void *data, uint16_t address, uint8_t byte) {
    uint8_t *memory = (uint8_t *) data;
    memory[address & PPU_REG_MASK] = byte;
}



uint8_t bus_read_cpu(Bus bus, uint16_t address) {
    uint8_t byte;

}
*/
