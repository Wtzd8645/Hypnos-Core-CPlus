#pragma once

#include "Hypnos-Core/Memory/MemoryPoolAllocator.hpp"
#include <vector>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T>
using List = std::vector<T, Memory::MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen