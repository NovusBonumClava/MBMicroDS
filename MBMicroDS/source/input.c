#include "input.h"

static uint8_t mvKeys = 0;

void initInput(void) {
    touchInit();
}

void scanInput(void) {
    scanKeys();
    uint16_t keys = keysDown();
    
    mvKeys = 0;
    if(keys & KEY_A) mvKeys |= 0x01;
    if(keys & KEY_B) mvKeys |= 0x02;
    if(keys & KEY_SELECT) mvKeys |= 0x04;
    if(keys & KEY_START) mvKeys |= 0x08;
    if(keys & KEY_UP) mvKeys |= 0x10;
    if(keys & KEY_DOWN) mvKeys |= 0x20;
    if(keys & KEY_LEFT) mvKeys |= 0x40;
    if(keys & KEY_RIGHT) mvKeys |= 0x80;
}

uint8_t getMVKeys(void) {
    return mvKeys;
}