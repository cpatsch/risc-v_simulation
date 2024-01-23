#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

using std::cout;
using std::endl;
using std::cerr;

//***************************************************************************
//
//  Caleb Patsch
//  11/04/2022
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

class hex
{
    public :
        static std :: string to_hex8 ( uint8_t i );
        static std :: string to_hex32 ( uint32_t i );
        static std :: string to_hex0x12( uint32_t i );
        static std :: string to_hex0x20( uint32_t i );
        static std :: string to_hex0x32 ( uint32_t i );
};
