//
// Created by Austin on 6/11/2015.
//

#ifndef GAMEBOYEMULATOR_MEMORYMANAGEMENTUNIT_HPP
#define GAMEBOYEMULATOR_MEMORYMANAGEMENTUNIT_HPP

#include <stdint.h>
#include <vector>
#include <array>
#include <string>

struct MemoryBankController {
    uint8_t rom_bank; // Current bank selected
    uint8_t ram_bank;
    bool ram_enabled;
    uint8_t mode;
};

class Processor;
class Input;

class MemoryManagementUnit {
public:
    std::array<uint8_t, 0x0100> bios;
    std::vector<uint8_t> rom;
    std::vector<uint8_t> cartridge_rom;
    std::vector<uint8_t> vram; // Graphics Memory
    std::vector<uint8_t> eram; // External RAM
    std::vector<uint8_t> wram; // Working RAM (internal 8K RAM to Gameboy)
    std::vector<uint8_t> oam; // Object Attribute Memory (for sprites)
    std::vector<uint8_t> zram; // Zero-Page RAM (ZRAM or sometimes called HRAM)
    uint8_t interrupt_enable; // IE register, which allows interrupts if enabled (1)
	uint8_t interrupt_flag; // IF register

    bool bios_mode;
    uint8_t cartridge_type; // Determines which memory bank controller is used
    uint8_t rom_size; // Number of 16KB (0x4000) banks available
    uint16_t rom_offset; // Depending on memory bank selected, determines where in cartridge rom to read for ROM bank 1
    uint8_t ram_size; // 0: None, 1: 1 bank @ 2KB, 2: 1 bank @ 8KB, 3: 4 banks @ 32KB total
    uint16_t ram_offset;
    MemoryBankController mbc1;

    MemoryManagementUnit();

    void Initialize(Processor* cpu_, Input* input_);
    void Reset();
    void LoadRom(std::string rom_name);

    // Memory Access
    uint8_t ReadByte(uint16_t address);
    uint16_t ReadWord(uint16_t address);
    void WriteByte(uint16_t address, uint8_t value);
    void WriteWord(uint16_t address, uint16_t value);

private:
    Processor* cpu;
    Input* input;

};

#endif //GAMEBOYEMULATOR_MEMORYMANAGEMENTUNIT_HPP
