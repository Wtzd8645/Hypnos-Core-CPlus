#pragma once

#include "Hypnos-Core/Memory/MemoryPoolAllocator.hpp"
#include <map>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename TKey, typename TValue>
using Map = std::map<TKey, TValue, std::less<TKey>, Memory::MemoryPoolAllocator<std::pair<const TKey, TValue>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen