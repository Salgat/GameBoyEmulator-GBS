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
    background = std::vector<sf::Color>(256*256, sf::Color::White);
    show_background = std::vector<bool>(256*256, false);
    window = std::vector<sf::Color>(256*256, sf::Color::White);
    show_window = std::vector<bool>(256*256, false);
}

/**
 * Returns the current frame for the GameBoy screen.
 */
sf::Image Display::RenderFrame() {
    // Draw background onto frame, then add window and sprite
    uint8_t scroll_y = mmu->ReadByte(0xFF42);
    uint8_t scroll_x = mmu->ReadByte(0xFF43);

    for (int x = 0; x < 160; ++x) {
        int x_offset = scroll_x;
        if (x+x_offset >= 256) {
            x_offset -= 256;
        }
        for (int y = 0; y < 144; ++y) {
            int y_offset = scroll_y;
            if (y+y_offset >= 256) {
                y_offset -= 256;
            }
            if (show_background[(y+y_offset)*256+(x+x_offset)]) {
                frame.setPixel(x, y, background[(y+y_offset)*256+(x+x_offset)]);
            }
        }
    }

    // Draw Window now
    scroll_y = mmu->ReadByte(0xFF4A);
    scroll_x = mmu->ReadByte(0xFF4B);

    for (int x = 0; x < 160; ++x) {
        int x_offset = static_cast<int>(scroll_x)-7;
        for (int y = 0; y < 144; ++y) {
            int y_offset = scroll_y;
            if (x+x_offset >= 0 and x+x_offset < 256 and y+y_offset >= 0 and y+y_offset < 256) {
                if (show_window[(y+y_offset)*256+(x+x_offset)]) {
                    frame.setPixel(x, y, window[(y+y_offset)*256+(x+x_offset)]);
                }
            }
        }
    }

    // Reset frames
    show_background = std::vector<bool>(256*256, false);
    show_window = std::vector<bool>(256*256, false);

    return frame;
}

/**
 * Renders the current scanline.
 */
void Display::RenderScanline(uint8_t line_number) {
    // First draw background (if enabled)
    uint8_t lcd_control = mmu->zram[0xFF40&0xFF];
    if (lcd_control & 0x01) {
        DrawBackground(lcd_control, line_number);
    }

    // Then draw Window
    if ((lcd_control & 0x20) and (lcd_control & 0x01)) { // I believe both need to be set to draw Window
        DrawWindow(lcd_control, line_number);
    }
}

/**
 * Updates Background vector for current scanline of background.
 */
void Display::DrawBackground(uint8_t lcd_control, uint8_t line_number) {
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

    // Determine which tile row and line of tile to draw
    uint8_t row = static_cast<uint8_t>(std::floor(static_cast<double>(line_number) / 8.0));
    uint8_t tile_row = line_number % 8;
    for (std::size_t x = 0; x < 32; ++x) {
        uint8_t tile_number = mmu->ReadByte(tile_map_address + (32*row+x));
        uint16_t tile_address = tile_set_address - tile_set_offset + tile_number*16;

        DrawTilePattern(background, show_background, x, row, tile_row, tile_address);
    }
}

/**
 * Updates frame buffer for current scanline of Window.
 */
void Display::DrawWindow(uint8_t lcd_control, uint8_t line_number) {
    // Determine if using tile map 0 or 1
    uint16_t tile_map_address;
    if ((lcd_control & 0x40) == 0) {
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

    // Determine which tile row and line of tile to draw
    uint8_t row = static_cast<uint8_t>(std::floor(static_cast<double>(line_number) / 8.0));
    uint8_t tile_row = line_number % 8;
    for (std::size_t x = 0; x < 32; ++x) {
        uint8_t tile_number = mmu->ReadByte(tile_map_address + (32*row+x));
        uint16_t tile_address = tile_set_address - tile_set_offset + tile_number*16;

        DrawTilePattern(window, show_window, x, row, tile_row, tile_address);
    }
}

void Display::DrawTilePattern(std::vector<sf::Color>& map, std::vector<bool>& show_map, std::size_t x, std::size_t y, std::size_t tile_x, uint16_t tile_address) {
    uint16_t line = tile_address + 2*tile_x;
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

        // Create a palette conversion
        uint8_t palette = mmu->zram[0x47];
        sf::Color white;
        sf::Color light_gray;
        sf::Color dark_gray;
        sf::Color black;
        for (uint8_t bits = 0; bits < 7; bits+=2) {
            uint8_t bit0 = (palette & (1<<bits))>>bits;
            uint8_t bit1 = (palette & (1<<(bits+1)))>>(bits+1);
            uint8_t value = bit0 + (bit1 << 1);

            sf::Color new_color;
            if (value == 0x00) {
                new_color = kBlack;
            } else if (value == 0x01) {
                new_color = kDarkGray;
            } else if (value == 0x02) {
                new_color = kLightGray;
            } else if (value == 0x03) {
                new_color = kWhite;
            }

            if (bits == 0) {
                black = new_color;
            } else if (bits == 2) {
                dark_gray = new_color;
            } else if (bits == 4) {
                light_gray = new_color;
            } else if (bits == 6) {
                white = new_color;
            }
        }

        std::size_t x_pixel = x*8 + 7-bit;
        std::size_t y_pixel = (y*8+tile_x);
        if (color == 0) {
            map[y_pixel*256+x_pixel] = white;
        } else if (color == 1) {
            map[y_pixel*256+x_pixel] = light_gray;
        } else if (color == 2) {
            map[y_pixel*256+x_pixel] = dark_gray;
        } else {
            map[y_pixel*256+x_pixel] = black;
        }

        show_map[y_pixel*256+x_pixel] = true;
    }
}