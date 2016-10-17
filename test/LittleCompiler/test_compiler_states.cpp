#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "LittleVirtualMachine/LittleCompiler/compiler_states.h"

class CompilerStateTest : public ::testing::Test {};

TEST_F(CompilerStateTest, IfGivenStreamIsNotOkay) {
	lvm::compiler::Compiler uut;
	uut.initiate();

	std::stringstream test;
	test.setstate(std::ios_base::badbit);

	uut.process_event(lvm::compiler::LoadFileEvent(&test));

	ASSERT_THAT(uut.instructions.empty(), true);
}

TEST_F(CompilerStateTest, ValidStream) {
	lvm::compiler::Compiler uut;
	uut.initiate();

	std::stringstream test;
	test << "10" << std::endl << "10" << std::endl << "ADD";

	uut.process_event(lvm::compiler::LoadFileEvent(&test));

	uut.process_event(lvm::compiler::ProcessCharEvent());
	uut.process_event(lvm::compiler::ProcessCharEvent());

	ASSERT_THAT(uut.instructions[0], "1");

	uut.process_event(lvm::compiler::ProcessCharEvent());

	ASSERT_THAT(uut.instructions[0], "10");

	uut.process_event(lvm::compiler::ProcessCharEvent());
	uut.process_event(lvm::compiler::ProcessCharEvent());
	uut.process_event(lvm::compiler::ProcessCharEvent());

	ASSERT_THAT(uut.instructions[1], "1");

	uut.process_event(lvm::compiler::ProcessCharEvent());

	ASSERT_THAT(uut.instructions[1], "10");

	uut.process_event(lvm::compiler::ProcessCharEvent());
	uut.process_event(lvm::compiler::ProcessCharEvent());
	uut.process_event(lvm::compiler::ProcessCharEvent());
	uut.process_event(lvm::compiler::ProcessCharEvent());
	uut.process_event(lvm::compiler::ProcessCharEvent());

	ASSERT_THAT(uut.instructions[2], "ADD");
}