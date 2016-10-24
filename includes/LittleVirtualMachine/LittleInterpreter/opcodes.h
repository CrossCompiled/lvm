//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H
#define LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H

#include <array>
#include <LittleVirtualMachine/LittleShared/opcodes.h>

#define GENERATE_HAS(member)                                               \
                                                                                  \
template < class T >                                                              \
class has_##member ## _impl                                                          \
{                                             \
    struct Fallback { int member; };                                                     \
    struct Derived : T, Fallback { };                                               \
                                                                                    \
    template<typename U, U> struct Check;                                           \
                                                                                    \
    typedef char ArrayOfOne[1];                                                     \
    typedef char ArrayOfTwo[2];                                                     \
                                                                                    \
    template<typename U> static ArrayOfOne & func(Check<int Fallback::*, &U::member> *); \
    template<typename U> static ArrayOfTwo & func(...);                             \
  public:                                                                           \
    static constexpr bool value = (sizeof(func<Derived>(0)) == 2);                       \
};  \
  \
template < class T >                                                              \
struct has_##member : public std::integral_constant<bool, has_##member ## _impl<T>::value> {};






namespace lvm {
    namespace interpreter {

        GENERATE_HAS(resize);
        GENERATE_HAS(data);
        GENERATE_HAS(at);
        GENERATE_HAS(push);

        //<editor-fold desc="VMStack">

        template <typename T>
        struct has_stack_ptr : has_push<T> {};

        template <typename STACK, bool> struct VMStack : STACK {};

        template <typename STACK>
        struct VMStack<STACK, true > : STACK {
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

        //<editor-fold desc="Set Templates">

        template<typename... T>
        struct set_holder {};

        template<typename... T>
        struct set {
            using values = set_holder<T...>;
            using size = typename std::integral_constant<int64_t, sizeof...(T)>;
        };


        //</editor-fold>

        //<editor-fold desc="Array Templates">

        template<typename B, typename A>
        struct array {};

        template<typename A, template<class...> class L, class... T>
        struct array<A, L<T...>> {
            static const std::array<A, sizeof...(T)> data;
        };

        template<typename A, template<class...> class L, class... T>
        const std::array<A, sizeof...(T)> array<A, L<T...>>::data = { T::execute... };

        //</editor-fold>


        template <typename OPCODESET, typename STACK, typename MEMORY, typename PROGRAM, typename CIN, typename COUT >
        struct vmsystem : VMStack<STACK, !has_stack_ptr<STACK>::value> {

            vmsystem(CIN& cin, COUT& cout) : cin(cin), cout(cout) {}

            using opcodemap_value_type = typename std::add_pointer<void volatile(vmsystem<OPCODESET, STACK, MEMORY, PROGRAM, CIN, COUT>&)>::type;

            using memory_type  = MEMORY;
            using program_type = PROGRAM;
            using cin_type     = CIN;
            using cout_type    = COUT;
            using stack_type   = VMStack<STACK, !has_stack_ptr<STACK>::value>;
            using opcodemap    = array<opcodemap_value_type, OPCODESET>;

            typename PROGRAM::value_type program_ptr;
            bool running = true;

            memory_type memory;
            program_type program;
            stack_type stack;
            cout_type& cout;
            cin_type& cin;

            template<typename ISTREAM>
            void init(ISTREAM& is) {
                this->load_program(is);
                this->running = true;
                this->program_ptr = 0;
            }

            template<typename ISTREAM, typename IPROGRAM=PROGRAM>
            typename std::enable_if<has_data<IPROGRAM>::value && (!has_resize<IPROGRAM>::value), void>::type load_program(ISTREAM& is) {
                if (is) {
                    is.seekg (0, is.end);
                    int length = is.tellg() * sizeof(typename ISTREAM::char_type);
                    is.seekg (0, is.beg);
                    is.read(reinterpret_cast<typename ISTREAM::char_type *>(program.data()), length);
                }

            };


            template<typename ISTREAM, typename IPROGRAM=PROGRAM>
            typename std::enable_if<has_data<IPROGRAM>::value && (has_resize<IPROGRAM>::value), void>::type load_program(ISTREAM& is) {
                if (is) {
                    is.seekg (0, is.end);
                    int length = is.tellg() * sizeof(typename ISTREAM::char_type);
                    is.seekg (0, is.beg);
                    program.resize(length / sizeof(typename PROGRAM::value_type));
                    is.read(reinterpret_cast<typename ISTREAM::char_type *>(program.data()), length);
                }

            };

            int run() {
                while(this->running) {
                    opcodemap::data[program[program_ptr]](*this);
                }
                return 0;
            }

            int exit_code() {
                return stack.top();
            };

        };



        namespace opcodes {
            struct In : shared::opcodes::In {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    vm.stack.push(0);
                    vm.cin >> vm.stack.top();
                    ++vm.program_ptr;
                };

                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    ++vm.stack.ptr;
                    vm.cin >> vm.stack[vm.stack.ptr];
                    ++vm.program_ptr;
                };
            };

            struct Out : shared::opcodes::Out {
                template<typename T>
                static volatile void execute(T& vm) {
                    vm.cout << (char)vm.stack.top();
                    vm.stack.pop();
                    ++vm.program_ptr;
                };
            };

            struct Add : shared::opcodes::Add {
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    --vm.stack.ptr;
                    vm.stack[vm.stack.ptr] += vm.stack[vm.stack.ptr+1];
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    vm.stack.top() += a;
                    ++vm.program_ptr;
                };
            };

            struct Sub : shared::opcodes::Sub {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Mul : shared::opcodes::Mul {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Div : shared::opcodes::Div {

                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    vm.stack.top() /= a;
                    ++vm.program_ptr;
                };

                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    --vm.stack.ptr;
                    vm.stack[vm.stack.ptr] = vm.stack[vm.stack.ptr] / vm.stack[vm.stack.ptr+1];
                    ++vm.program_ptr;
                };
            };

            struct Mod : shared::opcodes::Mod {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    vm.stack.top() = vm.stack.top() % a;
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    --vm.stack.ptr;
                    vm.stack[vm.stack.ptr] = vm.stack[vm.stack.ptr] % vm.stack[vm.stack.ptr+1];
                    ++vm.program_ptr;
                };
            };

            struct Neg : shared::opcodes::Neg {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Inc : shared::opcodes::Inc {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.stack.top();
                    ++vm.program_ptr;
                };
            };

            struct Dec : shared::opcodes::Dec {
                template<typename T>
                static volatile void execute(T& vm) {
                    --vm.stack.top();
                    ++vm.program_ptr;
                };
            };

            struct And : shared::opcodes::And {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Or : shared::opcodes::Or {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Not : shared::opcodes::Not {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Xor : shared::opcodes::Xor {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct ShiftLeft : shared::opcodes::ShiftLeft {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct ShiftRight : shared::opcodes::ShiftRight {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Push : shared::opcodes::Push {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                    vm.stack.push(vm.program[vm.program_ptr]);
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    vm.stack[++vm.stack.ptr] = vm.program[++vm.program_ptr];
                    ++vm.program_ptr;
                };
            };

            struct Pop : shared::opcodes::Pop {
                template<typename T>
                static volatile void execute(T& vm) {
                    vm.stack.pop();
                    ++vm.program_ptr;
                };
            };

            struct Duplicate : shared::opcodes::Duplicate {
                template<typename T>
                static volatile void execute(T& vm) {
                    vm.stack.push(vm.stack.top());
                    ++vm.program_ptr;
                };

            };

            struct Swap : shared::opcodes::Swap {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    auto b = vm.stack.top();
                    vm.stack.pop();
                    vm.stack.push(a);
                    vm.stack.push(b);
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack[vm.stack.ptr];
                    vm.stack[vm.stack.ptr] = vm.stack[vm.stack.ptr-1];
                    vm.stack[vm.stack.ptr-1] = a;
                    ++vm.program_ptr;
                };
            };

            struct CopyOver : shared::opcodes::CopyOver {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    auto b = vm.stack.top();
                    vm.stack.push(a);
                    vm.stack.push(b);
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    ++vm.stack.ptr;
                    vm.stack[vm.stack.ptr] = vm.stack[vm.stack.ptr-2];
                    ++vm.program_ptr;
                };
            };

            struct Load : shared::opcodes::Load {
                template<typename T>
                static volatile void execute(T& vm) {
                    vm.stack.top() = vm.memory[vm.stack.top()];
                    ++vm.program_ptr;
                };
            };

            struct Store : shared::opcodes::Store {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    vm.memory[a] = vm.stack.top();
                    vm.stack.pop();
                    ++vm.program_ptr;
                };
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    vm.stack.ptr -= 2;
                    vm.memory[vm.stack[vm.stack.ptr + 2]] = vm.stack[vm.stack.ptr + 1];
                    ++vm.program_ptr;
                };
            };

            struct Jump : shared::opcodes::Jump {
                template<typename T>
                static volatile void execute(T& vm) {
                    vm.program_ptr = vm.stack.top();
                    vm.stack.pop();
                };
            };

            struct JumpEqual : shared::opcodes::JumpEqual {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto addr = vm.stack.top();
                    vm.stack.pop();
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    auto b = vm.stack.top();
                    vm.stack.pop();
                    vm.program_ptr = (a == b) ? addr : vm.program_ptr + 1;
                };
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    vm.stack.ptr -= 3;
                    vm.program_ptr = (vm.stack[vm.stack.ptr + 2] == vm.stack[vm.stack.ptr + 1]) ? vm.stack[vm.stack.ptr + 3] : vm.program_ptr + 1;
                };
            };

            struct JumpNotEqual : shared::opcodes::JumpNotEqual {
                template<typename T, typename std::enable_if<!has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    auto addr = vm.stack.top();
                    vm.stack.pop();
                    auto a = vm.stack.top();
                    vm.stack.pop();
                    auto b = vm.stack.top();
                    vm.stack.pop();
                    vm.program_ptr = (a != b) ? addr : vm.program_ptr + 1;
                };
                template<typename T, typename std::enable_if<has_at<typename T::stack_type>::value, int>::type = 0>
                static volatile void execute(T& vm) {
                    vm.stack.ptr -= 3;
                    vm.program_ptr = (vm.stack[vm.stack.ptr + 2] != vm.stack[vm.stack.ptr + 1]) ? vm.stack[vm.stack.ptr + 3] : vm.program_ptr + 1;
                };
            };

            struct JumpGreater : shared::opcodes::JumpGreater {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct JumpGreaterEqual : shared::opcodes::JumpGreaterEqual {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct JumpLesser : shared::opcodes::JumpLesser {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct JumpLesserEqual : shared::opcodes::JumpLesserEqual {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Nop : shared::opcodes::Nop {
                template<typename T>
                static volatile void execute(T& vm) {
                    ++vm.program_ptr;
                };
            };

            struct Halt : shared::opcodes::Halt {
                template<typename T>
                static volatile void execute(T& vm) {
                    vm.running = false;
                    ++vm.program_ptr;
                };
            };

        };

        using oc_11 = set<
                opcodes::In,
                opcodes::Out,
                opcodes::Add,
                opcodes::Sub,
                opcodes::Mul,
                opcodes::Div,
                opcodes::Mod,
                opcodes::Neg,
                opcodes::Inc,
                opcodes::Dec,
                opcodes::And,
                opcodes::Or,
                opcodes::Not,
                opcodes::Xor,
                opcodes::ShiftLeft,
                opcodes::ShiftRight,
                opcodes::Push,
                opcodes::Pop,
                opcodes::Duplicate,
                opcodes::Swap,
                opcodes::CopyOver,
                opcodes::Load,
                opcodes::Store,
                opcodes::Jump,
                opcodes::JumpEqual,
                opcodes::JumpNotEqual,
                opcodes::JumpGreater,
                opcodes::JumpGreaterEqual,
                opcodes::JumpLesser,
                opcodes::JumpLesserEqual,
                opcodes::Nop,
                opcodes::Halt
        >;


    }
}


#endif //LITTLEVIRTUALMACHINE_INTERPRETER_OPCODES_H