



# ACE-8 Video Specification

## Introduction

The ACE-8 has a 256x156 pixel display. This display is divided into 8x8
pixel tiles. Each tile is represented by a single byte in Tilemap memory,
which references an 8-byte monochrome bitmap in Tile Bitmap memory.


## Video Memory

The ACE-8's video memory spans the addresses 0x1000 to 0x2104 inclusive.
Because of this, programs loaded into the low end of memory will need to
be under 4KiB to avoid overwriting video memory.


### Video Status - 0x1000

The Video Status Register uses 1 byte of memory for potential expansion,
although it currently only uses the bottom bit, which is used to keep 
track of frame drawing. If the value is 0, then a frame is being drawn.
If it is 1, then the frame has finished being drawn and the program can
safely modify video memory.


### Tilemap Memory - 0x1001 to 0x1401

Each byte in tilemap memory represents a single 8x8 tile on the screen.
The tilemap is 32 tiles wide and 32 tiles high, meaning there are a total
of 1024 tiles. Each tile uses a graphic from Tile bitmap memory, which is
indexed by the corresponding byte in tilemap memory.


### Tilemap Colour Memory - 0x1402 to 0x1802

Tilemap colour memory complements Tilemap memory, such that each byte
in colour memory corresponds to the same tile as in tilemap memory.
Each byte in colour memory corresponds to the [colour](#colour) of the
tile when it is drawn on the screen.


### Tile Bitmap Memory - 0x1803 to 0x2003

Each tile in tilemap memory, and each sprite references an 8-byte string 
in this block of video memory, which contains an 8x8 monochrome bitmap 
image that will be drawn to the screen. Each bit represents a pixel in the
bitmap, with the lowest bit of each byte representing the leftmost pixel
in a row. Because of this, the bitmap will appear mirrored from the data
in the assembler. For example:


#### Assembler Data:

0b00000000
0b01100000
0b00110000
0b00011000
0b00011000
0b00110000
0b01100000
0b00000000


#### Rendered Sprite:

........
.....##.
....##..
...##...
...##...
....##..
.....##.
........


### Sprite Memory - 0x2004 to 0x2104

Sprites are 8x8 bitmap images that are not limited to the tilemap grid.
The ACE-8 supports up to 64 hardware sprites, each using 4 bytes of memory
in this block. A sprite consists of the following:
- Bitmap Index
- X Position
- Y Position
- Colour

The Bitmap Index byte corresponds to a bitmap in Tile Bitmap Memory, which
will be used as the graphic for the sprite. If this is set to 0, the sprite
will not be rendered.


## Colour

The ACE-8 has a 16-colour palette. A colour byte actually consists of two
colours: A foreground and background colour. This allows a creative
programmer to overcome the monochrome limitation of graphics on the ACE-8
with the use of multiple sprites layered over each other.

The most significant four bits of a colour byte represents the foreground
colour, and the least significant four bits represent the background.


### Colour Pallete

- 0x0: Transparent
- 0x1: Dark Blue
- 0x2: Dark Green
- 0x3: Dark Cyan
- 0x4: Dark Red
- 0x5: Dark Magenta
- 0x6: Dark Yellow
- 0x7: Dark Grey
- 0x8: Black
- 0x9: Blue
- 0xA: Green
- 0xB: Cyan
- 0xC: Red
- 0xD: Magenta
- 0xE: Yellow
- 0xF: White
