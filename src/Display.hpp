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
    sf::Color const kWhite     = sf::Color(255, 255, 255, 255);
    sf::Color const kLightGray = sf::Color(192, 192, 192, 255);
    sf::Color const kDarkGray  = sf::Color( 96,  96,  96, 255);
    sf::Color const kBlack     = sf::Color(  0,   0,   0, 255);

public:
    Display();

    void Initialize(Processor* cpu_, MemoryManagementUnit* mmu_);
    void Reset();
	sf::Image RenderFrame();
    void RenderScanline(uint8_t line_number);
	
private:
    Processor* cpu;
    MemoryManagementUnit* mmu;

	sf::Image frame;
    std::vector<sf::Color> background;
    std::vector<sf::Color> frame_buffer;

    void DrawTilePattern(std::vector<sf::Color>& background_map, std::size_t x, std::size_t y, std::size_t tile_x, uint16_t tile_address);
    void DrawBackground(uint8_t lcd_control, uint8_t line_number);
};

#endif //GAMEBOYEMULATOR_DISPLAY_HPP
