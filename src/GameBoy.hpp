//
// Created by Austin on 6/11/2015.
//

#ifndef GAMEBOYEMULATOR_GAMEBOY_HPP
#define GAMEBOYEMULATOR_GAMEBOY_HPP

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "Processor.hpp"
#include "MemoryManagementUnit.hpp"
#include "Display.hpp"

/**
 * Emulates a GameBoy, outputting visuals to an sf::Image (160x144 pixels)
 */
class GameBoy {
public:
    GameBoy();
    //~GameBoy();

    void Reset();
    sf::Image RenderFrame();
    void LoadGame(std::string rom_name);

private:
    sf::Image last_frame;

    Processor cpu;
    MemoryManagementUnit mmu;
    Display display;
};

#endif //GAMEBOYEMULATOR_GAMEBOY_HPP
