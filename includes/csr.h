#ifndef CSR_DEF_H
#define CSR_DEF_H
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    CSR_PRIV_U = 0,
    CSR_PRIV_S = 1,
    CSR_PRIV_H = 2,
    CSR_PRIV_M = 3,
    CSR_PRIV_D = 4, /* debug-only*/
} csr_priv_t;

typedef enum
{
    CSR_ACC_URO,
    CSR_ACC_URW,
    CSR_ACC_SRO,
    CSR_ACC_SRW,
    CSR_ACC_HRO,
    CSR_ACC_HRW,
    CSR_ACC_MRO,
    CSR_ACC_MRW,
    CSR_ACC_DRW,
} csr_access_t;

typedef struct
{
    uint16_t num; /* 12-bit CSR address */
    const char* name;
    csr_access_t access;
    csr_priv_t min_priv;
    bool rv32_only_hi; /* e.g. cycleh/mstatush/... */
    bool optional;     /* may not exist in an implementation */
} csr_def_t;

static inline bool csr_read_only(uint16_t csr_addr)
{
    // 0000 1111 1111 1111
    // 0000 1100 0000 0000
    // 0000 0000 0000 0011
    return ((csr_addr >> 10) & 0x03u) == 0b11u;
}
static inline uint8_t csr_priv_level(uint16_t csr_addr)
{
    return (csr_addr >> 8) & 0x03u;
}

/** Unprivillaged CSRs */
enum
{
    /* Floating-point */
    CSR_FFLAGS = 0x001,
    CSR_FRM = 0x002,
    CSR_FCSR = 0x003,

    /* Vector */
    CSR_VSTART = 0x008,
    CSR_VXSAT = 0x009,
    CSR_VXRM = 0x00A,
    CSR_VCSR = 0x00F,
    CSR_VL = 0xC20,
    CSR_VTYPE = 0xC21,
    CSR_VLENB = 0xC22,

    /* Zicfiss */
    CSR_SSP = 0x011,

    /* Entropy */
    CSR_SEED = 0x015,

    /* Zcmt */
    CSR_JVT = 0x017,

    /* Counters/timers */
    CSR_CYCLE = 0xC00,
    CSR_TIME = 0xC01,
    CSR_INSTRET = 0xC02,
    CSR_HPMCOUNTER3 = 0xC03,
    CSR_HPMCOUNTER31 = 0xC1F,

    /* RV32 high halves */
    CSR_CYCLEH = 0xC80,
    CSR_TIMEH = 0xC81,
    CSR_INSTRETH = 0xC82,
    CSR_HPMCOUNTER3H = 0xC83,
    CSR_HPMCOUNTER31H = 0xC9F,
};

/** Supervisor CSRs */
enum
{
    /* Trap setup */
    CSR_SSTATUS = 0x100,
    CSR_SIE = 0x104,
    CSR_STVEC = 0x105,
    CSR_SCOUNTEREN = 0x106,

    /* Configuration */
    CSR_SENVCFG = 0x10A,

    /* Counter setup */
    CSR_SCOUNTINHIBIT = 0x120,

    /* Trap handling */
    CSR_SSCRATCH = 0x140,
    CSR_SEPC = 0x141,
    CSR_SCAUSE = 0x142,
    CSR_STVAL = 0x143,
    CSR_SIP = 0x144,
    CSR_SCOUNTOVF = 0xDA0,

    /* Indirect CSR access */
    CSR_SISELECT = 0x150,
    CSR_SIREG = 0x151,
    CSR_SIREG2 = 0x152,
    CSR_SIREG3 = 0x153,
    CSR_SIREG4 = 0x155,
    CSR_SIREG5 = 0x156,
    CSR_SIREG6 = 0x157,

    /* Protection/translation */
    CSR_SATP = 0x180,

    /* State enable */
    CSR_SSTATEEN0 = 0x10C,
    CSR_SSTATEEN1 = 0x10D,
    CSR_SSTATEEN2 = 0x10E,
    CSR_SSTATEEN3 = 0x10F,

    /* Control transfer records */
    CSR_SCTRCTL = 0x14E,
    CSR_SCTRSTATUS = 0x14F,
    CSR_SCTRDEPTH = 0x15F,
};

/** Hypervisor CSRs */
enum
{
    /* Hypervisor trap setup */
    CSR_HSTATUS = 0x600,
    CSR_HEDELEG = 0x602,
    CSR_HIDELEG = 0x603,
    CSR_HIE = 0x604,
    CSR_HCOUNTEREN = 0x606,
    CSR_HGEIE = 0x607,
    CSR_HEDELEGH = 0x612, /* RV32 upper 32 bits */

    /* Hypervisor trap handling */
    CSR_HTVAL = 0x643,
    CSR_HIP = 0x644,
    CSR_HVIP = 0x645,
    CSR_HTINST = 0x64A,
    CSR_HGEIP = 0xE12,

    /* Hypervisor configuration */
    CSR_HENVCFG = 0x60A,
    CSR_HENVCFGH = 0x61A, /* RV32 upper 32 bits */

    /* Hypervisor translation/protection */
    CSR_HGATP = 0x680,

    /* VS-mode */
    CSR_VSSTATUS = 0x200,
    CSR_VSIE = 0x204,
    CSR_VSTVEC = 0x205,
    CSR_VSSCRATCH = 0x240,
    CSR_VSEPC = 0x241,
    CSR_VSCAUSE = 0x242,
    CSR_VSTVAL = 0x243,
    CSR_VSIP = 0x244,
    CSR_VSATP = 0x280,

    /* VS indirect */
    CSR_VSISELECT = 0x250,
    CSR_VSIREG = 0x251,
    CSR_VSIREG2 = 0x252,
    CSR_VSIREG3 = 0x253,
    CSR_VSIREG4 = 0x255,
    CSR_VSIREG5 = 0x256,
    CSR_VSIREG6 = 0x257,

    /* VS timers / CTR */
    CSR_VSTIMECMP = 0x24D,
    CSR_VSTIMECMPH = 0x25D, /* RV32 only */
    CSR_VSCTRCTL = 0x24E,
};

/** Machine Level CSRs */
enum
{
    /* Information */
    CSR_MVENDORID = 0xF11,
    CSR_MARCHID = 0xF12,
    CSR_MIMPID = 0xF13,
    CSR_MHARTID = 0xF14,
    CSR_MCONFIGPTR = 0xF15,

    /* Trap setup */
    CSR_MSTATUS = 0x300,
    CSR_MISA = 0x301,
    CSR_MEDELEG = 0x302,
    CSR_MIDELEG = 0x303,
    CSR_MIE = 0x304,
    CSR_MTVEC = 0x305,
    CSR_MCOUNTEREN = 0x306,
    CSR_MSTATUSH = 0x310, /* RV32 only */
    CSR_MEDELEGH = 0x312, /* RV32 only */

    /* Configuration */
    CSR_MENVCFG = 0x30A,
    CSR_MENVCFGH = 0x31A, /* RV32 only */
    CSR_MSECCFG = 0x747,
    CSR_MSECCFGH = 0x757, /* RV32 only */

    /* State enable */
    CSR_MSTATEEN0 = 0x30C,
    CSR_MSTATEEN1 = 0x30D,
    CSR_MSTATEEN2 = 0x30E,
    CSR_MSTATEEN3 = 0x30F,
    CSR_MSTATEEN0H = 0x31C, /* RV32 only */
    CSR_MSTATEEN1H = 0x31D,
    CSR_MSTATEEN2H = 0x31E,
    CSR_MSTATEEN3H = 0x31F,

    /* Trap handling */
    CSR_MSCRATCH = 0x340,
    CSR_MEPC = 0x341,
    CSR_MCAUSE = 0x342,
    CSR_MTVAL = 0x343,
    CSR_MIP = 0x344,
    CSR_MTINST = 0x34A,
    CSR_MTVAL2 = 0x34B,

    /* Indirect CSR access */
    CSR_MISELECT = 0x350,
    CSR_MIREG = 0x351,
    CSR_MIREG2 = 0x352,
    CSR_MIREG3 = 0x353,
    CSR_MIREG4 = 0x355,
    CSR_MIREG5 = 0x356,
    CSR_MIREG6 = 0x357,

    /* Counter setup */
    CSR_MCOUNTINHIBIT = 0x320,
    CSR_MCYCLECFG = 0x321,
    CSR_MINSTRETCFG = 0x322,
    CSR_MHPMEVENT3 = 0x323,
    CSR_MHPMEVENT4 = 0x324,
    CSR_MHPMEVENT31 = 0x33F,
    CSR_MCYCLECFGH = 0x721,   /* RV32 only */
    CSR_MINSTRETCFGH = 0x722, /* RV32 only */
    CSR_MHPMEVENT3H = 0x723,  /* RV32 only */
    CSR_MHPMEVENT4H = 0x724,  /* RV32 only */
    CSR_MHPMEVENT31H = 0x73F, /* RV32 only */

    /* Machine counters */
    CSR_MCYCLE = 0xB00,
    CSR_MINSTRET = 0xB02,
    CSR_MHPMCOUNTER3 = 0xB03,
    CSR_MHPMCOUNTER4 = 0xB04,
    CSR_MHPMCOUNTER31 = 0xB1F,
    CSR_MCYCLEH = 0xB80,        /* RV32 only */
    CSR_MINSTRETH = 0xB82,      /* RV32 only */
    CSR_MHPMCOUNTER3H = 0xB83,  /* RV32 only */
    CSR_MHPMCOUNTER4H = 0xB84,  /* RV32 only */
    CSR_MHPMCOUNTER31H = 0xB9F, /* RV32 only */

    /* PMP base ranges */
    CSR_PMPCFG0 = 0x3A0,
    CSR_PMPCFG15 = 0x3AF,
    CSR_PMPADDR0 = 0x3B0,
    CSR_PMPADDR63 = 0x3EF,

    /* Resumable NMI */
    CSR_MNSCRATCH = 0x740,
    CSR_MNEPC = 0x741,
    CSR_MNCAUSE = 0x742,
    CSR_MNSTATUS = 0x744,

    /* Control transfer records */
    CSR_MCTRCTL = 0x34E,

    /* Debug/trace shared */
    CSR_TSELECT = 0x7A0,
    CSR_TDATA1 = 0x7A1,
    CSR_TDATA2 = 0x7A2,
    CSR_TDATA3 = 0x7A3,
    CSR_MCONTEXT = 0x7A8,

    /* Debug mode only */
    CSR_DCSR = 0x7B0,
    CSR_DPC = 0x7B1,
    CSR_DSCRATCH0 = 0x7B2,
    CSR_DSCRATCH1 = 0x7B3,
};

static inline uint16_t csr_hpmcounter_u(unsigned i)
{
    return (uint16_t)(0xC00u + i);
} /* i=3..31 => 0xC03..0xC1F */
static inline uint16_t csr_hpmcounterh_u(unsigned i)
{
    return (uint16_t)(0xC80u + i);
} /* i=3..31 => 0xC83..0xC9F */

static inline uint16_t csr_mhpmcounter(unsigned i)
{
    return (uint16_t)(0xB00u + i);
} /* i=3..31 => 0xB03..0xB1F */
static inline uint16_t csr_mhpmcounterh(unsigned i)
{
    return (uint16_t)(0xB80u + i);
} /* i=3..31 => 0xB83..0xB9F */

static inline uint16_t csr_mhpmevent(unsigned i)
{
    return (uint16_t)(0x320u + i);
} /* i=3..31 => 0x323..0x33F */
static inline uint16_t csr_mhpmeventh(unsigned i)
{
    return (uint16_t)(0x720u + i);
} /* i=3..31 => 0x723..0x73F */

static inline uint16_t csr_pmpcfg(unsigned i)
{
    return (uint16_t)(0x3A0u + i);
} /* i=0..15 */
static inline uint16_t csr_pmpaddr(unsigned i)
{
    return (uint16_t)(0x3B0u + i);
} /* i=0..63 */

static const csr_def_t riscv_allocated_csrs[] = {
    {CSR_FFLAGS, "fflags", CSR_ACC_URW, CSR_PRIV_U, false, true},
    {CSR_FRM, "frm", CSR_ACC_URW, CSR_PRIV_U, false, true},
    {CSR_FCSR, "fcsr", CSR_ACC_URW, CSR_PRIV_U, false, true},

    {CSR_CYCLE, "cycle", CSR_ACC_URO, CSR_PRIV_U, false, true},
    {CSR_TIME, "time", CSR_ACC_URO, CSR_PRIV_U, false, true},
    {CSR_INSTRET, "instret", CSR_ACC_URO, CSR_PRIV_U, false, true},
    {CSR_CYCLEH, "cycleh", CSR_ACC_URO, CSR_PRIV_U, true, true},
    {CSR_TIMEH, "timeh", CSR_ACC_URO, CSR_PRIV_U, true, true},
    {CSR_INSTRETH, "instreth", CSR_ACC_URO, CSR_PRIV_U, true, true},

    {CSR_SSTATUS, "sstatus", CSR_ACC_SRW, CSR_PRIV_S, false, true},
    {CSR_STVEC, "stvec", CSR_ACC_SRW, CSR_PRIV_S, false, true},
    {CSR_SEPC, "sepc", CSR_ACC_SRW, CSR_PRIV_S, false, true},
    {CSR_SCAUSE, "scause", CSR_ACC_SRW, CSR_PRIV_S, false, true},
    {CSR_STVAL, "stval", CSR_ACC_SRW, CSR_PRIV_S, false, true},
    {CSR_SIP, "sip", CSR_ACC_SRW, CSR_PRIV_S, false, true},
    {CSR_SATP, "satp", CSR_ACC_SRW, CSR_PRIV_S, false, true},

    {CSR_MVENDORID, "mvendorid", CSR_ACC_MRO, CSR_PRIV_M, false, true},
    {CSR_MARCHID, "marchid", CSR_ACC_MRO, CSR_PRIV_M, false, true},
    {CSR_MIMPID, "mimpid", CSR_ACC_MRO, CSR_PRIV_M, false, true},
    {CSR_MHARTID, "mhartid", CSR_ACC_MRO, CSR_PRIV_M, false, true},
    {CSR_MCONFIGPTR, "mconfigptr", CSR_ACC_MRO, CSR_PRIV_M, false, true},

    {CSR_MSTATUS, "mstatus", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MISA, "misa", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MEDELEG, "medeleg", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MIDELEG, "mideleg", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MIE, "mie", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MTVEC, "mtvec", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MCOUNTEREN, "mcounteren", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MSTATUSH, "mstatush", CSR_ACC_MRW, CSR_PRIV_M, true, true},

    {CSR_MSCRATCH, "mscratch", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MEPC, "mepc", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MCAUSE, "mcause", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MTVAL, "mtval", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MIP, "mip", CSR_ACC_MRW, CSR_PRIV_M, false, true},

    {CSR_MCYCLE, "mcycle", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MINSTRET, "minstret", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MCYCLEH, "mcycleh", CSR_ACC_MRW, CSR_PRIV_M, true, true},
    {CSR_MINSTRETH, "minstreth", CSR_ACC_MRW, CSR_PRIV_M, true, true},

    {CSR_TSELECT, "tselect", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_TDATA1, "tdata1", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_TDATA2, "tdata2", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_TDATA3, "tdata3", CSR_ACC_MRW, CSR_PRIV_M, false, true},
    {CSR_MCONTEXT, "mcontext", CSR_ACC_MRW, CSR_PRIV_M, false, true},

    {CSR_DCSR, "dcsr", CSR_ACC_DRW, CSR_PRIV_D, false, true},
    {CSR_DPC, "dpc", CSR_ACC_DRW, CSR_PRIV_D, false, true},
    {CSR_DSCRATCH0, "dscratch0", CSR_ACC_DRW, CSR_PRIV_D, false, true},
    {CSR_DSCRATCH1, "dscratch1", CSR_ACC_DRW, CSR_PRIV_D, false, true},
};

typedef struct
{
    const csr_def_t* def; /* points into allocated/spec table */
    uint64_t value;       /* backing store if simple */
    uint64_t write_mask;  /* implementation-specific */
    bool implemented;     /* on this hart */
} csr_slot_t;
#endif
