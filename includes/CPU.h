#include <stddef.h>
#include <stdint.h>
#ifndef CPU_H
#define CPU_H
#define REG_COUNT 32
#define INSTR_LEN 4
#define MEM_SIZE (1024u * 1024u * 128u)
#define MEM_BASE 0x80000000
typedef struct CPU
{
    uint32_t PC;
    uint32_t REG[REG_COUNT];
    uint8_t pc_set;
    uint8_t halted;
    uint32_t (*fetch)(struct CPU* cpu);
    void (*step)(struct CPU* cpu);
    void (*execute)(uint32_t instr, struct CPU* cpu);
} CPU;

uint32_t fetch(struct CPU* cpu);
uint32_t extract(uint32_t instr, uint32_t hi, uint32_t lo);
int32_t sign_extend(uint32_t value, size_t width);
uint32_t zero_extend(uint32_t value, size_t width);
void execute(uint32_t instr, struct CPU* cpu);
void step(struct CPU* cpu);

extern uint8_t DRAM[MEM_SIZE];
#endif
