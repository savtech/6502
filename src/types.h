#pragma once
#include "stdint.h"

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;

constexpr size_t KB(size_t kb) {
    return kb * 1024;
}
constexpr size_t MB(size_t mb) {
    return mb * 1024 * 1024;
}
constexpr size_t GB(size_t gb) {
    return gb * 1024 * 1024 * 1024;
}