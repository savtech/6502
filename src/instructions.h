#pragma once
#include <stdio.h>
#include "types.h"
#include "cpu.h"
#include "ram.h"

// NOP===============================================
constexpr u8 NOP = 0xEA,
             // AND===============================================
    AND_IMM = 0x29,
             AND_ZP = 0x25,
             AND_ZPX = 0x35,
             AND_ABS = 0x2D,
             AND_ABSX = 0x3D,
             AND_ABSY = 0x39,
             // LDA===============================================
    LDA_IMM = 0xA9,
             LDA_ZP = 0xA5,
             LDA_ZPX = 0xB5,
             LDA_ABS = 0xAD,
             LDA_ABSX = 0xBD,
             LDA_ABSY = 0xB9,
             // LDX===============================================
    LDX_IMM = 0xA2,
             LDX_ZP = 0xA6,
             LDX_ZPY = 0xB6,
             LDX_ABS = 0xAE,
             LDX_ABSY = 0xBE,
             // LDY===============================================
    LDY_IMM = 0xA0,
             LDY_ZP = 0xA4,
             LDY_ZPX = 0xB4,
             LDY_ABS = 0xAC,
             LDY_ABSX = 0xBC,
             // SEC===============================================
    SEC = 0x38,
             // SED===============================================
    SED = 0xF8,
             // SEI===============================================
    SEI = 0x78,
             // CLC===============================================
    CLC = 0x18,
             // CLD===============================================
    CLD = 0xD8,
             // CLI===============================================
    CLI = 0x58,
             // CLV===============================================
    CLV = 0xB8,
             // STA===============================================
    STA_ZP = 0x85,
             STA_ZPX = 0x95,
             STA_ABS = 0x8D,
             // STX===============================================
    STX_ZP = 0x86,
             STX_ZPY = 0x96,
             STX_ABS = 0x8E,
             // STY===============================================
    STY_ZP = 0x84,
             STY_ZPX = 0x94,
             STY_ABS = 0x8C,
             // TAX===============================================
    TAX = 0xAA,
             // TAY===============================================
    TAY = 0xA8,
             // TSX===============================================
    TSX = 0xBA,
             // TXA===============================================
    TXA = 0x8A,
             // TXS===============================================
    TXS = 0x9A,
             // TYA===============================================
    TYA = 0x98,
             // DEC===============================================
    DEC_ZP = 0xC6,
             DEC_ZPX = 0xD6,
             DEC_ABS = 0xCE,
             // DEX===============================================
    DEX = 0xCA,
             // DEY===============================================
    DEY = 0x88,
             // INC===============================================
    INC_ZP = 0xE6,
             INC_ZPX = 0xF6,
             INC_ABS = 0xEE,
             // INX===============================================
    INX = 0xE8,
             // INY===============================================
    INY = 0xC8;

void _unsupported(CPU &cpu, RAM &ram)
{
    if (cpu.debug)
    {
        const u16 unsupported_index = cpu.pc - 1;
        printf("Unsupported instruction: 0x%2.2x at 0x%4.4x\n", ram.read(unsupported_index), unsupported_index);
    }
}

void _nop(CPU &cpu, RAM &ram)
{
    cpu.debug_print_instruction("NOP");
}

void _and_imm(CPU &cpu, RAM &ram)
{
    u8 value = cpu.next_u8(ram);
    cpu.a &= value;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("AND_IMM", value);
}

void _and_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.a &= value;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("AND_ZP", value);
}

void _and_zpx(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.x;
    u8 value = cpu.read_u8(ram, address);
    cpu.a &= value;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("AND_ZPX", value);
}

void _and_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.a &= value;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("AND_ABS", value);
}

void _and_absx(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram) + cpu.x;
    u8 value = cpu.read_u8(ram, address);
    cpu.a &= value;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("AND_ABSX", value);
}

void _and_absy(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram) + cpu.y;
    u8 value = cpu.read_u8(ram, address);
    cpu.a &= value;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("AND_ABSY", value);
}

void _lda_imm(CPU &cpu, RAM &ram)
{
    u8 value = cpu.next_u8(ram);
    cpu.a = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDA_IMM", value);
}

void _lda_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.a = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDA_ZP", address);
}

void _lda_zpx(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.x;
    u8 value = cpu.read_u8(ram, address);
    cpu.a = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDA_ZPX", address);
}

void _lda_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    cpu.a = cpu.read_u8(ram, address);
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("LDA_ABS", address);
}

void _lda_absx(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram) + cpu.x;
    cpu.a = cpu.read_u8(ram, address);
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("LDA_ABSX", address);
}

void _lda_absy(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram) + cpu.y;
    cpu.a = cpu.read_u8(ram, address);
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("LDA_ABSY", address);
}

void _ldx_imm(CPU &cpu, RAM &ram)
{
    u8 value = cpu.next_u8(ram);
    cpu.x = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDX_IMM", value);
}

void _ldx_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.x = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDX_ZP", address);
}

void _ldx_zpy(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.y;
    u8 value = cpu.read_u8(ram, address);
    cpu.x = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDX_ZPY", address);
}

void _ldx_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    cpu.x = cpu.read_u8(ram, address);
    cpu.update_status(cpu.x);

    cpu.debug_print_instruction("LDX_ABS", address);
}

void _ldx_absy(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram) + cpu.y;
    cpu.x = cpu.read_u8(ram, address);
    cpu.update_status(cpu.x);

    cpu.debug_print_instruction("LDX_ABSY", address);
}

void _ldy_imm(CPU &cpu, RAM &ram)
{
    u8 value = cpu.next_u8(ram);
    cpu.y = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDY_IMM", value);
}

void _ldy_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.y = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDY_ZP", address);
}

void _ldy_zpx(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.x;
    u8 value = cpu.read_u8(ram, address);
    cpu.y = value;
    cpu.update_status(value);

    cpu.debug_print_instruction("LDY_ZPX", address);
}

void _ldy_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    cpu.y = cpu.read_u8(ram, address);
    cpu.update_status(cpu.y);

    cpu.debug_print_instruction("LDY_ABS", address);
}

void _ldy_absx(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram) + cpu.x;
    cpu.y = cpu.read_u8(ram, address);
    cpu.update_status(cpu.y);

    cpu.debug_print_instruction("LDY_ABSX", address);
}

void _sec(CPU &cpu, RAM &ram)
{
    cpu.set_status(CPU::CARRY_FLAG);

    cpu.debug_print_instruction("SEC");
}

void _sed(CPU &cpu, RAM &ram)
{
    cpu.set_status(CPU::DECIMAL_FLAG);

    cpu.debug_print_instruction("SED");
}

void _sei(CPU &cpu, RAM &ram)
{
    cpu.set_status(CPU::INTERRUPT_FLAG);

    cpu.debug_print_instruction("SEI");
}

void _clc(CPU &cpu, RAM &ram)
{
    cpu.clear_status(CPU::CARRY_FLAG);

    cpu.debug_print_instruction("CLC");
}

void _cld(CPU &cpu, RAM &ram)
{
    cpu.clear_status(CPU::DECIMAL_FLAG);

    cpu.debug_print_instruction("CLD");
}

void _cli(CPU &cpu, RAM &ram)
{
    cpu.clear_status(CPU::INTERRUPT_FLAG);

    cpu.debug_print_instruction("CLI");
}

void _clv(CPU &cpu, RAM &ram)
{
    cpu.clear_status(CPU::OVERFLOW_FLAG);

    cpu.debug_print_instruction("CLV");
}

void _sta_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    cpu.write_byte(ram, address, cpu.a);

    cpu.debug_print_instruction("STA_ZP", address);
}

void _sta_zpx(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.x;
    cpu.write_byte(ram, address, cpu.a);

    cpu.debug_print_instruction("STA_ZPX", address);
}

void _sta_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    cpu.write_byte(ram, address, cpu.a);

    cpu.debug_print_instruction("STA_ABS", address);
}

void _stx_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    cpu.write_byte(ram, address, cpu.x);

    cpu.debug_print_instruction("STX_ZP", address);
}

void _stx_zpy(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.y;
    cpu.write_byte(ram, address, cpu.x);

    cpu.debug_print_instruction("STX_ZPY", address);
}

void _stx_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    cpu.write_byte(ram, address, cpu.x);

    cpu.debug_print_instruction("STX_ABS", address);
}

void _sty_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    cpu.write_byte(ram, address, cpu.y);

    cpu.debug_print_instruction("STY_ZP", address);
}

void _sty_zpx(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.x;
    cpu.write_byte(ram, address, cpu.y);

    cpu.debug_print_instruction("STY_ZPX", address);
}

void _sty_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    cpu.write_byte(ram, address, cpu.y);

    cpu.debug_print_instruction("STY_ABS", address);
}

void _tax(CPU &cpu, RAM &ram)
{
    cpu.x = cpu.a;
    cpu.update_status(cpu.x);

    cpu.debug_print_instruction("TAX", cpu.a);
}

void _tay(CPU &cpu, RAM &ram)
{
    cpu.y = cpu.a;
    cpu.update_status(cpu.y);

    cpu.debug_print_instruction("TAY", cpu.a);
}

void _tsx(CPU &cpu, RAM &ram)
{
    cpu.x = cpu.sp;
    cpu.update_status(cpu.x);

    cpu.debug_print_instruction("TSX", cpu.sp);
}

void _txa(CPU &cpu, RAM &ram)
{
    cpu.a = cpu.x;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("TXA", cpu.x);
}

void _txs(CPU &cpu, RAM &ram)
{
    cpu.sp = cpu.x;

    cpu.debug_print_instruction("TXS", cpu.x);
}

void _tya(CPU &cpu, RAM &ram)
{
    cpu.a = cpu.y;
    cpu.update_status(cpu.a);

    cpu.debug_print_instruction("TXA", cpu.y);
}

void _dec_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.write_byte(ram, address, value - 1);
    cpu.update_status(value - 1);

    cpu.debug_print_instruction("DEC_ZP", address);
}

void _dec_zpx(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.x;
    u8 value = cpu.read_u8(ram, address);
    cpu.write_byte(ram, address, value - 1);
    cpu.update_status(value - 1);

    cpu.debug_print_instruction("DEC_ZPX", address);
}

void _dec_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.write_byte(ram, address, value - 1);
    cpu.update_status(value - 1);

    cpu.debug_print_instruction("DEC_ABS", address);
}

void _dex(CPU &cpu, RAM &ram)
{
    cpu.x--;
    cpu.update_status(cpu.x);

    cpu.debug_print_instruction("DEX");
}

void _dey(CPU &cpu, RAM &ram)
{
    cpu.y--;
    cpu.update_status(cpu.y);

    cpu.debug_print_instruction("DEY");
}

void _inc_zp(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.write_byte(ram, address, value + 1);
    cpu.update_status(value + 1);

    cpu.debug_print_instruction("INC_ZP", address);
}

void _inc_zpx(CPU &cpu, RAM &ram)
{
    u8 address = cpu.next_u8(ram) + cpu.x;
    u8 value = cpu.read_u8(ram, address);
    cpu.write_byte(ram, address, value + 1);
    cpu.update_status(value + 1);

    cpu.debug_print_instruction("INC_ZPX", address);
}

void _inc_abs(CPU &cpu, RAM &ram)
{
    u16 address = cpu.next_u16(ram);
    u8 value = cpu.read_u8(ram, address);
    cpu.write_byte(ram, address, value + 1);
    cpu.update_status(value + 1);

    cpu.debug_print_instruction("INC_ABS", address);
}

void _inx(CPU &cpu, RAM &ram)
{
    cpu.x++;
    cpu.update_status(cpu.x);

    cpu.debug_print_instruction("INX");
}

void _iny(CPU &cpu, RAM &ram)
{
    cpu.y++;
    cpu.update_status(cpu.y);

    cpu.debug_print_instruction("INY");
}

void load_instructions(CPU &cpu)
{
    for (size_t i = 0; i < CPU::MAX_INSTRUCTIONS; i++)
    {
        cpu.instructions[i] = _unsupported;
    }

    cpu.instructions[NOP] = _nop;
    cpu.instructions[AND_IMM] = _and_imm;
    cpu.instructions[AND_ZP] = _and_zp;
    cpu.instructions[AND_ZPX] = _and_zpx;
    cpu.instructions[AND_ABS] = _and_abs;
    cpu.instructions[AND_ABSX] = _and_absx;
    cpu.instructions[AND_ABSY] = _and_absy;
    cpu.instructions[LDA_IMM] = _lda_imm;
    cpu.instructions[LDA_ZP] = _lda_zp;
    cpu.instructions[LDA_ZPX] = _lda_zpx;
    cpu.instructions[LDA_ABS] = _lda_abs;
    cpu.instructions[LDA_ABSX] = _lda_absx;
    cpu.instructions[LDA_ABSY] = _lda_absy;
    cpu.instructions[LDX_IMM] = _ldx_imm;
    cpu.instructions[LDX_ZP] = _ldx_zp;
    cpu.instructions[LDX_ZPY] = _ldx_zpy;
    cpu.instructions[LDX_ABS] = _ldx_abs;
    cpu.instructions[LDX_ABSY] = _ldx_absy;
    cpu.instructions[LDY_IMM] = _ldy_imm;
    cpu.instructions[LDY_ZP] = _ldy_zp;
    cpu.instructions[LDY_ZPX] = _ldy_zpx;
    cpu.instructions[LDY_ABS] = _ldy_abs;
    cpu.instructions[LDY_ABSX] = _ldy_absx;
    cpu.instructions[SEC] = _sec;
    cpu.instructions[SED] = _sed;
    cpu.instructions[SEI] = _sei;
    cpu.instructions[CLC] = _clc;
    cpu.instructions[CLD] = _cld;
    cpu.instructions[CLI] = _cli;
    cpu.instructions[CLV] = _clv;
    cpu.instructions[STA_ZP] = _sta_zp;
    cpu.instructions[STA_ZPX] = _sta_zpx;
    cpu.instructions[STA_ABS] = _sta_abs;
    cpu.instructions[STX_ZP] = _stx_zp;
    cpu.instructions[STX_ZPY] = _stx_zpy;
    cpu.instructions[STX_ABS] = _stx_abs;
    cpu.instructions[STY_ZP] = _sty_zp;
    cpu.instructions[STY_ZPX] = _sty_zpx;
    cpu.instructions[STY_ABS] = _sty_abs;
    cpu.instructions[TAX] = _tax;
    cpu.instructions[TAY] = _tay;
    cpu.instructions[TSX] = _tsx;
    cpu.instructions[TXA] = _txa;
    cpu.instructions[TXS] = _txs;
    cpu.instructions[TYA] = _tya;
    cpu.instructions[DEC_ZP] = _dec_zp;
    cpu.instructions[DEC_ZPX] = _dec_zpx;
    cpu.instructions[DEC_ABS] = _dec_abs;
    cpu.instructions[DEX] = _dex;
    cpu.instructions[DEY] = _dey;
    cpu.instructions[INC_ZP] = _inc_zp;
    cpu.instructions[INC_ZPX] = _inc_zpx;
    cpu.instructions[INC_ABS] = _inc_abs;
    cpu.instructions[INX] = _inx;
    cpu.instructions[INY] = _iny;
}