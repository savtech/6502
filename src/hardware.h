#pragma once
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "utils.h"

struct RAM
{
    static constexpr size_t MAX_MEMORY = KB(64);

    u8 memory[MAX_MEMORY];

    [[nodiscard]] u8& operator[](size_t index) { return memory[index]; }
    void d_print() {printf("Ram memory available: %zu\n", sizeof(memory));};
};

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
    u16 pc; //Program Counter
    u8 sp; //Stack Pointer
    u8 a;
    u8 x;
    u8 y; //Registers
    Status s; //Status Register

    void (*instruction[256]) (CPU*, RAM*); //instruction table

    void execute(RAM& ram);
    [[nodiscard]] u8 next_byte(RAM& ram);
    [[nodiscard]] u8 read_byte(RAM& ram, u8 address);

    void lda_op();
    void ldx_op();
    void ldy_op();

    void d_print_regs() { printf("CPU Registers: [A:%i][X:%i][Y:%i] ", a, x, y); }
    void d_print_flags() { printf("CPU Flags: [C:%i][Z:%i][I:%i][D:%i][B:%i][V:%i][N:%i] ", s.c, s.z, s.i, s.d, s.b, s.v, s.n); }
    void d_print() { d_print_flags(); d_print_regs(); printf("\n"); }
};

void CPU::execute(RAM& ram)
{
    u8 opcode = next_byte(ram);
    instruction[opcode](this, &ram);
    d_print();
}

u8 CPU::next_byte(RAM& ram)
{
    return ram[pc++];
}

u8 CPU::read_byte(RAM& ram, u8 address)
{
    return ram[address];
}

void CPU::lda_op()
{
    s.z = (a == 0);
    s.n = is_negative(a);
}

void CPU::ldx_op()
{
    s.z = (x == 0);
    s.n = is_negative(x);
}

void CPU::ldy_op()
{
    s.z = (y == 0);
    s.n = is_negative(y);
}