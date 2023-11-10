#include "utest.h"
#include "../../src/instructions.h"
#include "../../src/cpu.h"
#include "../../src/ram.h"

UTEST_MAIN();

struct HardwareFunctionality
{
    CPU cpu;
    RAM ram;
};

UTEST_F_SETUP(HardwareFunctionality)
{
    load_instructions(utest_fixture->cpu);
    utest_fixture->cpu.debug = false;
}

UTEST_F_TEARDOWN(HardwareFunctionality) {}

struct Instructions
{
    CPU cpu;
    RAM ram;
};

UTEST_F_SETUP(Instructions)
{
    load_instructions(utest_fixture->cpu);
    utest_fixture->cpu.debug = false;
}

UTEST_F_TEARDOWN(Instructions) {}

UTEST_F(HardwareFunctionality, Read_U8)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, NOP);
    utest_fixture->ram.write(0x0001, 0xFF);

    u8 data = utest_fixture->cpu.next_u8(utest_fixture->ram);

    ASSERT_EQ_MSG(data, 0xFF, "The byte read should be 0xFF.");
}

UTEST_F(HardwareFunctionality, Read_U16)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, NOP);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, 0xFF);

    u16 data = utest_fixture->cpu.next_u16(utest_fixture->ram);

    ASSERT_EQ_MSG(data, 0xFFFF, "The two bytes read from memory should equate to 0xFFFF as an u16.");
}

UTEST_F(Instructions, NOP)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, NOP);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x0001, utest_fixture->cpu.pc, "The program counter should be incremented by 1.");
}

UTEST_F(Instructions, AND_Immediate_ZeroCase)
{
    static constexpr size_t instruction_count = 2;
 
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, AND_IMM);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, AND_Immediate_PositiveCase)
{
    static constexpr size_t instruction_count = 2;
    
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, AND_IMM);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_Immediate_NegativeCase)
{
    static constexpr size_t instruction_count = 2;
    
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, AND_IMM);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0xFF, utest_fixture->cpu.a, "The A register's value should be 0xFF (255).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_ZeroPage_ZeroCase)
{
    static constexpr size_t instruction_count = 2;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, AND_ZP);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, AND_ZeroPage_PositiveCase)
{
    static constexpr size_t instruction_count = 2;
    
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0xFF);

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, AND_ZP);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_ZeroPage_NegativeCase)
{
    static constexpr size_t instruction_count = 2;
    
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0xFF);

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, AND_ZP);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0xFF, utest_fixture->cpu.a, "The A register's value should be 0xFF (255).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x00FF.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_ZeroPageX_ZeroCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0xFF);

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0x0F);
    utest_fixture->ram.write(0x0004, AND_ZPX);
    utest_fixture->ram.write(0x0005, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, AND_ZeroPageX_PositiveCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0xFF);

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0x0F);
    utest_fixture->ram.write(0x0004, AND_ZPX);
    utest_fixture->ram.write(0x0005, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_ZeroPageX_NegativeCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0xFF);

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0x0F);
    utest_fixture->ram.write(0x0004, AND_ZPX);
    utest_fixture->ram.write(0x0005, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0xFF, utest_fixture->cpu.a, "The A register's value should be 0xFF (255).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x008F.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_Absolute_ZeroCase)
{
    static constexpr size_t instruction_count = 2;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, AND_ABS);
    utest_fixture->ram.write(0x0003, 0xBB);
    utest_fixture->ram.write(0x0004, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, AND_Absolute_PositiveCase)
{
    static constexpr size_t instruction_count = 2;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, AND_ABS);
    utest_fixture->ram.write(0x0003, 0xBB);
    utest_fixture->ram.write(0x0004, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_Absolute_NegativeCase)
{
    static constexpr size_t instruction_count = 2;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, AND_ABS);
    utest_fixture->ram.write(0x0003, 0xBB);
    utest_fixture->ram.write(0x0004, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0xFF, utest_fixture->cpu.a, "The A register's value should be 0xFF (255).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0xAABB.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_AbsoluteX_ZeroCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0x5C);
    utest_fixture->ram.write(0x0004, AND_ABSX);
    utest_fixture->ram.write(0x0005, 0xD0);
    utest_fixture->ram.write(0x0006, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x082C, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, AND_AbsoluteX_PositiveCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0x5C);
    utest_fixture->ram.write(0x0004, AND_ABSX);
    utest_fixture->ram.write(0x0005, 0xD0);
    utest_fixture->ram.write(0x0006, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x082C, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_AbsoluteX_NegativeCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0x5C);
    utest_fixture->ram.write(0x0004, AND_ABSX);
    utest_fixture->ram.write(0x0005, 0xD0);
    utest_fixture->ram.write(0x0006, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0xFF, utest_fixture->cpu.a, "The A register's value should be 0xFF (255).");
    ASSERT_EQ_MSG(0x082C, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x082C.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_AbsoluteY_ZeroCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, LDY_IMM);
    utest_fixture->ram.write(0x0003, 0x5C);
    utest_fixture->ram.write(0x0004, AND_ABSY);
    utest_fixture->ram.write(0x0005, 0xD0);
    utest_fixture->ram.write(0x0006, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x082C, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, AND_AbsoluteY_PositiveCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, LDY_IMM);
    utest_fixture->ram.write(0x0003, 0x5C);
    utest_fixture->ram.write(0x0004, AND_ABSY);
    utest_fixture->ram.write(0x0005, 0xD0);
    utest_fixture->ram.write(0x0006, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x082C, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, AND_AbsoluteY_NegativeCase)
{
    static constexpr size_t instruction_count = 3;
 
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0xFF);
    
    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, LDY_IMM);
    utest_fixture->ram.write(0x0003, 0x5C);
    utest_fixture->ram.write(0x0004, AND_ABSY);
    utest_fixture->ram.write(0x0005, 0xD0);
    utest_fixture->ram.write(0x0006, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0xFF, utest_fixture->cpu.a, "The A register's value should be 0xFF (255).");
    ASSERT_EQ_MSG(0x082C, utest_fixture->ram.most_recent_read, "The A register should have been compared to the zero page address 0x082C.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_Immediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_Immediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_Immediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x00);

    utest_fixture->ram.write(0x0000, LDA_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The A register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x40);

    utest_fixture->ram.write(0x0000, LDA_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The A register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x80);

    utest_fixture->ram.write(0x0000, LDA_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The A register should have been populated from the zero page address 0x00FF.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x00);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDA_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);
    
    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The A register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x40);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDA_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The A register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x80);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDA_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The A register should have been populated from address 0x008F.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_ZeroPageX_WrappingCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x007F, 0x80); //With $0xFF in the X register the A register will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F.

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, LDA_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x007F, utest_fixture->ram.most_recent_read, "With wrapping, the A register should have been populated from address 0x007F.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_Absolute_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x00);

    utest_fixture->ram.write(0x0000, LDA_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The A register should have been populated from the address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_Absolute_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x40);

    utest_fixture->ram.write(0x0000, LDA_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The A register should have been populated from the address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_Absolute_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x80);

    utest_fixture->ram.write(0x0000, LDA_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The A register should have been populated from the address 0xAABB.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_AbsoluteX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x00);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDA_ABSX);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The A register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_AbsoluteX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x40);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDA_ABSX);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The A register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_AbsoluteX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x80);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDA_ABSX);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The A register should have been populated from the address 0x082C.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_AbsoluteY_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x00);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDA_ABSY);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.a, "The A register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The A register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDA_AbsoluteY_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x40);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDA_ABSY);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.a, "The A register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The A register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDA_AbsoluteY_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x80);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDA_ABSY);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.a, "The A register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The A register should have been populated from the address 0x082C.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_Immediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x00);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_Immediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x40);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_Immediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x00);

    utest_fixture->ram.write(0x0000, LDX_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00.");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The X register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x40);

    utest_fixture->ram.write(0x0000, LDX_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The X register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x80);

    utest_fixture->ram.write(0x0000, LDX_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The X register should have been populated from the zero page address 0x00FF.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x00);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDX_ZPY);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The X register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x40);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDX_ZPY);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The X register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x80);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDX_ZPY);
    utest_fixture->ram.write(0x0003, 0x80);
    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The X register should have been populated from address 0x008F.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_ZeroPageY_WrappingCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x007F, 0x80); //With $0xFF in the Y register the X register will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F.

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, LDX_ZPY);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x007F, utest_fixture->ram.most_recent_read, "With wrapping, the X register should have been populated from address 0x007F.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_Absolute_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x00);

    utest_fixture->ram.write(0x0000, LDX_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The X register should have been populated from the address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_Absolute_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x40);

    utest_fixture->ram.write(0x0000, LDX_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The X register should have been populated from the address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_Absolute_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x80);

    utest_fixture->ram.write(0x0000, LDX_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The X register should have been populated from the address 0xAABB.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_AbsoluteY_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x00);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDX_ABSY);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.x, "The X register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The X register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDX_AbsoluteY_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x40);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDX_ABSY);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.x, "The X register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The X register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDX_AbsoluteY_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x80);

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDX_ABSY);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.x, "The X register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The X register should have been populated from the address 0x082C.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_Immediate_ZeroCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x00);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDY_Immediate_PositiveCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x40);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_Immediate_NegativeCase)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x00);

    utest_fixture->ram.write(0x0000, LDY_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The Y register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDY_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x40);

    utest_fixture->ram.write(0x0000, LDY_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The Y register should have been populated from the zero page address 0x00FF.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x80);

    utest_fixture->ram.write(0x0000, LDY_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x00FF, utest_fixture->ram.most_recent_read, "The Y register should have been populated from the zero page address 0x00FF.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_ZeroPageX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x00);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDY_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The Y register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDY_ZeroPageX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x40);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDY_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The Y register should have been populated from address 0x008F.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_ZeroPageX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x80);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, LDY_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);
    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "The Y register should have been populated from address 0x008F.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_ZeroPageX_WrappingCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x007F, 0x80); //With $0xFF in the Y register the X register will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F.

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, LDY_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(0x007F, utest_fixture->ram.most_recent_read, "With wrapping, the Y register should have been populated from address 0x007F.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_Absolute_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x00);

    utest_fixture->ram.write(0x0000, LDY_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The Y register should have been populated from the address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDY_Absolute_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x40);

    utest_fixture->ram.write(0x0000, LDY_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The Y register should have been populated from the address 0xAABB.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_Absolute_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x80);

    utest_fixture->ram.write(0x0000, LDY_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0xAABB, "The Y register should have been populated from the address 0xAABB.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_AbsoluteX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x00);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDY_ABSX);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x00, utest_fixture->cpu.y, "The Y register's value should be 0x00 (0).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The Y register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, LDY_AbsoluteX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x40);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDY_ABSX);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x40, utest_fixture->cpu.y, "The Y register's value should be 0x40 (64).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The Y register should have been populated from the address 0x082C.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, LDY_AbsoluteX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x082C, 0x80);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x5C);
    utest_fixture->ram.write(0x0002, LDY_ABSX);
    utest_fixture->ram.write(0x0003, 0xD0);
    utest_fixture->ram.write(0x0004, 0x07);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(0x80, utest_fixture->cpu.y, "The Y register's value should be 0x80 (128).");
    ASSERT_EQ_MSG(utest_fixture->ram.most_recent_read, 0x082C, "The Y register should have been populated from the address 0x082C.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, SetC)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, SEC);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_TRUE_MSG((utest_fixture->cpu.s & 0x01), "The carry status flag should be set.");
}

UTEST_F(Instructions, SetD)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, SED);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_TRUE_MSG((utest_fixture->cpu.s & 0x08), "The decimal status flag should be set.");
}

UTEST_F(Instructions, SetI)
{
    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, SEI);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_TRUE_MSG((utest_fixture->cpu.s & 0x04), "The interrupt status flag should be set.");
}

UTEST_F(Instructions, ClearC)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s |= 0x01;

    utest_fixture->ram.write(0x0000, CLC);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG((utest_fixture->cpu.s & 0x01), "The carry status flag should not be set.");
}

UTEST_F(Instructions, ClearD)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s |= 0x08;

    utest_fixture->ram.write(0x0000, CLD);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG((utest_fixture->cpu.s & 0x08), "The decimal status flag should not be set.");
}

UTEST_F(Instructions, ClearI)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s |= 0x04;

    utest_fixture->ram.write(0x0000, CLI);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG((utest_fixture->cpu.s & 0x04), "The interrupt status flag should not be set.");
}

UTEST_F(Instructions, ClearV)
{
    utest_fixture->cpu.reset();
    utest_fixture->cpu.s |= 0x40;

    utest_fixture->ram.write(0x0000, CLV);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_FALSE_MSG((utest_fixture->cpu.s & 0x40), "The overflow status flag should not be set.");
}

UTEST_F(Instructions, STA_ZeroPage)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, STA_ZP);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xFF), utest_fixture->cpu.a, "The zero page address 0xFF should contain the A register's value 0x40 (64).");
}

UTEST_F(Instructions, STA_ZeroPageX)
{
    static constexpr size_t instruction_count = 3;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0xFF);
    utest_fixture->ram.write(0x0004, STA_ZPX); 
    utest_fixture->ram.write(0x0005, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    //With $0xFF in the X register the A register will be stored in $007F (e.g. $80 + $FF => $7F) and not $017F.
    ASSERT_EQ_MSG(utest_fixture->ram.read(0x007F), utest_fixture->cpu.a, "The zero page address 0x007F should contain the A register's value 0x40 (64).");
}

UTEST_F(Instructions, STA_Absolute)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, STA_ABS);
    utest_fixture->ram.write(0x0003, 0xBB);
    utest_fixture->ram.write(0x0004, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), utest_fixture->cpu.a, "The address 0xAABB should contain the A register's value 0x40 (64).");
}

UTEST_F(Instructions, STX_ZeroPage)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, STX_ZP);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xFF), utest_fixture->cpu.x, "The zero page address 0xFF should contain the X register's value 0x40 (64).");
}

UTEST_F(Instructions, STX_ZeroPageY)
{
    static constexpr size_t instruction_count = 3;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, LDY_IMM);
    utest_fixture->ram.write(0x0003, 0xFF);
    utest_fixture->ram.write(0x0004, STX_ZPY); 
    utest_fixture->ram.write(0x0005, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    //With $0xFF in the Y register the X register will be stored in $007F (e.g. $80 + $FF => $7F) and not $017F.
    ASSERT_EQ_MSG(utest_fixture->ram.read(0x007F), utest_fixture->cpu.x, "The zero page address 0x007F should contain the X register's value 0x40 (64).");
}

UTEST_F(Instructions, STX_Absolute)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, STX_ABS);
    utest_fixture->ram.write(0x0003, 0xBB);
    utest_fixture->ram.write(0x0004, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), utest_fixture->cpu.x, "The address 0xAABB should contain the X register's value 0x40 (64).");
}

UTEST_F(Instructions, STY_ZeroPage)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, STY_ZP);
    utest_fixture->ram.write(0x0003, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xFF), utest_fixture->cpu.y, "The zero page address 0xFF should contain the Y register's value 0x40 (64).");
}

UTEST_F(Instructions, STY_ZeroPageX)
{
    static constexpr size_t instruction_count = 3;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, LDX_IMM);
    utest_fixture->ram.write(0x0003, 0xFF);
    utest_fixture->ram.write(0x0004, STY_ZPX); 
    utest_fixture->ram.write(0x0005, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    //With $0xFF in the X register the Y register will be stored in $007F (e.g. $80 + $FF => $7F) and not $017F.
    ASSERT_EQ_MSG(utest_fixture->ram.read(0x007F), utest_fixture->cpu.y, "The zero page address 0x007F should contain the Y register's value 0x40 (64).");
}

UTEST_F(Instructions, STY_Absolute)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, STY_ABS);
    utest_fixture->ram.write(0x0003, 0xBB);
    utest_fixture->ram.write(0x0004, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), utest_fixture->cpu.y, "The address 0xAABB should contain the Y register's value 0x40 (64).");
}

UTEST_F(Instructions, TAX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, TAX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.a, "The X register should contain the A register's value 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, TAX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, TAX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.a, "The X register should contain the A register's value 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TAX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x80);
    utest_fixture->ram.write(0x0002, TAX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.a, "The X register should contain the A register's value 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TAY_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, TAY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, utest_fixture->cpu.a, "The Y register should contain the A register's value 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, TAY_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, TAY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, utest_fixture->cpu.a, "The Y register should contain the A register's value 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TAY_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDA_IMM);
    utest_fixture->ram.write(0x0001, 0x80);
    utest_fixture->ram.write(0x0002, TAY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, utest_fixture->cpu.a, "The X register should contain the A register's value 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TSX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->cpu.sp = 0x00;

    utest_fixture->ram.write(0x0000, TSX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.sp, "The X register should contain the stack pointer register's value 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, TSX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->cpu.sp = 0x40;

    utest_fixture->ram.write(0x0000, TSX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.sp, "The X register should contain the stack pointer register's value 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TSX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->cpu.sp = 0x80;

    utest_fixture->ram.write(0x0000, TSX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, utest_fixture->cpu.sp, "The X register should contain the stack pointer register's value 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TXA_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, TXA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.x, "The A register should contain the X register's value 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, TXA_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, TXA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.x, "The A register should contain the X register's value 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TXA_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x80);
    utest_fixture->ram.write(0x0002, TXA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.x, "The A register should contain the X register's value 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TXS)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, TXS);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.sp, utest_fixture->cpu.x, "The stack pointer register should contain the X register's value 0xFF (255).");
}

UTEST_F(Instructions, TYA_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, TYA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.y, "The A register should contain the Y register's value 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, TYA_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x40);
    utest_fixture->ram.write(0x0002, TYA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.y, "The A register should contain the Y register's value 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, TYA_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x80);
    utest_fixture->ram.write(0x0002, TYA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.a, utest_fixture->cpu.y, "The A register should contain the Y register's value 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEC_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x01);

    utest_fixture->ram.write(0x0000, DEC_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x00FF), 0x00, "The value at memory address 0x00FF should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, DEC_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x41);

    utest_fixture->ram.write(0x0000, DEC_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x00FF), 0x40, "The value at memory address 0x00FF should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEC_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x00);

    utest_fixture->ram.write(0x0000, DEC_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x00FF), 0xFF, "The value at memory address 0x00FF should be 0xFF (255).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEC_ZeroPageX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x01);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, DEC_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x008F), 0x00, "The value at memory address 0x008F should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "Memory address 0x008F should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, DEC_ZeroPageX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x41);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, DEC_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x008F), 0x40, "The value at memory address 0x008F should be 0x40 (64).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "Memory address 0x008F should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEC_ZeroPageX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x00);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, DEC_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x008F), 0xFF, "The value at memory address 0x008F should be 0xFF (255).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "Memory address 0x008F should have been the most recently accessed memory index.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEC_Absolute_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x01);

    utest_fixture->ram.write(0x0000, DEC_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), 0x00, "The value at memory address 0xAABB should be 0x00 (0).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "Memory address 0xAABB should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, DEC_Absolute_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x41);

    utest_fixture->ram.write(0x0000, DEC_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), 0x40, "The value at memory address 0xAABB should be 0x40 (64).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "Memory address 0xAABB should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEC_Absolute_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x00);

    utest_fixture->ram.write(0x0000, DEC_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), 0xFF, "The value at memory address 0xAABB should be 0xFF (255).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "Memory address 0xAABB should have been the most recently accessed memory index.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEX_ZeroCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x01);
    utest_fixture->ram.write(0x0002, DEX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, 0x00, "The X register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, DEX_PositiveCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x41);
    utest_fixture->ram.write(0x0002, DEX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, 0x40, "The X register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEX_NegativeCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, DEX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, 0xFF, "The X register's value should be 0xFF (255).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEY_ZeroCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x01);
    utest_fixture->ram.write(0x0002, DEY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, 0x00, "The Y register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, DEY_PositiveCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x41);
    utest_fixture->ram.write(0x0002, DEY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, 0x40, "The Y register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, DEY_NegativeCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x00);
    utest_fixture->ram.write(0x0002, DEY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, 0xFF, "The Y register's value should be 0xFF (255).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_ZeroPage_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0xFF);

    utest_fixture->ram.write(0x0000, INC_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x00FF), 0x00, "The value at memory address 0x00FF should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, INC_ZeroPage_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x3F);

    utest_fixture->ram.write(0x0000, INC_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x00FF), 0x40, "The value at memory address 0x00FF should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_ZeroPage_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x00FF, 0x7F);

    utest_fixture->ram.write(0x0000, INC_ZP);
    utest_fixture->ram.write(0x0001, 0xFF);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x00FF), 0x80, "The value at memory address 0x00FF should be 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_ZeroPageX_ZeroCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0xFF);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, INC_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x008F), 0x00, "The value at memory address 0x008F should be 0x00 (0).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "Memory address 0x008F should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, INC_ZeroPageX_PositiveCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x00);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, INC_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x008F), 0x01, "The value at memory address 0x008F should be 0x01 (1).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "Memory address 0x008F should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_ZeroPageX_NegativeCase)
{
    static constexpr size_t instruction_count = 2;

    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0x008F, 0x80);

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x0F);
    utest_fixture->ram.write(0x0002, INC_ZPX);
    utest_fixture->ram.write(0x0003, 0x80);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, instruction_count);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0x008F), 0x81, "The value at memory address 0x008F should be 0x81 (129).");
    ASSERT_EQ_MSG(0x008F, utest_fixture->ram.most_recent_read, "Memory address 0x008F should have been the most recently accessed memory index.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_Absolute_ZeroCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0xFF);

    utest_fixture->ram.write(0x0000, INC_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), 0x00, "The value at memory address 0xAABB should be 0x00 (0).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "Memory address 0xAABB should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, INC_Absolute_PositiveCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x3F);

    utest_fixture->ram.write(0x0000, INC_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), 0x40, "The value at memory address 0xAABB should be 0x40 (64).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "Memory address 0xAABB should have been the most recently accessed memory index.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INC_Absolute_NegativeCase)
{
    utest_fixture->cpu.reset();
    utest_fixture->ram.write(0xAABB, 0x7F);

    utest_fixture->ram.write(0x0000, INC_ABS);
    utest_fixture->ram.write(0x0001, 0xBB);
    utest_fixture->ram.write(0x0002, 0xAA);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram);

    ASSERT_EQ_MSG(utest_fixture->ram.read(0xAABB), 0x80, "The value at memory address 0xAABB should be 0x80 (128).");
    ASSERT_EQ_MSG(0xAABB, utest_fixture->ram.most_recent_read, "Memory address 0xAABB should have been the most recently accessed memory index.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INX_ZeroCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, INX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, 0x00, "The X register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, INX_PositiveCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x3F);
    utest_fixture->ram.write(0x0002, INX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, 0x40, "The X register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INX_NegativeCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDX_IMM);
    utest_fixture->ram.write(0x0001, 0x7F);
    utest_fixture->ram.write(0x0002, INX);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.x, 0x80, "The X register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INY_ZeroCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0xFF);
    utest_fixture->ram.write(0x0002, INY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, 0x00, "The Y register's value should be 0x00 (0).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should be set.");
}

UTEST_F(Instructions, INY_PositiveCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x3F);
    utest_fixture->ram.write(0x0002, INY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, 0x40, "The Y register's value should be 0x40 (64).");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should not be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

UTEST_F(Instructions, INY_NegativeCase)
{
    static constexpr size_t intstruction_count = 2;

    utest_fixture->cpu.reset();

    utest_fixture->ram.write(0x0000, LDY_IMM);
    utest_fixture->ram.write(0x0001, 0x7F);
    utest_fixture->ram.write(0x0002, INY);

    utest_fixture->cpu.execute_instructions(utest_fixture->ram, intstruction_count);

    ASSERT_EQ_MSG(utest_fixture->cpu.y, 0x80, "The Y register's value should be 0x80 (128).");
    ASSERT_TRUE_MSG((utest_fixture->cpu.s & CPU::NEGATIVE_FLAG), "The negative status flag should be set.");
    ASSERT_FALSE_MSG((utest_fixture->cpu.s & CPU::ZERO_FLAG), "The zero status flag should not be set.");
}

