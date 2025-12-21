#pragma once

#include <algorithm>
#include <memory>
#include <vector>

namespace Blanketmen {

template<typename T, typename TAllocator = std::allocator<T>>
using List = std::vector<T, TAllocator>;

template<typename Container>
concept SequenceContainer = requires(Container c)
{
    { c.begin() } -> std::same_as<typename Container::iterator>;
    { c.end() } -> std::same_as<typename Container::iterator>;
    { c.erase(c.begin()) } -> std::same_as<typename Container::iterator>;
};

template<typename T, SequenceContainer Container>
    requires std::same_as<typename Container::value_type, T>
inline bool Remove(Container& container, const T& item)
{
    auto it = std::find(container.begin(), container.end(), item);
    if (it != container.end())
    {
        container.erase(it);
        return true;
    }
    return false;
}

} // namespace Blanketmen