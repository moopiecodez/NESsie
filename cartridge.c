#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cartridge.h"

#define HEADERSIZE 16
#define PRGBANKSIZE 16384
#define CHRBANKSIZE 8192
#define TRAINERSIZE 512
#define CART_SIZE_MIN 0x10000 - 0x4020 //CPU memory range used by cartridge
#define PRGROM_LOW_START 0x8000
#define PRGROMSIZE 0x10000 - PRGROM_LOW_START //PRG ROM memory range

const uint8_t ines[] = { 0x4E, 0x45, 0x53, 0x1A };

struct header {
    uint8_t prg_rom_size;
    uint8_t chr_rom_size; //CHR associated with PPU
    uint8_t mirror; // 0 means vertical arrangement / horizontal mirroring
    uint8_t battery; //0 means no battery backed RAM
    uint8_t trainer;
    uint8_t mapper_num;
    uint8_t PRG_RAM;
};

struct cartridge {
    struct header header;
    uint8_t *PPU_ROM; //constains CHR ROM
    uint8_t PRGROM[PRGROMSIZE]; //PRG ROM addressing space
    // void (*mapper_write)(uint8_t *data, uint16_t address, uint8_t byte);
    // uint8_t (*mapper_read)(uint8_t *data, uint16_t address);
};

size_t cartridge_size();
void loadgame(char *ROMfile);
struct header *extractHeader(FILE *fp);

uint8_t get_bit(uint8_t byte, uint8_t position) {
    uint8_t mask = ~(~0 << 1);
    return (byte >> position) & mask;
}

size_t cartridge_size() {
    size_t size;
    size = sizeof(struct cartridge);
    return size;
}

/* Attempts to open a file, returns error message and closes program if it fails*/
FILE *romfile_open(char *filename) {
    FILE *fp;
    // printf("%s\n", filename);
    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Error: file could not be opened\n");
        exit(1);
    }
    return fp;
}

/*
Attempts to parse header (first 16 bytes) from file, returns error message and
closes program if format incorrect. See iNES header format description for
details
*/
struct header romfile_extract_header(FILE *fp) {
    uint8_t data[HEADERSIZE];
    size_t read = fread(&data, sizeof(uint8_t), HEADERSIZE, fp);

    //checks that iNES constant is present
    if (read < HEADERSIZE || memcmp(data, ines, sizeof(ines)) != 0) {
        printf("Error: file not in iNES format\n");
        exit(1);
    }

    struct header header = {
        data[4],
        data[5],
        get_bit(data[6], 0),
        get_bit(data[6], 1),
        get_bit(data[6], 2),
        (data[6] >> 4) + ((data[7] >> 4) << 4),
        (data[8] == 0) ? 1 : data[8]
    };

    printf("ROM file contains %d PRG and %d CHR banks\n", header.prg_rom_size, header.chr_rom_size);
    if(header.mirror == 0) {
        printf("Vertical arrangement, horizontal mirrored\n");
    }
    if(header.battery != 0) {
        printf("Persistent memory present\n");
    }
    if(header.trainer != 0) {
        printf("Trainer present\n");
    }
    printf("Mapper number is: %d\n", header.mapper_num);

    return header;
}

Cartridge cartridge_load(char *filename) {
    FILE *fp = romfile_open(filename);
    Cartridge cartridge = malloc(cartridge_size());
    cartridge->header = romfile_extract_header(fp);

    return cartridge;
}

uint8_t cartridge_read(Cartridge cartridge, uint16_t address) {
    uint8_t byte;
    if(address >= PRGROM_LOW_START && address <= 0xFFFF) {
        uint16_t adjusted_addr = address - PRGROM_LOW_START;
        byte = cartridge->PRGROM[adjusted_addr];
    }
    return byte;
}

void loadgame(char *ROMfile) {
    /*
    int cart_size = CART_SIZE_MIN;
    struct cartridge *cart;

    hdr = extractHeader(fp);
    int extraPRGBanks = hdr->PRG_Banks_num - 2;
    if(extraPRGBanks > 0) {
        cart_size += CART_SIZE_MIN + (extraPRGBanks * PRGBANKSIZE);
    }
    uint8_t PRGROM[PRGBANKSIZE]; //donkeykong is simple
    // cart_mem
    // if(hdr->trainer != 0) {
    //     extractData(fp);
    // }; // load cartridge data
    */
}


// void extractROMdata(FILE *ifp, BYTE destination[], int destinationIndex, int size) {
//     int c;
//     for (int i = destinationIndex; i < (destinationIndex + size) && (c =getc(ifp)) != EOF; i++) {
//         // print in hex i and c
//         //printf("%06d is %x\n", i, c);
//         destination[i] = c;
//         //need to go from start
//     }
// }


//7000-$71FF where trainer goes if present
//trainer before PRG ram if present
//6000 to 7fff prg ram