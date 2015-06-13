//
// Created by Austin on 6/11/2015.
//

#ifndef GAMEBOYEMULATOR_DISPLAY_HPP
#define GAMEBOYEMULATOR_DISPLAY_HPP

class Processor;
class MemoryManagementUnit;

/**
 * Handles rendering to the "screen".
 */
class Display {
public:
    Display();

    void Initialize(Processor* cpu_, MemoryManagementUnit* mmu_);
    void Reset();

private:
    Processor* cpu;
    MemoryManagementUnit* mmu;

};

#endif //GAMEBOYEMULATOR_DISPLAY_HPP
