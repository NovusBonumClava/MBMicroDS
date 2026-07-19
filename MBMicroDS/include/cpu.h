#ifndef CPU_H
#define CPU_H

#include <nds.h>

typedef struct {
    uint8_t A;
    uint8_t PSW;
    uint8_t R[8];
    uint16_t PC;
    uint8_t P1;
    uint8_t P2;
    uint8_t T;
    uint8_t ST;
    uint32_t cycle_count;
} CPU_8021;

extern CPU_8021 cpu;
extern uint8_t memory[4096];

void initCPU(void);
void cpuStep(void);
void resetCPU(void);

#endif