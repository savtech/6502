#include "cpu.h"
#include "instructions.h"
#include "ram.h"
#include <stdio.h>

int main() {
    u8 data     = 0xFF;
    s8 new_data = static_cast<s8>(data);

    printf("Data: %c%c%c%c%c%c%c%c (%i)\n", BYTE_TO_BINARY(data), data);
    printf("New Data: %c%c%c%c%c%c%c%c (%i)", BYTE_TO_BINARY(new_data), new_data);

    return 0;
}