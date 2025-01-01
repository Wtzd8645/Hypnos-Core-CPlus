#pragma once

#include "Hypnos-Core/Allocator.hpp"
#include <list>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T>
using DLinkedList = std::list<T, MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen