#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <stack>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template <typename T, typename TSequence = std::deque<T, Memory::MemoryPoolAllocator<T>>>
using Stack = std::stack<T, TSequence>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen