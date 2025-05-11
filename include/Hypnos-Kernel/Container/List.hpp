#pragma once

#include "Hypnos-Kernel/Base/Memory/MemoryPoolAllocator.hpp"
#include <vector>

namespace Blanketmen {

template<typename T, typename TAllocator = MemoryPoolAllocator<T>>
using List = std::vector<T, TAllocator>;

} // namespace Blanketmen