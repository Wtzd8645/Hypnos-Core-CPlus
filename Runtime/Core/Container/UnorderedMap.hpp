#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <unordered_map>

namespace Blanketmen {

template<typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TAllocator = MemoryPoolAllocator<std::pair<const TKey, TValue>>>
using UnorderedMap = std::unordered_map<TKey, TValue, THash, TEqual, TAllocator>;

} // namespace Blanketmen