#pragma once

#include "EventHandlerBase.hpp"
#include <unordered_map>

namespace Blanketmen {
namespace Hypnos {

template<typename TKey, typename... TArgs>
class EventDispatcher
{
public:
    virtual ~EventDispatcher() = default;

    void Register(TKey id, EventHandlerBase<TArgs...>* handler)
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
    std::unordered_map<TKey, EventHandlerBase<TArgs...>*> handlerMap;
};

} // namespace Hypnos
} // namespace Blanketmen