#pragma once
#include "types.h"

constexpr size_t MAX_MEMORY = KB(64);
constexpr size_t STACK_ADDRESS = 0x0100;
constexpr size_t STACK_SIZE = 256;

enum Flags : u8
{
    C = 1 << 0,
    Z = 1 << 1,
    I = 1 << 2,
    D = 1 << 3,
    B = 1 << 4,
    V = 1 << 5,
    N = 1 << 6
};

struct CPU
{
    u16 pc; //Program Counter
    u8 sp; //Stack Pointer
    u8 A, X, Y; //Registers
    u8 flags;
};

struct RAM
{
    u8 memory[MAX_MEMORY];
};