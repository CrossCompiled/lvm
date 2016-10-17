#include <iostream>
#include <sstream>
#include "LittleVirtualMachine/LittleCompiler/compiler_states.h"
#include "LittleVirtualMachine/LittleCompiler/CodeGenerator.h"

namespace compiler = lvm::compiler;

int main() {
	try {
		compiler::CodeGenerator generator;
		compiler::Compiler compiler(&generator);
		compiler.initiate();

		std::stringstream stream;
		compiler.process_event(lvm::compiler::LoadFileEvent(&stream));
	}
	catch (std::runtime_error const& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unhandled exception!" << std::endl;
	}

	return 0;
}