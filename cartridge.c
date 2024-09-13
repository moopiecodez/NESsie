#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mappers.h"
#define HEADERSIZE 16
#define PRGBANKSIZE 16384
#define CHRBANKSIZE 8192
#define BYTE uint8_t

unsigned int getBit(uint8_t byte, int position) {
    uint8_t mask = ~(~0 << 1);
    return (byte >> position) & mask;
}

typedef uint8_t Mapper_read(uint16_t);
typedef void Mapper_write(uint16_t, uint8_t);

struct Mapper {
    Mapper_read *read;
    Mapper_write *write;
    // uint8_t (*mread)(uint16_t address);
    // void (*mwrite)(uint16_t address, uint8_t byte);
};

uint8_t NROMread(uint16_t address) {
    return 3;
}
void NROMwrite(uint16_t address, uint8_t byte);

struct Mapper mapper[1] = {{NROMread, NROMwrite}};

struct Header {
    int mapperno;
    int PRG_Banks_num;
    int CHR_Banks_num;
    int mirror; // 0 means vertical arrangement / horizontal mirroring
};

struct Cartridge {
    struct Header *header; //do these need to be pointers?
    union Mappers *mapper;
    uint8_t *PRG_Banks;
    uint8_t *CHR_Banks;
};

struct Header *extractHeader(FILE *fp) {
    int c;
    char *string;
    uint8_t Flags_6;
    uint8_t Flags_7;
    struct Header header;
    for( int i = 0; i < HEADERSIZE && (c = getc(fp)) != EOF; i++ ) {
        string[i] = c;
    }
    if(!(string[0] = 0x4E && 
        string[1] == 0x45 && 
        string[2] == 0x53 && 
        string[3] == 0x1A)) {
            // hdr = NULL;
            // return hdr; //check how to return this problem
            return NULL;
    } else {
        header.PRG_Banks_num = string[4];
        header.CHR_Banks_num = string[5];
        Flags_6 = string[6];
        Flags_7 = string[7];
        header.mirror = getBit(Flags_6, 0);
    }
    
}

struct Cartridge *loadgame(char *ROMfile) {
    FILE *fp; //pointer to file to be passed by nessie.c
    // struct Header *header = malloc(sizeof(struct Header)); // does this need to be released?
    
    fp = fopen(ROMfile, "r");

    if(fp == NULL) {
        printf("Error: can't opeen ROMfile: %s\n", ROMfile);
        return NULL; // as return type is cartridge need to return null?
    } else {
        extractHeader(fp);
    }

}