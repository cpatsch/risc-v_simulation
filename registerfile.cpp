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

/**
 * @brief Constructor for registerfile
*/
registerfile::registerfile()
{
    // Resize the register vector to the number of registers
    // in a rv32i machine
    reg.resize(num_regs);

    // Reset the registers
    reset();
}

/**
 * @brief Destructor for registerfile
*/
registerfile::~registerfile()
{
    // Clear the registers
    reg.clear();
}

/**
 * @brief Gets the register value
 * 
 * @param r The target register.
 * 
 * @return The value at register r.
 * @note If the target register is 0, return 0.
*/
int32_t registerfile::get(uint32_t r) const
{
    if (r != 0)
    {
        // Return value at register r
        return reg.at(r);
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Sets the register value
 * 
 * @param r The target register.
 * 
 * @note If the target register is 0, do nothing.
*/
void registerfile::set(uint32_t r, int32_t val)
{
    // Set the register at r to val
    if (r != 0)
    {
        reg[r] = val;
    }
}

/**
 * @brief Resets the registers
*/
void registerfile::reset()
{
    // Set register 0 to 0x0
    set(0, 0x0);

    // Set the other registers to 0xf0f0f0f0
    for (uint32_t i = 1; i < num_regs; i++)
    {
        set(i, 0xf0f0f0f0);
    }
}

/**
 * @brief Dump the contents of the registers.
*/
void registerfile::dump(const std::string &hdr) const
{
    // Cycle through the registers
    for (uint32_t i = 0; i < num_regs; i++)
    {
        // If i%8 = 0, print the register number.
        if ((i % 8) == 0)
        {
            cout << std::setfill(' ');
            cout << hdr << std::setw(3) << std::right << "x" + std::to_string(i);
        }

        // Print the register value at i.
        cout << " " << hex::to_hex32(reg.at(i));

        // If 4 registers have been printed, print an extra space.
        if (((i+1) % 4) == 0 && ((i+1) % 8) != 0)
        {
            cout << " ";
        }

        // If 8 registers have been printed, print a newline.
        if (((i+1) % 8) == 0)
        {
            cout << endl;
        }
    }
}