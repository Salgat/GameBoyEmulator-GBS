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

    sprite_map = std::vector<sf::Color>(256*256, sf::Color::White);
    show_sprite = std::vector<bool>(256*256, false);
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

    // Draw Sprites
    for (int x = 0; x < 160; ++x) {
        for (int y = 0; y < 144; ++y) {
            if (show_sprite[y*256 + x]) {
                frame.setPixel(x, y, sprite_map[y * 256 + x]);
            }
        }
    }

    // Reset frames
    show_background = std::vector<bool>(256*256, false);
    show_window = std::vector<bool>(256*256, false);
    show_sprite = std::vector<bool>(256*256, false);

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

    // Finally draw Sprites
    if (lcd_control & 0x02) {
        DrawSprites(lcd_control, line_number);
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

void Display::DrawSprites(uint8_t lcd_control, uint8_t line_number) {
    uint16_t oam_table_address = 0xFE00;
    uint16_t sprite_pattern_table = 0x8000; // Unsigned numbering
    uint8_t sprite_height = (lcd_control & 0x04)?16:8; // Height of each sprite in pixels

    // Iterate through all 40 sprites, drawing the first 10 on the current line_number
    std::vector<Sprite> sprites;
    for (int sprite = 0; sprite < 40; ++sprite) {
        uint8_t y_position = mmu->ReadByte(oam_table_address+sprite*4+0);

        // If on the scanline, add to sprites vector
        if (y_position <= line_number and y_position + sprite_height > line_number) {
            // Construct the sprite and add it to sprites
            Sprite new_sprite;
            new_sprite.x = mmu->ReadByte(oam_table_address+sprite*4+1);
            new_sprite.y = y_position;

            if (new_sprite.x > 0 and new_sprite.x < 160+8 and new_sprite.y > 0 and new_sprite.y < 144+sprite_height+16) {
                new_sprite.tile_number = mmu->ReadByte(oam_table_address + sprite * 4 + 2);
                new_sprite.attributes = mmu->ReadByte(oam_table_address + sprite * 4 + 3);
                new_sprite.x_flip = (new_sprite.attributes & 0x20) ? true : false;
                new_sprite.y_flip = (new_sprite.attributes & 0x40) ? true : false;
                new_sprite.draw_priority = (new_sprite.attributes & 0x80) ? true : false; // If true, don't draw over background/window colors 1-3
                // TODO: Might need to create another bool vector for this
                new_sprite.palette = (new_sprite.attributes & 0x10) ? mmu->ReadByte(0xFF49) : mmu->ReadByte(0xFF48); // Which palette to use
                new_sprite.height = sprite_height;

                sprites.push_back(std::move(new_sprite));
            }
        }
    }

    // Sort each sprite by its x position (the lowest x position is drawn last)
    std::sort(sprites.begin(), sprites.end(),
        [](Sprite const& first, Sprite const& second) -> bool {
            return first.x < second.x;
        });

    // Draw the last 10 sprites
    int size = sprites.size();
    for (int index = size-1; (index >= 0) and (index > size-11); --index) {
        uint16_t tile_address = sprite_pattern_table + sprites[index].tile_number*16;
        DrawTilePattern(sprite_map, show_sprite, sprites[index].x, sprites[index].y, line_number - sprites[index].y, tile_address, true, &sprites[index]);
    }
}

void Display::DrawTilePattern(std::vector<sf::Color>& map, std::vector<bool>& show_map, std::size_t x, std::size_t y, std::size_t tile_x, uint16_t tile_address, bool is_sprite, Sprite const* sprite) {
	// TODO: An idea for sprites is to simply create new sprites every time the OAM is updated, and then update their attributes and other information continuously (more efficient)
	
	uint16_t line = (is_sprite and sprite->y_flip) ? tile_address + 2*((sprite->height-1)-tile_x) : tile_address + 2*tile_x;
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
        uint8_t palette = (is_sprite)?sprite->palette:mmu->zram[0x47];
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

        // Todo: For sprites, don't render "white" pixels
        int x_pixel;
        int y_pixel;
        int destination;
        if (is_sprite) {
			int new_bit = (sprite->x_flip)?(7-bit):(bit);
            x_pixel = static_cast<int>(x + 7-new_bit) - 8;
            y_pixel = static_cast<int>(y+tile_x) - 16;
            destination = y_pixel*256+x_pixel;
        } else {
            x_pixel = x*8 + 7-bit;
            y_pixel = (y*8+tile_x);
            destination = y_pixel*256+x_pixel;
        }

        if (x_pixel >= 0 and x_pixel < 256 and y_pixel >= 0 and y_pixel < 256) {
            if (color == 0) {
                map[destination] = white;
            } else if (color == 1) {
                map[destination] = light_gray;
            } else if (color == 2) {
                map[destination] = dark_gray;
            } else {
                map[destination] = black;
            }
			
			if (is_sprite and sprite->draw_priority) {
				// If priority attribute set, only draw sprite if background and window are white (we assume that r=g=b)
				if ((background[destination].r == 255 or !show_background[destination]) and (window[destination].r == 255 or !show_window[destination])) {
					show_map[destination] = true;
				}
			} else {
				show_map[destination] = true;
			}
        }
    }
}