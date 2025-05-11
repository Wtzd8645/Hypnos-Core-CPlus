#pragma once

#include "Hypnos-Kernel/Base/Memory/MemoryPoolAllocator.hpp"
#include <forward_list>

namespace Blanketmen {

template<typename T, typename TAllocator = MemoryPoolAllocator<T>>
using ForwardList = std::forward_list<T, TAllocator>;

} // namespace Blanketmen