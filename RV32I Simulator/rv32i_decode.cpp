//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 5
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"

 /**
 *
 * @brief Function to decode instruction
 *
 * @param addr Address of instruction
 *
 * @param insn Instruction
 *
 ********************************************************************************/

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    // Declaring get functions for funct3 and funct7
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);

    // Check that instruction == insn_ecall
    if (insn == insn_ecall)
    {
        return render_ecall(insn);
    }
    // Check that instruction == insn_ebreak
    else if(insn == insn_ebreak)
    {
        return render_ebreak(insn);
    }

    // Instruction opcode's
    switch (get_opcode(insn))
    {
        default:			    return render_illegal_insn(insn);
        case opcode_lui:		return render_lui(insn);
        case opcode_auipc:		return render_auipc(insn);
        case opcode_jal:		return render_jal(addr, insn);
        case opcode_jalr:		return render_jalr(insn);

        case opcode_btype:
        switch (funct3)
        {
            default:		    return render_illegal_insn(insn);
	        case funct3_beq:	return render_btype(addr, insn, "beq");
	        case funct3_bne:	return render_btype(addr, insn, "bne");
	        case funct3_blt:	return render_btype(addr, insn, "blt");
	        case funct3_bge:	return render_btype(addr, insn, "bge");
	        case funct3_bltu:	return render_btype(addr, insn, "bltu");
	        case funct3_bgeu:	return render_btype(addr, insn, "bgeu");
        }
        assert (0 && "unrecognized funct3");    // It should be impossible to ever get here!

        case opcode_load_imm:
        switch (funct3)
        {
            default:		    return render_illegal_insn(insn);
            case funct3_lb:		return render_itype_load(insn, "lb");
	        case funct3_lh:		return render_itype_load(insn, "lh");
	        case funct3_lw:		return render_itype_load(insn, "lw");
	        case funct3_lbu:	return render_itype_load(insn, "lbu");
	        case funct3_lhu:	return render_itype_load(insn, "lhu");
        }
        assert (0 && "unrecognized funct3");    // It should be impossible to ever get here!

        case opcode_stype:
        switch (funct3)
        {
            default:		    return render_illegal_insn(insn);
	        case funct3_sb:		return render_stype(insn, "sb");
	        case funct3_sh:		return render_stype(insn, "sh");
	        case funct3_sw:		return render_stype(insn, "sw");
        }
        assert (0 && "unrecognized funct3");    // It should be impossible to ever get here!

        case opcode_alu_imm:
        switch (funct3)
        {
	        default:		    return render_illegal_insn(insn);
	        case funct3_add:	return render_itype_alu(insn, "addi", get_imm_i(insn));
	        case funct3_slt:	return render_itype_alu(insn, "slti", get_imm_i(insn));
	        case funct3_sltu:	return render_itype_alu(insn, "sltiu", get_imm_i(insn));
	        case funct3_xor:	return render_itype_alu(insn, "xori", get_imm_i(insn));
	        case funct3_or:		return render_itype_alu(insn, "ori", get_imm_i(insn));
	        case funct3_and:	return render_itype_alu(insn, "andi", get_imm_i(insn));
	        case funct3_sll:	return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);

	        case funct3_srx:
	        switch (funct7)
	        {
	            default:		    return render_illegal_insn(insn);
	            case funct7_srl:	return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
	            case funct7_sra:	return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
	        }
	        assert(0 && "unrecognized funct7"); // It should be impossible to ever get here!
        }
        assert (0 && "unrecognized funct3");    // It should be impossible to ever get here!

        case opcode_rtype:
        switch (funct3)
        {
	        default:		        return render_illegal_insn(insn);

            case funct3_add:
	        switch (funct7)
	        {
	            default:		    return render_illegal_insn(insn);
	            case funct7_add:	return render_rtype(insn, "add");
	            case funct7_sub:	return render_rtype(insn, "sub");
	        }
	        assert (0 && "unrecognized funct7");    // It should be impossible to ever get here!

	        case funct3_sll:	    return render_rtype(insn, "sll");
	        case funct3_slt:	    return render_rtype(insn, "slt");
	        case funct3_sltu:	    return render_rtype(insn, "sltu");
	        case funct3_xor:	    return render_rtype(insn, "xor");

	        case funct3_srx:
	        switch (funct7)
	        {
                default:		    return render_illegal_insn(insn);
	            case funct7_srl:	return render_rtype(insn, "srl");
	            case funct7_sra:	return render_rtype(insn, "sra");
	        }
	        assert (0 && "unrecognized funct7");    // It should be impossible to ever get here!
	        case funct3_or:		    return render_rtype(insn, "or");
	        case funct3_and:	    return render_rtype(insn, "and");
        }
        assert (0 && "unrecognized funct3");    // It should be impossible to ever get here!

        case opcode_system:
        switch (funct3)
        {
	        default:                return render_illegal_insn(insn);
	        case funct3_csrrw:  	return render_csrrx(insn, "csrrw");
	        case funct3_csrrs:  	return render_csrrx(insn, "csrrs");
	        case funct3_csrrc:	    return render_csrrx(insn, "csrrc");
	        case funct3_csrrwi:	    return render_csrrxi(insn, "csrrwi");
	        case funct3_csrrsi:	    return render_csrrxi(insn, "csrrsi");
	        case funct3_csrrci:	    return render_csrrxi(insn, "csrrci");
        }
        assert (0 && "unrecognized funct3");    // It should be impossible to ever get here!
    }
    assert (0 && "unrecognized opcode");    // It should be impossible to ever get here!
}

 /**
  *
  * @brief Function to return opcode
  * 
  * @param insn Instruction
  * 
  * @return 7 bits
  *
  ********************************************************************************/

uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
    return (insn & 0x0000007f);
}

 /**
  *
  * @brief Function to return rd
  *
  * @param insn Instruction
  *
  * @return Bits 7-11
  *
  ********************************************************************************/

uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    return (insn & 0x00000f80) >> 7;
}

 /**
  *
  * @brief Function to return rs1
  *
  * @param insn Instruction
  *
  * @return Bits 15-19
  *
  ********************************************************************************/

uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    return (insn & 0x000f8000) >> 15;
}

 /**
  *
  * @brief Function to return rs2
  *
  * @param insn Instruction
  *
  * @return Bits 20-24
  *
  ********************************************************************************/

uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    return (insn & 0x01f00000) >> 20;
}

 /**
  *
  * @brief Function to render an illegal instruction
  *
  * @param insn Instruction
  *
  * @return Error
  *
  ********************************************************************************/

std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

 /**
  *
  * @brief Function to render auipc
  *
  * @param insn Instruction
  *
  * @return "auipc" + rd, imm_u
  *
  ********************************************************************************/

std::string rv32i_decode::render_auipc(uint32_t insn)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic("auipc") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
    return os.str();
}

 /**
  *
  * @brief Function to render jal
  *
  * @param addr Address
  * 
  * @param insn Instruction
  *
  * @return "jal" + rd, pcrel_21
  *
  ********************************************************************************/

std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);
    int32_t pcrel_21 = addr + imm_j;

    // Assign string
    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << "," << hex::to_hex0x32(pcrel_21);
    return os.str();
}

 /**
  *
  * @brief Function to render jalr
  * 
  * @param insn Instruction
  *
  * @return "jalr" + rd, rs1, imm_i
  *
  ********************************************************************************/

std::string rv32i_decode::render_jalr(uint32_t insn)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
    return os.str();
}

 /**
  *
  * @brief Function to render btype
  *
  * @param addr Address
  *
  * @param insn Instruction
  * 
  * @param mnemonic Processor
  *
  * @return rs1, rs2, and pcrel_13
  *
  ********************************************************************************/

std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    int32_t pcrel_13 = addr + imm_b;

    // Assign string
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << hex::to_hex0x32(pcrel_13);
    return os.str();
}

 /**
  *
  * @brief Function to render itype
  *
  * @param insn Instruction
  *
  * @return rd, rs1, imm_i
  *
  ********************************************************************************/

 std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
    return os.str();
}

 /**
 *
 * @brief Function to render jal
 *
 * @param addr Address
 *
 * @param insn Instruction
 * 
 * @param mnemonic Processor
 *
 * @return rd, rs1, imm_i
 *
 ********************************************************************************/

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
  
    // Assign string
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;
    return os.str();
}

 /**
  *
  * @brief Function to render stype
  *
  * @param insn Instruction
  * 
  * @param mnemonic Processor
  *
  * @return rs1, rs2, imm_s
  *
  ********************************************************************************/

std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << render_base_disp(rs1, imm_s);
    return os.str();
}

 /**
  *
  * @brief Function to render rtype
  *
  * @param insn Instruction
  * 
  * @param mnemonic Processor
  *
  * @return rd, rs1, rs2
  *
  ********************************************************************************/

std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
    return os.str();
}

 /**
  *
  * @brief Function to render lui
  *
  * @param insn Instruction
  *
  * @return "lui" + rd, imm_u
  *
  ********************************************************************************/

std::string rv32i_decode::render_lui(uint32_t insn)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
    return os.str();
}

 /**
  *
  * @brief Function to render csrrx
  *
  * @param insn Instruction
  * 
  * @param mnemonic Processor
  *
  * @return rd, csr, rs1
  *
  ********************************************************************************/

std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t csr = get_imm_i(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << render_reg(rs1);
    return os.str();
}

/**
 *
 * @brief Function to render csrrxi
 * 
 * @param insn Instruction
 *
 * @param mnemonic Processor
 * 
 * @return rd, csr, zimm
 *
 ********************************************************************************/

 std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize
    uint32_t rd = get_rd(insn);
    int32_t csr = get_imm_i(insn);
    uint32_t zimm = get_rs1(insn);

    // Assign string
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << zimm;
    return os.str();
}

 /**
 *
 * @brief Function to render reg
 *
 * @param r Register
 *
 * @return Register
 *
 ********************************************************************************/

std::string rv32i_decode::render_reg(int r)
{
    // Assign string
    std::ostringstream os;
    os << "x" << r;
    return os.str();
}

 /**
 *
 * @brief Function to render base_disp (imm)
 *
 * @param r Register
 * 
 * @param imm Next value
 *
 * @return Register
 *
 ********************************************************************************/

std::string rv32i_decode::render_base_disp(uint32_t r, int32_t imm)
{
    // Assign string
    std::ostringstream os;
    os << imm << "(" << render_reg(r) << ")";
    return os.str();
}

 /**
 *
 * @brief Function to render mnemonic, formatted
 *
 * @param mnemonic Unformatted
 *
 * @return Fromatted mnemonic
 *
 ********************************************************************************/

std::string rv32i_decode::render_mnemonic(const std::string &mnemonic)
{
    // Assign string
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic;
    return os.str();
}

 /**
 *
 * @brief Used for ecalls
 *
 * @param insn Instruction
 *
 * @return ecall
 *
 ********************************************************************************/

std::string rv32i_decode::render_ecall(uint32_t insn)
{
    // Assign string
    std::ostringstream os;
    os << "ecall";
    return os.str();
}

 /**
 *
 * @brief Used for ebreak
 *
 * @param insn Instruction
 *
 * @return ebreak
 *
 ********************************************************************************/

std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    // Assign string
    std::ostringstream os;
    os << "ebreak";
    return os.str();
}

 /**
  *
  * @brief Function to return funct3
  *
  * @param insn Instruction
  *
  * @return Bits 12-14
  *
  ********************************************************************************/

uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    return (insn & 0x00007000) >> 12;
}

 /**
  *
  * @brief Function to return funct7
  *
  * @param insn Instruction
  *
  * @return Bits 25-31
  *
  ********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
    return (insn & 0xfe000000) >> 25;
}

 /**
  *
  * @brief Function to return imm_i
  *
  * @param insn Instruction
  *
  * @return imm_i bits
  *
  ********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
    // Shift right
    int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);

    // SE-MSB
    if (insn & 0x80000000)
    {
        imm_i |= 0xfffff000;
    }
    return imm_i;
}

 /**
  *
  * @brief Function to return imm_u
  *
  * @param insn Instruction
  *
  * @return imm_u bits
  *
  ********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    int32_t imm_u = (insn & 0x00000fff) & 0;
    imm_u |= (insn & 0xfffff000);
    return imm_u;
}

 /**
  *
  * @brief Function to return imm_b
  *
  * @param insn Instruction
  *
  * @return imm_b bits
  *
  ********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0x00000001) & 0;	//the lsb bit is always 0

    // Shift right
    imm_b |= (insn & 0x00000f00) >> (8 - 1);

    // Shift right
    imm_b |= (insn & 0x7e000000) >> (25 - 5);

    // Shift left
    imm_b |= (insn & 0x00000080) << (11 - 7);

    // Shift right
    imm_b |= (insn & 0x80000000) >> (31 - 12);

    // SE-MSB
    if (insn & 0x80000000)
    {
        imm_b |= 0xffffe000;
    }
    return imm_b;
}

 /**
  *
  * @brief Function to return imm_s
  *
  * @param insn Instruction
  *
  * @return imm_s bits
  *
  ********************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
    // Shift right
    int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);
    imm_s |= (insn & 0x00000f80) >> (7 - 0);

    // SE-MSB
    if (insn & 0x80000000)
    {
        imm_s |= 0xfffff000;
    }
    return imm_s;
}

 /**
  *
  * @brief Function to return imm_j
  *
  * @param insn Instruction
  *
  * @return imm_j bits
  *
  ********************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
    int32_t imm_j = (insn & 0x00000001) & 0;

    // Shift right
    imm_j |= (insn & 0x7fe00000) >> (21 - 1);

    // Shift right
    imm_j |= (insn & 0x00100000) >> (20 - 11);

    imm_j |= (insn & 0x000ff000);

    // Shift right
    imm_j |= (insn & 0x80000000) >> (31 - 20);

    // SE-MSB
    if (insn & 0x80000000)
    {
        imm_j |= 0xffe00000;
    }
    return imm_j;
}