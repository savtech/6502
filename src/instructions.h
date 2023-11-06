#pragma once
#include<stdio.h>
#include "types.h"
#include "hardware.h"

//NOP===============================================
constexpr u8 NOP = 0xEA;
//LDA===============================================
constexpr u8 LDA_IMM = 0xA9;
constexpr u8 LDA_ZP = 0xA5;
constexpr u8 LDA_ZPX = 0xB5;
//LDX===============================================
constexpr u8 LDX_IMM = 0xA2;
constexpr u8 LDX_ZP = 0xA6;
constexpr u8 LDX_ZPY = 0xB6;
//LDY===============================================
constexpr u8 LDY_IMM = 0xA0;
constexpr u8 LDY_ZP = 0xA4;
//SEC===============================================
constexpr u8 SEC = 0x38;
//SED===============================================
constexpr u8 SED = 0xF8;
//SEI===============================================
constexpr u8 SEI = 0x78;
//CLC===============================================
constexpr u8 CLC = 0x18;
//CLD===============================================
constexpr u8 CLD = 0xD8;
//CLI===============================================
constexpr u8 CLI = 0x58;
//CLV===============================================
constexpr u8 CLV = 0xB8;
//STA===============================================
constexpr u8 STA_ZP = 0x85;
//STX===============================================
constexpr u8 STX_ZP = 0x86;
//STY===============================================
constexpr u8 STY_ZP = 0x84;
//TAX===============================================
constexpr u8 TAX = 0xAA;
//TAY===============================================
constexpr u8 TAY = 0xA8;
//TSX===============================================
constexpr u8 TSX = 0xBA;
//TXA===============================================
constexpr u8 TXA = 0x8A;
//TXS===============================================
constexpr u8 TXS = 0x9A;
//TYA===============================================
constexpr u8 TYA = 0x98;
//INC===============================================
constexpr u8 INC_ZP = 0xE6;
constexpr u8 INC_ZPX = 0xF6;
//INX===============================================
constexpr u8 INX = 0xE8;
//INY===============================================
constexpr u8 INY = 0xC8;

void _unsupported(CPU& cpu, RAM& ram)
{
    if(cpu.debug)
    {
        const u16 unsupported_index = cpu.pc -1;
        printf("Unsupported instruction: 0x%2.2x at 0x%4.4x\n", ram[unsupported_index], unsupported_index);
    }
}

void _nop(CPU& cpu, RAM& ram)
{
    cpu.debug_print_instruction("NOP");
}

void _lda_imm(CPU& cpu, RAM& ram)
{
    u8 value = cpu.next_byte(ram);
    cpu.a = value;
    cpu.a_op();

    cpu.debug_print_instruction("LDA_IMM", value);
}

void _lda_zp(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram);
    cpu.a = cpu.read_byte(ram, address);
    cpu.a_op();

    cpu.debug_print_instruction("LDA_ZP", address);
}

void _lda_zpx(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram) + cpu.x;
    cpu.a = cpu.read_byte(ram, address);
    cpu.a_op();

    cpu.debug_print_instruction("LDA_ZPX", address);
}

void _ldx_imm(CPU& cpu, RAM& ram)
{
    u8 value = cpu.next_byte(ram);
    cpu.x = value;
    cpu.x_op();

    cpu.debug_print_instruction("LDX_IMM", value);
}

void _ldx_zp(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram);
    cpu.x = cpu.read_byte(ram, address);
    cpu.x_op();

    cpu.debug_print_instruction("LDX_ZP", address);
}

void _ldx_zpy(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram) + cpu.y;
    cpu.x = cpu.read_byte(ram, address);
    cpu.x_op();

    cpu.debug_print_instruction("LDX_ZPY", address);    
}

void _ldy_imm(CPU& cpu, RAM& ram)
{
    u8 value = cpu.next_byte(ram);
    cpu.y = value;
    cpu.y_op();

    cpu.debug_print_instruction("LDY_IMM", value);
}

void _ldy_zp(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram);
    cpu.y = cpu.read_byte(ram, address);
    cpu.y_op();

    cpu.debug_print_instruction("LDY_ZP", address);
}

void _sec(CPU& cpu, RAM& ram)
{
    cpu.s.c = 1;

    cpu.debug_print_instruction("SEC");
}

void _sed(CPU& cpu, RAM& ram)
{
    cpu.s.d = 1;

    cpu.debug_print_instruction("SED");
}

void _sei(CPU& cpu, RAM& ram)
{
    cpu.s.i = 1;

    cpu.debug_print_instruction("SEI");
}

void _clc(CPU& cpu, RAM& ram)
{
    cpu.s.c = 0;

    cpu.debug_print_instruction("CLC");
}

void _cld(CPU& cpu, RAM& ram)
{
    cpu.s.d = 0;

    cpu.debug_print_instruction("CLD");
}

void _cli(CPU& cpu, RAM& ram)
{
    cpu.s.i = 0;

    cpu.debug_print_instruction("CLI");
}

void _clv(CPU& cpu, RAM& ram)
{
    cpu.s.v = 0;
    
    cpu.debug_print_instruction("CLV");
}

void _sta_zp(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram);
    cpu.write_byte(ram, address, cpu.a);
    
    cpu.debug_print_instruction("STA_ZP", address);
}

void _stx_zp(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram);
    cpu.write_byte(ram, address, cpu.x);

    cpu.debug_print_instruction("STX_ZP", address);
}

void _sty_zp(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram);
    cpu.write_byte(ram, address, cpu.y);

    cpu.debug_print_instruction("STY_ZP", address);
}

void _tax(CPU& cpu, RAM& ram)
{
    cpu.x = cpu.a;
    cpu.x_op();

    cpu.debug_print_instruction("TAX", cpu.a);
}

void _tay(CPU& cpu, RAM& ram)
{
    cpu.y = cpu.a;
    cpu.y_op();

    cpu.debug_print_instruction("TAY", cpu.a);
}

void _tsx(CPU& cpu, RAM& ram)
{
    cpu.x = cpu.sp;
    cpu.x_op();

    cpu.debug_print_instruction("TSX", cpu.sp);
}

void _txa(CPU& cpu, RAM& ram)
{
    cpu.a = cpu.x;
    cpu.a_op();

    cpu.debug_print_instruction("TXA", cpu.x);
}

void _txs(CPU& cpu, RAM& ram)
{
    cpu.sp = cpu.x;

    cpu.debug_print_instruction("TXS", cpu.x);
}

void _tya(CPU& cpu, RAM& ram)
{
    cpu.a = cpu.y;
    cpu.a_op();

    cpu.debug_print_instruction("TXA", cpu.y);
}

void _inc_zp(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram);
    u8 value = cpu.read_byte(ram, address);
    cpu.write_byte(ram, address, value + 1);
    cpu.zp_op(ram, address);

    cpu.debug_print_instruction("INC_ZP", address);
}

void _inc_zpx(CPU& cpu, RAM& ram)
{
    u8 address = cpu.next_byte(ram) + cpu.x;
    (ram)[address]++;
    cpu.zp_op(ram, address);

    cpu.debug_print_instruction("INC_ZPX", address);
}

void _inx(CPU& cpu, RAM& ram)
{
    cpu.x++;
    cpu.x_op(); 
    cpu.debug_print_instruction("INX");
}

void _iny(CPU& cpu, RAM& ram)
{
    cpu.y++;
    cpu.y_op();
    cpu.debug_print_instruction("INY");
}

void load_instructions(CPU& cpu)
{
    for(size_t i = 0; i < CPU::MAX_INSTRUCTIONS; i++)
    {
        cpu.instruction[i] = _unsupported;
    }

    cpu.instruction[NOP] = _nop;
    cpu.instruction[LDA_IMM] = _lda_imm;
    cpu.instruction[LDA_ZP] = _lda_zp;
    cpu.instruction[LDA_ZPX] = _lda_zpx;
    cpu.instruction[LDX_IMM] = _ldx_imm;
    cpu.instruction[LDX_ZP] = _ldx_zp;
    cpu.instruction[LDX_ZPY] = _ldx_zpy;
    cpu.instruction[LDY_IMM] = _ldy_imm;
    cpu.instruction[LDY_ZP] = _ldy_zp;
    cpu.instruction[SEC] = _sec;
    cpu.instruction[SED] = _sed;
    cpu.instruction[SEI] = _sei;
    cpu.instruction[CLC] = _clc;
    cpu.instruction[CLD] = _cld;
    cpu.instruction[CLI] = _cli;
    cpu.instruction[CLV] = _clv;
    cpu.instruction[STA_ZP] = _sta_zp;
    cpu.instruction[STX_ZP] = _stx_zp;
    cpu.instruction[STY_ZP] = _sty_zp;
    cpu.instruction[TAX] = _tax;
    cpu.instruction[TAY] = _tay;
    cpu.instruction[TSX] = _tsx;
    cpu.instruction[TXA] = _txa;
    cpu.instruction[TXS] = _txs;
    cpu.instruction[TYA] = _tya;
    cpu.instruction[INC_ZP] = _inc_zp;
    cpu.instruction[INC_ZPX] = _inc_zpx;
    cpu.instruction[INX] = _inx;
    cpu.instruction[INY] = _iny;
}