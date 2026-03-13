#include "CPU.h"
#include "FN3.h"
#include "FN7.h"
#include "OPCODE.h"
#include <assert.h>

uint8_t DRAM[MEM_SIZE] = {0};

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

void execute(uint32_t instr, CPU* cpu)
{
    uint32_t opcode = extract(instr, 6, 0);
    uint32_t rd = extract(instr, 11, 7);
    uint32_t fn_3 = extract(instr, 14, 12);
    uint32_t rs1 = extract(instr, 19, 15);
    uint32_t rs2 = extract(instr, 24, 20);
    uint32_t fn_7 = extract(instr, 31, 25);

    switch (opcode)
    {
    case R_FMT:
    {
        switch ((R_FMT_FN3)fn_3)
        {
        case ADD_SUB:
            if (fn_7 == ADD_SRL)
            {
                cpu->REG[rd] = cpu->REG[rs1] + cpu->REG[rs2];
            }
            else if (fn_7 == SUB_SRA)
            {
                cpu->REG[rd] = cpu->REG[rs1] - cpu->REG[rs2];
            }
            else
                goto illegal;
            break;
        case SLL:
            cpu->REG[rd] = cpu->REG[rs1] << (cpu->REG[rs2] & 0x1F);
            break;

        case SLT:
            cpu->REG[rd] = ((int32_t)cpu->REG[rs1] < (int32_t)cpu->REG[rs2]) ? 1u : 0u;
            break;

        case SLTU:
            cpu->REG[rd] = (cpu->REG[rs1] < cpu->REG[rs2]) ? 1u : 0u;
            break;

        case XOR:
            cpu->REG[rd] = cpu->REG[rs1] ^ cpu->REG[rs2];
            break;

        case SRL_SRA:
        {
            uint32_t shamt = cpu->REG[rs2] & 0x1F;

            if (fn_7 == ADD_SRL)
            {
                cpu->REG[rd] = cpu->REG[rs1] >> shamt;
            }
            else if (fn_7 == SUB_SRA)
            {
                cpu->REG[rd] = (int32_t)cpu->REG[rs1] >> shamt;
            }
            else
            {
                goto illegal;
            }
            break;
        }

        case OR:
            cpu->REG[rd] = cpu->REG[rs1] | cpu->REG[rs2];
            break;

        case AND:
            cpu->REG[rd] = cpu->REG[rs1] & cpu->REG[rs2];
            break;

        default:
            goto illegal;
        }
        break;
    }

    case I_FMT:
    {
        uint32_t imm12 = extract(instr, 31, 20);
        int32_t imm = sign_extend(imm12, 12);

        switch ((I_FMT_FN3)fn_3)
        {
        case ADDI:
            cpu->REG[rd] = cpu->REG[rs1] + imm;
            break;

        case SLLI:
        {
            uint32_t shamt = extract(instr, 24, 20);
            if (fn_7 != SRL)
            {
                goto illegal;
            }
            cpu->REG[rd] = cpu->REG[rs1] << shamt;
            break;
        }

        case SLTI:
            cpu->REG[rd] = ((int32_t)cpu->REG[rs1] < imm) ? 1u : 0u;
            break;

        case SLTIU:
            cpu->REG[rd] = (cpu->REG[rs1] < (uint32_t)imm) ? 1u : 0u;
            break;

        case XORI:
            cpu->REG[rd] = cpu->REG[rs1] ^ imm;
            break;

        case SRLI_SRAI:
        {
            uint32_t shamt = extract(instr, 24, 20);

            if (fn_7 == SRL)
            {
                cpu->REG[rd] = cpu->REG[rs1] >> shamt;
            }
            else if (fn_7 == SRA)
            {
                cpu->REG[rd] = (int32_t)cpu->REG[rs1] >> shamt;
            }
            else
            {
                goto illegal;
            }
            break;
        }

        case ORI:
            cpu->REG[rd] = cpu->REG[rs1] | imm;
            break;

        case ANDI:
            cpu->REG[rd] = cpu->REG[rs1] & imm;
            break;

        default:
            goto illegal;
        }

        break;
    }

    case LOAD:
    {
        int32_t imm = sign_extend(extract(instr, 31, 20), 12);

        switch (fn_3)
        {

        case LB:
        {
            int32_t byte = (int8_t)DRAM[cpu->REG[rs1] + imm];
            cpu->REG[rd] = byte;
            break;
        }

        case LH:
        {
            uint8_t b0 = (uint8_t)DRAM[cpu->REG[rs1] + imm];
            uint8_t b1 = (uint8_t)DRAM[cpu->REG[rs1] + imm + 1];
            int32_t hw = sign_extend(((uint32_t)b1 << 8) | b0, 16);
            cpu->REG[rd] = hw;
            break;
        }

        case LW:
        {
            uint32_t addr = cpu->REG[rs1] + imm;
            uint8_t b0 = (uint8_t)DRAM[addr];
            uint8_t b1 = (uint8_t)DRAM[addr + 1];
            uint16_t hw0 = ((uint16_t)b1 << 8) | b0;

            uint8_t b2 = (uint8_t)DRAM[addr + 2];
            uint8_t b3 = (uint8_t)DRAM[addr + 3];
            uint16_t hw1 = ((uint16_t)b3 << 8) | b2;

            uint32_t word = ((uint32_t)hw1 << 16) | hw0;
            cpu->REG[rd] = word;
            break;
        }

        case LBU:
        {
            uint32_t byte = DRAM[cpu->REG[rs1] + imm];
            cpu->REG[rd] = byte;
            break;
        }

        case LHU:
        {
            uint8_t b0 = (uint8_t)DRAM[cpu->REG[rs1] + imm];
            uint8_t b1 = (uint8_t)DRAM[cpu->REG[rs1] + imm + 1];
            uint32_t halfWord = b1 << 8 | b0;
            cpu->REG[rd] = halfWord;
            break;
        }

        default:
            goto illegal;
        }
        break;
    }

    case STORE:
    {
        // S-type immediate: imm[11:5] | imm[4:0]
        uint32_t imm_lo = extract(instr, 11, 7);
        uint32_t imm_hi = extract(instr, 31, 25);
        uint32_t imm12 = (imm_hi << 5) | imm_lo;
        int32_t imm = sign_extend(imm12, 12);

        uint32_t addr = cpu->REG[rs1] + imm;
        uint32_t data = cpu->REG[rs2];

        switch (fn_3)
        {
        case SB:
        {
            uint8_t byte = (uint8_t)(data & 0x0FF);
            DRAM[addr] = byte;
            break;
        }

        case SH:
        {
            uint8_t b0 = (data & 0x0FF);
            uint8_t b1 = (data >> 8) & 0xFF;
            DRAM[addr] = b0;
            DRAM[addr + 1] = b1;
            break;
        }

        case SW:
        {
            uint8_t b0 = (data & 0x0FF);
            uint8_t b1 = (data >> 8) & 0xFF;
            uint8_t b2 = (data >> 16) & 0xFF;
            uint8_t b3 = (data >> 24) & 0xFF;
            DRAM[addr] = b0;
            DRAM[addr + 1] = b1;
            DRAM[addr + 2] = b2;
            DRAM[addr + 3] = b3;
            break;
        }
        default:
            goto illegal;
        }

        break;
    }

    case BRANCH:
    {

        uint32_t imm12 = extract(instr, 31, 31);
        uint32_t imm10_5 = extract(instr, 30, 25);
        uint32_t imm4_1 = extract(instr, 11, 8);
        uint32_t imm11 = extract(instr, 7, 7);
        uint32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
        int32_t offset = sign_extend(imm, 13);

        switch (fn_3)
        {
        case BEQ:
            if (cpu->REG[rs1] == cpu->REG[rs2])
            {
                cpu->PC += offset;
                cpu->pc_set = 1;
            }
            break;
        case BNE:
            if (cpu->REG[rs1] != cpu->REG[rs2])
            {
                cpu->PC += offset;
                cpu->pc_set = 1;
            }
            break;
        case BLT:
            if ((int32_t)cpu->REG[rs1] < (int32_t)cpu->REG[rs2])
            {
                cpu->PC += offset;
                cpu->pc_set = 1;
            }

            break;
        case BGE:
            if ((int32_t)cpu->REG[rs1] >= (int32_t)cpu->REG[rs2])
            {
                cpu->PC += offset;
                cpu->pc_set = 1;
            }
            break;
        case BLTU:
            if ((uint32_t)cpu->REG[rs1] < (uint32_t)cpu->REG[rs2])
            {
                cpu->PC += offset;
                cpu->pc_set = 1;
            }
            break;
        case BGEU:
            if ((uint32_t)cpu->REG[rs1] >= (uint32_t)cpu->REG[rs2])
            {
                cpu->PC += offset;
                cpu->pc_set = 1;
            }
            break;
        default:
            goto illegal;
        }
    }
    break;
    case JAL:
    {
        uint32_t imm_10_1 = extract(instr, 30, 21);
        uint32_t imm_20 = extract(instr, 31, 31);
        uint32_t imm_11 = extract(instr, 20, 20);
        uint32_t imm_19_12 = extract(instr, 19, 12);
        uint32_t imm21 = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);
        int32_t offset = sign_extend(imm21, 21);
        cpu->REG[rd] = cpu->PC + 4;
        cpu->PC = cpu->PC + offset;
        cpu->pc_set = 1;
        break;
    }
    case JALR:
    {
        uint32_t imm12 = extract(instr, 31, 20);
        int32_t offset = sign_extend(imm12, 12);
        cpu->REG[rd] = cpu->PC + 4;
        cpu->PC = (cpu->REG[rs1] + offset) & ~1u;
        cpu->pc_set = 1;
        break;
    }
    case LUI:
    {
        uint32_t imm = extract(instr, 31, 12);
        cpu->REG[rd] = imm << 12;
        break;
    }
    case AUIPC:
    {
        uint32_t imm = extract(instr, 31, 12);
        cpu->REG[rd] = cpu->PC + (imm << 12);
        break;
    }
    case SYSTEM:
    {
        switch (fn_7)
        {
        case DEBUG:
            break;
        case OS:
            break;
        default:
            goto illegal;
        }
        break;
    }
    default:
        goto illegal;
    }

illegal:
    // trap / halt / error handling
    cpu->halted = 1;
    return;
}

void step(CPU* cpu)
{
    cpu->PC += INSTR_LEN;
}
