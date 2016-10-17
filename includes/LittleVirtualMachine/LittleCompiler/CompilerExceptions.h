//
// Created by kaspersk on 10/17/16.
//

#ifndef LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
#define LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H

#include <stdexcept>

class CompilerExceptions : std::exception{

public:
    CompilerExceptions(const string& what_error){

    }
};

class WrongOpcode : CompilerExceptions{

};

class MissingLabel : CompilerExceptions{

};


#endif //LITTLEVIRTUALMACHINE_COMPILEREXCEPTIONS_H
