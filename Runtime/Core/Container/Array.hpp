#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <array>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T, std::size_t length>
using Array = std::array<T, length>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen