#include "rv32i_hart.h"

//***************************************************************************
//
//  Caleb Patsch
//  11/04/2022
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

/**
 * @brief Resets the hart.
*/

void rv32i_hart::reset()
{
    // Set program counter to 0.
    pc = 0;

    // Reset the registers.
    regs.reset();

    // Set the instruction coutner to 0.
    insn_counter = 0;

    // Set halt to false.
    halt = false;

    // Set halt_reason to "none".
    halt_reason = "none";
}

/**
 * @brief Dumps the contents of the hart.
 * 
 * @param hdr The header to print on each line.
 * 
*/

void rv32i_hart::dump(const std::string &hdr) const
{
    // Dump the regs with hdr.
    regs.dump(hdr);

    // Print the program counter.
    cout << " pc " << to_hex32(pc) << endl;
}

/**
 * @brief Gets each instruction and executes them.
 * 
 * @param hdr The header to print on each line.
 * 
 * @note The hart ticks until it is halted.
*/

void rv32i_hart::tick(const std::string &hdr)
{
    // If the hart has not been halted...
    if (!halt)
    {
        // Dump registers if show_regs is true.
        if (show_registers)
        {
            dump(hdr);
        }

        // If pc % 4 does not equal 0, the program counter
        // is not aligned.
        if (pc % 4 != 0)
        {
            // Halt the hart.
            halt = true;

            // Set the halt reason.
            halt_reason = "PC alignment error";

            return;
        }

        // Increment the instruction counter.
        insn_counter += 1;

        // Get the instruction at the program counter.
        uint32_t insn = mem.get32(pc);

        // If show_instructions is true, print the instruction
        // and what it does.
        if (show_instructions)
        {
            cout << hdr << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << "  ";
            // Execute the instruction, pass in cout.
            exec(insn, &std::cout);
            cout << endl;
        }
        else
        {
            // Execute the instruction, don't pass in cout.
            exec(insn, nullptr);
        }
    }
    else
    {
        return;
    }  
}

/**
 * @brief Determines what the insn is and executes it.
 * 
 * @param insn The instruction.
 * @param pos The ostream. In this program, it is cout.
 * 
 * @note If the instruction given is unrecognized, a message declaring
 * the instruction was unrecognized is printed.
*/

void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
    // Get the opcode, funct3, and funct7 of the insn.
    uint32_t opcode = get_opcode(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);

    switch (opcode)
    {
        case opcode_lui:
            exec_lui(insn, pos);
            return;
        case opcode_auipc:
            exec_auipc(insn, pos);
            return;
        case opcode_jal:
            exec_jal(insn, pos);
            return;
        case opcode_jalr:
            exec_jalr(insn, pos);
            return;
        case opcode_btype:
            // A switch defined by funct3. This determines which b-type
            // instruction the insn is.
            switch(funct3)
            {
                case funct3_beq:
                    exec_beq(insn, pos);
                    return;
                case funct3_bne:
                    exec_bne(insn, pos);
                    return;
                case funct3_blt:
                    exec_blt(insn, pos);
                    return;
                case funct3_bge:
                    exec_bge(insn, pos);
                    return;
                case funct3_bltu:
                    exec_bltu(insn, pos);
                    return;
                case funct3_bgeu:
                    exec_bgeu(insn, pos);
                    return;
                default:
                    // If none of the others, render the illegal_insn()
                    exec_illegal_insn(insn, pos);
                    return;
            }
            assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_load_imm:
            // A switch defined by funct3. This determines which load
            // instruction the insn is.
            switch(funct3)
                {
                    case funct3_lb:
                        exec_lb(insn, pos);
                        return;
                    case funct3_lh:
                        exec_lh(insn, pos);
                        return;
                    case funct3_lw:
                        exec_lw(insn, pos);
                        return;
                    case funct3_lbu:
                        exec_lbu(insn, pos);
                        return;
                    case funct3_lhu:
                        exec_lhu(insn, pos);
                        return;
                    default:
                        // If none of the others, render the illegal_insn()
                        exec_illegal_insn(insn, pos);
                        return;
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_stype:
            // A switch defined by funct3. This determines which s-type
            // instruction the insn is.
            switch(funct3)
                {
                    case funct3_sb:
                        exec_sb(insn, pos);
                        return;
                    case funct3_sh:
                        exec_sh(insn, pos);
                        return;
                    case funct3_sw:
                        exec_sw(insn, pos);
                        return;
                    default:
                        // If none of the others, render the illegal_insn()
                        exec_illegal_insn(insn, pos);
                        return;
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_alu_imm:
            // A switch defined by funct3. This determines which alu
            // instruction the insn is.
            switch(funct3)
                {
                    case funct3_add:
                        exec_addi(insn, pos);
                        return;
                    case funct3_slt:
                        exec_slti(insn, pos);
                        return;
                    case funct3_sltu:
                        exec_sltiu(insn, pos);
                        return;
                    case funct3_xor:
                        exec_xori(insn, pos);
                        return;
                    case funct3_or:
                        exec_ori(insn, pos);
                        return;
                    case funct3_and:
                        exec_andi(insn, pos);
                        return;
                    case funct3_sll:
                        exec_slli(insn, pos);
                        return;
                    case funct3_srx:
                        // An inner switch defiend by funct7. This determines which
                        // srx instruction the insn is.
                        switch(funct7)
                        {
                            case funct7_srl:
                                exec_srli(insn, pos);
                                return;
                            case funct7_sra:
                                exec_srai(insn, pos);
                                return;
                            default:
                                exec_illegal_insn(insn, pos);
                                return;
                        }
                        assert(0 && "unrecognized funct7"); // We should not get here
                    default:
                        // If none of the others, render the illegal_insn()
                        exec_illegal_insn(insn, pos);
                        return;
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_rtype:
            // A switch defined by funct3. This determines which r-type
            // instruction the insn is.
            switch(funct3)
                {
                    case funct3_add:
                        // An inner switch defiend by funct7. This determines which
                        // add instruction the insn is.
                        switch(funct7)
                        {
                            case funct7_add:
                                exec_add(insn, pos);
                                return;
                            case funct7_sub:
                                exec_sub(insn, pos);
                                return;
                            default:
                                // If none of the others, render the illegal_insn()
                                exec_illegal_insn(insn, pos);
                                return;
                        }
                        assert(0 && "unrecognized funct7"); // We should not get here
                    case funct3_sll:
                        exec_sll(insn, pos);
                        return;
                    case funct3_slt:
                        exec_slt(insn, pos);
                        return;
                    case funct3_sltu:
                        exec_sltu(insn, pos);
                        return;
                    case funct3_xor:
                        exec_xor(insn, pos);
                        return;
                    case funct3_srx:
                        // Another inner switch defiend by funct7. This determines which
                        // srx instruction the insn is.
                        switch(funct7)
                        {
                            case funct7_srl:
                                exec_srl(insn, pos);
                                return;
                            case funct7_sra:
                                exec_sra(insn, pos);
                                return;
                            default:
                                exec_illegal_insn(insn, pos);
                                return;
                        }
                        assert(0 && "unrecognized funct7"); // We should not get here
                    case funct3_or:
                        exec_or(insn, pos);
                        return;
                    case funct3_and:
                        exec_and(insn, pos);
                        return;
                    default:
                        // If none of the others, render the illegal_insn()
                        exec_illegal_insn(insn, pos);
                        return;
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_system:
            // A switch defined by funct3. This determines which system
            // instruction the insn is.
            switch(funct3)
                {
                    case 0b000:
                        // An inner switch defiend by the insn itself. This determines
                        // e instruction the insn is.
                        switch(insn)
                        {
                            case insn_ecall:
                                exec_ecall(insn, pos);
                                return;
                            case insn_ebreak:
                                exec_ebreak(insn, pos);
                                return;
                            default:
                                // If none of the others, render the illegal_insn()
                                exec_illegal_insn(insn, pos);
                                return;
                        }
                        assert(0 && "unrecognized insn"); // We should not get here
                    case funct3_csrrs:
                        exec_csrrs(insn, pos);
                        return;
                    default:
                        exec_illegal_insn(insn, pos);
                        return;
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        default:
            // If none of the others, render the illegal_insn()
            exec_illegal_insn(insn, pos);
            return;
    }
    assert(0 && "unrecognized opcode"); // We should not get here
}

/**
 * @defgroup exec_x
 * Executes an instruction.
 * 
 * @param insn The instruction to execute.
 * @param pos The ostream passed in. In this program, it is cout.
 * 
 * @note If pos is passed in and not nullptr, the fucntion prints what
 * the instruction does.
 * 
 * @{
*/

void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos)    ///< Execute illegal_insn
{
    if (pos)
    {
        *pos << render_illegal_insn(insn);
    }
    
    // Set halt to true.
    halt = true;
    // Set the halt_reason.
    halt_reason = "Illegal instruction";
}

void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos)             ///< Execute lui
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_u(insn);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_lui(insn);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm);
    }

    // Set the register at rd to the value of imm.
    regs.set(rd, imm);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos)       ///< Execite auipc
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_u(insn);

    // Determine the value.
    int32_t val = pc + imm;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_auipc(insn);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " 
             << hex::to_hex0x32(imm) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos)         ///< Execute jal
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_j(insn);

    // Determine the values.
    int32_t val = pc + 4;
    uint32_t val2 = pc + imm;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_jal(pc, insn);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(val) << ",  pc = "
             << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm) << " = " << hex::to_hex0x32(val2);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Set the pc to pc + imm.
    pc = val2;
}

void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos)        ///< Execute jalr
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    uint32_t val = (regs.get(rs1) + imm) & ~1;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_jalr(insn);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) << ",  pc = ("
             << hex::to_hex0x32(imm) << " + " << hex::to_hex0x32(regs.get(rs1)) << ") & " 
             << hex::to_hex0x32(~1) << " = " <<  hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of pc+4.
    regs.set(rd, (pc + 4));
    // Set the pc to val.
    pc = val;
}

void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos)         ///< Execute beq
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint32_t val = pc + ((regs.get(rs1) == regs.get(rs2)) ? imm : 4);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_btype(pc, insn, "beq");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == "
             << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm) << " : 4) = "
             << hex::to_hex0x32(val);
    }

    // Set the pc to val.
    pc = val;
}

void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos)         ///< Execute bne
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint32_t val = pc + ((regs.get(rs1) != regs.get(rs2)) ? imm : 4);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_btype(pc, insn, "bne");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != "
             << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm) << " : 4) = "
             << hex::to_hex0x32(val);
    }

    // Set the pc to val.
    pc = val;
}

void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos)         ///< Execute blt
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint32_t val = pc + ((regs.get(rs1) < regs.get(rs2)) ? imm : 4);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_btype(pc, insn, "blt");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < "
             << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm) << " : 4) = "
             << hex::to_hex0x32(val);
    }

    // Set the pc to val.
    pc = val;
}

void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos)         ///< Execute bge
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint32_t val = pc + ((regs.get(rs1) >= regs.get(rs2)) ? imm : 4);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_btype(pc, insn, "bge");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= "
             << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm) << " : 4) = "
             << hex::to_hex0x32(val);
    }

    // Set the pc to val.
    pc = val;
}

void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos)        ///< Execute bltu
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint32_t val = pc + (((uint32_t)regs.get(rs1)) < ((uint32_t)regs.get(rs2)) ? imm : 4);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_btype(pc, insn, "bltu");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " <U "
             << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm) << " : 4) = "
             << hex::to_hex0x32(val);
    }

    // Set the pc to val.
    pc = val;
}

void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos)        ///< Execute bgeu
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint32_t val = pc + (((uint32_t)regs.get(rs1)) >= ((uint32_t)regs.get(rs2)) ? imm : 4);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_btype(pc, insn, "bgeu");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U "
             << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm) << " : 4) = "
             << hex::to_hex0x32(val);
    }

    // Set the pc to val.
    pc = val;
}

void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos)          ///< Execute lb
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    uint32_t val = mem.get8_sx(regs.get(rs1) + imm);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_load(insn, "lb");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << "sx(m8(" << hex::to_hex0x32(regs.get(rs1)) 
             << " + " << hex::to_hex0x32(imm) << ")) = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos)          ///< Execute lh
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    uint32_t val = mem.get16_sx(regs.get(rs1) + imm);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_load(insn, "lh");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << "sx(m16(" << hex::to_hex0x32(regs.get(rs1)) 
             << " + " << hex::to_hex0x32(imm) << ")) = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos)          ///< Execute lw
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    uint32_t val = mem.get32_sx(regs.get(rs1) + imm);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_load(insn, "lw");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << "sx(m32(" << hex::to_hex0x32(regs.get(rs1)) 
             << " + " << hex::to_hex0x32(imm) << ")) = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos)         ///< Execute lbu
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    uint32_t val = mem.get8(regs.get(rs1) + imm);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_load(insn, "lbu");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << "zx(m8(" << hex::to_hex0x32(regs.get(rs1)) 
             << " + " << hex::to_hex0x32(imm) << ")) = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos)         ///< Execute lhu
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    uint32_t val = mem.get16(regs.get(rs1) + imm);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_load(insn, "lhu");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << "zx(m16(" << hex::to_hex0x32(regs.get(rs1)) 
             << " + " << hex::to_hex0x32(imm) << ")) = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos)          ///< Execute sb
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint8_t val = (regs.get(rs2) /*& 0x000000ff*/);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_stype(insn, "sb");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm) 
             << ") = " << hex::to_hex0x32(val);
    }

    // Set the 8 bytes at rs1+imm to val.
    mem.set8((regs.get(rs1)+imm), val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos)          ///< Execute sh
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    uint16_t val = (regs.get(rs2) /*& 0x0000ffff*/);

    if (pos)
    {
        std::string s = render_stype(insn, "sh");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm) 
             << ") = " << hex::to_hex0x32(val);
    }

    // Set the 16 bytes at rs1+imm to val.
    mem.set16((regs.get(rs1)+imm), val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos)          ///< Execute sw
{
    // Get the required parts of the insn.
    int32_t imm = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs2);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_stype(insn, "sw");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm) 
             << ") = " << hex::to_hex0x32(val);
    }

    // Set the 32 bytes at rs1+imm to val.
    mem.set32((regs.get(rs1)+imm), val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos)        ///< Execute addi
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) + imm;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "addi", imm);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " + " << hex::to_hex0x32(imm) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos)        ///< Execute slti
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = ((regs.get(rs1) < imm) ? 1 : 0);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "slti", imm);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) 
             << " < " << imm << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos)       ///< Execute sltiu
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = ((( uint32_t) regs.get(rs1) < (uint32_t) imm) ? 1 : 0);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "sltiu", imm);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) 
             << " <U " << imm << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos)        ///< Execute xori
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) ^ imm;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "xori", imm);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " ^ " << hex::to_hex0x32(imm) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos)         ///< Execute ori
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) | imm;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "ori", imm);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " | " << hex::to_hex0x32(imm) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos)        ///< Execute andi
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    int32_t imm = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) & imm;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "andi", imm);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " & " << hex::to_hex0x32(imm) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos)        ///< Execute slli
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t shamt = (get_imm_i(insn) & 0x0000001f);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = (regs.get(rs1) << shamt);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "slli", shamt);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " << " << shamt << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos)        ///< Execute srli
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t shamt = (get_imm_i(insn) & 0x0000001f);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = ((uint32_t)regs.get(rs1) >> shamt);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "srli", shamt);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " >> " << shamt << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos)        ///< Execute srai
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t shamt = (get_imm_i(insn) & 0x0000001f);
    uint32_t rs1 = get_rs1(insn);

    // Determine the value.
    int32_t val = (regs.get(rs1) >> shamt);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_itype_alu(insn, "srai", shamt);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " >> " << shamt << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)         ///< Execute add
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) + regs.get(rs2);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "add");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " + " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)         ///< Execute sub
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) - regs.get(rs2);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "sub");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " - " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)         ///< Execute sll
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) << ((regs.get(rs2))%XLEN);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "sll");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " << " << (regs.get(rs2))%XLEN << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)         ///< Execute slt
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "slt");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < "
             << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)        ///< Execute sltu
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "sltu");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U "
             << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)         ///< Execute xor
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) ^ regs.get(rs2);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "xor");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ "
             << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)         ///< Execute srl
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = (uint32_t) regs.get(rs1) >> ((uint32_t)(regs.get(rs2))%XLEN);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "srl");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " >> " << (regs.get(rs2))%XLEN << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)         ///< Execute sra
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) >> ((regs.get(rs2))%XLEN);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "sra");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) 
             << " >> " << (regs.get(rs2))%XLEN << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)          ///< Execute or
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) | regs.get(rs2);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "or");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | "
             << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)         ///< Execute and
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Determine the value.
    int32_t val = regs.get(rs1) & regs.get(rs2);

    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_rtype(insn, "and");
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & "
             << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
    }

    // Set the register at rd to the value of val.
    regs.set(rd, val);
    // Increment the program coutner by 4.
    pc += 4;
}

void rv32i_hart::exec_ecall(uint32_t insn, std::ostream* pos)       ///< Execute ecall
{
    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_ecall(insn);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// HALT";
    }

    // Set halt to true.
    halt = true;
    // Set halt_reason.
    halt_reason = "ECALL instruction";
}

void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos)      ///< Execute ebreak
{
    // If cout was passed, print what the instruction does.
    if (pos)
    {
        std::string s = render_ebreak(insn);
        *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
        *pos << "// HALT";
    }

    // Set halt to true.
    halt = true;
    // Set halt_reason.
    halt_reason = "EBREAK instruction";
}

void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)       ///< Execute csrrs
{
    // Get the required parts of the insn.
    uint32_t rd = get_rd(insn);
    uint32_t csr = (get_imm_i(insn) & 0x00000fff);
    //uint32_t rs1 = get_rs1(insn);

    if (csr == 0xf14 && rd != 0)
    {
        // If cout was passed, print what the instruction does.
        if (pos)
        {
            std::string s = render_csrrx(insn, "csrrs");
            *pos << std::setw(instruction_width) << std:: setfill(' ') << std::left << s;
            *pos << "// " << render_reg(rd) << " = " << mhartid;
        }

        // Set the register at rd to the hart id.
        regs.set(rd, mhartid);
        //csr = rs1;
        // Increment the program coutner by 4.
        pc += 4;
    }
    else
    {
        // Set halt to true.
        halt = true;
        // Set halt_reason.
        halt_reason = "Illegal CSR in CSRRS instruction";
    }
}

/**@}*/