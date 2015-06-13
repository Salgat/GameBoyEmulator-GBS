//
// Created by Austin on 6/11/2015.
//

#include "Processor.hpp"

Processor::Processor() {
    Reset();
}

void Processor::Initialize(MemoryManagementUnit* mmu_) {
    mmu = mmu_;
}

void Processor::Reset() {
    AF.higher = 0x01; // 0x01 = Normal Gameboy
    AF.lower = 0xB0; // Flag Register, default to this value
    BC.word = 0x0013;
    DE.word = 0x00D8;
    HL.word = 0x014D;
    stack_pointer.word = 0xFFFE;
    program_counter.word = 0x100;
}