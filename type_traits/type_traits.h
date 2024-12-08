#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H
#include <iostream>
#include <type_traits>

template <typename T>
struct type_identity
{
  using type = T;
};

template <typename T, T x>
struct intagral_constant
{
  static constexpr T value = x;
};

template <bool b>
using bool_constant = intagral_constant<bool, b>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T>
struct is_lvalue_reference : false_type {};

template <typename T>
struct is_lvalue_reference<T&> : true_type {};

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <bool Cond, typename TrueType, typename FalseType>
struct conditional : true_type {};

template <typename TrueType, typename FalseType>
struct conditional<false, TrueType, FalseType> : false_type {};

template <bool Cond, typename TrueType, typename FalseType>
using conditional_t = typename conditional<Cond, TrueType, FalseType>::type;

template <typename...>
struct conjunction : true_type {};

template <typename T>
struct conjunction<T> : T {};

template <typename T, typename... Ts>
struct conjunction<T, Ts...>
  : conditional<bool(T::value), conjunction<Ts...>, T> {};

// SFINAE and std::enable_if
// Substitution Failure Is Not A Error works only in declaration(compiler instanciate type only in declaration)
// Overloading by the returning type doesn't work
/*
template <typename T>
auto f(T) -> typename T::value_type
{
  std::cout << 1 << '\n';
  return {};
}

template <typename... T>
auto f(T...)
{
  std::cout << 2 << '\n';
}
*/
template <typename T>
struct is_integral : bool_constant<
  requires (T t, T* p, void (*f)(T))
  {
    reinterpret_cast<T>(t);
    f(0);
    p + t;
  }> {};

template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T>
{
  using type = T;
};

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <typename T, enable_if_t<std::is_integral_v<T>, bool> = true>
auto f(T) { std::cout << 1 << '\n'; }

template <typename T, enable_if_t<!std::is_integral_v<T>, bool> = true>
auto f(T) { std::cout << 2 << '\n'; }



namespace detail
{
  template <typename T>
  intagral_constant<bool, !std::is_union_v<T>> test(int T::*);

  template <typename>
  false_type test(...);
}

template <typename T>
struct is_class : decltype(detail::test<T>(nullptr)) {};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

namespace detail
{
  template <typename T>
  true_type detect_is_polymorphic(
    decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr)))
  );
  template <typename>
  false_type detect_is_polymorphic(...);
}

template <typename T>
struct is_polymorphic : decltype(detail::detect_is_polymorphic<T>(nullptr)) {};

#endif //TYPE_TRAITS_H
