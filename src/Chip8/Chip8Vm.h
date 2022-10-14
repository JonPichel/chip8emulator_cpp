//
// Created by jonathanpc on 23/07/22.
//

#ifndef CHIP8EMULATOR_CHIP8VM_H
#define CHIP8EMULATOR_CHIP8VM_H

#include <cstdint>
#include <array>
#include <vector>
#include <random>

#define SCREEN_WIDTH    64
#define SCREEN_HEIGHT   32
#define NUM_KEYS        16
#define NUM_REGISTERS   16

#define MEMORY_SIZE         0x1000
#define STACK_SIZE          16

#define START_ROM_ADDRESS   0x200
#define MAX_ROM_SIZE        (MEMORY_SIZE - START_ROM_ADDRESS)

using namespace std;

class Chip8Vm {
private:
    /* CPU State */
    array<uint8_t, MEMORY_SIZE>     memory;     // Memory Space
    array<uint8_t, NUM_REGISTERS>   V;          // Data Registers
    array<uint16_t, STACK_SIZE>      stack;      // Stack of Return Addresses
    uint16_t                        sp;         // Stack Pointer
    uint16_t                        I;          // Address Register
    uint16_t                        pc;         // Program Counter

    /* Timers */
    uint8_t                         delayTimer; // Delay Timer
    uint8_t                         soundTimer; // Sound Timer

    /* Auxiliary members */
    uint16_t                        opcode;     // Current OPCODE
    array<uint8_t, 80>              fontset;
    array<uint8_t, MEMORY_SIZE>     rom;

    /* Random number generation */
    std::mt19937                            generator;
    std::uniform_int_distribution<uint8_t>  distributor {0, 255};


public:
    Chip8Vm();
    ~Chip8Vm();

    /* Peripherals */
    array<bool, NUM_KEYS>                           keys;       // Keypad State
    array<array<bool, SCREEN_WIDTH>, SCREEN_HEIGHT> display;    // Screen State
    bool                                            drawFlag;   // If SET -> Update Screen
    bool                                            soundFlag;  // If SET -> Play Sound

    void init();
    int load(const vector<char>& buffer);
    void clearDisplay();
    void drawSprite(uint8_t baseX, uint8_t baseY, uint8_t height);
    int cycle();
};

#endif //CHIP8EMULATOR_CHIP8VM_H
