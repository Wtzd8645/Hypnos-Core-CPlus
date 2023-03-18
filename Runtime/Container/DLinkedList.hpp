#ifndef DLINKED_LIST_HPP_
#define DLINKED_LIST_HPP_

#include <list>
#include "../Allocator.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T>
using DLinkedList = std::list<T, MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen

#endif // DLINKED_LIST_HPP_