//
// Created by kaspersk on 9/25/16.
//

#ifndef LECTURE07_08_TYPELIST_H
#define LECTURE07_08_TYPELIST_H

#include <typeinfo>


namespace lvm {
    namespace shared {

        struct NullType {
        };

        template<typename L, typename R>
        struct TypeList {
            typedef L First;
            typedef R Left;
        };

        template<typename L, typename R>
        struct IsSame {
            static const bool value = false;
        };

        template<typename L>
        struct IsSame<L, L> {
            static const bool value = true;
        };

        template<typename TL, typename T = NullType>
        struct Contains {
            static const bool value = IsSame<typename TL::First, T>::value ? true : Contains<typename TL::Left, T>::value;
        };

        template<typename T>
        struct Contains<NullType, T> {
            static const bool value = false;
        };

        template<typename TL, size_t index>
        struct AtIndex {
            typedef typename AtIndex<typename TL::Left, index - 1>::type type;
        };

        template<typename TL>
        struct AtIndex<TL, 0> {
            typedef typename TL::First type;
        };

        template<typename TL>
        struct PrintIt {
            static std::string value() {
                return std::string(typeid(typename TL::First).name()) + " " + PrintIt<typename TL::Left>::value();
            }
        };

        template<>
        struct PrintIt<NullType> {
            static std::string value() { return ""; }
        };

        template<class TList, class T>
        struct Remove;

        template<typename T1, typename T2, typename T>
        struct Remove<TypeList<T1, T2>, T> {
            typedef TypeList<T1, typename Remove<T2, T>::type> type;
        };

        template<typename T, typename T2>
        struct Remove<TypeList<T, T2>, T> {
            typedef typename Remove<T2, T>::type type;
        };

        template<typename T>
        struct Remove<NullType, T> {
            typedef NullType type;
        };

        template<typename... Args>
        struct MakeTypeList;

        template <typename Head>
        struct MakeTypeList<Head> {
            typedef TypeList<Head, NullType> List;
        };

        template <typename Head, typename... Tail>
        struct MakeTypeList<Head, Tail...> {
            typedef TypeList<Head, typename MakeTypeList<Tail...>::List> List;
        };

    }
}
#endif //LECTURE07_08_TYPELIST_H
