# Verification Plan

## Scope

The objective is to verify correctness of a RISC-V CPU implementation (RV32I subset).

---

## Instruction Coverage

### Arithmetic
- ADD
- SUB

### Logical
- AND
- OR
- XOR

### Memory
- LW (Load Word)
- SW (Store Word)

### Control Flow
- BEQ (Branch if Equal)

---

## Test Strategy

### Basic Tests
- Validate expected outputs for known inputs

### Edge Cases
- Overflow behavior
- Negative values
- Zero register (`x0`) behavior

### Dependency Tests
- Back-to-back instructions using previous results

---

## Reference Model

The C implementation in `src/` will be used to:
- Generate expected outputs
- Validate hardware results