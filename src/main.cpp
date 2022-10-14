#include <iostream>
#include <SDL2/SDL.h>
#include "Emulator.h"


int main(int argc, char** argv) {
    Emulator* emulator = new Emulator();
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <ROM_FILEPATH>\n";
        return EXIT_SUCCESS;
    } else {
        emulator->run(string{argv[1]});
    }
    delete emulator;
    return EXIT_SUCCESS;
}
