#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <vector>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T, typename TAllocator = Memory::MemoryPoolAllocator<T>>
using List = std::vector<T, TAllocator>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen