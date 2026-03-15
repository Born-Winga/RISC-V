# RISC-V Emulator

Small RISC-V emulator in C, built as a learning project.

## Scope

Current support includes:

- RV32 integer register operations
- RV32M multiply, divide, and remainder
- Word-sized atomic ops, including `LR.W`, `SC.W`, and AMOs
- Immediate operations
- Loads and stores
- Branches
- `JAL`, `JALR`, `LUI`, and `AUIPC`

Current behavior:

- Starts at `PC = 0`
- Loads a raw binary into DRAM
- Stops on illegal instructions or invalid memory access
- Keeps `x0` at zero
- Uses a table-driven dispatcher

## Layout

- `src/main.c` - program entry and execution loop
- `src/cpu.c` - instruction dispatch
- `src/utils.c` - decode and memory helpers
- `src/handlers.c` - instruction handlers
- `includes/table.h` - instruction table

## Build

```bash
cc -Iincludes -Wall -Wextra -Wpedantic src/main.c src/cpu.c src/utils.c src/handlers.c -o riscv
```

## Run

```bash
./riscv path/to/program.bin
```

## Limits

- Not a complete RISC-V implementation
- No ELF loader yet
- Trap and syscall support is minimal
- No device model
- Test coverage is still limited
