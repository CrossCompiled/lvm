//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H
#define LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H

#include <iostream>
#include <stack>
#include <array>

#include <LittleVirtualMachine/LittleShared/opcodes.h>

namespace lvm {
    namespace interpreter {

        template <typename T>
        struct vmsystem {
            T program_ptr;
            std::stack<T> stack;
            std::array<T,1000> memory;
            std::array<T,1000> program;
            bool running = false;
        };

        template<typename... T>
        struct oc_list_holder {};

        template<typename A, typename... T>
        struct oc_list {
            using values = oc_list_holder<T...>;
            using size = typename std::integral_constant<A, sizeof...(T)>;
        };

        template<typename T, typename ...Args>
        struct oc_11_impl {
            using type = typename std::add_pointer<void volatile(vmsystem<T>&)>::type;
            using list = oc_list<T, Args...>;
        };

        namespace opcodes {
            struct In : shared::opcodes::In {
                template<typename T>
                static volatile void execute(T& system) {
                    system.stack.push(0);
                    std::cin >> system.stack.top();
                    ++system.program_ptr;
                };
            };

            struct Out : shared::opcodes::Out {
                template<typename T>
                static volatile void execute(T& system) {
                    std::cout << (char)system.stack.top();
                    system.stack.pop();
                    ++system.program_ptr;
                };
            };

            struct Add : shared::opcodes::Add {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Sub : shared::opcodes::Sub {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Mul : shared::opcodes::Mul {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Div : shared::opcodes::Div {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Mod : shared::opcodes::Mod {
                template<typename T>
                static volatile void execute(T& system) {
                    auto a = system.stack.top();
                    system.stack.pop();
                    system.stack.top() = system.stack.top() % a;
                    ++system.program_ptr;
                };
            };

            struct Neg : shared::opcodes::Neg {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Inc : shared::opcodes::Inc {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Dec : shared::opcodes::Dec {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct And : shared::opcodes::And {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Or : shared::opcodes::Or {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Not : shared::opcodes::Not {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Xor : shared::opcodes::Xor {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct ShiftLeft : shared::opcodes::ShiftLeft {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct ShiftRight : shared::opcodes::ShiftRight {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Push : shared::opcodes::Push {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                    system.stack.push(system.program[system.program_ptr]);
                    ++system.program_ptr;
                };
            };

            struct Pop : shared::opcodes::Pop {
                template<typename T>
                static volatile void execute(T& system) {
                    system.stack.pop();
                    ++system.program_ptr;
                };
            };

            struct Duplicate : shared::opcodes::Duplicate {
                template<typename T>
                static volatile void execute(T& system) {
                    system.stack.push(system.stack.top());
                    ++system.program_ptr;
                };
            };

            struct Swap : shared::opcodes::Swap {
                template<typename T>
                static volatile void execute(T& system) {
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.pop();
                    system.stack.push(a);
                    system.stack.push(b);
                    ++system.program_ptr;
                };
            };

            struct CopyOver : shared::opcodes::CopyOver {
                template<typename T>
                static volatile void execute(T& system) {
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.push(a);
                    system.stack.push(b);
                    ++system.program_ptr;
                };
            };

            struct Load : shared::opcodes::Load {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Store : shared::opcodes::Store {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Jump : shared::opcodes::Jump {
                template<typename T>
                static volatile void execute(T& system) {
                    system.program_ptr = system.stack.top();
                    system.stack.pop();
                };
            };

            struct JumpEqual : shared::opcodes::JumpEqual {
                template<typename T>
                static volatile void execute(T& system) {
                    auto addr = system.stack.top();
                    system.stack.pop();
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.pop();
                    system.program_ptr = (a == b) ? addr : system.program_ptr + 1;
                };
            };

            struct JumpNotEqual : shared::opcodes::JumpNotEqual {
                template<typename T>
                static volatile void execute(T& system) {
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.pop();
                    system.program_ptr = (a != b) ? system.stack.top() : system.program_ptr + 1;
                    system.stack.pop();
                };
            };

            struct JumpGreater : shared::opcodes::JumpGreater {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct JumpGreaterEqual : shared::opcodes::JumpGreaterEqual {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct JumpLesser : shared::opcodes::JumpLesser {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct JumpLesserEqual : shared::opcodes::JumpLesserEqual {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Nop : shared::opcodes::Nop {
                template<typename T>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                };
            };

            struct Halt : shared::opcodes::Halt {
                template<typename T>
                static volatile void execute(T& system) {
                    system.running = false;
                    ++system.program_ptr;
                };
            };



            template<typename T>
            using oc_11 = oc_11_impl<T, In, Out, Add, Sub, Mul, Div, Mod, Neg, Inc, Dec, And, Or, Not, Xor, ShiftLeft, ShiftRight, Push, Pop, Duplicate, Swap, CopyOver, Load, Store, Jump, JumpEqual, JumpNotEqual, JumpGreater, JumpGreaterEqual, JumpLesser, JumpLesserEqual, Nop, Halt>;
        };


        template<typename B, typename A>
        struct oc_array_impl{};

        template<typename A, template<class...> class L, class... T>
        struct oc_array_impl<A, L<T...>> {
            static const std::array<A, sizeof...(T)> data;
        };

        template<typename A, template<class...> class L, class... T>
        const std::array<A, sizeof...(T)> oc_array_impl<A, L<T...>>::data = { T::execute... };

        template<typename T>
        struct oc_array : oc_array_impl<typename T::type, typename T::list::values>  {};


    }
}


#endif //LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H