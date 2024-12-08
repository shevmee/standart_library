#ifndef MOVE_H
#define MOVE_H
#include <type_traits>

template <typename T>
struct remove_reference
{
    using type = T;
};

template <typename T>
struct remove_reference<T&>
{
    using type = T;
};

template <typename T>
struct remove_reference<T&&>
{
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct is_lvalue_reference : std::false_type {};

template <typename T>
struct is_lvalue_reference<T&> : std::true_type {};

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <typename T>
constexpr remove_reference_t<T>&& move(T&& value) noexcept
{
    return static_cast<remove_reference_t<T>&&>(value);
}



template <typename T>
constexpr typename std::conditional_t<
    !std::is_nothrow_move_constructible_v<T> && std::is_copy_constructible_v<T>,
    const T&,
    T&&
    > move_if_noexcept(T& value) noexcept
{
    return move(value);
}

template <typename T>
constexpr T&& forward(remove_reference<T>& value) noexcept
{
    return static_cast<T&&>(value);
}

template <typename T>
constexpr T&& forward(remove_reference_t<T>&& value) noexcept {
    static_assert(!is_lvalue_reference_v<T>, "bad forward call");
    return static_cast<T&&>(value);
}

#endif //MOVE_H
