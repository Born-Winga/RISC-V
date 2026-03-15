#ifndef FN7_CODES_H
#define FN7_CODES_H
typedef enum
{
    ADD_SRL = 0x00,
    SUB_SRA = 0x20,
    MUL_EXT = 0x01
} R_FMT_FN7;

typedef enum
{
    SRL = 0x00,
    SRA = 0x20
} I_FMT_FN7;

typedef enum
{
    DEBUG = 0x00,
    OS = 0x01,
} SYSTEM_FN7;

typedef enum
{
    AMOADD_W = 0x00,
    AMOSWAP_W = 0x04,
    LR_W = 0x08,
    SC_W = 0x0C,
    AMOXOR_W = 0x10,
    AMOOR_W = 0x20,
    AMOAND_W = 0x30,
    AMOMIN_W = 0x40,
    AMOMAX_W = 0x50,
    AMOMINU_W = 0x60,
    AMOMAXU_W = 0x70,
} AMO_FN7;
#endif
