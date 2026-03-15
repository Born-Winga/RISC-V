#ifndef OPCODES_H
#define OPCODES_H
typedef enum
{
    LUI = 0x37,
    AUIPC = 0x17,
    JAL = 0x6F,
    JALR = 0x67,
    BRANCH = 0x63,
    LOAD = 0x03,
    STORE = 0x23,
    AMO = 0x2F,
    I_FMT = 0x13,
    R_FMT = 0x33,
    SYSTEM = 0x73
} OPCODE;

#endif
