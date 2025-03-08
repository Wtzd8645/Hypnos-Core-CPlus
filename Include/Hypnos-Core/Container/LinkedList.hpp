#pragma once

#include "Hypnos-Core/Memory/MemoryPoolAllocator.hpp"
#include <list>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T>
using LinkedList = std::list<T, Memory::MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen