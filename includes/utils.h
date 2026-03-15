#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
typedef struct DecodedInstr
{
    uint32_t instr;
    uint32_t opcode;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t funct3;
    uint32_t funct7;
    int32_t imm;
} DecodedInstr;

DecodedInstr decode_basic(uint32_t instr);
void fill_immediate(DecodedInstr* ins);
uint32_t mem_read8(uint32_t addr);
uint32_t mem_read16(uint32_t addr);
uint32_t mem_read32(uint32_t addr);

void mem_write8(uint32_t addr, uint32_t value);
void mem_write16(uint32_t addr, uint32_t value);
void mem_write32(uint32_t addr, uint32_t value);
#endif
