#pragma once
#include <stdio.h>
#include "types.h"
#include "utils.h"

struct RAM
{
    static constexpr size_t MAX_MEMORY = KB(64);

    u16 most_recent_read;
    u16 most_recent_write;

	[[nodiscard]] u8 operator[](const size_t index) const { return memory[index]; } //Read from memory
	[[nodiscard]] u8& operator[](const size_t index) { return memory[index]; }      //Write to memory

    [[nodiscard]] const u8 read(const u16 address);
    void write(const u16 address, const u8 data);

    void debug_print() {printf("Ram memory available: %zu\n", sizeof(memory));};

    private:
    u8 memory[MAX_MEMORY];
};

const u8 RAM::read(const u16 address)
{
    most_recent_read = address;
    return memory[address];
}

void RAM::write(const u16 address, const u8 data)
{
    most_recent_write = address;
    memory[address] = data;
}

struct CPU
{
    static constexpr size_t MAX_INSTRUCTIONS = 256;
    static constexpr size_t STACK_MIN_ADDRESS = 0x0100;
    static constexpr size_t STACK_MAX_ADDRESS = 0x01FF;
    
    static constexpr u8 
        CARRY_FLAG =     1 << 0,
        ZERO_FLAG =      1 << 1,
        INTERRUPT_FLAG = 1 << 2,
        DECIMAL_FLAG =   1 << 3,
        BREAK_FLAG =     1 << 4,
        OVERFLOW_FLAG =  1 << 6,
        NEGATIVE_FLAG =  1 << 7;


    u16 pc; //Program Counter
    u8 sp; //Stack Pointer
    u8 a, x, y, s; //Registers
    bool debug = false;

    void (*instruction[MAX_INSTRUCTIONS]) (CPU&, RAM&); //Instruction table

    void execute(RAM& ram);
    void execute_instructions(RAM& ram, const size_t instruction_count = 1);
    [[nodiscard]] u8 read_byte(RAM& ram) const;
    [[nodiscard]] u8 read_byte(RAM& ram, const u16 address) const;
    void write_byte(RAM& ram, const u16 address, const u8 data);
    [[nodiscard]] u8 next_byte(RAM& ram);

    void reset();
    void update_flags(u8 address);

    void debug_print_instruction(const char* instruction_name, const u8 data = 0x00) const;
};

void CPU::execute(RAM& ram)
{
    u8 opcode = read_byte(ram);
    instruction[opcode](*this, ram);
    pc++;
}

void CPU::execute_instructions(RAM& ram, const size_t instruction_count)
{
    for(size_t i = 0; i < instruction_count; i++)
    {
        execute(ram);
    }
}

u8 CPU::read_byte(RAM& ram) const
{
    return ram.read(pc);
}

u8 CPU::read_byte(RAM& ram, const u16 address) const
{
    return ram.read(address);
}

void CPU::write_byte(RAM& ram, const u16 address, const u8 data)
{
    ram.write(address, data);
}

u8 CPU::next_byte(RAM& ram)
{
    return ram.read(++pc);
}

void CPU::reset()
{
    pc = sp = a = x = y = s = 0x00;
}

void CPU::update_flags(u8 address)
{
    (address == 0x00) ? s |= ZERO_FLAG : s &= ~ZERO_FLAG;
    (address & 0x80) ? s |= NEGATIVE_FLAG : s &= ~NEGATIVE_FLAG; //The most significant bit determines signage
}

void CPU::debug_print_instruction(const char* instruction_name, u8 data) const
{
    if(!debug) return;

    printf(
        "[%-*s] 0x%2.2x (%3i) |||||||||| REGISTERS: [PC] 0x%4.4x [SP] 0x%2.2x [A] 0x%2.2x (%3i) | [X] 0x%2.2x (%3i) | [Y] 0x%2.2x (%3i) |||||||||||| FLAGS: [N] %c [V] %c [~] %c [B] %c [D] %c [I] %c [Z] %c [C] %c\n", 
        8, instruction_name, data, data, pc, sp, a, a, x, x, y, y, BYTE_TO_BINARY(s)
    );
}