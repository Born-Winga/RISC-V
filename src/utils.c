#include "utils.h"
#include "CPU.h"
#include "OPCODE.h"
#include <assert.h>
#include <stdlib.h>

// TODO: Implement Memory utils
DecodedInstr decode_basic(uint32_t instr)
{
    DecodedInstr ins = {0};
    ins.instr = instr;
    ins.opcode = extract(instr, 6, 0);
    ins.rd = extract(instr, 11, 7);
    ins.funct3 = extract(instr, 14, 12);
    ins.rs1 = extract(instr, 19, 15);
    ins.rs2 = extract(instr, 24, 20);
    ins.funct7 = extract(instr, 31, 25);

    return ins;
}

void fill_immediate(DecodedInstr* ins)
{
    switch (ins->opcode)
    {
    case I_FMT:
    case LOAD:
    case JALR:
    case SYSTEM:
        ins->imm = sign_extend(extract(ins->instr, 31, 20), 12);
        break;

    case STORE:
    {
        uint32_t imm_lo = extract(ins->instr, 11, 7);
        uint32_t imm_hi = extract(ins->instr, 31, 25);
        ins->imm = sign_extend((imm_hi << 5) | imm_lo, 12);
        break;
    }

    case BRANCH:
    {
        uint32_t imm12 = extract(ins->instr, 31, 31);
        uint32_t imm10_5 = extract(ins->instr, 30, 25);
        uint32_t imm4_1 = extract(ins->instr, 11, 8);
        uint32_t imm11 = extract(ins->instr, 7, 7);
        uint32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
        ins->imm = sign_extend(imm, 13);
        break;
    }

    case LUI:
    case AUIPC:
        ins->imm = (int32_t)(extract(ins->instr, 31, 12) << 12);
        break;

    case JAL:
    {
        uint32_t imm20 = extract(ins->instr, 31, 31);
        uint32_t imm10_1 = extract(ins->instr, 30, 21);
        uint32_t imm11 = extract(ins->instr, 20, 20);
        uint32_t imm19_12 = extract(ins->instr, 19, 12);
        uint32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);
        ins->imm = sign_extend(imm, 21);
        break;
    }

    default:
        ins->imm = 0;
        break;
    }
}

uint32_t fetch(CPU* cpu)
{
    uint32_t addr = cpu->PC;
    uint32_t instr = (uint32_t)DRAM[addr] | ((uint32_t)DRAM[addr + 1] << 8) |
                     ((uint32_t)DRAM[addr + 2] << 16) | ((uint32_t)DRAM[addr + 3] << 24);
    /** OR  memcpy(&instr, DRAM[addr], 4); for little endian*/
    return instr;
}

uint32_t extract(uint32_t instr, uint32_t hi, uint32_t lo)
{
    assert(lo <= hi);
    assert(hi < 32);
    uint32_t width = hi - lo + 1;
    if (width == 32)
        return instr;
    uint32_t mask = ((uint32_t)1u << width) - 1u;
    return (instr >> lo) & mask;
}

int32_t sign_extend(uint32_t value, size_t width)
{
    if (width == 0)
    {
        return 0;
    }
    if (width >= 32)
    {
        return (int32_t)value;
    }

    value = zero_extend(value, width);
    uint32_t sign_mask = 1u << (width - 1);
    return (int32_t)((value ^ sign_mask) - sign_mask);
}

uint32_t zero_extend(uint32_t value, size_t width)
{
    if (width == 0 || width >= 32)
    {
        return value;
    }

    uint32_t mask = (1u << width) - 1u;
    return value & mask;
}

uint32_t mem_read8(uint32_t addr)
{
    uint32_t byte = (uint8_t)DRAM[addr];
    return byte;
}

uint32_t mem_read16(uint32_t addr)
{
    uint32_t b0 = (uint8_t)DRAM[addr];
    uint32_t b1 = (uint8_t)DRAM[addr + 1];
    uint32_t hw = (b1 << 8) | b0;
    return hw;
}

uint32_t mem_read32(uint32_t addr)
{
    uint32_t b0 = (uint8_t)DRAM[addr];
    uint32_t b1 = (uint8_t)DRAM[addr + 1];
    uint32_t b2 = (uint8_t)DRAM[addr + 2];
    uint32_t b3 = (uint8_t)DRAM[addr + 3];
    return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}

bool is_valid_addr(uint32_t addr)
{
    return addr + 3 <= MEM_SIZE;
}

void mem_write32(uint32_t addr, uint32_t value)
{
    DRAM[addr] = (uint8_t)(value & 0xFF);
    DRAM[addr + 1] = (uint8_t)((value >> 8) & 0xFF);
    DRAM[addr + 2] = (uint8_t)((value >> 16) & 0xFF);
    DRAM[addr + 3] = (uint8_t)((value >> 24) & 0xFF);
}

void mem_write16(uint32_t addr, uint32_t value)
{
    DRAM[addr] = (uint8_t)(value & 0xFF);
    DRAM[addr + 1] = (uint8_t)((value >> 8) & 0xFF);
}

void mem_write8(uint32_t addr, uint32_t value)
{
    DRAM[addr] = (uint8_t)(value & 0xFF);
}
