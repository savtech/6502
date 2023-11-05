#pragma once
#include "types.h"

[[nodiscard]] u8 is_negative(const u8 data)
{
    return (data & 0b10000000) > 0;
}