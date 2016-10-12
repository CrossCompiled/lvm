#include <map>
#include <chrono>
#include <thread>
#include <LittleVirtualMachine/LittleShared/opcodes.h>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>
using namespace lvm;

typedef void (*myfunc)(interpreter::vmsystem<int, int>&);

int main(){
    auto opmap = std::map<int, myfunc >();

    opmap.insert( std::pair<int, myfunc >(shared::Add::code, interpreter::Add<interpreter::vmsystem<int, int> >::execute ));
    opmap.insert( std::pair<int, myfunc >(shared::Out::code, interpreter::Out<interpreter::vmsystem<int, int> >::execute ));
    opmap.insert( std::pair<int, myfunc >(shared::Push::code, interpreter::Push<interpreter::vmsystem<int, int> >::execute ));
    opmap.insert( std::pair<int, myfunc >(shared::Halt::code, interpreter::Halt<interpreter::vmsystem<int, int> >::execute ));

    auto system = interpreter::vmsystem<int, int>();

    system.program.insert(std::pair<int, int>(0x0000, shared::Push::code));
    system.program.insert(std::pair<int, int>(0x0001, '0'));
    system.program.insert(std::pair<int, int>(0x0002, shared::Push::code));
    system.program.insert(std::pair<int, int>(0x0003, 4));
    system.program.insert(std::pair<int, int>(0x0004, shared::Add::code));
    system.program.insert(std::pair<int, int>(0x0005, shared::Out::code));
    system.program.insert(std::pair<int, int>(0x0006, shared::Halt::code));

    system.program_ptr = 0x000;

    system.running = true;
    while(system.running) {
        opmap[system.program[system.program_ptr]](system);
        ++system.program_ptr;
    }

    return 0;
}