#pragma once

#include "Hypnos-Core/Memory/MemoryPoolAllocator.hpp"
#include <stack>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template <typename T>
using Stack = std::stack<T, std::deque<T, Memory::MemoryPoolAllocator<T>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen