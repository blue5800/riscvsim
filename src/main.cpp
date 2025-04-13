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

uint8_t get_funct7(int ins){
	return (ins >> 25) & 0b1111111;
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

uint32_t get_Uimm(int ins){
	return (ins >> 12) & 0xFFFFF;
}

int main(int argc, char *argv[]) {
	//for now we're not using the args.
	int exit_code = 0;
	char memory[1024] = {0}; // Memory
	//addi format is [imm (12 bits) | rs (5 bits) | funct 3 000 | rd (5 bits) | opcode 0010011]

	int instruction1 = 0b01000000000100101000001010010011;
	int instruction2 = 0b10101010100100101110001010010011;
	int instruction3 = 0b00000000001100101001001010010011;
	*(int *)(memory + 0) = instruction1;
	*(int *)(memory + 4) = instruction2;
	*(int *)(memory + 8) = instruction1;
	*(int *)(memory + 12) = instruction1;
	*(int *)(memory + 16) = instruction3;

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

		cur_instruction = *(int *)(memory + cur_addr);
		uint8_t opcode = get_opcode(cur_instruction);

		if (opcode == 0){
			std::cout << "opcode 0 ends the program for now :D" << std::endl;
			break;
		}
		// i type instructions
		else if (opcode == 0b0010011){

			uint8_t funct3 = get_funct3(cur_instruction);
			int16_t imm = get_imm(cur_instruction);
			uint8_t rs1 = get_rs1(cur_instruction);
			uint8_t rd = get_rd(cur_instruction);

			std::optional<unsigned int> rs1_val = get_register(rs1);
			if (!rs1_val.has_value()){
				std::cout << "Invalid rs1" << std::endl;
				exit_code = 1;
				break;
			}
			switch (funct3){
				// addi
				case 0b000:
					set_register(rd, rs1_val.value() + imm);
					goto inc_pc;
					//continue;
				// slti
				case 0b010:
					set_register(rd, (int) rs1_val.value() < (int) imm ? 1 : 0);
					goto inc_pc;

					//continue;
				// sltiu
				case 0b011:
					set_register(rd, (unsigned int) rs1_val.value() < (unsigned int) imm ? 1 : 0);
					goto inc_pc;

					//continue;
				//andi
				case 0b111:
					set_register(rd, rs1_val.value() & imm);
					goto inc_pc;

					//continue;
				// ori
				case 0b110:
					set_register(rd, rs1_val.value() | imm);
					goto inc_pc;

					//continue;
				// xori
				case 0b100:
					set_register(rd, rs1_val.value() ^ imm);
					goto inc_pc;

					//continue;
				// slli
				case 0b001 :
					if (imm < 0 || imm > 31){
						std::cout << "Invalid immediate for slli" << std::endl;
						exit_code = 1;
						break;
					}
					set_register(rd, rs1_val.value() << imm);
					goto inc_pc;
					//continue;
				// srli or srai
				case 0b101:
					//special case: we need to consider the funct7, which will be the upper 7 bits of the immediate
					if(imm & 0b010000000000){
						// srai
						if ((imm & 0b11111) < 0 || (imm & 0b11111)  > 31){
							std::cout << "Invalid immediate for srai" << std::endl;
							exit_code = 1;
							break;
						}
						set_register(rd, (int) rs1_val.value() >> (imm & 0b11111));
						goto inc_pc;
					}
					else{
						// srli
						if (imm < 0 || imm > 31){
							std::cout << "Invalid immediate for srli" << std::endl;
							exit_code = 1;
							break;
						}
						set_register(rd, (unsigned int) rs1_val.value() >> imm);
						goto inc_pc;
					}
					//continue;
				default :
					std::cout << "Unknown funct3" << std::endl;
					exit_code = 1;
					break;
			}	
		}
		else if (opcode == 0b0110111){
			// lui
			uint8_t rd = get_rd(cur_instruction);
			uint32_t Uimm = get_Uimm(cur_instruction);
			set_register(rd, Uimm << 12);
			goto inc_pc;
			//continue;
		}
		else if (opcode == 0b0010111){
			// auipc
			uint8_t rd = get_rd(cur_instruction);
			uint32_t Uimm = get_Uimm(cur_instruction);
			set_register(rd, get_pc() + (Uimm << 12));
			goto inc_pc;

			//continue;
		}
		else if (opcode == 0b1101111){
			// jal
			uint8_t rd = get_rd(cur_instruction);
			int32_t Uimm = get_Uimm(cur_instruction);
			set_register(rd, get_pc() + 4);
			pc_jmp(Uimm << 1);
			goto inc_pc;

		}

		else{
			std::cout << "Unknown opcode" << std::endl;
			exit_code = 1;
			break;
		}
inc_pc:
		next_instruction();
	}
	
	print_registers();
	if (exit_code == 0){
		std::cout << "Program finished successfully" << std::endl;
	}
	else{
		std::cout << "Program finished with errors" << std::endl;
	}
}
