#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <list>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T, typename TAllocator = Memory::MemoryPoolAllocator<T>>
using LinkedList = std::list<T, TAllocator>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen