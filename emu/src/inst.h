#pragma once
#include "types.h"

enum _opcode_t {
    NOP = 0,
    
    LDI, LDA, LDD, STA,
    STD, PSH, POP,

    ADI, ADR, SBI, SBR, MLI,
    MLR, DVI, DVR, MDI, MDR,

    ANI, ANR, ORI, ORR,
    NOT, CPI, CPR,

    JMP, JIZ, JNZ, JIN,
    JIC, CLL, RET, 
};

typedef enum _opcode_t opcode_t;

struct _inst_t {
    u8 opcode;
    u8 rs;
};

typedef struct _inst_t inst_t;

inst_t instructionFromByte(u8 byte);
