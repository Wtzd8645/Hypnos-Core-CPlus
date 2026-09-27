#pragma once

#include <list>
#include <memory>

namespace Blanketmen {

template<typename T, typename TAllocator = std::allocator<T>>
using LinkedList = std::list<T, TAllocator>;

} // namespace Blanketmen