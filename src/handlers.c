#include "handlers.h"
#include "utils.h"
#include <limits.h>
#include <stdint.h>

// TODO: implement addr misalignment checks
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
void exec_mul(CPU* cpu, const DecodedInstr* ins)
{
    uint64_t product = (int64_t)(int32_t)cpu->REG[ins->rs1] * (int32_t)cpu->REG[ins->rs2];
    cpu->REG[ins->rd] = (uint32_t)product;
}
void exec_mulh(CPU* cpu, const DecodedInstr* ins)
{
    uint64_t product = (int64_t)(int32_t)cpu->REG[ins->rs1] * (int32_t)cpu->REG[ins->rs2];
    cpu->REG[ins->rd] = (uint32_t)(product >> 32);
}
void exec_mulhu(CPU* cpu, const DecodedInstr* ins)
{
    uint64_t product = (uint64_t)cpu->REG[ins->rs1] * (uint32_t)cpu->REG[ins->rs2];
    cpu->REG[ins->rd] = (uint32_t)(product >> 32);
}
void exec_mulhsu(CPU* cpu, const DecodedInstr* ins)
{
    uint64_t product = (int64_t)(int32_t)cpu->REG[ins->rs1] * (uint32_t)cpu->REG[ins->rs2];
    cpu->REG[ins->rd] = (uint32_t)(product >> 32);
}

void exec_div(CPU* cpu, const DecodedInstr* ins)
{
    int32_t a = (int32_t)cpu->REG[ins->rs1];
    int32_t b = (int32_t)cpu->REG[ins->rs2];
    uint32_t result;

    if (b == 0)
    {
        result = 0xFFFFFFFFu;
    }
    else if (a == INT32_MIN && b == -1)
    {
        result = (uint32_t)INT32_MIN;
    }
    else
    {
        result = (uint32_t)(a / b);
    }

    cpu->REG[ins->rd] = result;
}

void exec_divu(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t a = (uint32_t)cpu->REG[ins->rs1];
    uint32_t b = (uint32_t)cpu->REG[ins->rs2];

    if (b == 0)
    {
        cpu->REG[ins->rd] = 0xFFFFFFFFu;
    }
    else
    {
        cpu->REG[ins->rd] = a / b;
    }
}

void exec_rem(CPU* cpu, const DecodedInstr* ins)
{
    int32_t a = (int32_t)cpu->REG[ins->rs1];
    int32_t b = (int32_t)cpu->REG[ins->rs2];
    uint32_t result;

    if (b == 0)
    {
        result = (uint32_t)a;
    }
    else if (a == INT32_MIN && b == -1)
    {
        result = 0;
    }
    else
    {
        result = (uint32_t)(a % b);
    }

    cpu->REG[ins->rd] = result;
}

void exec_remu(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t a = (uint32_t)cpu->REG[ins->rs1];
    uint32_t b = (uint32_t)cpu->REG[ins->rs2];

    if (b == 0)
    {
        cpu->REG[ins->rd] = a;
    }
    else
    {
        cpu->REG[ins->rd] = a % b;
    }
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

void exec_lrw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t word = mem_read32(addr);
    cpu->REG[ins->rd] = word;
    cpu->reservation = addr;
    cpu->reservation_valid = true;
}
void exec_scw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t value = cpu->REG[ins->rs2];
    // SC.W requires 4-byte alignment
    if (addr & 0x3)
    {
        // raise_exception(cpu, STORE_ADDRESS_MISALIGNED, addr);
        return;
    }

    if (cpu->reservation_valid && cpu->reservation == addr)
    {
        mem_write32(addr, value);
        cpu->REG[ins->rd] = 0; // success
    }
    else
    {
        cpu->REG[ins->rd] = 1; // failure
    }
    cpu->reservation_valid = false;
}
void exec_amoaddw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t old = mem_read32(addr);
    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    cpu->REG[ins->rd] = old;
    uint32_t result = cpu->REG[ins->rs2] + old;
    mem_write32(addr, result);
    cpu->reservation_valid = false;
}
void exec_amoandw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t old = mem_read32(addr);
    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    cpu->REG[ins->rd] = old;
    uint32_t result = cpu->REG[ins->rs2] & old;
    mem_write32(addr, result);
    cpu->reservation_valid = false;
}
void exec_amoorw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t old = mem_read32(addr);
    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    cpu->REG[ins->rd] = old;
    uint32_t result = cpu->REG[ins->rs2] | old;
    mem_write32(addr, result);
    cpu->reservation_valid = false;
}
void exec_amoxorw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t old = mem_read32(addr);
    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    cpu->REG[ins->rd] = old;
    uint32_t result = cpu->REG[ins->rs2] ^ old;
    mem_write32(addr, result);
    cpu->reservation_valid = false;
}
void exec_amoswapw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t old = mem_read32(addr);
    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    cpu->REG[ins->rd] = old;
    uint32_t result = cpu->REG[ins->rs2];
    mem_write32(addr, result);
    cpu->reservation_valid = false;
}
void exec_amomaxw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    int32_t old = (int32_t)mem_read32(addr);
    int32_t src = (int32_t)cpu->REG[ins->rs2];

    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    int32_t result = (src > old) ? src : old;
    mem_write32(addr, (uint32_t)result);
    cpu->reservation_valid = false;
}

void exec_amominw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    int32_t old = (int32_t)mem_read32(addr);
    int32_t src = (int32_t)cpu->REG[ins->rs2];

    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    int32_t result = (src < old) ? src : old;
    mem_write32(addr, (uint32_t)result);
    cpu->reservation_valid = false;
}

void exec_amomaxuw(CPU* cpu, const DecodedInstr* ins)
{
    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t old = mem_read32(addr);
    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    cpu->REG[ins->rd] = old;
    uint32_t result = old;
    if (cpu->REG[ins->rs2] > old)
    {
        result = cpu->REG[ins->rs2];
    }
    mem_write32(addr, result);
    cpu->reservation_valid = false;
}
void exec_amominuw(CPU* cpu, const DecodedInstr* ins)
{

    uint32_t addr = cpu->REG[ins->rs1];
    uint32_t old = mem_read32(addr);
    if (ins->rd != 0)
        cpu->REG[ins->rd] = (uint32_t)old;

    cpu->REG[ins->rd] = old;
    uint32_t result = old;
    if (cpu->REG[ins->rs2] < old)
    {
        result = cpu->REG[ins->rs2];
    }
    mem_write32(addr, result);
    cpu->reservation_valid = false;
}
