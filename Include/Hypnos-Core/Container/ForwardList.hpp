#pragma once

#include <forward_list>
#include <memory>

namespace Blanketmen {

template<typename T, typename TAllocator = std::allocator<T>>
using ForwardList = std::forward_list<T, TAllocator>;

} // namespace Blanketmen