



# ACE-8 Controller Specification

## Introduction

The ACE-8 has a singular control port (with plans to extend this to two).
In the emulator, the arrow keys can be used to press the L, R, U and D
buttons, and the Z and X keys represent the A and B buttons respectively.


## Controller Status Register

The controller status register is located at address 0x2105 in memory,
and is updated 30 times a second. The top 2 bits in the register are
unused. The register is as follows:

- Bit 0A Button
- Bit 1B Button
- Bit 2L Button
- Bit 3R Button
- Bit 4U Button
- Bit 5D Button
