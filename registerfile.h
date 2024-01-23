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

class registerfile : public hex
{
    public:
        registerfile();
        ~registerfile();

        int32_t get(uint32_t r) const;

        void set(uint32_t r, int32_t val);

        void reset();
        void dump(const std::string &hdr) const;

    protected:
        static constexpr int num_regs = 32;

    private:
        std::vector <int32_t> reg;
};