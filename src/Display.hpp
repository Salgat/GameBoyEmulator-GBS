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
    int x = 0;
    int y = 0;
    int tile_number = 0;
    uint8_t attributes = 0;

    bool x_flip = false;
    bool y_flip = false;
    bool draw_priority = false;
    uint8_t palette = 0x00;
    int height = 8;
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
	
	void UpdateSprite(uint8_t sprite_address, uint8_t value);
	
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
    std::vector<bool> sprite_priority; // 0 = False, 1 = True
	std::array<Sprite, 40> sprite_array;

    std::array<int, 8> DrawTilePattern(uint16_t tile_address, int tile_line);
	void DrawBackground(uint8_t lcd_control, int line_number);
    void DrawWindow(uint8_t lcd_control, int line_number);
	void DrawBackgroundOrWindow(uint8_t lcd_control, int line_number, bool is_background);
    std::vector<Sprite*> ReadSprites(uint8_t lcd_control);
    void DrawSprites(uint8_t lcd_control, int line_number, std::vector<Sprite*> const& sprites);
};

#endif //GAMEBOYEMULATOR_DISPLAY_HPP
