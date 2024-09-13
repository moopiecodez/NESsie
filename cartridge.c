#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mappers.h"
#define HEADERSIZE 16
#define PRGBANKSIZE 16384
#define CHRBANKSIZE 8192
#define BYTE uint8_t

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
    char *headerstring;
    int mapperno;
    int PRG_Banks_num;
    int CHR_Banks_num;
    int mirror;
};

struct Cartridge {
    struct Header *header; //do these need to be pointers?
    union Mappers *mapper;
    uint8_t *PRG_Banks;
    uint8_t *CHR_Banks;
};

struct Header *extractHeader(FILE *fp, struct Header *header) {
    int c;
    struct Header *hdr;
    for( int i = 0; i < HEADERSIZE && (c = getc(fp)) != EOF; i++ ) {
        header->headerstring[i] = c;
    }
    if(!(header->headerstring[0] = 0x4E && 
        header->headerstring[1] == 0x45 && 
        header->headerstring[2] == 0x53 && 
        header->headerstring[3] == 0x1A)) {
            // hdr = NULL;
            // return hdr; //check how to return this problem
            return NULL;
        }
}

struct Cartridge *loadgame(char *ROMfile) {
    FILE *fp; //pointer to file to be passed by nessie.c
    struct Header *header = malloc(sizeof(struct Header)); // does this need to be released?
    
    fp = fopen(ROMfile, "r");

    if(fp == NULL) {
        printf("Error: can't opeen ROMfile: %s\n", ROMfile);
        return NULL; // as return type is cartridge need to return null?
    } else {
        extractHeader(fp, header);
    }

}