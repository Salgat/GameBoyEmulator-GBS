//
// Created by Austin on 6/11/2015.
//
#include "MemoryManagementUnit.hpp"
#include "Input.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

#include "Processor.hpp"
#include "Timer.hpp"

/**
 * Initialize memory
 */
MemoryManagementUnit::MemoryManagementUnit() {
    rom = std::vector<uint8_t>(0x10000, 0);
    cartridge_rom = std::vector<uint8_t>(0x200000, 0);
    vram = std::vector<uint8_t>(0x2000, 0);
    eram = std::vector<uint8_t>(0x20000, 0); // MBC5 allows up to 128KB of external RAM
    wram = std::vector<uint8_t>(0x2000, 0);
    oam = std::vector<uint8_t>(0x100, 0);
    zram = std::vector<uint8_t>(0x100, 0);
    hram = std::vector<uint8_t>(0x100, 0);

    Reset();
}

/**
 * Loads cartridge rom into rom array
 */
void MemoryManagementUnit::LoadRom(std::string rom_name) {
    // Load Gameboy ROM
    std::string rom_location = "../../rom/" + rom_name;
    std::ifstream input(rom_location, std::ios::in | std::ios::binary);
    char byte;
    std::size_t index = 0;
    while(input.get(byte)) {
        cartridge_rom[index++] = static_cast<uint8_t>(byte);
    }

    // Copy over relevant cartridge memory to rom read-only section
    for (index = 0; index < 0x8000; ++index) {
        rom[index] = cartridge_rom[index];
    }

    Reset();
}

void MemoryManagementUnit::Initialize(Processor* cpu_, Input* input_, Display* display_, Timer* timer_) {
    cpu = cpu_;
    input = input_;
	display = display_;
	timer = timer_;
}

void MemoryManagementUnit::Reset() {
    bios_mode = false;//true;
    bios = {0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, // 16/row (0-15)
            0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, // 16-31
            0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B, // 32-47
            0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9, // 48-63
            0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, // 64
            0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
            0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
            0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
            0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
            0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
            0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
            0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
            0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
            0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
            0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
            0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50};

    // Memory is documented to reset to these values
    zram[0xFF00&0xFF] = 0x00; // Reset Joystick to all unpressed
    zram[0xFF05&0xFF] = 0x00;
    zram[0xFF06&0xFF] = 0x00;
    zram[0xFF07&0xFF] = 0x00;
    zram[0xFF10&0xFF] = 0x80;
    zram[0xFF11&0xFF] = 0xBF;
    zram[0xFF12&0xFF] = 0xF3;
    zram[0xFF14&0xFF] = 0xBF;
    zram[0xFF16&0xFF] = 0x3F;
    zram[0xFF17&0xFF] = 0x00;
    zram[0xFF19&0xFF] = 0xBF;
    zram[0xFF1A&0xFF] = 0x7F;
    zram[0xFF1B&0xFF] = 0xFF;
    zram[0xFF1C&0xFF] = 0x9F;
    zram[0xFF1E&0xFF] = 0xBF;
    zram[0xFF20&0xFF] = 0xFF;
    zram[0xFF21&0xFF] = 0x00;
    zram[0xFF22&0xFF] = 0x00;
    zram[0xFF23&0xFF] = 0xBF;
    zram[0xFF24&0xFF] = 0x77;
    zram[0xFF25&0xFF] = 0xF3;
    zram[0xFF26&0xFF] = 0xF1;
    zram[0xFF40&0xFF] = 0x91;
    zram[0xFF41&0xFF] = 0x85;
    zram[0xFF42&0xFF] = 0x00;
    zram[0xFF43&0xFF] = 0x00;
    zram[0xFF45&0xFF] = 0x00;
    zram[0xFF47&0xFF] = 0xFC;
    zram[0xFF48&0xFF] = 0xFF;
    zram[0xFF49&0xFF] = 0xFF;
    zram[0xFF4A&0xFF] = 0x00;
    zram[0xFF4B&0xFF] = 0x00;
    hram[0xFFFF&0xFF] = 0x00; interrupt_enable = 0x00; // the two are equivalent
	interrupt_flag = 0xE1; // 0xFF0F

    // Setup ROM banks and RAM


    // Setup controller
    mbc = MemoryBankController();

    // Setup configuration of controller type
    cartridge_type = cartridge_rom[0x0147];
    std::cout << "Cartridge type: " << std::hex << static_cast<unsigned int>(cartridge_type) << std::endl;
    switch (cartridge_type) {
        case 0x00:
            // ROM only (no bank switching)
            break;

        case 0x01:
            mbc.mbc1 = true;
            break;

        case 0x02:
            mbc.mbc1 = true;
            mbc.sram = true;
            break;

        case 0x03:
            std::cout << "MBC1 + SRAM + BATTERY" << std::endl;
            mbc.mbc1 = true;
            mbc.sram = true;
            mbc.battery = true;
            break;

        case 0x05:
            mbc.mbc2 = true;
            break;

        case 0x06:
            mbc.mbc2 = true;
            mbc.battery = true;
            break;

        case 0x08:
            mbc.sram = true;
            break;

        case 0x09:
            mbc.sram = true;
            mbc.battery = true;
            break;

        case 0x0B:
            mbc.mmm01 = true;
            break;

        case 0x0C:
            mbc.mmm01 = true;
            mbc.sram = true;
            break;

        case 0x0D:
            mbc.mmm01 = true;
            mbc.sram = true;
            mbc.battery = true;
            break;

        case 0x0F:
            mbc.mbc3 = true;
            mbc.timer = true;
            mbc.battery = true;
            break;

        case 0x10:
            mbc.mbc3 = true;
            mbc.timer = true;
            mbc.battery = true;
            mbc.sram = true;
            break;

        case 0x11:
            mbc.mbc3 = true;
            break;

        case 0x12:
            mbc.mbc3 = true;
            mbc.sram = true;
            break;

        case 0x13:
            mbc.mbc3 = true;
            mbc.sram = true;
            mbc.battery = true;
            break;

        case 0x19:
            mbc.mbc5 = true;
            break;

        case 0x1A:
            mbc.mbc5 = true;
            mbc.sram =  true;
            break;

        case 0x1B:
            mbc.mbc5 = true;
            mbc.sram = true;
            mbc.battery = true;
            break;

        case 0x1C:
            mbc.rumble = true;
            break;

        case 0x1D:
            mbc.rumble = true;
            mbc.sram = true;
            break;

        case 0x1E:
            mbc.rumble = true;
            mbc.sram = true;
            mbc.battery = true;
            break;

        case 0x1F:
            mbc.camera = true;
            break;

        case 0x22:
            mbc.mbc7 = true;
            mbc.sram = true;
            mbc.battery = true;
            break;

        case 0xFD:
            mbc.tama5 = true;
            break;

        case 0xFE:
            mbc.huc3 = true;
            break;

        case 0xFF:
            mbc.huc1 = true;
            break;
    }

    // Number of 16KB (0x4000) banks available
    switch (cartridge_rom[0x0148]) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            mbc.number_rom_banks = std::pow(2, cartridge_rom[0x0148]+1);
            break;

        case 0x52:
            mbc.number_rom_banks = 72;
            break;

        case 0x53:
            mbc.number_rom_banks = 80;
            break;

        case 0x54:
            mbc.number_rom_banks = 96;
            break;
    }

    // Setup RAM
    if (mbc.mbc2) {
        mbc.number_ram_banks = 1; // Technically only 1/16th the size of a RAM bank
    } else if (mbc.mbc1 or mbc.rumble or mbc.mbc3 or mbc.huc3) {
        mbc.number_ram_banks = 4;
    } else if (mbc.mbc5) {
        mbc.number_ram_banks = 16;
    } else if (mbc.sram) {
        mbc.number_ram_banks = 1;
    }

    mbc.rom_offset = 0x4000;
    ram_size = cartridge_rom[0x0149]; // RAM type available

    mbc.rom_bank = 0;
    mbc.ram_bank = 0;
    mbc.ram_enabled = false;
    mbc.mode = 0;
}

/**
 * Returns byte read from provided address
 */
uint8_t MemoryManagementUnit::ReadByte(uint16_t address) {
    switch(address & 0xF000) {
        // ROM bank 0
        case 0x0000:
            if (bios_mode) {
                if (address < 0x0100) {
                    return bios[address];
                } else if (cpu->program_counter.word == 0x0100) {
                    bios_mode = false;
                } else {
                    return rom[address];
                }
            }

        case 0x1000:
        case 0x2000:
        case 0x3000:
            // ROM Bank 0 read
            return rom[address];

        // Rom banks 1 and higher
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return cartridge_rom[static_cast<unsigned int>(address&0x3FFF) + mbc.rom_offset];

        // VRAM
        case 0x8000:
        case 0x9000:
            return vram[address & 0x1FFF];

        // External RAM
        case 0xA000:
        case 0xB000:
            return eram[static_cast<unsigned int>(address & 0x1FFF) + mbc.ram_offset];

        // Working RAM and its' echo
        case 0xC000:
        case 0xD000:
        case 0xE000:
            return wram[address & 0x1FFF];

        // Remaining memory (including some of the echo)
        case 0xF000:
            switch(address & 0x0F00) {
                // Remaining Echo RAM
                case 0x000: case 0x100: case 0x200: case 0x300:
                case 0x400: case 0x500: case 0x600: case 0x700:
                case 0x800: case 0x900: case 0xA00: case 0xB00:
                case 0xC00: case 0xD00:
                    return wram[address & 0x1FFF];

                // OAM (Object Attribute Memory for Sprites)
                case 0xE00:
                    if ((address & 0xFF) <= 0x9F) {
                        //if (address == 0xFE02)
                            //std::cout << "Reading from FE02 for oam" << std::endl;
                        return oam[address & 0xFF];
                    } else {
                        // Outside range of OAM (empty)
                        return 0x00;
                    }

                // Zero-page RAM, Memory-mapped I/O, including sound, graphics, etc
                case 0xF00:
                    if (address == 0xFFFF) {
                        return interrupt_enable;
                    } else if (address > 0xFF7F) {
                        // Zero-Page RAM (ZRAM or sometimes called HRAM)
                        return hram[address & 0x7F];
                    } else {
                        // TODO: Finish this section (input, graphics registers, etc)
                        switch (address & 0xF0) {
                            case 0x00:
                                switch (address & 0xF) {
                                    // 0xFF00: Joystick
									case 0:
                                        input->UpdateInput();
                                        return input->ReadByte();
									
									// Timers
									case 4:
                                        return timer->divider_clock;

                                    case 5:
                                        return timer->counter_clock;

                                    case 6:
                                        return zram[address&0xFF];

                                    case 7:
										return zram[address&0xFF];
										
									case 15:
										return interrupt_flag;
										
									default: return 0x00;	
                                }
                        
							case 0x10: case 0x20: case 0x30:
								return 0;
								
							case 0x40: case 0x50: case 0x60: case 0x70:
								if (address == 0xFF44) {
									return timer->scanline;
								} else {
									return zram[address&0xFF];
								}
						}
                    }
            }
    }
}

/**
 * Reads and returns 2 bytes at address (lower) and address+1 (upper byte)
 */
uint16_t MemoryManagementUnit::ReadWord(uint16_t address) {
	return static_cast<uint16_t>(ReadByte(address)) + (static_cast<uint16_t>(ReadByte(address + 1)) << 8);
}

/**
 * Writes a single byte to memory.
 */
void MemoryManagementUnit::WriteByte(uint16_t address, uint8_t value) {
    switch(address & 0xF000) {
        // ROM and RAM configuration
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            if (mbc.mbc1) {
                if (address < 0x2000) {
                    // Enable RAM if == 0xXA, else disable
                    if ((value & 0x0F) == 0x0A) {
                        //std::cout << "RAM enabled" << std::endl;
                        mbc.ram_enabled = true;
                    } else {
                        //std::cout << "RAM disabled" << std::endl;
                        mbc.ram_enabled = false;
                    }
                } else if (address < 0x4000) {
                    // Select ROM bank's lower 5 bits
                    if (value == 0) {
                        mbc.rom_bank = 1;
                    } else {
                        // Rom bank's lower 5 bits are set to value's lower 5 bits
                        mbc.rom_bank = (mbc.rom_bank & 0x60) | (value & 0x1F);
                        if (mbc.rom_bank == 0x00 or mbc.rom_bank == 0x20 or mbc.rom_bank == 0x40 or mbc.rom_bank == 0x60) {
                            mbc.rom_bank += 1;
                        }
                        //std::cout << "Switched to ROM bank: " << std::hex << static_cast<unsigned int>(mbc.rom_bank) << std::endl;
                    }

                    mbc.rom_offset = mbc.rom_bank * 0x4000;
                } else if (address < 0x6000) {
                    // Set either upper 3 bits of ROM bank number (ROM banking mode) or RAM bank number (RAM banking mode)
                    if (mbc.mbc1_banking_mode == false) {
                        // ROM Banking mode
                        mbc.rom_bank = ((value & 0x03) << 5) | (mbc.rom_bank & 0x1F);
                        if (mbc.rom_bank == 0x00 or mbc.rom_bank == 0x20 or mbc.rom_bank == 0x40 or mbc.rom_bank == 0x60) {
                            mbc.rom_bank += 1;
                        }
                        //std::cout << "Switched to ROM bank: " << std::hex << static_cast<unsigned int>(mbc.rom_bank) << std::endl;
                        mbc.rom_offset = mbc.rom_bank * 0x4000;
                    } else {
                        // RAM Banking mode
                        mbc.ram_bank = value & 0x03;
                        //std::cout << "Switched to RAM bank: " << std::hex << static_cast<unsigned int>(mbc.ram_bank) << std::endl;
                        mbc.ram_offset = mbc.ram_bank * 0x2000; // Each RAM bank has a maximum size of 8KB
                    }
                } else {
                    // Select either ROM and RAM banking mode for MBC1
                    if (value == 0) {
                        // ROM Banking mode
                        mbc.mbc1_banking_mode = false;
                    } else {
                        // RAM Banking mode
                        mbc.mbc1_banking_mode = true;
                    }
                }
            } else if (mbc.mbc2) {
                if (address < 0x1000) {

                } else if (address >= 0x2100 and address < 0x2200) {

                } else {
                    // Ignore write
                }
            }
            break;

        // VRAM
        case 0x8000:
        case 0x9000:
            vram[address & 0x1FFF] = value;
            break;

        // External RAM
        case 0xA000:
        case 0xB000:
            eram[mbc.ram_offset + (address & 0x1FFF)] = value;
            break;

        // Work RAM and echo
        case 0xC000:
        case 0xD000:
        case 0xE000:
            wram[address & 0x1FFF] = value;
            break;

        // Remaining memory (including some of the echo)
        case 0xF000:
            switch(address & 0x0F00) {
                // Echo RAM
                case 0x000: case 0x100: case 0x200: case 0x300:
                case 0x400: case 0x500: case 0x600: case 0x700:
                case 0x800: case 0x900: case 0xA00: case 0xB00:
                case 0xC00: case 0xD00:
                    wram[address & 0x1FFF] = value;
                    break;

                // OAM (Object Attribute Memory for Sprites)
                case 0xE00:
                    if ((address & 0xFF) < 0xA0) {
                        //if (address == 0xFE02)
                            //std::cout << "Writing to FE02 for oam: " << std::hex << static_cast<unsigned int>(value) << std::endl;
                        oam[address & 0xFF] = value;
                    }
                    break;

                // Zero-page RAM, Memory-mapped I/O, including sound, graphics, etc
                case 0xF00:
                    if (address == 0xFFFF) {
                        interrupt_enable = value;
                    } else if (address > 0xFF7F) {
                        hram[address & 0x7F] = value;
                        // TODO: There is a bug here where there is some overlap in writing to zram, need to seperate these two
                        //if ((address & 0x7F) != 0x47)
                        //    zram[address & 0x7F] = value;
                    } else {
                        switch(address & 0xF0) {
                            case 0x00:
                                switch(address & 0xF) {
                                    case 0:
                                        input->WriteByte(value);
                                        break;

                                    case 1:
                                        //std::cout << "Writing value at address: " << std::hex << static_cast<unsigned int>(zram[address&0x01]) << ", " << static_cast<unsigned int>(address) << std::endl;
                                        zram[address&0xFF] = value;
                                        break;

                                    case 2:
                                        if (value & 0x80) {
                                            //std::cout << zram[0x01];
                                        }
                                        break;

                                    case 4:
                                        timer->divider_clock = 0;
                                        break;

                                    case 5:
                                        timer->counter_clock = value;

                                    case 6: case 7:
                                        zram[address&0xFF] = value;
                                        break;
										
									case 15:
										interrupt_flag = value;
                                        break;
                                }
								break;
								
							case 0x10: case 0x20: case 0x30:
								break;
								
							case 0x40: case 0x50: case 0x60: case 0x70:
                                if (address == 0xFF46) {
                                    // DMA Transfer (when this is written to, the value is used as the upper byte of
                                    // the address XX00-XX9F that is copied to FE00-FE9F
                                    uint16_t origin = static_cast<uint16_t>(value) << 8;
                                    TransferToOAM(origin);
								} else if (address == 0xFF44) {
									timer->scanline = 0;
                                } else {
                                    zram[address&0xFF] = value;
                                }
                                break;
                        }
                    }
            }
    }
}

/**
 * Writes a word (2 bytes) to memory.
 */
void MemoryManagementUnit::WriteWord(uint16_t address, uint16_t value) {
    WriteByte(address, static_cast<uint8_t>(value & 0xFF));
    WriteByte(address+1, static_cast<uint8_t>(value >> 8)); // Higher byte in next address location
}

/**
 * Transfers from origin->origin+9F to FE00->FE9F.
 */
void MemoryManagementUnit::TransferToOAM(uint16_t origin) {
    for (uint16_t offset = 0; offset < 0xA0; ++offset) {
        uint8_t value = ReadByte(origin+offset);
        oam[offset] = value;
    }
}