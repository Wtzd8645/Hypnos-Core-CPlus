#pragma once

#include "Hypnos-Core/Allocator.hpp"
#include <unordered_set>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T>
using UnorderedSet = std::unordered_set<T, std::hash<T>, std::equal_to<T>, MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen