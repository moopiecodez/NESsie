#include <stdio.h>
#include <stdint.h>
#include "cartridge.h"

#define HEADERSIZE 16
#define PRGBANKSIZE 16384
#define CHRBANKSIZE 8192
#define TRAINERSIZE 512
#define CART_SIZE_MIN 0xBFDF //CPU memory range used by cartridge $4020 to $FFFF

struct Header {
    uint8_t PRG_Banks_num;
    uint8_t CHR_Banks_num; //CHR associated with PPU
    uint8_t mapper_num;
    uint8_t battery; //0 means no battery backed RAM
    uint8_t mirror; // 0 means vertical arrangement / horizontal mirroring
    uint8_t trainer;
    uint8_t PRG_RAM;
};

struct Cartridge {
    struct Header *head;
    uint8_t *CPU_ROM; //contains PRG ROM
    uint8_t *PPU_ROM; //constains CHR ROM
    void (*mapper_write)(uint8_t *data, uint16_t address, uint8_t byte);
    uint8_t (*mapper_read)(uint8_t *data, uint16_t address);

};

uint8_t get_Bit(uint8_t byte, uint8_t position) {
    uint8_t mask = ~(~0 << 1);
    return (byte >> position) & mask;
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

void loadgame(char *ROMfile) {
    FILE *fp; //pointer to file to be passed by nessie.c
    struct Header *hdr;
    int cart_size = CART_SIZE_MIN;
    struct Cartridge *cart;
    fp = fopen(ROMfile, "r");
    if(fp == NULL) {
        printf("Error: file could not be opened\n");
    } else {
        hdr = extractHeader(fp);
        if(hdr == NULL) {
            printf("Error: header not loaded\n");
        } else {
            int extraPRGBanks = hdr->PRG_Banks_num - 2;
            if(extraPRGBanks > 0) {
                cart_size += CART_SIZE_MIN + (extraPRGBanks * PRGBANKSIZE);
            }
            uint8_t PRGROM[PRGBANKSIZE]; //donkeykong is simple
            // cart_mem
            // if(hdr->trainer != 0) {
            //     extractData(fp);
            // }; // load cartridge data
        }
    }
}

struct Header *extractHeader(FILE *fp) {
    int c;
    struct Header header;
    struct Header *ptr;
    uint8_t Flags_6;
    uint8_t Flags_7;
    uint8_t Flags_8;
    char str[HEADERSIZE];
    for(int i = 0; i < HEADERSIZE && (c = getc(fp)) != EOF; i++) { //extracts 16 byte header from file
        str[i] = c;
    }
    if(!(str[0] == 0x4E && str[1] == 0x45 && str[2] == 0x53 && str[3] == 0x1A)) {
        printf("Error: file not in iNES format\n");
        ptr = NULL;
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

        Flags_8 = str[8]; //Size of PRG RAM in 8KB units, 0 infers 8KB
        if(Flags_8 == 0) {
            header.PRG_RAM = 1;
        } else {
            header.PRG_RAM = Flags_8;
        }
        ptr = &header;
    }
    return ptr;
}



//7000-$71FF where trainer goes if present
//trainer before PRG ram if present
//6000 to 7fff prg ram