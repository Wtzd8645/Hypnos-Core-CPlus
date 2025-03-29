#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <unordered_set>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T, typename THash = std::hash<T>, typename TEqual = std::equal_to<T>, typename TAllocator = Memory::MemoryPoolAllocator<T>>
using UnorderedSet = std::unordered_set<T, THash, TEqual, TAllocator>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen