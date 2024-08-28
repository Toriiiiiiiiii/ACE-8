



# ACE-8 Sound Specification

## Introduction

The ACE-8 has a very basic sound system. There are only 4 channels,
each limited to 256 different frequencies. Their volume is also static,
meaning the sound is very limited. However, this means that only a few
bytes of memory have to be used, and means sound does not need to be driven
entirely by the CPU.


## Sound Memory

Sound memory is layed out as follows:
- 0x2106 .. 0x2107 -> Square Wave Registers
- 0x2108 .. 0x2109 -> Sawtooth Wave Registers
- 0x210A .. 0x210B -> Triangle Wave Registers
- 0x210C .. 0x210D -> Noise Registers


## Sound Generation

Each sound channel has 2 registers in memory - a length register and
frequency register. When both are non-zero, the sound synthesizer will play
the respective waveform at FREQ Hertz for LENGTH seconds. The length
register is an 8-bit unsigned fixed-point number, with the top 4 bits 
representing the integer component, and the bottom 4 bits representing the
fractional component.
