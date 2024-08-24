#pragma once
#include "types.h"

u8  memReadByte(u16 addr);
u16 memReadWord(u16 addr);

void memWriteByte(u16 addr, u8  data);
void memWriteWord(u16 addr, u16 data);
