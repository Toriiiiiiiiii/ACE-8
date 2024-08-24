#include "memory.h"
static u8 mem[65536] = {0};

u8 memReadByte(u16 addr) {
    return mem[addr];
}

u16 memReadWord(u16 addr) {
    return (u16)(mem[addr]) | mem[addr + 1] << 8;
}

void memWriteByte(u16 addr, u8 data) {
    mem[addr] = data;
}

void memWriteWord(u16 addr, u16 data) {
    mem[addr] = data & 0xff;
    mem[addr + 1] = (data >> 8) & 0xff;
}
