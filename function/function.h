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
    using destroy_fn_t = void(*)(void*);
    using copy_fn_t = void*(*)(const void*, void*);
    using move_fn_t = void*(*)(void*, void*);

    invoke_fn_t invoke_fn;
    destroy_fn_t destroy_fn;
    copy_fn_t copy_fn;
    move_fn_t move_fn;

    template <typename Functor>
    static Ret invoker(Functor* func, Args&&... args)
    {
        return std::invoke(*func, std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void destroyer(Functor* fptr)
    {
        if constexpr (sizeof(Functor) > BUFFER_SIZE)
        {
            delete fptr;
        }
        else
        {
            fptr->~Functor();
        }
    }

    template <typename Functor>
    static void* copier(const void* source, void* buffer)
    {
        if constexpr (sizeof(Functor) > BUFFER_SIZE)
        {
            return new Functor(*static_cast<const Functor*>(source));
        }
        else
        {
            new(buffer) Functor(*static_cast<const Functor*>(source));
            return buffer;
        }
    }

    template <typename Functor>
    static void* mover(void* source, void* buffer)
    {
        auto src_functor = static_cast<Functor*>(source);

        if constexpr (sizeof(Functor) > BUFFER_SIZE)
        {
            auto new_functor = new Functor(std::move(*src_functor));
            delete src_functor;
            return new_functor;
        }
        else
        {
            new(buffer) Functor(std::move(*src_functor));
            src_functor->~Functor();
            return buffer;
        }
    }

    void clear() {
        if (_fptr) {
            destroy_fn(_fptr);
            _fptr = nullptr;
            invoke_fn = nullptr;
            destroy_fn = nullptr;
            copy_fn = nullptr;
            move_fn = nullptr;
        }
    }
public:
    function()
        : _fptr(nullptr)
        , invoke_fn(nullptr)
        , destroy_fn(nullptr)
        , copy_fn(nullptr)
        , move_fn(nullptr) {}

    template <typename Functor>
    function(Functor func)
        : _fptr(&func)
        , invoke_fn(reinterpret_cast<invoke_fn_t>(&invoker<Functor>))
        , destroy_fn(reinterpret_cast<destroy_fn_t>(&destroyer<Functor>))
        , copy_fn(reinterpret_cast<copy_fn_t>(&copier<Functor>))
        , move_fn(reinterpret_cast<move_fn_t>(&mover<Functor>))
    {
        if constexpr (sizeof(Functor) > BUFFER_SIZE)
        {
            _fptr = new Functor(func);
        }
        else
        {
            new(_buffer) Functor(func);
            _fptr = _buffer;
        }
    }

    function(const function& other)
        : _fptr(nullptr)
        , invoke_fn(other.invoke_fn)
        , destroy_fn(other.destroy_fn)
        , copy_fn(other.copy_fn)
        , move_fn(other.move_fn)
    {
        if (other._fptr)
        {
            _fptr = copy_fn(other._fptr, _buffer);
        }
    }

    function(function&& other) noexcept
        : _fptr(nullptr)
        , invoke_fn(other.invoke_fn)
        , destroy_fn(other.destroy_fn)
        , copy_fn(other.copy_fn)
        , move_fn(other.move_fn)
    {
        if (other._fptr)
        {
            _fptr = move_fn(other._fptr, _buffer);
            other.clear();
        }
    }

    function& operator=(const function& other) {
        if (this != &other)
        {
            clear();
            invoke_fn = other.invoke_fn;
            destroy_fn = other.destroy_fn;
            copy_fn = other.copy_fn;
            move_fn = other.move_fn;
            if (other._fptr)
            {
                _fptr = copy_fn(other._fptr, _buffer);
            }
        }
        return *this;
    }

    function& operator=(function&& other) noexcept {
        if (this != &other)
        {
            clear();
            invoke_fn = other.invoke_fn;
            destroy_fn = other.destroy_fn;
            copy_fn = other.copy_fn;
            move_fn = other.move_fn;
            if (other._fptr)
            {
                _fptr = move_fn(other._fptr, _buffer);
                other.clear();
            }
        }
        return *this;
    }

    ~function()
    {
        clear();
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
