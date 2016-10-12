//
// Created by kaspersk on 9/25/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LittleVirtualMachine/LittleShared/TypeList.h>
#include <LittleVirtualMachine/LittleShared/SupportedTypes.h>
#include <LittleVirtualMachine/LittleShared/opcodes.h>


using namespace lvm::shared;

//region CompareFunction

class TypeListTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F (TypeListTest, OneType)
{
    typedef MakeTypeList<int> IntTL;

    auto size = sizeof(int);
    auto tlSize = sizeof(IntTL::List::First);

    EXPECT_EQ(size, tlSize);
}

TEST_F (TypeListTest, MixTypeList)
{
    typedef MakeTypeList<char , int , long , short> MixTL;

    auto size = sizeof(int);
    auto tlSize = sizeof(MixTL::List::Left::First);

    EXPECT_EQ(size, tlSize);
}

TEST_F (TypeListTest, MixTypeListShort)
{
    typedef MakeTypeList<char , int , long , short> MixTL;

    auto size = sizeof(short);
    auto tlSize = sizeof(MixTL::List::Left::Left::Left::First);

    EXPECT_EQ(size, tlSize);
}

TEST_F (TypeListTest, IsSameTrue)
{
    auto isSame = IsSame<char,char>::value;
    EXPECT_EQ(true, isSame);
}

TEST_F (TypeListTest, IsSameNotTrue)
{
    auto isSame = IsSame<char,int>::value;
    EXPECT_EQ(false, isSame);
}

TEST_F (TypeListTest, Contains)
{
    typedef MakeTypeList<char , int , long , short> MixTL;

    auto IGotIt = Contains<MixTL::List, short>::value;


    EXPECT_EQ(true, IGotIt);
}

TEST_F (TypeListTest, AtIndex)
{
    typedef MakeTypeList<char , int , long , short> MixTL;

    auto TypeChecker = IsSame<typename AtIndex<MixTL::List, 1>::type, int>::value;

    EXPECT_EQ(true, TypeChecker);
}

TEST_F (TypeListTest, PrintIt)
{
    typedef MakeTypeList<char , int , long , short> MixTL;

    auto printed = PrintIt<MixTL::List>::value();

    EXPECT_EQ("c i l s ", printed);
}

TEST_F (TypeListTest, Remove)
{
    typedef MakeTypeList<char , int , long , short> MixTL;

    auto IGotIt = Contains<Remove<MixTL::List, short>::type, short>::value;

    EXPECT_EQ(false, IGotIt);
}

TEST_F (TypeListTest, RemovePartDuex)
{
    typedef MakeTypeList<char , int , long , short> MixTL;

    auto IGotIt = Contains<Remove<MixTL::List, long>::type, short>::value;

    EXPECT_EQ(true, IGotIt);
}

TEST_F (TypeListTest, Opcodemap)
{
    std::map<std::string, int> codes;
    MakeOpcodeMap<SupportedOpcodes>::run(codes);

    EXPECT_EQ(codes["IN"], 0x00);
    EXPECT_EQ(codes["OUT"], 0x01);
    EXPECT_EQ(codes["ADD"], 0x02);
}