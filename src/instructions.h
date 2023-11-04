#pragma once
#include "types.h"
#include "hardware.h"

//LDA===============================================
constexpr u8 LDA_IMMEDIATE = 0xA9;
constexpr u8 LDA_ZERO_PAGE = 0xA5;
//LDX===============================================
constexpr u8 LDX_IMMEDIATE = 0xA2;
constexpr u8 LDX_ZERO_PAGE = 0xA6;
//LDY===============================================
constexpr u8 LDY_IMMEDIATE = 0xA0;
constexpr u8 LDY_ZERO_PAGE = 0xA4;

void _lda_immediate(CPU* cpu, RAM* ram)
{
    u8 value = cpu->next_byte(*ram);
    printf("LDA_IMMEDIATE (%i): ", value);
    cpu->a = value;
    cpu->lda_op();
}

void _lda_zero_page(CPU* cpu, RAM* ram)
{
    u8 address = cpu->next_byte(*ram);
    printf("LDA_ZERO_PAGE (%i): ", address);
    cpu->a = cpu->read_byte(*ram, address);
    cpu->lda_op();
}

void _ldx_immediate(CPU* cpu, RAM* ram)
{
    u8 value = cpu->next_byte(*ram);
    printf("LDX_IMMEDIATE (%i): ", value);
    cpu->x = value;
    cpu->ldx_op();
}

void _ldx_zero_page(CPU* cpu, RAM* ram)
{
    u8 address = cpu->next_byte(*ram);
    printf("LDX_ZERO_PAGE (%i): ", address);
    cpu->x = cpu->read_byte(*ram, address);
    cpu->ldx_op();
}

void _ldy_immediate(CPU* cpu, RAM* ram)
{
    u8 value = cpu->next_byte(*ram);
    printf("LDY_IMMEDIATE (%i): ", value);
    cpu->y = value;
    cpu->ldy_op();
}

void _ldy_zero_page(CPU* cpu, RAM* ram)
{
    u8 address = cpu->next_byte(*ram);
    printf("LDY_ZERO_PAGE (%i): ", address);
    cpu->y = cpu->read_byte(*ram, address);
    cpu->ldy_op();
}

void load_instructions(CPU* cpu)
{
    cpu->instruction[LDA_IMMEDIATE] = _lda_immediate;
    cpu->instruction[LDA_ZERO_PAGE] = _lda_zero_page;
    cpu->instruction[LDX_IMMEDIATE] = _ldx_immediate;
    cpu->instruction[LDX_ZERO_PAGE] = _ldx_zero_page;
    cpu->instruction[LDY_IMMEDIATE] = _ldy_immediate;
    cpu->instruction[LDY_ZERO_PAGE] = _ldy_zero_page;
}