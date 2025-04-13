#include <optional>
#include <string>
/*
 * @brief initialises all registers to 0
*/
void init_registers();
/*
 * @brief gets the value of a given registeer.
 * @params reg_num the register index
 * @return empty if invalid reg, otherwise value of register
*/
std::optional<unsigned int> get_register(int reg_num);
/*
 * @brief gets the value of a given registeer.
 * @params reg_name the name of the register
 * @return empty if invalid reg, otherwise value of register
 */
std::optional<unsigned int> get_register(std::string reg_name);
/*
 * @brief sets the register to value
 * @params reg_num register index
 * @params value the new register value
*/
void set_register(int reg_num, int value);
/*
 * @brief adds the offset to the PC
 */
void pc_jmp(int offset);
/*
 * @sets the PC for the next instruction
 */
void next_instruction();

unsigned int get_pc();

void print_registers();
