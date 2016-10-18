// read a file into memory
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <array>
#include <LittleVirtualMachine/LittleInterpreter/opcodes.h>

int main(int argn, char* argc[]) {

    auto system = lvm::interpreter::vmsystem<int32_t>();

    using oc_11       = lvm::interpreter::opcodes::oc_11<int32_t>;
    using oc_11_array = lvm::interpreter::oc_array<oc_11>;

    std::cout << argc[1] << std::endl;

    std::ifstream is (argc[1], std::ifstream::binary);
    if (is) {
        is.seekg (0, is.end);
        int length = (int32_t)is.tellg() / 4;
        is.seekg (0, is.beg);
        is.read(reinterpret_cast<char *>(system.program.data()), sizeof(int32_t)*length);
        is.close();
    }

    system.running = true;
    system.program_ptr = 0;

    while(system.running) {
        oc_11_array::data[system.program[system.program_ptr]](system);
    }

    return 0;

}