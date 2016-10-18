#ifndef LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
#define LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H

#include <stdexcept>
#include <string>

<<<<<<< b9ceccc9984cd9f09f561c905a28ba5dfe82fc56
namespace lvm{
    namespace compiler {

        class CompilerExceptions : std::exception{

        public:
            explicit CompilerExceptions(const std::string& what_error) : whats_wrong(what_error){
            }

            virtual const char* what() const throw(){
                return whats_wrong.c_str();
            }

        protected:
            std::string whats_wrong;
        };

        class WrongOpcode : CompilerExceptions{
        public:
            WrongOpcode(const std::string& what_error) : CompilerExceptions(what_error){}
        };

        class MissingLabel : CompilerExceptions{
        public:
            MissingLabel(const std::string& what_error) : CompilerExceptions(what_error){}

        };

        class DuplicateLabel : CompilerExceptions{
        public:
            DuplicateLabel(const std::string& what_error) : CompilerExceptions(what_error){}

        };
    }
}
=======
namespace lvm {
	namespace compiler {
		struct CompilerException : std::runtime_error {
			CompilerException(const std::string& what_error) {}
		};

		struct WrongOpcode : CompilerException {};

		struct MissingLabel : CompilerException {};
>>>>>>> Refactoring

		struct ParsingErrorException : CompilerException {};
	}
}

#endif //LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H