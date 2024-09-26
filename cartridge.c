#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cartridge.h"

#define HEADERSIZE 16
#define PRGBANKSIZE 16384
#define CHRBANKSIZE 8192
#define TRAINERSIZE 512
#define PRGROM_LOW_START 0x8000
#define PRGROM_HIGH_START 0xC000
#define ADDR_RANGE_END 0x10000
#define PRGROMSIZE ADDR_RANGE_END - PRGROM_LOW_START //PRG ROM memory range

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

typedef uint8_t *RomBank;

typedef struct mapper {
    int id;
    char *name;
    void (*initialise)(Cartridge);
    uint8_t (*read)(void *, uint16_t);
    void (*write)(void *, uint16_t, uint8_t);
} Mapper;

struct cartridge {
    struct header header;
    RomBank *bank_prg; //array of pointers to PRG ROM banks
    RomBank *bank_chr; //array of pointers to CHR ROM banks
    Mapper mapper;
    RomBank prg_low;
    RomBank prg_high;
};


void map_000_write(void *, uint16_t, uint8_t);
uint8_t map_000_read(void *, uint16_t);
void map_000_init(Cartridge);
size_t cartridge_size();
struct header *extractHeader(FILE *fp);
Mapper mapper_get(uint8_t);

Mapper mapper[] = {
    {0, "NROM", map_000_init, map_000_read, map_000_write }
};

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
void romfile_extract_header(FILE *fp, Cartridge cartridge) {
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

    printf("Mapper number is: %d\n", header.mapper_num);

    cartridge->header = header;
}

/*
Placeholder for handling of trainers which is not yet implemented.
Emulator currently only handles ROMs without trainers.
*/
void romfile_extract_trainer(FILE *fp, Cartridge cartridge) {
    if(cartridge->header.trainer != 0) {
        printf("Trainer present, handling trainers not implemented. Exiting\n");
        exit(1);
    }
}

/*
Attempts to read PRG ROM banks.
*/
void romfile_extract_prg_rom(FILE *fp, Cartridge cartridge) {
    int num_of_banks = cartridge->header.prg_rom_size;
    cartridge->bank_prg = malloc(sizeof(RomBank) * num_of_banks);
    for(int i = 0; i < num_of_banks; i++) {
        cartridge->bank_prg[i] = malloc(PRGBANKSIZE); //conveniently works because we want bytes
        int read = fread(cartridge->bank_prg[i], sizeof(uint8_t), PRGBANKSIZE, fp);
        if(read != PRGBANKSIZE) {
            printf("Error: insufficient ROM data, bytes read: %d\n", read);
            exit(1);
        }
    }
}

/*
Attempts to read CHR ROM banks.
*/
void romfile_extract_chr_rom(FILE *fp, Cartridge cartridge) {
    int num_of_banks = cartridge->header.chr_rom_size;
    cartridge->bank_chr = malloc(sizeof(RomBank) * num_of_banks);
    for(int i = 0; i < num_of_banks; i++) {
        cartridge->bank_chr[i] = malloc(CHRBANKSIZE); //conveniently works because we want bytes
        int read = fread(cartridge->bank_chr[i], sizeof(uint8_t), CHRBANKSIZE, fp);
        if(read != CHRBANKSIZE) {
            printf("Error: insufficient ROM data, bytes read: %d\n", read);
            exit(1);
        }
    }
}

void map_000_write(void *data, uint16_t address, uint8_t byte) {
    // Cartridge cartridge = (Cartridge) data;
    if(address >= PRGROM_LOW_START && address < ADDR_RANGE_END) {
        // printf("PRG ROM: Unexpected write\n");
    };
}

uint8_t map_000_read(void *data, uint16_t address) {
    uint8_t byte;
    uint16_t bank_addr;
    Cartridge cartridge = (Cartridge) data;
    if(address >= PRGROM_LOW_START && address < PRGROM_HIGH_START) {
        bank_addr = address - PRGROM_LOW_START;
        byte = cartridge->prg_low[bank_addr];
    } else if(address >= PRGROM_HIGH_START && address < ADDR_RANGE_END) {
        bank_addr = address - PRGROM_HIGH_START;
        byte = cartridge->prg_high[bank_addr];
    }
    return byte;
}

void map_000_init(Cartridge cartridge) {
    cartridge->prg_low = cartridge->bank_prg[0];
    cartridge->prg_high = cartridge->bank_prg[0];
}

void set_mapper(Cartridge cartridge) {
    cartridge->mapper = mapper_get(cartridge->header.mapper_num);
    cartridge->mapper.initialise(cartridge);
}

Device cartridge_load(char *filename) {
    FILE *fp = romfile_open(filename);
    Cartridge data = malloc(cartridge_size());
    romfile_extract_header(fp, data);
    romfile_extract_trainer(fp, data);
    romfile_extract_prg_rom(fp, data);
    romfile_extract_chr_rom(fp, data);
    fclose(fp);

    set_mapper(data);

    Device cartridge = {
        data->mapper.read,
        data->mapper.write,
        data
    };

    return cartridge;
}

Mapper mapper_get(uint8_t mapper_num) {
    return mapper[mapper_num];
}




//7000-$71FF where trainer goes if present
//trainer before PRG ram if present
//6000 to 7fff prg ram

// char *mappers[] = {
//     "NROM",
//     "UnROM switch",
//     "CNROM switch",
//     "MMC3",
//     "MMC5",
//     "FFE F4xxx",
//     "AOROM switch",
//     "FFE F3xxx",
//     "MMC2",
//     "MMC4",
//     "ColorDreams chip",
//     "FFE F6xxx",
//     "CPROM switch",
//     '\0',
//     "100-in-1 switch",
//     "Bandai chip",
//     "FFE F8xxx",
//     "Jaleco SS8806",
//     "Namcot 106",
//     "Nintendo DiskSystem",
//     "Konami VRC4a",
//     "Konami VRC2a",
//     "Konami VRC4a?",
//     "Konami VRC6",
//     "Konami VRC4b",
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     "Irem G-101 chip",
//     "Taito TC0190/TC0350",
//     "Nina-1",
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     "Tengen RAMBO-1 chip",
//     "Irem H-3001 chip",
//     "GNROM switch",
//     "SunSoft3 chip",
//     "SunSoft4 chip",
//     "SunSoft5 FME-7 chip",
//     '\0',
//     "Camerica chip",
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     "Irem 74HC161/32-based",
//     "AVE Nina-3 board",
//     '\0',
//     "AVE Nina-6 board",
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     '\0',
//     "Pirate HK-SF3 chip"
//      };
