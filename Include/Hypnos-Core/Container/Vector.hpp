#pragma once

#include "Hypnos-Core/Allocator.hpp"
#include <vector>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T>
using Vector = std::vector<T, MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen