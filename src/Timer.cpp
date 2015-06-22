//
// Created by Austin on 6/19/2015.
//

#include "Timer.hpp"
#include "Processor.hpp"
#include "MemoryManagementUnit.hpp"

#include <iostream>

Timer::Timer() {
    Reset();
}

void Timer::Initialize(Processor* cpu_, MemoryManagementUnit* mmu_) {
    cpu = cpu_;
    mmu = mmu_;
}

void Timer::Reset() {
    divider_clock = 0;
    divider_clock_tracker = 0;
    counter_clock = 0;
    counter_clock_tracker = 0;
    scanline_clock = 0;
    scanline_clock_tracker = 0;
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
        if (mmu->ReadByte(0xFF07)&0x03 == 0x0) {
            counter_increment_count = 256;
        } else if (mmu->ReadByte(0xFF07)&0x03 == 0x1) {
            counter_increment_count = 4;
        } else if (mmu->ReadByte(0xFF07)&0x03 == 0x2) {
            counter_increment_count = 16;
        } else {
            counter_increment_count = 64;
        }

        counter_clock_tracker += cycles;
        if (counter_clock_tracker >= counter_increment_count) {
            if (counter_clock == 0xFF) {
                // Overflow to occur, set interrupt flag
                mmu->interrupt_flag |= 0x04;
            }
            ++counter_clock;
            counter_clock_tracker -= counter_increment_count;
        }
    }

    // Also keep track of scanline (0xFF44)
    scanline_clock_tracker += cycles;
    if (scanline_clock_tracker >= 456) {
        ++scanline_clock;
        scanline_clock_tracker -= 456;
    }
    if (scanline_clock > 153) {
        scanline_clock -= 153;
    }

    // Update LCD Status based on scanline and timing
    uint8_t lcd_status = mmu->zram[0xFF41&0xFF] & 0xF8; // Last 3 bits are updated here
    if (scanline_clock >= 144) {
        // V-Blank
        lcd_status |= 0b01;
        mmu->interrupt_flag |= 0x01;
    } else if (scanline_clock_tracker > 80+172 ) {
        // H-Blank
        lcd_status |= 0b00;
    } else if (scanline_clock_tracker > 80) {
        // Scanline accessing VRAM
        lcd_status |= 0b11;
    } else {
        // Scanline accessing OAM
        lcd_status |= 0b10;
    }
    // Finally, check if 0xFF45 matches scanline_clock and set bit 2 of 0xFF41 and possibly trigger interrupt
    uint8_t ly_compare = mmu->zram[0xFF45&0xFF];
    if (ly_compare == scanline_clock) {
        lcd_status |= 0b100;
        mmu->interrupt_flag |= 0x02;
    }
    // TODO: Trigger interrupt?

    // Update timers
    //mmu->WriteByte(0xFF04, divider_clock);
    //mmu->WriteByte(0xFF05, counter_clock);
    mmu->zram[0x04] = divider_clock;
    mmu->zram[0x05] = counter_clock;
    mmu->zram[0xFF41&0xFF] = lcd_status;
    mmu->zram[0xFF44&0xFF] = scanline_clock;

    clock = cpu->clock;
}