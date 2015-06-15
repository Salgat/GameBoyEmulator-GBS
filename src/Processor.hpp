//
// Created by Austin on 6/11/2015.
//

#ifndef GAMEBOYEMULATOR_PROCESSOR_HPP
#define GAMEBOYEMULATOR_PROCESSOR_HPP

#include <inttypes.h>

class MemoryManagementUnit;

/**
 * 16 bit register whose 8 bit contents (upper and lower) can be accessed individually
 */
union Register {
    uint16_t word;
    struct {
        uint8_t lower;
        uint8_t higher;
    };
};

class Processor {
public:
    // Processor Registers
    Register AF; // A = higher, F = lower
    Register BC; // etc
    Register DE;
    Register HL;
    Register stack_pointer;
    Register program_counter;
	
	uint8_t m_clock; // Tracks cycles/4 passed for an instruction

    Processor();

    void Initialize(MemoryManagementUnit* mmu_);
    void Reset();
    void ExecuteOpCode(uint8_t opcode);


private:
    MemoryManagementUnit* mmu;
	
	// Long list of opcodes
	
	
	
};

#endif //GAMEBOYEMULATOR_PROCESSOR_HPP

// TODO: Create unit tests for opcodes, where registers are loaded with values, opcode performed, 
//		 then tests are made to verify values in memory or registers or flags.
/* Load Section */
// Load register from other register: register = register
void LDrr_bb() {BC.higher = BC.higher; m_clock = 1;}
void LDrr_bc() {BC.higher = BC.lower;  m_clock = 1;}
void LDrr_bd() {BC.higher = DE.higher; m_clock = 1;}
void LDrr_be() {BC.higher = DE.lower;  m_clock = 1;}
void LDrr_bh() {BC.higher = HL.higher; m_clock = 1;}
void LDrr_bl() {BC.higher = HL.lower;  m_clock = 1;}
void LDrr_ba() {BC.higher = AF.higher; m_clock = 1;}
void LDrr_cb() {BC.lower  = BC.higher; m_clock = 1;}
void LDrr_cc() {BC.lower  = BC.lower;  m_clock = 1;}
void LDrr_cd() {BC.lower  = DE.higher; m_clock = 1;}
void LDrr_ce() {BC.lower  = DE.lower;  m_clock = 1;}
void LDrr_ch() {BC.lower  = HL.higher; m_clock = 1;}
void LDrr_cl() {BC.lower  = HL.lower;  m_clock = 1;}
void LDrr_ca() {BC.lower  = AF.higher; m_clock = 1;}
void LDrr_db() {DE.higher = BC.higher; m_clock = 1;}
void LDrr_dc() {DE.higher = BC.lower;  m_clock = 1;}
void LDrr_dd() {DE.higher = DE.higher; m_clock = 1;}
void LDrr_de() {DE.higher = DE.lower;  m_clock = 1;}
void LDrr_dh() {DE.higher = HL.higher; m_clock = 1;}
void LDrr_dl() {DE.higher = HL.lower;  m_clock = 1;}
void LDrr_da() {DE.higher = AF.higher; m_clock = 1;}
void LDrr_eb() {DE.lower  = BC.higher; m_clock = 1;}
void LDrr_ec() {DE.lower  = BC.lower;  m_clock = 1;}
void LDrr_ed() {DE.lower  = DE.higher; m_clock = 1;}
void LDrr_ee() {DE.lower  = DE.lower;  m_clock = 1;}
void LDrr_eh() {DE.lower  = HL.higher; m_clock = 1;}
void LDrr_el() {DE.lower  = HL.lower;  m_clock = 1;}
void LDrr_ea() {DE.lower  = AF.higher; m_clock = 1;}
void LDrr_hb() {HL.higher = BC.higher; m_clock = 1;}
void LDrr_hc() {HL.higher = BC.lower;  m_clock = 1;}
void LDrr_hd() {HL.higher = DE.higher; m_clock = 1;}
void LDrr_he() {HL.higher = DE.lower;  m_clock = 1;}
void LDrr_hh() {HL.higher = HL.higher; m_clock = 1;}
void LDrr_hl() {HL.higher = HL.lower;  m_clock = 1;}
void LDrr_ha() {HL.higher = AF.higher; m_clock = 1;}
void LDrr_lb() {HL.lower  = BC.higher; m_clock = 1;}
void LDrr_lc() {HL.lower  = BC.lower;  m_clock = 1;}
void LDrr_ld() {HL.lower  = DE.lower;  m_clock = 1;}
void LDrr_le() {HL.lower  = DE.higher; m_clock = 1;}
void LDrr_lh() {HL.lower  = HL.higher; m_clock = 1;}
void LDrr_ll() {HL.lower  = HL.lower;  m_clock = 1;}
void LDrr_la() {HL.lower  = AF.higher; m_clock = 1;}
void LDrr_ab() {AF.higher = BC.higher; m_clock = 1;}
void LDrr_ac() {AF.higher = BC.lower;  m_clock = 1;}
void LDrr_ad() {AF.higher = DE.higher; m_clock = 1;}
void LDrr_ae() {AF.higher = DE.lower;  m_clock = 1;}
void LDrr_ah() {AF.higher = HL.higher; m_clock = 1;}
void LDrr_al() {AF.higher = HL.lower;  m_clock = 1;}
void LDrr_aa() {AF.higher = AF.higher; m_clock = 1;}

// Load register from memory: register = *(HL)
void LDrHLm_b() {BC.higher = mmu->ReadByte(HL.word); m_clock = 2;}
void LDrHLm_c() {BC.lower  = mmu->ReadByte(HL.word); m_clock = 2;}
void LDrHLm_d() {DE.higher = mmu->ReadByte(HL.word); m_clock = 2;}
void LDrHLm_e() {DE.lower  = mmu->ReadByte(HL.word); m_clock = 2;}
void LDrHLm_h() {HL.higher = mmu->ReadByte(HL.word); m_clock = 2;}
void LDrHLm_l() {HL.lower  = mmu->ReadByte(HL.word); m_clock = 2;}
void LDrHLm_a() {AF.higher = mmu->ReadByte(HL.word); m_clock = 2;}

// Write memory from register: *(HL) = register
void LDHLmr_b() {mmu->WriteByte(HL.word, BC.higher); m_clock = 2;}
void LDHLmr_c() {mmu->WriteByte(HL.word, BC.lower);  m_clock = 2;}
void LDHLmr_d() {mmu->WriteByte(HL.word, DE.higher); m_clock = 2;}
void LDHLmr_e() {mmu->WriteByte(HL.word, DE.lower);  m_clock = 2;}
void LDHLmr_h() {mmu->WriteByte(HL.word, HL.higher); m_clock = 2;}
void LDHLmr_l() {mmu->WriteByte(HL.word, HL.lower);  m_clock = 2;}
void LDHLmr_a() {mmu->WriteByte(HL.word, AF.higher); m_clock = 2;}

// Load register with value: register = value (where value is at program counter)
void LDrn_b() {BC.higher = mmu->ReadByte(program_counter.word); ++program_counter; m_clock = 2;}
void LDrn_c() {BC.lower  = mmu->ReadByte(program_counter.word); ++program_counter; m_clock = 2;}
void LDrn_d() {DE.higher = mmu->ReadByte(program_counter.word); ++program_counter; m_clock = 2;}
void LDrn_e() {DE.lower  = mmu->ReadByte(program_counter.word); ++program_counter; m_clock = 2;}
void LDrn_h() {HL.higher = mmu->ReadByte(program_counter.word); ++program_counter; m_clock = 2;}
void LDrn_l() {HL.lower  = mmu->ReadByte(program_counter.word); ++program_counter; m_clock = 2;}
void LDrn_a() {AF.higher = mmu->ReadByte(program_counter.word); ++program_counter; m_clock = 2;}

void LDHLmn() {mmu->WriteByte(HL.word, mmu->ReadByte(program_counter.word)); ++program_counter.word; m_clock = 3;}

void LDBCmA() {mmu->WriteByte(BC.word, AF.higher); m_clock = 2;}
void LDDEmA() {mmu->WriteByte(DE.word, AF.higher}; m_clock = 2;}

void LDmmA() {mmu->WriteByte(mmu->ReadWord(program_counter.word), AF.higher); program_counter.word += 2; m_clock = 4;}

void LDABCm() {AF.higher = mmu->ReadByte(BC.word); m_clock = 2;}
void LDADEm() {AF.higher = mmu->ReadByte(DE.word); m_clock = 2;}

void LDAmm() {AF.higher = mmu->ReadByte(mmu->ReadWord(program_counter.word)); program_counter.word += 2; m_clock = 4;}

void LDBCnn() {BC.lower = mmu->ReadByte(program_counter.word); BC.higher = mmu->ReadByte(program_counter.word+1); program_counter.word += 2; m_clock = 3;}
void LDDEnn() {DE.lower = mmu->ReadByte(program_counter.word); DE.higher = mmu->ReadByte(program_counter.word+1); program_counter.word += 2; m_clock = 3;}
void LDHLnn() {HL.lower = mmu->ReadByte(program_counter.word); HL.higher = mmu->ReadByte(program_counter.word+1); program_counter.word += 2; m_clock = 3;}
void LDSPnn() {stack_pointer.lower = mmu->ReadByte(program_counter); stack_pointer.higher = mmu->ReadByte(program_counter+1); program_counter.word += 2; m_clock = 3;}

//void LDHLmm() {}
//void LDmmHL() {}

void LDHLIA() {mmu->WriteByte(HL.word, AF.higher); HL.word += 1; m_clock = 2;}
void LDAHLI() {AF.higher = mmu->ReadByte(HL.word); HL.word += 1; m_clock = 2;}

void LDHLDA() {mmu->WriteByte(HL.word, AF.higher); HL.word -= 1; m_clock = 2;}
void LDAHLD() {AF.higher = mmu->ReadByte(HL.word); HL.word -= 1; m_clock = 2;}

void LDAIOn() {AF.higher = mmu->ReadByte(0xFF00 + mmu->ReadByte(program_counter.word)); ++program_counter.word; m_clock = 3;}
void LDIOnA() {mmu->WriteByte(0xFF00 + mmu->ReadByte(program_counter.word), AF.higher); ++program_counter.word; m_clock = 3;}
void LDAIOC() {AF.higher = mmu->ReadByte(0xFF00 + BC.lower); m_clock = 2;}
void LDIOCA() {mmu->WriteByte(0xFF00 + BC.lower, AF.higher); m_clock = 2;}

void LDHLSPn() {HL.word = mmu->ReadWord(stack_pointer.word + mmu->ReadByte(program_counter.word)); ++program_counter.word; m_clock = 3;}

// Swap upper and lower nibbles of register (affects zero flag)
void SWAPr_b() {uint8_t upper_nibble = BC.higher & 0xF0; uint8_t lower_nibble = BC.higher & 0x0F; BC.higher = (lower_nibble << 4) | (higher_nibble >> 4); if(BC.higher) AF.lower = 0x80; m_clock = 1;}
void SWAPr_c() {uint8_t upper_nibble = BC.lower  & 0xF0; uint8_t lower_nibble = BC.lower  & 0x0F; BC.lower  = (lower_nibble << 4) | (higher_nibble >> 4); if(BC.lower)  AF.lower = 0x80; m_clock = 1;}
void SWAPr_d() {uint8_t upper_nibble = DE.higher & 0xF0; uint8_t lower_nibble = DE.higher & 0x0F; DE.higher = (lower_nibble << 4) | (higher_nibble >> 4); if(DE.higher) AF.lower = 0x80; m_clock = 1;}
void SWAPr_e() {uint8_t upper_nibble = DE.lower  & 0xF0; uint8_t lower_nibble = DE.lower  & 0x0F; DE.lower  = (lower_nibble << 4) | (higher_nibble >> 4); if(DE.lower)  AF.lower = 0x80; m_clock = 1;}
void SWAPr_h() {uint8_t upper_nibble = HL.higher & 0xF0; uint8_t lower_nibble = HL.higher & 0x0F; HL.higher = (lower_nibble << 4) | (higher_nibble >> 4); if(HL.higher) AF.lower = 0x80; m_clock = 1;}
void SWAPr_l() {uint8_t upper_nibble = HL.lower  & 0xF0; uint8_t lower_nibble = HL.lower  & 0x0F; HL.lower  = (lower_nibble << 4) | (higher_nibble >> 4); if(HL.lower)  AF.lower = 0x80; m_clock = 1;}
void SWAPr_a() {uint8_t upper_nibble = AF.higher & 0xF0; uint8_t lower_nibble = AF.higher & 0x0F; AF.higher = (lower_nibble << 4) | (higher_nibble >> 4); if(AF.higher) AF.lower = 0x80; m_clock = 1;}

// Data Manipulation
// Add to register or memory
void ADDr_b() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(BC.higher); AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + BC.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += BC.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void ADDr_c() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(BC.lower);  AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + BC.lower  & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += BC.lower;  if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void ADDr_d() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(DE.higher); AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + DE.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += DE.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void ADDr_e() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(DE.lower);  AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + DE.lower  & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += DE.lower;  if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void ADDr_h() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(HL.higher); AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + HL.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += HL.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void ADDr_l() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(HL.lower);  AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + HL.lower  & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += HL.lower;  if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void ADDr_a() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(AF.higher); AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + AF.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += AF.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void ADDHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint16_t result = static_cast<uint16_t>(AF.higher) + memory_value; AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + memory_value & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += memory_value; if(!AF.higher) AF.lower |= 0x80; m_clock = 2;}
void ADDn() {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; uint16_t result = static_cast<uint16_t>(AF.higher) + memory_value; AF.lower = (result > 0xFF)?0x10:0x00; if((AF.higher & 0xF + memory_value & 0xF) & 0x10) AF.lower |= 0x20; AF.higher += memory_value; if(!AF.higher) AF.lower |= 0x80; m_clock = 2;}
void ADDHLBC() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(BC.word); if(result > 0xFFFF) AF.lower |= 0x10; else AF.lower &= 0xEF; HL.word += BC.word; m_clock = 3;}
void ADDHLDE() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(DE.word); if(result > 0xFFFF) AF.lower |= 0x10; else AF.lower &= 0xEF; HL.word += DE.word; m_clock = 3;}
void ADDHLHL() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(HL.word); if(result > 0xFFFF) AF.lower |= 0x10; else AF.lower &= 0xEF; HL.word += HL.word; m_clock = 3;}
void ADDHLSP() {uint32_t result = static_cast<uint32_t>(stack_pointer.word) + static_cast<uint32_t>(HL.word); if(result > 0xFFFF) AF.lower |= 0x10; else AF.lower &= 0xEF; HL.word += stack_pointer.word; m_clock = 3;}
void ADDSPn() {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; uint32_t result = static_cast<uint32_t>(memory_value) + static_cast<uint32_t>(stack_pointer.word); if(result > 0xFFFF) AF.lower |= 0x10; else AF.lower &= 0xEF; HL.word += stack_pointer.word; m_clock = 4;}

// Add to register or memory then add carry bit
void ADCr_b() {uint16_t copy = AF.higher; AF.higher += BC.higher; AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+BC.higher>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + BC.higher & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 1;}
void ADCr_c() {uint16_t copy = AF.higher; AF.higher += BC.lower;  AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+BC.lower >0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + BC.lower  & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 1;}
void ADCr_d() {uint16_t copy = AF.higher; AF.higher += DE.higher; AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+DE.higher>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + DE.higher & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 1;}
void ADCr_e() {uint16_t copy = AF.higher; AF.higher += DE.lower;  AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+DE.lower >0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + DE.lower  & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 1;}
void ADCr_h() {uint16_t copy = AF.higher; AF.higher += HL.higher; AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+HL.higher>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + HL.higher & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 1;}
void ADCr_l() {uint16_t copy = AF.higher; AF.higher += HL.lower;  AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+HL.lower >0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + HL.lower  & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 1;}
void ADCr_a() {uint16_t copy = AF.higher; AF.higher += AF.higher; AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+AF.higher>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + AF.higher & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 1;}
void ADCHL() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(HL.word); AF.higher += memory_value; AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+memory_value>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + memory_value & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 2;}
void ADCn() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; AF.higher += memory_value; AF.higher += (AF.lower&0x10}?1:0; AF.lower = (copy+memory_value>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if ((copy & 0xF + memory_value & 0xF) & 0x10) AF.lower |= 0x20; m_clock = 2;}

// Subtract register or memory from A
void SUBr_b() {int result = static_cast<int>(AF.higher) - static_cast<int>(BC.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + BC.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= BC.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBr_c() {int result = static_cast<int>(AF.higher) - static_cast<int>(BC.lower);  AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + BC.lower  & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= BC.lower;  if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBr_d() {int result = static_cast<int>(AF.higher) - static_cast<int>(DE.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + DE.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= DE.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBr_e() {int result = static_cast<int>(AF.higher) - static_cast<int>(DE.lower);  AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + DE.lower  & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= DE.lower;  if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBr_h() {int result = static_cast<int>(AF.higher) - static_cast<int>(HL.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + HL.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= HL.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBr_l() {int result = static_cast<int>(AF.higher) - static_cast<int>(HL.lower);  AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + HL.lower  & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= HL.lower;  if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBr_a() {int result = static_cast<int>(AF.higher) - static_cast<int>(AF.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + AF.higher & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= AF.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBHL() {uint8_t memory_value = mmu->ReadByte(HL.word); int result = static_cast<int>(AF.higher) - static_cast<int>(memory_value); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + memory_value & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= memory_value; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void SUBn() {uint8_t memory_value = mmu->ReadByte(program_counter.word++); int result = static_cast<int>(AF.higher) - static_cast<int>(memory_value); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF + memory_value & 0xF) & 0x10) AF.lower |= 0x20; AF.higher -= memory_value; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}

// Subtract register or memory then carry (where carry represents under or overflow)
void SBCr_b() {uint16_t copy = AF.higher; AF.higher -= BC.higher; AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-BC.higher<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < BC.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void SBCr_c() {uint16_t copy = AF.higher; AF.higher -= BC.lower;  AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-BC.lower <0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < BC.lower  & 0xF) AF.lower |= 0x20; m_clock = 1;}
void SBCr_d() {uint16_t copy = AF.higher; AF.higher -= DE.higher; AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-DE.higher<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < DE.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void SBCr_e() {uint16_t copy = AF.higher; AF.higher -= DE.lower;  AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-DE.lower <0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < DE.lower  & 0xF) AF.lower |= 0x20; m_clock = 1;}
void SBCr_h() {uint16_t copy = AF.higher; AF.higher -= HL.higher; AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-HL.higher<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < HL.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void SBCr_l() {uint16_t copy = AF.higher; AF.higher -= HL.lower;  AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-HL.lower <0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < HL.lower  & 0xF) AF.lower |= 0x20; m_clock = 1;}
void SBCr_a() {uint16_t copy = AF.higher; AF.higher -= AF.higher; AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-AF.higher<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < AF.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void SBCHL() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(HL.word); AF.higher -= memory_value; AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-memory_value<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < memory_value & 0xF) AF.lower |= 0x20; m_clock = 2;}
void SBCn() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(program_counter.word++); AF.higher -= memory_value; AF.higher -= (AF.lower&0x10}?1:0; AF.lower = (copy-memory_value<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < memory_value & 0xF) AF.lower |= 0x20; m_clock = 2;}

// Same as subtraction but results are discarded
void CPr_b() {uint16_t copy = AF.higher; uint8_t result = AF.higher - BC.higher; AF.lower = (copy-BC.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < BC.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void CPr_c() {uint16_t copy = AF.higher; uint8_t result = AF.higher - BC.lower;  AF.lower = (copy-BC.lower <0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < BC.lower  & 0xF) AF.lower |= 0x20; m_clock = 1;}
void CPr_d() {uint16_t copy = AF.higher; uint8_t result = AF.higher - DE.higher; AF.lower = (copy-DE.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < DE.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void CPr_e() {uint16_t copy = AF.higher; uint8_t result = AF.higher - DE.lower;  AF.lower = (copy-DE.lower <0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < DE.lower  & 0xF) AF.lower |= 0x20; m_clock = 1;}
void CPr_h() {uint16_t copy = AF.higher; uint8_t result = AF.higher - HL.higher; AF.lower = (copy-HL.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < HL.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void CPr_l() {uint16_t copy = AF.higher; uint8_t result = AF.higher - HL.lower;  AF.lower = (copy-HL.lower <0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < HL.lower  & 0xF) AF.lower |= 0x20; m_clock = 1;}
void CPr_a() {uint16_t copy = AF.higher; uint8_t result = AF.higher - AF.higher; AF.lower = (copy-AF.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < AF.higher & 0xF) AF.lower |= 0x20; m_clock = 1;}
void CPHL() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t result = AF.higher - memory_value; AF.lower = (copy-memory_value<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if (copy & 0xF < memory_value & 0xF) AF.lower |= 0x20; m_clock = 2;}
void CPn() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(program_counter.word++); uint8_t result = AF.higher - memory_value; AF.lower = (copy-memory_value<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if (copy & 0xF < memory_value & 0xF) AF.lower |= 0x20; m_clock = 2;}

// Decimal Adjust register A
void DAA() {uint8_t A = AF.higher; if((AF.lower&0x20) or (AF.higher&15 > 9)) AF.higher += 6; AF.lower &= 0xEF; if ((AF.lower&0x20) or (A>0x99)) {AF.higher += 0x60; AF.lower |= 0x10;} m_clock = 1;}

// Boolean logic A with register or memory
void ANDr_b() {AF.higher &= BC.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDr_c() {AF.higher &= BC.lower ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDr_d() {AF.higher &= DE.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDr_e() {AF.higher &= DE.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDr_h() {AF.higher &= HL.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDr_l() {AF.higher &= HL.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDr_a() {AF.higher &= AF.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDHL() {AF.higher &= mmu->ReadByte(HL.word); AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ANDn() {AF.higher &= mmu->ReadByte(program_counter.word++); AF.lower = AF.higher?0:0x80; m_clock = 1;}

void ORr_b() {AF.higher |= BC.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORr_c() {AF.higher |= BC.lower ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORr_d() {AF.higher |= DE.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORr_e() {AF.higher |= DE.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORr_h() {AF.higher |= HL.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORr_l() {AF.higher |= HL.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORr_a() {AF.higher |= AF.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORHL() {AF.higher |= mmu->ReadByte(HL.word); AF.lower = AF.higher?0:0x80; m_clock = 1;}
void ORn() {AF.higher |= mmu->ReadByte(program_counter.word++); AF.lower = AF.higher?0:0x80; m_clock = 1;}

void XORr_b() {AF.higher ^= BC.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORr_c() {AF.higher ^= BC.lower ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORr_d() {AF.higher ^= DE.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORr_e() {AF.higher ^= DE.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORr_h() {AF.higher ^= HL.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORr_l() {AF.higher ^= HL.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORr_a() {AF.higher ^= AF.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORHL() {AF.higher ^= mmu->ReadByte(HL.word); AF.lower = AF.higher?0:0x80; m_clock = 1;}
void XORn() {AF.higher ^= mmu->ReadByte(program_counter.word++); AF.lower = AF.higher?0:0x80; m_clock = 1;}

void INCr_b() {BC.higher++; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void INCr_c() {BC.lower++ ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void INCr_d() {DE.higher++; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void INCr_e() {DE.lower++ ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void INCr_h() {HL.higher++; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void INCr_l() {HL.lower++ ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void INCr_a() {AF.higher++; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void INCHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); mmu->WriteByte(HL.word, ++memory_value); AF.lower = memory_value?0:0x80; m_clock = 3;}

void DECr_b() {BC.higher--; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void DECr_c() {BC.lower-- ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void DECr_d() {DE.higher--; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void DECr_e() {DE.lower-- ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void DECr_h() {HL.higher--; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void DECr_l() {HL.lower-- ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void DECr_a() {AF.higher--; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void DECHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); mmu->WriteByte(HL.word, --memory_value); AF.lower = memory_value?0:0x80; m_clock = 3;}
 
void INCBC() {++BC.word; m_clock = 2;} // Note: z80.js shows as 1 cycle?
void INCDE() {++DE.word; m_clock = 2;}
void INCHL() {++HL.word; m_clock = 2;}
void INCSP() {++stack_pointer.word; m_clock = 2;}
 
void DECBC() {--BC.word; m_clock = 2;}
void DECDE() {--DE.word; m_clock = 2;}
void DECHL() {--HL.word; m_clock = 2;}
void DECSP() {--stack_pointer.word; m_clock = 2;}

// Bit manipulation
// Test if bit in register is set
void BIT0b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x01) AF.lower |= 0x80; m_clock = 2;} // Note: z80.js shows this as = 0x80 not |= 0x80
void BIT0c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x01) AF.lower |= 0x80; m_clock = 2;}
void BIT0d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x01) AF.lower |= 0x80; m_clock = 2;}
void BIT0e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x01) AF.lower |= 0x80; m_clock = 2;}
void BIT0h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x01) AF.lower |= 0x80; m_clock = 2;}
void BIT0l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x01) AF.lower |= 0x80; m_clock = 2;}
void BIT0a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x01) AF.lower |= 0x80; m_clock = 2;}
void BIT0m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x01) AF.lower |= 0x80; m_clock = 3;}

void RES0b() {BC.higher &=0xFE; m_clock = 2;}
void RES0c() {BC.lower  &=0xFE; m_clock = 2;}
void RES0d() {DE.higher &=0xFE; m_clock = 2;}
void RES0e() {DE.lower  &=0xFE; m_clock = 2;}
void RES0h() {HL.higher &=0xFE; m_clock = 2;}
void RES0l() {HL.lower  &=0xFE; m_clock = 2;}
void RES0a() {AF.higher &=0xFE; m_clock = 2;}
void RES0m() {uint8_t result = mmu->ReadByte(HL.word)&0xFE; mmu->WriteByte(HL.word, result); m_clock = 4;}
 
void SET0b() {BC.higher |= 0x01; m_clock = 2;}
void SET0c() {BC.lower  |= 0x01; m_clock = 2;}
void SET0d() {DE.higher |= 0x01; m_clock = 2;}
void SET0e() {DE.lower  |= 0x01; m_clock = 2;}
void SET0h() {HL.higher |= 0x01; m_clock = 2;}
void SET0l() {HL.lower  |= 0x01; m_clock = 2;}
void SET0a() {AF.higher |= 0x01; m_clock = 2;}
void SET0m() {uint8_t result = mmu->ReadByte(HL.word)|0x01; mmu->WriteByte(HL.word, result); m_clock = 4;}

void BIT1b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x02) AF.lower |= 0x80; m_clock = 2;}
void BIT1c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x02) AF.lower |= 0x80; m_clock = 2;}
void BIT1d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x02) AF.lower |= 0x80; m_clock = 2;}
void BIT1e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x02) AF.lower |= 0x80; m_clock = 2;}
void BIT1h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x02) AF.lower |= 0x80; m_clock = 2;}
void BIT1l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x02) AF.lower |= 0x80; m_clock = 2;}
void BIT1a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x02) AF.lower |= 0x80; m_clock = 2;}
void BIT1m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x02) AF.lower |= 0x80; m_clock = 3;}

void RES1b() {BC.higher &=0xFD; m_clock = 2;}
void RES1c() {BC.lower  &=0xFD; m_clock = 2;}
void RES1d() {DE.higher &=0xFD; m_clock = 2;}
void RES1e() {DE.lower  &=0xFD; m_clock = 2;}
void RES1h() {HL.higher &=0xFD; m_clock = 2;}
void RES1l() {HL.lower  &=0xFD; m_clock = 2;}
void RES1a() {AF.higher &=0xFD; m_clock = 2;}
void RES1m() {uint8_t result = mmu->ReadByte(HL.word)&0xFD; mmu->WriteByte(HL.word, result); m_clock = 4;}

void SET1b() {BC.higher |= 0x02; m_clock = 2;}
void SET1c() {BC.lower  |= 0x02; m_clock = 2;}
void SET1d() {DE.higher |= 0x02; m_clock = 2;}
void SET1e() {DE.lower  |= 0x02; m_clock = 2;}
void SET1h() {HL.higher |= 0x02; m_clock = 2;}
void SET1l() {HL.lower  |= 0x02; m_clock = 2;}
void SET1a() {AF.higher |= 0x02; m_clock = 2;}
void SET1m() {uint8_t result = mmu->ReadByte(HL.word)|0x02; mmu->WriteByte(HL.word, result); m_clock = 4;}

void BIT2b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x04) AF.lower |= 0x80; m_clock = 2;}
void BIT2c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x04) AF.lower |= 0x80; m_clock = 2;}
void BIT2d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x04) AF.lower |= 0x80; m_clock = 2;}
void BIT2e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x04) AF.lower |= 0x80; m_clock = 2;}
void BIT2h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x04) AF.lower |= 0x80; m_clock = 2;}
void BIT2l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x04) AF.lower |= 0x80; m_clock = 2;}
void BIT2a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x04) AF.lower |= 0x80; m_clock = 2;}
void BIT2m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x04) AF.lower |= 0x80; m_clock = 3;}

void RES2b() {BC.higher &=0xFB; m_clock = 2;}
void RES2c() {BC.lower  &=0xFB; m_clock = 2;}
void RES2d() {DE.higher &=0xFB; m_clock = 2;}
void RES2e() {DE.lower  &=0xFB; m_clock = 2;}
void RES2h() {HL.higher &=0xFB; m_clock = 2;}
void RES2l() {HL.lower  &=0xFB; m_clock = 2;}
void RES2a() {AF.higher &=0xFB; m_clock = 2;}
void RES2m() {uint8_t result = mmu->ReadByte(HL.word)&0xFB; mmu->WriteByte(HL.word, result); m_clock = 4;}

void SET2b() {BC.higher |= 0x04; m_clock = 2;}
void SET2c() {BC.lower  |= 0x04; m_clock = 2;}
void SET2d() {DE.higher |= 0x04; m_clock = 2;}
void SET2e() {DE.lower  |= 0x04; m_clock = 2;}
void SET2h() {HL.higher |= 0x04; m_clock = 2;}
void SET2l() {HL.lower  |= 0x04; m_clock = 2;}
void SET2a() {AF.higher |= 0x04; m_clock = 2;}
void SET2m() {uint8_t result = mmu->ReadByte(HL.word)|0x04; mmu->WriteByte(HL.word, result); m_clock = 4;}

void BIT3b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x08) AF.lower |= 0x80; m_clock = 2;}
void BIT3c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x08) AF.lower |= 0x80; m_clock = 2;}
void BIT3d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x08) AF.lower |= 0x80; m_clock = 2;}
void BIT3e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x08) AF.lower |= 0x80; m_clock = 2;}
void BIT3h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x08) AF.lower |= 0x80; m_clock = 2;}
void BIT3l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x08) AF.lower |= 0x80; m_clock = 2;}
void BIT3a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x08) AF.lower |= 0x80; m_clock = 2;}
void BIT3m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x08) AF.lower |= 0x80; m_clock = 3;}

void RES3b() {BC.higher &=0xF7; m_clock = 2;}
void RES3c() {BC.lower  &=0xF7; m_clock = 2;}
void RES3d() {DE.higher &=0xF7; m_clock = 2;}
void RES3e() {DE.lower  &=0xF7; m_clock = 2;}
void RES3h() {HL.higher &=0xF7; m_clock = 2;}
void RES3l() {HL.lower  &=0xF7; m_clock = 2;}
void RES3a() {AF.higher &=0xF7; m_clock = 2;}
void RES3m() {uint8_t result = mmu->ReadByte(HL.word)&0xF7; mmu->WriteByte(HL.word, result); m_clock = 4;}

void SET3b() {BC.higher |= 0x08; m_clock = 2;}
void SET3c() {BC.lower  |= 0x08; m_clock = 2;}
void SET3d() {DE.higher |= 0x08; m_clock = 2;}
void SET3e() {DE.lower  |= 0x08; m_clock = 2;}
void SET3h() {HL.higher |= 0x08; m_clock = 2;}
void SET3l() {HL.lower  |= 0x08; m_clock = 2;}
void SET3a() {AF.higher |= 0x08; m_clock = 2;}
void SET3m() {uint8_t result = mmu->ReadByte(HL.word)|0x08; mmu->WriteByte(HL.word, result); m_clock = 4;}

void BIT4b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x10) AF.lower |= 0x80; m_clock = 2;}
void BIT4c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x10) AF.lower |= 0x80; m_clock = 2;}
void BIT4d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x10) AF.lower |= 0x80; m_clock = 2;}
void BIT4e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x10) AF.lower |= 0x80; m_clock = 2;}
void BIT4h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x10) AF.lower |= 0x80; m_clock = 2;}
void BIT4l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x10) AF.lower |= 0x80; m_clock = 2;}
void BIT4a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x10) AF.lower |= 0x80; m_clock = 2;}
void BIT4m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x10) AF.lower |= 0x80; m_clock = 3;}

void RES4b() {BC.higher &=0xEF; m_clock = 2;}
void RES4c() {BC.lower  &=0xEF; m_clock = 2;}
void RES4d() {DE.higher &=0xEF; m_clock = 2;}
void RES4e() {DE.lower  &=0xEF; m_clock = 2;}
void RES4h() {HL.higher &=0xEF; m_clock = 2;}
void RES4l() {HL.lower  &=0xEF; m_clock = 2;}
void RES4a() {AF.higher &=0xEF; m_clock = 2;}
void RES4m() {uint8_t result = mmu->ReadByte(HL.word)&0xEF; mmu->WriteByte(HL.word, result); m_clock = 4;}

void SET4b() {BC.higher |= 0x10; m_clock = 2;}
void SET4c() {BC.lower  |= 0x10; m_clock = 2;}
void SET4d() {DE.higher |= 0x10; m_clock = 2;}
void SET4e() {DE.lower  |= 0x10; m_clock = 2;}
void SET4h() {HL.higher |= 0x10; m_clock = 2;}
void SET4l() {HL.lower  |= 0x10; m_clock = 2;}
void SET4a() {AF.higher |= 0x10; m_clock = 2;}
void SET4m() {uint8_t result = mmu->ReadByte(HL.word)|0x10; mmu->WriteByte(HL.word, result); m_clock = 4;}

void BIT5b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x20) AF.lower |= 0x80; m_clock = 2;}
void BIT5c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x20) AF.lower |= 0x80; m_clock = 2;}
void BIT5d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x20) AF.lower |= 0x80; m_clock = 2;}
void BIT5e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x20) AF.lower |= 0x80; m_clock = 2;}
void BIT5h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x20) AF.lower |= 0x80; m_clock = 2;}
void BIT5l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x20) AF.lower |= 0x80; m_clock = 2;}
void BIT5a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x20) AF.lower |= 0x80; m_clock = 2;}
void BIT5m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x20) AF.lower |= 0x80; m_clock = 3;}

void RES5b() {BC.higher &=0xDF; m_clock = 2;}
void RES5c() {BC.lower  &=0xDF; m_clock = 2;}
void RES5d() {DE.higher &=0xDF; m_clock = 2;}
void RES5e() {DE.lower  &=0xDF; m_clock = 2;}
void RES5h() {HL.higher &=0xDF; m_clock = 2;}
void RES5l() {HL.lower  &=0xDF; m_clock = 2;}
void RES5a() {AF.higher &=0xDF; m_clock = 2;}
void RES5m() {uint8_t result = mmu->ReadByte(HL.word)&0xDF; mmu->WriteByte(HL.word, result); m_clock = 4;}

void SET5b() {BC.higher |= 0x20; m_clock = 2;}
void SET5c() {BC.lower  |= 0x20; m_clock = 2;}
void SET5d() {DE.higher |= 0x20; m_clock = 2;}
void SET5e() {DE.lower  |= 0x20; m_clock = 2;}
void SET5h() {HL.higher |= 0x20; m_clock = 2;}
void SET5l() {HL.lower  |= 0x20; m_clock = 2;}
void SET5a() {AF.higher |= 0x20; m_clock = 2;}
void SET5m() {uint8_t result = mmu->ReadByte(HL.word)|0x20; mmu->WriteByte(HL.word, result); m_clock = 4;}

void BIT6b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x40) AF.lower |= 0x80; m_clock = 2;}
void BIT6c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x40) AF.lower |= 0x80; m_clock = 2;}
void BIT6d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x40) AF.lower |= 0x80; m_clock = 2;}
void BIT6e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x40) AF.lower |= 0x80; m_clock = 2;}
void BIT6h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x40) AF.lower |= 0x80; m_clock = 2;}
void BIT6l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x40) AF.lower |= 0x80; m_clock = 2;}
void BIT6a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x40) AF.lower |= 0x80; m_clock = 2;}
void BIT6m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x40) AF.lower |= 0x80; m_clock = 3;}

void RES6b() {BC.higher &=0xBF; m_clock = 2;}
void RES6c() {BC.lower  &=0xBF; m_clock = 2;}
void RES6d() {DE.higher &=0xBF; m_clock = 2;}
void RES6e() {DE.lower  &=0xBF; m_clock = 2;}
void RES6h() {HL.higher &=0xBF; m_clock = 2;}
void RES6l() {HL.lower  &=0xBF; m_clock = 2;}
void RES6a() {AF.higher &=0xBF; m_clock = 2;}
void RES6m() {uint8_t result = mmu->ReadByte(HL.word)&0xBF; mmu->WriteByte(HL.word, result); m_clock = 4;}

void SET6b() {BC.higher |= 0x40; m_clock = 2;}
void SET6c() {BC.lower  |= 0x40; m_clock = 2;}
void SET6d() {DE.higher |= 0x40; m_clock = 2;}
void SET6e() {DE.lower  |= 0x40; m_clock = 2;}
void SET6h() {HL.higher |= 0x40; m_clock = 2;}
void SET6l() {HL.lower  |= 0x40; m_clock = 2;}
void SET6a() {AF.higher |= 0x40; m_clock = 2;}
void SET6m() {uint8_t result = mmu->ReadByte(HL.word)|0x40; mmu->WriteByte(HL.word, result); m_clock = 4;}

void BIT7b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.higher&0x80) AF.lower |= 0x80; m_clock = 2;}
void BIT7c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(BC.lower &0x80) AF.lower |= 0x80; m_clock = 2;}
void BIT7d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.higher&0x80) AF.lower |= 0x80; m_clock = 2;}
void BIT7e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(DE.lower &0x80) AF.lower |= 0x80; m_clock = 2;}
void BIT7h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.higher&0x80) AF.lower |= 0x80; m_clock = 2;}
void BIT7l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(HL.lower &0x80) AF.lower |= 0x80; m_clock = 2;}
void BIT7a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(AF.higher&0x80) AF.lower |= 0x80; m_clock = 2;}
void BIT7m() {AF.lower &= 0x1F; AF.lower |= 0x20; if((mmu->ReadByte(HL.word))&0x80) AF.lower |= 0x80; m_clock = 3;}

void RES7b() {BC.higher &=0x7F; m_clock = 2;}
void RES7c() {BC.lower  &=0x7F; m_clock = 2;}
void RES7d() {DE.higher &=0x7F; m_clock = 2;}
void RES7e() {DE.lower  &=0x7F; m_clock = 2;}
void RES7h() {HL.higher &=0x7F; m_clock = 2;}
void RES7l() {HL.lower  &=0x7F; m_clock = 2;}
void RES7a() {AF.higher &=0x7F; m_clock = 2;}
void RES7m() {uint8_t result = mmu->ReadByte(HL.word)&0x7F; mmu->WriteByte(HL.word, result); m_clock = 4;}

void SET7b() {BC.higher |= 0x80; m_clock = 2;}
void SET7c() {BC.lower  |= 0x80; m_clock = 2;}
void SET7d() {DE.higher |= 0x80; m_clock = 2;}
void SET7e() {DE.lower  |= 0x80; m_clock = 2;}
void SET7h() {HL.higher |= 0x80; m_clock = 2;}
void SET7l() {HL.lower  |= 0x80; m_clock = 2;}
void SET7a() {AF.higher |= 0x80; m_clock = 2;}
void SET7m() {uint8_t result = mmu->ReadByte(HL.word)|0x80; mmu->WriteByte(HL.word, result); m_clock = 4;}

// Rotate A register Note: Missing Zero flag?
void RLA() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = AF.higher&0x80?0x10:0; AF.higher = AF.higher << 1 + carry_in; AF.lower = AF.lower&0xEF + carry_out; m_clock = 1;}
void RLCA() {uint8_t carry_in = AF.higher&0x80?1:0; uint8_t carry_out = AF.higher&0x80?0x10:0; AF.higher = AF.higher << 1 + carry_in; AF.lower = AF.lower&0xEF + carry_out; m_clock = 1;}
void RRA() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = AF.higher&1?0x10:0; AF.higher = AF.higher >> 1 + carry_in; AF.lower = AF.lower&0xEF + carry_out; m_clock = 1;}
void RRCA() {uint8_t carry_in = AF.higher&1?0x80:0; uint8_t carry_out = AF.higher&1?0x10:0; AF.higher = AF.higher >> 1 + carry_in; AF.lower = AF.lower&0xEF + carry_out; m_clock = 1;}

// Rotate register left
void RLr_b() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = BC.higher&0x80?0x10:0; BC.higher = BC.higher << 1 + carry_in; AF.lower = BC.higher?0:0x80; AF.lower = AF.lower&0xEF + carry_out; m_clock = 2;}
void RLr_c() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = BC.lower &0x80?0x10:0; BC.lower  = BC.lower  << 1 + carry_in; AF.lower = BC.lower ?0:0x80; AF.lower = AF.lower&0xEF + carry_out; m_clock = 2;}
void RLr_d() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = DE.higher&0x80?0x10:0; DE.higher = DE.higher << 1 + carry_in; AF.lower = DE.higher?0:0x80; AF.lower = AF.lower&0xEF + carry_out; m_clock = 2;}
void RLr_e() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = DE.lower &0x80?0x10:0; DE.lower  = DE.lower  << 1 + carry_in; AF.lower = DE.lower ?0:0x80; AF.lower = AF.lower&0xEF + carry_out; m_clock = 2;}
void RLr_h() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = HL.higher&0x80?0x10:0; HL.higher = HL.higher << 1 + carry_in; AF.lower = HL.higher?0:0x80; AF.lower = AF.lower&0xEF + carry_out; m_clock = 2;}
void RLr_l() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = HL.lower &0x80?0x10:0; HL.lower  = HL.lower  << 1 + carry_in; AF.lower = HL.lower ?0:0x80; AF.lower = AF.lower&0xEF + carry_out; m_clock = 2;}
void RLr_a() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = AF.higher&0x80?0x10:0; AF.higher = AF.higher << 1 + carry_in; AF.lower = AF.higher?0:0x80; AF.lower = AF.lower&0xEF + carry_out; m_clock = 2;}
void RLHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = memory_value&0x80?0x10:0; memory_value = memory_value << 1 + carry_in; AF.lower = memory_value?0:0x80; AF.lower = AF.lower&0xEF + carry_out; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

void RLCr_b
void RLCr_c
void RLCr_d
void RLCr_e
void RLCr_h
void RLCr_l
void RLCr_a
void RLCHL

void RRr_b
void RRr_c
void RRr_d
void RRr_e
void RRr_h
void RRr_l
void RRr_a
void RRHL

void RRCr_b
void RRCr_c
void RRCr_d
void RRCr_e
void RRCr_h
void RRCr_l
void RRCr_a
void RRCHL

void SLAr_b
void SLAr_c
void SLAr_d
void SLAr_e
void SLAr_h
void SLAr_l
void SLAr_a

void SLLr_b
void SLLr_c
void SLLr_d
void SLLr_e
void SLLr_h
void SLLr_l
void SLLr_a

void SRAr_b
void SRAr_c
void SRAr_d
void SRAr_e
void SRAr_h
void SRAr_l
void SRAr_a

void SRLr_b
void SRLr_c
void SRLr_d
void SRLr_e
void SRLr_h
void SRLr_l
void SRLr_a

void CPL
void NEG

void CCF
void SCF

// Stack
void PUSHBC
void PUSHDE
void PUSHHL
void PUSHAF

void POPBC
void POPDE
void POPHL
void POPAF

// Jump
void JPnn
void JPHL
void JPNZnn
void JPZnn
void JPNCnn
void JPCnn

void JRn
void JRNZn
void JRZn
void JRNCn
void JRCn

void DJNZn

void CALLnn
void CALLNZnn
void CALLZnn
void CALLNCnn
void CALLCnn

void RET
void RETI
void RETNZ
void RETZ
void RETNC
void RETC

void RST00
void RST08
void RST10
void RST18
void RST20
void RST28
void RST30
void RST38
void RST40
void RST48
void RST50
void RST58
void RST60

void NOP
void HALT

void DI
void EI