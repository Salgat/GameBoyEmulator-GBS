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
void ADDr_b() {}
void ADDr_c
void ADDr_d
void ADDr_e
void ADDr_h
void ADDr_l
void ADDr_a
void ADDHL
void ADDn
void ADDHLBC
void ADDHLDE
void ADDHLHL
void ADDHLSP
void ADDSPn

void ADCr_b
void ADCr_c
void ADCr_d
void ADCr_e
void ADCr_h
void ADCr_l
void ADCr_a
void ADCHL
void ADCn

void SUBr_b
void SUBr_c
void SUBr_d
void SUBr_e
void SUBr_h
void SUBr_l
void SUBr_a
void SUBHL
void SUBn

void SBCr_b
void SBCr_c
void SBCr_d
void SBCr_e
void SBCr_h
void SBCr_l
void SBCr_a
void SBCHL
void SBCn

void CPr_b
void CPr_c
void CPr_d
void CPr_e
void CPr_h
void CPr_l
void CPr_a
void CPHL
void CPn

void DAA

void ANDr_b
void ANDr_c
void ANDr_d
void ANDr_e
void ANDr_h
void ANDr_l
void ANDr_a
void ANDHL
void ANDn

void ORr_b
void ORr_c
void ORr_d
void ORr_e
void ORr_h
void ORr_l
void ORr_a
void ORHL
void ORn

void XORr_b
void XORr_c
void XORr_d
void XORr_e
void XORr_h
void XORr_l
void XORr_a
void XORHL
void XORn

void INCr_b
void INCr_c
void INCr_d
void INCr_e
void INCr_h
void INCr_l
void INCr_a
void INCHLm

void DECr_b
void DECr_c
void DECr_d
void DECr_e
void DECr_h
void DECr_l
void DECr_a
void DECHLm
 
void INCBC
void INCDE
void INCHL
void INCSP
 
void DECBC
void DECDE
void DECHL
void DECSP

// Bit manipulation
void BIT0b
void BIT0c
void BIT0d
void BIT0e
void BIT0h
void BIT0l
void BIT0a
void BIT0m

void RES0b
void RES0c
void RES0d
void RES0e
void RES0h
void RES0l
void RES0a
void RES0m
 
void SET0b
void SET0c
void SET0d
void SET0e
void SET0h
void SET0l
void SET0a
void SET0m

void BIT1b
void BIT1c
void BIT1d
void BIT1e
void BIT1h
void BIT1l
void BIT1a
void BIT1m

void RES1b
void RES1c
void RES1d
void RES1e
void RES1h
void RES1l
void RES1a
void RES1m

void SET1b
void SET1c
void SET1d
void SET1e
void SET1h
void SET1l
void SET1a
void SET1m

void BIT2b
void BIT2c
void BIT2d
void BIT2e
void BIT2h
void BIT2l
void BIT2a
void BIT2m

void RES2b
void RES2c
void RES2d
void RES2e
void RES2h
void RES2l
void RES2a
void RES2m

void SET2b
void SET2c
void SET2d
void SET2e
void SET2h
void SET2l
void SET2a
void SET2m

void BIT3b
void BIT3c
void BIT3d
void BIT3e
void BIT3h
void BIT3l
void BIT3a
void BIT3m

void RES3b
void RES3c
void RES3d
void RES3e
void RES3h
void RES3l
void RES3a
void RES3m

void SET3b
void SET3c
void SET3d
void SET3e
void SET3h
void SET3l
void SET3a
void SET3m

void BIT4b
void BIT4c
void BIT4d
void BIT4e
void BIT4h
void BIT4l
void BIT4a
void BIT4m

void RES4b
void RES4c
void RES4d
void RES4e
void RES4h
void RES4l
void RES4a
void RES4m

void SET4b
void SET4c
void SET4d
void SET4e
void SET4h
void SET4l
void SET4a
void SET4m

void BIT5b
void BIT5c
void BIT5d
void BIT5e
void BIT5h
void BIT5l
void BIT5a
void BIT5m

void RES5b
void RES5c
void RES5d
void RES5e
void RES5h
void RES5l
void RES5a
void RES5m

void SET5b
void SET5c
void SET5d
void SET5e
void SET5h
void SET5l
void SET5a
void SET5m

void BIT6b
void BIT6c
void BIT6d
void BIT6e
void BIT6h
void BIT6l
void BIT6a
void BIT6m

void RES6b
void RES6c
void RES6d
void RES6e
void RES6h
void RES6l
void RES6a
void RES6m

void SET6b
void SET6c
void SET6d
void SET6e
void SET6h
void SET6l
void SET6a
void SET6m

void BIT7b
void BIT7c
void BIT7d
void BIT7e
void BIT7h
void BIT7l
void BIT7a
void BIT7m

void RES7b
void RES7c
void RES7d
void RES7e
void RES7h
void RES7l
void RES7a
void RES7m

void SET7b
void SET7c
void SET7d
void SET7e
void SET7h
void SET7l
void SET7a
void SET7m

void RLA
void RLCA
void RRA
void RRCA
 
void RLr_b
void RLr_c
void RLr_d
void RLr_e
void RLr_h
void RLr_l
void RLr_a
void RLHL

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