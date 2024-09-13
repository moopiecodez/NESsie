#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <stdio.h>
#include <stdint.h>

void loadgame(char *ROMfile);
void extractHeader(FILE *fp);

#endif