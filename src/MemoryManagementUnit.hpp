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
    unsigned int rom_bank = 1; // Current bank selected
    uint8_t number_rom_banks = 1; // Number of 16KB (0x4000) banks available
    unsigned int rom_offset = 0x4000; // Depending on memory bank selected, determines where in cartridge rom to read for ROM bank 1

    uint8_t ram_bank = 0;
    uint8_t number_ram_banks = 0;
    bool ram_enabled = false;
    unsigned int ram_offset = 0x0000;
    uint8_t mode = 0;

    // Controller Type
    bool mbc1 = false;
    bool mbc1_banking_mode = false; // False == ROM banking mode, True == RAM banking mode
    bool mbc2 = false;
    bool mmm01 = false;
    bool mbc3 = false;
    uint8_t latch_data = 0;
    bool mbc5 = false;
    bool mbc7 = false;
    bool tama5 = false;
    bool huc3 = false;
    bool huc1 = false;
    bool sram = false;
    bool battery = false;
    bool timer = false;
    bool rumble = false;
    bool camera = false;

    struct RTC {
        uint8_t seconds = 0; // 0-59
        uint8_t minutes = 0; // 0-59
        uint8_t hours = 0;   // 0-23
        uint8_t day_lower = 0; // lower 8 bits of day (0-255)
        uint8_t day_upper = 0; // higher 1 bit of day, carry bit, halt flag
                               // Bit 0: day counter MSB
                               // Bit 6: Halt (0=active, 1=stop timer)
                               // Bit 7: Day Counter Carry Bit (1=overflow)
    } rtc;
};

class Processor;
class Input;
class Display;
class Timer;

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
    std::vector<uint8_t> hram; // High RAM from 0xFF80 to 0xFFFF
    uint8_t interrupt_enable; // IE register, which allows interrupts if enabled (1)
	uint8_t interrupt_flag; // IF register

    bool bios_mode;
    uint8_t cartridge_type; // Determines which memory bank controller is used
    uint8_t ram_size; // 0: None, 1: 1 bank @ 2KB, 2: 1 bank @ 8KB, 3: 4 banks @ 32KB total
    MemoryBankController mbc;
    bool updateSaveFile;
	
	std::string game_title;

    MemoryManagementUnit();

    void Initialize(Processor* cpu_, Input* input_, Display* display_, Timer* timer_);
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
	Display* display;
	Timer* timer;

    void TransferToOAM(uint16_t origin);
};

#endif //GAMEBOYEMULATOR_MEMORYMANAGEMENTUNIT_HPP
