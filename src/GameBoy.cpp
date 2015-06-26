//
// Created by Austin on 6/11/2015.
//

#include "GameBoy.hpp"

#include <iostream>

GameBoy::GameBoy() {
    cpu.Initialize(&mmu);
    mmu.Initialize(&cpu, &input);
    display.Initialize(&cpu, &mmu);
    timer.Initialize(&cpu, &mmu);
	input.Initialize(&mmu);

	Reset();
}

void GameBoy::LoadGame(std::string rom_name) {
    mmu.LoadRom(rom_name);
}

void GameBoy::Reset() {
    cpu.Reset();
    mmu.Reset();
    timer.Reset();
}

// Todo: Frame calling v-blank 195-196x per frame??
std::pair<sf::Image, bool> GameBoy::RenderFrame(sf::RenderWindow& window) {
	cpu.frame_clock = cpu.clock + 17556; // Number of cycles/4 for one frame before v-blank
    bool running = true;
	do {
        if(!input.PollEvents(window)) running = false;
        if (cpu.halt) {
            cpu.m_clock = 1;
        } else {
            cpu.ExecuteNextInstruction();
        }
        cpu.clock += cpu.m_clock;
        cpu.m_clock = 0;

        uint8_t if_memory_value = mmu.ReadByte(0xFF0F);
        if (mmu.interrupt_enable and cpu.interrupt_master_enable and if_memory_value) {
			cpu.halt = 0;
			cpu.interrupt_master_enable = 0;
			uint8_t interrupt_fired = mmu.interrupt_enable & if_memory_value;

            if (interrupt_fired & 0x01) {if_memory_value &= 0XFE; cpu.RST40();}
			else if (interrupt_fired & 0x02) {if_memory_value &= 0XFD; cpu.RST48();}
			else if (interrupt_fired & 0x04) {if_memory_value &= 0XFB; cpu.RST50();}
			else if (interrupt_fired & 0x08) {if_memory_value &= 0XF7; cpu.RST58();}
			else if (interrupt_fired & 0x10) {if_memory_value &= 0XEF; cpu.RST60();}
			else {cpu.interrupt_master_enable = 1;}
			
			mmu.WriteByte(0xFF0F, if_memory_value);
			//cpu.interrupt_master_enable = 1; // Not sure if Halt only temporarily enables interrupts
		}
		
		cpu.clock += cpu.m_clock;
        cpu.m_clock = 0;
		
		timer.Increment();
	} while(cpu.clock < cpu.frame_clock);

	
	return std::make_pair(display.RenderFrame(), running);
}
