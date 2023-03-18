#ifndef MAP_HPP_
#define MAP_HPP_

#include <map>
#include "../Allocator.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class TKey, class TValue>
using Map = std::map<TKey, TValue, std::less<TKey>, MemoryPoolAllocator<std::pair<const TKey, TValue>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen

#endif // MAP_HPP