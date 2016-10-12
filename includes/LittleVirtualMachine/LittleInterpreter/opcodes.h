//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEstaticMACHINE_OPCODES_H
#define LITTLEstaticMACHINE_OPCODES_H

#include <string>
#include <iostream>
#include <LittlestaticMachine/LittleShared/opcodes.h>
#include <stack>
#include <unordered_map>
#include <stdexcept>

namespace lvm {
    namespace interpreter {

        template <typename T, typename A = int>
        struct vmsystem {
            A program_ptr;
            std::stack<T> stack;
            std::unordered_map<A,T> memory;
            std::unordered_map<A,T> program;
            bool running = false;
        };

        template<typename T>
        class In : public shared::In {
        public:
            static void operator()(T &system) {
                system.stack.push('a'); //Missing cin
            };
        };

        template<typename T>
        class Out : public lvm::shared::Out {
        public:
            static void operator()(T &system) {
                std::cout << static_cast<char>(system.stack.top());
                system.stack.pop();
            };
        };

        template<typename T>
        class Add : public lvm::shared::Add {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top();
                system.stack.pop();
                system.stack.top() += a;
            };
        };

        template<typename T>
        class Sub : public shared::Sub {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top();
                system.stack.pop();
                system.stack.top() -= a;
            };
        };

        template<typename T>
        class Mul : public shared::Mul {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top();
                system.stack.pop();
                system.stack.top() *= a;
            };
        };

        template<typename T>
        class Div : public shared::Div {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top();
                system.stack.pop();
                system.stack.top() /= a;
            };
        };

        template<typename T>
        class Mod : public shared::Mod {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top();
                system.stack.pop();
                system.stack.top() %= a;
            };
        };

        template<typename T>
        class Neg : public shared::Neg {
        public:
            static void operator()(T &system) {
                system.stack.top() = -system.stack.top();
            };
        };

        template<typename T>
        class Inc : public shared::Inc {
        public:
            static void operator()(T &system) {
                system.stack.top() = ++system.stack.top();
            };
        };

        template<typename T>
        class Dec : public shared::Dec {
        public:
            static void operator()(T &system) {
                system.stack.top() = --system.stack.top();
            };
        };

        template<typename T>
        class And : public shared::And {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top;
                system.stack.pop();
                system.stack.top() &= a;
            };
        };

        template<typename T>
        class Or : public shared::Or {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top;
                system.stack.pop();
                system.stack.top() |= a;
            };
        };

        template<typename T>
        class Not : public shared::Not {
        public:
            static void operator()(T &system) {
                system.stack.top() = !system.stack.top();
            };
        };

        template<typename T>
        class Xor : public shared::Xor {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top;
                system.stack.pop();
                system.stack.top() ^= a;
            };
        };

        template<typename T>
        class ShiftLeft : public shared::ShiftLeft {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top;
                system.stack.pop();
                system.stack.top() <<= a;
            };
        };

        template<typename T>
        class ShiftRight : public shared::ShiftRight {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top;
                system.stack.pop();
                system.stack.top() >>= a;
            };
        };

        template<typename T>
        class Pop : public shared::Pop {
        public:
            static void operator()(T &system) {
                system.stack.pop();
            };
        };

        template<typename T>
        class Duplicate : public shared::Duplicate {
        public:
            static void operator()(T &system) {
                system.stack.push(system.stack.top());
            };
        };

        template<typename T>
        class Swap : public shared::Swap {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top();
                system.stack.pop();
                std::swap(system.stack.top(), a);
                system.stack.push(a);
            };
        };

        template<typename T>
        class CopyOver : public shared::CopyOver {
        public:
            static void operator()(T &system) {
                auto a = system.stack.top();
                system.stack.pop();
                auto b = system.stack.top();
                system.stack.push(a);
                system.stack.push(b);
            };
        };

        template<typename T>
        class Load : public shared::Load {
        public:
            static void operator()(T &system) {
                system.stack.top() = system.memory[system.stack.top()];
            };
        };

        template<typename T>
        class Store : public shared::Store {
        public:
            static void operator()(T &system) {
                auto location = system.stack.top();
                system.stack.pop();
                system.memory[location] = system.stack.top();
                system.stack.pop();
            };
        };

        template<typename T>
        class Jump : public shared::Jump {
        public:
            static void operator()(T &system) {
                system.program_ptr = system.stack.top();
                system.stack.pop();
            };
        };

        template<typename T>
        class JumpEqual : public shared::JumpEqual {
        public:
            static void operator()(T &system) {
                auto location = system.stack.top();
                system.stack.pop();
                auto a = system.stack.top();
                system.stack.pop();
                auto b = system.stack.top();
                system.stack.pop();
                if (a == b) {
                    system.program_ptr = location;
                }
            };
        };

        template<typename T>
        class JumpNotEqual : public shared::JumpNotEqual {
        public:
            static void operator()(T &system) {
                auto location = system.stack.top();
                system.stack.pop();
                auto a = system.stack.top();
                system.stack.pop();
                auto b = system.stack.top();
                system.stack.pop();
                if (a != b) {
                    system.program_ptr = location;
                }
            };
        };

        template<typename T>
        class JumpGreater : public shared::JumpGreater {
        public:
            static void operator()(T &system) {
                auto location = system.stack.top();
                system.stack.pop();
                auto a = system.stack.top();
                system.stack.pop();
                auto b = system.stack.top();
                system.stack.pop();
                if (a > b) {
                    system.program_ptr = location;
                }
            };
        };

        template<typename T>
        class JumpGreaterEqual : public shared::JumpGreaterEqual {
        public:
            static void operator()(T &system) {
                auto location = system.stack.top();
                system.stack.pop();
                auto a = system.stack.top();
                system.stack.pop();
                auto b = system.stack.top();
                system.stack.pop();
                if (a >= b) {
                    system.program_ptr = location;
                };
            };
        };

        template<typename T>
        class JumpLesser : public shared::JumpLesser {
            static void operator()(T &system) {
                auto location = system.stack.top();
                system.stack.pop();
                auto a = system.stack.top();
                system.stack.pop();
                auto b = system.stack.top();
                system.stack.pop();
                if (a < b) {
                    system.program_ptr = location;
                }
            };
        };

        template<typename T>
        class JumpLesserEqual : public shared::JumpLesserEqual {
        public:
            static void operator()(T &system) {
                auto location = system.stack.top();
                system.stack.pop();
                auto a = system.stack.top();
                system.stack.pop();
                auto b = system.stack.top();
                system.stack.pop();
                if (a <= b) {
                    system.program_ptr = location;
                }
            };
        };

        template<typename T>
        class Nop : public shared::Nop {
        public:
            static void operator()(T &system) {};
        };

        template<typename T>
        class Halt : public shared::Halt {
        public:
            static void operator()(T &system) {
                system.running = false;
            };
        };

        template<typename T>
        class Push : public lvm::shared::Push {
        public:
            static void operator()(T &system) {
                ++system.program_ptr;
                system.stack.push(system.program[system.program_ptr]);
            };
        };


    }
}


#endif //LITTLEstaticMACHINE_OPCODES_H
