#include <stdio.h>
#include <stdint.h>
#include "cartridge.h"

#define HEADERSIZE 16
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
    char str[HEADERSIZE];
    for(int i = 0; i < HEADERSIZE && (c = getc(fp)) != EOF; i++) {
        str[i] = c;
    }
    if(!(str[0] == 0x4E && str[1] == 0x45 && str[2] == 0x53 && str[3] == 0x1A)) {
        printf("Error: file not in iNES format\n");
    }
}