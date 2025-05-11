#pragma once

#include <array>

namespace Blanketmen {

template<typename T, std::size_t length>
using Array = std::array<T, length>;

} // namespace Blanketmen