#pragma once

#include <memory>
#include <queue>

namespace Blanketmen {

template <typename T, typename TSequence = std::deque<T, std::allocator<T>>>
using Queue = std::queue<T, TSequence>;

} // namespace Blanketmen