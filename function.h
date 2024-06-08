#ifndef FUNCTION_H
#define FUNCTION_H
#include <cstddef>
#include <functional>

template <typename T>
class function;

template <typename Ret, typename... Args>
class function<Ret(Args...)>
{
private:
    static const size_t BUFFER_SIZE = 16;
    alignas(max_align_t) char _buffer[BUFFER_SIZE];
    void* _fptr;

    using invoke_fn_t = Ret(*)(void*, Args&&...);
    using construct_fn_t = void(*)(void*, void*);
    using destroy_fn_t = void(*)(void*);

    invoke_fn_t invoke_fn;
    construct_fn_t construct_fn;
    destroy_fn_t destroy_fn;

    template <typename Functor>
    static Ret invoker(void* func, Args&&... args)
    {
        return std::invoke(*static_cast<Functor*>(func), std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void constructor(void* dst, void* src)
    {
        Functor* srcFunc = static_cast<Functor*>(src);
        if constexpr (sizeof(Functor) > BUFFER_SIZE)
        {
            *static_cast<void**>(dst) = new Functor(std::move(*srcFunc));
        }
        else
        {
            new (dst) Functor(std::move(*srcFunc));
        }
    }

    template <typename Functor>
    static void destroyer(void* func)
    {
        Functor* f = static_cast<Functor*>(func);
        if constexpr (sizeof(Functor) > BUFFER_SIZE)
        {
            delete f;
        }
        else
        {
            f->~Functor();
        }
    }
public:
    function()
        : _fptr(nullptr)
        , invoke_fn(nullptr)
        , construct_fn(nullptr)
        , destroy_fn(nullptr) {}

    template <typename Functor>
    function(Functor func)
    {
        if constexpr (sizeof(Functor) > BUFFER_SIZE)
        {
            _fptr = new Functor(std::move(func));
        }
        else
        {
            new (_buffer) Functor(std::move(func));
            _fptr = _buffer;
        }
        invoke_fn = invoker<Functor>;
        construct_fn = constructor<Functor>;
        destroy_fn = destroyer<Functor>;
    }

    function(const function& other)
    {
        if (other._fptr)
        {
            other.construct_fn(_fptr, other._fptr);
            invoke_fn = other.invoke_fn;
            construct_fn = other.construct_fn;
            destroy_fn = other.destroy_fn;
        }
        else
        {
            _fptr = nullptr;
            invoke_fn = nullptr;
            construct_fn = nullptr;
            destroy_fn = nullptr;
        }

    }

    function(function&& other) noexcept
    {
        _fptr = other._fptr;
        invoke_fn = other.invoke_fn;
        construct_fn = other.construct_fn;
        destroy_fn = other.destroy_fn;

        other._fptr = nullptr;
        other.invoke_fn = nullptr;
        other.construct_fn = nullptr;
        other.destroy_fn = nullptr;
    }

    function& operator= (const function& other)
    {
        if (this != &other)
        {
            if (_fptr)
            {
                destroy_fn(_fptr);
            }
            if (other._fptr)
            {
                other.construct_fn(_fptr, other._fptr);
                invoke_fn = other.invoke_fn;
                construct_fn = other.construct_fn;
                destroy_fn = other.destroy_fn;
            }
            else
            {
                other._fptr = nullptr;
                other.invoke_fn = nullptr;
                other.construct_fn = nullptr;
                other.destroy_fn = nullptr;
            }
        }
        return *this;
    }

    function& operator= (function&& other) noexcept
    {
        if (this != &other)
        {
            if (_fptr)
            {
                destroy_fn(_fptr);
            }
            _fptr = other._fptr;
            invoke_fn = other.invoke_fn;
            construct_fn = other.construct_fn;
            destroy_fn = other.destroy_fn;

            other._fptr = nullptr;
            other.invoke_fn = nullptr;
            other.construct_fn = nullptr;
            other.destroy_fn = nullptr;
        }
        return *this;
    }


    ~function()
    {
        if (_fptr)
        {
            destroy_fn(_fptr);
        }
    }

    Ret operator() (Args&&... args) const
    {
        if (!_fptr)
        {
            throw std::bad_function_call();
        }
        return invoke_fn(_fptr, std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept
    {
        return _fptr != nullptr;
    }
};

#endif //FUNCTION_H
