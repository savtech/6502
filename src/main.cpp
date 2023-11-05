#include <stdio.h>
#include "hardware.h"
#include "instructions.h"

void mock_data(CPU& cpu, RAM& ram)
{
    ram[0x8F] = 0;
    ram[0] = LDX_IMM;
    ram[1] = 0x0F;
    ram[2] = LDA_ZPX;
    ram[3] = 0x80;
}

int main()
{
    CPU cpu = {.debug = true};
    RAM ram = {};

    load_instructions(&cpu);
    mock_data(cpu, ram);

    cpu.execute(ram, 2);

    return 0;
}