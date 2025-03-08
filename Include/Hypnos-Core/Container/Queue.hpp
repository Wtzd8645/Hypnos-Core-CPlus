#pragma once

#include "Hypnos-Core/Memory/MemoryPoolAllocator.hpp"
#include <queue>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template <typename T>
using Queue = std::queue<T, std::deque<T, Memory::MemoryPoolAllocator<T>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen