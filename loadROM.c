#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define HEADERSIZE 100
#define BYTE uint8_t

//size of PRG ROM in 16kb units
BYTE PRGROM_BANKS;
//size of CHR ROM in 8kb units, value 0 means board uses CHR RAM
BYTE CHRROM_BANKS;

//FLAGS 6 mapper, mirroring, battery, trainer
BYTE FLAGS_6;

BYTE FLAGS_7;
BYTE FLAGS_8;
BYTE FLAGS_9;

//note in cpu file as well
unsigned int getBit(BYTE byte, int position) {
    BYTE mask = ~(~0 << 1);
    return (byte >> position) & mask;
}

void extractHeader(FILE *, char contents[]);


int main(int argc, char *argv[]) {
    FILE *fp;
    char header [HEADERSIZE];

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
            extractHeader(fp, header);
            fclose(fp);
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
    if (getBit(FLAGS_6, 2) ==1) {
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
    printf("\n");

    FLAGS_8 = header[8];
    /*size of PRG RAM in 8kb units, vlue 0 infers 8KB or 1 page of RAM, not 
    widely used. NES2.0 recommended to specify PRG RAM
    */
    return 0;
}

void extractHeader(FILE *ifp, char contents[]) {
    int c;
    
    for (int i = 0; i< HEADERSIZE && (c =getc(ifp)) != EOF; i++) {
        contents[i] = c;
    }
}