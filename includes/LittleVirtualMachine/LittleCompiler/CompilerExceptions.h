#ifndef LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
#define LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace lvm{
    namespace compiler {
			struct CompilerException : std::runtime_error {
				explicit CompilerException(const std::string& message) : runtime_error(message) {}
			};

			struct WrongOpcode : public CompilerException {
				WrongOpcode(const std::string& message) : CompilerException(message) {}
			};

			struct MissingLabel : public CompilerException {
				MissingLabel(const std::string& message) : CompilerException(message) {}
			};

			struct DuplicateLabel : public CompilerException {
				DuplicateLabel(const std::string& message) : CompilerException(message) {}
			};

			struct ParsingError : public CompilerException {
				ParsingError(const std::string& message) : CompilerException(message) {}
			};

			struct StreamError : public CompilerException {
				StreamError(const std::string& message) : CompilerException(message) {}
			};
		}
}

#endif //LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
