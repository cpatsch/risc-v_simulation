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
 * @brief Dissassembles the simulated memory.
 * Goes through the memory one byte at a time and decodes it.
 * 
 * @param mem The simulated memory.
*/
static void disassemble(const memory &mem)
{
	for (uint32_t i = 0; i < mem.get_size(); i += 4)
	{
		cout << hex::to_hex32(i) << ": "<< hex::to_hex32(mem.get32(i)) << "  " << rv32i_decode::decode(i, mem.get32(i)) << endl;
	}
}

/**
 * @brief Print the usage of the program.
*/
static void usage()
{
	cerr << "Usage: rv32i [-d] [-i] [-l execution-limit] [-m hex-mem-size] [-r] [-z] infile" << endl;
	cerr << "    -d show disassembly before program execution" << endl;
	cerr << "    -i show instruction printing during execution" << endl;
	cerr << "    -l maximum number of instructions to exec" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	cerr << "    -r show register printing during exectuion" << endl;
	cerr << "    -z show a dump of the regs & memory after simulation" << endl;
	exit(1);
}
/**
 * @brief The main program.
 * 
 * @param argc The number of arguments.
 * @param argv Pointer array of the arguments.
 * 
 * @return Returns 0 if the program succeeds.
*/
int main(int argc, char **argv)
{
	bool show_disassembly = false;
	bool show_instructions = false;
	bool show_regs = false;
	bool show_dump = false;
	uint32_t memory_limit = 0x100; // default memory size = 256 bytes
	uint32_t exec_limit = 0x000;

	int opt;
	while ((opt = getopt(argc, argv, "dirzl:m:")) != -1)
	{
		switch (opt)
		{
		case 'd':
			{
				show_disassembly = true;
			}
			break;
		case 'i':
			{
				show_instructions = true;
			}
			break;
		case 'l':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> exec_limit;
			}
			break;
		case 'm':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> memory_limit;
			}
			break;
		case 'r':
			{
				show_regs = true;
			}
			break;
		case 'z':
			{
				show_dump = true;
			}
			break;
		default: /* ’?’ */
			usage();
		}
	}

	if (optind >= argc)
		usage(); // missing filename

	memory mem(memory_limit);

	if (!mem.load_file(argv[optind]))
		usage();

	if (show_disassembly)
	{
		disassemble(mem);
	}

	cpu_single_hart cpu(mem);
	cpu.reset();

	if (show_instructions)
	{
		cpu.set_show_instructions(true);
	}

	if (show_regs)
	{
		cpu.set_show_registers(true);
	}

	cpu.run(exec_limit);

	if (show_dump)
	{
		cpu.dump();
		mem.dump();
	}

	return 0;
}