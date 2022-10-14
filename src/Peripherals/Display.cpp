//
// Created by jonathanpc on 24/07/22.
//

#include "Display.h"

Display::Display() {}

Display::~Display() {
    close();
}

int Display::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    this->window = SDL_CreateWindow("CHIP-8",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH * SCALING_FACTOR,
                              SCREEN_HEIGHT * SCALING_FACTOR,
                              0);
    if (window == nullptr) {
        SDL_Log("Unable to create a window: %s", SDL_GetError());
        return 1;
    }

    this->renderer = SDL_CreateRenderer(this->window,-1,
                                        SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Unable to create a renderer: %s", SDL_GetError());
        return 1;
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    }

    return 0;
}

void Display::close() {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
}

void Display::drawDisplay(array<array<bool, SCREEN_WIDTH>, SCREEN_HEIGHT> &display) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (display[y][x]) {
                SDL_Rect fillRect {
                    SCALING_FACTOR * x,
                    SCALING_FACTOR * y,
                    SCALING_FACTOR,
                    SCALING_FACTOR
                };
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}