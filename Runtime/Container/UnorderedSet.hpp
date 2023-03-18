#ifndef UNORDERED_SET_HPP_
#define UNORDERED_SET_HPP_

#include <unordered_set>
#include "../Allocator.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T>
using UnorderedSet = std::unordered_set<T, std::hash<T>, std::equal_to<T>, MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen

#endif // UNORDERED_SET_HPP_