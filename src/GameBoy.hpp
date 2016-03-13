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
	int screen_size; // Multiplier
	int game_speed; // Multiplier

    GameBoy(sf::RenderWindow& window);

    void Reset();
    std::pair<sf::Image, bool> RenderFrame();
    void LoadGame(std::string rom_name, std::string save_file);

//private:
    sf::Image frame;
    unsigned int frame_counter;

    Processor cpu;
    MemoryManagementUnit mmu;
    Display display;
    Timer timer;
    Input input;
    
    void SaveGame();
};

#endif //GAMEBOYEMULATOR_GAMEBOY_HPP
