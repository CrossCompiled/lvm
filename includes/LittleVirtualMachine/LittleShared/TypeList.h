//
// Created by kaspersk on 9/25/16.
//

#ifndef LECTURE07_08_TYPELIST_H
#define LECTURE07_08_TYPELIST_H

#include <typeinfo>


namespace exercise03 {

    struct NullType {
    };

    template<typename L, typename R>
    struct TypeList {
        typedef L First;
        typedef R Left;
    };

    template <typename L, typename R>
    struct IsSame{
        static const bool value = false;
    };

    template <typename L>
    struct IsSame<L,L>{
        static const bool value = true;
    };

    template <typename TL, typename T = NullType>
    struct Contains{
        static const bool value = IsSame<typename TL::First, T>::value ? true : Contains<typename TL::Left, T>::value;
    };

    template <typename T>
    struct Contains<NullType, T>{
        static const bool value = false;
    };

    template <typename TL, size_t index>
    struct AtIndex{
        typedef typename AtIndex<typename TL::Left, index-1>::type type;
    };

    template <typename TL>
    struct AtIndex<TL,0>{
        typedef typename TL::First type;
    };

    template <typename TL>
    struct PrintIt{
        static std::string value() {return std::string(typeid(typename TL::First).name()) + " " + PrintIt<typename TL::Left>::value(); }
    };

    template <>
    struct PrintIt<NullType> {
        static std::string value() { return "";}
    };

    template <class TList, class T> struct Remove;

    template <typename T1, typename T2, typename T>
    struct Remove<TypeList<T1, T2>, T>{
        typedef TypeList<T1, typename Remove<T2,T>::type> type;
    };

    template <typename T, typename T2>
    struct Remove<TypeList<T, T2>, T>{
        typedef typename Remove<T2, T>::type type;
    };

    template <typename T>
    struct Remove<NullType, T>{
        typedef NullType type;
    };


#define TYPELIST_1(T1) TypeList<T1, NullType>
#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4) TypeList<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5) TypeList<T1, TYPELIST_4(T2, T3, T4, T5)>
#define TYPELIST_6(T1, T2, T3, T4, T5, T6) TypeList<T1, TYPELIST_5(T2, T3, T4, T5, T6)>
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) TypeList<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7)>
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) TypeList<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8)>
#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) TypeList<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9)>
#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) TypeList<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10)>

//OR!
    template<typename T1, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType>
    struct MakeTypeList {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, TypeList<T5, TypeList<T6, TypeList<T7, TypeList<T8, TypeList<T9, TypeList<T10, NullType>>>>>>>>>> type;
    };

    template<typename T1>
    struct MakeTypeList<T1, NullType, NullType, NullType, NullType, NullType, NullType, NullType, NullType, NullType> {
        typedef TypeList<T1, NullType> type;
    };

    template<typename T1, typename T2>
    struct MakeTypeList<T1, T2, NullType, NullType, NullType, NullType, NullType, NullType, NullType, NullType> {
        typedef TypeList<T1, TypeList<T2, NullType>> type;
    };

    template<typename T1, typename T2, typename T3>
    struct MakeTypeList<T1, T2, T3, NullType, NullType, NullType, NullType, NullType, NullType, NullType> {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, NullType>>> type;
    };

    template<typename T1, typename T2, typename T3, typename T4>
    struct MakeTypeList<T1, T2, T3, T4, NullType, NullType, NullType, NullType, NullType, NullType> {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, NullType>>>> type;
    };

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    struct MakeTypeList<T1, T2, T3, T4, T5, NullType, NullType, NullType, NullType, NullType> {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, TypeList<T5, NullType>>>>> type;
    };

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    struct MakeTypeList<T1, T2, T3, T4, T5, T6, NullType, NullType, NullType, NullType> {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, TypeList<T5, TypeList<T6, NullType>>>>>> type;
    };

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    struct MakeTypeList<T1, T2, T3, T4, T5, T6, T7, NullType, NullType, NullType> {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, TypeList<T5, TypeList<T6, TypeList<T7, NullType>>>>>>> type;
    };

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    struct MakeTypeList<T1, T2, T3, T4, T5, T6, T7, T8, NullType, NullType> {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, TypeList<T5, TypeList<T6, TypeList<T7, TypeList<T8, NullType>>>>>>>> type;
    };

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    struct MakeTypeList<T1, T2, T3, T4, T5, T6, T7, T8, T9, NullType> {
        typedef TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, TypeList<T5, TypeList<T6, TypeList<T7, TypeList<T8, TypeList<T9, NullType>>>>>>>>> type;
    };


}
#endif //LECTURE07_08_TYPELIST_H
