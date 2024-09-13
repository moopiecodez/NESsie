#include <stdio.h>
#include <stdint.h>

void loadgame(char *ROMfile) {
    FILE *fp; //pointer to file to be passed by nessie.c
 
    fp = fopen(ROMfile, "r");

    if(fp == NULL) {
        printf("Error: file could not be opened\n");
    } 
}