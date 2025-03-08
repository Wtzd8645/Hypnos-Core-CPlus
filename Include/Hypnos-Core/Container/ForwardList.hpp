#pragma once

#include "Hypnos-Core/Memory/MemoryPoolAllocator.hpp"
#include <forward_list>

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T>
using ForwardList = std::forward_list<T, Memory::MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen