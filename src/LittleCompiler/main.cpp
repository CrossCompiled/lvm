#include <iostream>
#include <sstream>
#include "LittleVirtualMachine/LittleCompiler/Compiler.h"
#include "LittleVirtualMachine/LittleCompiler/CodeGenerator.h"

namespace compiler = lvm::compiler;

int main() {
	try {
		compiler::CodeGenerator generator;
		compiler::Compiler compiler(&generator);
		compiler.initiate();

		std::stringstream input;
		compiler.process_event(lvm::compiler::LoadFileEvent(&input));

		while (!input.eof())
			compiler.process_event(lvm::compiler::ProcessCharEvent());

		std::stringstream output;
		compiler.process_event(lvm::compiler::WriteByteCodeEvent(&output));
	}
	catch (std::runtime_error const& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unhandled exception!" << std::endl;
	}

	return 0;
}