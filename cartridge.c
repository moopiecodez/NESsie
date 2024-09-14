#include <stdio.h>
#include <stdint.h>
#include "cartridge.h"

#define HEADERSIZE 16

struct Header {
    uint8_t PRG_Banks_num;
    uint8_t CHR_Banks_num;
    uint8_t mapper_num;
    uint8_t battery; //0 means no battery backed RAM
    uint8_t mirror; // 0 means vertical arrangement / horizontal mirroring
    uint8_t trainer;
    uint8_t PRG_RAM;
};

uint8_t get_Bit(uint8_t byte, uint8_t position) {
    uint8_t mask = ~(~0 << 1);
    return (byte >> position) & mask;
}


void loadgame(char *ROMfile) {
    FILE *fp; //pointer to file to be passed by nessie.c
 
    fp = fopen(ROMfile, "r");

    if(fp == NULL) {
        printf("Error: file could not be opened\n");
    } else {
        extractHeader(fp);
    }
}

void extractHeader(FILE *fp) {
    int c;
    struct Header header;
    uint8_t Flags_6;
    uint8_t Flags_7;
    uint8_t Flags_8; //
    char str[HEADERSIZE];
    for(int i = 0; i < HEADERSIZE && (c = getc(fp)) != EOF; i++) {
        str[i] = c;
    }
    if(!(str[0] == 0x4E && str[1] == 0x45 && str[2] == 0x53 && str[3] == 0x1A)) {
        printf("Error: file not in iNES format\n");
    } else {
        header.PRG_Banks_num = str[4];  
        header.CHR_Banks_num = str[5];
        printf("ROM file contains %d PRG and %d CHR banks\n", header.PRG_Banks_num, header.CHR_Banks_num);

        Flags_6 = str[6];
        header.mirror = get_Bit(Flags_6, 0);
        if(header.mirror == 0) {printf("Vertical arrangement, horizontal mirrored\n");}
        header.battery = get_Bit(Flags_6, 1);
        if(header.battery != 0) {printf("Persistent memory present\n");}
        header.trainer = get_Bit(Flags_6, 2);
        if(header.trainer != 0) {printf("Trainer present\n");}
        uint8_t low_map = Flags_6 >> 4;

        Flags_7 = str[7];
        header.mapper_num = ((Flags_7 >> 4) << 4) + low_map;
        printf("Mapper number is: %d\n", header.mapper_num);
    }
}