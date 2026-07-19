#include <nds.h>
#include <stdio.h>
#include <fat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "cpu.h"
#include "display.h"
#include "input.h"
#include "rom_loader.h"

#define MAX_ROMS 32
#define ROM_PATH "roms/"

char rom_list[MAX_ROMS][256];
int rom_count = 0;
int selected = 0;

void scan_roms(void) {
    DIR *dir;
    struct dirent *entry;
    
    rom_count = 0;
    dir = opendir(ROM_PATH);
    if(!dir) {
        iprintf("No roms/ folder found!\n");
        return;
    }
    
    while((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_REG) {
            char *ext = strrchr(entry->d_name, '.');
            if(ext && strcasecmp(ext, ".bin") == 0) {
                if(rom_count < MAX_ROMS) {
                    strcpy(rom_list[rom_count], entry->d_name);
                    rom_count++;
                }
            }
        }
    }
    closedir(dir);
}

void draw_menu(void) {
    iprintf("\x1b[2J");
    iprintf("\x1b[1;1H");
    iprintf("MBMicroDS - ROM Selector\n");
    iprintf("========================\n");
    
    if(rom_count == 0) {
        iprintf("No .bin ROMs found in roms/\n");
        iprintf("Place your ROMs there.\n");
        return;
    }
    
    for(int i = 0; i < rom_count; i++) {
        if(i == selected)
            iprintf("> ");
        else
            iprintf("  ");
        iprintf("%s\n", rom_list[i]);
    }
    iprintf("\nPress A to load, START to exit\n");
}

int main(void) {
    consoleDemoInit();
    
    iprintf("MBMicroDS v0.1\n");
    iprintf("Initializing FAT...\n");
    
    if(!fatInitDefault()) {
        iprintf("FAT init failed!\n");
        iprintf("Insert SD card\n");
        while(1) swiWaitForVBlank();
    }
    
    scan_roms();
    draw_menu();
    
    while(1) {
        scanKeys();
        uint16_t keys = keysDown();
        
        if(keys & KEY_UP) {
            if(selected > 0) selected--;
            draw_menu();
        }
        if(keys & KEY_DOWN) {
            if(selected < rom_count - 1) selected++;
            draw_menu();
        }
        if(keys & KEY_A) {
            if(rom_count > 0) break;
        }
        if(keys & KEY_START) {
            return 0;
        }
        swiWaitForVBlank();
    }
    
    char fullpath[300];
    sprintf(fullpath, "%s%s", ROM_PATH, rom_list[selected]);
    
    iprintf("\nLoading %s...\n", rom_list[selected]);
    
    uint32_t size = loadROM(fullpath, memory);
    if(size == 0) {
        iprintf("Load failed!\n");
        while(1) swiWaitForVBlank();
    }
    
    iprintf("Loaded %d bytes\n", size);
    iprintf("Starting emulation...\n");
    
    initCPU();
    initDisplay();
    initInput();
    
    // Clear console (optional)
    iprintf("\x1b[2J");
    iprintf("Running... Press START to exit\n");
    
    while(1) {
        scanInput();
        
        for(int i = 0; i < 1000; i++) {
            cpuStep();
        }
        
        render_from_cpu();
        updateDisplay();
        swiWaitForVBlank();
        
        if(keysDown() & KEY_START) break;
    }
    
    return 0;
}