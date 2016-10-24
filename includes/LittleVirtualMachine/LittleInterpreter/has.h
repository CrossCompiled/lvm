//
// Created by Kalle Møller on 24/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_LITTLEINTERPRETER_HAS_H
#define LITTLEVIRTUALMACHINE_LITTLEINTERPRETER_HAS_H

#include <type_traits>

#define GENERATE_HAS(member)                                               \
                                                                                  \
template < class T >                                                              \
class has_##member ## _impl                                                          \
{                                                                                     \
    struct Fallback { int member; };                                                     \
    struct Derived : T, Fallback { };                                               \
                                                                                    \
    template<typename U, U> struct Check;                                           \
                                                                                    \
    typedef char InvalidMember[1];                                                     \
    typedef char ValidMember[2];                                                     \
                                                                                    \
    template<typename U> static InvalidMember & func(Check<int Fallback::*, &U::member> *); \
    template<typename U> static ValidMember & func(...);                             \
  public:                                                                           \
    static constexpr bool value = (sizeof(func<Derived>(0)) == 2);                       \
};  \
  \
template < class T >                                                              \
struct has_##member : public std::integral_constant<bool, has_##member ## _impl<T>::value> {};

GENERATE_HAS(resize);

GENERATE_HAS(data);

GENERATE_HAS(at);

GENERATE_HAS(push);

#endif //LITTLEVIRTUALMACHINE_LITTLEINTERPRETER_HAS_H
