#pragma once

namespace Blanketmen {
namespace Hypnos {

template<typename... T>
class EventHandlerBase
{
public:
    virtual ~EventHandlerBase() = default;

    virtual void Handle(T... args) = 0;
};

} // namespace Hypnos
} // namespace Blanketmen