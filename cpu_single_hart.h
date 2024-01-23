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

class cpu_single_hart : public rv32i_hart
{
public:
    /**
     * @brief Constructor for the cpu.
     * 
     * @param mem The simulated memory the CPU will access.
    */
    cpu_single_hart(memory &mem) : rv32i_hart(mem) {}

    void run(uint64_t exec_limit);
};