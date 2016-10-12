//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
#define LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H

namespace lvm {
    namespace shared {
        class BaseOpcode {
        public:
            static constexpr const int code = 0xFF;
            static constexpr const char* name = "";
        };

        class In : BaseOpcode {
        public:
            static constexpr const int code = 0x00;
            static constexpr const char* name = "IN";
        };

        class Out : BaseOpcode {
        public:
            static constexpr const int code = 0x01;
            static constexpr const char* name = "OUT";
        };

        class Add : BaseOpcode {
        public:
            static constexpr const int code = 0x02;
            static constexpr const char* name = "ADD";
        };

        class Sub : BaseOpcode {
        public:
            static constexpr const int code = 0x03;
            static constexpr const char* name = "SUB";
        };

        class Mul : BaseOpcode{
        public:
            static constexpr const int code = 0x04;
            static constexpr const char* name = "MUL";
        };

        class Div : BaseOpcode{
        public:
            static constexpr const int code = 0x05;
            static constexpr const char* name = "DIV";
        };

        class Mod : BaseOpcode{
        public:
            static constexpr const int code = 0x06;
            static constexpr const char* name = "Mod";
        };

        class Neg : BaseOpcode{
        public:
            static constexpr const int code = 0x07;
            static constexpr const char* name = "NEG";
        };

        class Inc : BaseOpcode{
        public:
            static constexpr const int code = 0x08;
            static constexpr const char* name = "INC";
        };

        class Dec : BaseOpcode{
        public:
            static constexpr const int code = 0x09;
            static constexpr const char* name = "DEC";
        };

        class And : BaseOpcode{
        public:
            static constexpr const int code = 0x0A;
            static constexpr const char* name = "AND";
        };

        class Or : BaseOpcode{
        public:
            static constexpr const int code = 0x0B;
            static constexpr const char* name = "OR";
        };

        class Not : BaseOpcode{
        public:
            static constexpr const int code = 0x0C;
            static constexpr const char* name = "NOT";
        };

        class Xor : BaseOpcode{
        public:
            static constexpr const int code = 0x0D;
            static constexpr const char* name = "XOR";
        };

        class Shl : BaseOpcode{
        public:
            static constexpr const int code = 0x0E;
            static constexpr const char* name = "SHL";
        };

        class Shr : BaseOpcode{
        public:
            static constexpr const int code = 0x0F;
            static constexpr const char* name = "SHR";
        };

        class Pop : BaseOpcode{
        public:
            static constexpr const int code = 0x10;
            static constexpr const char* name = "POP";
        };

        class Dup : BaseOpcode{
        public:
            static constexpr const int code = 0x11;
            static constexpr const char* name = "DUP";
        };

        class Swp : BaseOpcode{
        public:
            static constexpr const int code = 0x12;
            static constexpr const char* name = "SWP";
        };

        class Ovr : BaseOpcode{
        public:
            static constexpr const int code = 0x13;
            static constexpr const char* name = "OVR";
        };

        class Load : BaseOpcode{
        public:
            static const uint8_t code = 0x14;
            static constexpr const char* name = "LOAD";
        };

        class Stor : BaseOpcode{
        public:
            static constexpr const int code = 0x15;
            static constexpr const char* name = "STOR";
        };

        class Jmp : BaseOpcode{
        public:
            static constexpr const int code = 0x16;
            static constexpr const char* name = "JMP";
        };

        class Je : BaseOpcode{
        public:
            static constexpr const int code = 0x17;
            static constexpr const char* name = "JE";
        };

        class Jne : BaseOpcode{
        public:
            static constexpr const int code = 0x18;
            static constexpr const char* name = "JNE";
        };

        class Jg : BaseOpcode{
        public:
            static constexpr const int code = 0x19;
            static constexpr const char* name = "JG";
        };

        class Jge : BaseOpcode{
        public:
            static constexpr const int code = 0x1A;
            static constexpr const char* name = "JGE";
        };

        class Jl : BaseOpcode{
        public:
            static constexpr const int code = 0x1B;
            static constexpr const char* name = "JL";
        };

        class Jle : BaseOpcode{
        public:
            static constexpr const int code = 0x1C;
            static constexpr const char* name = "JLE";
        };

        class Nop : BaseOpcode{
        public:
            static constexpr const int code = 0x1D;
            static constexpr const char* name = "NOP";
        };

        class Halt : BaseOpcode{
        public:
            static constexpr const int code = 0x1E;
            static constexpr const char* name = "HALT";
        };
    }
}




#endif //LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
