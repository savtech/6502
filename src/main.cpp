#include <stdio.h>
#include "hardware.h"
#include "instructions.h"

void mock_data(CPU& cpu, RAM& ram)
{
    ram.write(0x0000, LDA_IMM);
    ram.write(0x0001, 0x40);
    ram.write(0x0002, STA_ABS);
    ram.write(0x0003, 0xBB);
    ram.write(0x0004, 0xAA);
    ram.write(0x0005, NOP);
}

int main()
{
    CPU cpu = {.debug = true};
    RAM ram = {};

    load_instructions(cpu);
    mock_data(cpu, ram);

    cpu.execute_instructions(ram, 3);

    return 0;
}