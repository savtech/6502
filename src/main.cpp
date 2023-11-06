#include <stdio.h>
#include "hardware.h"
#include "instructions.h"

void mock_data(CPU& cpu, RAM& ram)
{
    ram.write(0x0000, LDX_IMM);
    ram.write(0x0001, 0xFF);
    ram.write(0x0002, INX);
}

int main()
{
    CPU cpu = {.debug = true};
    RAM ram = {};

    load_instructions(cpu);
    mock_data(cpu, ram);

    cpu.execute_instructions(ram, 2);

    return 0;
}