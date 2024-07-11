#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mappers.h"
#define HEADERSIZE 16
#define PRGBANKSIZE 16384
#define CHRBANKSIZE 8192
#define BYTE uint8_t

//allocate space for NES memory
BYTE memory[0xFFFF];

    
//size of PRG ROM in 16kb units
BYTE PRGROM_BANKS;
//size of CHR ROM in 8kb units, value 0 means board uses CHR RAM
BYTE CHRROM_BANKS;

//FLAGS 6 mapper, mirroring, battery, trainer
BYTE FLAGS_6;
BYTE FLAGS_7;
BYTE FLAGS_8;
BYTE FLAGS_9;
BYTE FLAGS_10;
//11-15 unused should be 0;
BYTE FLAGS_11;
BYTE FLAGS_12;
BYTE FLAGS_13;
BYTE FLAGS_14;
BYTE FLAGS_15;

//note in cpu file as well
unsigned int getBit(BYTE byte, int position) {
    BYTE mask = ~(~0 << 1);
    return (byte >> position) & mask;
}

void extractROMdata(FILE *, BYTE destination[], int destinationIndex, int size);


int main(int argc, char *argv[]) {
    
    
    
    FILE *fp;
    BYTE header [HEADERSIZE];

    if (argc == 1) {
        printf("Error no file given\n");
    }
    if (argc > 2) {
        printf("Error: too many arguments, please specify just one file\n");
    }
    else {
        if ((fp = fopen(*++argv, "r")) == NULL) {
            printf("Error: can't open %s\n", *argv);
            return 1;
        } else {
            extractROMdata(fp, header, 0, HEADERSIZE);
            //need to remember to close file when all extracted
            //fclose(fp);
        }
    }
    if (strstr(header, "NES") != NULL) {
        printf("%s has been found\n", header);
    } else {
        printf("%s has not been found\n", header);
    }   
    //check for constant in iNES format (NES followed by $1A)
    if (header[0] = 0x4E && header[1] == 0x45 && header[2] == 0x53 && header[3] == 0x1A) {
        printf("Constant has been found\n");
    }
    PRGROM_BANKS = header[4];
    CHRROM_BANKS = header[5];

    printf("%d\n", PRGROM_BANKS);
    printf("%d\n", CHRROM_BANKS);

    FLAGS_6 = header[6];
    unsigned int mirror;
    mirror = getBit(FLAGS_6, 0);
    if (mirror == 0) {
        printf("vertical arrangment / horizontal mirrored\n");
    } else {
        printf("horizontal arrangement/ vertical mirrored\n");
    }
    if (getBit(FLAGS_6, 1) == 1) {
        printf("Battery-backed PRG Ram/persistent memory");
    }
    if (getBit(FLAGS_6, 2) == 1) {
        printf("Contains a trainer");
    }
    BYTE low_mapper = FLAGS_6 >> 4;
    for (int i = 7; i >= 0; i--) {
        printf("%d", getBit(low_mapper, i));
    }
    printf("\n");

    FLAGS_7 = header[7];
    /*  bits 0-3 Playchoice-10, NES2.0 format (if bits 3 and 2 equal 2 then rest
        of flags in NES 2.0 format) and VS Unisystem reserved for
        future use and not used */
    BYTE high_mapper = FLAGS_7 >> 4;
    for (int i = 7; i >= 0; i--) {
        printf("%d", getBit(high_mapper, i));
    }
    printf("\n");

    BYTE mapper_number = (high_mapper << 4) + low_mapper;
    for (int i = 7; i >= 0; i--) {
        printf("%d", getBit(mapper_number, i));
    }

    //mapper_number = 16;
    char *mapper_name = mappers[mapper_number];
    if (mapper_name == NULL) {
        mapper_name = "undefined"; 
    }
    printf(" is mapper %s \n", mapper_name);

    /*size of PRG RAM in 8kb units, vlue 0 infers 8KB or 1 page of RAM, not 
    widely used. NES2.0 recommended to specify PRG RAM
    */
    FLAGS_8 = header[8];
    
    //TV system - rarely used
    FLAGS_9 = header[9];
    
    //TV system, PRG-RAM presence (rarely used)
    FLAGS_10 = header[10];

    FLAGS_11 = header[11];
    FLAGS_12 = header[12];
    FLAGS_13 = header[13];
    FLAGS_14 = header[14];
    FLAGS_15 = header[15];

    //deal with optional trainer
    if (getBit(FLAGS_6, 2) == 1) {
        //there is a trainer and first 512 bytes after header are the trainer
        // goes locations $7000-$71FF
        printf("%d\n",memory[0x7000]);
        extractROMdata(fp, memory, 0x7000, 512);
        printf("Now memory holds %d\n",memory[0x7000]);

    }

    //get PRG ROM data
    int PRG_ROM_Size = PRGBANKSIZE * PRGROM_BANKS;
    printf("PRG Banks %d\n",PRGROM_BANKS);

    BYTE PRG_ROM_Data[PRG_ROM_Size];
    printf("PRG ROM Size %d\n", PRG_ROM_Size);
    printf("Initial data %u\n",PRG_ROM_Data[PRG_ROM_Size -1]);

    extractROMdata(fp, PRG_ROM_Data, 0, PRG_ROM_Size);
    
    /*
    for(int i = 0; i < PRG_ROM_Size; i++) {
        printf("position %d is %x\n", i, PRG_ROM_Data[i]);
    }
    */    
    printf("Now data holds %x\n",PRG_ROM_Data[(PRG_ROM_Size -3)]);
    printf("Now data holds %x\n",PRG_ROM_Data[PRG_ROM_Size -1]);

    int CHR_ROM_Size = CHRBANKSIZE * CHRROM_BANKS;
    if (CHR_ROM_Size > 0) {
        printf("extracting CHR ROM\n");
        BYTE CHR_ROM_DATA[CHR_ROM_Size];
        extractROMdata(fp, CHR_ROM_DATA, 0, CHR_ROM_Size);
    } else {
        printf("no CHR data\n");
    }
    fclose(fp);

    //Playchoice ignored for now as not often used
    return 0;
}

void extractROMdata(FILE *ifp, BYTE destination[], int destinationIndex, int size) {
    int c;
    for (int i = destinationIndex; i < (destinationIndex + size) && (c =getc(ifp)) != EOF; i++) {
        // print in hex i and c
        //printf("%06d is %x\n", i, c);
        destination[i] = c;
        //need to go from start
    }
}