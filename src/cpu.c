#include "CPU.h"
#include "table.h"
#include "utils.h"
uint8_t DRAM[MEM_SIZE] = {0};
void execute(uint32_t instr, CPU* cpu)
{
    DecodedInstr ins = decode_basic(instr);
    fill_immediate(&ins);
    const InstructionDecsriptor* desc = find_instruction(&ins);
    if (!desc)
    {
        cpu->halted = 1;
        return;
    }
    desc->handler(cpu, &ins);
}

void step(CPU* cpu)
{
    cpu->PC += INSTR_LEN;
}
