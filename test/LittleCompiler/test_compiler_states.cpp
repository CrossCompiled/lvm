#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "LittleVirtualMachine/LittleCompiler/compiler_states.h"

class MockCodeGenerator : public lvm::compiler::ICodeGenerator {
public:
	MOCK_METHOD0(GenerateInstruction, void());
	MOCK_METHOD0(GenerateLabelDef, void());
	MOCK_METHOD0(GenerateLabelRef, void());
	MOCK_METHOD0(GeneratePrint, void());
	MOCK_METHOD0(GenerateStringValue, void());
	MOCK_METHOD0(GenerateValue, void());
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
	lvm::compiler::Compiler compiler(&mock);
	compiler.initiate();

	std::stringstream stream;
	stream << "10" << std::endl << "10" << std::endl << "ADD" << std::endl;

	compiler.process_event(lvm::compiler::LoadFileEvent(&stream));

	EXPECT_CALL(mock, GenerateInstruction()).Times(testing::AtLeast(1));
}

TEST_F(CompilerStateTest, StringStream) {
	MockCodeGenerator mock;
	lvm::compiler::Compiler compiler(&mock);
	compiler.initiate();

	std::stringstream stream;
	stream << "\"Hello, world\"" << std::endl << "PRINT" << std::endl;

	compiler.process_event(lvm::compiler::LoadFileEvent(&stream));
}