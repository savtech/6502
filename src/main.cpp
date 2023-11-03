#include <stdio.h>
#include "types.h"
#include "hardware.h"

int isSet(CPU cpu, Flags flag)
{
    if(cpu.flags & flag)
    {
        return 1;
    }

    return 0;
}

int main()
{
    CPU cpu = {};
    RAM ram = {};

    cpu.flags |= Flags::C | Flags::B | Flags::N;

    printf("C set? %i\n", isSet(cpu, Flags::C));
    printf("Z set? %i\n", isSet(cpu, Flags::Z));
    printf("I set? %i\n", isSet(cpu, Flags::I));
    printf("D set? %i\n", isSet(cpu, Flags::D));
    printf("B set? %i\n", isSet(cpu, Flags::B));
    printf("V set? %i\n", isSet(cpu, Flags::V));
    printf("N set? %i\n", isSet(cpu, Flags::N));

    printf("Ram memory available: %u", (uint32_t) sizeof(ram.memory));

    return 0;
}