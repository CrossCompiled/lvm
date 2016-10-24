#ifndef LITTLEVIRTUALMACHINE_INTERPRETER_VMSYSTEM_H
#define LITTLEVIRTUALMACHINE_INTERPRETER_VMSYSTEM_H

#include <iostream>

#include <LittleVirtualMachine/LittleInterpreter/has.h>

namespace lvm {
    namespace interpreter {

        //<editor-fold desc="VMStack">

        template <typename T>
        struct need_stack_ptr : has_at<T> {};

        template <typename STACK, bool> struct VMStack : STACK {
            void init(){};
        };

        template <typename STACK>
        struct VMStack<STACK, false > : STACK {
            void init(){};
            typename STACK::value_type ptr = 0;

            void pop() {
                --ptr;
            };

            void push(typename STACK::value_type value) {
                (*this)[++ptr] = value;
            };

            typename STACK::value_type& top () {
                return (*this)[ptr];
            }
        };

        //</editor-fold>

        //<editor-fold desc="VMMemory">


        template <typename MEMORY, bool> struct VMMemory : MEMORY {
            void init(){};
        };

        template <typename MEMORY>
        struct VMMemory<MEMORY, true > : MEMORY {
            void init(){
                this->resize(10000);
            };
        };

        template <typename PROGRAM, bool> struct VMProgram : PROGRAM {
            void init(){};
        };

        template <typename PROGRAM>
        struct VMProgram<PROGRAM, true > : PROGRAM {
            void init(){
                this->resize(10000);
            };
        };

        //</editor-fold>


        template<template<typename...> class OPMAP, typename OPCODESET, typename STACK, typename MEMORY, typename PROGRAM, typename CIN, typename COUT>
        struct vmsystem : VMStack<STACK, has_push<STACK>::value>,
                          VMMemory<MEMORY, has_resize<MEMORY>::value>,
                          VMProgram<PROGRAM, has_resize<PROGRAM>::value> {

            vmsystem(CIN &cin, COUT &cout) : cin(cin), cout(cout) {}



            using cin_type     = CIN;
            using cout_type    = COUT;
            using memory_type  = VMMemory<MEMORY, has_resize<MEMORY>::value>;
            using program_type = VMProgram<PROGRAM, has_resize<PROGRAM>::value>;
            using stack_type   = VMStack<STACK, has_push<STACK>::value>;
            using opcodemap_value_type = typename std::add_pointer<void volatile(
                    vmsystem<OPMAP, OPCODESET, STACK, MEMORY, PROGRAM, CIN, COUT> &)>::type;

            using opcodemap    = OPMAP<opcodemap_value_type, OPCODESET>;

            typename PROGRAM::value_type program_ptr;
            bool running = true;

            memory_type memory;
            program_type program;
            stack_type stack;
            cout_type &cout;
            cin_type &cin;

            template<typename ISTREAM>
            void init(ISTREAM &is) {
                memory.init();
                program.init();
                stack.init();
                this->load_program(is);
                this->running = true;
                this->program_ptr = 0;
            }

            template<typename ISTREAM, typename IPROGRAM=PROGRAM>
            typename std::enable_if<has_data<IPROGRAM>::value && (!has_resize<IPROGRAM>::value), void>::type
            load_program(ISTREAM &is) {
                if (is) {
                    is.seekg(0, is.end);
                    int length = is.tellg() * sizeof(typename ISTREAM::char_type);
                    is.seekg(0, is.beg);
                    is.read(reinterpret_cast<typename ISTREAM::char_type *>(program.data()), length);
                }

            };


            template<typename ISTREAM, typename IPROGRAM=PROGRAM>
            typename std::enable_if<has_data<IPROGRAM>::value && (has_resize<IPROGRAM>::value), void>::type
            load_program(ISTREAM &is) {
                if (is) {
                    is.seekg(0, is.end);
                    int length = is.tellg() * sizeof(typename ISTREAM::char_type);
                    is.seekg(0, is.beg);
                    program.resize(length / sizeof(typename PROGRAM::value_type));
                    is.read(reinterpret_cast<typename ISTREAM::char_type *>(program.data()), length);
                }

            };

            void run() {
                while (this->running) {
                    opcodemap::data[program[program_ptr]](*this);
                }
            }

            int exit_code() {
                return stack.top();
            };

        };
    }
}

#endif //LITTLEVIRTUALMACHINE_INTERPRETER_VMSYSTEM_H