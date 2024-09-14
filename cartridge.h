#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <stdio.h>
#include <stdint.h>

void loadgame(char *ROMfile);
struct Header *extractHeader(FILE *fp);

#endif