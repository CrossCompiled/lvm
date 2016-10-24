#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "LittleVirtualMachine/LittleCompiler/Compiler.h"
#include "LittleVirtualMachine/LittleCompiler/CodeGenerator.h"

namespace compiler = lvm::compiler;

int main(int argc, char** argv) {
	if (argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " {source file} {output file}" << std::endl;
		return 1;
	};

	std::string inputPath(argv[1]);
	std::string outputPath(argv[2]);

	try {
		compiler::CodeGenerator codeGenerator;
		compiler::FileHelper fileHelper;

		compiler::Compiler compiler(codeGenerator, fileHelper);
		compiler.initiate();

		compiler.process_event(lvm::compiler::LoadFileEvent(inputPath));

		while (!compiler.get_input().get_ro_stream().eof())
			compiler.process_event(lvm::compiler::ProcessCharEvent());

		compiler.process_event(lvm::compiler::ProcessCharEvent());
		compiler.process_event(lvm::compiler::ProcessCharEvent());

		std::ofstream output(outputPath, std::ios_base::binary);

		compiler.process_event(lvm::compiler::WriteByteCodeEvent(output));
		compiler.process_event(lvm::compiler::WriteByteCodeEvent(output));

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