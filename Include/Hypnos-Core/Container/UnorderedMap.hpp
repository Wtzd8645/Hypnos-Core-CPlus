#pragma once

#include "Hypnos-Core/Allocator.hpp"
#include <unordered_map>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class TKey, class TValue>
using UnorderedMap = std::unordered_map<TKey, TValue, std::hash<TKey>, std::equal_to<TKey>, MemoryPoolAllocator<std::pair<const TKey, TValue>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen