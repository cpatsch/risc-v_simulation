#include "memory.h"

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
 * @brief Constructor. Creates the simulated memory.
 * 
 * @param s The number of bytes to create in the simulated memory.
 * @note The value of s will be rounded up to the next multiple of 16.
*/
memory::memory(uint32_t s)
{
    s = (s+15) & 0xfffffff0;  // Round the length up, mod-16

    // Resize the mem vector.
    mem.resize(s, 0xa5);
}

/**
 * @brief Deallocate the simulated memory.
*/
memory::~memory()
{
    // Clear the mem vector.
    mem.clear();
}

/**
 * @brief Checks if the address is in the range of the simulated memory.
 * 
 * @param i The address to check.
 * @return True if the address is not within the range of the simulated memory,
 *  False if the address is within the range of the simulated memory.
*/

bool memory::check_illegal(uint32_t i) const
{
    // If the address is greater than the size of the simulated memory.
    if ( i >= get_size() )
    {
        // The address is out of range. Print so on std::cerr
        cerr << "WARNING: Address out of range: " + hex::to_hex0x32(i) << endl;
        
        // Return true.
        return true;
    }
    else
    {
        // Return false.
        return false;
    }
}

/**
 * @brief Get the size of the simulated memory.
 * 
 * @return The size of the simulated memory.
*/

uint32_t memory::get_size() const
{
    // Return the size of the mem vector.
    return mem.size();
}

/**
 * @defgroup getX Get little-endian
 * Read and return a little-endian value from memory.
 * 
 * @param addr The address of the byte to read in the simulated memory.
 * @return The little-endian value from the simulated memory starting at
 *  address addr.
 * @note If an address is not within range of the simulated memory, a
 *  warning message will be printed to std::cerr.
 * @{
*/

uint8_t memory::get8(uint32_t addr) const       ///< Get an 8-bit value from the simulated memory.
{
    // Check if the address is legal.
    if(!check_illegal(addr))
    {
        // Return the byte at the address.
        return mem[addr];
    }
    else
    {
        return 0;
    }
}

uint16_t memory::get16(uint32_t addr) const     ///< Get a 16-bit little-endian value from the simulated memory.
{
    return get8(addr) | (get8(addr+1) << 8);
}

uint32_t memory::get32(uint32_t addr) const     ///< Get a 32-bit little-endian value from the simulated memory.
{
    return get16(addr) | (get16(addr+2) << 16);
}
/**@}*/

/**
 * @defgroup getX_sx Get signed-extended little-endian
 * Read and return a signed-extended little-endian value from memory.
 * 
 * @param addr The address of the byte to read in the simulated memory.
 * @return The signed-extended little-endian value from the simulated memory
 *  starting at address addr.
 * @note If an address is not within range of the simulated memory, a
 *  warning message will be printed to std::cerr.
 * @{
*/
int32_t memory::get8_sx(uint32_t addr) const    ///< Get a signed-extended 8-bit value from the simulated memory.
{
    // Set val to get8() at the current address.
    int32_t val = get8(addr);

    return val | (val & 0x00000080 ? 0xffffff00 : 0);
}

int32_t memory::get16_sx(uint32_t addr) const   ///< Get a signed-extended 16-bit little-endian value from the simulated memory.
{
    // Set val to get16() at the current address.
    int32_t val = get16(addr);

    return val | (val & 0x00008000 ? 0xffff0000 : 0);
}

int32_t memory::get32_sx(uint32_t addr) const   ///< Get a signed-extended 32-bit little-endian value from the simulated memory.
{
    return get32(addr);
}
/**@}*/

/**
 * @defgroup setX Set little-endian
 * Set a little-endian value into memory.
 * 
 * @param addr The address of the byte to set in the simulated memory.
 * @param val The value to set at the given address.
 * @note If an address is not within range of the simulated memory, a
 *  warning message will be printed to std::cerr.
 * @{
*/

void memory::set8(uint32_t addr, uint8_t val)
{
    // Check if the the address is legal.
    if(!check_illegal(addr))
    {
        // Set the byte at the address.
        mem[addr] = val;
    }
}

void memory::set16(uint32_t addr, uint16_t val)
{
    // Set the first 8 bytes of val.
    set8(addr, (uint8_t) val);

    // Bitshift val to the right 8.
    val >>= 8;

    // Set the last 8 bytes of val.
    set8(addr+1, (uint8_t) val);
}

void memory::set32(uint32_t addr, uint32_t val)
{
    // Set the first 16 bytes of val.
    set16(addr, (uint16_t) val);

    // Bitshift val to the right 16.
    val >>= 16;

    // Set the last 16 bytes of val.
    set16(addr+2, (uint16_t) val);
}

/**@}*/

/**
 * @brief Dump the contents of the simulated memory.
*/

void memory::dump() const
{
    // Loop through the mem vector.
    for (uint32_t i = 0; i < mem.size(); i++)
    {
        // If i mod 16 == 0, print the current address.
        if ((i % 16) == 0)
        {
            cout << std::setfill('0');
            cout << hex::to_hex32(i) << ": ";
        }

        // Print the current byte at i.
        cout << hex::to_hex8(mem.at(i)) << " ";

        // For every 8th item, print an extra space.
        if (((i+1) % 8) == 0 && ((i+1) % 16) != 0)
        {
            cout << " ";
        }

        // For every 16 elements, print the line in character values.
        if (((i+1) % 16) == 0)
        {
            // Print a star.
            cout << '*';

            // Cycle through the current 16 bytes of the memory.
            for (uint32_t j = (i - 15); j <= i; j++)
            {
                // Set char to the current byte.
                uint8_t ch = get8(j);
                // If it has a character to print, set that
                // character, otherwise set to a period.
                ch = isprint(ch) ? ch : '.';

                // Print the current character.
                cout << ch;
            }

            // Print the ending star.
            cout << '*' << endl;
        }
    }
}

/**
 * @brief Loads the contents of a file into the simulated memory.
 * 
 * @param fname The name of the file to open.
 * @return True if the value can be loaded into the simulated memory, False 
 *  if the file cannot be opened, or if the program is too big for the 
 *  simulated memory.
 * @note If the file cannot be opened, a warning message will be printed to
 *  std::cerr.
 * @note If an address is not within range of the simulated memory, a
 *  warning message will be printed to std::cerr.
*/
bool memory::load_file(const std::string &fname)
{
    // Create the infile.
    std::ifstream infile(fname, std::ios::in|std::ios::binary);
    
    // If the file cannot be opened.
    if (!infile.is_open())
    {
        // Print to std::cerr.
        cerr << "Can't open file '" + fname + "' for reading." << endl;
        return false;
    }
    else // Else, the file can be opened.
    {
        // Loop through the vector, setting i to the infile's
        // next digit.
        uint8_t i;
        infile >> std::noskipws;
        for (uint32_t addr = 0; infile >> i; ++addr)
        {
            // Check if the current address is illegal.
            if (check_illegal(addr))
            {
                // If so, print that the program is too big.
                cerr << "Program too big." << endl;
                // Close the file.
                infile.close();

                return false;
            }
            else // Else, the address is valid. Set the byte at
                 // that address.
            {
                set8(addr, i);
            }
        }

        // Close the file.
        infile.close();

        return true;
    }
}