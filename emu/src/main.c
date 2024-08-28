#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <raylib.h>

#include "cpu.h"
#include "font.h"

#define VIDMEM_SIZE 1024
#define VIDMEM_BASE 0x1001
#define CTABLE_BASE VIDMEM_BASE + VIDMEM_SIZE + 1
#define CTABLE_SIZE 1024
#define TILE_BASE   CTABLE_BASE + CTABLE_SIZE + 1
#define TILE_SIZE   2048
#define SPRITE_BASE TILE_BASE + TILE_SIZE + 1
#define SPRITE_SIZE 256 

#define CONT_A 0b00000001
#define CONT_B 0b00000010
#define CONT_L 0b00000100
#define CONT_R 0b00001000
#define CONT_U 0b00010000
#define CONT_D 0b00100000
#define CONT_ADDR 0x2105

cpu_t cpu = {0};

static int isRunning = 1;
static Color cols[16] = {
    {   0,   0,   0, 255},
    {   0,   0, 127, 255},
    {   0, 127,   0, 255},
    {   0, 127, 127, 255},
    { 127,   0,   0, 255},
    { 127,   0, 127, 255},
    { 127, 127,   0, 255},
    { 127, 127, 127, 255},
    {   0,   0,   0, 255},
    {   0,   0, 255, 255},
    {   0, 255,   0, 255},
    {   0, 255, 255, 255},
    { 255,   0,   0, 255},
    { 255,   0, 255, 255},
    { 255, 255,   0, 255},
    { 255, 255, 255, 255},
};

void signalHandler(int sig) {
    printf("Exception Occured!\n"); 
    printf("CPU STATUS\n");
    printf("----------\n");

    printf("  PC: %04x\n", cpu.pc);
    printf("  SP: %04x\n", cpu.sp);
    printf("  FL: %02x\n", cpu.fl);
    printf("  IR: %02x (%02x, %x)\n", cpu.ir, (cpu.ir >> 3)&0b11111, (cpu.ir&0b11));
    printf("  REGS:\n");

    for(int i = 0; i < 8; ++i) {
        printf("    %02x\n", cpu.regs[i]);
    }

    exit(-1);
}

void drawCharacter(u8 t, unsigned int x, unsigned int y, u8 c) {
    if(t == 0) return;
    for(int i = 0; i < 8; ++i) {
        for(int n = 0; n < 8; ++n) {
            u8 row = memReadByte(TILE_BASE + (t * 8) + i);
            u8 pix = (row >> n) & 1;
            if(pix && (c >> 4) & 0xf) DrawRectangle((x+n)*2, (y+i)*2, 2, 2, cols[(c >> 4) & 0xf]);
            else if(c & 0xf) DrawRectangle((x+n)*2, (y+i)*2, 2, 2, cols[c & 0xf]);
        }
    }
}

void loadFont() {
    u16 memoryAddress = TILE_BASE;
    for(int arrIndex = 0; arrIndex < 0x80; ++arrIndex) {
        for(int byteIndex = 0; byteIndex < 8; ++byteIndex) {
            memWriteByte(memoryAddress, font8x8_basic[arrIndex][byteIndex]);
            memoryAddress++;
        }
    }
}

void *displayThread(void *vargp) {
    InitWindow(512, 512, "Video Output");
    SetTargetFPS(30);

    while(isRunning && !WindowShouldClose()) {
        BeginDrawing();
        memWriteByte(0x1000, 0);
        ClearBackground(BLACK);

        unsigned int width = 32;
        unsigned int height = 32;

        // Draw tiles
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                unsigned int i = y * width + x;
                u8 c = memReadByte(VIDMEM_BASE + i);

                unsigned int px = x * 8;
                unsigned int py = y * 8;
                drawCharacter(c, px, py, memReadByte(CTABLE_BASE + i));
            }
        }

        // Draw Sprites
        for(int sprIndex = 0; sprIndex < 64; ++sprIndex) {
            u16 baseAddress = SPRITE_BASE + (4 * sprIndex);

            u8 tileIndex = memReadByte(baseAddress);
            if(!tileIndex) continue; // Dont bother rendering if no tile is set.

            u8 x = memReadByte(baseAddress + 1);
            u8 y = memReadByte(baseAddress + 2);
            u8 c = memReadByte(baseAddress + 3);
            drawCharacter(tileIndex, x, y, c);
        }

        u8 status = 0;

        status |= (IsKeyDown(KEY_Z))? CONT_A : 0;
        status |= (IsKeyDown(KEY_X))? CONT_B : 0;
        status |= (IsKeyDown(KEY_LEFT))? CONT_L : 0;
        status |= (IsKeyDown(KEY_RIGHT))? CONT_R : 0;
        status |= (IsKeyDown(KEY_UP))? CONT_U : 0;
        status |= (IsKeyDown(KEY_DOWN))? CONT_D : 0;

        memWriteByte(CONT_ADDR, status);

        EndDrawing();
        memWriteByte(0x1000, 1);
    }

    if(isRunning) isRunning = 0;

    CloseWindow();
    return NULL;
}

long currentUS() {
    struct timeval tp;
    gettimeofday(&tp, NULL);

    return tp.tv_sec * 1000000 + tp.tv_usec;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if(!f) {
        printf("Couldnt read file!\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    loadFont();
    u8 mBuffer[65536] = {0};
    fread(mBuffer, 1, size, f);
    for(int i = 0; i < size; ++i) {
        memWriteByte(i, mBuffer[i]);
    }

    cpu.sp = 0xff;

    struct sigaction sigact;
    sigact.sa_handler = signalHandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGFPE, &sigact, (struct sigaction *)NULL);

    pthread_t renderthread_id;
    pthread_create(&renderthread_id, NULL, displayThread, NULL);

    while(isRunning) {
        cpuExec(&cpu);
    }

    pthread_join(renderthread_id, NULL);
    fclose(f);
    return 0;
}
