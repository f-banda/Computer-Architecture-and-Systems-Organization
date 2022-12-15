//***************************************************************************
//
//  Francisco Banda
//  Z1912220
//  CSCI463 - Spring 2022
//  Assignment 4
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"

/**
 * @brief A function that will decode an instruction
 *
 * @param addr Address of instruction that needs decoding
 * @param insn Instruction itself that will be decoded
 *
 * @return Disassembled instruction
 **********************************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    // Declaring and initializing variables funct3/funct7 as shown in section 7.
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);

    // Check if instruction == insn_ecall
    if (insn == insn_ecall)
    {
        // Render ecall
        return render_ecall(insn);
    }

    // Check if instruction == insn_ebreak
    else if(insn == insn_ebreak)
    {
        // Render ebreak
        return render_ebreak(insn);
    }

    // Switch statement for each instruction opcode
    switch (get_opcode(insn))
    {
        default:			    return render_illegal_insn(insn);
        case opcode_lui:		return render_lui(insn);
        case opcode_auipc:		return render_auipc(insn);
        case opcode_jal:		return render_jal(addr, insn);
        case opcode_jalr:		return render_jalr(insn);

        // Use funct3 to isolate
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

        // Use funct3 to isolate
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

        // Use funct3 to isolate
        case opcode_stype:
        switch (funct3)
        {
            default:		    return render_illegal_insn(insn);
	        case funct3_sb:		return render_stype(insn, "sb");
	        case funct3_sh:		return render_stype(insn, "sh");
	        case funct3_sw:		return render_stype(insn, "sw");
        }
        assert (0 && "unrecognized funct3");    // It should be impossible to ever get here!

        // Use funct3 to isolate
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

	        // Use funct7 to isolate
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

        // Use funct3 to isolate
        case opcode_rtype:
        switch (funct3)
        {
	        default:		        return render_illegal_insn(insn);

	        // Use funct3 to isolate
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

	        // Use funct7 to isolate
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

        // Use funct3 to isolate
        case opcode_system:
        switch (funct3)
        {
            // Use funct3 to isolate
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
 * @brief Extract and return the opcode field from the given instruction
 *
 * @param insn to extract
 *
 * @return Final 7 bits, opcode
 **********************************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
    return (insn & 0x0000007f);
}

/**
 * @brief Extract and return the rd field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits in 7-11, rd field
 **********************************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    return (insn & 0x00000f80) >> 7;
}

/**
 * @brief Extract and return the rs1 field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits in 15-19, rs1 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    return (insn & 0x000f8000) >> 15;
}

/**
 * @brief Extract and return the rs2 field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits in 20-24 of the instruction representing the rs2 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    return (insn & 0x01f00000) >> 20;
}

/**
 * @brief Function to be called when an instruction cannot be processed
 *
 * @return Error, unimplemented instruction
 **********************************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * @brief Function to decode the AUIPC instruction
 *
 * @param insn to be decoded
 *
 * @return Instruction rd and imm_u
 **********************************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic("auipc") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
    return os.str();
}

/**
 * @brief Function to decode the JAL instruction
 *
 * @param addr of instruction
 * @param insn to be decoded
 *
 * @return Instruction rd and pcrel_21
 **********************************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);
    int32_t pcrel_21 = addr + imm_j;

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << "," << hex::to_hex0x32(pcrel_21);
    return os.str();
}

/**
 * @brief Function to decode the JALR instruction
 *
 * @param insn to be decoded
 *
 * @return Instruction rd, rs1, and imm_i
 **********************************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
    return os.str();
}

/**
 * @brief Function to decode B instructions
 *
 * @param addr to be decoded
 * @param insn to be processed
 * @param mnemonic Instruction to be processed
 *
 * @return Instruction rs1, rs2, imm_b, and pcrel_13
 **********************************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize variables
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    int32_t pcrel_13 = addr + imm_b;

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << hex::to_hex0x32(pcrel_13);
    return os.str();
}

/**
 * @brief Function to decode I instructions
 *
 * @param insn to be decoded
 * @param mnemonic Instruction to be processed
 *
 * @return Instruction rd, rs1, imm_i
 **********************************************************************************************/
 std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
    return os.str();
}

/**
 * @brief Function to decode I (ALU) instructions
 *
 * @param insn to be decoded
 * @param mnemonic Instruction to be processed
 * @param imm_i Following value of instruction
 *
 * @return Instruction rd and rs1
 **********************************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
  
    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;
    return os.str();
}

/**
 * @brief Function to decode S instructions
 *
 * @param insn to be decoded
 * @param mnemonic Instruction to be processed
 *
 * @return Instruction rs1, rs2, imm_s
 **********************************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize variables
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << render_base_disp(rs1, imm_s);
    return os.str();
}

/**
 * @brief Function to decode R instructions
 *
 * @param insn to be decoded
 * @param mnemonic Instruction to be processed
 *
 * @return Instruction rd, rs1, rs2
 **********************************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
    return os.str();
}

/**
 * @brief Function to decode LUI instructions
 *
 * @param insn to be decoded
 *
 * @return Instruction rd and imm_u
 **********************************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
    return os.str();
}

/**
 * @brief Function to decode CSRRX instructions
 *
 * @param insn to be decoded
 * @param mnemonic Instruction to be processed
 *
 * @return Instruction rd, rs1, imm_i
 **********************************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t csr = get_imm_i(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << render_reg(rs1);
    return os.str();
}

/**
 * @brief Function to decode CSRRXI instructions
 *
 * @param insn to be decoded
 * @param mnemonic Instruction to be processed
 *
 * @return Instruction rd, imm_i, rs1
 **********************************************************************************************/
 std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    // Declare/Initialize variables
    uint32_t rd = get_rd(insn);
    int32_t csr = get_imm_i(insn);
    uint32_t zimm = get_rs1(insn);

    // Declare and initialize string, then return
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << zimm;
    return os.str();
}

/**
 * @brief Function to render register, r
 *
 * @param r Value
 *
 * @return Register as string
 **********************************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
    // Declare/Initialize string + register
    std::ostringstream os;
    os << "x" << r;
    return os.str();
}

/**
 * @brief Function to render the operands of imm_register
 *
 * @param register to be rendered
 * @param imm the following value
 *
 * @return Operands of form imm(register)
 **********************************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t r, int32_t imm)
{
    //set string to the register in paranthese with a x in front of it and return it
    std::ostringstream os;
    os << imm << "(" << render_reg(r) << ")";
    return os.str();
}

/**
 * @brief Function to render mnemonic with spacing
 *
 * @param mnemonic
 *
 * @return Padded mnemonic
 **********************************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &mnemonic)
{
    // Declare/Initialize string, formatted, with mnemonic
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic;
    return os.str();
}

/**
 * @brief Adds ecall when required
 *
 * @param insn for ecall
 *
 * @return "ecall"
 **********************************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
    // Declare/Initialize string to "ecall"
    std::ostringstream os;
    os << "ecall";
    return os.str();
}

/**
 * @brief Adds ebreak when required
 *
 * @param insn for ebreak
 *
 * @return "ebreak"
 **********************************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    // Declare/Initialize string to "ebreak"
    std::ostringstream os;
    os << "ebreak";
    return os.str();
}

/**
 * @brief Extract and return the funct3 field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits in 12-14, funct3 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    return (insn & 0x00007000) >> 12;
}

/**
 * @brief Extract and return the funct7 field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits in 25-31 of the instruction representing the funct7 field
 **********************************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
    return (insn & 0xfe000000) >> 25;
}

/**
 * @brief Extract and return the imm_i field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits representing imm_i field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
    // Shift bits
    int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);

    // Sign extend MSB
    if (insn & 0x80000000)
    {
        imm_i |= 0xfffff000;
    }
    return imm_i;
}

/**
 * @brief Extract and return the imm_u field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits representing imm_u field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    // Shift bits
    int32_t imm_u = (insn & 0x00000fff) & 0;
    imm_u |= (insn & 0xfffff000);
    return imm_u;
}

/**
 * @brief Extract and return the imm_b field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits representing imm_b field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    // Shift bits
    int32_t imm_b = (insn & 0x00000001) & 0;	//the lsb bit is always 0

    // Right shift 8 to 1
    imm_b |= (insn & 0x00000f00) >> (8 - 1);

    // Right shift 25 to 5
    imm_b |= (insn & 0x7e000000) >> (25 - 5);

    // Left shift 11 to 7
    imm_b |= (insn & 0x00000080) << (11 - 7);

    // Right shift 31 to 12
    imm_b |= (insn & 0x80000000) >> (31 - 12);

    // Sign extended MSB
    if (insn & 0x80000000)
    {
        imm_b |= 0xffffe000;
    }
    return imm_b;
}

/**
 * @brief Extract and return the imm_s field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits representing imm_s field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
    // Shift bits
    int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);
    imm_s |= (insn & 0x00000f80) >> (7 - 0);

    // Sign-extend MSB
    if (insn & 0x80000000)
    {
        imm_s |= 0xfffff000;
    }
    return imm_s;
}

/**
 * @brief Extract and return the imm_j field from the given instruction
 *
 * @param insn to extract
 *
 * @return Bits representing imm_j field
 **********************************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
    // Shift bits
    int32_t imm_j = (insn & 0x00000001) & 0;

    // Right shift 21 to 1
    imm_j |= (insn & 0x7fe00000) >> (21 - 1);

    // Right shift 20 to 11
    imm_j |= (insn & 0x00100000) >> (20 - 11);
    imm_j |= (insn & 0x000ff000);

    // Right shift 31 to 20
    imm_j |= (insn & 0x80000000) >> (31 - 20);

    // Sign extend MSB
    if (insn & 0x80000000)
    {
        imm_j |= 0xffe00000;
    }
    return imm_j;
}