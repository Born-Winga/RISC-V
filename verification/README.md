# Verification Framework

## Overview
This directory contains the verification infrastructure for the RISC-V CPU project.

The repository currently includes a software-based RISC-V emulator written in C (located in `src/`). This emulator will serve as a **reference model** for expected CPU behavior.

The goal of this verification environment is to validate a future SystemVerilog hardware implementation against this reference.

---

## Structure

verification/
├── README.md
├── verification_plan.md
└── tb/
    └── cpu_tb.sv

---

## Approach

Verification will be developed in stages:

1. Directed testing of individual instructions
2. Comparison against the C reference model
3. Expansion to more complex and edge-case scenarios

---

## Future Work

- Integrate DUT (hardware CPU) into testbench
- Implement result checking (scoreboarding)
- Automate comparisons with reference model