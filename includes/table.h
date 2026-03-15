#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H
#include "CPU.h"
#include "FN3.h"
#include "FN7.h"
#include "OPCODE.h"
#include "handlers.h"
#include "utils.h"
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MATCH_ANY (-1)

typedef struct
{
    const char* name;
    uint32_t opcode;
    int funct3;
    int funct7;
    void (*handler)(CPU* cpu, const DecodedInstr* instr);
} InstructionDecsriptor;

static const InstructionDecsriptor instruction_table[] = {
    {"add", R_FMT, ADD_SUB, ADD_SRL, exec_add},
    {"sub", R_FMT, ADD_SUB, SUB_SRA, exec_sub},
    {"sll", R_FMT, SLL, ADD_SRL, exec_sll},
    {"slt", R_FMT, SLT, ADD_SRL, exec_slt},
    {"sltu", R_FMT, SLTU, ADD_SRL, exec_sltu},
    {"xor", R_FMT, XOR, ADD_SRL, exec_xor},
    {"srl", R_FMT, SRL_SRA, ADD_SRL, exec_srl},
    {"sra", R_FMT, SRL_SRA, SUB_SRA, exec_sra},
    {"or", R_FMT, OR, ADD_SRL, exec_or},
    {"and", R_FMT, AND, ADD_SRL, exec_and},
    {"mul", R_FMT, MUL, MUL_EXT, exec_mul},
    {"mulh", R_FMT, MULH, MUL_EXT, exec_mulh},
    {"mulhsu", R_FMT, MULHSU, MUL_EXT, exec_mulhsu},
    {"mulhu", R_FMT, MULHU, MUL_EXT, exec_mulhu},
    {"div", R_FMT, DIV, MUL_EXT, exec_div},
    {"divu", R_FMT, DIVU, MUL_EXT, exec_divu},
    {"rem", R_FMT, REM, MUL_EXT, exec_rem},
    {"remu", R_FMT, REMU, MUL_EXT, exec_remu},

    {"addi", I_FMT, ADDI, MATCH_ANY, exec_addi},
    {"slli", I_FMT, SLLI, SRL, exec_slli},
    {"slti", I_FMT, SLTI, MATCH_ANY, exec_slti},
    {"sltiu", I_FMT, SLTIU, MATCH_ANY, exec_sltiu},
    {"xori", I_FMT, XORI, MATCH_ANY, exec_xori},
    {"srli", I_FMT, SRLI_SRAI, SRL, exec_srli},
    {"srai", I_FMT, SRLI_SRAI, SRA, exec_srai},
    {"ori", I_FMT, ORI, MATCH_ANY, exec_ori},
    {"andi", I_FMT, ANDI, MATCH_ANY, exec_andi},

    {"lb", LOAD, LB, MATCH_ANY, exec_lb},
    {"lh", LOAD, LH, MATCH_ANY, exec_lh},
    {"lw", LOAD, LW, MATCH_ANY, exec_lw},
    {"lbu", LOAD, LBU, MATCH_ANY, exec_lbu},
    {"lhu", LOAD, LHU, MATCH_ANY, exec_lhu},

    {"sb", STORE, SB, MATCH_ANY, exec_sb},
    {"sh", STORE, SH, MATCH_ANY, exec_sh},
    {"sw", STORE, SW, MATCH_ANY, exec_sw},

    {"lr.w", AMO, LR_SC_W, LR_W, exec_lrw},
    {"sc.w", AMO, LR_SC_W, SC_W, exec_scw},

    {"beq", BRANCH, BEQ, MATCH_ANY, exec_beq},
    {"bne", BRANCH, BNE, MATCH_ANY, exec_bne},
    {"blt", BRANCH, BLT, MATCH_ANY, exec_blt},
    {"bge", BRANCH, BGE, MATCH_ANY, exec_bge},
    {"bltu", BRANCH, BLTU, MATCH_ANY, exec_bltu},
    {"bgeu", BRANCH, BGEU, MATCH_ANY, exec_bgeu},

    {"jal", JAL, MATCH_ANY, MATCH_ANY, exec_jal},
    {"jalr", JALR, ADDI, MATCH_ANY, exec_jalr},
    {"lui", LUI, MATCH_ANY, MATCH_ANY, exec_lui},
    {"auipc", AUIPC, MATCH_ANY, MATCH_ANY, exec_auipc},

    {"debug", SYSTEM, MATCH_ANY, DEBUG, exec_debug},
    {"os", SYSTEM, MATCH_ANY, OS, exec_osctlr},
};

const InstructionDecsriptor* find_instruction(const DecodedInstr* ins)
{
    for (size_t i = 0; i < ARRAY_LEN(instruction_table); i++)
    {
        const InstructionDecsriptor* d = &instruction_table[i];

        if (d->opcode != ins->opcode)
            continue;
        if (d->funct3 != -1 && d->funct3 != (int)ins->funct3)
            continue;
        if (d->funct7 != -1)
        {
            if (d->opcode == AMO)
            {
                if ((ins->funct7 & ~0x3u) != (uint32_t)d->funct7)
                    continue;
            }
            else if (d->funct7 != (int)ins->funct7)
            {
                continue;
            }
        }

        return d;
    }

    return NULL;
}
#endif
