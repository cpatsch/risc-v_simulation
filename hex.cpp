#include "hex.h"

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
 * @defgroup to_hex
 * Print value in hex.
 * 
 * @param i The value to print in hex.
 * @return The hex equivalent of the value in the form of a string.
 * 
 * @{
*/
std::string hex::to_hex8(uint8_t i)     ///< Print 8 byted as hex.
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}

std::string hex::to_hex32(uint32_t i)   ///< Print 32 bytes as hex.
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(i);
    return os.str();
}

std::string hex::to_hex0x12(uint32_t i) ///< Print 12 bytes as hex.
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(3) << (i & 0x00000fff);
    return std::string("0x") + os.str();
}

std::string hex::to_hex0x20(uint32_t i) ///< Print 20 bytes as hex.
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(5) << (i & 0x000fffff);
    return std::string("0x") + os.str();
}

std::string hex::to_hex0x32(uint32_t i) ///< Print 32 bytes as hex, led with 0x.
{
    return std::string("0x")+to_hex32(i);
}

/**@}*/