#pragma once

#include "Hypnos-Core/Container/UnorderedMap.hpp"

// TODO: Decouple from Container::UnorderedMap.
namespace Blanketmen {
namespace Hypnos {

template<typename... T>
class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual void Handle(T... args) = 0;
};

template<typename TKey, typename... TArgs>
class EventDispatcher
{
public:
    virtual ~EventDispatcher() = default;

    void Register(TKey id, EventHandler<TArgs...>* handler)
    {
        handlerMap[id] = handler;
    }

    void Unregister(TKey id)
    {
        handlerMap.erase(id);
    }

protected:
    void Dispatch(TKey id, TArgs... args) const
    {
        auto it = handlerMap.find(id);
        if (it != handlerMap.end())
        {
            it->second->Handle(std::forward<TArgs>(args)...);
        }
    }

private:
    Container::UnorderedMap<TKey, EventHandler<TArgs...>*> handlerMap;
};

} // namespace Hypnos
} // namespace Blanketmen