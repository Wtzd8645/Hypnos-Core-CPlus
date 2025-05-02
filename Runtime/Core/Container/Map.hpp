#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <map>

namespace Blanketmen {

template<typename TKey, typename TValue, typename TCompare = std::less<TKey>, typename TAllocator = MemoryPoolAllocator<std::pair<const TKey, TValue>>>
using Map = std::map<TKey, TValue, TCompare, TAllocator>;

} // namespace Blanketmen