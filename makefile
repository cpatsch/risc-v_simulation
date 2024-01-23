#
# PROGRAM: rv32i
# AUTHOR:  Caleb Patsch
#

CXXFLAGS = -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14

rv32i: main.o hex.o memory.o rv32i_decode.o registerfile.o rv32i_hart.o cpu_single_hart.o
	g++ $(CXXFLAGS) -o rv32i main.o hex.o memory.o rv32i_decode.o registerfile.o rv32i_hart.o cpu_single_hart.o

main.o: main.cpp
	g++ $(CXXFLAGS) -c main.cpp

hex.o: hex.cpp
	g++ $(CXXFLAGS) -c hex.cpp

memory.o: memory.cpp
	g++ $(CXXFLAGS) -c memory.cpp

rv32i_decode.o: rv32i_decode.cpp
	g++ $(CXXFLAGS) -c rv32i_decode.cpp

registerfile.o: registerfile.cpp
	g++ $(CXXFLAGS) -c registerfile.cpp

rv32i_hart.o: rv32i_hart.cpp
	g++ $(CXXFLAGS) -c rv32i_hart.cpp

cpu_single_hart.o: cpu_single_hart.cpp
	g++ $(CXXFLAGS) -c cpu_single_hart.cpp

clean:
	rm -f *.o rv32i
