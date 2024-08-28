



# ACE-8 Processor Specification

## Introduction

The ACE-8 uses a custom processor. The design of the processor was inspired 
by other 8-bit microprocessors such as the 6502, however the final 8
design shares little similarities.


## Instructions

Instructions are one byte in size, with the potential for up to 2 bytes for
the operand. The instruction byte is layed out as follows:

- Bits 7-3: Opcode
- Bits 2-0: [Register Select](#accessible-registers)


### Opcodes:

Opcodes are grouped into 5 categories:
- [Uncategorised](#uncategorised-instructions) (1)
- [Data Manipulation](#data-manipulation-instructions) (7)
- [Arithmetic Operations](#arithmetic-operation-instructions) (10)
- [Logical Operations](#logical-operation-instructions) (7)
- [Control Flow Operations](#control-flow-operation-instructions) (7)


#### Uncategorised Instructions

```
- 0b00000: NOP (No Operand)      -> No Operation
```


#### Data Manipulation Instructions
```
- 0b00001: LDI (imm8)            -> Loads an immediate value into a register.
- 0b00010: LDA (abs16)           -> Directly loads a value from memory.
- 0b00011: LDD (ind16)           -> Indirectly loads a value from memory.
- 0b00100: STA (abs16)           -> Directly stores a value to memory.
- 0b00101: STD (ind16)           -> Indirectly stores a value to memory.
- 0b00110: PSH (impl)            -> Pushes the value of a register to the stack.
- 0b00111: POP (impl)            -> Pops a register value from the stack.
```

#### Arithmetic Operation Instructions

```
- 0b01000: ADI (imm8)            -> Adds an immediate value to a register.
- 0b01001: ADR (reg8)            -> Adds a register's value to another register.
- 0b01010: SBI (imm8)            -> Subtracts an immediate value from a register.
- 0b01011: SBR (reg8)            -> Subtracts a register's value from another register.
- 0b01100: MLI (imm8)            -> Multiplies a register by an immediate value.
- 0b01101: MLR (reg8)            -> Multiplies a register by another register.
- 0b01110: DVI (imm8)            -> Divides a register by an immediate value.
- 0b01111: DVR (reg8)            -> Divides a register by another register.
- 0b10000: MDI (imm8)            -> Performs DVI and stores the remainder.
- 0b10001: MDR (reg8)            -> Performs DVR and stores the remainder.
```


#### Logical Operation Instructions

```
- 0b10010: ANI (imm8)            -> ANDs a register with an immediate value.
- 0b10011: ANR (reg8)            -> ANDs a register with another register.
- 0b10100: ORI (imm8)            -> ORs a register with an immediate value.
- 0b10101: ORR (reg8)            -> ORs a register with another register.
- 0b10110: NOT (impl)            -> NOTs the value of a register.
- 0b10111: CPI (imm8)            -> Compares a register with an immediate value.
- 0b11000: CPR (reg8)            -> Compares the value of two registers.
```


#### Control Flow Operation Instructions

```
- 0b11001: JMP (abs16)           -> Jumps to an address.
- 0b11010: JIZ (abs16)           -> Jumps to an address if the zero flag is set.
- 0b11011: JNZ (abs16)           -> Jumps to an address if the zero flag is not set.
- 0b11100: JIN (abs16)           -> Jumps to an address if the negative flag is set.
- 0b11101: JIC (abs16)           -> Jumps to an address if the carry flag is set.
- 0b11110: CLL (abs16)           -> Puts the PC on the stack and jumps to an address.
- 0b11111: RET (impl)            -> Restores the PC from the stack.
```


## Registers

The processor has 8 registers directly accessible to the programmer. All of
these accessible registers have a data width of 8 bits. Additionally, the
ACE-8 has 3 registers that are not directly accessible, and can only be 
modified through instructions such as JMP or CPI.


### Accessible Registers

The accessible registers are indexed in instructions with a 3-bit number.
The registers are labelled as A-E respectively. These registers can be 
directly modified through instructions such as LDI and DVI.


### Inaccessible Registers

Inaccessible registers are used internally by the processor for control
flow and logical instructions. Two of these registers are 16-bits in size,
the other being a single byte. The registers are as follows:

```
- Program Counter (PC) -> 16-bit
- Stack Pointer (SP)   -> 16-bit
- Flag Register (FL)   -> 8-bit
```


#### Flag Register

The flag register is 8-bits in size. However, in the current iteration
of the processor, only the lowest 3 bits of the register are used. The
register is layed out as follows:
```

- Bit 0: Zero Flag
- Bit 1: Negative Flag
- Bit 2: Carry Flag
```
