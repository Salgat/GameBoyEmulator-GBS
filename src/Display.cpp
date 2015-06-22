//
// Created by Austin on 6/11/2015.
//

#include "Display.hpp"
#include "Processor.hpp"
#include "MemoryManagementUnit.hpp"

#include <iostream>

Display::Display() {
    Reset();
}

/**
 * Initializes references to other Gameboy components
 */
void Display::Initialize(Processor* cpu_, MemoryManagementUnit* mmu_) {
    cpu = cpu_;
    mmu = mmu_;
}

void Display::Reset() {
	frame = sf::Image();
    frame.create(160, 144);
}

sf::Image Display::RenderFrame() {
    std::vector<sf::Color> background_map(256*256, sf::Color::White);

    // First render the background (32x32 tiles, each tile is 8x8 pixels, each pixel has a color)
    uint8_t lcd_control = mmu->zram[0xFF40&0xFF];
    if(lcd_control & 0x01) {
        // Determine if using tile map 0 or 1
        uint16_t tile_map_address;
        if ((lcd_control & 0x08) == 0) {
            tile_map_address = 0x9800; // tile map #0
        } else {
            tile_map_address = 0x9c00; // tile map #1
        }

        // Determine if using tile set 0 or 1
        uint16_t tile_set_address;
        int tile_set_offset;
        if ((lcd_control & 0x10) == 0) {
            tile_set_address = 0x8800; // tile set #0
            tile_set_offset = -128;
        } else {
            tile_set_address = 0x8000; // tile set #1
            tile_set_offset = 0;
        }

        // Go through each entry in tile map and draw it to the background_map
        // 1 row = 32 bytes, each byte is a tile number, 32 rows total
        for (std::size_t y = 0; y < 32; ++y) {
            for (std::size_t x = 0; x < 32; ++x) {
				//std::cout << "Tile number located at: " << std::hex << static_cast<unsigned int>(tile_map_address + (32*y+x)) << std::endl;
                uint8_t tile_number = mmu->ReadByte(tile_map_address + (32*y+x));
                uint16_t tile_address = tile_set_address - tile_set_offset + tile_number*16;
                //std::cout << "Drawing tile number: " << std::hex << static_cast<unsigned int>(tile_number) << " at address: " << static_cast<unsigned int>(tile_address) << std::endl;
                DrawTilePattern(background_map, x, y, tile_address);
            }
        }

        // Scroll
        uint8_t scroll_y = mmu->ReadByte(0xFF42);
        uint8_t scroll_x = mmu->ReadByte(0xFF43);

        // Translate background to frame
        for (std::size_t x = 0; x < 160; ++x) {
            int x_offset = scroll_x;
            if (x+x_offset >= 256) {
                x_offset -= 256;
            }
            for (std::size_t y = 0; y < 144; ++y) {
                int y_offset = scroll_y;
                if (y+y_offset >= 256) {
                    y_offset -= 256;
                }
                frame.setPixel(x, y, background_map[(y+y_offset)*256+(x+x_offset)]);
            }
        }
/*
        std::cout << "tile_map_address: " << std::hex << static_cast<unsigned int>(tile_map_address) << std::endl;
        std::cout << "tile_set_address: " << std::hex << static_cast<unsigned int>(tile_set_address) << std::endl;
        std::cout << "tile_set_offset: " << std::hex << static_cast<unsigned int>(tile_set_offset) << std::endl;
        for (uint16_t index = tile_map_address; index < tile_map_address+8; ++index) {
            std::cout << "MAP index value: " << std::hex << static_cast<unsigned int>(index) << ", " << static_cast<unsigned int>(mmu->ReadByte(index)) << std::endl;
        }*/
    }

    return frame;
}

/**
 * Draws tile on background_map using given pattern address and coordinates.
 */
void Display::DrawTilePattern(std::vector<sf::Color>& background_map, std::size_t x, std::size_t y, uint16_t tile_address) {
    // Iterate through each line, drawing it to the map
    std::size_t line_count = 0;
    for (uint16_t line = tile_address; line < tile_address + 16; line += 2) {
        // A line is made up of two bytes
        uint8_t line_0 = mmu->ReadByte(line);
        uint8_t line_1 = mmu->ReadByte(line+1);
        for (int bit = 7; bit >= 0; --bit) {
            bool bit_0 = line_0 & (0x1<<bit);
            bool bit_1 = line_1 & (0x1<<bit);

            int color;
            if (!bit_1 and !bit_0) {
                color = 0;
            } else if (!bit_1 and bit_0) {
                color = 1;
            } else if (bit_1 and !bit_0) {
                color = 2;
            } else {
                color = 3;
            }

            // TODO: Palette translation
            std::size_t x_pixel = x*8 + 7-bit;
            std::size_t y_pixel = (y*8+line_count);
            sf::Color final_color;
            if (color == 0) {
                background_map[y_pixel*256+x_pixel] = sf::Color(255, 255, 255, 255);
            } else if (color == 1) {
                background_map[y_pixel*256+x_pixel] = sf::Color(192, 192, 192, 255);
            } else if (color == 2) {
                background_map[y_pixel*256+x_pixel] = sf::Color( 96,  96,  96, 255);
            } else {
                background_map[y_pixel*256+x_pixel] = sf::Color(  0,   0,   0, 255);
            }

            //std::cout << "Set pixel for: " << std::dec << y_pixel*256+x_pixel << "... " << y << ", " << x << std::endl;
        }
        ++line_count;
    }
}