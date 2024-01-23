#include "cpu_single_hart.h"

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
 * @brief Runs the cpu.
 * 
 * @param exec_limit The maximum number of instructions to be executed
*/
void cpu_single_hart::run(uint64_t exec_limit)
{
    // Set the 2nd register to the size of memory.
    regs.set(2, mem.get_size());

    // If there is not a limit...
    if (exec_limit == 0)
    {
        // tick() until the program is halted.
        while (!is_halted())
        {
            tick();
        }
    }
    else    // There is a limit.
    {
        // tick() until the program is halted or we hit the limit.
        while (!is_halted() && get_insn_counter() < exec_limit)
        {
            tick();
        }
    }

    // If the cpu was halted, execution was terminated. Provide the reason.
    if (is_halted())
    {
        cout << "Execution terminated. Reason: " << get_halt_reason() << endl;
    }

    // Print the number of instructions executed.
    cout << get_insn_counter() << " instructions executed" << endl;
}