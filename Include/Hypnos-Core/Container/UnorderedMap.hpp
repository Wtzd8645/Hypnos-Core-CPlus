#pragma once

#include <memory>
#include <unordered_map>

namespace Blanketmen {

template<typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TAllocator = std::allocator<std::pair<const TKey, TValue>>>
using UnorderedMap = std::unordered_map<TKey, TValue, THash, TEqual, TAllocator>;

} // namespace Blanketmen