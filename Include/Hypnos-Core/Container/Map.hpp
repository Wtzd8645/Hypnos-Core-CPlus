#pragma once

#include "Hypnos-Core/Allocator.hpp"
#include <map>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class TKey, class TValue>
using Map = std::map<TKey, TValue, std::less<TKey>, MemoryPoolAllocator<std::pair<const TKey, TValue>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen