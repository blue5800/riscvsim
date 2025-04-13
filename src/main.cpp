#include <iostream>
#include "registers.h"
#include <cstdint>
#include <sys/types.h>

uint8_t get_opcode(int ins){
	return ins & 0b1111111;
}
uint8_t get_funct3(int ins){
	return (ins >> 12) & 0b111;
}
uint8_t get_rd(int ins){
	return (ins >> 7) & 0b11111;
}
uint8_t get_rs1(int ins){
	return (ins >> 15) & 0b11111;
}
int16_t get_imm(int ins){
	return (ins >> 20) & 0xFFF;
}
int main(int argc, char *argv[]) {
	//for now we're not using the args.
	int exit_code = 0;
	char memory[1024] = {0}; // Memory
	//addi format is [imm (12 bits) | rs (5 bits) | funct 3 000 | rd (5 bits) | opcode 0010011]
	int instruction = 0b01000000000100101000001010010011;
	*(int *)(memory + 0) = instruction;
	*(int *)(memory + 4) = instruction;
	*(int *)(memory + 8) = instruction;
	*(int *)(memory + 12) = instruction;
	*(int *)(memory + 16) = instruction;

	init_registers();
	
	while(1){
		unsigned int cur_addr = get_pc();
		unsigned int cur_instruction = 0;
		if (cur_addr >= 1024){
			std::cout << "PC out of bounds" << std::endl;
			exit_code = 1;
			break;
		}
		if(cur_addr % 4 != 0){
			std::cout << "PC not word aligned" << std::endl;
			exit_code = 1;
			break;
		}

		next_instruction();

		cur_instruction = *(int *)(memory + cur_addr);
		uint8_t opcode = get_opcode(cur_instruction);

		if (opcode == 0){
			std::cout << "NOP ends the program for now :D" << std::endl;
			break;
		}
		else if (opcode == 0b0010011){
			uint8_t funct3 = get_funct3(cur_instruction);
			if (funct3 == 0b000){
			//ADDI
			int16_t imm = get_imm(cur_instruction);
			uint8_t rs1 = get_rs1(cur_instruction);
			uint8_t rd = get_rd(cur_instruction);
			std::optional<unsigned int> rs1_val = get_register(rs1);
			if (!rs1_val.has_value()){
				std::cout << "Invalid rs1" << std::endl;
				exit_code = 1;
				break;
			}
			set_register(rd, rs1_val.value() + imm);
			}
			else{
			std::cout << "Unknown funct3 " << (int)funct3 << std::endl;
			exit_code = 1;
			break;
			}
		}
		else{
			std::cout << "Unknown opcode" << std::endl;
			exit_code = 1;
			break;
		
		}

	}
	
	print_registers();
	if (exit_code == 0){
		std::cout << "Program finished successfully" << std::endl;
	}
	else{
		std::cout << "Program finished with errors" << std::endl;
	}
}
