#ifndef PRIME_H
#define PRIME_H
#include <iostream>
#include <type_traits>

// sqrt
template <int64_t L, int64_t R, int64_t N, typename Enable = void>
struct Root;

template <int64_t N>
inline constexpr int64_t kSqrtV = Root<0, N, N>::value;

template <int64_t L, int64_t R, int64_t N>
struct Root<L, R, N, std::enable_if_t<(R > 1'000'000'000)>> : Root<L, 1'000'000'000, N> {};

template <int64_t L, int64_t R, int64_t N>
struct Root<L, R, N, std::enable_if_t<(((L + R) / 2) * ((L + R) / 2) == N)>>
    : std::integral_constant<int64_t, (L + R) / 2> {};

template <int64_t L, int64_t R, int64_t N>
struct Root<L, R, N, std::enable_if_t<(((L + R) / 2) * ((L + R) / 2) > N)>> : Root<L, (L + R) / 2 - 1, N> {};

template <int64_t L, int64_t R, int64_t N>
struct Root<L, R, N,
            std::enable_if_t<(((L + R) / 2) * ((L + R) / 2) < N && ((L + R) / 2 + 1) * ((L + R) / 2 + 1) <= N)>>
    : Root<(L + R) / 2 + 1, R, N> {};

template <int64_t L, int64_t R, int64_t N>
struct Root<L, R, N, std::enable_if_t<(((L + R) / 2) * ((L + R) / 2) < N && ((L + R) / 2 + 1) * ((L + R) / 2 + 1) > N)>>
    : std::integral_constant<int64_t, (L + R) / 2> {};

template <size_t N, size_t L, size_t R, bool T>
struct HasDivisorOnV;

template <size_t N, size_t L, size_t R>
struct HasDivisorOnV<N, L, R, true>
    : std::integral_constant<bool,
                             std::disjunction<HasDivisorOnV<N, L, (L + R) / 2, ((L + R) / 2 > L)>,
                                              HasDivisorOnV<N, (L + R) / 2 + 1, R, ((L + R) / 2 + 1 < R)>>::value> {};

template <size_t N, size_t L, size_t R>
struct HasDivisorOnV<N, L, R, false> : std::true_type {};

template <size_t N, size_t L>
struct HasDivisorOnV<N, L, L, false> : std::integral_constant<bool, N % L == 0> {};

template <size_t N, size_t L, size_t R>
static const bool kHasDivisorOnV = HasDivisorOnV<N, L, R, (L < R)>::value;

// is prime
template <int64_t N, typename Enable = void>
struct IsPrime;

template <int64_t N>
inline constexpr int64_t kIsPrimeV = IsPrime<N>::value;

template <int64_t N>
struct IsPrime<N, std::enable_if_t<N == 1>> : std::false_type {};

template <int64_t N>
struct IsPrime<N, std::enable_if_t<N == 2 || N == 3>> : std::true_type {};

template <int64_t N>
struct IsPrime<N, std::enable_if_t<(N > 3)>> : std::integral_constant<bool, !kHasDivisorOnV<N, 2, kSqrtV<N>>> {};
#endif