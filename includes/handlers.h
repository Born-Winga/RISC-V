#ifndef INSTR_HANDLER_H
#define INSTR_HANDLER_H
#include "CPU.h"
#include "utils.h"

/** R_FMT */
void exec_add(CPU* cpu, const DecodedInstr* instr);
void exec_sub(CPU* cpu, const DecodedInstr* instr);
void exec_sll(CPU* cpu, const DecodedInstr* instr);
void exec_slt(CPU* cpu, const DecodedInstr* instr);
void exec_sltu(CPU* cpu, const DecodedInstr* instr);
void exec_xor(CPU* cpu, const DecodedInstr* instr);
void exec_or(CPU* cpu, const DecodedInstr* instr);
void exec_and(CPU* cpu, const DecodedInstr* instr);
void exec_srl(CPU* cpu, const DecodedInstr* instr);
void exec_sra(CPU* cpu, const DecodedInstr* instr);
void exec_mul(CPU* cpu, const DecodedInstr* instr);
void exec_mulh(CPU* cpu, const DecodedInstr* instr);
void exec_mulhsu(CPU* cpu, const DecodedInstr* instr);
void exec_mulhu(CPU* cpu, const DecodedInstr* instr);
void exec_div(CPU* cpu, const DecodedInstr* instr);
void exec_divu(CPU* cpu, const DecodedInstr* instr);
void exec_rem(CPU* cpu, const DecodedInstr* instr);
void exec_remu(CPU* cpu, const DecodedInstr* instr);

/** I_FMT */
void exec_addi(CPU* cpu, const DecodedInstr* instr);
void exec_slli(CPU* cpu, const DecodedInstr* instr);
void exec_slti(CPU* cpu, const DecodedInstr* instr);
void exec_sltiu(CPU* cpu, const DecodedInstr* instr);
void exec_xori(CPU* cpu, const DecodedInstr* instr);
void exec_ori(CPU* cpu, const DecodedInstr* instr);
void exec_andi(CPU* cpu, const DecodedInstr* instr);
void exec_srli(CPU* cpu, const DecodedInstr* instr);
void exec_srai(CPU* cpu, const DecodedInstr* instr);

/** L_FMT */
void exec_lb(CPU* cpu, const DecodedInstr* instr);
void exec_lh(CPU* cpu, const DecodedInstr* instr);
void exec_lw(CPU* cpu, const DecodedInstr* instr);
void exec_lbu(CPU* cpu, const DecodedInstr* instr);
void exec_lhu(CPU* cpu, const DecodedInstr* instr);

/** S_FMT */
void exec_sb(CPU* cpu, const DecodedInstr* instr);
void exec_sh(CPU* cpu, const DecodedInstr* instr);
void exec_sw(CPU* cpu, const DecodedInstr* instr);

/** B_FMT */
void exec_beq(CPU* cpu, const DecodedInstr* instr);
void exec_bne(CPU* cpu, const DecodedInstr* instr);
void exec_blt(CPU* cpu, const DecodedInstr* instr);
void exec_bge(CPU* cpu, const DecodedInstr* instr);
void exec_bltu(CPU* cpu, const DecodedInstr* instr);
void exec_bgeu(CPU* cpu, const DecodedInstr* instr);

/** J_FMT */
void exec_jal(CPU* cpu, const DecodedInstr* instr);
void exec_jalr(CPU* cpu, const DecodedInstr* instr);

/** */
void exec_lui(CPU* cpu, const DecodedInstr* instr);
void exec_auipc(CPU* cpu, const DecodedInstr* instr);

/** System */
void exec_debug(CPU* cpu, const DecodedInstr* instr);
void exec_osctlr(CPU* cpu, const DecodedInstr* instr);

/** A extension */
void exec_lrw(CPU* cpu, const DecodedInstr* instr);
void exec_scw(CPU* cpu, const DecodedInstr* instr);

#endif
