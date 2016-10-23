// read a file into memory
#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <fstream>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>

#include <type_traits>

namespace {

#ifndef STATIC_STACK
    using vmsystem    = lvm::interpreter::vmsystem<lvm::interpreter::oc_11, std::array<int32_t,10000>, std::array<int32_t, 10000>, std::array<int32_t, 10000>>;
#else
    using vmsystem    = lvm::interpreter::vmsystem<lvm::interpreter::oc_11, std::stack<int32_t>, std::array<int32_t, 10000>, std::array<int32_t, 10000>>;;
#endif
    //using oc_11       = lvm::interpreter::opcodes::oc_11<vmsystem>;
    //using oc_11_array = lvm::interpreter::oc_array<oc_11>;


}



int main(int argn, char* argc[]) {

    auto vm = vmsystem();

    vm.init(argc[1]);
    return vm.run();

}