#include <stdio.h>
#include "types.h"
#include "hardware.h"
#include "instructions.h"

int main()
{
    CPU cpu = {};
    RAM ram = {};

    size_t instruction_count = 18;
    load_instructions(&cpu);

    ram[66] = 0;
    ram[67] = 32;
    ram[68] = 128;

    ram[0] = LDA_IMMEDIATE;
    ram[1] = 0; //zero case
    ram[2] = LDA_IMMEDIATE;
    ram[3] = 64; //positive integer
    ram[4] = LDA_IMMEDIATE;
    ram[5] = 128; //"negative" integer

    ram[6] = LDX_IMMEDIATE;
    ram[7] = 0; //zero case
    ram[8] = LDX_IMMEDIATE;
    ram[9] = 64; //positive integer
    ram[10] = LDX_IMMEDIATE;
    ram[11] = 128; //"negative" integer

    ram[12] = LDY_IMMEDIATE;
    ram[13] = 0; //zero case
    ram[14] = LDY_IMMEDIATE;
    ram[15] = 64; //positive integer
    ram[16] = LDY_IMMEDIATE;
    ram[17] = 128; //"negative" integer

    ram[18] = LDA_ZERO_PAGE;
    ram[19] = 66; //zero case
    ram[20] = LDA_ZERO_PAGE;
    ram[21] = 67; //positive integer
    ram[22] = LDA_ZERO_PAGE;
    ram[23] = 68; //"negative" integer

    ram[24] = LDX_ZERO_PAGE;
    ram[25] = 66; //zero case
    ram[26] = LDX_ZERO_PAGE;
    ram[27] = 67; //positive integer
    ram[28] = LDX_ZERO_PAGE;
    ram[29] = 68; //"negative" integer

    ram[30] = LDY_ZERO_PAGE;
    ram[31] = 66; //zero case
    ram[32] = LDY_ZERO_PAGE;
    ram[33] = 67; //positive integer
    ram[34] = LDY_ZERO_PAGE;
    ram[35] = 68; //"negative" integer

    for(size_t i = 0; i < instruction_count; i++)
        cpu.execute(ram);

    return 0;
}