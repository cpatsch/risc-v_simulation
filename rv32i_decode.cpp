#include "rv32i_decode.h"

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
 * @brief Decodes the current instruction.
 * 
 * @param addr The current address in the simulated memory.
 * @param insn The instruction at point addr.
 * 
 * @return A rendered instruction string.
 * @note If the instruction given is unrecognized, a message declaring
 * the instruction was unrecognized is printed.
*/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    // Switch defined by the opcode. This determines the type of instruction.
    switch (get_opcode(insn))
    {
        case opcode_lui:
            return render_lui(insn);
            break;
        case opcode_auipc:
            return render_auipc(insn);
            break;
        case opcode_jal:
            return render_jal(addr, insn);
            break;
        case opcode_jalr:
            return render_jalr(insn);
            break;
        case opcode_btype:
            // A switch defined by funct3. This determines which b-type
            // instruction the insn is.
            switch(get_funct3(insn))
            {
                case funct3_beq:
                    return render_btype(addr, insn, "beq");
                    break;
                case funct3_bne:
                    return render_btype(addr, insn, "bne");
                    break;
                case funct3_blt:
                    return render_btype(addr, insn, "blt");
                    break;
                case funct3_bge:
                    return render_btype(addr, insn, "bge");
                    break;
                case funct3_bltu:
                    return render_btype(addr, insn, "bltu");
                    break;
                case funct3_bgeu:
                    return render_btype(addr, insn, "bgeu");
                    break;
                default:
                    // If none of the others, render the illegal_insn()
                    return render_illegal_insn(insn);
            }
            assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_load_imm:
            // A switch defined by funct3. This determines which load
            // instruction the insn is.
            switch(get_funct3(insn))
                {
                    case funct3_lb:
                        return render_itype_load(insn, "lb");
                        break;
                    case funct3_lh:
                        return render_itype_load(insn, "lh");
                        break;
                    case funct3_lw:
                        return render_itype_load(insn, "lw");
                        break;
                    case funct3_lbu:
                        return render_itype_load(insn, "lbu");
                        break;
                    case funct3_lhu:
                        return render_itype_load(insn, "lhu");
                        break;
                    default:
                        // If none of the others, render the illegal_insn()
                        return render_illegal_insn(insn);
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_stype:
            // A switch defined by funct3. This determines which s-type
            // instruction the insn is.
            switch(get_funct3(insn))
                {
                    case funct3_sb:
                        return render_stype(insn, "sb");
                        break;
                    case funct3_sh:
                        return render_stype(insn, "sh");
                        break;
                    case funct3_sw:
                        return render_stype(insn, "sw");
                        break;
                    default:
                        // If none of the others, render the illegal_insn()
                        return render_illegal_insn(insn);
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_alu_imm:
            // A switch defined by funct3. This determines which alu
            // instruction the insn is.
            switch(get_funct3(insn))
                {
                    case funct3_add:
                        return render_itype_alu(insn, "addi", get_imm_i(insn));
                        break;
                    case funct3_slt:
                        return render_itype_alu(insn, "slti", get_imm_i(insn));
                        break;
                    case funct3_sltu:
                        return render_itype_alu(insn, "sltiu", get_imm_i(insn));
                        break;
                    case funct3_xor:
                        return render_itype_alu(insn, "xori", get_imm_i(insn));
                        break;
                    case funct3_or:
                        return render_itype_alu(insn, "ori", get_imm_i(insn));
                        break;
                    case funct3_and:
                        return render_itype_alu(insn, "andi", get_imm_i(insn));
                        break;
                    case funct3_sll:
                        return render_itype_alu(insn, "slli", get_rs2(insn));
                        break;
                    case funct3_srx:
                        // An inner switch defiend by funct7. This determines which
                        // srx instruction the insn is.
                        switch(get_funct7(insn))
                        {
                            case funct7_srl:
                                return render_itype_alu(insn, "srli", get_rs2(insn));
                                break;
                            case funct7_sra:
                                return render_itype_alu(insn, "srai", get_rs2(insn));
                                break;
                            default:
                                return render_illegal_insn(insn);
                        }
                        assert(0 && "unrecognized funct7"); // We should not get here
                    default:
                        // If none of the others, render the illegal_insn()
                        return render_illegal_insn(insn);
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_rtype:
            // A switch defined by funct3. This determines which r-type
            // instruction the insn is.
            switch(get_funct3(insn))
                {
                    case funct3_add:
                        // An inner switch defiend by funct7. This determines which
                        // add instruction the insn is.
                        switch(get_funct7(insn))
                        {
                            case funct7_add:
                                return render_rtype(insn, "add");
                                break;
                            case funct7_sub:
                                return render_rtype(insn, "sub");
                                break;
                            default:
                                // If none of the others, render the illegal_insn()
                                return render_illegal_insn(insn);
                        }
                        assert(0 && "unrecognized funct7"); // We should not get here
                    case funct3_sll:
                        return render_rtype(insn, "sll");
                        break;
                    case funct3_slt:
                        return render_rtype(insn, "slt");
                        break;
                    case funct3_sltu:
                        return render_rtype(insn, "sltu");
                        break;
                    case funct3_xor:
                        return render_rtype(insn, "xor");
                        break;
                    case funct3_srx:
                        // Another inner switch defiend by funct7. This determines which
                        // srx instruction the insn is.
                        switch(get_funct7(insn))
                        {
                            case funct7_srl:
                                return render_rtype(insn, "srl");
                                break;
                            case funct7_sra:
                                return render_rtype(insn, "sra");
                                break;
                            default:
                                return render_illegal_insn(insn);
                        }
                        assert(0 && "unrecognized funct7"); // We should not get here
                    case funct3_or:
                        return render_rtype(insn, "or");
                        break;
                    case funct3_and:
                        return render_rtype(insn, "and");
                        break;
                    default:
                        // If none of the others, render the illegal_insn()
                        return render_illegal_insn(insn);
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        case opcode_system:
            // A switch defined by funct3. This determines which system
            // instruction the insn is.
            switch(get_funct3(insn))
                {
                    case 0b000:
                        // An inner switch defiend by the insn itself. This determines
                        // e instruction the insn is.
                        switch(insn)
                        {
                            case insn_ecall:
                                return render_ecall(insn);
                                break;
                            case insn_ebreak:
                                return render_ebreak(insn);
                                break;
                            default:
                                // If none of the others, render the illegal_insn()
                                return render_illegal_insn(insn);
                        }
                        assert(0 && "unrecognized insn"); // We should not get here
                    case funct3_csrrw:
                        return render_csrrx(insn, "csrrw");
                        break;
                    case funct3_csrrs:
                        return render_csrrx(insn, "csrrs");
                        break;
                    case funct3_csrrc:
                        return render_csrrx(insn, "csrrc");
                        break;
                    case funct3_csrrwi:
                        return render_csrrxi(insn, "csrrwi");
                        break;
                    case funct3_csrrsi:
                        return render_csrrxi(insn, "csrrsi");
                        break;
                    case funct3_csrrci:
                        return render_csrrxi(insn, "csrrci");
                        break;
                    default:
                        return render_illegal_insn(insn);
                }
                assert(0 && "unrecognized funct3"); // We should not get here
        default:
            // If none of the others, render the illegal_insn()
            return render_illegal_insn(insn);
    }
    assert(0 && "unrecognized opcode"); // We should not get here
}

/**
 * @brief Renders an unimplemented instruction message.
 * 
 * @param insn The instruction.
 * 
 * @return A rendered unimplemented instruction message string.
*/

std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    // Cast the insn as a void so that it does not need to get used.
    (void) insn;
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * @brief Renders the lui instruction.
 * 
 * @param insn The instruction.
 * 
 * @return A rendered lui string.
*/

std::string rv32i_decode::render_lui(uint32_t insn)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    int32_t immu = get_imm_u(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd) << "," << to_hex0x20((immu >> 12) & 0x0fffff);

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the auipc instruction.
 * 
 * @param insn The instruction.
 * 
 * @return A rendered auipc string.
*/

std::string rv32i_decode::render_auipc(uint32_t insn)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    int32_t immu = get_imm_u(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic("auipc") << render_reg(rd) << "," << to_hex0x20((immu >> 12) & 0x0fffff);

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the jal instruction.
 * 
 * @param addr The current address of the instruction.
 * @param insn The instruction.
 * 
 * @return A rendered jal string.
*/

std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << "," << to_hex0x32(addr+get_imm_j(insn));

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the jalr instruction.
 * 
 * @param insn The instruction.
 * 
 * @return A rendered jalr string.
*/

std::string rv32i_decode::render_jalr(uint32_t insn)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    int32_t immi = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs1, immi);

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders b-type instructions
 * 
 * @param addr The current address of the instruction.
 * @param insn The instruction.
 * @param mnemonic The mnemonic of the b-type instruction.
 * 
 * @return A rendered b-type instruction string.
*/

std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    // Get the needed parts.
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << to_hex0x32(addr + get_imm_b(insn));

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the i-type load instruction.
 * 
 * @param insn The instruction.
 * @param mnemonic The mnemonic of the i-type load instruction.
 * 
 * @return A rendered i-type load instruction string.
*/

std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    int32_t immi = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp(rs1, immi);

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the s-type instructions.
 * 
 * @param insn The instruction.
 * @param mnemonic The mnemonic of the s-type instruction.
 * 
 * @return A rendered s-type instruction string.
*/

std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    // Get the needed parts.
    uint32_t rs2 = get_rs2(insn);
    int32_t imms = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << render_base_disp(rs1, imms);

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the i-type alu instructions.
 * 
 * @param insn The instruction.
 * @param mnemonic The mnemonic of the s-type instruction.
 * @param imm_i The imm_i of the instruction.
 * 
 * @return A rendered i-type alu instruction string.
*/

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the r-type instructions.
 * 
 * @param insn The instruction.
 * @param mnemonic The mnemonic of the s-type instruction.
 * 
 * @return A rendered r-type instruction string.
*/

std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the ecall instruction.
 * 
 * @param insn The instruction.
 * 
 * @return A rendered ecall instruction string.
*/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
    // Cast the insn as a void so that it does not need to get used.
    (void) insn;

    // Build the ostringstream.
    std::ostringstream os;
    os << "ecall";

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the ebreak instruction.
 * 
 * @param insn The instruction.
 * 
 * @return A rendered ebreak instruction string.
*/

std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    // Cast the insn as a void so that it does not need to get used.
    (void) insn;

    // Build the ostringstream.
    std::ostringstream os;
    os << "ebreak";

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the csrrx instructions.
 * 
 * @param insn The instruction.
 * @param mnemonic The mnemonic of the instruction.
 * 
 * @return A rendered csrrx instruction string.
*/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    uint32_t csr = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr) << "," << render_reg(rs1);

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @brief Renders the csrrxi instructions.
 * 
 * @param insn The instruction.
 * @param mnemonic The mnemonic of the instruction.
 * 
 * @return A rendered csrrxi instruction string.
*/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    // Get the needed parts.
    uint32_t rd = get_rd(insn);
    int32_t csr = get_imm_i(insn);
    uint32_t zimm = get_rs1(insn);

    // Build the ostringstream.
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr) << "," << zimm;

    // Return osstringstream as a string.
    return os.str();
}

/**
 * @defgroup get_X Get instruction parts.
 * Return various parts of a given insn.
 * 
 * @param insn The instruction we are taking from.
 * @return The desired part of the insn.
 * @{
*/

uint32_t rv32i_decode::get_opcode(uint32_t insn)    ///< Get the opcode from the insn.
{
    return (insn & 0x0000007f);
}

uint32_t rv32i_decode::get_rd(uint32_t insn)        ///< Get the rd of the insn.
{
    return ((insn & 0x00000f80) >> 7);
}

uint32_t rv32i_decode::get_rs1(uint32_t insn)       ///< Get the rs1 of the insn.
{
    return ((insn & 0x000f8000) >> 15);
}

uint32_t rv32i_decode::get_rs2(uint32_t insn)       ///< Get the rs2 of the insn.
{
    return ((insn & 0x01f00000) >> 20);
}

uint32_t rv32i_decode::get_funct3(uint32_t insn)    ///< Get the funct3 of the insn.
{
    return ((insn & 0x00007000) >> 12);
}

uint32_t rv32i_decode::get_funct7(uint32_t insn)    ///< Get the funct7 of the insn.
{
    return ((insn & 0xfe000000) >> 25);
}

/**@}*/

/**
 * @defgroup get_imm_x Get imm
 * Get the imm of the current instruction type.
 * 
 * @param insn The instruction.
 * @return The imm value.
 * @{
*/

int32_t rv32i_decode::get_imm_i(uint32_t insn)      ///< Get the imm of an i instruction.
{
    return ((int32_t)(insn & 0xfff00000) >> 20);
}

int32_t rv32i_decode::get_imm_u(uint32_t insn)      ///< Get the imm of an u instruction.
{
    return ((insn & 0xfffff000));
}

int32_t rv32i_decode::get_imm_b(uint32_t insn)      ///< Get the imm of an b instruction.
{
    int32_t val = insn;

    val = (val) >> 31;  // Shift 31 for sign extension.
    val <<= 12;         // Shift back 12 to make space for other sections.
    
    val = val | ((insn & 0x7e000000) >> 20);    // Shift and place b-g.
    val = val | ((insn & 0x00000080) << 4);     // Shift and place y.
    val = val | ((insn & 0x00000f00) >> 7);     // Shift and place u-x.

    return val;
}

int32_t rv32i_decode::get_imm_s(uint32_t insn)      ///< Get the imm of an s instruction.
{
    return ((int32_t)(insn & 0xfe000000) >> 20) | get_rd(insn);
}

int32_t rv32i_decode::get_imm_j(uint32_t insn)      ///< Get the imm of an j instruction.
{
    int32_t val = insn;

    val = (val) >> 31;  // Shift 31 for sign extension.
    val <<= 19;         // Shift back 19 to make space for other sections.
    
    val = val | ((insn & 0x7fe00000) >> 20);    // Shift and place b-k.
    val = val | ((insn & 0x00100000) >> 9);     // Shift and place l.
    val = val | ((insn & 0x000ff000));          // Place m-t.

    return val;
}

/**@}*/

/**
 * @brief Renders the parameter as a register.
 * 
 * @param r The register.
 * 
 * @return A rendered register string.
*/

std::string rv32i_decode::render_reg(int r)
{
    std::ostringstream os;
    os << "x" << r;
    return os.str();
}

/**
 * @brief Renders the parameters as disp(base)
 * 
 * @param base The base register.
 * @param disp The displacement.
 * 
 * @return A rendered string in the form of disp(base).
*/

std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp)
{
    std::ostringstream os;
    os << disp << "(" << render_reg(base) << ")";
    return os.str();
}

/**
 * @brief Renders the mnemonic.
 * 
 * @param m The mnemonic to render.
 * 
 * @return A rendered mnemonic string.
 * @note The mnemonic is padded with spaces for formatting.
*/

std::string rv32i_decode::render_mnemonic(const std::string &m)
{
    std::ostringstream os;
    os << std::setfill(' ') << std::setw(mnemonic_width) << std::left << m;
    return os.str();
}