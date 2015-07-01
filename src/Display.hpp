//
// Created by Austin on 6/11/2015.
//

#ifndef GAMEBOYEMULATOR_DISPLAY_HPP
#define GAMEBOYEMULATOR_DISPLAY_HPP

#include <vector>
#include <utility>
#include <algorithm>
#include <array>

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

class Processor;
class MemoryManagementUnit;

/**
 * Defines each of the 40 sprites in the OAM sprite table
 */
struct Sprite {
    uint8_t x;
    uint8_t y;
    uint8_t tile_number;
    uint8_t attributes;

    bool x_flip;
    bool y_flip;
    bool draw_priority;
    uint8_t palette;
    uint8_t height;
};

/**
 * Handles rendering to the "screen".
 */
class Display {
    sf::Color const kWhite       = sf::Color(255, 255, 255, 255);
    sf::Color const kLightGray   = sf::Color(192, 192, 192, 255);
    sf::Color const kDarkGray    = sf::Color( 96,  96,  96, 255);
    sf::Color const kBlack       = sf::Color(  0,   0,   0, 255);
    sf::Color const kTransparent = sf::Color(  1,   1,   1,   0);

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
    std::vector<bool> show_background;
    std::vector<sf::Color> window;
    std::vector<bool> show_window;
    std::vector<sf::Color> sprite_map;
    std::vector<bool> show_sprite;

    std::array<int, 8> DrawTilePattern(uint16_t tile_address, int tile_line);
	void DrawBackground(uint8_t lcd_control, int line_number);
    void DrawWindow(uint8_t lcd_control, int line_number);
    void DrawSprites(uint8_t lcd_control, int line_number);
};

#endif //GAMEBOYEMULATOR_DISPLAY_HPP
