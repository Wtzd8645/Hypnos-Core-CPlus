#pragma once

#include "Hypnos-Kernel/Base/Memory/MemoryPoolAllocator.hpp"
#include <list>

namespace Blanketmen {

template<typename T, typename TAllocator = MemoryPoolAllocator<T>>
using LinkedList = std::list<T, TAllocator>;

} // namespace Blanketmen