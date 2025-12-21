#pragma once

namespace Blanketmen {

template<typename... T>
class IEventHandler
{
public:
    virtual ~IEventHandler() = default;

    virtual void Handle(const T&... args) = 0;
};

} // namespace Blanketmen