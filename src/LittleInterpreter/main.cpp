// read a file into memory
#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <fstream>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>



namespace {
    using vmsystem    = lvm::interpreter::vmsystem<int32_t, std::array<int32_t,10000>, std::array<int32_t, 10000>, std::array<int32_t, 10000>>;
//    using vmsystem    = lvm::interpreter::vmsystem<int32_t, std::stack<int32_t>, std::array<int32_t, 100>, std::array<int32_t, 100>>;
    using oc_11       = lvm::interpreter::opcodes::oc_11<vmsystem>;
    using oc_11_array = lvm::interpreter::oc_array<oc_11>;

    void load_program(vmsystem& vm, const char* filename) {
        std::ifstream is (filename, std::ifstream::binary);
        if (is) {
            is.seekg (0, is.end);
            int length = (int32_t)is.tellg() / 4;
            is.seekg (0, is.beg);
            is.read(reinterpret_cast<char *>(vm.program.data()), sizeof(int32_t)*length);
            is.close();
        }
    };
}

int main(int argn, char* argc[]) {

    auto vm = vmsystem();

    std::cout << argc[1] << std::endl;
    load_program(vm, argc[1]);

    while(vm.running) {
        oc_11_array::data[vm.program[vm.program_ptr]](vm);
    }

    return lvm::interpreter::exit_code(vm);

}