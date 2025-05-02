#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <stack>

namespace Blanketmen {

template <typename T, typename TSequence = std::deque<T, MemoryPoolAllocator<T>>>
using Stack = std::stack<T, TSequence>;

} // namespace Blanketmen