#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <vector>
#include "../Allocator.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T>
using Vector = std::vector<T, MemoryPoolAllocator<T>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen

#endif // VECTOR_HPP_