//
// Created by jonathanpc on 24/07/22.
//

#ifndef CHIP8EMULATOR_EMULATOR_H
#define CHIP8EMULATOR_EMULATOR_H

#include <string>
#include "Chip8/Chip8Vm.h"
#include "Peripherals/Input.h"
#include "Peripherals/Display.h"

using std::string;

class Emulator {
private:
    Chip8Vm*    chip8;
    Input*      input;
    Display*    display;

    bool isRunning {false};
    bool isLoaded;

    void init();

public:
    Emulator();
    ~Emulator();

    int loadRom(string file);
    int run(string file);
};
#endif //CHIP8EMULATOR_EMULATOR_H
