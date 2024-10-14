#include <stdlib.h>
#include "bus.h"

struct bus {
    Device *cartridge;
    //Device *io;
    //Device *ppu;
    Device *ram;
};

Bus bus_create(Device *cartridge, Device *ram) {
    Bus bus = malloc(sizeof(struct bus));
    bus->cartridge = cartridge;
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
