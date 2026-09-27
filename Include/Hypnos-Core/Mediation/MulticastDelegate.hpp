#pragma once

#include "Delegate.hpp"
#include <cassert>
#include <vector>

namespace Blanketmen {

template<class... Args>
class MulticastDelegate
{
public:
    MulticastDelegate(size_t cap = 8)
    {
        delegates.reserve(cap);
    }

    void operator()(Args... args)
    {
        ++invokeDepth;
        for (size_t i = 0; i < delegates.size(); ++i)
        {
            delegates[i](args...);
        }
        --invokeDepth;
    }

    template<void(*F)(Args...)>
    void Register() { Register(Delegate<Args...>::template Bind<F>()); }

    template<class T, void(T::* M)(Args...)>
    void Register(T* obj) { Register(Delegate<Args...>::template Bind<T, M>(obj)); }

    template<class T, void(T::* M)(Args...) const>
    void Register(const T* obj) { Register(Delegate<Args...>::template Bind<T, M>(obj)); }

    template<void(*F)(Args...)>
    void Unregister() { Unregister(Delegate<Args...>::template Bind<F>()); }

    template<class T, void(T::* M)(Args...)>
    void Unregister(T* obj) { Unregister(Delegate<Args...>::template Bind<T, M>(obj)); }

    template<class T, void(T::* M)(Args...) const>
    void Unregister(const T* obj) { Unregister(Delegate<Args...>::template Bind<T, M>(obj)); }

    void Clear()
    {
        invokeDepth = 0;
        delegates.clear();
    }

private:
    void Register(const Delegate<Args...>& dlg)
    {
        assert(invokeDepth == 0 && "[MulticastDelegate] Cannot register delegate during invocation");
        assert(static_cast<bool>(dlg) && "[MulticastDelegate] Trying to bind an unbound delegate");
        for (auto& d : delegates)
        {
            if (d == dlg)
            {
                return;
            }
        }
        
        delegates.push_back(dlg);
    }

    void Unregister(const Delegate<Args...>& dlg)
    {
        assert(invokeDepth == 0 && "[MulticastDelegate] Cannot unbind delegate during invocation");
        assert(static_cast<bool>(dlg) && "[MulticastDelegate] Trying to unbind an unbound delegate");
        for (auto it = delegates.begin(); it != delegates.end(); ++it)
        {
            if (*it == dlg)
            {
                delegates.erase(it);
                return;
            }
        }
    }

    uint32 invokeDepth = 0;
    std::vector<Delegate<Args...>> delegates;
};

} // namespace Blanketmen