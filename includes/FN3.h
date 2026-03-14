#ifndef FN3_CODE_H
#define FN3_CODE_H
typedef enum
{
    ADD_SUB = 0x0,
    SLL = 0x1,
    SLT = 0x2,
    SLTU = 0x3,
    XOR = 0x4,
    SRL_SRA = 0x5,
    OR = 0x6,
    AND = 0x7
} R_FMT_FN3;

typedef enum
{
    MUL = 0x0,
    MULH = 0x1,
    MULHSU = 0x2,
    MULHU = 0x3,
    DIV = 0x4,
    DIVU = 0x5,
    REM = 0x6,
    REMU = 0x7
} M_EXT_FN3;

typedef enum
{
    BEQ = 0x0,
    BNE = 0x1,
    BLT = 0x4,
    BGE = 0x5,
    BLTU = 0x6,
    BGEU = 0x7
} BRANCH_FN3;

typedef enum
{
    ADDI = 0x0,
    SLLI = 0x1,
    SLTI = 0x2,
    SLTIU = 0x3,
    XORI = 0x4,
    SRLI_SRAI = 0x5,
    ORI = 0x6,
    ANDI = 0x7
} I_FMT_FN3;

typedef enum
{
    LB = 0x0,
    LH = 0x1,
    LW = 0x2,
    LBU = 0x4,
    LHU = 0x5
} LOAD_FN3;

typedef enum
{
    SB = 0x0,
    SH = 0x1,
    SW = 0x2
} STORE_FN3;

#endif
