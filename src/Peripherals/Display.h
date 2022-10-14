//
// Created by jonathanpc on 24/07/22.
//

#ifndef CHIP8EMULATOR_DISPLAY_H
#define CHIP8EMULATOR_DISPLAY_H

#include <SDL.h>
#include <array>
#include "../Chip8/Chip8Vm.h"

#define SCALING_FACTOR 10

using namespace std;
class Display {
public:
    Display();
    ~Display();

    SDL_Window*     window = nullptr;
    SDL_Texture*    texture = nullptr;
    SDL_Renderer*   renderer = nullptr;

    void drawDisplay(array<array<bool, SCREEN_WIDTH>, SCREEN_HEIGHT>& display);
    int init();
    void close();
};
#endif //CHIP8EMULATOR_DISPLAY_H
