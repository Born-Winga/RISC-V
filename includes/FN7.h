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
    LR_W = 0x08,
    SC_W = 0x0C,
} AMO_FN7;
#endif
