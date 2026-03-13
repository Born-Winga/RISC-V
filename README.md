# RISC-V Emulator

This repository is a small RISC-V emulator written in C as a learning project.

The goal here is not to build a production-grade emulator. The goal is to understand how instruction fetch, decode, execution, register state, and memory fit together in a simple CPU model. The code is intentionally straightforward and still evolving as I learn more about the ISA and emulator design.

## What This Project Is

- A hands-on exercise in implementing parts of a RISC-V CPU
- A place to practice bit manipulation, instruction decoding, and control flow
- A simple emulator that loads a raw binary into memory and steps through instructions

## What This Project Is Not

- A complete RISC-V implementation
- A polished or heavily optimized emulator
- A finished teaching resource with full test coverage and documentation

## Current Scope

Right now the emulator is focused on a subset of RV32-style integer behavior, including:

- Register-register integer operations
- Immediate integer operations
- Loads and stores
- Branches
- `JAL`, `JALR`, `LUI`, and `AUIPC`

The emulator currently:

- Starts execution at `PC = 0`
- Loads a raw input binary directly into emulated DRAM
- Executes until an illegal instruction is hit or memory bounds stop execution
- Keeps `x0` pinned to zero after each step

## Project Layout

- `src/main.c`
  Loads a binary file into memory and drives the fetch/execute/step loop.
- `src/cpu.c`
  Contains the CPU core: fetch, bit extraction, sign extension, decode, and execution.
- `includes/CPU.h`
  Shared CPU state and function declarations.
- `includes/OPCODE.h`
  Opcode definitions.
- `includes/FN3.h`
  `funct3` values grouped by instruction class.
- `includes/FN7.h`
  `funct7` values used by shift and arithmetic instruction variants.

## Build

The current `Makefile` is not set up yet, so the simplest way to build is with `cc`:

```bash
cc -Iincludes -Wall -Wextra -Wpedantic src/main.c src/cpu.c -o riscv
```

## Run

Pass a raw binary file to the emulator:

```bash
./riscv path/to/program.bin
```

The binary is copied directly into emulator memory starting at address `0`.

## Notes and Limitations

- This is still a learning codebase, so correctness and clarity matter more than completeness.
- Error handling is minimal.
- Trap, syscall, and OS behavior are not developed yet.
- There is no ELF loader yet, only raw binary loading.
- Memory-mapped devices and a real platform model are not implemented.
- Test coverage is still very limited.

## Why I Built This

I wanted a project that forced me to understand the mechanics behind CPU execution instead of only reading about them. Writing the emulator by hand has been a good way to practice:

- reading instruction encodings
- working with bit fields
- handling signed vs unsigned behavior
- reasoning about program counters and jumps
- debugging low-level state transitions

## Next Steps

Some likely next improvements:

- add tests for instruction decoding and execution
- support more RV32I instructions
- improve illegal-instruction and trap handling
- add an ELF loader instead of raw binary loading
- improve memory checking and diagnostics

## Status

This project is actively experimental. Expect rough edges, missing features, and changes made in the spirit of learning rather than maintaining a stable interface.
