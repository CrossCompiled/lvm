//
// Created by kaspersk on 9/25/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LittleVirtualMachine/LittleCompiler/Binary.h>



using namespace exercise02;

class Dummy {
public:
    Dummy(size_t n) : n(n) {}
    virtual ~Dummy() {}
    size_t n;
};

//region CompareFunction

class BinaryTesting : public ::testing::Test {
protected:
    virtual void SetUp() {
        this->value = new Dummy(this->ORG_VALUE);
    }

    Dummy* value;
    const size_t ORG_VALUE = 200;
};

TEST_F (BinaryTesting, BinaryNumber)
{
    auto binary = Binary<1101>::value;
    EXPECT_EQ(13, binary);
}

//TEST_F (BinaryTesting, NotBinaryNumber)
//{
//    auto binary = Binary<1103>::value;
//    EXPECT_EQ(13, binary);
//}