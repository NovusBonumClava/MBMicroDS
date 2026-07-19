#include "cpu.h"
#include <string.h>

CPU_8021 cpu;
uint8_t memory[4096];

static void op_nop(void) {}
static void op_inc_a(void) { cpu.A++; }
static void op_dec_a(void) { cpu.A--; }
static void op_com_a(void) { cpu.A = ~cpu.A; }
static void op_clr_a(void) { cpu.A = 0; }
static void op_set_a(void) { cpu.A = 0xFF; }

static void op_inc_r0(void) { cpu.R[0]++; }
static void op_inc_r1(void) { cpu.R[1]++; }
static void op_inc_r2(void) { cpu.R[2]++; }
static void op_inc_r3(void) { cpu.R[3]++; }
static void op_inc_r4(void) { cpu.R[4]++; }
static void op_inc_r5(void) { cpu.R[5]++; }
static void op_inc_r6(void) { cpu.R[6]++; }
static void op_inc_r7(void) { cpu.R[7]++; }

static void op_dec_r0(void) { cpu.R[0]--; }
static void op_dec_r1(void) { cpu.R[1]--; }
static void op_dec_r2(void) { cpu.R[2]--; }
static void op_dec_r3(void) { cpu.R[3]--; }
static void op_dec_r4(void) { cpu.R[4]--; }
static void op_dec_r5(void) { cpu.R[5]--; }
static void op_dec_r6(void) { cpu.R[6]--; }
static void op_dec_r7(void) { cpu.R[7]--; }

static void op_clr_r0(void) { cpu.R[0] = 0; }
static void op_clr_r1(void) { cpu.R[1] = 0; }
static void op_clr_r2(void) { cpu.R[2] = 0; }
static void op_clr_r3(void) { cpu.R[3] = 0; }
static void op_clr_r4(void) { cpu.R[4] = 0; }
static void op_clr_r5(void) { cpu.R[5] = 0; }
static void op_clr_r6(void) { cpu.R[6] = 0; }
static void op_clr_r7(void) { cpu.R[7] = 0; }

static void op_set_r0(void) { cpu.R[0] = 0xFF; }
static void op_set_r1(void) { cpu.R[1] = 0xFF; }
static void op_set_r2(void) { cpu.R[2] = 0xFF; }
static void op_set_r3(void) { cpu.R[3] = 0xFF; }
static void op_set_r4(void) { cpu.R[4] = 0xFF; }
static void op_set_r5(void) { cpu.R[5] = 0xFF; }
static void op_set_r6(void) { cpu.R[6] = 0xFF; }
static void op_set_r7(void) { cpu.R[7] = 0xFF; }

static void op_add_r0(void) { uint8_t r = cpu.A + cpu.R[0]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_add_r1(void) { uint8_t r = cpu.A + cpu.R[1]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_add_r2(void) { uint8_t r = cpu.A + cpu.R[2]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_add_r3(void) { uint8_t r = cpu.A + cpu.R[3]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_add_r4(void) { uint8_t r = cpu.A + cpu.R[4]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_add_r5(void) { uint8_t r = cpu.A + cpu.R[5]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_add_r6(void) { uint8_t r = cpu.A + cpu.R[6]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_add_r7(void) { uint8_t r = cpu.A + cpu.R[7]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }

static void op_sub_r0(void) { uint8_t r = cpu.A - cpu.R[0]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[0]) cpu.PSW |= 0x20; cpu.A = r; }
static void op_sub_r1(void) { uint8_t r = cpu.A - cpu.R[1]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[1]) cpu.PSW |= 0x20; cpu.A = r; }
static void op_sub_r2(void) { uint8_t r = cpu.A - cpu.R[2]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[2]) cpu.PSW |= 0x20; cpu.A = r; }
static void op_sub_r3(void) { uint8_t r = cpu.A - cpu.R[3]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[3]) cpu.PSW |= 0x20; cpu.A = r; }
static void op_sub_r4(void) { uint8_t r = cpu.A - cpu.R[4]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[4]) cpu.PSW |= 0x20; cpu.A = r; }
static void op_sub_r5(void) { uint8_t r = cpu.A - cpu.R[5]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[5]) cpu.PSW |= 0x20; cpu.A = r; }
static void op_sub_r6(void) { uint8_t r = cpu.A - cpu.R[6]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[6]) cpu.PSW |= 0x20; cpu.A = r; }
static void op_sub_r7(void) { uint8_t r = cpu.A - cpu.R[7]; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < cpu.R[7]) cpu.PSW |= 0x20; cpu.A = r; }

static void op_and_r0(void) { cpu.A &= cpu.R[0]; }
static void op_and_r1(void) { cpu.A &= cpu.R[1]; }
static void op_and_r2(void) { cpu.A &= cpu.R[2]; }
static void op_and_r3(void) { cpu.A &= cpu.R[3]; }
static void op_and_r4(void) { cpu.A &= cpu.R[4]; }
static void op_and_r5(void) { cpu.A &= cpu.R[5]; }
static void op_and_r6(void) { cpu.A &= cpu.R[6]; }
static void op_and_r7(void) { cpu.A &= cpu.R[7]; }

static void op_or_r0(void) { cpu.A |= cpu.R[0]; }
static void op_or_r1(void) { cpu.A |= cpu.R[1]; }
static void op_or_r2(void) { cpu.A |= cpu.R[2]; }
static void op_or_r3(void) { cpu.A |= cpu.R[3]; }
static void op_or_r4(void) { cpu.A |= cpu.R[4]; }
static void op_or_r5(void) { cpu.A |= cpu.R[5]; }
static void op_or_r6(void) { cpu.A |= cpu.R[6]; }
static void op_or_r7(void) { cpu.A |= cpu.R[7]; }

static void op_xor_r0(void) { cpu.A ^= cpu.R[0]; }
static void op_xor_r1(void) { cpu.A ^= cpu.R[1]; }
static void op_xor_r2(void) { cpu.A ^= cpu.R[2]; }
static void op_xor_r3(void) { cpu.A ^= cpu.R[3]; }
static void op_xor_r4(void) { cpu.A ^= cpu.R[4]; }
static void op_xor_r5(void) { cpu.A ^= cpu.R[5]; }
static void op_xor_r6(void) { cpu.A ^= cpu.R[6]; }
static void op_xor_r7(void) { cpu.A ^= cpu.R[7]; }

static void op_rr_a(void) { cpu.A = cpu.A >> 1; }
static void op_rl_a(void) { cpu.A = cpu.A << 1; }
static void op_rr4_a(void) { cpu.A = cpu.A >> 4; }
static void op_rl4_a(void) { cpu.A = cpu.A << 4; }

static void op_mov_a_r0(void) { cpu.A = cpu.R[0]; }
static void op_mov_a_r1(void) { cpu.A = cpu.R[1]; }
static void op_mov_a_r2(void) { cpu.A = cpu.R[2]; }
static void op_mov_a_r3(void) { cpu.A = cpu.R[3]; }
static void op_mov_a_r4(void) { cpu.A = cpu.R[4]; }
static void op_mov_a_r5(void) { cpu.A = cpu.R[5]; }
static void op_mov_a_r6(void) { cpu.A = cpu.R[6]; }
static void op_mov_a_r7(void) { cpu.A = cpu.R[7]; }

static void op_mov_r0_a(void) { cpu.R[0] = cpu.A; }
static void op_mov_r1_a(void) { cpu.R[1] = cpu.A; }
static void op_mov_r2_a(void) { cpu.R[2] = cpu.A; }
static void op_mov_r3_a(void) { cpu.R[3] = cpu.A; }
static void op_mov_r4_a(void) { cpu.R[4] = cpu.A; }
static void op_mov_r5_a(void) { cpu.R[5] = cpu.A; }
static void op_mov_r6_a(void) { cpu.R[6] = cpu.A; }
static void op_mov_r7_a(void) { cpu.R[7] = cpu.A; }

static void op_mov_a_imm(void) { cpu.A = memory[cpu.PC++]; }
static void op_add_imm(void) { uint8_t v = memory[cpu.PC++]; uint8_t r = cpu.A + v; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(r < cpu.A) cpu.PSW |= 0x20; cpu.A = r; }
static void op_and_imm(void) { cpu.A &= memory[cpu.PC++]; }
static void op_or_imm(void) { cpu.A |= memory[cpu.PC++]; }
static void op_xor_imm(void) { cpu.A ^= memory[cpu.PC++]; }
static void op_sub_imm(void) { uint8_t v = memory[cpu.PC++]; uint8_t r = cpu.A - v; cpu.PSW = 0; if(r == 0) cpu.PSW |= 0x40; if(r & 0x80) cpu.PSW |= 0x80; if(cpu.A < v) cpu.PSW |= 0x20; cpu.A = r; }

static void op_jmp(void) { uint16_t addr = (memory[cpu.PC+1] << 8) | memory[cpu.PC]; cpu.PC += 2; cpu.PC = addr; }
static void op_jz(void) { uint16_t addr = (memory[cpu.PC+1] << 8) | memory[cpu.PC]; cpu.PC += 2; if(cpu.A == 0) cpu.PC = addr; }
static void op_jnz(void) { uint16_t addr = (memory[cpu.PC+1] << 8) | memory[cpu.PC]; cpu.PC += 2; if(cpu.A != 0) cpu.PC = addr; }

static void op_call(void) { cpu.ST++; memory[0x800 | cpu.ST] = cpu.PC & 0xFF; cpu.ST++; memory[0x800 | cpu.ST] = (cpu.PC >> 8) & 0xFF; uint8_t addr = memory[cpu.PC++]; cpu.PC = (cpu.PC & 0xFF00) | addr; }
static void op_ret(void) { cpu.PC = memory[0x800 | cpu.ST] | (memory[0x800 | cpu.ST-1] << 8); cpu.ST -= 2; }

static void op_stc(void) { cpu.PSW |= 0x20; }
static void op_clc(void) { cpu.PSW &= ~0x20; }
static void op_cmc(void) { cpu.PSW ^= 0x20; }
static void op_sts(void) { cpu.PSW |= 0x80; }
static void op_cls(void) { cpu.PSW &= ~0x80; }

static void op_out_p1(void) { cpu.P1 = cpu.A; }
static void op_out_p2(void) { cpu.P2 = cpu.A; }
static void op_in_p1(void) { cpu.A = cpu.P1; }
static void op_in_p2(void) { cpu.A = cpu.P2; }

static void op_mov_t_a(void) { cpu.T = cpu.A; }
static void op_mov_a_t(void) { cpu.A = cpu.T; }

static void (*op_table[256])(void);

void initCPU(void) {
    memset(&cpu, 0, sizeof(CPU_8021));
    memset(memory, 0, sizeof(memory));
    
    for(int i = 0; i < 256; i++) op_table[i] = op_nop;
    
    op_table[0x00] = op_nop;
    op_table[0x01] = op_inc_a;
    op_table[0x02] = op_dec_a;
    op_table[0x03] = op_com_a;
    op_table[0x04] = op_clr_a;
    op_table[0x05] = op_set_a;
    
    op_table[0x06] = op_inc_r0; op_table[0x07] = op_inc_r1;
    op_table[0x08] = op_inc_r2; op_table[0x09] = op_inc_r3;
    op_table[0x0A] = op_inc_r4; op_table[0x0B] = op_inc_r5;
    op_table[0x0C] = op_inc_r6; op_table[0x0D] = op_inc_r7;
    
    op_table[0x0E] = op_dec_r0; op_table[0x0F] = op_dec_r1;
    op_table[0x10] = op_dec_r2; op_table[0x11] = op_dec_r3;
    op_table[0x12] = op_dec_r4; op_table[0x13] = op_dec_r5;
    op_table[0x14] = op_dec_r6; op_table[0x15] = op_dec_r7;
    
    op_table[0x16] = op_clr_r0; op_table[0x17] = op_clr_r1;
    op_table[0x18] = op_clr_r2; op_table[0x19] = op_clr_r3;
    op_table[0x1A] = op_clr_r4; op_table[0x1B] = op_clr_r5;
    op_table[0x1C] = op_clr_r6; op_table[0x1D] = op_clr_r7;
    
    op_table[0x1E] = op_set_r0; op_table[0x1F] = op_set_r1;
    op_table[0x20] = op_set_r2; op_table[0x21] = op_set_r3;
    op_table[0x22] = op_set_r4; op_table[0x23] = op_set_r5;
    op_table[0x24] = op_set_r6; op_table[0x25] = op_set_r7;
    
    op_table[0x26] = op_add_r0; op_table[0x27] = op_add_r1;
    op_table[0x28] = op_add_r2; op_table[0x29] = op_add_r3;
    op_table[0x2A] = op_add_r4; op_table[0x2B] = op_add_r5;
    op_table[0x2C] = op_add_r6; op_table[0x2D] = op_add_r7;
    
    op_table[0x2E] = op_sub_r0; op_table[0x2F] = op_sub_r1;
    op_table[0x30] = op_sub_r2; op_table[0x31] = op_sub_r3;
    op_table[0x32] = op_sub_r4; op_table[0x33] = op_sub_r5;
    op_table[0x34] = op_sub_r6; op_table[0x35] = op_sub_r7;
    
    op_table[0x36] = op_and_r0; op_table[0x37] = op_and_r1;
    op_table[0x38] = op_and_r2; op_table[0x39] = op_and_r3;
    op_table[0x3A] = op_and_r4; op_table[0x3B] = op_and_r5;
    op_table[0x3C] = op_and_r6; op_table[0x3D] = op_and_r7;
    
    op_table[0x3E] = op_or_r0; op_table[0x3F] = op_or_r1;
    op_table[0x40] = op_or_r2; op_table[0x41] = op_or_r3;
    op_table[0x42] = op_or_r4; op_table[0x43] = op_or_r5;
    op_table[0x44] = op_or_r6; op_table[0x45] = op_or_r7;
    
    op_table[0x46] = op_xor_r0; op_table[0x47] = op_xor_r1;
    op_table[0x48] = op_xor_r2; op_table[0x49] = op_xor_r3;
    op_table[0x4A] = op_xor_r4; op_table[0x4B] = op_xor_r5;
    op_table[0x4C] = op_xor_r6; op_table[0x4D] = op_xor_r7;
    
    op_table[0x4E] = op_rr_a; op_table[0x4F] = op_rl_a;
    op_table[0x50] = op_rr4_a; op_table[0x51] = op_rl4_a;
    
    op_table[0x52] = op_mov_a_r0; op_table[0x53] = op_mov_a_r1;
    op_table[0x54] = op_mov_a_r2; op_table[0x55] = op_mov_a_r3;
    op_table[0x56] = op_mov_a_r4; op_table[0x57] = op_mov_a_r5;
    op_table[0x58] = op_mov_a_r6; op_table[0x59] = op_mov_a_r7;
    
    op_table[0x60] = op_jmp;
    op_table[0x61] = op_jmp;
    op_table[0x62] = op_jz;
    op_table[0x63] = op_jnz;
    op_table[0x64] = op_call;
    op_table[0x65] = op_ret;
    
    op_table[0x80] = op_mov_a_imm;
    op_table[0x81] = op_add_imm;
    op_table[0x82] = op_and_imm;
    op_table[0x83] = op_or_imm;
    op_table[0x84] = op_xor_imm;
    op_table[0x85] = op_sub_imm;
    
    op_table[0x90] = op_mov_r0_a; op_table[0x91] = op_mov_r1_a;
    op_table[0x92] = op_mov_r2_a; op_table[0x93] = op_mov_r3_a;
    op_table[0x94] = op_mov_r4_a; op_table[0x95] = op_mov_r5_a;
    op_table[0x96] = op_mov_r6_a; op_table[0x97] = op_mov_r7_a;
    
    op_table[0x98] = op_mov_a_r0; op_table[0x99] = op_mov_a_r1;
    op_table[0x9A] = op_mov_a_r2; op_table[0x9B] = op_mov_a_r3;
    op_table[0x9C] = op_mov_a_r4; op_table[0x9D] = op_mov_a_r5;
    op_table[0x9E] = op_mov_a_r6; op_table[0x9F] = op_mov_a_r7;
    
    op_table[0xB0] = op_out_p1; op_table[0xB1] = op_out_p2;
    op_table[0xB2] = op_in_p1; op_table[0xB3] = op_in_p2;
    
    op_table[0xC0] = op_mov_t_a; op_table[0xC1] = op_mov_a_t;
}

void cpuStep(void) {
    uint8_t opcode = memory[cpu.PC++];
    op_table[opcode]();
    cpu.cycle_count++;
}

void resetCPU(void) {
    memset(&cpu, 0, sizeof(CPU_8021));
    cpu.PC = 0x000;
}