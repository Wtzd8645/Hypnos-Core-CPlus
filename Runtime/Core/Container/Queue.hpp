#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <queue>

namespace Blanketmen {

template <typename T, typename TSequence = std::deque<T, MemoryPoolAllocator<T>>>
using Queue = std::queue<T, TSequence>;

} // namespace Blanketmen