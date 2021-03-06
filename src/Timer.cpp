//
// Created by Austin on 6/19/2015.
//

#include "Timer.hpp"
#include "Processor.hpp"
#include "MemoryManagementUnit.hpp"
#include "Display.hpp"

#include <iostream>

Timer::Timer() {
    Reset();
}

void Timer::Initialize(Processor* cpu_, MemoryManagementUnit* mmu_, Display* display_) {
    cpu = cpu_;
    mmu = mmu_;
    display = display_;
}

void Timer::Reset() {
	clock = 0;
    divider_clock = 0xAB;
    divider_clock_tracker = 0;
    counter_clock = 0;
    counter_clock_tracker = 0;
    scanline = 0x00;
    scanline_tracker = 0;
	
	v_blank_triggered = false;
}

void Timer::Increment() {
    auto cycles = cpu->clock - clock; // Difference in clocks
	
    divider_clock_tracker += cycles;
    if (divider_clock_tracker >= 64) {
        ++divider_clock;
        divider_clock_tracker -= 64;
    }

    // If counter is active, increment based on clock select
    if (mmu->ReadByte(0xFF07)&0x04) {
        unsigned int counter_increment_count;
        if ((mmu->ReadByte(0xFF07)&0x03) == 0x0) {
            counter_increment_count = 256;
        } else if ((mmu->ReadByte(0xFF07)&0x03) == 0x1) {
            counter_increment_count = 4;
        } else if ((mmu->ReadByte(0xFF07)&0x03) == 0x2) {
            counter_increment_count = 16;
        } else {
            counter_increment_count = 64;
        }

        counter_clock_tracker += cycles;
        if (counter_clock_tracker > counter_increment_count) {
            if (counter_clock == 0xFF) {
                // Overflow to occur, set interrupt flag
                mmu->interrupt_flag |= 0x04;
                counter_clock = mmu->ReadByte(0xFF06);
            } else {
                ++counter_clock;
            }

            counter_clock_tracker -= counter_increment_count;
        }
    }

    // Also keep track of scanline (0xFF44)
    uint8_t lcd_control = mmu->zram[0xFF40&0xFF];
    if (lcd_control & 0x80) {
        // Only update scanline if lcd is enabled
        scanline_tracker += cycles;
        if (scanline_tracker >= 456/4) { // Check if this is >=  or just >
            ++scanline;
            scanline_tracker -= 456/4;
            if (scanline < 144) {
                display->RenderScanline(scanline);
            } else if (scanline > 153) {
                display->RenderScanline(0);
            }
        }
        if (scanline > 153) {
            scanline = 0;
            v_blank_triggered = false;
        }
    } else {
		scanline = 0;
		scanline_tracker = 0;
	}

    // Update LCD Status based on scanline and timing	
    uint8_t lcd_status = mmu->zram[0xFF41&0xFF] & 0xF8; // Last 3 bits are updated here
    if (scanline >= 144) {
        // V-Blank
        lcd_status |= 0b01;
		if (!v_blank_triggered) {
			mmu->interrupt_flag |= 0x01;
			v_blank_triggered = true;
		}
    } else if (scanline_tracker > (80+172)/4 ) {
        // H-Blank
        lcd_status |= 0b00;
    } else if (scanline_tracker > 80/4) {
        // Scanline accessing VRAM
        lcd_status |= 0b11;
    } else {
        // Scanline accessing OAM
        lcd_status |= 0b10;
    }
    // Finally, check if 0xFF45 matches scanline and set bit 2 of 0xFF41 and possibly trigger interrupt
    uint8_t ly_compare = mmu->zram[0xFF45&0xFF];
    if (ly_compare == scanline) {
        lcd_status |= 0b100;
        mmu->interrupt_flag |= 0x02;
    }

    // Update timers
    //mmu->zram[0x04] = divider_clock;
    //mmu->zram[0x05] = counter_clock;
    mmu->zram[0xFF41&0xFF] = lcd_status;
    //mmu->zram[0xFF44&0xFF] = scanline;

    clock = cpu->clock;

    //std::cout << "PC: " << std::hex << static_cast<unsigned int>(cpu->program_counter.word) << ", CYCLES: " << static_cast<unsigned int>(cycles) << ": \tDIV - " << static_cast<unsigned int>(divider_clock) << ", \tDIV Tracker - " << static_cast<unsigned int>(divider_clock_tracker) << ", \tCNT - " << static_cast<unsigned int>(counter_clock) << ", \tCNT Tracker - " << static_cast<unsigned int>(counter_clock_tracker) << std::endl;
}