#include "handlers.h"
#include <stdint.h>
void exec_add(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] + cpu->REG[ins->rs2];
}
void exec_sub(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] - cpu->REG[ins->rs2];
}
void exec_sll(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] << (cpu->REG[ins->rs2] & 0x1F);
}
void exec_slt(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = ((int32_t)cpu->REG[ins->rs1] < (int32_t)cpu->REG[ins->rs2]) ? 1u : 0u;
}
void exec_sltu(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = (cpu->REG[ins->rs1] < cpu->REG[ins->rs2]) ? 1u : 0u;
}
void exec_xor(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] ^ cpu->REG[ins->rs2];
}
void exec_or(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] | cpu->REG[ins->rs2];
}
void exec_and(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] & cpu->REG[ins->rs2];
}
void exec_srl(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t shamt = cpu->REG[ins->rs2] & 0x1F;
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] >> shamt;
}
void exec_sra(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t shamt = cpu->REG[ins->rs2] & 0x1F;
    cpu->REG[ins->rd] = (int32_t)cpu->REG[ins->rs1] >> shamt;
}

/** I_FMT */
void exec_addi(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] + ins->imm;
}
void exec_slli(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t shamt = extract(ins->instr, 24, 20);
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] << shamt;
}
void exec_slti(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = ((int32_t)cpu->REG[ins->rs1] < ins->imm) ? 1u : 0u;
}
void exec_sltiu(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = (cpu->REG[ins->rs1] < (uint32_t)ins->imm) ? 1u : 0u;
}
void exec_xori(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] ^ ins->imm;
}
void exec_ori(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] | ins->imm;
}
void exec_andi(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] & ins->imm;
}
void exec_srli(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t shamt = extract(ins->instr, 24, 20);
    cpu->REG[ins->rd] = cpu->REG[ins->rs1] >> shamt;
}
void exec_srai(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t shamt = extract(ins->instr, 24, 20);
    cpu->REG[ins->rd] = (int32_t)cpu->REG[ins->rs1] >> shamt;
}

/** L_FMT */
void exec_lb(CPU* cpu, const DecodedInstr* ins)
{
    int32_t byte = (int8_t)DRAM[cpu->REG[ins->rs1] + ins->imm];
    cpu->REG[ins->rd] = byte;
}
void exec_lh(CPU* cpu, const DecodedInstr* ins)
{
    uint8_t b0 = (uint8_t)DRAM[cpu->REG[ins->rs1] + ins->imm];
    uint8_t b1 = (uint8_t)DRAM[cpu->REG[ins->rs1] + ins->imm + 1];
    int32_t hw = sign_extend(((uint32_t)b1 << 8) | b0, 16);
    cpu->REG[ins->rd] = hw;
}
void exec_lw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1] + ins->imm;
    uint8_t b0 = (uint8_t)DRAM[addr];
    uint8_t b1 = (uint8_t)DRAM[addr + 1];
    uint16_t hw0 = ((uint16_t)b1 << 8) | b0;

    uint8_t b2 = (uint8_t)DRAM[addr + 2];
    uint8_t b3 = (uint8_t)DRAM[addr + 3];
    uint16_t hw1 = ((uint16_t)b3 << 8) | b2;

    uint32_t word = ((uint32_t)hw1 << 16) | hw0;
    cpu->REG[ins->rd] = word;
}
void exec_lbu(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t byte = DRAM[cpu->REG[ins->rs1] + ins->imm];
    cpu->REG[ins->rd] = byte;
}
void exec_lhu(CPU* cpu, const DecodedInstr* ins)
{
    uint8_t b0 = (uint8_t)DRAM[cpu->REG[ins->rs1] + ins->imm];
    uint8_t b1 = (uint8_t)DRAM[cpu->REG[ins->rs1] + ins->imm + 1];
    uint32_t halfWord = b1 << 8 | b0;
    cpu->REG[ins->rd] = halfWord;
}

/** S_FMT */
void exec_sb(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1] + ins->imm;
    uint32_t data = cpu->REG[ins->rs2];
    uint8_t byte = (uint8_t)(data & 0x0FF);
    DRAM[addr] = byte;
}
void exec_sh(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1] + ins->imm;
    uint32_t data = cpu->REG[ins->rs2];
    uint8_t b0 = (data & 0x0FF);
    uint8_t b1 = (data >> 8) & 0xFF;
    DRAM[addr] = b0;
    DRAM[addr + 1] = b1;
}
void exec_sw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1] + ins->imm;
    uint32_t data = cpu->REG[ins->rs2];
    uint8_t b0 = (data & 0x0FF);
    uint8_t b1 = (data >> 8) & 0xFF;
    uint8_t b2 = (data >> 16) & 0xFF;
    uint8_t b3 = (data >> 24) & 0xFF;
    DRAM[addr] = b0;
    DRAM[addr + 1] = b1;
    DRAM[addr + 2] = b2;
    DRAM[addr + 3] = b3;
}

/** B_FMT */
void exec_beq(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 13);
    if (cpu->REG[ins->rs1] == cpu->REG[ins->rs2])
    {
        cpu->PC += offset;
        cpu->pc_set = 1;
    }
}
void exec_bne(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 13);
    if (cpu->REG[ins->rs1] != cpu->REG[ins->rs2])
    {
        cpu->PC += offset;
        cpu->pc_set = 1;
    }
}
void exec_blt(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 13);
    if ((int32_t)cpu->REG[ins->rs1] < (int32_t)cpu->REG[ins->rs2])
    {
        cpu->PC += offset;
        cpu->pc_set = 1;
    }
}
void exec_bge(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 13);
    if ((int32_t)cpu->REG[ins->rs1] >= (int32_t)cpu->REG[ins->rs2])
    {
        cpu->PC += offset;
        cpu->pc_set = 1;
    }
}
void exec_bltu(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 13);
    if ((uint32_t)cpu->REG[ins->rs1] < (uint32_t)cpu->REG[ins->rs2])
    {
        cpu->PC += offset;
        cpu->pc_set = 1;
    }
}
void exec_bgeu(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 13);
    if ((uint32_t)cpu->REG[ins->rs1] >= (uint32_t)cpu->REG[ins->rs2])
    {
        cpu->PC += offset;
        cpu->pc_set = 1;
    }
}

/** J_FMT */
void exec_jal(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 21);
    cpu->REG[ins->rd] = cpu->PC + 4;
    cpu->PC = cpu->PC + offset;
    cpu->pc_set = 1;
}
void exec_jalr(CPU* cpu, const DecodedInstr* ins)
{
    int32_t offset = sign_extend(ins->imm, 12);
    cpu->REG[ins->rd] = cpu->PC + 4;
    cpu->PC = (cpu->REG[ins->rs1] + offset) & ~1u;
    cpu->pc_set = 1;
}

/** */
void exec_lui(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = (uint32_t)ins->imm;
}
void exec_auipc(CPU* cpu, const DecodedInstr* ins)
{
    cpu->REG[ins->rd] = cpu->PC + (uint32_t)ins->imm;
}

/** System */
void exec_debug(CPU* cpu, const DecodedInstr* ins)
{
    (void)cpu;
    (void)ins;
}
void exec_osctlr(CPU* cpu, const DecodedInstr* ins)
{
    (void)cpu;
    (void)ins;
}
