//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H
#define LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H

#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <fstream>

#include <LittleVirtualMachine/LittleShared/opcodes.h>
namespace lvm {
    namespace interpreter {

        template <typename T>
        struct has_at {
            enum { value = false };
        };

        template <typename T>
        struct has_at<std::vector<T>> {
            enum { value = true };
        };

        template <typename T, size_t F>
        struct has_at<std::array<T, F>> {
            enum { value = true };
        };

        template <typename T>
        struct has_pop {
            enum { value = false };
        };

        template <typename T>
        struct has_pop<std::stack<T>> {
            enum { value = true };
        };

        template <typename T>
        struct has_push {
            enum { value = false };
        };

        template <typename T>
        struct has_push<std::stack<T>> {
            enum { value = true };
        };

        template <typename T>
        struct need_stack_ptr {
            enum { value = false };
        };

        template <typename T, size_t F>
        struct need_stack_ptr<std::array<T, F>> {
            enum { value = true };
        };

        template <typename T, bool> struct NeedStackPointer { };

        template <typename T>
        struct NeedStackPointer<T, true > {
            T stack_ptr = 0;
        };


        template <typename T, typename STACK, typename MEMORY, typename PROGRAM >
        struct vmsystem : NeedStackPointer<T, need_stack_ptr<STACK>::value> {
            using mytype = vmsystem<T, STACK, MEMORY, PROGRAM>;
            using type = T;
            using optype = typename std::add_pointer<void volatile(vmsystem<T, STACK, MEMORY, PROGRAM>&)>::type;

            using stack_type = STACK;
            using memory_type = MEMORY;
            using program_type = PROGRAM;

            T program_ptr = 0;
            bool running = true;

            MEMORY memory;
            PROGRAM program;
            STACK stack;

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
            using type = T;
            using list = oc_list<typename T::type, Args...>;
        };

        namespace opcodes {
            struct In : shared::opcodes::In {
                template<typename T, typename std::enable_if<has_push<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack.push(0);
                    std::cin >> system.stack.top();
                    ++system.program_ptr;
                };

                template<typename T, typename std::enable_if<!has_push<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    // std::cout << "IN" << std::endl;
                    ++system.stack_ptr;
                    std::cin >> system.stack[system.stack_ptr];
                    ++system.program_ptr;
                };
            };

            struct Out : shared::opcodes::Out {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    std::cout << (char)system.stack.top();
                    system.stack.pop();
                    ++system.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    std::cout << (char)system.stack[system.stack_ptr];
                    --system.stack_ptr;
                    ++system.program_ptr;
                };

            };

            struct Add : shared::opcodes::Add {
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    --vm.stack_ptr;
                    vm.stack[vm.stack_ptr] += vm.stack[vm.stack_ptr+1];
                    // std::cout << "ADD " << vm.stack[vm.stack_ptr] << std::endl;
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    vm.stack.top() += a;
                    ++vm.program_ptr;
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
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    --system.stack_ptr;
                    system.stack[system.stack_ptr] = system.stack[system.stack_ptr] / system.stack[system.stack_ptr+1];
                    // std::cout << "DIV " << system.stack[system.stack_ptr] << std::endl;
                    ++system.program_ptr;
                };
            };

            struct Mod : shared::opcodes::Mod {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    vm.stack.top() = vm.stack.top() % a;
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    --system.stack_ptr;
                    system.stack[system.stack_ptr] = system.stack[system.stack_ptr] % system.stack[system.stack_ptr+1];
                    // std::cout << "MOD " << system.stack[system.stack_ptr] << std::endl;
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
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    ++system.stack[system.stack_ptr];
                    // std::cout << "INC " << system.stack[system.stack_ptr] << std::endl;
                    ++system.program_ptr;
                };
            };

            struct Dec : shared::opcodes::Dec {
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    --system.stack[system.stack_ptr];
                    // std::cout << "INC " << system.stack[system.stack_ptr] << std::endl;
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
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    ++system.program_ptr;
                    system.stack.push(system.program[system.program_ptr]);
                    ++system.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack[++system.stack_ptr] = system.program[++system.program_ptr];
                    ++system.program_ptr;
                };
            };

            struct Pop : shared::opcodes::Pop {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack.pop();
                    ++system.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    --system.stack_ptr;
                    ++system.program_ptr;
                };
            };

            struct Duplicate : shared::opcodes::Duplicate {

                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack.push(system.stack.top());
                    ++system.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    ++system.stack_ptr;
                    system.stack[system.stack_ptr] = system.stack[system.stack_ptr - 1];
                    ++system.program_ptr;
                };

            };

            struct Swap : shared::opcodes::Swap {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.pop();
                    system.stack.push(a);
                    system.stack.push(b);
                    ++system.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    auto a = system.stack[system.stack_ptr];
                    system.stack[system.stack_ptr] = system.stack[system.stack_ptr-1];
                    system.stack[system.stack_ptr-1] = a;
                    // std::cout << "SWAP " << system.stack[system.stack_ptr] << std::endl;
                    ++system.program_ptr;
                };
            };

            struct CopyOver : shared::opcodes::CopyOver {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.push(a);
                    system.stack.push(b);
                    ++system.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    ++system.stack_ptr;
                    system.stack[system.stack_ptr] = system.stack[system.stack_ptr-2];
                    ++system.program_ptr;
                };
            };

            struct Load : shared::opcodes::Load {
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack[system.stack_ptr] = system.memory[system.stack[system.stack_ptr]];
                    ++system.program_ptr;
                };
            };

            struct Store : shared::opcodes::Store {
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack_ptr -= 2;
                    system.memory[system.stack[system.stack_ptr + 2]] = system.stack[system.stack_ptr + 1];
                    ++system.program_ptr;
                };
            };

            struct Jump : shared::opcodes::Jump {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.program_ptr = system.stack.top();
                    system.stack.pop();
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.program_ptr = system.stack[system.stack_ptr];
                    --system.stack_ptr;
                };
            };

            struct JumpEqual : shared::opcodes::JumpEqual {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    auto addr = system.stack.top();
                    system.stack.pop();
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.pop();
                    system.program_ptr = (a == b) ? addr : system.program_ptr + 1;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack_ptr -= 3;
                    system.program_ptr = (system.stack[system.stack_ptr + 2] == system.stack[system.stack_ptr + 1]) ? system.stack[system.stack_ptr + 3] : system.program_ptr + 1;
                };
            };

            struct JumpNotEqual : shared::opcodes::JumpNotEqual {
                template<typename T, typename std::enable_if<has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    auto addr = system.stack.top();
                    system.stack.pop();
                    auto a = system.stack.top();
                    system.stack.pop();
                    auto b = system.stack.top();
                    system.stack.pop();
                    system.program_ptr = (a != b) ? addr : system.program_ptr + 1;
                };
                template<typename T, typename std::enable_if<!has_pop<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& system) {
                    system.stack_ptr -= 3;
                    system.program_ptr = (system.stack[system.stack_ptr + 2] != system.stack[system.stack_ptr + 1]) ? system.stack[system.stack_ptr + 3] : system.program_ptr + 1;
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
        struct oc_array : oc_array_impl<typename T::type::optype, typename T::list::values>  {};


        template<typename T, typename std::enable_if<need_stack_ptr<typename T::stack_type>::value, int>::type = 0>
        static int exit_code(T& vm) {
            return vm.stack[vm.stack_ptr];
        }

        template<typename T, typename std::enable_if<!need_stack_ptr<typename T::stack_type>::value, int>::type = 0>
        static int exit_code(T& vm) {
            return vm.stack.top();
        }

    }
}


#endif //LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H