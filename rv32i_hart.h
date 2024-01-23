#include "registerfile.h"

//***************************************************************************
//
//  Caleb Patsch
//  11/04/2022
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

class rv32i_hart : public rv32i_decode
{
public:
    /**
     * @brief Constructs the hart.
     * 
     * @param mem The memory assigned to the hart.
    */
    rv32i_hart(memory &m) : mem(m) { }
    /**
     * @brief Sets show_instructions
     * 
     * @param b The bool value that determines if instructions will 
     * be shown or not.
    */
    void set_show_instructions(bool b) { show_instructions = b; }
    /**
     * @brief Sets show_registers
     * 
     * @param b The bool value that determines if registers will 
     * be shown or not.
    */
    void set_show_registers(bool b) { show_registers = b; }
    /**
     * @brief Getter for halt
     * 
     * @return The value of halt.
    */
    bool is_halted() const { return halt; }
    /**
     * @brief Getter for halt_reason
     * 
     * @return The value of halt_reason.
    */
    const std::string &get_halt_reason() const { return halt_reason; }
    /**
     * @brief Getter for insn_counter
     * 
     * @return The value of insn_counter.
    */
    uint64_t get_insn_counter() const { return insn_counter; }
    /**
     * @brief Setter for mhartid
    */
    void set_mhartid(int i) { mhartid = i; }

    void tick(const std::string &hdr="");
    void dump(const std::string &hdr="") const;
    void reset();

private:
    static constexpr int instruction_width = 35;
    void exec(uint32_t insn, std::ostream*);
    void exec_illegal_insn(uint32_t insn, std::ostream*);

    void exec_lui(uint32_t insn, std::ostream*);
    void exec_auipc(uint32_t insn, std::ostream*);
    void exec_jal(uint32_t insn, std::ostream*);
    void exec_jalr(uint32_t insn, std::ostream*);

    void exec_beq(uint32_t insn, std::ostream*);
    void exec_bne(uint32_t insn, std::ostream*);
    void exec_blt(uint32_t insn, std::ostream*);
    void exec_bge(uint32_t insn, std::ostream*);
    void exec_bltu(uint32_t insn, std::ostream*);
    void exec_bgeu(uint32_t insn, std::ostream*);

    void exec_lb(uint32_t insn, std::ostream*);
    void exec_lh(uint32_t insn, std::ostream*);
    void exec_lw(uint32_t insn, std::ostream*);
    void exec_lbu(uint32_t insn, std::ostream*);
    void exec_lhu(uint32_t insn, std::ostream*);

    void exec_sb(uint32_t insn, std::ostream*);
    void exec_sh(uint32_t insn, std::ostream*);
    void exec_sw(uint32_t insn, std::ostream*);

    void exec_addi(uint32_t insn, std::ostream*);
    void exec_slti(uint32_t insn, std::ostream*);
    void exec_sltiu(uint32_t insn, std::ostream*);
    void exec_xori(uint32_t insn, std::ostream*);
    void exec_ori(uint32_t insn, std::ostream*);
    void exec_andi(uint32_t insn, std::ostream*);
    void exec_slli(uint32_t insn, std::ostream*);
    void exec_srli(uint32_t insn, std::ostream*);
    void exec_srai(uint32_t insn, std::ostream*);

    void exec_add(uint32_t insn, std::ostream*);
    void exec_sub(uint32_t insn, std::ostream*);
    void exec_sll(uint32_t insn, std::ostream*);
    void exec_slt(uint32_t insn, std::ostream*);
    void exec_sltu(uint32_t insn, std::ostream*);
    void exec_xor(uint32_t insn, std::ostream*);
    void exec_srx(uint32_t insn, std::ostream*);
    void exec_or(uint32_t insn, std::ostream*);
    void exec_and(uint32_t insn, std::ostream*);

    void exec_srl(uint32_t insn, std::ostream*);
    void exec_sra(uint32_t insn, std::ostream*);

    void exec_ecall(uint32_t insn, std::ostream*);
    void exec_ebreak(uint32_t insn, std::ostream*);

    void exec_csrrs(uint32_t insn, std::ostream*);

    bool show_instructions = false;
    bool show_registers = false;

    bool halt = { false };
    std::string halt_reason = { "none" };

    uint64_t insn_counter = { 0 };
    uint32_t pc = { 0 };
    uint32_t mhartid = { 0 };

protected:
    registerfile regs;
    memory &mem;
};