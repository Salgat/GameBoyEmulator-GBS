//
// Created by Austin on 6/11/2015.
//

#include "GameBoy.hpp"

#include <iostream>

GameBoy::GameBoy() {
    cpu.Initialize(&mmu);
    mmu.Initialize(&cpu);
    display.Initialize(&cpu, &mmu);
    timer.Initialize(&cpu, &mmu);
	
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

sf::Image GameBoy::RenderFrame() {
	cpu.frame_clock = cpu.clock + 17556; // Number of cycles/4 for one frame before v-blank
	do {
		if (cpu.halt) {
			cpu.m_clock = 1;
		} else {
			//std::cout << "Executing next instruction" << std::endl;
			cpu.ExecuteNextInstruction();
			//std::cout << "Finished executing instruction" << std::endl;
		}



		uint8_t if_memory_value = mmu.ReadByte(0xFF0F);
		if (mmu.interrupt_enable and cpu.interrupt_master_enable and if_memory_value) {
			cpu.halt = 0;
			cpu.interrupt_master_enable = 0;
			uint8_t interrupt_fired = mmu.interrupt_enable & if_memory_value;

            if (interrupt_fired & 1) {if_memory_value &= 0XFE; cpu.RST40();}
			else if (interrupt_fired & 2) {if_memory_value &= 0XFD; cpu.RST48();}
			else if (interrupt_fired & 4) {if_memory_value &= 0XFB; cpu.RST50();}
			else if (interrupt_fired & 8) {if_memory_value &= 0XF7; cpu.RST58();}
			else if (interrupt_fired & 16) {if_memory_value &= 0XEF; cpu.RST60();}
			else {cpu.interrupt_master_enable = 1;}
			
			mmu.WriteByte(0xFF0F, if_memory_value);
		}
		
		cpu.clock += cpu.m_clock;
        timer.Increment();
        cpu.m_clock = 0;
	} while(cpu.clock < cpu.frame_clock);

    //auto frame = sf::Image();
    //frame.create(160, 144);
	//return frame;
	
	return display.RenderFrame();
}