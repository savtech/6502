#pragma once
#include "types.h"

[[nodiscard]] static u8 is_negative(u8 data)
{
    return (data & 0b10000000) > 0;
}