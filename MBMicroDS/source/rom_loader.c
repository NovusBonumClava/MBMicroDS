#include "rom_loader.h"
#include <stdio.h>
#include <fat.h>

uint32_t loadROM(const char* filename, uint8_t* buffer) {
    FILE* file = fopen(filename, "rb");
    if(!file) return 0;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    
    if(size > 4096) { // We have 4KB memory, but ROMs are 2KB
        fclose(file);
        return 0;
    }
    
    size_t read = fread(buffer, 1, size, file);
    fclose(file);
    
    return read;
}