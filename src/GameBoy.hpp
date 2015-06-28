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

#include <Utility>

#include "Processor.hpp"
#include "MemoryManagementUnit.hpp"
#include "Display.hpp"
#include "Timer.hpp"
#include "Input.hpp"

/**
 * Emulates a GameBoy, outputting visuals to an sf::Image (160x144 pixels)
 */
class GameBoy {
public:
    GameBoy(sf::RenderWindow& window);
    //~GameBoy();

    void Reset();
    std::pair<sf::Image, bool> RenderFrame();
    void LoadGame(std::string rom_name);

//private:
    sf::Image last_frame;

    Processor cpu;
    MemoryManagementUnit mmu;
    Display display;
    Timer timer;
    Input input;
};

#endif //GAMEBOYEMULATOR_GAMEBOY_HPP
