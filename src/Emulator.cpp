//
// Created by jonathanpc on 24/07/22.
//

#include <fstream>
#include <iostream>
#include "Emulator.h"

Emulator::Emulator() {
    chip8 = new Chip8Vm();
    display = new Display();
    input = new Input();
}

Emulator::~Emulator() {
    delete input;
    input = nullptr;
    delete display;
    display = nullptr;
    delete chip8;
    chip8 = nullptr;
}

void Emulator::init() {
    chip8->init();
    display->init();
    input->init();
    isRunning = true;
}

int Emulator::loadRom(string file) {
    ifstream romFile {file, ios::in | ios::binary | ios::ate};
    if (!romFile.good()) {
        cout << "Couldn't read '" << file << "'!\n";
        return 1;
    }
    vector<char> buffer;
    int fileSize = romFile.tellg();
    romFile.seekg(ios::beg);

    buffer.resize(fileSize);
    if (fileSize > MAX_ROM_SIZE) {
        cout << "File is too big!\n";
        return 1;
    }
    romFile.read(buffer.data(), fileSize);
    romFile.close();

    chip8->load(buffer);
    return 0;
}

int Emulator::run(string file) {
    init();

    cout << "Starting emulator...\n";
    if (file.empty()) {
        cout << "No file specified!\n";
        return 1;
    }
    if (loadRom(file)) {
        cout << "No ROM loaded!\n";
        return 1;
    }

    while (isRunning) {
        if (chip8->cycle()) {
            isRunning = false;
        }

        if (chip8->drawFlag) {
            display->drawDisplay(chip8->display);
            chip8->drawFlag = false;
        }

        if (chip8->soundFlag) {
            cout << '\a' << flush;
            chip8->soundFlag = false;
        }
        input->setKeys(chip8->keys);
        isRunning = !input->quit;
        if (input->reset) {
            input->reset = false;
        }
        // Delay
        // this_thread::sleep_for(std::chrono::microseconds(input->speed));
        SDL_Delay(input->speed / 1000);
    }

    return 1;
}