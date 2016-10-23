// read a file into memory
#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <fstream>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>



namespace {

#ifdef STATIC_STACK
    using vmsystem    = lvm::interpreter::vmsystem<int32_t, std::array<int32_t,10000>, std::array<int32_t, 10000>, std::array<int32_t, 10000>>;
#else
    using vmsystem    = lvm::interpreter::vmsystem<int32_t, std::stack<int32_t>, std::array<int32_t, 100>, std::array<int32_t, 100>>;
#endif
    using oc_11       = lvm::interpreter::opcodes::oc_11<vmsystem>;
    using oc_11_array = lvm::interpreter::oc_array<oc_11>;


}

int main(int argn, char* argc[]) {

    auto vm = vmsystem();

    vm.load_program(argc[1]);

    while(vm.running) {
        oc_11_array::data[vm.program[vm.program_ptr]](vm);
    }

    return lvm::interpreter::exit_code(vm);

}