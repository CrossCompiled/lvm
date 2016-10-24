//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
#define LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H

#include <cstdint>

namespace lvm {
    namespace shared {
        namespace opcodes {

            template<int id>
            struct base {
                using code = std::integral_constant<int64_t, id>;
            };
            
            struct In : base<0x00> {
                static constexpr const char *name = "IN";
            };

            struct Out : base<0x01> {
                static constexpr const char *name = "OUT";
            };

            struct Add : base<0x02> {
                static constexpr const char *name = "ADD";
            };

            struct Sub : base<0x03> {
                static constexpr const char *name = "SUB";
            };

            struct Mul : base<0x04> {
                static constexpr const char *name = "MUL";
            };

            struct Div : base<0x05> {
                static constexpr const char *name = "DIV";
            };

            struct Mod : base<0x06> {
                static constexpr const char *name = "MOD";
            };

            struct Neg : base<0x07> {
                static constexpr const char *name = "NEG";
            };

            struct Inc : base<0x08> {
                static constexpr const char *name = "INC";
            };

            struct Dec : base<0x09> {
                static constexpr const char *name = "DEC";
            };

            struct And : base<0x0A> {
                static constexpr const char *name = "AND";
            };

            struct Or : base<0x0B> {
                static constexpr const char *name = "OR";
            };

            struct Not : base<0x0C> {
                static constexpr const char *name = "NOT";
            };

            struct Xor : base<0x0D> {
                static constexpr const char *name = "XOR";
            };

            struct ShiftLeft : base<0x0E> {
                static constexpr const char *name = "SHL";
            };

            struct ShiftRight : base<0x0F> {
                static constexpr const char *name = "SHR";
            };

            struct Push : base<0x10> {
                static constexpr const char *name = "PUSH";
            };

            struct Pop : base<0x11> {
                static constexpr const char *name = "POP";
            };

            struct Duplicate : base<0x12> {
                static constexpr const char *name = "DUP";
            };

            struct Swap : base<0x13> {
                static constexpr const char *name = "SWP";
            };

            struct CopyOver : base<0x14> {
                static constexpr const char *name = "OVR";
            };

            struct Load : base<0x15> {
                static constexpr const char *name = "LOAD";
            };

            struct Store : base<0x16> {
                static constexpr const char *name = "STOR";
            };

            struct Jump : base<0x17> {
                static constexpr const char *name = "JMP";
            };

            struct JumpEqual : base<0x18> {
                static constexpr const char *name = "JE";
            };

            struct JumpNotEqual : base<0x19> {
                static constexpr const char *name = "JNE";
            };

            struct JumpGreater : base<0x1A> {
                static constexpr const char *name = "JG";
            };

            struct JumpGreaterEqual : base<0x1B> {
                static constexpr const char *name = "JGE";
            };

            struct JumpLesser : base<0x1C> {
                static constexpr const char *name = "JL";
            };

            struct JumpLesserEqual : base<0x1D> {
                static constexpr const char *name = "JLE";
            };

            struct Nop : base<0x1E> {
                static constexpr const char *name = "NOP";
            };

            struct Halt : base<0x1F> {
                static constexpr const char *name = "HALT";
            };
        }
    }
}


#endif //LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
