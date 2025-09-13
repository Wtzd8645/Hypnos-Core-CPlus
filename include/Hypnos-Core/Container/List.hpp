#pragma once

#include <memory>
#include <vector>

namespace Blanketmen {

template<typename T, typename TAllocator = std::allocator<T>>
using List = std::vector<T, TAllocator>;

} // namespace Blanketmen