#ifndef ROM_LOADER_H
#define ROM_LOADER_H

#include <nds.h>

uint32_t loadROM(const char* filename, uint8_t* buffer);

#endif