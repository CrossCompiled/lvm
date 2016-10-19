#include <iostream>
#include <sstream>
#include <fstream>
#include "LittleVirtualMachine/LittleCompiler/Compiler.h"
#include "LittleVirtualMachine/LittleCompiler/CodeGenerator.h"

namespace compiler = lvm::compiler;

int main(int argc, char** argv) {
	try {
		compiler::CodeGenerator generator;
		compiler::Compiler compiler(&generator);
		compiler.initiate();

		std::ifstream input(argv[1]);
		std::cout << argv[0] << std::endl;
		std::cout << argv[1] << std::endl;
		compiler.process_event(lvm::compiler::LoadFileEvent(&input));

		while (!input.eof())
			compiler.process_event(lvm::compiler::ProcessCharEvent());

		compiler.process_event(lvm::compiler::ProcessCharEvent());
		compiler.process_event(lvm::compiler::ProcessCharEvent());

		std::ofstream output("TestOut", std::ios::binary);

		compiler.process_event(lvm::compiler::WriteByteCodeEvent(&output));
		compiler.process_event(lvm::compiler::WriteByteCodeEvent(&output));

		output.close();
	}
	catch (std::runtime_error const& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception const& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unhandled exception!" << std::endl;
	}

	return 0;
}