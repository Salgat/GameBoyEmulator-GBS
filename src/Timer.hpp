//
// Created by Austin on 6/19/2015.
//

#ifndef GAMEBOYEMULATOR_TIMER_HPP
#define GAMEBOYEMULATOR_TIMER_HPP

#include <inttypes.h>

class Processor;
class MemoryManagementUnit;
class Display;

class Timer {
public:
    uint64_t clock;
    uint64_t divider_clock_tracker;
    uint8_t divider_clock; // 0xFF04: 16,384Hz (every 64 m_clock cycles)
    uint64_t counter_clock_tracker;
    uint8_t counter_clock; // 0xFF05: Controlled by 0xFF07
                           // 0xFF06: When counter_clock overflows, load it with this value
                           // 0xFF07: Counter Control
                           //   - Bit 2: 0 = stop, 1 = start
                           //   - Bits 0-1:
                           //       - 00: 4096Hz
                           //       - 01: 262144Hz
                           //       - 10: 65536Hz
                           //       - 11: 16384Hz
    uint8_t scanline;
    uint64_t scanline_tracker;

    Timer();

    void Initialize(Processor* cpu_, MemoryManagementUnit* mmu_, Display* display_);
    void Reset();
    void Increment();

private:
    Processor* cpu;
    MemoryManagementUnit* mmu;
    Display* display;

	bool v_blank_triggered;
};

#endif //GAMEBOYEMULATOR_TIMER_HPP
