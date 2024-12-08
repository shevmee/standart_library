#ifndef VARIANT_H
#define VARIANT_H

#include <stdexcept>
#include <new>
#include <type_traits>
#include <utility>
#include <algorithm>

// Helper to get the maximum size of types
template <typename... Types>
struct max_sizeof;

template <typename Head, typename... Tail>
struct max_sizeof<Head, Tail...>
{
    static const size_t value = std::max(sizeof(Head), max_sizeof<Tail...>::value);
};

template <typename T>
struct max_sizeof<T>
{
    static const size_t value = sizeof(T);
};

template <typename... Types>
const size_t max_sizeof_v = max_sizeof<Types...>::value;

// Forward declaration of variant class
template <typename... Types>
class variant;

// Helper to find the index of a type in a type list
template <typename T, typename... Types>
struct type_index;

template <typename T, typename Head, typename... Tail>
struct type_index<T, Head, Tail...>
{
    static const size_t value = std::is_same<T, Head>::value ? 0 : 1 + type_index<T, Tail...>::value;
};

template <typename T>
struct type_index<T>
{
    static const size_t value = -1; // Type not found
};

// Base class for all alternatives in the variant
template <typename T, typename... Types>
struct variant_alternative
{
    variant_alternative() {}
    ~variant_alternative() {}

    variant_alternative(const T& value)
    {
        auto* this_variant = static_cast<variant<Types...>*>(this);
        new (&this_variant->buffer) T(value);
        this_variant->active_index = type_index<T, Types...>::value;
    }

    variant_alternative(T&& value)
    {
        auto* this_variant = static_cast<variant<Types...>*>(this);
        new (&this_variant->buffer) T(std::move(value));
        this_variant->active_index = type_index<T, Types...>::value;
    }

    void destroy()
    {
        auto* this_variant = static_cast<variant<Types...>*>(this);
        if (this_variant->active_index == type_index<T, Types...>::value)
        {
            reinterpret_cast<T*>(&this_variant->buffer)->~T();
        }
    }

    void copy(const variant_alternative& other)
    {
        auto* this_variant = static_cast<variant<Types...>*>(this);
        if (other.active_index == type_index<T, Types...>::value)
        {
            new (&this_variant->buffer) T(*reinterpret_cast<const T*>(&other.buffer));
            this_variant->active_index = other.active_index;
        }
    }

    void move(variant_alternative&& other)
    {
        auto* this_variant = static_cast<variant<Types...>*>(this);
        if (other.active_index == type_index<T, Types...>::value)
        {
            new (&this_variant->buffer) T(std::move(*reinterpret_cast<T*>(&other.buffer)));
            this_variant->active_index = other.active_index;
            other.active_index = -1; // Invalidate other
        }
    }
};

// The variant class itself
template <typename... Types>
class variant : private variant_alternative<Types, Types...>...
{
public:
    using variant_alternative<Types, Types...>::variant_alternative...;

    template <typename U, typename = std::enable_if_t<!std::is_same_v<std::decay_t<U>, variant>>>
    variant(U&& value)
    {
        static_assert(type_index<std::decay_t<U>, Types...>::value != -1, "Type not in variant");
        new (&buffer) std::decay_t<U>(std::forward<U>(value));
        active_index = type_index<std::decay_t<U>, Types...>::value;
    }

    variant(const variant& other)
    {
        copy_variant(other);
    }

    variant(variant&& other) noexcept
    {
        move_variant(std::move(other));
    }

    ~variant()
    {
        destroy_variant();
    }

    variant& operator=(const variant& other)
    {
        if (this != &other)
        {
            destroy_variant();
            copy_variant(other);
        }
        return *this;
    }

    variant& operator=(variant&& other) noexcept
    {
        if (this != &other)
        {
            destroy_variant();
            move_variant(std::move(other));
        }
        return *this;
    }

    template <typename U>
    bool holds_alternative() const
    {
        return active_index == type_index<U, Types...>::value;
    }

    template <typename U>
    U& get()
    {
        if (!holds_alternative<U>())
        {
            throw std::bad_variant_access();
        }
        return *reinterpret_cast<U*>(&buffer);
    }

    template <typename U>
    const U& get() const
    {
        if (!holds_alternative<U>())
        {
            throw std::bad_variant_access();
        }
        return *reinterpret_cast<const U*>(&buffer);
    }

private:
    alignas(Types...) char buffer[max_sizeof_v<Types...>];
    size_t active_index = -1;

    void destroy_variant()
    {
        (variant_alternative<Types, Types...>::destroy(), ...);
    }

    void copy_variant(const variant& other)
    {
        (variant_alternative<Types, Types...>::copy(other), ...);
    }

    void move_variant(variant&& other)
    {
        (variant_alternative<Types, Types...>::move(std::move(other)), ...);
    }

    template <typename T, typename... Ts>
    friend struct variant_alternative;
};

#endif // VARIANT_H
