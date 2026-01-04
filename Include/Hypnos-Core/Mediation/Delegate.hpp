#pragma once

#include <cassert>

namespace Blanketmen {

template<class... Args>
class Delegate
{
public:
    template<void(*F)(Args...)>
    static Delegate Bind() { return Delegate(nullptr, &InvokeFunction<F>); }

    template<class T, void(T::* M)(Args...)>
    static Delegate Bind(T* obj) { return Delegate(static_cast<void*>(obj), &InvokeMethod<T, M>); }

    template<class T, void(T::* M)(Args...) const>
    static Delegate Bind(const T* obj) { return Delegate { const_cast<T*>(obj), &InvokeConstMethod<T, M> }; }

    Delegate() = default;

    explicit operator bool() const { return stub != nullptr; }

    bool operator==(const Delegate& other) const { return ctx == other.ctx && stub == other.stub; }

    void operator()(Args... args) const
    {
        if (stub != nullptr)
        {
            stub(ctx, std::forward<Args>(args)...);
        }
    }

    template<void(*F)(Args...)>
    void Register()
    {
        ctx = nullptr;
        stub = &InvokeFunction<F>;
    }

    template<class T, void(T::* M)(Args...)>
    void Register(T* obj)
    {
        ctx = obj;
        stub = &InvokeMethod<T, M>;
    }

    template<class T, void(T::* M)(Args...) const>
    void Register(const T* obj)
    {
        ctx = const_cast<T*>(obj);
        stub = &InvokeConstMethod<T, M>;
    }

    void Unregister()
    {
        ctx = nullptr;
        stub = nullptr;
    }

private:
    template<void(*F)(Args...)>
    static void InvokeFunction(void*, Args... args)
    {
        F(std::forward<Args>(args)...);
    }

    template<class T, void(T::* M)(Args...)>
    static void InvokeMethod(void* ctx, Args... args)
    {
        auto* obj = static_cast<T*>(ctx);
        (obj->*M)(std::forward<Args>(args)...);
    }

    template<class T, void(T::* M)(Args...) const>
    static void InvokeConstMethod(void* ctx, Args... args)
    {
        auto* obj = static_cast<const T*>(ctx);
        (obj->*M)(std::forward<Args>(args)...);
    }

    using Stub = void(*)(void* ctx, Args...);

    Delegate(void* ctx, Stub stub) : ctx(ctx), stub(stub) { }

    void* ctx = nullptr;
    Stub stub = nullptr;
};

} // namespace Blanketmen