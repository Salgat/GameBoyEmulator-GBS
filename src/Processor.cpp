//
// Created by Austin on 6/11/2015.
//
#include <iostream>
#include <iomanip>
#include "Processor.hpp"
#include "MemoryManagementUnit.hpp"

Processor::Processor() {
	opcode_map = {
		// 00
		[this](){return NOP();},		[this](){return LDBCnn();},		[this](){return LDBCmA();},		[this](){return INCBC();},
		[this](){return INCr_b();},		[this](){return DECr_b();},		[this](){return LDrn_b();},		[this](){return RLCA();},
		[this](){return LDmmSP();},		[this](){return ADDHLBC();},	[this](){return LDABCm();},		[this](){return DECBC();},
		[this](){return INCr_c();},		[this](){return DECr_c();},		[this](){return LDrn_c();},		[this](){return RRCA();},
		// 10                   
		[this](){return STOP();},		[this](){return LDDEnn();},		[this](){return LDDEmA();},		[this](){return INCDE();},
		[this](){return INCr_d();},		[this](){return DECr_d();},		[this](){return LDrn_d();},		[this](){return RLA();},
		[this](){return JRn();},		[this](){return ADDHLDE();},	[this](){return LDADEm();},		[this](){return DECDE();},
		[this](){return INCr_e();},		[this](){return DECr_e();},		[this](){return LDrn_e();},		[this](){return RRA();},
		// 20                   
		[this](){return JRNZn();},		[this](){return LDHLnn();},		[this](){return LDHLIA();},		[this](){return INCHL();},
		[this](){return INCr_h();},		[this](){return DECr_h();},		[this](){return LDrn_h();},		[this](){return DAA();},
		[this](){return JRZn();},		[this](){return ADDHLHL();},	[this](){return LDAHLI();},		[this](){return DECHL();},
		[this](){return INCr_l();},		[this](){return DECr_l();},		[this](){return LDrn_l();},		[this](){return CPL();},
		// 30                   
		[this](){return JRNCn();},		[this](){return LDSPnn();},		[this](){return LDHLDA();},		[this](){return INCSP();},
		[this](){return INCHLm();},		[this](){return DECHLm();},		[this](){return LDHLmn();},		[this](){return SCF();},
		[this](){return JRCn();},		[this](){return ADDHLSP();},	[this](){return LDAHLD();},		[this](){return DECSP();},
		[this](){return INCr_a();},		[this](){return DECr_a();},		[this](){return LDrn_a();},		[this](){return CCF();},
		// 40                   
		[this](){return LDrr_bb();},	[this](){return LDrr_bc();},	[this](){return LDrr_bd();},	[this](){return LDrr_be();},
		[this](){return LDrr_bh();},	[this](){return LDrr_bl();},	[this](){return LDrHLm_b();},	[this](){return LDrr_ba();},
		[this](){return LDrr_cb();},	[this](){return LDrr_cc();},	[this](){return LDrr_cd();},	[this](){return LDrr_ce();},
		[this](){return LDrr_ch();},	[this](){return LDrr_cl();},	[this](){return LDrHLm_c();},	[this](){return LDrr_ca();},
		// 50                   
		[this](){return LDrr_db();},	[this](){return LDrr_dc();},	[this](){return LDrr_dd();},	[this](){return LDrr_de();},
		[this](){return LDrr_dh();},	[this](){return LDrr_dl();},	[this](){return LDrHLm_d();},	[this](){return LDrr_da();},
		[this](){return LDrr_eb();},	[this](){return LDrr_ec();},	[this](){return LDrr_ed();},	[this](){return LDrr_ee();},
		[this](){return LDrr_eh();},	[this](){return LDrr_el();},	[this](){return LDrHLm_e();},	[this](){return LDrr_ea();},
		// 60                   
		[this](){return LDrr_hb();},	[this](){return LDrr_hc();},	[this](){return LDrr_hd();},	[this](){return LDrr_he();},
		[this](){return LDrr_hh();},	[this](){return LDrr_hl();},	[this](){return LDrHLm_h();},	[this](){return LDrr_ha();},
		[this](){return LDrr_lb();},	[this](){return LDrr_lc();},	[this](){return LDrr_ld();},	[this](){return LDrr_le();},
		[this](){return LDrr_lh();},	[this](){return LDrr_ll();},	[this](){return LDrHLm_l();},	[this](){return LDrr_la();},
		// 70                   
		[this](){return LDHLmr_b();},	[this](){return LDHLmr_c();},	[this](){return LDHLmr_d();},	[this](){return LDHLmr_e();},
		[this](){return LDHLmr_h();},	[this](){return LDHLmr_l();},	[this](){return HALT();},		[this](){return LDHLmr_a();},
		[this](){return LDrr_ab();},	[this](){return LDrr_ac();},	[this](){return LDrr_ad();},	[this](){return LDrr_ae();},
		[this](){return LDrr_ah();},	[this](){return LDrr_al();},	[this](){return LDrHLm_a();},	[this](){return LDrr_aa();},
		// 80                   
		[this](){return ADDr_b();},		[this](){return ADDr_c();},		[this](){return ADDr_d();},		[this](){return ADDr_e();},
		[this](){return ADDr_h();},		[this](){return ADDr_l();},		[this](){return ADDHL();},		[this](){return ADDr_a();},
		[this](){return ADCr_b();},		[this](){return ADCr_c();},		[this](){return ADCr_d();},		[this](){return ADCr_e();},
		[this](){return ADCr_h();},		[this](){return ADCr_l();},		[this](){return ADCHL();},		[this](){return ADCr_a();},
		// 90                   
		[this](){return SUBr_b();},		[this](){return SUBr_c();},		[this](){return SUBr_d();},		[this](){return SUBr_e();},
		[this](){return SUBr_h();},		[this](){return SUBr_l();},		[this](){return SUBHL();},		[this](){return SUBr_a();},
		[this](){return SBCr_b();},		[this](){return SBCr_c();},		[this](){return SBCr_d();},		[this](){return SBCr_e();},
		[this](){return SBCr_h();},		[this](){return SBCr_l();},		[this](){return SBCHL();},		[this](){return SBCr_a();},
		// A0                   
		[this](){return ANDr_b();},		[this](){return ANDr_c();},		[this](){return ANDr_d();},		[this](){return ANDr_e();},
		[this](){return ANDr_h();},		[this](){return ANDr_l();},		[this](){return ANDHL();},		[this](){return ANDr_a();},
		[this](){return XORr_b();},		[this](){return XORr_c();},		[this](){return XORr_d();},		[this](){return XORr_e();},
		[this](){return XORr_h();},		[this](){return XORr_l();},		[this](){return XORHL();},		[this](){return XORr_a();},
		// B0                   
		[this](){return ORr_b();},		[this](){return ORr_c();},		[this](){return ORr_d();},		[this](){return ORr_e();},
		[this](){return ORr_h();},		[this](){return ORr_l();},		[this](){return ORHL();},		[this](){return ORr_a();},
		[this](){return CPr_b();},		[this](){return CPr_c();},		[this](){return CPr_d();},		[this](){return CPr_e();},
		[this](){return CPr_h();},		[this](){return CPr_l();},		[this](){return CPHL();},		[this](){return CPr_a();},
		// C0                                           
		[this](){return RETNZ();},		[this](){return POPBC();},		[this](){return JPNZnn();},		[this](){return JPnn();},
		[this](){return CALLNZnn();},	[this](){return PUSHBC();},		[this](){return ADDn();},		[this](){return RST00();},
		[this](){return RETZ();},		[this](){return RET();},		[this](){return JPZnn();},		[this](){return MAPcb();},
		[this](){return CALLZnn();},	[this](){return CALLnn();},		[this](){return ADCn();},		[this](){return RST08();},
		// D0                                           
		[this](){return RETNC();},		[this](){return POPDE();},		[this](){return JPNCnn();},		[this](){return XX();},
		[this](){return CALLNCnn();},	[this](){return PUSHDE();},		[this](){return SUBn();},		[this](){return RST10();},
		[this](){return RETC();},		[this](){return RETI();},		[this](){return JPCnn();},		[this](){return XX();},
		[this](){return CALLCnn();},	[this](){return XX();},			[this](){return SBCn();},		[this](){return RST18();},
		// E0                                           
		[this](){return LDIOnA();},		[this](){return POPHL();},		[this](){return LDIOCA();},		[this](){return XX();},
		[this](){return XX();},			[this](){return PUSHHL();},		[this](){return ANDn();},		[this](){return RST20();},
		[this](){return ADDSPn();},		[this](){return JPHL();},		[this](){return LDmmA();},		[this](){return XX();},
		[this](){return XX();},			[this](){return XX();},			[this](){return XORn();},		[this](){return RST28();},
		// F0                   
		[this](){return LDAIOn();},		[this](){return POPAF();},		[this](){return LDAIOC();},		[this](){return DI();},
		[this](){return XX();},			[this](){return PUSHAF();},		[this](){return ORn();},		[this](){return RST30();},
		[this](){return LDHLSPn();},	[this](){return LDSPHL();},		[this](){return LDAmm();},		[this](){return EI();},
		[this](){return XX();},			[this](){return XX();},			[this](){return CPn();},		[this](){return RST38();}};
	
	cb_opcode_map = {
		// CB00
		[this](){return RLCr_b();},		[this](){return RLCr_c();},		[this](){return RLCr_d();},		[this](){return RLCr_e();},
		[this](){return RLCr_h();},		[this](){return RLCr_l();},		[this](){return RLCHL();},		[this](){return RLCr_a();},
		[this](){return RRCr_b();},		[this](){return RRCr_c();},		[this](){return RRCr_d();},		[this](){return RRCr_e();},
		[this](){return RRCr_h();},		[this](){return RRCr_l();},		[this](){return RRCHL();},		[this](){return RRCr_a();},
		// CB10	                
		[this](){return RLr_b();},		[this](){return RLr_c();},		[this](){return RLr_d();},		[this](){return RLr_e();},
		[this](){return RLr_h();},		[this](){return RLr_l();},		[this](){return RLHL();},		[this](){return RLr_a();},
		[this](){return RRr_b();},		[this](){return RRr_c();},		[this](){return RRr_d();},		[this](){return RRr_e();},
		[this](){return RRr_h();},		[this](){return RRr_l();},		[this](){return RRHL();},		[this](){return RRr_a();},
		// CB20	                
		[this](){return SLAr_b();},		[this](){return SLAr_c();},		[this](){return SLAr_d();},		[this](){return SLAr_e();},
		[this](){return SLAr_h();},		[this](){return SLAr_l();},		[this](){return SLAHLm();},		[this](){return SLAr_a();},
		[this](){return SRAr_b();},		[this](){return SRAr_c();},		[this](){return SRAr_d();},		[this](){return SRAr_e();},
		[this](){return SRAr_h();},		[this](){return SRAr_l();},		[this](){return SRAHLm();},		[this](){return SRAr_a();},
		// CB30	                
		[this](){return SWAPr_b();},	[this](){return SWAPr_c();},	[this](){return SWAPr_d();},	[this](){return SWAPr_e();},
		[this](){return SWAPr_h();},	[this](){return SWAPr_l();},	[this](){return SWAPHLm();},	[this](){return SWAPr_a();},
		[this](){return SRLr_b();},		[this](){return SRLr_c();},		[this](){return SRLr_d();},		[this](){return SRLr_e();},
		[this](){return SRLr_h();},		[this](){return SRLr_l();},		[this](){return SRLHLm();},			[this](){return SRLr_a();},
		// CB40	                
		[this](){return BIT0b();},		[this](){return BIT0c();},		[this](){return BIT0d();},		[this](){return BIT0e();},
		[this](){return BIT0h();},		[this](){return BIT0l();},		[this](){return BIT0m();},		[this](){return BIT0a();},
		[this](){return BIT1b();},		[this](){return BIT1c();},		[this](){return BIT1d();},		[this](){return BIT1e();},
		[this](){return BIT1h();},		[this](){return BIT1l();},		[this](){return BIT1m();},		[this](){return BIT1a();},
		// CB50	                                        
		[this](){return BIT2b();},		[this](){return BIT2c();},		[this](){return BIT2d();},		[this](){return BIT2e();},
		[this](){return BIT2h();},		[this](){return BIT2l();},		[this](){return BIT2m();},		[this](){return BIT2a();},
		[this](){return BIT3b();},		[this](){return BIT3c();},		[this](){return BIT3d();},		[this](){return BIT3e();},
		[this](){return BIT3h();},		[this](){return BIT3l();},		[this](){return BIT3m();},		[this](){return BIT3a();},
		// CB60	                                        
		[this](){return BIT4b();},		[this](){return BIT4c();},		[this](){return BIT4d();},		[this](){return BIT4e();},
		[this](){return BIT4h();},		[this](){return BIT4l();},		[this](){return BIT4m();},		[this](){return BIT4a();},
		[this](){return BIT5b();},		[this](){return BIT5c();},		[this](){return BIT5d();},		[this](){return BIT5e();},
		[this](){return BIT5h();},		[this](){return BIT5l();},		[this](){return BIT5m();},		[this](){return BIT5a();},
		// CB70	                                                         
		[this](){return BIT6b();},		[this](){return BIT6c();},		[this](){return BIT6d();},		[this](){return BIT6e();},
		[this](){return BIT6h();},		[this](){return BIT6l();},		[this](){return BIT6m();},		[this](){return BIT6a();},
		[this](){return BIT7b();},		[this](){return BIT7c();},		[this](){return BIT7d();},		[this](){return BIT7e();},
		[this](){return BIT7h();},		[this](){return BIT7l();},		[this](){return BIT7m();},		[this](){return BIT7a();},
		// CB80	                                                                                 
		[this](){return RES0b();},		[this](){return RES0c();},		[this](){return RES0d();},		[this](){return RES0e();},
		[this](){return RES0h();},		[this](){return RES0l();},		[this](){return RES0m();},		[this](){return RES0a();},
		[this](){return RES1b();},		[this](){return RES1c();},		[this](){return RES1d();},		[this](){return RES1e();},
		[this](){return RES1h();},		[this](){return RES1l();},		[this](){return RES1m();},		[this](){return RES1a();},
		// CB90	                                                                                     
		[this](){return RES2b();},		[this](){return RES2c();},		[this](){return RES2d();},		[this](){return RES2e();},
		[this](){return RES2h();},		[this](){return RES2l();},		[this](){return RES2m();},		[this](){return RES2a();},
		[this](){return RES3b();},		[this](){return RES3c();},		[this](){return RES3d();},		[this](){return RES3e();},
		[this](){return RES3h();},		[this](){return RES3l();},		[this](){return RES3m();},		[this](){return RES3a();},
		// CBA0	                                        
		[this](){return RES4b();},		[this](){return RES4c();},		[this](){return RES4d();},		[this](){return RES4e();},
		[this](){return RES4h();},		[this](){return RES4l();},		[this](){return RES4m();},		[this](){return RES4a();},
		[this](){return RES5b();},		[this](){return RES5c();},		[this](){return RES5d();},		[this](){return RES5e();},
		[this](){return RES5h();},		[this](){return RES5l();},		[this](){return RES5m();},		[this](){return RES5a();},
		// CBB0	                                                                                     
		[this](){return RES6b();},		[this](){return RES6c();},		[this](){return RES6d();},		[this](){return RES6e();},
		[this](){return RES6h();},		[this](){return RES6l();},		[this](){return RES6m();},		[this](){return RES6a();},
		[this](){return RES7b();},		[this](){return RES7c();},		[this](){return RES7d();},		[this](){return RES7e();},
		[this](){return RES7h();},		[this](){return RES7l();},		[this](){return RES7m();},		[this](){return RES7a();},
		// CBC0	                                                                                 
		[this](){return SET0b();},		[this](){return SET0c();},		[this](){return SET0d();},		[this](){return SET0e();},
		[this](){return SET0h();},		[this](){return SET0l();},		[this](){return SET0m();},		[this](){return SET0a();},
		[this](){return SET1b();},		[this](){return SET1c();},		[this](){return SET1d();},		[this](){return SET1e();},
		[this](){return SET1h();},		[this](){return SET1l();},		[this](){return SET1m();},		[this](){return SET1a();},
		// CBD0	                                                         
		[this](){return SET2b();},		[this](){return SET2c();},		[this](){return SET2d();},		[this](){return SET2e();},
		[this](){return SET2h();},		[this](){return SET2l();},		[this](){return SET2m();},		[this](){return SET2a();},
		[this](){return SET3b();},		[this](){return SET3c();},		[this](){return SET3d();},		[this](){return SET3e();},
		[this](){return SET3h();},		[this](){return SET3l();},		[this](){return SET3m();},		[this](){return SET3a();},
		// CBE0	                                                                                 
		[this](){return SET4b();},		[this](){return SET4c();},		[this](){return SET4d();},		[this](){return SET4e();},
		[this](){return SET4h();},		[this](){return SET4l();},		[this](){return SET4m();},		[this](){return SET4a();},
		[this](){return SET5b();},		[this](){return SET5c();},		[this](){return SET5d();},		[this](){return SET5e();},
		[this](){return SET5h();},		[this](){return SET5l();},		[this](){return SET5m();},		[this](){return SET5a();},
		// CBF0	                                                                                     
		[this](){return SET6b();},		[this](){return SET6c();},		[this](){return SET6d();},		[this](){return SET6e();},
		[this](){return SET6h();},		[this](){return SET6l();},		[this](){return SET6m();},		[this](){return SET6a();},
		[this](){return SET7b();},		[this](){return SET7c();},		[this](){return SET7d();},		[this](){return SET7e();},
		[this](){return SET7h();},		[this](){return SET7l();},		[this](){return SET7m();},		[this](){return SET7a();}};
		
    Reset();
}

void Processor::Initialize(MemoryManagementUnit* mmu_) {
    mmu = mmu_;
}

void Processor::ExecuteNextInstruction() {
	m_clock = 0;
	uint8_t memory_value = mmu->ReadByte(program_counter.word++);
	//std::cout << "Opcode, Program counter, Flag: " << std::hex << static_cast<unsigned int>(memory_value) << ", " << program_counter.word-1 << ", " << (AF.word) << std::endl;
    // LCDC: LCD control register (FF40)
    // STAT: LCD status register (FF41)
    // LY:   LCDC Y-Coordinate which determines which line on the screen is at (0-153, where 144-153 is V-Blank) (FF44)
	/*static bool after = false;
	if (program_counter.word-1 == 0x411A or after) {
		//after = true;
		std::cout << "---------------------------------------------------" << std::endl;
		std::cout << std::hex << "AF  : " << std::setw(8) << static_cast<unsigned int>(AF.word) << "\tLCDC: " << std::setw(8) <<  static_cast<unsigned int>(mmu->zram[0xFF40&0xFF]) << std::endl
							  << "BC  : " << std::setw(8) <<  static_cast<unsigned int>(BC.word) << "\tSTAT: " << std::setw(8) <<  static_cast<unsigned int>(mmu->zram[0xFF41&0xFF]) << std::endl
							  << "DE  : " << std::setw(8) <<  static_cast<unsigned int>(DE.word) << "\tLY  : " << std::setw(8) <<  static_cast<unsigned int>(mmu->zram[0xFF44&0xFF]) << std::endl
							  << "HL  : " << std::setw(8) <<  static_cast<unsigned int>(HL.word) << "\tCNTR: " << std::setw(8) <<  0 << std::endl
							  << "SP  : " << std::setw(8) <<  static_cast<unsigned int>(stack_pointer.word) << "\tIE  : " << std::setw(8) <<  static_cast<unsigned int>(mmu->interrupt_enable) << std::endl
							  << "PC  : " << std::setw(8) <<  static_cast<unsigned int>(program_counter.word-1) << "\tIF  : " << std::setw(8) <<  static_cast<unsigned int>(mmu->interrupt_flag) << std::endl
							  << "IME : " << std::setw(8) <<  static_cast<unsigned int>(interrupt_master_enable) << "\tCLK : " << std::setw(8) <<  static_cast<unsigned int>(clock) << std::endl
							  << "TIMA: " << std::setw(8) << static_cast<unsigned int>(mmu->zram[0xFF05&0xFF]) << "\tTAC : " << std::setw(8) <<  static_cast<unsigned int>(mmu->zram[0xFF07&0xFF]) << std::endl
							  << "OPC : " << std::setw(8) << static_cast<unsigned int>(memory_value) << std::endl;
	}
    /*
	if (program_counter.word-1 == 0xCB28 or program_counter.word-1 == 0xCB19 or program_counter.word-1 == 0xCE42 or program_counter.word-1 == 0xD801) {
		//while(true) {
            std::cout << "Got it: " << std::hex << static_cast<unsigned int>(program_counter.word-1) << std::endl;
        //}
	} else {
        //std::cout << "Program counter: " << std::hex << static_cast<unsigned int>(program_counter.word) << std::endl;
    }
*/
    opcode_map[memory_value]();
	clock += m_clock;
	//m_clock = 0;
}

void Processor::ExecuteOpcode(uint8_t opcode) {
	opcode_map[opcode]();
}

void Processor::Reset() {
    AF.higher = 0x01; // 0x01 = Normal Gameboy
    AF.lower = 0xB0; // Flag Register, default to this value
    BC.word = 0x0013;
    DE.word = 0x00D8;
    HL.word = 0x014D;
    stack_pointer.word = 0xFFFE;
    program_counter.word = 0x100;
	
	interrupt_master_enable = 0x01;
	
	halt = 0;
	frame_clock = 0;
	clock = 0;
	m_clock = 0;
}

/**
 * Returns from an interrupt, restoring registers stored on "stack".
 *
 * Note: This seems like the wrong way to approach this (with if this is called twice?)...
 */
void Processor::InterruptReturn() {
	AF.word = AF_stack.word;
	BC.word = BC_stack.word;
	DE.word = DE_stack.word;
	HL.word = HL_stack.word;
}

/**
 * Stores registers onto "stack".
 */
void Processor::InterruptStore() {
	AF_stack.word = AF.word;
	BC_stack.word = BC.word;
	DE_stack.word = DE.word;
	HL_stack.word = HL.word;
}

// Opcodes
void Processor::XX() {
	std::cout << "Error at opcode: " << program_counter.word-1 << ", ";
	
	uint8_t opcode = mmu->ReadByte(program_counter.word-1);
	uint8_t opcode_before = mmu->ReadByte(program_counter.word-2);
	if (opcode_before = 0xCB) {
		std::cout << std::hex << static_cast<unsigned int>(opcode_before) << " ";
	}
	std::cout << std::hex << static_cast<unsigned int>(opcode) << std::endl;
	
}

void Processor::LDrr_bb() {BC.higher = BC.higher; m_clock = 1;}
void Processor::LDrr_bc() {BC.higher = BC.lower;  m_clock = 1;}
void Processor::LDrr_bd() {BC.higher = DE.higher; m_clock = 1;}
void Processor::LDrr_be() {BC.higher = DE.lower;  m_clock = 1;}
void Processor::LDrr_bh() {BC.higher = HL.higher; m_clock = 1;}
void Processor::LDrr_bl() {BC.higher = HL.lower;  m_clock = 1;}
void Processor::LDrr_ba() {BC.higher = AF.higher; m_clock = 1;}
void Processor::LDrr_cb() {BC.lower  = BC.higher; m_clock = 1;}
void Processor::LDrr_cc() {BC.lower  = BC.lower;  m_clock = 1;}
void Processor::LDrr_cd() {BC.lower  = DE.higher; m_clock = 1;}
void Processor::LDrr_ce() {BC.lower  = DE.lower;  m_clock = 1;}
void Processor::LDrr_ch() {BC.lower  = HL.higher; m_clock = 1;}
void Processor::LDrr_cl() {BC.lower  = HL.lower;  m_clock = 1;}
void Processor::LDrr_ca() {BC.lower  = AF.higher; m_clock = 1;}
void Processor::LDrr_db() {DE.higher = BC.higher; m_clock = 1;}
void Processor::LDrr_dc() {DE.higher = BC.lower;  m_clock = 1;}
void Processor::LDrr_dd() {DE.higher = DE.higher; m_clock = 1;}
void Processor::LDrr_de() {DE.higher = DE.lower;  m_clock = 1;}
void Processor::LDrr_dh() {DE.higher = HL.higher; m_clock = 1;}
void Processor::LDrr_dl() {DE.higher = HL.lower;  m_clock = 1;}
void Processor::LDrr_da() {DE.higher = AF.higher; m_clock = 1;}
void Processor::LDrr_eb() {DE.lower  = BC.higher; m_clock = 1;}
void Processor::LDrr_ec() {DE.lower  = BC.lower;  m_clock = 1;}
void Processor::LDrr_ed() {DE.lower  = DE.higher; m_clock = 1;}
void Processor::LDrr_ee() {DE.lower  = DE.lower;  m_clock = 1;}
void Processor::LDrr_eh() {DE.lower  = HL.higher; m_clock = 1;}
void Processor::LDrr_el() {DE.lower  = HL.lower;  m_clock = 1;}
void Processor::LDrr_ea() {DE.lower  = AF.higher; m_clock = 1;}
void Processor::LDrr_hb() {HL.higher = BC.higher; m_clock = 1;}
void Processor::LDrr_hc() {HL.higher = BC.lower;  m_clock = 1;}
void Processor::LDrr_hd() {HL.higher = DE.higher; m_clock = 1;}
void Processor::LDrr_he() {HL.higher = DE.lower;  m_clock = 1;}
void Processor::LDrr_hh() {HL.higher = HL.higher; m_clock = 1;}
void Processor::LDrr_hl() {HL.higher = HL.lower;  m_clock = 1;}
void Processor::LDrr_ha() {HL.higher = AF.higher; m_clock = 1;}
void Processor::LDrr_lb() {HL.lower  = BC.higher; m_clock = 1;}
void Processor::LDrr_lc() {HL.lower  = BC.lower;  m_clock = 1;}
void Processor::LDrr_ld() {HL.lower  = DE.higher;  m_clock = 1;}
void Processor::LDrr_le() {HL.lower  = DE.lower; m_clock = 1;}
void Processor::LDrr_lh() {HL.lower  = HL.higher; m_clock = 1;}
void Processor::LDrr_ll() {HL.lower  = HL.lower;  m_clock = 1;}
void Processor::LDrr_la() {HL.lower  = AF.higher; m_clock = 1;}
void Processor::LDrr_ab() {AF.higher = BC.higher; m_clock = 1;}
void Processor::LDrr_ac() {AF.higher = BC.lower;  m_clock = 1;}
void Processor::LDrr_ad() {AF.higher = DE.higher; m_clock = 1;}
void Processor::LDrr_ae() {AF.higher = DE.lower;  m_clock = 1;}
void Processor::LDrr_ah() {AF.higher = HL.higher; m_clock = 1;}
void Processor::LDrr_al() {AF.higher = HL.lower;  m_clock = 1;}
void Processor::LDrr_aa() {AF.higher = AF.higher; m_clock = 1;}

void Processor::LDmmSP() {uint16_t address = mmu->ReadWord(program_counter.word); program_counter.word += 2; mmu->WriteWord(address, stack_pointer.word); m_clock = 5;}

// Load register from memory: register = *(HL)
void Processor::LDrHLm_b() {BC.higher = mmu->ReadByte(HL.word); m_clock = 2;}
void Processor::LDrHLm_c() {BC.lower  = mmu->ReadByte(HL.word); m_clock = 2;}
void Processor::LDrHLm_d() {DE.higher = mmu->ReadByte(HL.word); m_clock = 2;}
void Processor::LDrHLm_e() {DE.lower  = mmu->ReadByte(HL.word); m_clock = 2;}
void Processor::LDrHLm_h() {HL.higher = mmu->ReadByte(HL.word); m_clock = 2;}
void Processor::LDrHLm_l() {HL.lower  = mmu->ReadByte(HL.word); m_clock = 2;}
void Processor::LDrHLm_a() {AF.higher = mmu->ReadByte(HL.word); m_clock = 2;}

// Write memory from register: *(HL) = register
void Processor::LDHLmr_b() {mmu->WriteByte(HL.word, BC.higher); m_clock = 2;}
void Processor::LDHLmr_c() {mmu->WriteByte(HL.word, BC.lower);  m_clock = 2;}
void Processor::LDHLmr_d() {mmu->WriteByte(HL.word, DE.higher); m_clock = 2;}
void Processor::LDHLmr_e() {mmu->WriteByte(HL.word, DE.lower);  m_clock = 2;}
void Processor::LDHLmr_h() {mmu->WriteByte(HL.word, HL.higher); m_clock = 2;}
void Processor::LDHLmr_l() {mmu->WriteByte(HL.word, HL.lower);  m_clock = 2;}
void Processor::LDHLmr_a() {mmu->WriteByte(HL.word, AF.higher); m_clock = 2;}

// Load register with value: register = value (where value is at program counter)
void Processor::LDrn_b() {BC.higher = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2;}
void Processor::LDrn_c() {BC.lower  = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2;}
void Processor::LDrn_d() {DE.higher = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2;}
void Processor::LDrn_e() {DE.lower  = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2;}
void Processor::LDrn_h() {HL.higher = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2;}
void Processor::LDrn_l() {HL.lower  = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2;}
void Processor::LDrn_a() {AF.higher = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2;}

void Processor::LDHLmn() {mmu->WriteByte(HL.word, mmu->ReadByte(program_counter.word)); ++program_counter.word; m_clock = 3;}

void Processor::LDBCmA() {mmu->WriteByte(BC.word, AF.higher); m_clock = 2;}
void Processor::LDDEmA() {mmu->WriteByte(DE.word, AF.higher); m_clock = 2;}

void Processor::LDmmA() {mmu->WriteByte(mmu->ReadWord(program_counter.word), AF.higher); program_counter.word += 2; m_clock = 4;}

void Processor::LDABCm() {AF.higher = mmu->ReadByte(BC.word); m_clock = 2;}
void Processor::LDADEm() {AF.higher = mmu->ReadByte(DE.word); m_clock = 2;}

void Processor::LDAmm() {AF.higher = mmu->ReadByte(mmu->ReadWord(program_counter.word)); program_counter.word += 2; m_clock = 4;}

void Processor::LDBCnn() {BC.lower = mmu->ReadByte(program_counter.word); BC.higher = mmu->ReadByte(program_counter.word+1); program_counter.word += 2; m_clock = 3;}
void Processor::LDDEnn() {DE.lower = mmu->ReadByte(program_counter.word); DE.higher = mmu->ReadByte(program_counter.word+1); program_counter.word += 2; m_clock = 3;}
void Processor::LDHLnn() {HL.lower = mmu->ReadByte(program_counter.word); HL.higher = mmu->ReadByte(program_counter.word+1); program_counter.word += 2; m_clock = 3;}
void Processor::LDSPnn() {stack_pointer.lower = mmu->ReadByte(program_counter.word); stack_pointer.higher = mmu->ReadByte(program_counter.word+1); program_counter.word += 2; m_clock = 3;}

//void Processor::LDHLmm() {}
//void Processor::LDmmHL() {}

void Processor::LDHLIA() {mmu->WriteByte(HL.word, AF.higher); HL.word += 1; m_clock = 2;}
void Processor::LDAHLI() {AF.higher = mmu->ReadByte(HL.word); HL.word += 1; m_clock = 2;}

void Processor::LDHLDA() {mmu->WriteByte(HL.word, AF.higher); HL.word -= 1; m_clock = 2;}
void Processor::LDAHLD() {AF.higher = mmu->ReadByte(HL.word); HL.word -= 1; m_clock = 2;}

void Processor::LDAIOn() {AF.higher = mmu->ReadByte(0xFF00 + mmu->ReadByte(program_counter.word)); ++program_counter.word; m_clock = 3;}
void Processor::LDIOnA() {mmu->WriteByte(0xFF00 + mmu->ReadByte(program_counter.word), AF.higher); ++program_counter.word; m_clock = 3;}
void Processor::LDAIOC() {AF.higher = mmu->ReadByte(0xFF00 + BC.lower); m_clock = 2;}
void Processor::LDIOCA() {mmu->WriteByte(0xFF00 + BC.lower, AF.higher); m_clock = 2;}

void Processor::LDHLSPn() {
    uint8_t memory_value = mmu->ReadByte(program_counter.word++);

    AF.lower = 0x00;
    if (((stack_pointer.word&0xF) + (memory_value&0xF)) & 0xF0) {
        AF.lower |= 0x20;
    }
    if (((stack_pointer.word&0xFF) + (memory_value&0xFF)) & 0xF00) {
        AF.lower |= 0x10;
    }

    HL.word = stack_pointer.word +  memory_value;
    if (memory_value > 127) {
        HL.word -= 256;
    }
    m_clock = 3;
}

void Processor::LDSPHL() {stack_pointer.word = HL.word; m_clock = 2;}

// Swap upper and lower nibbles of register (affects zero flag)
void Processor::SWAPr_b() {uint8_t higher_nibble = BC.higher & 0xF0; uint8_t lower_nibble = BC.higher & 0x0F; BC.higher = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!BC.higher)?0x80:0; m_clock = 2;}
void Processor::SWAPr_c() {uint8_t higher_nibble = BC.lower  & 0xF0; uint8_t lower_nibble = BC.lower  & 0x0F; BC.lower  = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!BC.lower )?0x80:0; m_clock = 2;}
void Processor::SWAPr_d() {uint8_t higher_nibble = DE.higher & 0xF0; uint8_t lower_nibble = DE.higher & 0x0F; DE.higher = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!DE.higher)?0x80:0; m_clock = 2;}
void Processor::SWAPr_e() {uint8_t higher_nibble = DE.lower  & 0xF0; uint8_t lower_nibble = DE.lower  & 0x0F; DE.lower  = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!DE.lower )?0x80:0; m_clock = 2;}
void Processor::SWAPr_h() {uint8_t higher_nibble = HL.higher & 0xF0; uint8_t lower_nibble = HL.higher & 0x0F; HL.higher = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!HL.higher)?0x80:0; m_clock = 2;}
void Processor::SWAPr_l() {uint8_t higher_nibble = HL.lower  & 0xF0; uint8_t lower_nibble = HL.lower  & 0x0F; HL.lower  = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!HL.lower )?0x80:0; m_clock = 2;}
void Processor::SWAPr_a() {uint8_t higher_nibble = AF.higher & 0xF0; uint8_t lower_nibble = AF.higher & 0x0F; AF.higher = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!AF.higher)?0x80:0; m_clock = 2;}
void Processor::SWAPHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t higher_nibble = memory_value & 0xF0; uint8_t lower_nibble = memory_value & 0x0F; memory_value = (lower_nibble << 4) | (higher_nibble >> 4); AF.lower = (!memory_value)?0x80:0; mmu->WriteByte(HL.word, memory_value); m_clock = 4;} // Double check this value

// Data Manipulation
// Add to register or memory
void Processor::ADDr_b() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(BC.higher); AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (BC.higher & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += BC.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADDr_c() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(BC.lower);  AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (BC.lower  & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += BC.lower;  if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADDr_d() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(DE.higher); AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (DE.higher & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += DE.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADDr_e() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(DE.lower);  AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (DE.lower  & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += DE.lower;  if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADDr_h() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(HL.higher); AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (HL.higher & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += HL.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADDr_l() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(HL.lower);  AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (HL.lower  & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += HL.lower;  if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADDr_a() {uint16_t result = static_cast<uint16_t>(AF.higher) + static_cast<uint16_t>(AF.higher); AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (AF.higher & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += AF.higher; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADDHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint16_t result = static_cast<uint16_t>(AF.higher) + memory_value; AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (memory_value & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += memory_value; if(!AF.higher) AF.lower |= 0x80; m_clock = 2;}
void Processor::ADDn() {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; uint16_t result = static_cast<uint16_t>(AF.higher) + memory_value; AF.lower = (result > 0xFF)?0x10:0x00; if(((AF.higher & 0xF) + (memory_value & 0xF)) & 0x10) AF.lower |= 0x20; AF.higher += memory_value; if(!AF.higher) AF.lower |= 0x80; m_clock = 2;}
//void Processor::ADDHLBC() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(BC.word); if(result > 0xFFFF) AF.lower |= 0x10; else AF.lower &= 0xEF; HL.word += BC.word; m_clock = 3;}
void Processor::ADDHLBC() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(BC.word); AF.lower &= 0x80; if(result > 0xFFFF) AF.lower |= 0x10; if((HL.word&0xFFF)>(result&0xFFF)) AF.lower |= 0x20; HL.word += BC.word; m_clock = 2;}
void Processor::ADDHLDE() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(DE.word); AF.lower &= 0x80; if(result > 0xFFFF) AF.lower |= 0x10; if((HL.word&0xFFF)>(result&0xFFF)) AF.lower |= 0x20; HL.word += DE.word; m_clock = 2;}
void Processor::ADDHLHL() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(HL.word); AF.lower &= 0x80; if(result > 0xFFFF) AF.lower |= 0x10; if((HL.word&0xFFF)>(result&0xFFF)) AF.lower |= 0x20; HL.word += HL.word; m_clock = 2;}
void Processor::ADDHLSP() {uint32_t result = static_cast<uint32_t>(HL.word) + static_cast<uint32_t>(stack_pointer.word); AF.lower &= 0x80; if(result > 0xFFFF) AF.lower |= 0x10; if((HL.word&0xFFF)>(result&0xFFF)) AF.lower |= 0x20; HL.word += stack_pointer.word; m_clock = 2;}
void Processor::ADDSPn() {
    uint8_t memory_value = mmu->ReadByte(program_counter.word++);

    AF.lower = 0x00;
    if (((stack_pointer.word&0xF) + (memory_value&0xF)) & 0xF0) {
        AF.lower |= 0x20;
    }
    if (((stack_pointer.word&0xFF) + (memory_value&0xFF)) & 0xF00) {
        AF.lower |= 0x10;
    }

    stack_pointer.word += memory_value;
    if (memory_value > 127) {
        stack_pointer.word -= 256;
    }
    m_clock = 3;
}

// Add to register or memory then add carry bit
//void Processor::ADCr_b() {uint16_t copy = AF.higher; AF.higher += BC.higher; AF.higher += (AF.lower&0x10)?1:0; AF.lower = (copy+BC.higher>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if (((copy & 0xF) + (BC.higher & 0xF) + ((copy+BC.higher>0xFF)?0x10:0)) > 0xF) AF.lower |= 0x20; m_clock = 1;}
void Processor::ADCr_b() {uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + BC.higher + carry; AF.lower = 0x00; if((AF.higher&0xF) + (BC.higher&0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += BC.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADCr_c() {uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + BC.lower  + carry; AF.lower = 0x00; if((AF.higher&0xF) + (BC.lower &0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += BC.lower  + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADCr_d() {uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + DE.higher + carry; AF.lower = 0x00; if((AF.higher&0xF) + (DE.higher&0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += DE.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADCr_e() {uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + DE.lower  + carry; AF.lower = 0x00; if((AF.higher&0xF) + (DE.lower &0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += DE.lower  + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADCr_h() {uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + HL.higher + carry; AF.lower = 0x00; if((AF.higher&0xF) + (HL.higher&0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += HL.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADCr_l() {uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + HL.lower  + carry; AF.lower = 0x00; if((AF.higher&0xF) + (HL.lower &0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += HL.lower  + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::ADCr_a() {uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + AF.higher + carry; AF.lower = 0x00; if((AF.higher&0xF) + (AF.higher&0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += AF.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
//void Processor::ADCHL() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(HL.word); AF.higher += memory_value; AF.higher += (AF.lower&0x10)?1:0; AF.lower = (copy+memory_value>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if (((copy & 0xF) + (memory_value & 0xF)) & 0x10) AF.lower |= 0x20; m_clock = 2;}
void Processor::ADCHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + memory_value + carry; AF.lower = 0x00; if((AF.higher&0xF) + (memory_value&0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += memory_value + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
//void Processor::ADCn() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; AF.higher += memory_value; AF.higher += ((AF.lower&0x10)?1:0); AF.lower = (copy+memory_value>0xFF)?0x10:0; if(!AF.higher) AF.lower |= 0x80; if (((copy & 0xF) + (memory_value & 0xF)) & 0x10) AF.lower |= 0x20; m_clock = 2;}
void Processor::ADCn() {uint8_t memory_value = mmu->ReadByte(program_counter.word++); uint8_t carry = (AF.lower&0x10)?1:0; uint16_t result = AF.higher + memory_value + carry; AF.lower = 0x00; if((AF.higher&0xF) + (memory_value&0xF) + carry > 0xF) AF.lower |= 0x20; if(result>0xFF) AF.lower |= 0x10; AF.higher += memory_value + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}

// Subtract register or memory from A
void Processor::SUBr_b() {int result = static_cast<int>(AF.higher) - static_cast<int>(BC.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (BC.higher & 0xF)) AF.lower |= 0x20; AF.higher -= BC.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBr_c() {int result = static_cast<int>(AF.higher) - static_cast<int>(BC.lower);  AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (BC.lower  & 0xF)) AF.lower |= 0x20; AF.higher -= BC.lower;  if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBr_d() {int result = static_cast<int>(AF.higher) - static_cast<int>(DE.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (DE.higher & 0xF)) AF.lower |= 0x20; AF.higher -= DE.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBr_e() {int result = static_cast<int>(AF.higher) - static_cast<int>(DE.lower);  AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (DE.lower  & 0xF)) AF.lower |= 0x20; AF.higher -= DE.lower;  if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBr_h() {int result = static_cast<int>(AF.higher) - static_cast<int>(HL.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (HL.higher & 0xF)) AF.lower |= 0x20; AF.higher -= HL.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBr_l() {int result = static_cast<int>(AF.higher) - static_cast<int>(HL.lower);  AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (HL.lower  & 0xF)) AF.lower |= 0x20; AF.higher -= HL.lower;  if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBr_a() {int result = static_cast<int>(AF.higher) - static_cast<int>(AF.higher); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (AF.higher & 0xF)) AF.lower |= 0x20; AF.higher -= AF.higher; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBHL() {uint8_t memory_value = mmu->ReadByte(HL.word); int result = static_cast<int>(AF.higher) - static_cast<int>(memory_value); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (memory_value & 0xF)) AF.lower |= 0x20; AF.higher -= memory_value; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SUBn() {uint8_t memory_value = mmu->ReadByte(program_counter.word++); int result = static_cast<int>(AF.higher) - static_cast<int>(memory_value); AF.lower = (result<0)?0x50:0x40; if((AF.higher & 0xF) < (memory_value & 0xF)) AF.lower |= 0x20; AF.higher -= memory_value; if (!AF.higher) AF.lower |= 0x80; m_clock = 1;}

// Subtract register or memory then carry (where carry represents under or overflow)
//void Processor::SBCr_b() {uint16_t copy = AF.higher; AF.higher -= BC.higher; AF.higher -= (AF.lower&0x10)?1:0; AF.lower = (copy-BC.higher<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if((copy & 0xF) < (BC.higher & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::SBCr_b() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (BC.higher&0xF) + carry) AF.lower |= 0x20; if(AF.higher<BC.higher+carry) AF.lower |= 0x10; AF.higher -= BC.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SBCr_c() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (BC.lower &0xF) + carry) AF.lower |= 0x20; if(AF.higher<BC.lower +carry) AF.lower |= 0x10; AF.higher -= BC.lower  + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SBCr_d() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (DE.higher&0xF) + carry) AF.lower |= 0x20; if(AF.higher<DE.higher+carry) AF.lower |= 0x10; AF.higher -= DE.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SBCr_e() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (DE.lower &0xF) + carry) AF.lower |= 0x20; if(AF.higher<DE.lower +carry) AF.lower |= 0x10; AF.higher -= DE.lower  + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SBCr_h() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (HL.higher&0xF) + carry) AF.lower |= 0x20; if(AF.higher<HL.higher+carry) AF.lower |= 0x10; AF.higher -= HL.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SBCr_l() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (HL.lower &0xF) + carry) AF.lower |= 0x20; if(AF.higher<HL.lower +carry) AF.lower |= 0x10; AF.higher -= HL.lower  + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
void Processor::SBCr_a() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (AF.higher&0xF) + carry) AF.lower |= 0x20; if(AF.higher<AF.higher+carry) AF.lower |= 0x10; AF.higher -= AF.higher + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
//void Processor::SBCHL() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(HL.word); AF.higher -= memory_value; AF.higher -= (AF.lower&0x10)?1:0; AF.lower = (copy-memory_value<0)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if((copy & 0xF) < (memory_value & 0xF)) AF.lower |= 0x20; m_clock = 2;}
void Processor::SBCHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (memory_value&0xF) + carry) AF.lower |= 0x20; if(AF.higher<memory_value+carry) AF.lower |= 0x10; AF.higher -= memory_value + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}
//void Processor::SBCn() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(program_counter.word++); AF.higher -= memory_value; AF.higher -= (AF.lower&0x10)?1:0; AF.lower = (copy<memory_value)?0x50:0x40; if(!AF.higher) AF.lower |= 0x80; if((copy & 0xF) < (memory_value & 0xF)) AF.lower |= 0x20; m_clock = 2;}
void Processor::SBCn() {uint8_t memory_value = mmu->ReadByte(program_counter.word++); uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = 0x40; if((AF.higher&0xF) < (memory_value&0xF) + carry) AF.lower |= 0x20; if(AF.higher<memory_value+carry) AF.lower |= 0x10; AF.higher -= memory_value + carry; if(!AF.higher) AF.lower |= 0x80; m_clock = 1;}

// Same as subtraction but results are discarded
void Processor::CPr_b() {uint16_t copy = AF.higher; uint8_t result = AF.higher - BC.higher; AF.lower = (copy-BC.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (BC.higher & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::CPr_c() {uint16_t copy = AF.higher; uint8_t result = AF.higher - BC.lower;  AF.lower = (copy-BC.lower <0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (BC.lower  & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::CPr_d() {uint16_t copy = AF.higher; uint8_t result = AF.higher - DE.higher; AF.lower = (copy-DE.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (DE.higher & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::CPr_e() {uint16_t copy = AF.higher; uint8_t result = AF.higher - DE.lower;  AF.lower = (copy-DE.lower <0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (DE.lower  & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::CPr_h() {uint16_t copy = AF.higher; uint8_t result = AF.higher - HL.higher; AF.lower = (copy-HL.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (HL.higher & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::CPr_l() {uint16_t copy = AF.higher; uint8_t result = AF.higher - HL.lower;  AF.lower = (copy-HL.lower <0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (HL.lower  & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::CPr_a() {uint16_t copy = AF.higher; uint8_t result = AF.higher - AF.higher; AF.lower = (copy-AF.higher<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (AF.higher & 0xF)) AF.lower |= 0x20; m_clock = 1;}
void Processor::CPHL() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t result = AF.higher - memory_value; AF.lower = (copy-memory_value<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (memory_value & 0xF)) AF.lower |= 0x20; m_clock = 2;}
void Processor::CPn() {uint16_t copy = AF.higher; uint8_t memory_value = mmu->ReadByte(program_counter.word++); uint8_t result = AF.higher - memory_value; AF.lower = (copy-memory_value<0)?0x50:0x40; if(!result) AF.lower |= 0x80; if((copy & 0xF) < (memory_value & 0xF)) AF.lower |= 0x20; m_clock = 2;}

// Decimal Adjust register A
// Lets try this one more time using Game Boy Online's implementation
void Processor::DAA() {
	if (!(AF.lower&0x40)) {
		if ((AF.lower&0x10) or (AF.higher > 0x99)) {
			AF.higher += 0x60;
			AF.lower |= 0x10;
		}
		
		if ((AF.lower&0x20) or ((AF.higher&0xF) > 0x9)) {
			AF.higher += 0x06;
			AF.lower &= 0xDF;
		}
	} else if ((AF.lower&0x10) and (AF.lower&0x20)) {
		AF.higher += 0x9A;
		AF.lower &= 0xDF;
	} else if (AF.lower&0x10) {
		AF.higher += 0xA0;
	} else if (AF.lower&0x20) {
		AF.higher += 0xFA;
		AF.lower &= 0xDF;
	}
	
	if (AF.higher) {
		AF.lower &= 0x7F;
	} else {
		AF.lower |= 0x80;
	}
}

// Boolean logic A with register or memory
void Processor::ANDr_b() {AF.higher &= BC.higher; AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDr_c() {AF.higher &= BC.lower ; AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDr_d() {AF.higher &= DE.higher; AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDr_e() {AF.higher &= DE.lower;  AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDr_h() {AF.higher &= HL.higher; AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDr_l() {AF.higher &= HL.lower;  AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDr_a() {AF.higher &= AF.higher; AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDHL() {AF.higher &= mmu->ReadByte(HL.word); AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}
void Processor::ANDn() {AF.higher &= mmu->ReadByte(program_counter.word++); AF.lower = AF.higher?0x20:0xA0; m_clock = 1;}

void Processor::ORr_b() {AF.higher |= BC.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORr_c() {AF.higher |= BC.lower ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORr_d() {AF.higher |= DE.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORr_e() {AF.higher |= DE.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORr_h() {AF.higher |= HL.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORr_l() {AF.higher |= HL.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORr_a() {AF.higher |= AF.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORHL() {AF.higher |= mmu->ReadByte(HL.word); AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::ORn() {AF.higher |= mmu->ReadByte(program_counter.word++); AF.lower = AF.higher?0:0x80; m_clock = 1;}

void Processor::XORr_b() {AF.higher ^= BC.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORr_c() {AF.higher ^= BC.lower ; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORr_d() {AF.higher ^= DE.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORr_e() {AF.higher ^= DE.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORr_h() {AF.higher ^= HL.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORr_l() {AF.higher ^= HL.lower;  AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORr_a() {AF.higher ^= AF.higher; AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORHL() {AF.higher ^= mmu->ReadByte(HL.word); AF.lower = AF.higher?0:0x80; m_clock = 1;}
void Processor::XORn() {AF.higher ^= mmu->ReadByte(program_counter.word++); AF.lower = AF.higher?0:0x80; m_clock = 1;}


void Processor::INCr_b() {uint8_t half = (((BC.higher&0xF)+1)&0x10)?0x20:0x00; BC.higher++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= BC.higher?0:0x80; m_clock = 1;}
void Processor::INCr_c() {uint8_t half = (((BC.lower &0xF)+1)&0x10)?0x20:0x00; BC.lower ++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= BC.lower ?0:0x80; m_clock = 1;}
void Processor::INCr_d() {uint8_t half = (((DE.higher&0xF)+1)&0x10)?0x20:0x00; DE.higher++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= DE.higher?0:0x80; m_clock = 1;}
void Processor::INCr_e() {uint8_t half = (((DE.lower &0xF)+1)&0x10)?0x20:0x00; DE.lower ++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= DE.lower ?0:0x80; m_clock = 1;}
void Processor::INCr_h() {uint8_t half = (((HL.higher&0xF)+1)&0x10)?0x20:0x00; HL.higher++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= HL.higher?0:0x80; m_clock = 1;}
void Processor::INCr_l() {uint8_t half = (((HL.lower &0xF)+1)&0x10)?0x20:0x00; HL.lower ++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= HL.lower ?0:0x80; m_clock = 1;}
void Processor::INCr_a() {uint8_t half = (((AF.higher&0xF)+1)&0x10)?0x20:0x00; AF.higher++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= AF.higher?0:0x80; m_clock = 1;}
void Processor::INCHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t half = (((memory_value&0xF)+1)&0x10)?0x20:0x00; memory_value++; AF.lower &= 0x10; AF.lower |= half; AF.lower |= memory_value?0:0x80; mmu->WriteByte(HL.word, memory_value); m_clock = 3;}

void Processor::DECr_b() {uint8_t half = (((BC.higher-1)&0xF) == 0xF)?0x20:0x00; BC.higher--; AF.lower &= 0x10; AF.lower |= BC.higher?0:0x80; AF.lower |= 0x40; if(half) AF.lower |= 0x20; m_clock = 1;}
void Processor::DECr_c() {uint8_t half = (((BC.lower -1)&0xF) == 0xF)?0x20:0x00; BC.lower --; AF.lower &= 0x10; AF.lower |= BC.lower ?0:0x80; AF.lower |= 0x40; if(half) AF.lower |= 0x20; m_clock = 1;}
void Processor::DECr_d() {uint8_t half = (((DE.higher-1)&0xF) == 0xF)?0x20:0x00; DE.higher--; AF.lower &= 0x10; AF.lower |= DE.higher?0:0x80; AF.lower |= 0x40; if(half) AF.lower |= 0x20; m_clock = 1;}
void Processor::DECr_e() {uint8_t half = (((DE.lower -1)&0xF) == 0xF)?0x20:0x00; DE.lower --; AF.lower &= 0x10; AF.lower |= DE.lower ?0:0x80; AF.lower |= 0x40; if(half) AF.lower |= 0x20; m_clock = 1;}
void Processor::DECr_h() {uint8_t half = (((HL.higher-1)&0xF) == 0xF)?0x20:0x00; HL.higher--; AF.lower &= 0x10; AF.lower |= HL.higher?0:0x80; AF.lower |= 0x40; if(half) AF.lower |= 0x20; m_clock = 1;}
void Processor::DECr_l() {uint8_t half = (((HL.lower -1)&0xF) == 0xF)?0x20:0x00; HL.lower --; AF.lower &= 0x10; AF.lower |= HL.lower ?0:0x80; AF.lower |= 0x40; if(half) AF.lower |= 0x20; m_clock = 1;}
void Processor::DECr_a() {uint8_t half = (((AF.higher-1)&0xF) == 0xF)?0x20:0x00; AF.higher--; AF.lower &= 0x10; AF.lower |= AF.higher?0:0x80; AF.lower |= 0x40; if(half) AF.lower |= 0x20; m_clock = 1;}
void Processor::DECHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t half = (((memory_value-1)&0xF) == 0xF)?0x20:0x00; memory_value--; AF.lower &= 0x10; AF.lower |= memory_value?0:0x80; AF.lower |= 0x40; mmu->WriteByte(HL.word, memory_value); if(half) AF.lower |= 0x20; m_clock = 3;}
 
void Processor::INCBC() {++BC.word; m_clock = 2;} // Note: z80.js shows as 1 cycle?
void Processor::INCDE() {++DE.word; m_clock = 2;}
void Processor::INCHL() {++HL.word; m_clock = 2;}
void Processor::INCSP() {++stack_pointer.word; m_clock = 2;}
 
void Processor::DECBC() {--BC.word; m_clock = 2;}
void Processor::DECDE() {--DE.word; m_clock = 2;}
void Processor::DECHL() {--HL.word; m_clock = 2;}
void Processor::DECSP() {--stack_pointer.word; m_clock = 2;}

// Bit manipulation
// Test if bit in register is set
void Processor::BIT0b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x01)) AF.lower |= 0x80; m_clock = 2;} // Note: z80.js shows this as = 0x80 not |= 0x80
void Processor::BIT0c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x01)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT0d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x01)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT0e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x01)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT0h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x01)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT0l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x01)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT0a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x01)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT0m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x01)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES0b() {BC.higher &=0xFE; m_clock = 2;}
void Processor::RES0c() {BC.lower  &=0xFE; m_clock = 2;}
void Processor::RES0d() {DE.higher &=0xFE; m_clock = 2;}
void Processor::RES0e() {DE.lower  &=0xFE; m_clock = 2;}
void Processor::RES0h() {HL.higher &=0xFE; m_clock = 2;}
void Processor::RES0l() {HL.lower  &=0xFE; m_clock = 2;}
void Processor::RES0a() {AF.higher &=0xFE; m_clock = 2;}
void Processor::RES0m() {uint8_t result = mmu->ReadByte(HL.word)&0xFE; mmu->WriteByte(HL.word, result); m_clock = 4;}
 
void Processor::SET0b() {BC.higher |= 0x01; m_clock = 2;}
void Processor::SET0c() {BC.lower  |= 0x01; m_clock = 2;}
void Processor::SET0d() {DE.higher |= 0x01; m_clock = 2;}
void Processor::SET0e() {DE.lower  |= 0x01; m_clock = 2;}
void Processor::SET0h() {HL.higher |= 0x01; m_clock = 2;}
void Processor::SET0l() {HL.lower  |= 0x01; m_clock = 2;}
void Processor::SET0a() {AF.higher |= 0x01; m_clock = 2;}
void Processor::SET0m() {uint8_t result = mmu->ReadByte(HL.word)|0x01; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::BIT1b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x02)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT1c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x02)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT1d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x02)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT1e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x02)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT1h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x02)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT1l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x02)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT1a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x02)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT1m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x02)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES1b() {BC.higher &=0xFD; m_clock = 2;}
void Processor::RES1c() {BC.lower  &=0xFD; m_clock = 2;}
void Processor::RES1d() {DE.higher &=0xFD; m_clock = 2;}
void Processor::RES1e() {DE.lower  &=0xFD; m_clock = 2;}
void Processor::RES1h() {HL.higher &=0xFD; m_clock = 2;}
void Processor::RES1l() {HL.lower  &=0xFD; m_clock = 2;}
void Processor::RES1a() {AF.higher &=0xFD; m_clock = 2;}
void Processor::RES1m() {uint8_t result = mmu->ReadByte(HL.word)&0xFD; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::SET1b() {BC.higher |= 0x02; m_clock = 2;}
void Processor::SET1c() {BC.lower  |= 0x02; m_clock = 2;}
void Processor::SET1d() {DE.higher |= 0x02; m_clock = 2;}
void Processor::SET1e() {DE.lower  |= 0x02; m_clock = 2;}
void Processor::SET1h() {HL.higher |= 0x02; m_clock = 2;}
void Processor::SET1l() {HL.lower  |= 0x02; m_clock = 2;}
void Processor::SET1a() {AF.higher |= 0x02; m_clock = 2;}
void Processor::SET1m() {uint8_t result = mmu->ReadByte(HL.word)|0x02; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::BIT2b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x04)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT2c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x04)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT2d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x04)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT2e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x04)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT2h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x04)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT2l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x04)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT2a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x04)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT2m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x04)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES2b() {BC.higher &=0xFB; m_clock = 2;}
void Processor::RES2c() {BC.lower  &=0xFB; m_clock = 2;}
void Processor::RES2d() {DE.higher &=0xFB; m_clock = 2;}
void Processor::RES2e() {DE.lower  &=0xFB; m_clock = 2;}
void Processor::RES2h() {HL.higher &=0xFB; m_clock = 2;}
void Processor::RES2l() {HL.lower  &=0xFB; m_clock = 2;}
void Processor::RES2a() {AF.higher &=0xFB; m_clock = 2;}
void Processor::RES2m() {uint8_t result = mmu->ReadByte(HL.word)&0xFB; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::SET2b() {BC.higher |= 0x04; m_clock = 2;}
void Processor::SET2c() {BC.lower  |= 0x04; m_clock = 2;}
void Processor::SET2d() {DE.higher |= 0x04; m_clock = 2;}
void Processor::SET2e() {DE.lower  |= 0x04; m_clock = 2;}
void Processor::SET2h() {HL.higher |= 0x04; m_clock = 2;}
void Processor::SET2l() {HL.lower  |= 0x04; m_clock = 2;}
void Processor::SET2a() {AF.higher |= 0x04; m_clock = 2;}
void Processor::SET2m() {uint8_t result = mmu->ReadByte(HL.word)|0x04; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::BIT3b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x08)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT3c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x08)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT3d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x08)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT3e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x08)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT3h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x08)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT3l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x08)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT3a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x08)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT3m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x08)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES3b() {BC.higher &=0xF7; m_clock = 2;}
void Processor::RES3c() {BC.lower  &=0xF7; m_clock = 2;}
void Processor::RES3d() {DE.higher &=0xF7; m_clock = 2;}
void Processor::RES3e() {DE.lower  &=0xF7; m_clock = 2;}
void Processor::RES3h() {HL.higher &=0xF7; m_clock = 2;}
void Processor::RES3l() {HL.lower  &=0xF7; m_clock = 2;}
void Processor::RES3a() {AF.higher &=0xF7; m_clock = 2;}
void Processor::RES3m() {uint8_t result = mmu->ReadByte(HL.word)&0xF7; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::SET3b() {BC.higher |= 0x08; m_clock = 2;}
void Processor::SET3c() {BC.lower  |= 0x08; m_clock = 2;}
void Processor::SET3d() {DE.higher |= 0x08; m_clock = 2;}
void Processor::SET3e() {DE.lower  |= 0x08; m_clock = 2;}
void Processor::SET3h() {HL.higher |= 0x08; m_clock = 2;}
void Processor::SET3l() {HL.lower  |= 0x08; m_clock = 2;}
void Processor::SET3a() {AF.higher |= 0x08; m_clock = 2;}
void Processor::SET3m() {uint8_t result = mmu->ReadByte(HL.word)|0x08; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::BIT4b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x10)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT4c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x10)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT4d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x10)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT4e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x10)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT4h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x10)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT4l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x10)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT4a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x10)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT4m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x10)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES4b() {BC.higher &=0xEF; m_clock = 2;}
void Processor::RES4c() {BC.lower  &=0xEF; m_clock = 2;}
void Processor::RES4d() {DE.higher &=0xEF; m_clock = 2;}
void Processor::RES4e() {DE.lower  &=0xEF; m_clock = 2;}
void Processor::RES4h() {HL.higher &=0xEF; m_clock = 2;}
void Processor::RES4l() {HL.lower  &=0xEF; m_clock = 2;}
void Processor::RES4a() {AF.higher &=0xEF; m_clock = 2;}
void Processor::RES4m() {uint8_t result = mmu->ReadByte(HL.word)&0xEF; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::SET4b() {BC.higher |= 0x10; m_clock = 2;}
void Processor::SET4c() {BC.lower  |= 0x10; m_clock = 2;}
void Processor::SET4d() {DE.higher |= 0x10; m_clock = 2;}
void Processor::SET4e() {DE.lower  |= 0x10; m_clock = 2;}
void Processor::SET4h() {HL.higher |= 0x10; m_clock = 2;}
void Processor::SET4l() {HL.lower  |= 0x10; m_clock = 2;}
void Processor::SET4a() {AF.higher |= 0x10; m_clock = 2;}
void Processor::SET4m() {uint8_t result = mmu->ReadByte(HL.word)|0x10; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::BIT5b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x20)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT5c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x20)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT5d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x20)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT5e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x20)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT5h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x20)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT5l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x20)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT5a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x20)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT5m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x20)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES5b() {BC.higher &=0xDF; m_clock = 2;}
void Processor::RES5c() {BC.lower  &=0xDF; m_clock = 2;}
void Processor::RES5d() {DE.higher &=0xDF; m_clock = 2;}
void Processor::RES5e() {DE.lower  &=0xDF; m_clock = 2;}
void Processor::RES5h() {HL.higher &=0xDF; m_clock = 2;}
void Processor::RES5l() {HL.lower  &=0xDF; m_clock = 2;}
void Processor::RES5a() {AF.higher &=0xDF; m_clock = 2;}
void Processor::RES5m() {uint8_t result = mmu->ReadByte(HL.word)&0xDF; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::SET5b() {BC.higher |= 0x20; m_clock = 2;}
void Processor::SET5c() {BC.lower  |= 0x20; m_clock = 2;}
void Processor::SET5d() {DE.higher |= 0x20; m_clock = 2;}
void Processor::SET5e() {DE.lower  |= 0x20; m_clock = 2;}
void Processor::SET5h() {HL.higher |= 0x20; m_clock = 2;}
void Processor::SET5l() {HL.lower  |= 0x20; m_clock = 2;}
void Processor::SET5a() {AF.higher |= 0x20; m_clock = 2;}
void Processor::SET5m() {uint8_t result = mmu->ReadByte(HL.word)|0x20; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::BIT6b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x40)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT6c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x40)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT6d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x40)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT6e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x40)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT6h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x40)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT6l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x40)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT6a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x40)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT6m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x40)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES6b() {BC.higher &=0xBF; m_clock = 2;}
void Processor::RES6c() {BC.lower  &=0xBF; m_clock = 2;}
void Processor::RES6d() {DE.higher &=0xBF; m_clock = 2;}
void Processor::RES6e() {DE.lower  &=0xBF; m_clock = 2;}
void Processor::RES6h() {HL.higher &=0xBF; m_clock = 2;}
void Processor::RES6l() {HL.lower  &=0xBF; m_clock = 2;}
void Processor::RES6a() {AF.higher &=0xBF; m_clock = 2;}
void Processor::RES6m() {uint8_t result = mmu->ReadByte(HL.word)&0xBF; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::SET6b() {BC.higher |= 0x40; m_clock = 2;}
void Processor::SET6c() {BC.lower  |= 0x40; m_clock = 2;}
void Processor::SET6d() {DE.higher |= 0x40; m_clock = 2;}
void Processor::SET6e() {DE.lower  |= 0x40; m_clock = 2;}
void Processor::SET6h() {HL.higher |= 0x40; m_clock = 2;}
void Processor::SET6l() {HL.lower  |= 0x40; m_clock = 2;}
void Processor::SET6a() {AF.higher |= 0x40; m_clock = 2;}
void Processor::SET6m() {uint8_t result = mmu->ReadByte(HL.word)|0x40; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::BIT7b() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.higher&0x80)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT7c() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(BC.lower &0x80)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT7d() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.higher&0x80)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT7e() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(DE.lower &0x80)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT7h() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.higher&0x80)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT7l() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(HL.lower &0x80)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT7a() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!(AF.higher&0x80)) AF.lower |= 0x80; m_clock = 2;}
void Processor::BIT7m() {AF.lower &= 0x1F; AF.lower |= 0x20; if(!((mmu->ReadByte(HL.word))&0x80)) AF.lower |= 0x80; m_clock = 3;}

void Processor::RES7b() {BC.higher &=0x7F; m_clock = 2;}
void Processor::RES7c() {BC.lower  &=0x7F; m_clock = 2;}
void Processor::RES7d() {DE.higher &=0x7F; m_clock = 2;}
void Processor::RES7e() {DE.lower  &=0x7F; m_clock = 2;}
void Processor::RES7h() {HL.higher &=0x7F; m_clock = 2;}
void Processor::RES7l() {HL.lower  &=0x7F; m_clock = 2;}
void Processor::RES7a() {AF.higher &=0x7F; m_clock = 2;}
void Processor::RES7m() {uint8_t result = mmu->ReadByte(HL.word)&0x7F; mmu->WriteByte(HL.word, result); m_clock = 4;}

void Processor::SET7b() {BC.higher |= 0x80; m_clock = 2;}
void Processor::SET7c() {BC.lower  |= 0x80; m_clock = 2;}
void Processor::SET7d() {DE.higher |= 0x80; m_clock = 2;}
void Processor::SET7e() {DE.lower  |= 0x80; m_clock = 2;}
void Processor::SET7h() {HL.higher |= 0x80; m_clock = 2;}
void Processor::SET7l() {HL.lower  |= 0x80; m_clock = 2;}
void Processor::SET7a() {AF.higher |= 0x80; m_clock = 2;}
void Processor::SET7m() {uint8_t result = mmu->ReadByte(HL.word)|0x80; mmu->WriteByte(HL.word, result); m_clock = 4;}

// Rotate A register Note: Missing Zero flag?
void Processor::RLA() {uint8_t carry = (AF.lower&0x10)?1:0; AF.lower = (AF.higher&0x80)?0x10:0; AF.higher = (AF.higher<<1) | carry; m_clock = 1;}
void Processor::RLCA() {AF.lower = (AF.higher&0x80)?0x10:0; AF.higher = (AF.higher<<1) | (AF.higher>>7); m_clock = 1;}
void Processor::RRA() {uint8_t carry = (AF.lower&0x10)?0x80:0; AF.lower = (AF.higher&0x1)?0x10:0; AF.higher = (AF.higher>>1) | carry; m_clock = 1;}
void Processor::RRCA() {AF.lower = (AF.higher&0x1)?0x10:0; AF.higher = (AF.higher>>1) | ((AF.higher&0x1)<<7); m_clock = 1;}

// Rotate register left
void Processor::RLr_b() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = BC.higher&0x80?0x10:0; BC.higher = (BC.higher << 1) + carry_in; AF.lower = BC.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLr_c() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = BC.lower &0x80?0x10:0; BC.lower  = (BC.lower  << 1) + carry_in; AF.lower = BC.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLr_d() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = DE.higher&0x80?0x10:0; DE.higher = (DE.higher << 1) + carry_in; AF.lower = DE.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLr_e() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = DE.lower &0x80?0x10:0; DE.lower  = (DE.lower  << 1) + carry_in; AF.lower = DE.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLr_h() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = HL.higher&0x80?0x10:0; HL.higher = (HL.higher << 1) + carry_in; AF.lower = HL.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLr_l() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = HL.lower &0x80?0x10:0; HL.lower  = (HL.lower  << 1) + carry_in; AF.lower = HL.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLr_a() {uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = AF.higher&0x80?0x10:0; AF.higher = (AF.higher << 1) + carry_in; AF.lower = AF.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry_in = AF.lower&0x10?1:0; uint8_t carry_out = memory_value&0x80?0x10:0; memory_value = (memory_value << 1) + carry_in; AF.lower = memory_value?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

void Processor::RLCr_b() {uint8_t carry_in = BC.higher&0x80?1:0; uint8_t carry_out = BC.higher&0x80?0x10:0; BC.higher = (BC.higher << 1) + carry_in; AF.lower = BC.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLCr_c() {uint8_t carry_in = BC.lower &0x80?1:0; uint8_t carry_out = BC.lower &0x80?0x10:0; BC.lower  = (BC.lower  << 1) + carry_in; AF.lower = BC.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLCr_d() {uint8_t carry_in = DE.higher&0x80?1:0; uint8_t carry_out = DE.higher&0x80?0x10:0; DE.higher = (DE.higher << 1) + carry_in; AF.lower = DE.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLCr_e() {uint8_t carry_in = DE.lower &0x80?1:0; uint8_t carry_out = DE.lower &0x80?0x10:0; DE.lower  = (DE.lower  << 1) + carry_in; AF.lower = DE.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLCr_h() {uint8_t carry_in = HL.higher&0x80?1:0; uint8_t carry_out = HL.higher&0x80?0x10:0; HL.higher = (HL.higher << 1) + carry_in; AF.lower = HL.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLCr_l() {uint8_t carry_in = HL.lower &0x80?1:0; uint8_t carry_out = HL.lower &0x80?0x10:0; HL.lower  = (HL.lower  << 1) + carry_in; AF.lower = HL.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLCr_a() {uint8_t carry_in = AF.higher&0x80?1:0; uint8_t carry_out = AF.higher&0x80?0x10:0; AF.higher = (AF.higher << 1) + carry_in; AF.lower = AF.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RLCHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry_in = memory_value&0x80?1:0; uint8_t carry_out = memory_value&0x80?0x10:0; memory_value = (memory_value << 1) + carry_in; AF.lower = memory_value?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

// Rotate register right
void Processor::RRr_b() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = BC.higher&1?0x10:0; BC.higher = (BC.higher >> 1) + carry_in; AF.lower = BC.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRr_c() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = BC.lower &1?0x10:0; BC.lower  = (BC.lower  >> 1) + carry_in; AF.lower = BC.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRr_d() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = DE.higher&1?0x10:0; DE.higher = (DE.higher >> 1) + carry_in; AF.lower = DE.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRr_e() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = DE.lower &1?0x10:0; DE.lower  = (DE.lower  >> 1) + carry_in; AF.lower = DE.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRr_h() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = HL.higher&1?0x10:0; HL.higher = (HL.higher >> 1) + carry_in; AF.lower = HL.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRr_l() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = HL.lower &1?0x10:0; HL.lower  = (HL.lower  >> 1) + carry_in; AF.lower = HL.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRr_a() {uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = AF.higher&1?0x10:0; AF.higher = (AF.higher >> 1) + carry_in; AF.lower = AF.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry_in = AF.lower&0x10?0x80:0; uint8_t carry_out = memory_value&1?0x10:0; memory_value = (memory_value >> 1) + carry_in; AF.lower = memory_value?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

void Processor::RRCr_b() {uint8_t carry_in = BC.higher&1?0x80:0; uint8_t carry_out = BC.higher&1?0x10:0; BC.higher = (BC.higher >> 1) + carry_in; AF.lower = BC.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRCr_c() {uint8_t carry_in = BC.lower &1?0x80:0; uint8_t carry_out = BC.lower &1?0x10:0; BC.lower  = (BC.lower  >> 1) + carry_in; AF.lower = BC.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRCr_d() {uint8_t carry_in = DE.higher&1?0x80:0; uint8_t carry_out = DE.higher&1?0x10:0; DE.higher = (DE.higher >> 1) + carry_in; AF.lower = DE.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRCr_e() {uint8_t carry_in = DE.lower &1?0x80:0; uint8_t carry_out = DE.lower &1?0x10:0; DE.lower  = (DE.lower  >> 1) + carry_in; AF.lower = DE.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRCr_h() {uint8_t carry_in = HL.higher&1?0x80:0; uint8_t carry_out = HL.higher&1?0x10:0; HL.higher = (HL.higher >> 1) + carry_in; AF.lower = HL.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRCr_l() {uint8_t carry_in = HL.lower &1?0x80:0; uint8_t carry_out = HL.lower &1?0x10:0; HL.lower  = (HL.lower  >> 1) + carry_in; AF.lower = HL.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRCr_a() {uint8_t carry_in = AF.higher&1?0x80:0; uint8_t carry_out = AF.higher&1?0x10:0; AF.higher = (AF.higher >> 1) + carry_in; AF.lower = AF.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::RRCHL() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry_in = memory_value&1?0x80:0; uint8_t carry_out = memory_value&1?0x10:0; memory_value = (memory_value >> 1) + carry_in; AF.lower = memory_value?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

void Processor::SLAr_b() {uint8_t carry_out = BC.higher&0x80?0x10:0; BC.higher = BC.higher << 1; AF.lower = BC.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SLAr_c() {uint8_t carry_out = BC.lower &0x80?0x10:0; BC.lower  = BC.lower  << 1; AF.lower = BC.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SLAr_d() {uint8_t carry_out = DE.higher&0x80?0x10:0; DE.higher = DE.higher << 1; AF.lower = DE.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SLAr_e() {uint8_t carry_out = DE.lower &0x80?0x10:0; DE.lower  = DE.lower  << 1; AF.lower = DE.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SLAr_h() {uint8_t carry_out = HL.higher&0x80?0x10:0; HL.higher = HL.higher << 1; AF.lower = HL.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SLAr_l() {uint8_t carry_out = HL.lower &0x80?0x10:0; HL.lower  = HL.lower  << 1; AF.lower = HL.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SLAr_a() {uint8_t carry_out = AF.higher&0x80?0x10:0; AF.higher = AF.higher << 1; AF.lower = AF.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SLAHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry_out = memory_value&0x80?0x10:0; memory_value = memory_value << 1; AF.lower = memory_value?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

//void Processor::SLLr_b
//void Processor::SLLr_c
//void Processor::SLLr_d
//void Processor::SLLr_e
//void Processor::SLLr_h
//void Processor::SLLr_l
//void Processor::SLLr_a

void Processor::SRAr_b() {AF.lower = (BC.higher&0x1)?0x10:0; BC.higher = (BC.higher&0x80) | (BC.higher>>1); if(!BC.higher) AF.lower |= 0x80; m_clock = 2;}
void Processor::SRAr_c() {AF.lower = (BC.lower &0x1)?0x10:0; BC.lower  = (BC.lower &0x80) | (BC.lower >>1); if(!BC.lower ) AF.lower |= 0x80; m_clock = 2;}
void Processor::SRAr_d() {AF.lower = (DE.higher&0x1)?0x10:0; DE.higher = (DE.higher&0x80) | (DE.higher>>1); if(!DE.higher) AF.lower |= 0x80; m_clock = 2;}
void Processor::SRAr_e() {AF.lower = (DE.lower &0x1)?0x10:0; DE.lower  = (DE.lower &0x80) | (DE.lower >>1); if(!DE.lower ) AF.lower |= 0x80; m_clock = 2;}
void Processor::SRAr_h() {AF.lower = (HL.higher&0x1)?0x10:0; HL.higher = (HL.higher&0x80) | (HL.higher>>1); if(!HL.higher) AF.lower |= 0x80; m_clock = 2;}
void Processor::SRAr_l() {AF.lower = (HL.lower &0x1)?0x10:0; HL.lower  = (HL.lower &0x80) | (HL.lower >>1); if(!HL.lower ) AF.lower |= 0x80; m_clock = 2;}
void Processor::SRAr_a() {AF.lower = (AF.higher&0x1)?0x10:0; AF.higher = (AF.higher&0x80) | (AF.higher>>1); if(!AF.higher) AF.lower |= 0x80; m_clock = 2;}
void Processor::SRAHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); AF.lower = (memory_value&0x1)?0x10:0; memory_value = (memory_value&0x80) | (memory_value>>1); if(!memory_value) AF.lower |= 0x80; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

void Processor::SRLr_b() {uint8_t carry_out = BC.higher&1?0x10:0; BC.higher = BC.higher>>1; AF.lower = BC.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SRLr_c() {uint8_t carry_out = BC.lower &1?0x10:0; BC.lower  = BC.lower >>1; AF.lower = BC.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SRLr_d() {uint8_t carry_out = DE.higher&1?0x10:0; DE.higher = DE.higher>>1; AF.lower = DE.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SRLr_e() {uint8_t carry_out = DE.lower &1?0x10:0; DE.lower  = DE.lower >>1; AF.lower = DE.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SRLr_h() {uint8_t carry_out = HL.higher&1?0x10:0; HL.higher = HL.higher>>1; AF.lower = HL.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SRLr_l() {uint8_t carry_out = HL.lower &1?0x10:0; HL.lower  = HL.lower >>1; AF.lower = HL.lower ?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SRLr_a() {uint8_t carry_out = AF.higher&1?0x10:0; AF.higher = AF.higher>>1; AF.lower = AF.higher?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; m_clock = 2;}
void Processor::SRLHLm() {uint8_t memory_value = mmu->ReadByte(HL.word); uint8_t carry_out = memory_value&1?0x10:0; memory_value = memory_value>>1; AF.lower = memory_value?0:0x80; AF.lower = (AF.lower&0xEF) + carry_out; mmu->WriteByte(HL.word, memory_value); m_clock = 4;}

// Note: This is a great example of where Nazar's code differs from the documentation. Instead of checking for zero, should just set N and H flags. Need to double check opcodes!!!
void Processor::CPL() {AF.higher ^= 0xFF; AF.lower |= 0x60; m_clock = 1;}
//void Processor::NEG() {}

// Complement carry flag and Set carry flag
void Processor::CCF() {uint8_t carry_flag = AF.lower&0x10; carry_flag ^= 0xFF; AF.lower &= 0x80; AF.lower |= (carry_flag&0x10); m_clock = 1;}
void Processor::SCF() {AF.lower &= 0x80; AF.lower |= 0x10; m_clock = 1;}

// Stack
void Processor::PUSHBC() {--stack_pointer.word; mmu->WriteByte(stack_pointer.word--, BC.higher); mmu->WriteByte(stack_pointer.word, BC.lower); m_clock = 3;}
void Processor::PUSHDE() {--stack_pointer.word; mmu->WriteByte(stack_pointer.word--, DE.higher); mmu->WriteByte(stack_pointer.word, DE.lower); m_clock = 3;}
void Processor::PUSHHL() {--stack_pointer.word; mmu->WriteByte(stack_pointer.word--, HL.higher); mmu->WriteByte(stack_pointer.word, HL.lower); m_clock = 3;}
void Processor::PUSHAF() {--stack_pointer.word; mmu->WriteByte(stack_pointer.word--, AF.higher); mmu->WriteByte(stack_pointer.word, AF.lower); m_clock = 3;}

void Processor::POPBC() {BC.lower = mmu->ReadByte(stack_pointer.word++); BC.higher = mmu->ReadByte(stack_pointer.word++); m_clock = 3;}
void Processor::POPDE() {DE.lower = mmu->ReadByte(stack_pointer.word++); DE.higher = mmu->ReadByte(stack_pointer.word++); m_clock = 3;}
void Processor::POPHL() {HL.lower = mmu->ReadByte(stack_pointer.word++); HL.higher = mmu->ReadByte(stack_pointer.word++); m_clock = 3;}
void Processor::POPAF() {AF.lower = mmu->ReadByte(stack_pointer.word++)&0xF0; AF.higher = mmu->ReadByte(stack_pointer.word++); m_clock = 3;}

// Jump
void Processor::JPnn() {program_counter.word = mmu->ReadWord(program_counter.word); m_clock = 3;}
void Processor::JPHL() {program_counter.word = HL.word; m_clock = 1;}
void Processor::JPNZnn() {m_clock = 3; if (!(AF.lower&0x80)) {program_counter.word = mmu->ReadWord(program_counter.word); ++m_clock;} else program_counter.word+=2;}
void Processor::JPZnn() {m_clock = 3; if (AF.lower&0x80) {program_counter.word = mmu->ReadWord(program_counter.word); ++m_clock;} else program_counter.word+=2;}
void Processor::JPNCnn() {m_clock = 3; if (!(AF.lower&0x10)) {program_counter.word = mmu->ReadWord(program_counter.word); ++m_clock;} else program_counter.word+=2;}
void Processor::JPCnn() {m_clock = 3; if (AF.lower&0x10) {program_counter.word = mmu->ReadWord(program_counter.word); ++m_clock;} else program_counter.word+=2;}

// Jump by adding signed value to program counter (make sure the conversion with int16_t is right!!!!!!!!)
void Processor::JRn() {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2; program_counter.word += memory_value; if (memory_value > 127) program_counter.word -= 256; ++m_clock;}
void Processor::JRNZn() {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2; if(!(AF.lower&0x80)) {program_counter.word += memory_value; if (memory_value > 127) program_counter.word -= 256; ++m_clock;}}
void Processor::JRZn() {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2; if(AF.lower&0x80) {program_counter.word += memory_value; if (memory_value > 127) program_counter.word -= 256; ++m_clock;}}
void Processor::JRNCn()  {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2; if(!(AF.lower&0x10)) {program_counter.word += memory_value; if (memory_value > 127) program_counter.word -= 256; ++m_clock;}}
void Processor::JRCn() {uint8_t memory_value = mmu->ReadByte(program_counter.word); ++program_counter.word; m_clock = 2; if(AF.lower&0x10) {program_counter.word += memory_value; if (memory_value > 127) program_counter.word -= 256; ++m_clock;}}

// Todo: Implement this
void Processor::STOP() {}

// Note: 5 instead of 3 cycles? Need to see the extra 2 cycles are true or just an implementation rule
void Processor::CALLnn() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word+2); program_counter.word = mmu->ReadWord(program_counter.word); m_clock = 5;}
void Processor::CALLNZnn() {m_clock = 3; if (!(AF.lower&0x80)) {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word+2); program_counter.word = mmu->ReadWord(program_counter.word); m_clock += 2;} else program_counter.word += 2;}
void Processor::CALLZnn() {m_clock = 3; if (AF.lower&0x80) {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word+2); program_counter.word = mmu->ReadWord(program_counter.word); m_clock += 2;} else program_counter.word += 2;}
void Processor::CALLNCnn() {m_clock = 3; if (!(AF.lower&0x10)) {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word+2); program_counter.word = mmu->ReadWord(program_counter.word); m_clock += 2;} else program_counter.word += 2;}
void Processor::CALLCnn() {m_clock = 3; if (AF.lower&0x10) {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word+2); program_counter.word = mmu->ReadWord(program_counter.word); m_clock += 2;} else program_counter.word += 2;}

void Processor::RET() {program_counter.word = mmu->ReadWord(stack_pointer.word); stack_pointer.word += 2; m_clock = 3;}
void Processor::RETI() {interrupt_master_enable = 1; program_counter.word = mmu->ReadWord(stack_pointer.word); stack_pointer.word += 2; m_clock = 3;} // InterruptReturn();  removed this?
void Processor::RETNZ() {m_clock = 1; if(!(AF.lower&0x80)) {program_counter.word = mmu->ReadWord(stack_pointer.word); stack_pointer.word += 2; m_clock += 2;}}
void Processor::RETZ() {m_clock = 1; if((AF.lower&0x80)) {program_counter.word = mmu->ReadWord(stack_pointer.word); stack_pointer.word += 2; m_clock += 2;}}
void Processor::RETNC() {m_clock = 1; if(!(AF.lower&0x10)) {program_counter.word = mmu->ReadWord(stack_pointer.word); stack_pointer.word += 2; m_clock += 2;}}
void Processor::RETC() {m_clock = 1; if((AF.lower&0x10)) {program_counter.word = mmu->ReadWord(stack_pointer.word); stack_pointer.word += 2; m_clock += 2;}}

void Processor::RST00() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x00; m_clock = 3;}
void Processor::RST08() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x08; m_clock = 3;}
void Processor::RST10() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x10; m_clock = 3;}
void Processor::RST18() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x18; m_clock = 3;}
void Processor::RST20() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x20; m_clock = 3;}
void Processor::RST28() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x28; m_clock = 3;}
void Processor::RST30() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x30; m_clock = 3;}
void Processor::RST38() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x38; m_clock = 3;}
void Processor::RST40() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x40; m_clock = 3;}
void Processor::RST48() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x48; m_clock = 3;}
void Processor::RST50() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x50; m_clock = 3;}
void Processor::RST58() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x58; m_clock = 3;}
void Processor::RST60() {stack_pointer.word -= 2; mmu->WriteWord(stack_pointer.word, program_counter.word); program_counter.word = 0x60; m_clock = 3;}

void Processor::NOP() {m_clock = 1;}
void Processor::HALT() {if (!interrupt_master_enable) {program_counter.word +=1;} else halt = 1; m_clock = 1;}

// Todo: Determine if interrupt master or IE is enabled
void Processor::DI() {interrupt_master_enable = 0; m_clock = 1;} //mmu->interrupt_enable = 0;
void Processor::EI() {interrupt_master_enable = 1; m_clock = 1;} //mmu->interrupt_enable = 1;

void Processor::MAPcb() {uint8_t memory_value = mmu->ReadByte(program_counter.word++); cb_opcode_map[memory_value]();}