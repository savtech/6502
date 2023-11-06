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

struct Status
{
    u8 c : 1; //Carry Flag
    u8 z : 1; //Zero Flag
    u8 i : 1; //Interrupt Disable Flag
    u8 d : 1; //Decimal Mode Flag
    u8 b : 1; //Break Command Flag
    u8 v : 1; //Overflow Flag
    u8 n : 1; //Negative Flag
};

struct CPU
{
    static constexpr size_t MAX_INSTRUCTIONS = 256;
    static constexpr size_t STACK_MIN_ADDRESS = 0x0100;
    static constexpr size_t STACK_MAX_ADDRESS = 0x01FF;
    

    u16 pc; //Program Counter
    u8 sp; //Stack Pointer
    u8 a, x, y; //Registers
    Status s; //Status Register
    bool debug = false;

    void (*instruction[MAX_INSTRUCTIONS]) (CPU&, RAM&); //Instruction table

    void execute(RAM& ram);
    void execute_instructions(RAM& ram, const size_t instruction_count = 1);
    [[nodiscard]] u8 read_byte(RAM& ram) const;
    [[nodiscard]] u8 read_byte(RAM& ram, const u16 address) const;
    void write_byte(RAM& ram, const u16 address, const u8 data);
    [[nodiscard]] u8 next_byte(RAM& ram);

    void reset();
    void a_op();
    void x_op();
    void y_op();
    void zp_op(RAM& ram, const u8& address);

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
    pc = 0x0000;
    sp = 0x00;
    a = 0;
    x = 0;
    y = 0;
    s.b = 0;
    s.c = 0;
    s.d = 0;
    s.i = 0;
    s.n = 0;
    s.v = 0;
    s.z = 0;
}

void CPU::a_op()
{
    s.z = (a == 0);
    s.n = is_negative(a);
}

void CPU::x_op()
{
    s.z = (x == 0);
    s.n = is_negative(x);
}

void CPU::y_op()
{
    s.z = (y == 0);
    s.n = is_negative(y);
}

void CPU::zp_op(RAM& ram, const u8& address)
{
    s.z = (ram[address] == 0);
    s.n = is_negative(ram[address]);
}

void CPU::debug_print_instruction(const char* instruction_name, u8 data) const
{
    if(!debug) return;

    static constexpr size_t BUFFER_SIZE = 128;
    static constexpr size_t INSTRUCTION_PADDING = 8;

    static char instruction_buffer[BUFFER_SIZE];
    static char register_buffer[BUFFER_SIZE];
    static char flags_buffer[BUFFER_SIZE];
    
    snprintf(instruction_buffer, BUFFER_SIZE, "[%-*s] 0x%2.2x (%3i) ||||||||||", (int)INSTRUCTION_PADDING, instruction_name, data, data);
    snprintf(register_buffer, BUFFER_SIZE, " REGISTERS: [PC] 0x%4.4x [SP] 0x%2.2x [A] 0x%2.2x (%3i) | [X] 0x%2.2x (%3i) | [Y] 0x%2.2x (%3i) ||||||||||||", pc, sp, a, a, x, x, y, y);
    snprintf(flags_buffer, BUFFER_SIZE, " FLAGS: [C] %i [Z] %i [I] %i [D] %i [B] %i [V] %i [N] %i", s.c, s.z, s.i, s.d, s.b, s.v, s.n);
   
    printf("%s%s%s\n", instruction_buffer, register_buffer, flags_buffer);
}