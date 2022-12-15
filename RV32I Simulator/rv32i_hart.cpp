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

#include "rv32i_hart.h"
#include "registerfile.h"
#include "hex.h"
#include "memory.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include <sstream>
#include <getopt.h>

using std::string;
using std::setw;
using std::cout;
using std::endl;

 /**
 *
 * @brief Method that treats every instruction as illegal, and is used to develop
 * rv32i_hart::tick() and cpu_single_hart::run() methods.
 *
 * @param insn
 * 
 * @param pos
 *
 ********************************************************************************/

void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{

    switch (get_opcode(insn))
    {

        case opcode_lui:   exec_lui(insn, pos);     return;
        case opcode_auipc: exec_auipc(insn, pos);   return;
        case opcode_jal:   exec_jal(insn, pos);     return;
        case opcode_jalr:  exec_jalr(insn, pos);    return;

        case opcode_system:
            switch (get_funct3(insn))
            {
                case funct3_beq:
                switch (insn)
                {
                    case insn_ecall:        exec_ecall(pos);                return;
                    case insn_ebreak:       exec_ebreak(pos);               return;
                    default:	            exec_illegal_insn(insn, pos);   return;
                }
                
                break;

                case funct3_csrrs:          exec_csrrs(insn, pos);          return;
                default:	        		exec_illegal_insn(insn, pos);   return;

            }
            
            break;

        case opcode_btype:
            switch (get_funct3(insn))
            {
                case funct3_beq:	        exec_beq(insn, pos);            return;
                case funct3_bne:            exec_bne(insn, pos);            return;
                case funct3_blt:	        exec_blt(insn, pos);            return;
                case funct3_bge:	        exec_bge(insn, pos);            return;
                case funct3_bltu: 	        exec_bltu(insn, pos);           return;
                case funct3_bgeu:	        exec_bgeu(insn, pos);           return;
                default:                    exec_illegal_insn(insn, pos);   return;
            }
            
            break;

        case opcode_load_imm:
            switch (get_funct3(insn))
            {
                case funct3_lb:	        exec_lb(insn, pos);             return;
                case funct3_lh:         exec_lh(insn, pos);             return;
                case funct3_lw:	        exec_lw(insn, pos);             return;
                case funct3_lbu:	    exec_lbu(insn, pos);            return;
                case funct3_lhu: 	    exec_lhu(insn, pos);            return;
                default:                exec_illegal_insn(insn, pos);   return;
            }
            
            break;

        case opcode_stype:
            switch (get_funct3(insn))
            {
                case funct3_sb:	    exec_sb(insn, pos);             return;
                case funct3_sh:     exec_sh(insn, pos);             return;
                case funct3_sw:	    exec_sw(insn, pos);             return;
                default:            exec_illegal_insn(insn, pos);   return;
            }
            
            break;

        case opcode_alu_imm:
            switch (get_funct3(insn))
            {
                case funct3_add:	    exec_addi(insn, pos);   return;
                case funct3_slt:	    exec_slti(insn, pos);   return;
                case funct3_sltu:	    exec_sltiu(insn, pos);  return;
                case funct3_xor:        exec_xori(insn, pos);   return;
                case funct3_or:	    	exec_ori(insn, pos);    return;
                case funct3_sll:	    exec_slli(insn, pos);   return;
                case funct3_and:	    exec_andi(insn, pos);   return;
                case funct3_srx:

                switch (get_funct7(insn))
                {
                    case funct7_srl:    exec_srli(insn, pos);           return;
                    case funct7_sra:    exec_srai(insn, pos);           return;
                    default:            exec_illegal_insn(insn, pos);   return;
                }
                
                break;

                default: exec_illegal_insn(insn, pos); return;

            }
            
            break;

        case opcode_rtype:
            switch (get_funct3(insn))
            {
                case funct3_add:
                switch (get_funct7(insn))
                {
                    case funct7_add: exec_add(insn, pos);           return;
                    case funct7_sub: exec_sub(insn, pos);           return;
                    default: 		 exec_illegal_insn(insn, pos);  return;
                }
            
                break;

                case funct3_sll:	    exec_sll(insn, pos);  return;
                case funct3_slt:        exec_slt(insn, pos);  return;
                case funct3_sltu:	    exec_sltu(insn, pos); return;
                case funct3_xor:	    exec_xor(insn, pos);  return;
                case funct3_and:	    exec_and(insn, pos);  return;
                case funct3_or:	        exec_or(insn, pos);   return;
                case funct3_srx:

                switch (get_funct7(insn))
                {
                    case funct7_srl: exec_srl(insn, pos);           return;
                    case funct7_sra: exec_sra(insn, pos);           return;
                    default:         exec_illegal_insn(insn, pos);  return;
                }
            
                break;

                default: exec_illegal_insn(insn, pos); return;
            }

            break;
    }
}

 /**
 *
 * @brief Function to execute an instruction
 * 
 * @param hdr Formatting
 * 
 * @return insn, formatted
 *
 ********************************************************************************/

void rv32i_hart::tick(const string& hdr)
{
    // Increment counter
    insn_counter += 1;

    // Dump
    if (show_registers) regs.dump("");

    // Format
    if (show_registers)
    {
        cout << " pc " << hex::to_hex32(pc) << endl;
    }

    uint32_t insn = mem.get32(pc);

    if (show_instructions)
    {
        cout << hdr << setw(8) << std::setfill('0') << std::right << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << "  ";
        exec(insn, &cout);
    }
    else
    {
        exec(insn, nullptr);
    }
}

 /**
 *
 * @brief Print out GP-regs and pc register
 *
 * @param hdr Formatting
 *
 ********************************************************************************/

void rv32i_hart::dump(const string& hdr) const
{
    regs.dump(hdr);
    cout << " pc " << hex::to_hex32(pc) << endl;
}

 /**
 *
 * @brief Function to reset regs, pc, and counter
 *
 ********************************************************************************/

void rv32i_hart::reset()
{
    regs.reset();

    // Reset variables
    insn_counter = 0;
    halt = false;
    pc = 0;
}

 /**
 *
 * @brief Execute ecall
 *
 ********************************************************************************/
void rv32i_hart::exec_ecall(std::ostream* pos)
{
    if (pos)
    {
        // Initialize string "s"
        string s = render_ecall(0);
        *pos << setw(instruction_width) << std::setfill(' ') << std::left << s << "// ECALL" << endl;

    }

    // Re-assign
    halt = true;
}

 /**
 *
 * @brief Execute ebreak
 *
 ********************************************************************************/
void rv32i_hart::exec_ebreak(std::ostream* pos)
{
    if (pos)
    {
        // Initialize string "s"
        string s = render_ebreak(0);
        *pos << setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// HALT" << endl;
    }

    // Re-assign
    halt = true;

    // Assign reason
    halt_reason = "EBREAK instruction";
}

/**
*
* @brief Execute illegal instruction
*
********************************************************************************/

void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
    // Re-assign
    halt = true;

    // Assign reason
    halt_reason = " Illegal instruction ";

    if (pos != nullptr)
    {
        string s = render_illegal_insn(insn);
        s.resize(instruction_width, ' ');
        *pos << s << ": " << hex::to_hex0x32(insn);
    }
}

 /**
 *
 * @brief Execute lui instruction
 * 
 * @param insn Instruction (address)
 * 
 * @param pos Output
 *
 ********************************************************************************/
void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize imm_u
    int32_t imm_u = get_imm_u(insn);

    // Set register
    regs.set(rd, imm_u);

    // Dump/render
    if (pos)
    {
        string s = render_lui(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(imm_u) << endl;
    }

    // Add
    pc += 4;
}

 /**
 *
 * @brief Execute auipc instruction
 *
 * @param insn Instruction (address)
 *
 * @param pos Output
 *
 ********************************************************************************/

void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize imm_u
    int32_t imm_u = get_imm_u(insn);

    // Set register
    regs.set(rd, pc + imm_u);

    // Dump/render
    if (pos)
    {
        string s = render_auipc(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_u) << " = " << hex::to_hex0x32(imm_u + pc) << endl;
    }

    // Add
    pc += 4;
}

 /**
 *
 * @brief Execute jal instruction
 *
 * @param insn Instruction (address)
 *
 * @param pos Output
 *
 ********************************************************************************/

void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize imm_u
    int32_t imm_j = get_imm_j(insn);

    // Set register
    regs.set(rd, pc + 4);

    // Dump/render
    if (pos)
    {
        string s = render_jal(pc, insn);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(pc + 4) << ",  pc = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(pc + imm_j) << endl;
    }

    // Add imm_j to pc
    pc = pc + imm_j;
}

 /**
 *
 * @brief Execute jalr instruction
 *
 * @param insn Instruction (address)
 *
 * @param pos Output
 *
 ********************************************************************************/

void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Set register
    uint32_t val = (regs.get(rs1) + imm_i) & ~1;

    // Dump/render
    if (pos)
    {
        string s = render_jalr(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(pc + 4) << ",  pc = (" << hex::to_hex0x32(imm_i) << " + " << hex::to_hex0x32(regs.get(rs1)) << ") & " << "0xfffffffe" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, and add value to pc
    regs.set(rd, pc + 4);
    pc = val;
}

/**
*
* @brief Execute beq instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_b
    int32_t imm_b = get_imm_b(insn);

    // Initialize val
    uint32_t val = (regs.get(rs1) == regs.get(rs2)) ? imm_b : 4;

    // Render/Dump
    if (pos)
    {
        string s = render_btype(pc, insn, "beq");
        s.resize(instruction_width, ' ');
        *pos << s << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc + val) << endl;
    }

    // Add value to pc
    pc += val;
}

/**
*
* @brief Execute bne instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_b
    int32_t imm_b = get_imm_b(insn);

    // Initialize val
    uint32_t val = (regs.get(rs1) != regs.get(rs2)) ? imm_b : 4;

    // Render/Dump
    if (pos)
    {
        string s = render_btype(pc, insn, "bne");
        s.resize(instruction_width, ' ');
        *pos << s << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc + val) << endl;
    }

    // Add value to pc
    pc += val;
}

/**
*
* @brief Execute bacon lettuce tomato instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_b
    int32_t imm_b = get_imm_b(insn);

    // Initialize val
    uint32_t val = (regs.get(rs1) < regs.get(rs2)) ? imm_b : 4;

    // Render/Dump
    if (pos)
    {
        string s = render_btype(pc, insn, "blt");
        s.resize(instruction_width, ' ');
        *pos << s << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc + val) << endl;
    }

    // Add value to pc
    pc += val;
}

/**
*
* @brief Execute bge instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_b
    int32_t imm_b = get_imm_b(insn);

    // Initialize val
    uint32_t val = (regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4;

    // Render/Dump
    if (pos) {
        string s = render_btype(pc, insn, "bge");
        s.resize(instruction_width, ' ');
        *pos << s << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc + val) << endl;
    }

    // Add value to pc
    pc += val;
}

/**
*
* @brief Execute bltu instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos)
{

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_b
    int32_t imm_b = get_imm_b(insn);

    // Initialize val
    uint32_t val = ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2)) ? imm_b : 4;;

    // Render/Dump
    if (pos)
    {
        string s = render_btype(pc, insn, "bltu");
        s.resize(instruction_width, ' ');
        *pos << s << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc + val) << endl;
    }

    // Add value to pc
    pc += val;
}

/**
*
* @brief Execute bgeu instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_b
    int32_t imm_b = get_imm_b(insn);

    // Initialize val
    uint32_t val = ((uint32_t)regs.get(rs1) >= (uint32_t)regs.get(rs2)) ? imm_b : 4;

    // Render/Dump
    if (pos)
    {
        string s = render_btype(pc, insn, "bgeu");
        s.resize(instruction_width, ' ');
        *pos << s << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc + val) << endl;
    }

    // Add value to pc
    pc += val;
}

/**
*
* @brief Execute add instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    int32_t val = regs.get(rs1) + regs.get(rs2);

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "add");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute sub instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "sub");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(regs.get(rs1) - regs.get(rs2)) << endl;
    }

    // Set register, add to pc
    regs.set(rd, regs.get(rs1) - regs.get(rs2));
    pc += 4;
}

/**
*
* @brief Execute srl instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = (uint32_t)regs.get(rs1) >> (regs.get(rs2) & 0x01f);

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "srl");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << (uint32_t)regs.get(rs2) % 32 << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute sra instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = regs.get(rs1) >> ((regs.get(rs2) & 0x01f));

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "sra");

        s.resize(instruction_width, ' ');

        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << ((uint32_t)regs.get(rs2)) % 32 << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute sll instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = regs.get(rs1) << ((regs.get(rs2) & 0x01f));

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "sll");

        s.resize(instruction_width, ' ');

        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " << (uint32_t)regs.get(rs2) % 32 << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute slt instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = (regs.get(rs1) < regs.get(rs2) ? 1 : 0);

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "slt");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << ") ? " << "1 " << ": " << "0 " << "= " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute sltu instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2) ? 1 : 0);

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "sltu");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << ") ? " << "1 " << ": " << "0" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute xor instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = ((regs.get(rs1) ^ regs.get(rs2)));

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "xor");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute or instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = regs.get(rs1) | regs.get(rs2);

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "or");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute and instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize val
    uint32_t val = regs.get(rs1) & regs.get(rs2);

    // Render/Dump
    if (pos)
    {
        string s = render_rtype(insn, "and");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute sb instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_s
    int32_t imm_s = get_imm_s(insn);

    // Memory assignment, assign register 
    mem.set8(regs.get(rs1) + imm_s, regs.get(rs2) & 0x000000ff);

    // Render/Dump
    if (pos)
    {
        string s = render_stype(insn, "sb");
        s.resize(instruction_width, ' ');
        *pos << s << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(mem.get8(regs.get(rs1) + imm_s)) << endl;
    }

    // Add to pc
    pc += 4;
}

/**
*
* @brief Execute sh instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);

    // Initialize imm_s
    int32_t imm_s = get_imm_s(insn);

    // Memory assignment, assign register 
    mem.set16(regs.get(rs1) + imm_s, regs.get(rs2) & 0x0000ffff);

    // Rendering the details of sh
    if (pos)
    {
        string s = render_stype(insn, "sh");
        s.resize(instruction_width, ' ');
        *pos << s << "// m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(mem.get16(regs.get(rs1) + imm_s)) << endl;
    }

    // Add to pc
    pc += 4;
}

/**
*
* @brief Execute sw instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos)
{
    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize rs2
    uint32_t rs2 = get_rs2(insn);
    
    // Initialize imm_s
    int32_t imm_s = get_imm_s(insn);

    // Memory assignment, assign register 
    mem.set32(regs.get(rs1) + imm_s, regs.get(rs2));

    // Render/Dump
    if (pos)
    {
        string s = render_stype(insn, "sw");
        s.resize(instruction_width, ' ');
        *pos << s << "// m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(mem.get32(regs.get(rs1) + imm_s)) << endl;
    }

    // Add to pc
    pc += 4;
}

/**
*
* @brief Execute lb instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    int8_t val = mem.get8(imm_i + regs.get(rs1));

    // Render/Dump
    if (pos)
    {
        string s = render_itype_load(insn, "lb");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << "))" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute lh instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    int16_t val = mem.get16(regs.get(rs1) + imm_i);

    // Render/Dump
    if (pos)
    {
        string s = render_itype_load(insn, "lh");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << "))" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute lw instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = mem.get32(regs.get(rs1) + imm_i);

    // Render/Dump
    if (pos)
    {
        string s = render_itype_load(insn, "lw");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute lbu instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    uint32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint8_t val = mem.get8(regs.get(rs1) + imm_i) & 0x000000ff;

    // Render/Dump
    if (pos)
    {
        string s = render_itype_load(insn, "lbu");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << "))" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute lhu instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    uint32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint16_t val = mem.get16(regs.get(rs1) + imm_i) & 0x0000ffff;

    // Render/Dump
    if (pos)
    {
        string s = render_itype_load(insn, "lhu");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << "))" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute addi instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Render/Dump
    if (pos)
    {
        string s = render_itype_alu(insn, "addi", imm_i);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(regs.get(rs1) + imm_i) << endl;
    }

    // Set register, add to pc
    regs.set(rd, regs.get(rs1) + imm_i);
    pc += 4;
}

/**
*
* @brief Execute slti instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = (regs.get(rs1) < imm_i) ? 1 : 0;

    // Render/Dump
    if (pos)
    {
        string s = render_itype_alu(insn, "slti", imm_i);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << imm_i << ") ? " << "1 " << ": " << "0" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute sltiu instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    uint32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = ((uint32_t)regs.get(rs1) < imm_i) ? 1 : 0;

    // Render/Dump
    if (pos)
    {
        string s = render_itype_alu(insn, "sltiu", imm_i);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << imm_i << ") ? " << "1 " << ": " << "0" << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute xori instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = ((regs.get(rs1) ^ imm_i));

    // Render/dump
    if (pos)
    {
        string s = render_itype_alu(insn, "xori", imm_i);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute ori instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = ((regs.get(rs1) | imm_i));

    // Render/dump
    if (pos)
    {
        string s = render_itype_alu(insn, "ori", imm_i);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute andi instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = ((regs.get(rs1) & imm_i));

    // Render/Dump
    if (pos)
    {
        string s = render_itype_alu(insn, "andi", imm_i);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute slli instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = regs.get(rs1) << imm_i;

    // Render/dump
    if (pos)
    {
        string s = render_itype_alu(insn, "slli", imm_i % XLEN);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " << imm_i << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, and to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute srli instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(insn);

    // Initialize val
    uint32_t val = (uint32_t)regs.get(rs1) >> imm_i;

    // Render/Dump
    if (pos)
    {
        string s = render_itype_alu(insn, "srli", imm_i % XLEN);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << imm_i << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, and add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute srai instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos)
{
    // Initialize shamt
    uint32_t shamt = (insn & 0x01ffffff);

    // Initialize imm_i
    int32_t imm_i = get_imm_i(shamt);

    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    int32_t rs1 = regs.get(get_rs1(insn));

    // Initialize val
    uint32_t val = rs1 >> imm_i;

    // Render/Dump
    if (pos)
    {
        string s = render_itype_alu(insn, "srai", imm_i % XLEN);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex::to_hex0x32(rs1) << " >> " << imm_i << " = " << hex::to_hex0x32(val) << endl;
    }

    // Set register, and add to pc
    regs.set(rd, val);
    pc += 4;
}

/**
*
* @brief Execute csrrs instruction
*
* @param insn Instruction (address)
*
* @param pos Output
*
********************************************************************************/

void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)
{
    // Initialize rd
    uint32_t rd = get_rd(insn);

    // Initialize rs1
    uint32_t rs1 = get_rs1(insn);

    // Initialize imm_i
    int32_t csr = get_imm_i(insn) & 0x00000fff;

    // Render/dump
    if (csr != 0xf14 || rs1 != 0)
    {
        halt = true;
        halt_reason = "Illegal CSR in CRRSS instruction";
    }
    if (pos)
    {
        string s = render_csrrx(insn, "csrrs");
        s.resize(instruction_width, ' ');
        *pos << s << "// " << render_reg(rd) << " = " << std::dec << mhartid << endl;
    }
    if (!halt)
    {
        // Set register, and add to pc
        regs.set(rd, mhartid);
        pc += 4;
    }
}