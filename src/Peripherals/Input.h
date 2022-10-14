//
// Created by jonathanpc on 24/07/22.
//

#ifndef CHIP8EMULATOR_INPUT_H
#define CHIP8EMULATOR_INPUT_H

#include "../Chip8/Chip8Vm.h"
#include <SDL.h>

class Input {
public:
    bool quit {false};
    bool reset {false};
    int speed {1200};

    int init();
    void setKeys(array<bool, NUM_KEYS>& keys);
};
#endif //CHIP8EMULATOR_INPUT_H
