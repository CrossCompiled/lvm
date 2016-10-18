#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "LittleVirtualMachine/LittleCompiler/compiler_states.h"

struct MockCodeGenerator : public lvm::compiler::ICodeGenerator {
	MOCK_METHOD0(GeneratePrint, void());
	MOCK_METHOD1(GenerateInstruction, void(const std::string&));
	MOCK_METHOD1(GenerateLabelDef, void(const std::string&));
	MOCK_METHOD1(GenerateLabelRef, void(const std::string&));
	MOCK_METHOD1(GenerateStringValue, void(const std::string&));
	MOCK_METHOD1(GenerateValue, void(const std::string&));
};

class CompilerStateTest : public ::testing::Test {};

TEST_F(CompilerStateTest, IfGivenStreamIsNotOkay) {
	MockCodeGenerator mock;
	lvm::compiler::Compiler compiler(&mock);
	compiler.initiate();

	std::stringstream stream;
	stream.setstate(std::ios_base::failbit);
	compiler.process_event(lvm::compiler::LoadFileEvent(&stream));

	// Assertion????
}

TEST_F(CompilerStateTest, InstructionStream) {
	MockCodeGenerator mock;
	EXPECT_CALL(mock, GenerateValue("10")).Times(testing::Exactly(2));
	EXPECT_CALL(mock, GenerateInstruction("ADD")).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(&mock);
	compiler.initiate();

	std::stringstream stream;
	stream << "10" << std::endl << "10" << std::endl << "ADD" << std::endl;

	compiler.process_event(lvm::compiler::LoadFileEvent(&stream));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}

TEST_F(CompilerStateTest, InstructionStreamWithComment) {
	MockCodeGenerator mock;
	EXPECT_CALL(mock, GenerateValue("5")).Times(testing::Exactly(1));
	EXPECT_CALL(mock, GenerateValue("20")).Times(testing::Exactly(1));
	EXPECT_CALL(mock, GenerateInstruction("ADD")).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(&mock);
	compiler.initiate();

	std::stringstream stream;
	stream << "5" << std::endl << "20" << std::endl << "ADD ; Trying to add 5 + 20, should work." << std::endl;

	compiler.process_event(lvm::compiler::LoadFileEvent(&stream));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}

TEST_F(CompilerStateTest, StringStream) {
	MockCodeGenerator mock;
	EXPECT_CALL(mock, GenerateStringValue("Hello, world\n")).Times(testing::Exactly(1));
	EXPECT_CALL(mock, GeneratePrint()).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(&mock);
	compiler.initiate();

	std::stringstream stream;
	stream << "\"Hello, world\n\"" << std::endl << "PRiNT" << std::endl;

	compiler.process_event(lvm::compiler::LoadFileEvent(&stream));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}

TEST_F(CompilerStateTest, LabelRefDefinition) {
	MockCodeGenerator mock;
	EXPECT_CALL(mock, GenerateLabelDef("HelloWorld")).Times(testing::Exactly(1));
	EXPECT_CALL(mock, GenerateLabelRef("HelloWorld")).Times(testing::Exactly(1));
	EXPECT_CALL(mock, GenerateStringValue("Hello, world!")).Times(testing::Exactly(1));
	EXPECT_CALL(mock, GeneratePrint()).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(&mock);
	compiler.initiate();

	std::stringstream stream;
	stream << ":HelloWorld" << std::endl << "\"Hello, world!\"" << std::endl  << "PRINT" << std::endl << std::endl << "@HelloWorld" << std::endl;

	compiler.process_event(lvm::compiler::LoadFileEvent(&stream));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}