#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <array>
#include "../Type.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T, size_t length>
using Array = std::array<T, length>;

// template <typename T>
// class Array
// {
// public:
//     inline int32 Length() const noexcept { return length; }
// 
//     Array(int32 capacity) : items(new T[capacity]), length(capacity) { }
// 
//     ~Array()
//     {
//         if (items != nullptr)
//         {
//             delete[] items;
//         }
//     }
// 
//     T& operator[] (int32 index)
//     {
//         if (index < 0 || index >= length)
//         {
//             // TODO: Handle exception.
//             return *items;
//         }
//         return items[index];
//     }
// 
// private:
//     T* items = nullptr;
//     int32 length = 0;
// };

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen

#endif // ARRAY_HPP_