#include "cpu.h"
#include "inst.h"
#include "memory.h"

#define FL_ZERO  0b00000001
#define FL_NEG   0b00000010
#define FL_CARRY 0b00000100

void cpuSetFlag(cpu_t *cpu, u8 flag, u8 value) {
    cpu->fl &= ~flag;
    cpu->fl |= (value == 0)? 0 : (0xff&flag);
}

u8 cpuGetFlag(cpu_t *cpu, u8 flag) {
    return cpu->fl & flag;
}

// This is a horrific function that does everything.
// I am almost certainly shooting myself in the foot by writing
// code this way, but oh well.
//
// If you are reading this, I'm sorry.
void cpuExec(cpu_t *cpu) {
    inst_t i = instructionFromByte( memReadByte(cpu->pc) );
    cpu->ir = (i.opcode << 3) + i.rs;
    cpu->pc++;

    if(i.opcode == NOP) return;

    //////////////////////////////////////////
    // BEGIN DATA MANIPULATION INSTRUCTIONS //
    //////////////////////////////////////////
    if(i.opcode == LDI) {
        cpu->regs[i.rs] = memReadByte(cpu->pc);
        cpu->pc++;

        cpuSetFlag(cpu, FL_ZERO, cpu->regs[i.rs] == 0);
        cpuSetFlag(cpu, FL_NEG, cpu->regs[i.rs] & 0b10000000); 
    } else if(i.opcode == LDA) {
        cpu->regs[i.rs] = memReadByte( memReadWord(cpu->pc) );
        cpu->pc += 2;

        cpuSetFlag(cpu, FL_ZERO, cpu->regs[i.rs] == 0);
        cpuSetFlag(cpu, FL_NEG, cpu->regs[i.rs] & 0b10000000); 
    } else if(i.opcode == LDD) {
        cpu->regs[i.rs] = memReadByte( memReadWord( memReadWord(cpu->pc) ) );
        cpu->pc += 2;

        cpuSetFlag(cpu, FL_ZERO, cpu->regs[i.rs] == 0);
        cpuSetFlag(cpu, FL_NEG, cpu->regs[i.rs] & 0b10000000); 
    } else if(i.opcode == STA) {
        memWriteByte( memReadWord(cpu->pc), cpu->regs[i.rs] );
        cpu->pc += 2;
    } else if(i.opcode == STD) {
        memWriteByte( memReadWord(memReadWord(cpu->pc)), cpu->regs[i.rs] );
        cpu->pc += 2;
    } else if(i.opcode == PSH) {
        cpu->sp--;
        memWriteByte( STACK_BASE + cpu->sp, cpu->regs[i.rs] );
    } else if(i.opcode == POP) {
        cpu->regs[i.rs] = memReadByte(STACK_BASE + cpu->sp);
        cpu->sp++;

        cpuSetFlag(cpu, FL_ZERO, cpu->regs[i.rs] == 0);
        cpuSetFlag(cpu, FL_NEG, cpu->regs[i.rs] & 0b10000000); 
    }
    
    ///////////////////////////////////
    // BEGIN ARITHMETIC INSTRUCTIONS //
    ///////////////////////////////////
    else if(i.opcode == ADI) {
        u8 before = cpu->regs[i.rs];
        u8 result = cpu->regs[i.rs] + memReadByte(cpu->pc);
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
        cpuSetFlag(cpu, FL_CARRY, result < before);
    } else if(i.opcode == ADR) {
        u8 before = cpu->regs[i.rs];
        u8 result = cpu->regs[i.rs] + cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
        cpuSetFlag(cpu, FL_CARRY, result < before);
    } else if(i.opcode == SBI) {
        u8 before = cpu->regs[i.rs];
        u8 result = cpu->regs[i.rs] - memReadByte(cpu->pc);
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
        cpuSetFlag(cpu, FL_CARRY, result > before);
    } else if(i.opcode == SBR) {
        u8 before = cpu->regs[i.rs];
        u8 result = cpu->regs[i.rs] - cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
        cpuSetFlag(cpu, FL_CARRY, result > before);
    } else if(i.opcode == MLI) {
        u8 before = cpu->regs[i.rs];
        u8 result = cpu->regs[i.rs] * memReadByte(cpu->pc);
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
        cpuSetFlag(cpu, FL_CARRY, result < before);
    } else if(i.opcode == MLR) {
        u8 before = cpu->regs[i.rs];
        u8 result = cpu->regs[i.rs] * cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
        cpuSetFlag(cpu, FL_CARRY, result < before);
    } else if(i.opcode == DVI) {
        u8 result = cpu->regs[i.rs] / memReadByte(cpu->pc);
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == DVR) {
        u8 result = cpu->regs[i.rs] - cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == MDI) {
        u8 result = cpu->regs[i.rs] % memReadByte(cpu->pc);
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == MDR) {
        u8 result = cpu->regs[i.rs] % cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    }
    
    ////////////////////////////////
    // BEGIN LOGICAL INSTRUCTIONS //
    ////////////////////////////////
    else if(i.opcode == ANI) {
        u8 result = cpu->regs[i.rs] & memReadByte(cpu->pc);
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == ANR) {
        u8 result = cpu->regs[i.rs] & cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == ORI) {
        u8 result = cpu->regs[i.rs] | memReadByte(cpu->pc);
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == ORR) {
        u8 result = cpu->regs[i.rs] | cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == NOT) {
        u8 result = ~cpu->regs[i.rs];
        cpu->regs[i.rs] = result;

        cpuSetFlag(cpu, FL_ZERO, result == 0);
        cpuSetFlag(cpu, FL_NEG, result * 0b10000000);
    } else if(i.opcode == CPI) {
        u8 a = cpu->regs[i.rs];
        u8 b = memReadByte(cpu->pc);
        cpu->pc++;

        cpuSetFlag(cpu, FL_ZERO, a == b);
        cpuSetFlag(cpu, FL_NEG, a < b);
        cpuSetFlag(cpu, FL_CARRY, a > b);
    } else if(i.opcode == CPR) {
        u8 a = cpu->regs[i.rs];
        u8 b = cpu->regs[ memReadByte(cpu->pc) ];
        cpu->pc++;

        cpuSetFlag(cpu, FL_ZERO, a == b);
        cpuSetFlag(cpu, FL_NEG, a < b);
        cpuSetFlag(cpu, FL_CARRY, a > b);
    }
    
    /////////////////////////////////////
    // BEGIN CONTROL FLOW INSTRUCTIONS //
    /////////////////////////////////////
    else if(i.opcode == JMP) {
        cpu->pc = memReadWord(cpu->pc);
    } else if(i.opcode == JIZ) {
        u16 addr = memReadWord(cpu->pc);
        cpu->pc += 2;

        if(cpuGetFlag(cpu, FL_ZERO))
            cpu->pc = addr;
    } else if(i.opcode == JNZ) {
        u16 addr = memReadWord(cpu->pc);
        cpu->pc += 2;

        if(!cpuGetFlag(cpu, FL_ZERO))
            cpu->pc = addr;
    } else if(i.opcode == JIN) {
        u16 addr = memReadWord(cpu->pc);
        cpu->pc += 2;

        if(cpuGetFlag(cpu, FL_NEG))
            cpu->pc = addr;
    } else if(i.opcode == JIC) {
        u16 addr = memReadWord(cpu->pc);
        cpu->pc += 2;

        if(cpuGetFlag(cpu, FL_ZERO))
            cpu->pc = addr;
    } else if(i.opcode == CLL) {
        u16 addr = memReadWord(cpu->pc);
        cpu->pc += 2;

        cpu->sp -= 2;
        memWriteWord(STACK_BASE + cpu->sp, cpu->pc);
        cpu->pc = addr;
    } else if(i.opcode == RET) {
        cpu->pc = memReadWord(STACK_BASE + cpu->sp);
        cpu->sp += 2;
    } 
}
