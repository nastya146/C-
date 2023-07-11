#ifndef Type_VECTOR_H
#define Type_VECTOR_H
#include <iostream>
#include <cstdint>
#include <type_traits>

template <class... Args>
struct TypeVector;

// Size
template <class... Args>
struct Size;

template <>
struct Size<TypeVector<>> {
  static constexpr int64_t kValue = 0;
};

template <class T>
struct Size<TypeVector<T>> {
  static constexpr int64_t kValue = 1;
};

template <class T, class... Args>
struct Size<TypeVector<T, Args...>> {
  static constexpr int64_t kValue = Size<TypeVector<Args...>>::kValue + 1;
};

template <class TV>
inline constexpr int64_t kSizeV = Size<TV>::kValue;

// Empty
template <class TV>
inline constexpr bool kEmptyV = (kSizeV<TV> == 0);

// Front
template <class... Args>
struct Front;

template <class T, class... Args>
struct Front<TypeVector<T, Args...>> {
  using Type = T;
};

template <>
struct Front<TypeVector<>> {};

template <class TV>
using FrontT = typename Front<TV>::Type;

// Back
template <class... Args>
struct Back;

template <class T, class... Args>
struct Back<TypeVector<T, Args...>> : Back<TypeVector<Args...>> {};

template <class T>
struct Back<TypeVector<T>> {
  using Type = T;
};

template <>
struct Back<TypeVector<>>;

template <class TV>
using BackT = typename Back<TV>::Type;

// At

template <class TV, int Idx>
struct At;

template <class TV, int Idx>
struct At : At<TV, Idx - 1> {};

template <class TV>
struct At<TV, 0> {
  using Type = FrontT<TV>;
};

// MyAt[Tv, Idx]

template <int Idx, class... Args>
struct MyAt;

template <int Idx, class T, class... Args>
struct MyAt<Idx, TypeVector<T, Args...>> : MyAt<Idx - 1, TypeVector<Args...>> {};

template <class T, class... Args>
struct MyAt<0, TypeVector<T, Args...>> {
  using Type = T;
};

template <int Idx>
struct MyAt<Idx, TypeVector<>>;

template <int Idx>
struct At<TypeVector<>, Idx>;

template <class TV, int Idx>
using AtT = typename MyAt<Idx, TV>::Type;

// Contains
template <class T, class Head, class... Args>
struct Contains;

template <class T, class Head, class... Args>
struct Contains<T, TypeVector<Head, Args...>> {
  static constexpr bool kValue = std::is_same_v<T, Head> ? true : Contains<T, TypeVector<Args...>>::kValue;
};

template <class T, class Head>
struct Contains<T, TypeVector<Head>> {
  static constexpr bool kValue = std::is_same_v<T, Head>;
};

template <class T>
struct Contains<T, TypeVector<>> {
  static constexpr bool kValue = false;
};

template <class TV, class T>
inline constexpr bool kContainsV = Contains<T, TV>::kValue;

// Count

template <class T, class Head, class... Args>
struct Count;

template <class T, class Head, class... Args>
struct Count<T, TypeVector<Head, Args...>> {
  static constexpr int kValue =
      (std::is_same_v<T, Head> ? Count<T, TypeVector<Args...>>::kValue + 1 : Count<T, TypeVector<Args...>>::kValue);
};

template <class T, class Head>
struct Count<T, TypeVector<Head>> {
  static constexpr int kValue = (std::is_same_v<T, Head> ? 1 : 0);
};

template <class T>
struct Count<T, TypeVector<>> {
  static constexpr int kValue = 0;
};

template <class TV, class T>
inline constexpr int kCountV = Count<T, TV>::kValue;

// PushFrontT

template <class T, class... Args>
struct PushFront;

template <class T, class... Args>
struct PushFront<T, TypeVector<Args...>> {
  using Type = TypeVector<T, Args...>;
};

template <class TV, class T>
using PushFrontT = typename PushFront<T, TV>::Type;

// PushBack
template <class T, class... Args>
struct PushBack;

template <class T, class... Args>
struct PushBack<T, TypeVector<Args...>> {
  using Type = TypeVector<Args..., T>;
};

template <class TV, class T>
using PushBackT = typename PushBack<T, TV>::Type;

// PopFrontT

template <class... Args>
struct PopFront;

template <class T, class... Args>
struct PopFront<TypeVector<T, Args...>> {
  using Type = TypeVector<Args...>;
};

template <>
struct PopFront<TypeVector<>> {};

template <class TV>
using PopFrontT = typename PopFront<TV>::Type;

// PopBack
template <class... Args>
struct PopBack;

template <class T, class... Args>
struct PopBack<TypeVector<T, Args...>> {
  using Type = PushFrontT<typename PopBack<TypeVector<Args...>>::Type, T>;
};

template <class T>
struct PopBack<TypeVector<T>> {
  using Type = TypeVector<>;
};

template <>
struct PopBack<TypeVector<>> {};

template <class TV>
using PopBackT = typename PopBack<TV>::Type;

// Concatenate
template <class TV1, class TV2>
struct Concatenate;

template <class TV1, class TV2>
struct Concatenate {
  using Type = typename Concatenate<PushBackT<TV1, FrontT<TV2>>, PopFrontT<TV2>>::Type;
};

template <class TV1, class T>
struct Concatenate<TV1, TypeVector<T>> {
  using Type = PushBackT<TV1, T>;
};

template <class TV1>
struct Concatenate<TV1, TypeVector<>> {
  using Type = TV1;
};

template <class TV1, class TV2>
using ConcatenateT = typename Concatenate<TV1, TV2>::Type;

#endif