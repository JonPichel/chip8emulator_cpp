//
// Created by jonathanpc on 24/07/22.
//

#include <cstdlib>
#include <iostream>
#include "Chip8Vm.h"

Chip8Vm::Chip8Vm() {
    this->fontset = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    this->rom.fill(0);
}

void unknownOpcode(uint16_t opcode) {
    cout << "CHIP-8: Unknown instruction: " << std::hex << opcode << '\n';
}

Chip8Vm::~Chip8Vm() {}

void Chip8Vm::init() {
    this->opcode        = 0;
    this->I             = 0;
    this->delayTimer    = 0;
    this->soundTimer    = 0;

    this->memory.fill(0);
    this->V.fill(0);
    this->stack.fill(0);
    this->clearDisplay();
    this->keys.fill(false);
    for (int i = 0; i < 80; i++) {
        this->memory[i] = fontset[i];
    }
    this->pc = 0x200;

    // Random number seeding
    std::random_device os_seed;
    const uint32_t seed = os_seed();
    this->generator.seed(seed);

}

int Chip8Vm::load(const vector<char> &buffer) {
    if (buffer.size() > MAX_ROM_SIZE) {
        std::cout << "CHIP-8: Buffer too big\n";
        return 1;
    }

    this->init();

    for (int i = 0; i < buffer.size(); i++) {
        this->rom[i] = buffer[i];
        this->memory[i + 0x200] = rom[i];
    }
    return 0;
}

void Chip8Vm::clearDisplay() {
    for (auto& item : display) {
        item.fill(false);
    }
}

int Chip8Vm::cycle() {
    if (pc >= MEMORY_SIZE - 1) {
        cout << "CHIP-8: Exceeded maximum memory\n";
        return 1;
    }

    opcode = memory[pc] << 8 | memory[pc + 1];
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x0FFF) {
                case 0x00E0:    // 0x00E0: Clear the screen
                    clearDisplay();
                    drawFlag = true;
                    pc += 2;
                    break;
                case 0x00EE:    // 0x00EE: Return from a subroutine
                    if (sp > 0) {
                        pc = stack[--sp];
                    } else {
                        cout << "CHIP-8: Not enough stack levels\n";
                        return 1;
                    }
                    break;
                default:        // 0x0NNN: Call machine code routine at address NNN
                    cout << "CHIP-8: Unimplemented instruction: "
                        << std::hex << opcode << '\n';
                    return 1;   // Modern interpreters ignore this instruction
            }
            break;
        case 0x1000:            // 0x1NNN: Jump to address NNN
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:            // 0x2NNN: Call subroutine at address NNN
            if (sp < STACK_SIZE) {
                stack[sp++] = pc + 2;
            } else {
                cout << "CHIP-8: Too many stack levels\n";
                return 1;
            }
            pc = opcode & 0x0FFF;
            break;
        case 0x3000:            // 0x3XNN: Skip the next instruction if VX equals NN
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x4000:            // 0x4XNN: Skip the next instruction if VX does not equal NN
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x5000:
            switch (opcode & 0x000F) {
                case 0:         // 0x5XY0: Skip the next instruction if VX equals VY
                    if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                default:
                    unknownOpcode(opcode);
                    return 1;
            }
            break;
        case 0x6000:            // 0x6XNN: Set VX to NN
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x7000:            // 0x7XNN: Add NN to VX, and store the result in VX
            // Carry flag is not changed
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0:       // 0x8XY0: Set VX to the value of VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x1:       // 0x8XY1: Set VX to VX OR VY
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x2:       // 0x8XY2: Set VX to VX AND VY
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x3:       // 0x8XY3: Set VX to VX XOR VY
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x4:       // 0x8XY4: Add VY to VY, and store the result in VX
                    // On carry -> VF = 1; else -> VF = 0
                    V[0xF] = ((int) V[(opcode & 0x0F00) >> 8]
                            + (int) V[(opcode & 0x00F0) >> 4]) > 0xFF ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x5:       // 0x8XY5: Subtract VY from VX, and store the result in VX
                    // On borrow -> VF = 0; else -> VF = 1
                    V[0xF] = (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                            ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x6:       // 0x8XY6: Set VF to the LSB of VY and store VY shr 1 bit in VX
                    V[0xF] = V[(opcode & 0x00F0) >> 4] & 0x0001;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] >> 1;
                    pc += 2;
                    break;
                case 0x7:       // 0x8XY7: Subtract VX from VY, and store the result in VX
                    // On borrow -> VF = 0; els -> VF = 1
                    V[0xF] = (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
                            ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4]
                            - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0xE:       // 0x8XYE: Set VF to the MSB of VY and store VY shl 1 bit in VX
                    V[0xF] = V[(opcode & 0x00F0) >> 4] >> 7;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] << 1;
                    pc += 2;
                    break;
                default:
                    unknownOpcode(opcode);
                    return 1;
            }
            break;
        case 0x9000:
            switch (opcode & 0x000F) {
                case 0x0:       // 0x9XY0: Skip the next instruction if VX does not equal VY
                    if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                default:
                    unknownOpcode(opcode);
                    return 1;
            }
            break;
        case 0xA000:            // 0xANNN: Set I register to the address NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000:            // 0xBNNN: Jump to the address NNN plus V0
            pc = (opcode & 0x0FFF) + V[0x0];
            break;
        case 0xC000:            // 0xCXNN: Set VX to the result of a bitwise AND
            // of NN and a random number (0 to 255)
            V[(opcode & 0x0F00) >> 8] = (opcode & 0x0FF)
                    & this->distributor(generator);
            pc += 2;
            break;
        case 0xD000:            // 0xDXYN: Draw a sprite at (VX, VY)
            // The sprite is 8 pixels wide and N pixels high
            // Each row of 8 pixels is read as bit-coded starting from address I
            // VF -> 1: if any of the pixels are flipped from set to unset
            drawSprite(V[(opcode & 0x0F00) >> 8] % SCREEN_WIDTH,
                       V[(opcode & 0x00F0) >> 4] % SCREEN_HEIGHT,
                       opcode & 0x000F);
            pc += 2;
            break;
        case 0xE000: {          // 0xE000: Skip the next instruction...
            uint8_t keyIndex = V[(opcode & 0x0F00) >> 8];

            if (keyIndex > NUM_KEYS) {
                cout << "CHIP-8: Key too high\n";
                return 1;
            }

            switch (opcode & 0x00FF) {
                case 0x9E:      // 0xEX9E: If the key stored in VX is pressed
                    if (keys[keyIndex]) {
                        pc += 4;
                    } else {
                        pc += 2;
                    }
                    break;
                case 0xA1:      // 0xEXA1: If the key stored in VX is not pressed
                    if (keys[keyIndex]) {
                        pc += 2;
                    } else {
                        pc += 4;
                    }
                    break;
                default:
                    unknownOpcode(opcode);
                    return 1;
            }
            break;
        }
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x07:      // 0xFX07: Set VX to the value of the delay timer
                    V[(opcode & 0x0F00) >> 8] = delayTimer;
                    pc += 2;
                    break;
                case 0x0A: {    // 0xFX0A: Await a keypress, and store it on VX
                    // Blocking operation: instruction halted until next key event
                    bool keyPressed = false;
                    for (int i = 0; i < NUM_KEYS; i++) {
                        if (keys[i]) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            keyPressed = true;
                            break;
                        }
                    }
                    if (keyPressed) {
                        pc += 2;
                    }
                    break;
                }
                case 0x15:      // 0xFX15: Set the delay timer to VX
                    delayTimer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x18:      // 0xFX18: Set the sound timer to VX
                    soundTimer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x1E:      // 0xFX1E: Add VX to register I
                    // If overflow -> VF = 1; else -> VF = 0
                    if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }

                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x29:      // 0xFX29: Set I -> & of sprite for the character in VX
                    // Characters 0-F (in hexadecimal) are represented by a 4x5 font
                    I = V[(opcode & 0x0F00) >> 8] * 5;
                    pc += 2;
                    break;
                case 0x33: {    // 0xFX33: Store the BCD representation of VX at I
                    // With MSD at address I and LSD at address I + 2
                    uint8_t x = V[(opcode & 0x0F00) >> 8];
                    memory[I] = x / 100;
                    memory[I+1] = (x % 100) / 10;
                    memory[I+2] = x % 10;
                    pc += 2;
                    break;
                }
                case 0x55:      // 0xFX55: Store V0 to VX (both included)
                    // In memory starting at address I
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
                        memory[I+i] = V[i];
                    }
                    pc += 2;
                    break;
                case 0x65:      // 0xFX65: Fill V0 to VX (both included)
                    // With values from memory starting at address I
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
                        V[i] = memory[I+i];
                    }
                    pc += 2;
                    break;
                default:
                    unknownOpcode(opcode);
                    return 1;
            }
    }

    if (delayTimer > 0) delayTimer--;
    if (soundTimer > 0) {
        if (soundTimer == 1) {
            soundFlag = true;
        }
        soundTimer--;
    }

    return 0;
}

void Chip8Vm::drawSprite(uint8_t baseX, uint8_t baseY, uint8_t height) {
    V[0xF] = 0;

    for (int j = 0; j < height; j++) {
        uint8_t row = memory[I + j];
        for (int i = 0; i < 8; i++) {
            int x = (baseX + i) % SCREEN_WIDTH;
            int y = (baseY + j) % SCREEN_HEIGHT;
            if (row & (0x80 >> i)) {
                if (display[y][x]) {
                    V[0xF] = 1;
                }
                display[y][x] ^= 1;
            }
        }
    }

    drawFlag = true;
}

