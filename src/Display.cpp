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
	uint8_t lcd_control = mmu->zram[0xFF40 & 0xFF];

	if (lcd_control & 0x01) {
		for (unsigned int line = 0; line < 144; ++line) {
			DrawBackground(lcd_control, line);
		}
	}

	if (lcd_control & 0x20) {
		for (unsigned int line = 0; line < 144; ++line) {
			DrawWindow(lcd_control, line);
		}
	}

	if (lcd_control & 0x02) {
		auto sprites = ReadSprites(lcd_control);
		//std::cout << "Sprite size: " << sprites.size() << std::endl;
		for (unsigned int line = 0; line < 144; ++line) {
			DrawSprites(lcd_control, line, sprites);
		}
	}

	for (int x = 0; x < 160; ++x) {
        for (int y = 0; y < 144; ++y) {
			if (show_background[y*256+x])
				frame.setPixel(x, y, background[y*256+x]);
				
			if (show_window[y*256+x])
				frame.setPixel(x, y, window[y*256+x]);

			if (show_sprite[y*256+x])
				frame.setPixel(x, y, sprite_map[y*256+x]);

        }
    }

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
    /*uint8_t lcd_control = mmu->zram[0xFF40&0xFF];
    if (lcd_control & 0x01) {
        DrawBackground(lcd_control, line_number);
    }
	
    // Then draw Window
    if ((lcd_control & 0x20) and (lcd_control & 0x01)) { // I believe both need to be set to draw Window
        //DrawWindow(lcd_control, line_number);
    }

    // Finally draw Sprites
    if (lcd_control & 0x02) {
        DrawSprites(lcd_control, line_number);
    }
    */
}

/**
 * Updates Background vector for current scanline of background.
 */
void Display::DrawBackground(uint8_t lcd_control, int line_number) {
    // Determine if using tile map 0 or 1
    uint16_t tile_map_address;
    if ((lcd_control & 0x08) == 0) {
        tile_map_address = 0x9800; // tile map #0
    } else {
        tile_map_address = 0x9c00; // tile map #1
    }

    // Determine if using tile set 0 or 1
    int tile_set_address;
    int tile_set_offset;
    if ((lcd_control & 0x10) == 0) {
        tile_set_address = 0x9000; // tile set #0
        tile_set_offset = 256;
    } else {
        tile_set_address = 0x8000; // tile set #1
        tile_set_offset = 0;
    }

    // Determine which tile row and line of tile to draw
    // First find which tile the scanline is on
	int scroll_y = mmu->ReadByte(0xFF42);
	int scroll_y_tile = std::floor(scroll_y/8.0);// Sets scroll_y to be divisible by 8
	//int row = (line_number + scroll_y_tile) % 32; // Which row (32x32) on background
	int row = static_cast<int>(std::floor(static_cast<double>(line_number+scroll_y) / 8.0)) % 32;
	int tile_line = (scroll_y + line_number) % 8; // Which y tile (8x8 or 8x16) on background
												  // TODO: Change modulus to be either 8 or 16
	
	// Setup Palette for scanline
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
	
	// For each tile, draw result
	for (int x_tile = 0; x_tile < 32; ++x_tile) {
		//std::cout << "Tile Number from address: " << tile_map_address + (32*row+x_tile) << std::endl;
		int tile_number = mmu->ReadByte(tile_map_address + (32*row+x_tile));
        if (tile_number > 127) {
            tile_number -= tile_set_offset;
        }
	
		//std::cout << "Scan line: " << static_cast<unsigned int>(line_number) << std::endl;
		uint16_t tile_address = tile_set_address + tile_number*16;
		auto line_pixels = DrawTilePattern(tile_address, tile_line);
		
		// Convert color based on palette
		std::array<sf::Color, 8> color_pixels;
		for (unsigned int index = 0; index < 8; ++index) {
			if (line_pixels[index] == 0) {
				color_pixels[index] = white;
            } else if (line_pixels[index] == 1) {
                color_pixels[index] = light_gray;
            } else if (line_pixels[index] == 2) {
                color_pixels[index] = dark_gray;
            } else {
                color_pixels[index] = black;
            }
		}
		
		// Write pixels to background map
		int bit = 7;
		for (auto pixel : color_pixels) {
		    int scroll_x = mmu->ReadByte(0xFF43);
			unsigned int x_position = (x_tile*8 + bit + 256-scroll_x) % 256;
			unsigned int y_position = line_number;
			background[y_position*256+x_position] = pixel;
			show_background[y_position*256+x_position] = true;
			
			--bit;
		}
	}
}

/**
 * Updates frame buffer for current scanline of Window.
 */
void Display::DrawWindow(uint8_t lcd_control, int line_number) {
	// Determine if using tile map 0 or 1
    uint16_t tile_map_address;
    if ((lcd_control & 0x40) == 0) {
        tile_map_address = 0x9800; // tile map #0
    } else {
        tile_map_address = 0x9c00; // tile map #1
    }

    // Determine if using tile set 0 or 1
    int tile_set_address;
    int tile_set_offset;
    if ((lcd_control & 0x10) == 0) {
        tile_set_address = 0x9000; // tile set #0
        tile_set_offset = 256;
    } else {
        tile_set_address = 0x8000; // tile set #1
        tile_set_offset = 0;
    }

    // Determine which tile row and line of tile to draw
    // First find which tile the scanline is on
	int scroll_y = mmu->ReadByte(0xFF4A);
	int scroll_y_tile = std::floor(scroll_y/8.0);// Sets scroll_y to be divisible by 8
	//int row = (line_number + scroll_y_tile) % 32; // Which row (32x32) on background
	int row = static_cast<int>(std::floor(static_cast<double>(line_number+scroll_y) / 8.0)) % 32;
	int tile_line = (scroll_y + line_number) % 8; // Which y tile (8x8 or 8x16) on background
												  // TODO: Change modulus to be either 8 or 16
	
	// Setup Palette for scanline
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
	
	// For each tile, draw result
	for (int x_tile = 0; x_tile < 32; ++x_tile) {
		//std::cout << "Tile Number from address: " << tile_map_address + (32*row+x_tile) << std::endl;
		int tile_number = mmu->ReadByte(tile_map_address + (32*row+x_tile));
        if (tile_number > 127) {
            tile_number -= tile_set_offset;
        }
	
		//std::cout << "Scan line: " << static_cast<unsigned int>(line_number) << std::endl;
		uint16_t tile_address = tile_set_address + tile_number*16;
		auto line_pixels = DrawTilePattern(tile_address, tile_line);
		
		// Convert color based on palette
		std::array<sf::Color, 8> color_pixels;
		for (unsigned int index = 0; index < 8; ++index) {
			if (line_pixels[index] == 0) {
				color_pixels[index] = white;
            } else if (line_pixels[index] == 1) {
                color_pixels[index] = light_gray;
            } else if (line_pixels[index] == 2) {
                color_pixels[index] = dark_gray;
            } else {
                color_pixels[index] = black;
            }
		}
		
		// Write pixels to window map
		int bit = 7;
		for (auto pixel : color_pixels) {
		    int scroll_x = mmu->ReadByte(0xFF4B);
			unsigned int x_position = (x_tile*8+bit);
			unsigned int y_position = line_number+16;
			if (x_position-8 >= scroll_x and y_position >= scroll_y) {
				window[y_position*256+x_position] = pixel;
				show_window[y_position*256+x_position] = true;
			}
			
			--bit;
		}
	}
}

/**
 * Returns all Sprites in OAM (0xFE00-0xFE9F).
 */
std::vector<Sprite> Display::ReadSprites(uint8_t lcd_control) {
	std::vector<Sprite> sprites;

	uint16_t oam_table_address = 0xFE00;
	uint8_t sprite_height = (lcd_control & 0x04)?16:8; // Height of each sprite in pixels
	for (int index = 0; index < 40; ++index) {
		int y_position = mmu->ReadByte(oam_table_address+index*4+0);
		int x_position = mmu->ReadByte(oam_table_address+index*4+1);

		// Test if within screen view
		if (y_position-16 >= 0 and y_position-16 < 144 and x_position-8 >= 0 and x_position-8 < 160) {
			Sprite new_sprite;
			new_sprite.y = y_position;
			new_sprite.x = x_position;
			new_sprite.tile_number = mmu->ReadByte(oam_table_address + index * 4 + 2);
			new_sprite.attributes = mmu->ReadByte(oam_table_address + index * 4 + 3);
			new_sprite.x_flip = (new_sprite.attributes & 0x20) ? true : false;
			new_sprite.y_flip = (new_sprite.attributes & 0x40) ? true : false;
			new_sprite.draw_priority = (new_sprite.attributes & 0x80) ? true : false; // If true, don't draw over background/window colors 1-3
			new_sprite.palette = (!(new_sprite.attributes & 0x10)) ? mmu->ReadByte(0xFF49) : mmu->ReadByte(0xFF48); // Which palette to use (0=OBP0, 1=OBP1)
			new_sprite.height = sprite_height;

			sprites.push_back(std::move(new_sprite));
		}
	}

	// Sort each sprite by its x position (the lowest x position is drawn last)
	// TODO: Add a condition where sprites with the same X and sorted by their OAM address
	std::sort(sprites.begin(), sprites.end(),
			  [](Sprite const& first, Sprite const& second) -> bool {
				  return first.x < second.x;
			  });

	return sprites;
}

void Display::DrawSprites(uint8_t lcd_control, int line_number, std::vector<Sprite> const& sprites) {
	uint16_t sprite_pattern_table = 0x8000; // Unsigned numbering

	int drawn_count = 0;
    for (int index = sprites.size()-1; index >= 0; --index) {
		if (drawn_count >= 10) break; // Limit to drawing the first 10 sprites of highest priority
		//std::cout << "Testing for index: " << index << std::endl;
		if (sprites[index].y + sprites[index].height > line_number and sprites[index].y <= line_number) {
			//std::cout << "Found sprite on scanline: " << line_number << std::endl;
			// If on the scanline, draw the sprite's current line
			uint16_t tile_address = sprite_pattern_table + sprites[index].tile_number*16;
			int tile_line = line_number-sprites[index].y;
			if (sprites[index].y_flip) {
				tile_line = sprites[index].height - 1 - tile_line;
			}
			auto line_pixels = DrawTilePattern(tile_address, tile_line);

			// Setup Palette for scanline
			uint8_t palette = sprites[index].palette;
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

			// Convert color based on palette
			std::array<sf::Color, 8> color_pixels;
			for (unsigned int index = 0; index < 8; ++index) {
				if (line_pixels[index] == 0) {
					color_pixels[index] = white;
				} else if (line_pixels[index] == 1) {
					color_pixels[index] = light_gray;
				} else if (line_pixels[index] == 2) {
					color_pixels[index] = dark_gray;
				} else {
					color_pixels[index] = black;
				}
			}

			// Write pixels to window map
			int bit = 7;
			for (auto pixel : color_pixels) {
				unsigned int x_position = (sprites[index].x_flip)?sprites[index].x+7-bit-8:sprites[index].x+bit-8;
				unsigned int y_position = line_number-16;
				sprite_map[y_position*256+x_position] = pixel;
				show_sprite[y_position*256+x_position] = true;

				--bit;
			}

			++drawn_count;
		}
	}
}

/**
 * Returns the 8 pixel row of the given tile.
 */
std::array<int, 8> Display::DrawTilePattern(uint16_t tile_address, int tile_line) {
	//std::cout << "Pattern and line: " << static_cast<unsigned int>(tile_address) << ", " << static_cast<unsigned int>(tile_line) << std::endl;
	uint16_t line = tile_address + 2*tile_line;
	uint8_t line_0 = mmu->ReadByte(line);
    uint8_t line_1 = mmu->ReadByte(line+1);
	std::array<int, 8> pixels;
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
        } else if (bit_1 and bit_0) {
            color = 3;
        }
		
		pixels[bit] = color;
	}	
	
	return pixels;
}