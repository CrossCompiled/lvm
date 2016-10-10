//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
#define LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H

namespace lvm {
    namespace shared {
        class BaseOpcode {
        public:
            static const uint8_t code;
            static const std::string name;
        };

        class In : BaseOpcode {
        public:
            static const uint8_t code =0x00;
            static const std::string name = "IN";
        };

        class Out : BaseOpcode {
        public:
            static const uint8_t code =0x01;
            static const std::string name = "OUT";
        };

        class Add : BaseOpcode {
        public:
            static const uint8_t code = 0x02;
            static const std::string name = "ADD";
        };

        class Sub : BaseOpcode {
        public:
            static const uint8_t code = 0x03;
            static const std::string name = "SUB";
        };

        class Mul : BaseOpcode{
        public:
            static const uint8_t code = 0x04;
            static const std::string name = "MUL";
        };

        class Div : BaseOpcode{
        public:
            static const uint8_t code = 0x05;
            static const std::string name = "DIV";
        };

        class Mod : BaseOpcode{
        public:
            static const uint8_t code = 0x06;
            static const std::string name = "Mod";
        };

        class Neg : BaseOpcode{
        public:
            static const uint8_t code = 0x07;
            static const std::string name = "NEG";
        };

        class Inc : BaseOpcode{
        public:
            static const uint8_t code = 0x08;
            static const std::string name = "INC";
        };

        class Dec : BaseOpcode{
        public:
            static const uint8_t code = 0x09;
            static const std::string name = "DEC";
        };

        class And : BaseOpcode{
        public:
            static const uint8_t code = 0x0A;
            static const std::string name = "AND";
        };

        class Or : BaseOpcode{
        public:
            static const uint8_t code = 0x0B;
            static const std::string name = "OR";
        };

        class Not : BaseOpcode{
        public:
            static const uint8_t code = 0x0C;
            static const std::string name = "NOT";
        };

        class Xor : BaseOpcode{
        public:
            static const uint8_t code = 0x0D;
            static const std::string name = "XOR";
        };

        class Shl : BaseOpcode{
        public:
            static cMulonst uint8_t code = 0x0E;
            static const std::string name = "SHL";
        };

        class Shr : BaseOpcode{
        public:
            static const uint8_t code = 0x0F;
            static const std::string name = "SHR";
        };

        class Pop : BaseOpcode{
        public:
            static const uint8_t code = 0x10;
            static const std::string name = "POP";
        };

        class Dup : BaseOpcode{
        public:
            static const uint8_t code = 0x11;
            static const std::string name = "DUP";
        };

        class Swp : BaseOpcode{
        public:
            static const uint8_t code = 0x12;
            static const std::string name = "SWP";
        };

        class Ovr : BaseOpcode{
        public:
            static const uint8_t code = 0x13;
            static const std::string name = "OVR";
        };

        class Load : BaseOpcode{
        public:
            static const uint8_t code = 0x14;
            static const std::string name = "LOAD";
        };

        class Stor : BaseOpcode{
        public:
            static const uint8_t code = 0x15;
            static const std::string name = "STOR";
        };

        class Jmp : BaseOpcode{
        public:
            static const uint8_t code = 0x16;
            static const std::string name = "JMP";
        };

        class Je : BaseOpcode{
        public:
            static const uint8_t code = 0x17;
            static const std::string name = "JE";
        };

        class Jne : BaseOpcode{
        public:
            static const uint8_t code = 0x18;
            static const std::string name = "JNE";
        };

        class Jg : BaseOpcode{
        public:
            static const uint8_t code = 0x19;
            static const std::string name = "JG";
        };

        class Jl : BaseOpcode{
        public:
            static const uint8_t code = 0x1A;
            static const std::string name = "JL";
        };

        class Jle : BaseOpcode{
        public:
            static const uint8_t code = 0x1B;
            static const std::string name = "JLE";
        };

        class Nop : BaseOpcode{
        public:
            static const uint8_t code = 0x1C;
            static const std::string name = "NOP";
        };

        class Halt : BaseOpcode{
        public:
            static const uint8_t code = 0x1D;
            static const std::string name = "HALT";
        };
    }
}




#endif //LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
