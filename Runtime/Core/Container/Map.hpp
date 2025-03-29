#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <map>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename TKey, typename TValue, typename TCompare = std::less<TKey>, typename TAllocator = Memory::MemoryPoolAllocator<std::pair<const TKey, TValue>>>
using Map = std::map<TKey, TValue, TCompare, TAllocator>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen