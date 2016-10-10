#include "LittleVirtualMachine/LittleCompiler/compiler_states.h"

using namespace lvm;

int main(){
    compiler::Compiler compiler;
		compiler.initiate();

		compiler.process_event(compiler::LoadFileEvent(""));

		while (!compiler.fileStream.eof()) {
			compiler.process_event(compiler::ProcessCharEvent());
		}

		compiler.process_event(compiler::WriteByteCodeEvent());
}