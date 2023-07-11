#ifndef I_TRAITS_H_
#define I_TRAITS_H_
#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <forward_list>
#include <iterator>

// 1) kIsDereferenceable
template <class T, typename = std::enable_if_t<!std::is_void<T>::value>>
decltype(*std::declval<T>(), std::true_type{}) Test1(int) noexcept;

template <class T>
std::false_type Test1(int64_t) noexcept;

template <class T>
struct IsDereferenceableV : decltype(Test1<T>(0)) {};

template <class T>
struct IsDereferenceableV<T*> : std::true_type {};

template <typename T>
inline const bool kIsDereferenceableV = IsDereferenceableV<T>::value;

// 2) kIsIncrementableV
template <class T>
decltype(++std::declval<T&>(), std::declval<T&>()++, std::true_type{}) Test2(int) noexcept;

template <class T>
std::false_type Test2(int64_t) noexcept;

template <class T>
struct IsIncrementableV : decltype(Test2<T>(0)) {};

template <typename T>
inline const bool kIsIncrementableV = IsIncrementableV<T>::value;

// 3) kIsDecrementable
template <class T>
decltype(--std::declval<T&>(), std::declval<T&>()--, std::true_type{}) Test3(int) noexcept;

template <class T>
std::false_type Test3(int64_t) noexcept;

template <class T>
struct IsDecrementableV : decltype(Test3<T>(0)) {};

template <typename T>
inline const bool kIsDecrementableV = IsDecrementableV<T>::value;

// 4) kIsArrowDereferenceable
template <class T>
decltype(std::declval<T>().operator->(), std::true_type{}) Test4(int) noexcept;

template <class T>
std::false_type Test4(int64_t) noexcept;

template <class T>
struct IsArrowDereferenceableV : decltype(Test4<T>(0)) {};

template <class T>
struct IsArrowDereferenceableV<T*> : std::true_type {};

template <typename T>
inline const bool kIsArrowDereferenceableV = IsArrowDereferenceableV<T>::value;

// 5) kIsEqualityComparable
template <class T>
decltype(static_cast<bool>(std::declval<T>() == std::declval<T>()), std::true_type{}) Test5(int) noexcept;

template <class T>
std::false_type Test5(int64_t) noexcept;

template <class T>
struct IsEqualityComparableV : decltype(Test5<T>(0)) {};

template <typename T>
inline const bool kIsEqualityComparableV = IsEqualityComparableV<T>::value;

// 6) kIsOrdered
template <class T>
decltype(static_cast<bool>(std::declval<T>() < std::declval<T>()), std::true_type{}) Test6(int) noexcept;

template <class T>
std::false_type Test6(int64_t) noexcept;

template <class T>
struct IsOrderedV : decltype(Test6<T>(0)) {};

template <typename T>
inline const bool kIsOrderedV = IsOrderedV<T>::value;

// 7) kIsSubtractableV

template <class T>
struct TypeIdentity {
  using Type = T;
};

template <class T>
decltype(std::declval<T>() - std::declval<T>(), std::true_type{}) Test7(int) noexcept;

template <class T>
std::false_type Test7(int64_t) noexcept;

template <class T>
struct IsSubtractableV : decltype(Test7<T>(0)) {};

template <typename T>
inline const bool kIsSubtractableV = IsSubtractableV<T>::value;

template <typename T>
using DifferenceType = decltype(std::declval<T>() - std::declval<T>());

// 8) kHasIntegralArithmeticV

template <class T, typename = std::enable_if_t<std::is_integral_v<DifferenceType<T>>>>
decltype(std::declval<T&>() += std::declval<DifferenceType<T>>(),
         std::declval<T&>() -= std::declval<DifferenceType<T>>(), std::true_type{})
Test8(int) noexcept;

template <class T>
std::false_type Test8(int64_t) noexcept;

template <class T>
struct HasIntegralArithmeticV : decltype(Test8<T>(0)) {};

template <typename T>
inline const bool kHasIntegralArithmeticV = HasIntegralArithmeticV<T>::value;

// 9) kIsSubscriptable
template <class T, typename = std::enable_if_t<std::is_integral_v<DifferenceType<T>>>>
decltype(std::declval<T>()[std::declval<std::ptrdiff_t>()], std::true_type{}) Test9(int) noexcept;

template <class T>
std::false_type Test9(int64_t) noexcept;

template <class T>
struct IsSubscriptableV : decltype(Test9<T>(0)) {};

template <typename T>
inline const bool kIsSubscriptableV = IsSubscriptableV<T>::value;

// 10) kIsIterator
template <class T,
          typename = std::enable_if_t<std::is_copy_constructible<T>::value && std::is_copy_assignable<T>::value &&
                                      std::is_destructible<T>::value && kIsDereferenceableV<T> && kIsIncrementableV<T>>>
std::true_type Test10(int) noexcept;

template <class T>
std::false_type Test10(int64_t) noexcept;

template <class T>
struct IsIteratorV : decltype(Test10<T>(0)) {};

template <typename T>
inline const bool kIsIteratorV = IsIteratorV<T>::value;

// 11) kIsInputIterator
template <class T,
          typename = std::enable_if_t<kIsIteratorV<T> && kIsEqualityComparableV<T> && kIsArrowDereferenceableV<T>>>
std::true_type Test11(int) noexcept;

template <class T>
std::false_type Test11(int64_t) noexcept;

template <class T>
struct IsInputIteratorV : decltype(Test11<T>(0)) {};

template <typename T>
inline const bool kIsInputIteratorV = IsInputIteratorV<T>::value;

// 12) kIsForwardIterator
template <class T, typename = std::enable_if_t<kIsInputIteratorV<T>>>
decltype(T(), std::true_type{}) Test12(int) noexcept;

template <class T>
std::false_type Test12(int64_t) noexcept;

template <class T>
struct IsForwardIteratorV : decltype(Test12<T>(0)) {};

template <typename T>
inline const bool kIsForwardIteratorV = IsForwardIteratorV<T>::value;

// 13) kIsBidirectionalIterator
template <class T, typename = std::enable_if_t<kIsForwardIteratorV<T> && kIsDecrementableV<T>>>
std::true_type Test13(int) noexcept;

template <class T>
std::false_type Test13(int64_t) noexcept;

template <class T>
struct IsBidirectionalIteratorV : decltype(Test13<T>(0)) {};

template <typename T>
inline const bool kIsBidirectionalIteratorV = IsBidirectionalIteratorV<T>::value;

// 14) kIsRandomAccessIterator
template <class T, typename = std::enable_if_t<kIsBidirectionalIteratorV<T> && kIsSubtractableV<T> &&
                                               kHasIntegralArithmeticV<T> && kIsSubscriptableV<T> && kIsOrderedV<T>>>
std::true_type Test14(int) noexcept;

template <class T>
std::false_type Test14(int64_t) noexcept;

template <class T>
struct IsRandomAccessIteratorV : decltype(Test14<T>(0)) {};

template <typename T>
inline const bool kIsRandomAccessIteratorV = IsRandomAccessIteratorV<T>::value;
#endif