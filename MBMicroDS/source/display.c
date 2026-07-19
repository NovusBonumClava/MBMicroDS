#include "display.h"
#include "cpu.h"
#include <string.h>

static uint16_t framebuffer[16][16];

void initDisplay(void) {
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    bgInit(0, BgType_Bmp16, BgSize_B16_256x256, 0, 1);
    clearDisplay();
}

void clearDisplay(void) {
    memset(framebuffer, 0, sizeof(framebuffer));
}

void updateDisplay(void) {
    uint16_t* vram = (uint16_t*)bgGetGfxPtr(0);
    
    for(int y = 0; y < 16; y++) {
        for(int x = 0; x < 16; x++) {
            uint16_t color = framebuffer[y][x] ? RGB15(31,31,31) : RGB15(0,0,0);
            for(int sy = 0; sy < 8; sy++) {
                for(int sx = 0; sx < 8; sx++) {
                    int dx = x * 8 + sx;
                    int dy = y * 8 + sy;
                    vram[dy * 256 + dx] = color;
                }
            }
        }
    }
}

void render_from_cpu(void) {
    for(int row = 0; row < 16; row++) {
        uint16_t row_data = 0;
        for(int col = 0; col < 16; col++) {
            if(cpu.P1 & (1 << col)) {
                row_data |= (1 << (15 - col));
            }
        }
        for(int col = 0; col < 16; col++) {
            framebuffer[row][col] = (row_data >> (15 - col)) & 1;
        }
    }
}