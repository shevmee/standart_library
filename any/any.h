#ifndef ANY_H
#define ANY_H

#include <any>
#include <utility>

class any
{
private:
    struct Base
    {
        [[nodiscard]] virtual Base* getCopy() const = 0;
        virtual ~Base() = default;
    };

    template <typename T>
    struct Derived : Base
    {
        T _value;
        explicit Derived(const T& value) : _value(value) {}
        explicit Derived(T&& value) : _value(std::move(value)) {}
        Derived* getCopy() const override
        {
            return new Derived(_value);
        }
    };

    Base* _ptr;
public:
    template <typename T>
    explicit any(const T& value) : _ptr(new Derived<T>(value)) {}

    any(const any& other) : _ptr(other._ptr->getCopy()) {}

    any(any&& other) noexcept : _ptr(other._ptr)
    {
        other._ptr = nullptr;
    }

    template <typename T>
    any& operator= (const T& other)
    {
        delete _ptr;
        _ptr = new Derived<T>(other);
        return *this;
    }

    any& operator= (T&& other) noexcept
    {
        delete _ptr;
        _ptr = new Derived<T>(std::move(other));
        return *this;
    }

    ~any()
    {
        delete _ptr;
    }
};

template <typename T>
T any(any& a)
{
    auto* p = dynamic_cast<any::Derived<std::remove_reference_t<T>>*>(a._ptr);
    if (!p)
    {
        return std::bad_any_cast();
    }
    return p->_value;
}
#endif //ANY_H
