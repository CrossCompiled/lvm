// read a file into memory
#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <fstream>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>

#include <type_traits>

namespace {

#ifdef STATIC_STACK
    using STACK = std::array<int32_t,10000>;
#else
    using STACK = std::stack<int32_t>;
#endif
    using MEMORY = std::array<int32_t, 10000>;
    using PROGRAM = std::array<int32_t, 10000>;
    using OPCODESET = lvm::interpreter::oc_11;
    using vmsystem    = lvm::interpreter::vmsystem<OPCODESET, STACK, MEMORY, PROGRAM>;
    //using oc_11       = lvm::interpreter::opcodes::oc_11<vmsystem>;
    //using oc_11_array = lvm::interpreter::oc_array<oc_11>;


}



int main(int argn, char* argc[]) {
#ifdef STATIC_STACK
//    std::cout << "Static Build" << '\n';
#else
//    std::cout << "Dynamic Build" << '\n';
#endif

    //std::cout << lvm::interpreter::need_stack_ptr<std::array<int, 10>>::value << '\n';

    auto vm = vmsystem();

    vm.init(argc[1]);
    return vm.run();

}