#pragma once

#include <memory>
#include <stack>

namespace Blanketmen {

template <typename T, typename TSequence = std::deque<T, std::allocator<T>>>
using Stack = std::stack<T, TSequence>;

} // namespace Blanketmen