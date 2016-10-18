#ifndef LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
#define LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace lvm{
    namespace compiler {

        struct CompilerException : std::runtime_error {
            explicit CompilerException(const std::string& what_error) : runtime_error(what_error) {}
        };

        struct WrongOpcode : public CompilerException{
            WrongOpcode(const std::string& what_error) : CompilerException(what_error){}
        };

        struct MissingLabel : public CompilerException{
        public:
            MissingLabel(const std::string& what_error) : CompilerException(what_error){}

        };

        struct DuplicateLabel : public CompilerException{
            DuplicateLabel(const std::string& what_error) : CompilerException(what_error){}
        };
    }
}

#endif //LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
