#include "utest.h"
//#include "../../src/hardware.h"
//#include "../../src/instructions.h"

UTEST_MAIN();

UTEST(foo, bar)
{
    ASSERT_TRUE(1);
}


// TEST(InstructionsTests, LDAImmediate) {
//     CPU cpu = {};
//     RAM ram = {};

//     ram[0] = LDA_IMM;
//     ram[1] = 0;
//     cpu.execute(ram);

//     EXPECT_EQ(0, cpu.a);
//     EXPECT_EQ(0, cpu.s.n);
//     EXPECT_EQ(1, cpu.s.z);

//     ram[2] = LDA_IMM;
//     ram[3] = 64;
//     cpu.execute(ram);

//     EXPECT_EQ(64, cpu.a);
//     EXPECT_EQ(0, cpu.s.n);
//     EXPECT_EQ(0, cpu.s.z);

//     ram[3] = LDA_IMM;
//     ram[4] = 128;
//     cpu.execute(ram);

//     EXPECT_EQ(128, cpu.a);
//     EXPECT_EQ(1, cpu.s.n);
//     EXPECT_EQ(0, cpu.s.z);
// }
