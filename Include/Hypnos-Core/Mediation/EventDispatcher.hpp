#pragma once

#include "Hypnos-Core/Container/UnorderedMap.hpp"

namespace Blanketmen {
namespace Hypnos {

class EventHandlerBase
{
public:
    virtual ~EventHandlerBase() = default;
    virtual void Handle() = 0;
};

template<typename... T>
class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual void Handle(T&&... args) = 0;
};

template<typename TKey>
class EventDispatcherBase
{
public:
    virtual ~EventDispatcherBase() = default;

    void Register(TKey id, EventHandlerBase* handler)
    {
        handlerMap[id] = handler;
    }

    void Unregister(TKey id)
    {
        handlerMap.erase(id);
    }

    void Dispatch(TKey id)
    {
        auto it = handlerMap.find(id);
        if (it != handlerMap.end())
        {
            it->second.Handle(id);
        }
    }

private:
    Container::UnorderedMap<TKey, EventHandlerBase&> handlerMap;
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

    void Dispatch(TKey id, TArgs&&... args)
    {
        auto it = handlerMap.find(id);
        if (it != handlerMap.end())
        {
            it->second.Handle(std::forward<TArgs>(args)...);
        }
    }

private:
    Container::UnorderedMap<TKey, EventHandler<TArgs...>&> handlerMap;
};

} // namespace Hypnos
} // namespace Blanketmen