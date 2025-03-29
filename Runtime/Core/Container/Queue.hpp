#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <queue>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template <typename T, typename TSequence = std::deque<T, Memory::MemoryPoolAllocator<T>>>
using Queue = std::queue<T, TSequence>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen