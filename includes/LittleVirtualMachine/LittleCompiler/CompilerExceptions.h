#ifndef LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
#define LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H

#include <stdexcept>
#include <string>

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

#endif //LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
