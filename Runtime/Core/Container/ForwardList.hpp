#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include <forward_list>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T, typename TAllocator = Memory::MemoryPoolAllocator<T>>
using ForwardList = std::forward_list<T, TAllocator>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen