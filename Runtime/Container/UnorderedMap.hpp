#ifndef UNORDERED_MAP_HPP_
#define UNORDERED_MAP_HPP_

#include <unordered_map>
#include "../Allocator.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class TKey, class TValue>
using UnorderedMap = std::unordered_map<TKey, TValue, std::hash<TKey>, std::equal_to<TKey>, MemoryPoolAllocator<std::pair<const TKey, TValue>>>;

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen

#endif // UNORDERED_MAP_HPP_