#pragma once

#include "Hypnos-Core/Memory/MemoryPoolAllocator.hpp"
#include <unordered_map>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename TKey, typename TValue>
using UnorderedMap = std::unordered_map<TKey, TValue, std::hash<TKey>, std::equal_to<TKey>, Memory::MemoryPoolAllocator<std::pair<const TKey, TValue>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen