// read a file into memory


#include <LittleVirtualMachine/LittleInterpreter/vmsystem.h>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <list>
#include <array>

namespace {

#ifdef STATIC_STACK
    using STACK = std::array<int32_t,10000>;
#else
    using STACK = std::stack<int32_t>;
#endif

#ifdef STATIC_PROGRAM
    using PROGRAM = std::array<int32_t,10000>;
#else
    using PROGRAM = std::vector<int32_t>;
#endif

#ifdef STATIC_MEMORY
    using MEMORY = std::array<int32_t,10000>;
#else
    using MEMORY = std::vector<int32_t>;
#endif



    template<typename ...A>
    using OPCODEMAP = lvm::interpreter::vector<A...>;
    using OPCODESET = lvm::interpreter::oc_11;
    using CIN = std::istream;
    using COUT = std::ostream;
    using vmsystem    = lvm::interpreter::vmsystem<lvm::interpreter::vector, OPCODESET, STACK, MEMORY, PROGRAM, CIN, COUT>;


}



int main(int argn, char* argc[]) {
#ifdef STATIC_STACK
    std::cout << "Static Stack" << std::endl;
#else
    std::cout << "Dynamic Stack" << std::endl;
#endif

#ifdef STATIC_PROGRAM
    std::cout << "Static Program" << std::endl;
#else
    std::cout << "Dynamic Program" << std::endl;
#endif

#ifdef STATIC_MEMORY
    std::cout << "Static Memory" << std::endl;
#else
    std::cout << "Dynamic Memory" << std::endl;
#endif

    auto vm = vmsystem(std::cin, std::cout);

    std::ifstream binary(argc[1]);
    if (binary) {
        vm.init(binary);
        binary.close();
    }

    vm.run();

    return vm.exit_code();

}