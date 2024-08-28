#include <cstdio>
#include <string>
#include <raylib.h>

#include "font.h"

typedef unsigned char u8;

unsigned char data[8 * 256] = {0};
unsigned char selectedGraphic = 0;

void drawCharacter(u8 t, unsigned int x, unsigned int y, Color c) {
    if(t == 0) return;
    for(int i = 0; i < 8; ++i) {
        for(int n = 0; n < 8; ++n) {
            u8 row = data[(t * 8) + i];
            u8 pix = (row >> n) & 1;
            if(pix) DrawRectangle((x+n)*2, (y+i)*2, 2, 2, c);
        }
    }
}

void drawEditPane() {
    DrawRectangleLines(272, 8, 176, 176, WHITE);
    for(int i = 0; i < 8; ++i) {
        for(int n = 0; n < 8; ++n) {
            u8 row = data[(selectedGraphic * 8) + i];
            u8 pix = (row >> n) & 1;
            if(pix) DrawRectangle(280 + (n)*20, 16 + (i)*20, 20, 20, WHITE);
        }
    }

    Vector2 pos = GetMousePosition();
    if((pos.x > 279 && pos.x < 452) && (pos.y >= 16 && pos.y <= (16+172))) {
        u8 xindex = (int)(pos.x - 280) / 20;
        u8 yindex = (int)(pos.y - 16) / 20;

        u8 row = data[(selectedGraphic * 8) + yindex];
        u8 pix = (row >> xindex) & 1;
        if(!pix) DrawRectangle(280 + xindex*20, 16 + yindex*20, 20, 20, GRAY);
    }
}

void loadFont() {
    for(int arrIndex = 0; arrIndex < 0x80; ++arrIndex) {
        for(int byteIndex = 0; byteIndex < 8; ++byteIndex) {
            data[(arrIndex*8)+byteIndex] = font8x8_basic[arrIndex][byteIndex];
        }
    }
}
void handleInput() {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 pos = GetMousePosition();
        if(pos.x < 256 && pos.y < 256) {
            u8 xindex = (int)pos.x / 16;
            u8 yindex = (int)pos.y / 16;

            selectedGraphic = yindex * 16 + xindex;
        }
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 pos = GetMousePosition();
        if((pos.x > 279 && pos.x < 452) && (pos.y >= 16 && pos.y <= (16+172))) {
            u8 xindex = (int)(pos.x - 280) / 20;
            u8 yindex = (int)(pos.y - 16) / 20;

            data[(selectedGraphic * 8) + yindex] |= 1 << xindex;
        }
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 pos = GetMousePosition();
        if((pos.x > 279 && pos.x < 452) && (pos.y >= 16 && pos.y <= (16+172))) {
            u8 xindex = (int)(pos.x - 280) / 20;
            u8 yindex = (int)(pos.y - 16) / 20;

            data[(selectedGraphic * 8) + yindex] &= ~(1 << xindex);
        }
    }
}

void executeCommand(std::string s) {
    int first_space = s.find(' ');
    std::string cmd = s.substr(0, first_space);
    std::string arg = s.substr(first_space+1);

    if(cmd == "save") {
        FILE *f = fopen(arg.c_str(), "wb");
        if(!f) {
            printf("[ERR] Could not open file %s.\n", arg.c_str());
            return;
        }
        fwrite(data, 1, 256*8, f);
        fclose(f);
    }

    if(cmd == "load") {
        FILE *f = fopen(arg.c_str(), "rb"); 
        if(!f) {
            printf("[ERR] Could not open file %s.\n", arg.c_str());
            return;
        }

        fread(data, 1, 256*8, f);
        fclose(f);
    }

    if(cmd == "new") {
        for(int i = 0; i < 8*256; ++i) {
            data[i] = 0;
        }
    }
}

void drawTxt(std::string string, int x, int y, Color c) {
    for(int i = 0; i < string.length(); ++i) {
        u8 *bitmap = font8x8_basic[string[i]];

        for(int row = 0; row < 8; ++row) {
            for(int col = 0; col < 8; ++col) {
                if((bitmap[row] >> col) & 1) DrawRectangle(x+(8*i)+(col), y+(row*2), 1, 2, c);
            }
        }
    } 
}

int main() {
    InitWindow(456, 272, "ACE-8 GraphEd v1.0");
    loadFont();

    std::string command = "";

    while(!WindowShouldClose()) {
        handleInput();

        int key = GetCharPressed();
        while(key > 0) {
            if((key >= 32) && (key <= 125)) {
                command += (char)key;
            }

            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE) && !command.empty()) {
            command.pop_back();
        }

        if(IsKeyPressed(KEY_ENTER)) {
            executeCommand(command);
            command = "";
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for(int i = 0; i < 256; ++i) {
            if(i == selectedGraphic) {
                drawCharacter(i, (i%16)*8, (i/16)*8, WHITE);
                DrawRectangleLines((i%16)*16, (i/16)*16, 16, 16, WHITE);
            }
            else drawCharacter(i, (i%16)*8, (i/16)*8, GRAY);
        }

        DrawLine(264, 0, 264, 255, WHITE);
        DrawRectangle(0, 256, 456, 16, DARKGRAY);

        drawTxt("GraphEd v1.0", 272, 192, WHITE);
        drawTxt("Created by Tori Hall", 272, 208, WHITE);

        if(!command.empty()) {
            drawTxt(command.c_str(), 0, 256, WHITE);
        } else {
            drawTxt("Type 'save <path>' to save, or 'load <path>' to load.", 0, 256, GRAY);
        }
        drawEditPane();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
