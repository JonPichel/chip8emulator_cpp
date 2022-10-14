//
// Created by jonathanpc on 24/07/22.
//

#include <iostream>
#include "Input.h"

constexpr uint8_t KEYMAP[16] {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v
};

int Input::init() {
    /* I don't think this is necessary as SDL_INIT_VIDEO already initializes events
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize events: %s", SDL_GetError());
        return 1;
    }
     */

    return 0;
}

void Input::setKeys(array<bool, NUM_KEYS> &keys) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    if (speed > 200) {
                        this->speed -= 200;
                        cout << "Speed: " << speed << '\n';
                    }
                    break;
                case SDLK_DOWN:
                    this->speed += 200;
                    cout << "Speed: " << speed << '\n';
                    break;
                case KEYMAP[0x0]:
                    keys[0x0] = true;
                    break;
                case KEYMAP[0x1]:
                    keys[0x1] = true;
                    break;
                case KEYMAP[0x2]:
                    keys[0x2] = true;
                    break;
                case KEYMAP[0x3]:
                    keys[0x3] = true;
                    break;
                case KEYMAP[0x4]:
                    keys[0x4] = true;
                    break;
                case KEYMAP[0x5]:
                    keys[0x5] = true;
                    break;
                case KEYMAP[0x6]:
                    keys[0x6] = true;
                    break;
                case KEYMAP[0x7]:
                    keys[0x7] = true;
                    break;
                case KEYMAP[0x8]:
                    keys[0x8] = true;
                    break;
                case KEYMAP[0x9]:
                    keys[0x9] = true;
                    break;
                case KEYMAP[0xa]:
                    keys[0xa] = true;
                    break;
                case KEYMAP[0xb]:
                    keys[0xb] = true;
                    break;
                case KEYMAP[0xc]:
                    keys[0xc] = true;
                    break;
                case KEYMAP[0xd]:
                    keys[0xd] = true;
                    break;
                case KEYMAP[0xe]:
                    keys[0xe] = true;
                    break;
                case KEYMAP[0xf]:
                    keys[0xf] = true;
                    break;
            }
        } else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case KEYMAP[0x0]:
                    keys[0x0] = false;
                    break;
                case KEYMAP[0x1]:
                    keys[0x1] = false;
                    break;
                case KEYMAP[0x2]:
                    keys[0x2] = false;
                    break;
                case KEYMAP[0x3]:
                    keys[0x3] = false;
                    break;
                case KEYMAP[0x4]:
                    keys[0x4] = false;
                    break;
                case KEYMAP[0x5]:
                    keys[0x5] = false;
                    break;
                case KEYMAP[0x6]:
                    keys[0x6] = false;
                    break;
                case KEYMAP[0x7]:
                    keys[0x7] = false;
                    break;
                case KEYMAP[0x8]:
                    keys[0x8] = false;
                    break;
                case KEYMAP[0x9]:
                    keys[0x9] = false;
                    break;
                case KEYMAP[0xa]:
                    keys[0xa] = false;
                    break;
                case KEYMAP[0xb]:
                    keys[0xb] = false;
                    break;
                case KEYMAP[0xc]:
                    keys[0xc] = false;
                    break;
                case KEYMAP[0xd]:
                    keys[0xd] = false;
                    break;
                case KEYMAP[0xe]:
                    keys[0xe] = false;
                    break;
                case KEYMAP[0xf]:
                    keys[0xf] = false;
                    break;
            }
        }
    }
}