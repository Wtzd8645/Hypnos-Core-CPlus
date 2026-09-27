#pragma once

#include <map>
#include <memory>

namespace Blanketmen {

template<typename TKey, typename TValue, typename TCompare = std::less<TKey>, typename TAllocator = std::allocator<std::pair<const TKey, TValue>>>
using Map = std::map<TKey, TValue, TCompare, TAllocator>;

} // namespace Blanketmen