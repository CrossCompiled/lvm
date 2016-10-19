#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "LittleVirtualMachine/LittleCompiler/Compiler.h"
#include "LittleVirtualMachine/LittleCompiler/ICodeGenerator.h"
#include "LittleVirtualMachine/LittleCompiler/IStreamHelper.h"

struct MockCodeGenerator : public lvm::compiler::ICodeGenerator {
	MOCK_METHOD0(GeneratePrint, void());
	MOCK_METHOD1(GenerateInstruction, void(const std::string&));
	MOCK_METHOD1(GenerateLabelDef, void(const std::string&));
	MOCK_METHOD1(GenerateLabelRef, void(const std::string&));
	MOCK_METHOD1(GenerateStringValue, void(const std::string&));
	MOCK_METHOD1(GenerateValue, void(const std::string&));
	MOCK_METHOD1(OutputCode, void(std::ostream& out));
};

struct MockStreamHelper : public lvm::compiler::IStreamHelper {
	MockStreamHelper(std::iostream& stream) : stream_(stream) {}

	void open(std::string& path) {}

	std::istream& get_ro_stream() { return stream_; }
	std::ostream& get_rw_stream() { return stream_; }

private:
	std::iostream& stream_;
};

class CompilerStateTest : public ::testing::Test {};

TEST_F(CompilerStateTest, IfGivenStreamIsNotOkay) {
	std::stringstream stream;
	stream.setstate(std::ios_base::failbit);

	MockCodeGenerator mockCodeGenerator;
	MockStreamHelper mockStreamHelper(stream);
	lvm::compiler::Compiler compiler(mockCodeGenerator, mockStreamHelper);
	compiler.initiate();

	std::string hello = "Hello";
	compiler.process_event(lvm::compiler::LoadFileEvent(hello));

	// Assertion????
}

TEST_F(CompilerStateTest, InstructionStream) {
	std::stringstream stream;
	stream << "10" << std::endl << "10" << std::endl << "ADD" << std::endl;

	MockCodeGenerator mockCodeGenerator;
	MockStreamHelper mockStreamHelper(stream);

	EXPECT_CALL(mockCodeGenerator, GenerateValue("10")).Times(testing::Exactly(2));
	EXPECT_CALL(mockCodeGenerator, GenerateInstruction("ADD")).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(mockCodeGenerator,mockStreamHelper);
	compiler.initiate();

	std::string hello = "Hello";
	compiler.process_event(lvm::compiler::LoadFileEvent(hello));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}

TEST_F(CompilerStateTest, InstructionStreamWithComment) {
	std::stringstream stream;
	stream << "5" << std::endl << "20" << std::endl << "ADD ; Trying to add 5 + 20, should work." << std::endl;

	MockCodeGenerator mockCodeGenerator;
	MockStreamHelper mockStreamHelper(stream);

	EXPECT_CALL(mockCodeGenerator, GenerateValue("5")).Times(testing::Exactly(1));
	EXPECT_CALL(mockCodeGenerator, GenerateValue("20")).Times(testing::Exactly(1));
	EXPECT_CALL(mockCodeGenerator, GenerateInstruction("ADD")).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(mockCodeGenerator, mockStreamHelper);
	compiler.initiate();

	std::string hello = "Hello";
	compiler.process_event(lvm::compiler::LoadFileEvent(hello));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}

TEST_F(CompilerStateTest, StringStream) {
	std::stringstream stream;
	stream << "\"Hello, world\n\"" << std::endl << "PRiNT" << std::endl;

	MockCodeGenerator mockCodeGenerator;
	MockStreamHelper mockStreamHelper(stream);

	EXPECT_CALL(mockCodeGenerator, GenerateStringValue("Hello, world\n")).Times(testing::Exactly(1));
	EXPECT_CALL(mockCodeGenerator, GeneratePrint()).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(mockCodeGenerator, mockStreamHelper);
	compiler.initiate();

	std::string hello = "Hello";
	compiler.process_event(lvm::compiler::LoadFileEvent(hello));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}

TEST_F(CompilerStateTest, LabelRefDefinition) {
	std::stringstream stream;
	stream << ":HelloWorld" << std::endl << "\"Hello, world!\"" << std::endl  << "PRINT" << std::endl << std::endl << "@HelloWorld" << std::endl;

	MockCodeGenerator mockCodeGenerator;
	MockStreamHelper mockStreamHelper(stream);

	EXPECT_CALL(mockCodeGenerator, GenerateLabelDef("HelloWorld")).Times(testing::Exactly(1));
	EXPECT_CALL(mockCodeGenerator, GenerateLabelRef("HelloWorld")).Times(testing::Exactly(1));
	EXPECT_CALL(mockCodeGenerator, GenerateStringValue("Hello, world!")).Times(testing::Exactly(1));
	EXPECT_CALL(mockCodeGenerator, GeneratePrint()).Times(testing::Exactly(1));

	lvm::compiler::Compiler compiler(mockCodeGenerator, mockStreamHelper);
	compiler.initiate();

	std::string hello = "Hello";
	compiler.process_event(lvm::compiler::LoadFileEvent(hello));
	while (!stream.eof())
		compiler.process_event(lvm::compiler::ProcessCharEvent());
}