//
// Special credit goes to Imran Nazar's JavaScript implementation of the input (written in JavaScript), which was
// used as a reference. imrannazar.com/GameBoy-Emulation-in-Javascript:-Input
//
#include <iostream>

#include "Input.hpp"
#include "MemoryManagementUnit.hpp"
#include "Display.hpp"
#include "Processor.hpp"
#include "Timer.hpp"
#include "GameBoy.hpp"

#include "serq.hpp"

Input::Input()
	: current_save_slot(1) {
    Reset();
}

void Input::Initialize(MemoryManagementUnit* mmu_, Display* display_, 
				       Timer* timer_, Processor* cpu_, GameBoy* gameboy_, sf::RenderWindow* window_) {
    mmu = mmu_;
	display = display_;
	timer = timer_;
	cpu = cpu_;
    window = window_;
}

/**
 * Resets Input to an initial state.
 */
void Input::Reset() {
    rows[0] = 0x0F;
    rows[1] = 0x0F;
    column = 0x00;
}

/**
 * Depending on the bits written to 5 and 6 of 0xFF00, returns which selected inputs were pressed and not pressed.
 */
uint8_t Input::ReadByte() {
    switch (column) {
        case 0x00:
            return 0x00;
        case 0x10:
            return rows[0];
        case 0x20:
            return rows[1];
        default:
            return 0x00;
    }
}

/**
 * Selects which inputs to return upon reading.
 */
void Input::WriteByte(uint8_t value) {
    column = value & 0x30;
}

/**
 * Sets bits for inputs that are released (1 = not pressed).
 */
void Input::KeyUp(KeyType key) {
    switch(key) {
        case KeyType::RIGHT:  rows[1] |= 0x01; break;
        case KeyType::LEFT:   rows[1] |= 0x02; break;
        case KeyType::UP:     rows[1] |= 0x04; break;
        case KeyType::DOWN:   rows[1] |= 0x08; break;
        case KeyType::A:      rows[0] |= 0x01; break;
        case KeyType::B:      rows[0] |= 0x02; break;
        case KeyType::SELECT: rows[0] |= 0x04; break;
        case KeyType::START:  rows[0] |= 0x08; break;
    }
}

/**
 * Clears bits for inputs that are pressed (0 = pressed).
 */
void Input::KeyDown(KeyType key) {
    mmu->interrupt_flag |= 0x04;

    switch(key) {
        case KeyType::RIGHT:  rows[1] &= 0xE; break;
        case KeyType::LEFT:   rows[1] &= 0xD; break;
        case KeyType::UP:     rows[1] &= 0xB; break;
        case KeyType::DOWN:   rows[1] &= 0x7; break;
        case KeyType::A:      rows[0] &= 0xE; break;
        case KeyType::B:      rows[0] &= 0xD; break;
        case KeyType::SELECT: rows[0] &= 0xB; break;
        case KeyType::START:  rows[0] &= 0x7; break;
    }
}

/**
 * Handles user inputs such as close window and resize window.
 */
bool Input::PollEvents() {
    sf::Event event;
	sf::Image screenshot;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return false;
        } else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
				// Choose Save/Load Slot for game
				case sf::Keyboard::Num1:
				case sf::Keyboard::Numpad1:
					current_save_slot = 1;
					break;
					
				case sf::Keyboard::Num2:
					current_save_slot = 2;
					break;
					
				case sf::Keyboard::Num3:
					current_save_slot = 3;
					break;	
					
				case sf::Keyboard::Num4:
					current_save_slot = 4;
					break;	
					
				case sf::Keyboard::Num5:
					current_save_slot = 5;
					break;	
					
				case sf::Keyboard::Num6:
					current_save_slot = 6;
					break;	
					
				case sf::Keyboard::Num7:
					current_save_slot = 7;
					break;	
					
				case sf::Keyboard::Num8:
					current_save_slot = 8;
					break;	
				
				case sf::Keyboard::Num9:
					current_save_slot = 9;
					break;	
					
				// Save game state for current slot	
				case sf::Keyboard::S:
					SaveGameState(current_save_slot);
					break;
					
				// Load game state for current slot	
				case sf::Keyboard::L:
					LoadGameState(current_save_slot);
					break;	
					
				// Print screen (for some reason PrintScrn isn't support by SFML	
				case sf::Keyboard::P:
					screenshot = window->capture();
					screenshot.saveToFile(mmu->game_title + ".png");
					break;
					
				// Print Screen (original resolution)	
				case sf::Keyboard::O:
					gameboy->frame.saveToFile(mmu->game_title + ".png");
					break;
					
				// Increase screen size
				case sf::Keyboard::Add:
					
					break;

				// Decrease screen size
				case sf::Keyboard::Subtract:
				
					break;
				
				// Increase game speed
				case sf::Keyboard::LBracket:
				
					break;
				
				// Decrease game speed
				case sf::Keyboard::RBracket:
				
					break;
			}
		}
    }

    return true;
}

/**
 * Upon reading 0xFF00, updates the inputs to be returned.
 */
void Input::UpdateInput() {
    static bool left = false;
    static bool right = false;
    static bool up = false;
    static bool down = false;
    static bool a = false;
    static bool b = false;
    static bool start = false;
    static bool select = false;

    // Read button states
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) and !left) {
        KeyDown(KeyType::LEFT);
        left = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) and left) {
        KeyUp(KeyType::LEFT);
        left = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) and !right) {
        KeyDown(KeyType::RIGHT);
        right = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) and right) {
        KeyUp(KeyType::RIGHT);
        right = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and !up) {
        KeyDown(KeyType::UP);
        up = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and up) {
        KeyUp(KeyType::UP);
        up = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and !down) {
        KeyDown(KeyType::DOWN);
        down = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and down) {
        KeyUp(KeyType::DOWN);
        down = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) and !a) {
        KeyDown(KeyType::A);
        a = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::X) and a) {
        KeyUp(KeyType::A);
        a = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) and !b) {
        KeyDown(KeyType::B);
        b = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z) and b) {
        KeyUp(KeyType::B);
        b = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) and !start) {
        KeyDown(KeyType::START);
        start = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) and start) {
        KeyUp(KeyType::START);
        start = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) and !select) {
        KeyDown(KeyType::SELECT);
        select = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) and select) {
        KeyUp(KeyType::SELECT);
        select = false;
    }
}

/**
 * Saves game by serializing game state for given slot.
 */
void Input::SaveGameState(int save_slot) {
	// Save file format: GAMENAME_SLOTNUMBER.gbs
	
	// Serialize game state
	serq::SerializeQueue save_data;
	
	// Cartridge Header
	save_data.push<std::string>(mmu->game_title);
	save_data.push<uint64_t>(mmu->cartridge_type);
	save_data.push<uint64_t>(mmu->ram_size);
	
	// MBC
	save_data.push<uint64_t>(mmu->mbc.rom_bank);
	save_data.push<uint64_t>(mmu->mbc.number_rom_banks);
	save_data.push<uint64_t>(mmu->mbc.rom_offset);
	save_data.push<uint64_t>(mmu->mbc.ram_bank);
	save_data.push<uint64_t>(mmu->mbc.number_ram_banks);
	save_data.push<uint64_t>(mmu->mbc.ram_enabled?1:0);
	save_data.push<uint64_t>(mmu->mbc.ram_offset);
	save_data.push<uint64_t>(mmu->mbc.mode);
	
	save_data.push<uint64_t>(mmu->mbc.mbc1?1:0);
	save_data.push<uint64_t>(mmu->mbc.mbc1_banking_mode?1:0);
	save_data.push<uint64_t>(mmu->mbc.mbc2?1:0);
	save_data.push<uint64_t>(mmu->mbc.mmm01?1:0);
	save_data.push<uint64_t>(mmu->mbc.mbc3?1:0);
	save_data.push<uint64_t>(mmu->mbc.latch_data);
	save_data.push<uint64_t>(mmu->mbc.mbc5?1:0);
	save_data.push<uint64_t>(mmu->mbc.mbc7?1:0);
	save_data.push<uint64_t>(mmu->mbc.tama5?1:0);
	save_data.push<uint64_t>(mmu->mbc.huc3?1:0);
	save_data.push<uint64_t>(mmu->mbc.huc1?1:0);
	save_data.push<uint64_t>(mmu->mbc.sram?1:0);
	save_data.push<uint64_t>(mmu->mbc.battery?1:0);
	save_data.push<uint64_t>(mmu->mbc.timer?1:0);
	save_data.push<uint64_t>(mmu->mbc.rumble?1:0);
	save_data.push<uint64_t>(mmu->mbc.camera?1:0);
	
	save_data.push<uint64_t>(mmu->mbc.rtc.seconds);
	save_data.push<uint64_t>(mmu->mbc.rtc.minutes);
	save_data.push<uint64_t>(mmu->mbc.rtc.hours);
	save_data.push<uint64_t>(mmu->mbc.rtc.day_lower);
	save_data.push<uint64_t>(mmu->mbc.rtc.day_upper);
	
	// Registers
	save_data.push<uint64_t>(mmu->interrupt_enable);
	save_data.push<uint64_t>(mmu->interrupt_flag);
	save_data.push<uint64_t>(mmu->bios_mode?1:0);
	
	// RAM state
	SaveCharVector(save_data, mmu->vram, 0x2000);
	SaveCharVector(save_data, mmu->eram, 0x20000);
	SaveCharVector(save_data, mmu->wram, 0x2000);
	SaveCharVector(save_data, mmu->oam, 0x100);
	SaveCharVector(save_data, mmu->zram, 0x100);
	SaveCharVector(save_data, mmu->hram, 0x100);
	
	// Display (only need to save sprite array)
	for (std::size_t index = 0; index < 40; ++index) {
		save_data.push<uint64_t>(display->sprite_array[index].x);
		save_data.push<uint64_t>(display->sprite_array[index].y);
		save_data.push<uint64_t>(display->sprite_array[index].tile_number);
		save_data.push<uint64_t>(display->sprite_array[index].attributes);
		
		save_data.push<uint64_t>(display->sprite_array[index].x_flip?1:0);
		save_data.push<uint64_t>(display->sprite_array[index].y_flip?1:0);
		save_data.push<uint64_t>(display->sprite_array[index].draw_priority?1:0);
		save_data.push<uint64_t>(display->sprite_array[index].palette);
		save_data.push<uint64_t>(display->sprite_array[index].height);
	}
	
	// Timer
	save_data.push<uint64_t>(timer->clock);
	save_data.push<uint64_t>(timer->divider_clock_tracker);
	save_data.push<uint64_t>(timer->divider_clock);
	save_data.push<uint64_t>(timer->counter_clock_tracker);
	save_data.push<uint64_t>(timer->counter_clock);
	save_data.push<uint64_t>(timer->scanline);
	save_data.push<uint64_t>(timer->scanline_tracker);
	save_data.push<uint64_t>(timer->v_blank_triggered?1:0);
	
	// Processor
	save_data.push<uint64_t>(cpu->AF.word);
	save_data.push<uint64_t>(cpu->AF_stack.word);
	save_data.push<uint64_t>(cpu->BC.word);
	save_data.push<uint64_t>(cpu->BC_stack.word);
	save_data.push<uint64_t>(cpu->DE.word);
	save_data.push<uint64_t>(cpu->DE_stack.word);
	save_data.push<uint64_t>(cpu->HL.word);
	save_data.push<uint64_t>(cpu->HL_stack.word);
	save_data.push<uint64_t>(cpu->stack_pointer.word);
	save_data.push<uint64_t>(cpu->program_counter.word);
	save_data.push<uint64_t>(cpu->interrupt_master_enable);
	save_data.push<uint64_t>(cpu->halt);
	save_data.push<uint64_t>(cpu->frame_clock);
	save_data.push<uint64_t>(cpu->clock);
	save_data.push<uint64_t>(cpu->m_clock);
	
	std::string save_name = mmu->game_title + std::string("_") 
							+ std::to_string(save_slot) + std::string(".gbs");
	save_data.Serialize(save_name); // Todo: May want to differentiate by version number also to prevent incompatibilities (0x014C)
}

/**
 * Saves the provided 8 bit vector to the serialize queue.
 */
void Input::SaveCharVector(serq::SerializeQueue& save_data, std::vector<uint8_t>& data, std::size_t length) {
	for (std::size_t offset = 0; offset < length; offset += 8) {
		// Since data is stored 8 bits at a time, pack 8 bytes into one uint64_t
		uint64_t package = (static_cast<uint64_t>(data[offset+0]) << (0*8)) +
						   (static_cast<uint64_t>(data[offset+1]) << (1*8)) +
						   (static_cast<uint64_t>(data[offset+2]) << (2*8)) +
						   (static_cast<uint64_t>(data[offset+3]) << (3*8)) +
						   (static_cast<uint64_t>(data[offset+4]) << (4*8)) +
						   (static_cast<uint64_t>(data[offset+5]) << (5*8)) +
						   (static_cast<uint64_t>(data[offset+6]) << (6*8)) +
						   (static_cast<uint64_t>(data[offset+7]) << (7*8));
		save_data.push<uint64_t>(package);				   
	}
}

/**
 * Loads game state based on given save slot from a file.
 */
void Input::LoadGameState(int save_slot) {
	// Load file format: GAMENAME_SLOTNUMBER.gbs
	
	// Deserialize game state
    serq::SerializeQueue save_data;
	std::string save_name = mmu->game_title + std::string("_") 
							+ std::to_string(save_slot) + std::string(".gbs");
	save_data.Deserialize(save_name);
	
	// Cartridge Header
	mmu->game_title = save_data.pop<std::string>();
	mmu->cartridge_type = save_data.pop<uint64_t>();
	mmu->ram_size = save_data.pop<uint64_t>();
	
	// MBC
	mmu->mbc.rom_bank = save_data.pop<uint64_t>();
    mmu->mbc.number_rom_banks = save_data.pop<uint64_t>();
    mmu->mbc.rom_offset = save_data.pop<uint64_t>();
    mmu->mbc.ram_bank = save_data.pop<uint64_t>();
    mmu->mbc.number_ram_banks = save_data.pop<uint64_t>();
    mmu->mbc.ram_enabled = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.ram_offset = save_data.pop<uint64_t>();
    mmu->mbc.mode = save_data.pop<uint64_t>();
    
    mmu->mbc.mbc1 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.mbc1_banking_mode = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.mbc2 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.mmm01 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.mbc3 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.latch_data = save_data.pop<uint64_t>();
    mmu->mbc.mbc5 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.mbc7 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.tama5 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.huc3 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.huc1 = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.sram = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.battery = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.timer = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.rumble = save_data.pop<uint64_t>()?true:false;
    mmu->mbc.camera = save_data.pop<uint64_t>()?true:false;
    
    mmu->mbc.rtc.seconds = save_data.pop<uint64_t>();
    mmu->mbc.rtc.minutes = save_data.pop<uint64_t>();
    mmu->mbc.rtc.hours = save_data.pop<uint64_t>();
    mmu->mbc.rtc.day_lower = save_data.pop<uint64_t>();
    mmu->mbc.rtc.day_upper = save_data.pop<uint64_t>();
	
	// Registers
	mmu->interrupt_enable = save_data.pop<uint64_t>();
    mmu->interrupt_flag = save_data.pop<uint64_t>();
    mmu->bios_mode = save_data.pop<uint64_t>()?true:false;
	
	// RAM state
	LoadCharVector(save_data, mmu->vram, 0x2000);
	LoadCharVector(save_data, mmu->eram, 0x20000);
	LoadCharVector(save_data, mmu->wram, 0x2000);
	LoadCharVector(save_data, mmu->oam, 0x100);
	LoadCharVector(save_data, mmu->zram, 0x100);
	LoadCharVector(save_data, mmu->hram, 0x100);
	
	// Display (only need to load sprite array)
	for (std::size_t index = 0; index < 40; ++index) {
		display->sprite_array[index].x = save_data.pop<uint64_t>();
		display->sprite_array[index].y = save_data.pop<uint64_t>();
		display->sprite_array[index].tile_number = save_data.pop<uint64_t>();
		display->sprite_array[index].attributes = save_data.pop<uint64_t>();
		
		display->sprite_array[index].x_flip = save_data.pop<uint64_t>()?true:false;
		display->sprite_array[index].y_flip = save_data.pop<uint64_t>()?true:false;
		display->sprite_array[index].draw_priority = save_data.pop<uint64_t>()?true:false;
		display->sprite_array[index].palette = save_data.pop<uint64_t>();
		display->sprite_array[index].height = save_data.pop<uint64_t>();
	}
	
	// Timer
	timer->clock = save_data.pop<uint64_t>();
	timer->divider_clock_tracker = save_data.pop<uint64_t>();
	timer->divider_clock = save_data.pop<uint64_t>();
	timer->counter_clock_tracker = save_data.pop<uint64_t>();
	timer->counter_clock = save_data.pop<uint64_t>();
	timer->scanline = save_data.pop<uint64_t>();
	timer->scanline_tracker = save_data.pop<uint64_t>();
	timer->v_blank_triggered = save_data.pop<uint64_t>()?true:false;
	
	// Processor
	cpu->AF.word = save_data.pop<uint64_t>();
	cpu->AF_stack.word = save_data.pop<uint64_t>();
	cpu->BC.word = save_data.pop<uint64_t>();
	cpu->BC_stack.word = save_data.pop<uint64_t>();
	cpu->DE.word = save_data.pop<uint64_t>();
	cpu->DE_stack.word = save_data.pop<uint64_t>();
	cpu->HL.word = save_data.pop<uint64_t>();
	cpu->HL_stack.word = save_data.pop<uint64_t>();
	cpu->stack_pointer.word = save_data.pop<uint64_t>();
	cpu->program_counter.word = save_data.pop<uint64_t>();
	cpu->interrupt_master_enable = save_data.pop<uint64_t>();
	cpu->halt = save_data.pop<uint64_t>();
	cpu->frame_clock = save_data.pop<uint64_t>();
	cpu->clock = save_data.pop<uint64_t>();
	cpu->m_clock = save_data.pop<uint64_t>();
}

/**
 * Loads the provided 8 bit vector from the serialize queue.
 */
void Input::LoadCharVector(serq::SerializeQueue& save_data, std::vector<uint8_t>& data, std::size_t length) {
	for (std::size_t offset = 0; offset < length; offset += 8) {
		// Since data is stored 8 bits at a time, pack 8 bytes into one uint64_t
		uint64_t package = save_data.pop<uint64_t>();
		
		data[offset+0] = (package & (static_cast<uint64_t>(0xFF)<<(0*8))) >> 0*8;
		data[offset+1] = (package & (static_cast<uint64_t>(0xFF)<<(1*8))) >> 1*8;
		data[offset+2] = (package & (static_cast<uint64_t>(0xFF)<<(2*8))) >> 2*8;
		data[offset+3] = (package & (static_cast<uint64_t>(0xFF)<<(3*8))) >> 3*8;
		data[offset+4] = (package & (static_cast<uint64_t>(0xFF)<<(4*8))) >> 4*8;
		data[offset+5] = (package & (static_cast<uint64_t>(0xFF)<<(5*8))) >> 5*8;
		data[offset+6] = (package & (static_cast<uint64_t>(0xFF)<<(6*8))) >> 6*8;
		data[offset+7] = (package & (static_cast<uint64_t>(0xFF)<<(7*8))) >> 7*8;
	}
}