//
// Created by Austin on 6/11/2015.
//

#include "GameBoy.hpp"

#include <iostream>
#include <fstream>


GameBoy::GameBoy(sf::RenderWindow& window)
	: screen_size(1)
	, game_speed(1) {
    cpu.Initialize(&mmu);
    mmu.Initialize(&cpu, &input, &display, &timer);
    display.Initialize(&cpu, &mmu);
    timer.Initialize(&cpu, &mmu, &display);
	input.Initialize(&mmu, &display, &timer, &cpu, this, &window);

	Reset();
}

void GameBoy::LoadGame(std::string rom_name) {
    mmu.LoadRom(rom_name);
}

void GameBoy::Reset() {
    cpu.Reset();
    mmu.Reset();
    timer.Reset();
    frame_counter = 0;
}

// Todo: Frame calling v-blank 195-196x per frame??
std::pair<sf::Image, bool> GameBoy::RenderFrame() {
    bool running = (input.PollEvents())?true:false;
	cpu.frame_clock = cpu.clock + 17556; // Number of cycles/4 for one frame before v-blank
	bool v_blank = false;
	do {
        if (cpu.halt) {
            cpu.clock += 1;
        } else {
            cpu.ExecuteNextInstruction();
        }

        uint8_t if_memory_value = mmu.ReadByte(0xFF0F);
        if (mmu.interrupt_enable and cpu.interrupt_master_enable and if_memory_value) {
			cpu.halt = 0;
			cpu.interrupt_master_enable = 0;
			uint8_t interrupt_fired = mmu.interrupt_enable & if_memory_value;

            if (interrupt_fired & 0x01) {if_memory_value &= 0XFE; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST40();}
			else if (interrupt_fired & 0x02) {if_memory_value &= 0XFD; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST48();}
			else if (interrupt_fired & 0x04) {if_memory_value &= 0XFB; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST50();}
			else if (interrupt_fired & 0x08) {if_memory_value &= 0XF7; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST58();}
			else if (interrupt_fired & 0x10) {if_memory_value &= 0XEF; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST60();}
			else {cpu.interrupt_master_enable = 1;}
			
			mmu.WriteByte(0xFF0F, if_memory_value);
		}
		
		timer.Increment();
	} while(cpu.clock < cpu.frame_clock);
	
	if (!v_blank) {
		frame = display.RenderFrame();
	}
    
    // Update the .SAV file if flagged (once per second)
    if (++frame_counter >= 60) {
        frame_counter = 0;
        if (mmu.updateSaveFile) {
            SaveGame();
            mmu.updateSaveFile = false;
        }
    }
	return std::make_pair(frame, running);
}

/**
 * Saves eram to a .SAV file. (RTC is not implemented yet)
 */
void GameBoy::SaveGame() {
    std::string save_name = mmu.game_title + std::string(".sav");
    
    std::ofstream OutFile;
    OutFile.open(save_name, std::ios::out | std::ios::binary);
    std::copy(mmu.eram.begin(), mmu.eram.begin()+0x8000, std::ostreambuf_iterator<char>(OutFile));
    OutFile.close();
}