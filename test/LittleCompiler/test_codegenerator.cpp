//
// Created by kaspersk on 10/17/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LittleVirtualMachine/LittleCompiler/CodeGenerator.h>
#include <iostream>

using namespace lvm::compiler;

class CompilerCodeGenTest : public ::testing::Test {

};

TEST_F (CompilerCodeGenTest, GeneratorInstruction)
{
    CodeGenerator uut;

    uut.GenerateInstruction("ADD");
    uut.GenerateInstruction("OUT");

    std::streambuf* sbp = new std::stringbuf();
    std::stringbuf* ssb = (std::stringbuf*)sbp;
    std::ostream os(sbp);

    uut.OutputCode(os);

    EXPECT_EQ(ssb->sbumpc(), 2);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 1);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
}

TEST_F (CompilerCodeGenTest, LabelDefRef)
{
    CodeGenerator uut;
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("MUL");
    uut.GenerateInstruction("OUT");
    uut.GenerateLabelDef("LABEL1");
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("SUB");
    uut.GenerateInstruction("OUT");
    uut.GenerateLabelRef("LABEL1");
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("SUB");
    uut.GenerateInstruction("OUT");
    uut.GenerateLabelRef("LABEL1");
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("IN");
    uut.GenerateInstruction("ADD");
    uut.GenerateInstruction("OUT");

    std::streambuf* sbp = new std::stringbuf();
    std::stringbuf* ssb = (std::stringbuf*)sbp;
    std::ostream os(sbp);

    uut.OutputCode(os);

    ssb->pubseekpos(32);
    EXPECT_EQ(ssb->sbumpc(), 4);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    ssb->pubseekpos(52);
    EXPECT_EQ(ssb->sbumpc(), 4);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
}

TEST_F (CompilerCodeGenTest, GenerateString)
{
    CodeGenerator uut;
    uut.GenerateStringValue("Hello");

    std::streambuf* sbp = new std::stringbuf();
    std::stringbuf* ssb = (std::stringbuf*)sbp;
    std::ostream os(sbp);

    uut.OutputCode(os);

    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'o');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'l');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'l');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'e');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'H');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

}

TEST_F (CompilerCodeGenTest, GenerateStringEscapedChar)
{
    CodeGenerator uut;
    uut.GenerateStringValue("Hello\n");

    std::streambuf* sbp = new std::stringbuf();
    std::stringbuf* ssb = (std::stringbuf*)sbp;
    std::ostream os(sbp);

    uut.OutputCode(os);

    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), '\n');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'o');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'l');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'l');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'e');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'H');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

}


TEST_F (CompilerCodeGenTest, GenerateValue)
{
    CodeGenerator uut;
    uut.GenerateValue("100");

    std::streambuf* sbp = new std::stringbuf();
    std::stringbuf* ssb = (std::stringbuf*)sbp;
    std::ostream os(sbp);

    uut.OutputCode(os);

    EXPECT_EQ(ssb->sbumpc(), 100);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
}

TEST_F (CompilerCodeGenTest, GeneratePrint)
{
    CodeGenerator uut;
    uut.GenerateStringValue("Hello");
    uut.GeneratePrint();

    std::streambuf* sbp = new std::stringbuf();
    std::stringbuf* ssb = (std::stringbuf*)sbp;
    std::ostream os(sbp);

    uut.OutputCode(os);

    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'o');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'l');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'l');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'e');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 'H');
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 17);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 11);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 0x17);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 0x01);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 0x06);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 0x16);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);

    EXPECT_EQ(ssb->sbumpc(), 0x10);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
    EXPECT_EQ(ssb->sbumpc(), 0);
}
