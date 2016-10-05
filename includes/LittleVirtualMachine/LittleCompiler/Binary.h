//
// Created by kaspersk on 9/25/16.
//

#ifndef LECTURE07_08_BINARY_H
#define LECTURE07_08_BINARY_H

namespace exercise02 {
    template<size_t T>
    struct isBinary;

    template<>
    struct isBinary<1> {
        static const size_t value = 1;
    };

    template<>
    struct isBinary<0> {
        static const size_t value = 0;
    };

    template<size_t N>
    struct Binary {
        static const size_t value = Binary<N/10>::value << 1 | isBinary<N%10>::value;
    };

    template<>
    struct Binary<0> {
        static const size_t value = 0;
    };
}

#endif //LECTURE07_08BINARY_H
