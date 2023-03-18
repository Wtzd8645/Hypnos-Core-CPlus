#ifndef SLINKED_LIST_HPP_
#define SLINKED_LIST_HPP_

#include <forward_list>
#include "../Allocator.hpp"
// #include "../Cache/ObjectPool.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<class T>
using SLinkedList = std::forward_list<T, MemoryPoolAllocator<T>>;

// template<typename T>
// class SLinkedList
// {
// private:
//     struct SinglyNode
//     {
//         T object;
//         SinglyNode* nextNode = nullptr;
//     };
// 
//     struct MemoryChunk
//     {
//         int32 capacity;
//         SinglyNode* singlyNodes;
//         MemoryChunk* nextNode = nullptr;
// 
//         MemoryChunk(int32 nodeCount) : capacity(nodeCount), singlyNodes(new SinglyNode[nodeCount])
//         {
//             --nodeCount;
//             for (int i = 0; i < nodeCount; ++i)
//             {
//                 SinglyNode* node = singlyNodes + i;
//                 (node)->nextNode = (node + 1);
//             }
//             (singlyNodes + nodeCount)->nextNode = nullptr;
//         }
// 
//         ~MemoryChunk()
//         {
//             delete[] singlyNodes;
//         }
//     };
// 
//     int32 capacity = 0;
//     MemoryChunk* currentChunk = nullptr;
//     SinglyNode* freeNodes = nullptr;
// 
//     SinglyNode* headNode = nullptr;
//     SinglyNode* tailNode = nullptr;
// 
// public:
//     inline int32 Capacity() const noexcept { return capacity; }
// 
//     SLinkedList(int32 cap = 8)
//     {
//         Allocate(cap < 8 ? 8 : cap);
//     }
// 
//     ~SLinkedList()
//     {
//         MemoryChunk* chunk = currentChunk;
//         while (chunk != nullptr)
//         {
//             MemoryChunk* nextChunk = chunk->nextNode;
//             delete chunk;
//             chunk = nextChunk;
//         }
//     }
// 
//     void Clear()
//     {
//         while (headNode != nullptr)
//         {
//             SinglyNode* nextNode = headNode->nextNode;
//             headNode->nextNode = freeNodes;
//             freeNodes = headNode;
//             headNode = nextNode;
//         }
//         tailNode = nullptr;
//     }
// 
//     void AddFirst(T obj)
//     {
//         if (freeNodes == nullptr)
//         {
//             Allocate(capacity);
//         }
// 
//         SinglyNode* node = freeNodes;
//         freeNodes = freeNodes->nextNode;
// 
//         node->object = obj;
//         node->nextNode = headNode;
//         headNode = node;
//         if (tailNode == nullptr)
//         {
//             tailNode = node;
//         }
//     }
// 
//     void AddLast(T obj)
//     {
//         if (freeNodes == nullptr)
//         {
//             Allocate(capacity);
//         }
// 
//         SinglyNode* node = freeNodes;
//         freeNodes = freeNodes->nextNode;
// 
//         node->object = obj;
//         node->nextNode = nullptr;
//         if (headNode == nullptr)
//         {
//             headNode = node;
//             tailNode = node;
//         }
//         else
//         {
//             tailNode->nextNode = node;
//             tailNode = node;
//         }
//     }
// 
//     void RemoveFirst()
//     {
//         if (headNode == tailNode)
//         {
//             headNode = nullptr;
//             tailNode = nullptr;
//             return;
//         }
// 
//         SinglyNode* node = headNode;
//         headNode = headNode->nextNode;
//         node->nextNode = freeNodes;
//         freeNodes = node;
//     }
// 
//     void RemoveFirst(T obj)
//     {
//         SinglyNode* prevNode = nullptr;
//         SinglyNode* currNode = headNode;
//         while (currNode != nullptr)
//         {
//             if (currNode->object == obj)
//             {
//                 // Remove node.
//                 if (currNode == headNode)
//                 {
//                     headNode = currNode->nextNode; // NOTE: NextNode can't be self.
//                 }
//                 else
//                 {
//                     prevNode->nextNode = currNode->nextNode;
//                 }
// 
//                 // Set last node.
//                 if (currNode == tailNode)
//                 {
//                     tailNode = prevNode;
//                 }
// 
//                 currNode->nextNode = freeNodes;
//                 freeNodes = currNode;
//                 break;
//             }
// 
//             prevNode = currNode;
//             currNode = currNode->nextNode;
//         }
//     }
// 
//     void RemoveLast()
//     {
//         if (headNode == tailNode)
//         {
//             headNode = nullptr;
//             tailNode = nullptr;
//             return;
//         }
// 
//         SinglyNode* currNode = headNode;
//         while (currNode->nextNode != tailNode)
//         {
//             currNode = currNode->nextNode;
//         }
// 
//         tailNode->nextNode = freeNodes;
//         freeNodes = tailNode;
// 
//         currNode->nextNode = nullptr;
//         tailNode = currNode;
//     }
// 
// private:
//     void Allocate(int32 nodeCount)
//     {
//         MemoryChunk* chunk = new MemoryChunk(nodeCount);
//         capacity += nodeCount;
//         freeNodes = chunk->singlyNodes;
//         chunk->nextNode = currentChunk;
//         currentChunk = chunk;
//     }
// 
// public:
//     struct Iterator
//     {
//         using iterator_category = std::forward_iterator_tag;
//         using difference_type = std::ptrdiff_t;
//         using value_type = T;
//         using pointer = T*;
//         using reference = T&;
// 
//         Iterator(pointer ptr) : ptr(ptr) {}
// 
//         reference operator*() const { return *ptr; }
//         pointer operator->() { return ptr; }
//         Iterator& operator++() { ptr = reinterpret_cast<T*>(reinterpret_cast<SinglyNode*>(ptr)->nextNode); return *this; }
//         Iterator operator++(int) { Iterator it = *this; ++(*this); return it; }
//         friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; };
//         friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; };
// 
//     private:
//         pointer ptr;
//     };
// 
//     Iterator begin() { return Iterator(reinterpret_cast<T*>(headNode)); }
//     Iterator end() { return Iterator(nullptr); }
// };

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen

#endif // SLINKED_LIST_HPP_