#include <stdio.h>
#include <stdint.h>

#define CPU_RAM_MIRROR 0x07FF
#define PPU_REG_MIRROR 0x0007

//would need to implement other I/O for DMA, controllers APU

uint8_t cpumemory[0xFFFF];

typedef uint8_t Mapper_read(uint16_t);
typedef void Mapper_write(uint16_t, uint8_t);

struct Mapper {
    Mapper_read *read;
    Mapper_write *write;
    // uint8_t (*mread)(uint16_t address);
    // void (*mwrite)(uint16_t address, uint8_t byte);
};


struct Header {
    int mapperno;
    int PRG_Banks_num;
    int CHR_Banks_num;
    int mirror;
};

struct Cartridge {
    struct Mapper *mapper;
    struct Header *head;
    uint8_t *PRG_Banks;
    uint8_t *CHR_Banks;

};

struct Mapper mapper0 = { };

uint8_t mapper0_read(uint16_t address) {

}

uint8_t cpu_read (uint16_t address) {
    uint8_t byte;
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
    return byte;
}

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