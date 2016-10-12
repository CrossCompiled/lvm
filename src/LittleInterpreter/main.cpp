#include <map>
#include <chrono>
#include <thread>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>
using namespace lvm;

typedef void (*myfunc)(interpreter::vmsystem<int, int>&);

int main(){
    auto opmap = std::map<int, myfunc >();

    opmap.insert( std::pair<int, myfunc >(interpreter::Add<interpreter::vmsystem<int, int> >::code, interpreter::Add<interpreter::vmsystem<int, int> >::operator() ));
    opmap.insert( std::pair<int, interpreter::BaseOpcode<interpreter::vmsystem<int, int> > * >((int)shared::Out::code, new interpreter::Out<interpreter::vmsystem<int, int> >()) );
    opmap.insert( std::pair<int, interpreter::BaseOpcode<interpreter::vmsystem<int, int> > * >((int)shared::Push::code, new interpreter::Push<interpreter::vmsystem<int, int> >()) );
    opmap.insert( std::pair<int, interpreter::BaseOpcode<interpreter::vmsystem<int, int> > * >((int)shared::Halt::code, new interpreter::Halt<interpreter::vmsystem<int, int> >()) );


    auto system = interpreter::vmsystem<int, int>();

    auto u = interpreter::Push<int>::code;
    std::cout << typeid(interpreter::Push<int>::code).name() << std::endl;
    system.program.insert(std::pair<int, int>(0x0000, u));
    system.program.insert(std::pair<int, int>(0x0001, '0'));
    system.program.insert(std::pair<int, int>(0x0002, (int)interpreter::Push<int>::code));
    system.program.insert(std::pair<int, int>(0x0003, 4));
    system.program.insert(std::pair<int, int>(0x0004, (int)interpreter::Add<int>::code));
    system.program.insert(std::pair<int, int>(0x0005, (int)interpreter::Out<int>::code));
    system.program.insert(std::pair<int, int>(0x0006, (int)interpreter::Halt<int>::code));

    system.program_ptr = 0x000;

    system.running = true;
    while(system.running) {
        //std::cout << system.program_ptr << ": " << system.program[system.program_ptr] << std::endl;

        opmap[system.program[system.program_ptr]]->operator()(system);
        ++system.program_ptr;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}