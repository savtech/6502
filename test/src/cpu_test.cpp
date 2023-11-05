#include "utest.h"
#include "../../src/instructions.h"
#include "../../src/hardware.h"

UTEST_MAIN();

struct Instructions
{
    CPU cpu;
    RAM ram;
};

UTEST_F_SETUP(Instructions)
{
    load_instructions(&utest_fixture->cpu);
    utest_fixture->cpu.debug = false;
}

UTEST_F_TEARDOWN(Instructions) {}

UTEST_F(Instructions, NOP)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = NOP;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x0001, utest_fixture->cpu.pc, "The program counter should be incremented.");
}

UTEST_F(Instructions, LDA_Immediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x00;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_Immediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x40;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_Immediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x80;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x00;

    utest_fixture->ram[0x0000] = LDA_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The A register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x40;

    utest_fixture->ram[0x0000] = LDA_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The A register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x80;

    utest_fixture->ram[0x0000] = LDA_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The A register should have been populated from the zero page address 0x00FF.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x00;

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The A register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x40;

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The A register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x80;

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The A register should have been populated from address 0x008F.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_WrappingCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x007F] = 0x80; //With $0xFF in the X register the A register will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F.

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0xFF;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x007F, utest_fixture->ram.last_address_accessed, "With wrapping, the A register should have been populated from address 0x007F.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_Immediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x00;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_Immediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x40;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_Immediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x80;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x00;

    utest_fixture->ram[0x0000] = LDX_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00.");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The X register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x40;

    utest_fixture->ram[0x0000] = LDX_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The X register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x80;

    utest_fixture->ram[0x0000] = LDX_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The X register should have been populated from the zero page address 0x00FF.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x00;

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The X register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x40;

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The X register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x80;

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The X register should have been populated from address 0x008F.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_WrappingCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x007F] = 0x80; //With $0xFF in the Y register the X register will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F.

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0xFF;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x007F, utest_fixture->ram.last_address_accessed, "With wrapping, the X register should have been populated from address 0x007F.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_Immediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x00;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDY_Immediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x40;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_Immediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x80;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x00;

    utest_fixture->ram[0x0000] = LDY_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The Y register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDY_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x40;

    utest_fixture->ram[0x0000] = LDY_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The Y register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x80;

    utest_fixture->ram[0x0000] = LDY_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.last_address_accessed, "The Y register should have been populated from the zero page address 0x00FF.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, SetC)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = SEC;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_TRUE_MSG(utest_fixture->cpu.s.c, "The carry status flag should be set.");
}

UTEST_F(Instructions, SetD)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = SED;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_TRUE_MSG(utest_fixture->cpu.s.d, "The decimal status flag should be set.");
}

UTEST_F(Instructions, SetI)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = SEI;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_TRUE_MSG(utest_fixture->cpu.s.i, "The interrupt status flag should be set.");
}

UTEST_F(Instructions, ClearC)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.c = 1;

    utest_fixture->ram[0x0000] = CLC;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.c, "The carry status flag should not be set.");
}

UTEST_F(Instructions, ClearD)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.d = 1;

    utest_fixture->ram[0x0000] = CLD;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.d, "The decimal status flag should not be set.");
}

UTEST_F(Instructions, ClearI)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.i = 1;

    utest_fixture->ram[0x0000] = CLI;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.i, "The interrupt status flag should not be set.");
}

UTEST_F(Instructions, ClearV)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.v = 1;

    utest_fixture->ram[0x0000] = CLV;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.v, "The overflow status flag should not be set.");
}

UTEST_F(Instructions, STA_ZeroPage)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x40;
    utest_fixture->ram[0x0002] = STA_ZP;
    utest_fixture->ram[0x0003] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram[0xFF], utest_fixture->cpu.a, "The zero page address 0xFF should contain the A register's value 0x40 (64).");
}

UTEST_F(Instructions, STX_ZeroPage)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x40;
    utest_fixture->ram[0x0002] = STX_ZP;
    utest_fixture->ram[0x0003] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram[0xFF], utest_fixture->cpu.x, "The zero page address 0xFF should contain the X register's value 0x40 (64).");
}

UTEST_F(Instructions, STY_ZeroPage)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x40;
    utest_fixture->ram[0x0002] = STY_ZP;
    utest_fixture->ram[0x0003] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram[0xFF], utest_fixture->cpu.y, "The zero page address 0xFF should contain the Y register's value 0x40 (64).");
}

UTEST_F(Instructions, TAX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x00;
    utest_fixture->ram[0x0002] = TAX;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.a, "The X register should contain the A register's value 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, TAX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x40;
    utest_fixture->ram[0x0002] = TAX;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.a, "The X register should contain the A register's value 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TAX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x80;
    utest_fixture->ram[0x0002] = TAX;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.a, "The X register should contain the A register's value 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TAY_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x00;
    utest_fixture->ram[0x0002] = TAY;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, utest_fixture->cpu.a, "The Y register should contain the A register's value 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, TAY_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x40;
    utest_fixture->ram[0x0002] = TAY;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, utest_fixture->cpu.a, "The Y register should contain the A register's value 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TAY_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x80;
    utest_fixture->ram[0x0002] = TAY;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, utest_fixture->cpu.a, "The X register should contain the A register's value 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TSX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->cpu.sp = 0x00;

    utest_fixture->ram[0x0000] = TSX;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.sp, "The X register should contain the stack pointer register's value 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, TSX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->cpu.sp = 0x40;

    utest_fixture->ram[0x0000] = TSX;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.sp, "The X register should contain the stack pointer register's value 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TSX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->cpu.sp = 0x80;

    utest_fixture->ram[0x0000] = TSX;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.sp, "The X register should contain the stack pointer register's value 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TXA_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x00;
    utest_fixture->ram[0x0002] = TXA;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.x, "The A register should contain the X register's value 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, TXA_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x40;
    utest_fixture->ram[0x0002] = TXA;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.x, "The A register should contain the X register's value 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TXA_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x80;
    utest_fixture->ram[0x0002] = TXA;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.x, "The A register should contain the X register's value 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TXS)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0xFF;
    utest_fixture->ram[0x0002] = TXS;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.sp, utest_fixture->cpu.x, "The stack pointer register should contain the X register's value 0xFF (255).");
}

UTEST_F(Instructions, TYA_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x00;
    utest_fixture->ram[0x0002] = TYA;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.y, "The A register should contain the Y register's value 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, TYA_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x40;
    utest_fixture->ram[0x0002] = TYA;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.y, "The A register should contain the Y register's value 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, TYA_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x80;
    utest_fixture->ram[0x0002] = TYA;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.y, "The A register should contain the Y register's value 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0xFF;

    utest_fixture->ram[0x0000] = INC_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram[0x00FF], 0x00, "The value at memory address 0x00FF should be 0x00 (0).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, INC_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x3F;

    utest_fixture->ram[0x0000] = INC_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram[0x00FF], 0x40, "The value at memory address 0x00FF should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x7F;

    utest_fixture->ram[0x0000] = INC_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram[0x00FF], 0x80, "The value at memory address 0x00FF should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

