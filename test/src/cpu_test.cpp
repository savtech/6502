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

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x0001, utest_fixture->cpu.pc, "The program counter should be incremented.");
}

UTEST_F(Instructions, LDAImmediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x00;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDAImmediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x40;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDAImmediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDA_IMM;
    utest_fixture->ram[0x0001] = 0x80;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDAZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x00;

    utest_fixture->ram[0x0000] = LDA_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDAZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x40;

    utest_fixture->ram[0x0000] = LDA_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDAZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x80;

    utest_fixture->ram[0x0000] = LDA_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDAZeroPageX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x00;

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The memory location should have been taken from 0x008F.");
}

UTEST_F(Instructions, LDAZeroPageX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x40;

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The memory location should have been taken from 0x008F.");
}

UTEST_F(Instructions, LDAZeroPageX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x80;

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The memory location should have been taken from 0x008F.");
}

UTEST_F(Instructions, LDAZeroPageX_WrappingCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x007F] = 0x80; //With $0xFF in the X register the A register will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F.

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0xFF;
    utest_fixture->ram[0x0002] = LDA_ZPX;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
    ASSERT_EQ_MSG(0x007F, utest_fixture->ram.last_address_accessed, "With wrapping, the memory location should have been taken from 0x007F.");
}

UTEST_F(Instructions, LDXImmediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x00;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDXImmediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x40;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDXImmediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDX_IMM;
    utest_fixture->ram[0x0001] = 0x80;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDXZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x00;

    utest_fixture->ram[0x0000] = LDX_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDXZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x40;

    utest_fixture->ram[0x0000] = LDX_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDXZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x80;

    utest_fixture->ram[0x0000] = LDX_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDXZeroPageY_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x00;

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The memory location should have been taken from 0x008F.");
}

UTEST_F(Instructions, LDXZeroPageY_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x40;

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;

    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The memory location should have been taken from 0x008F.");
}

UTEST_F(Instructions, LDXZeroPageY_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x008F] = 0x80;

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x0F;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.last_address_accessed, "The memory location should have been taken from 0x008F.");
}

UTEST_F(Instructions, LDXZeroPageY_WrappingCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram[0x007F] = 0x80; //With $0xFF in the Y register the X register will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F.

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0xFF;
    utest_fixture->ram[0x0002] = LDX_ZPY;
    utest_fixture->ram[0x0003] = 0x80;
    utest_fixture->cpu.execute(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
    ASSERT_EQ_MSG(0x007F, utest_fixture->ram.last_address_accessed, "With wrapping, the memory location should have been taken from 0x007F.");
}

UTEST_F(Instructions, LDYImmediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x00;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDYImmediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x40;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDYImmediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = LDY_IMM;
    utest_fixture->ram[0x0001] = 0x80;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDYZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x00;

    utest_fixture->ram[0x0000] = LDY_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register should be zero.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.z, "The zero status flag should be set.");
}

UTEST_F(Instructions, LDYZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x40;

    utest_fixture->ram[0x0000] = LDY_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register should be 0x40 (64).");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.n, "The negative status flag should not be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDYZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram[0x00FF] = 0x80;

    utest_fixture->ram[0x0000] = LDY_ZP;
    utest_fixture->ram[0x0001] = 0xFF;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register should be 0x80 (128).");
    ASSERT_TRUE_MSG(utest_fixture->cpu.s.n, "The negative status flag should be set.");
    ASSERT_FALSE_MSG(utest_fixture->cpu.s.z, "The zero status flag should not be set.");
}

UTEST_F(Instructions, SetC)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = SEC;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_TRUE_MSG(utest_fixture->cpu.s.c, "The carry status flag should be set.");
}

UTEST_F(Instructions, SetD)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = SED;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_TRUE_MSG(utest_fixture->cpu.s.d, "The decimal status flag should be set.");
}

UTEST_F(Instructions, SetI)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram[0x0000] = SEI;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_TRUE_MSG(utest_fixture->cpu.s.i, "The interrupt status flag should be set.");
}

UTEST_F(Instructions, ClearC)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.c = 1;

    utest_fixture->ram[0x0000] = CLC;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.c, "The carry status flag should not be set.");
}

UTEST_F(Instructions, ClearD)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.d = 1;

    utest_fixture->ram[0x0000] = CLD;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.d, "The decimal status flag should not be set.");
}

UTEST_F(Instructions, ClearI)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.i = 1;

    utest_fixture->ram[0x0000] = CLI;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.i, "The interrupt status flag should not be set.");
}

UTEST_F(Instructions, ClearV)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s.v = 1;

    utest_fixture->ram[0x0000] = CLV;

    utest_fixture->cpu.execute(utest_fixture->ram);

    ASSERT_FALSE_MSG(utest_fixture->cpu.s.v, "The overflow status flag should not be set.");
}

