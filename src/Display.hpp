//
// Created by Austin on 6/11/2015.
//

#ifndef GAMEBOYEMULATOR_DISPLAY_HPP
#define GAMEBOYEMULATOR_DISPLAY_HPP

#include <vector>
#include <utility>

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

class Processor;
class MemoryManagementUnit;

/**
 * Handles rendering to the "screen".
 */
class Display {
public:
    Display();

    void Initialize(Processor* cpu_, MemoryManagementUnit* mmu_);
    void Reset();
	sf::Image RenderFrame();
	
private:
    Processor* cpu;
    MemoryManagementUnit* mmu;
	sf::Image frame;

    void DrawTilePattern(std::vector<sf::Color>& background_map, std::size_t x, std::size_t y, uint16_t tile_address);
};

#endif //GAMEBOYEMULATOR_DISPLAY_HPP
