#pragma once
#include <stdio.h>
#include "types.h"

struct RAM
{
    static constexpr size_t MAX_MEMORY = KB(64);

    u16 most_recent_read;
    u16 most_recent_write;

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