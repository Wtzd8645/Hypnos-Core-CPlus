#pragma once

#include "Hypnos-Kernel/Base/Memory/MemoryPoolAllocator.hpp"
#include <unordered_set>

namespace Blanketmen {

template<typename T, typename THash = std::hash<T>, typename TEqual = std::equal_to<T>, typename TAllocator = MemoryPoolAllocator<T>>
using UnorderedSet = std::unordered_set<T, THash, TEqual, TAllocator>;

} // namespace Blanketmen