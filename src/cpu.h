#pragma once
#include "ram.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>

struct CPU {
    static constexpr size_t MAX_INSTRUCTIONS  = 256;
    static constexpr size_t STACK_MIN_ADDRESS = 0x0100;
    static constexpr size_t STACK_MAX_ADDRESS = 0x01FF;

    static constexpr u8
        CARRY_FLAG     = 1 << 0,
        ZERO_FLAG      = 1 << 1,
        INTERRUPT_FLAG = 1 << 2,
        DECIMAL_FLAG   = 1 << 3,
        BREAK_FLAG     = 1 << 4,
        OVERFLOW_FLAG  = 1 << 6,
        NEGATIVE_FLAG  = 1 << 7;

    u16 pc;                                             // Program Counter
    u8 sp;                                              // Stack Pointer
    u8 a, x, y, s;                                      // Registers
    void (*instructions[MAX_INSTRUCTIONS])(CPU&, RAM&); // Instruction table
    bool debug = false;

    void execute(RAM& ram);
    void execute_instructions(RAM& ram, const size_t instruction_count = 1);
    [[nodiscard]] u8 read_u8(RAM& ram) const;
    [[nodiscard]] u8 read_u8(RAM& ram, const u16 address) const;
    [[nodiscard]] u8 next_u8(RAM& ram);
    [[nodiscard]] u16 next_u16(RAM& ram);
    void write_byte(RAM& ram, const u16 address, const u8 data);
    void reset();
    void set_status(u8 flags);
    void clear_status(u8 flags);
    void update_status(u8 address);

    void debug_print_instruction(const char* instruction_name, const u16 data = 0x0000) const;
};

void CPU::execute(RAM& ram) {
    u8 opcode = read_u8(ram);
    instructions[opcode](*this, ram);
    pc++;
}

void CPU::execute_instructions(RAM& ram, const size_t instruction_count) {
    for(size_t i = 0; i < instruction_count; i++) {
        execute(ram);
    }
}

u8 CPU::read_u8(RAM& ram) const {
    return ram.read(pc);
}

u8 CPU::read_u8(RAM& ram, const u16 address) const {
    return ram.read(address);
}

u8 CPU::next_u8(RAM& ram) {
    return ram.read(++pc);
}

u16 CPU::next_u16(RAM& ram) {
    u8 lsb = next_u8(ram);
    u8 msb = next_u8(ram);

    return ((msb << 8) | lsb); // The 6502 is little endian
}

void CPU::write_byte(RAM& ram, const u16 address, const u8 data) {
    ram.write(address, data);
}

void CPU::reset() {
    pc = sp = a = x = y = s = 0x00;
}

void CPU::set_status(u8 flags) {
    s |= flags;
}

void CPU::clear_status(u8 flags) {
    s &= ~flags;
}

void CPU::update_status(u8 address) {
    (address == 0x00) ? set_status(ZERO_FLAG) : clear_status(ZERO_FLAG);
    (address & 0x80) ? set_status(NEGATIVE_FLAG) : clear_status(NEGATIVE_FLAG);
}

void CPU::debug_print_instruction(const char* instruction_name, u16 data) const {
    if(!debug) return;

    printf(
        "[%-*s] 0x%4.4x (%5i) |||||||||| REGISTERS: [PC] 0x%4.4x [SP] 0x%2.2x [A] 0x%2.2x (%3i) | [X] 0x%2.2x (%3i) | [Y] 0x%2.2x (%3i) |||||||||||| FLAGS: [N] %c [V] %c [~] %c [B] %c [D] %c [I] %c [Z] %c [C] %c\n",
        8,
        instruction_name,
        data,
        data,
        pc,
        sp,
        a,
        a,
        x,
        x,
        y,
        y,
        BYTE_TO_BINARY(s)
    );
}