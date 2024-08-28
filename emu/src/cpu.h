#pragma once
#include "types.h"
#include "inst.h"
#include "memory.h"

#define REG_A 0
#define REG_B 1
#define REG_C 2
#define REG_D 3
#define REG_E 4
#define REG_F 5
#define REG_G 6
#define REG_H 7

#define STACK_BASE 0xff00

struct _cpu_t {
    u8 regs[8];
    u8 fl, ir, sp;
    u16 pc;
};

typedef struct _cpu_t cpu_t;

void cpuExec(cpu_t *cpu);
