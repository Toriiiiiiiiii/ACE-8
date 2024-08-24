#include "inst.h"

inst_t instructionFromByte(u8 byte) {
    inst_t res = {0};

    res.opcode = (byte >> 3) & 0b11111; 
    res.rs = byte & 0b111;

    return res;
}
