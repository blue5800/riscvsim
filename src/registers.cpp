#include "registers.h"
#include <cstdint>
#include <map>
#include <iostream>
#include <iomanip>
unsigned int registers[32] = {0}; // General purpose registers
unsigned int pc = 0; // Program counter

std::map<std::string,uint8_t> abi_to_reg = {
    {"zero", 0},
    {"ra", 1},
    {"sp", 2},
    {"gp", 3},
    {"tp", 4},
    {"t0", 5},
    {"t1", 6},
    {"t2", 7},
    {"s0", 8},
    {"s1", 9},
    {"a0", 10},
    {"a1", 11},
    {"a2", 12},
    {"a3", 13},
    {"a4", 14},
    {"a5", 15},
    {"a6", 16},
    {"a7", 17},
    {"s2", 18},
    {"s3", 19},
    {"s4", 20},
    {"s5", 21},
    {"s6", 22},
    {"s7", 23},
    {"s8", 24},
    {"s9", 25},
    {"s10", 26},
    {"s11", 27},
    {"t3", 28},
    {"t4", 29},
    {"t5", 30},
    {"t6", 31}
};



void init_registers(){
    for (int i = 0; i < 32; i++){
	registers[i] = 0;
    }
    pc = 0;
}


std::optional<unsigned int> get_register(int reg_num){
    if (reg_num < 0 || reg_num >= 32){
	return {};
    }
    return registers[reg_num];
}

std::optional<unsigned int> get_register(std::string reg_name){
    auto it = abi_to_reg.find(reg_name);
    if (it != abi_to_reg.end()){
	return registers[it->second];
    }
    return {};
}

void set_register(int reg_num, int value){
    if (reg_num <= 0 || reg_num >= 32) {
	return; // Invalid register number
    }
    registers[reg_num] = value;
}
unsigned int get_pc(){
    return pc;
}
void pc_jmp(int offset){
    pc += offset;
}

void next_instruction(){
    pc += 4;
}

void print_registers(){
	for(auto const &pair : abi_to_reg){
		std::cout << pair.first << ": 0x" << std::hex << std::setw(8) << std::setfill('0') << registers[pair.second] << std::endl;
	}
	std::cout << "PC: 0x" << std::hex << std::setw(8) << std::setfill('0') << pc << std::endl;
}



