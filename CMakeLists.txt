cmake_minimum_required(VERSION 3.22)
project(chip8emulator)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(chip8emulator ${SDL2_INCLUDE_DIRS} src)

add_executable(chip8emulator src/main.cpp src/Chip8/Chip8Vm.h src/Chip8/Chip8Vm.cpp src/Peripherals/Display.h src/Peripherals/Display.cpp src/Peripherals/Input.h src/Peripherals/Input.cpp src/Emulator.cpp src/Emulator.h)
target_link_libraries(chip8emulator ${SDL2_LIBRARIES})
