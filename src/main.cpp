#include <iostream>

#include "GameBoy.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    GameBoy gameboy;
    gameboy.LoadGame("TESTGAME.GB");

    while(true) {
        //sf::Image frame = gameboy.RenderFrame();
        //DrawFrame(frame);

        break;
    }

}