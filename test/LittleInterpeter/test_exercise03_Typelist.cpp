//
// Created by kaspersk on 9/25/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LittleVirtualMachine/LittleInterpeter/TypeList.h>

using namespace exercise03;

//region CompareFunction

class TypeListTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F (TypeListTest, OneType)
{
    typedef TYPELIST_1(int) IntTL;

    auto size = sizeof(int);
    auto tlSize = sizeof(IntTL::First);

    EXPECT_EQ(size, tlSize);
}

TEST_F (TypeListTest, MixTypeList)
{
    typedef TYPELIST_4(char , int , long , short ) MixTL;

    auto size = sizeof(int);
    auto tlSize = sizeof(MixTL::Left::First);

    EXPECT_EQ(size, tlSize);
}

TEST_F (TypeListTest, MixTypeListShort)
{
    typedef TYPELIST_4(char , int , long , short ) MixTL;

    auto size = sizeof(short);
    auto tlSize = sizeof(MixTL::Left::Left::Left::First);

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
    typedef TYPELIST_4(char , int , long , short ) MixTL;

    auto IGotIt = Contains<MixTL, short>::value;


    EXPECT_EQ(true, IGotIt);
}

TEST_F (TypeListTest, AtIndex)
{
    typedef TYPELIST_4(char , int , long , short ) MixTL;

    auto TypeChecker = IsSame<typename AtIndex<MixTL, 1>::type, int>::value;

    EXPECT_EQ(true, TypeChecker);
}

TEST_F (TypeListTest, PrintIt)
{
    typedef TYPELIST_4(char , int , long , short ) MixTL;

    auto printed = PrintIt<MixTL>::value();

    EXPECT_EQ("c i l s ", printed);
}

TEST_F (TypeListTest, Remove)
{
    typedef TYPELIST_4(char , int , long , short ) MixTL;

    auto IGotIt = Contains<Remove<MixTL, short>::type, short>::value;

    EXPECT_EQ(false, IGotIt);
}

TEST_F (TypeListTest, RemovePartDuex)
{
    typedef TYPELIST_4(char , int , long , short ) MixTL;

    auto IGotIt = Contains<Remove<MixTL, long>::type, short>::value;

    EXPECT_EQ(true, IGotIt);
}