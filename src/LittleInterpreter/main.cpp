// read a file into memory


#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <array>
#include <type_traits>

namespace {

#ifndef STATIC_STACK
    using STACK = std::array<int32_t,10000>;
#else
    using STACK = std::stack<int32_t>;
#endif
    using MEMORY = std::array<int32_t, 10000>;
    using PROGRAM = std::vector<int32_t>;
    using OPCODESET = lvm::interpreter::oc_11;
    using CIN = std::istream;
    using COUT = std::ostream;
    using vmsystem    = lvm::interpreter::vmsystem<OPCODESET, STACK, MEMORY, PROGRAM, CIN, COUT>;


}



int main(int argn, char* argc[]) {
#ifndef STATIC_STACK
//    std::cout << "Static Build" << '\n';
#else
//    std::cout << "Dynamic Build" << '\n';
#endif

    //std::cout << lvm::interpreter::need_stack_ptr<std::array<int, 10>>::value << '\n';

    auto vm = vmsystem(std::cin, std::cout);

    std::ifstream binary(argc[1]);
    if (binary) {
        vm.init(binary);
        binary.close();
    }

    vm.run();

    return vm.exit_code();

}