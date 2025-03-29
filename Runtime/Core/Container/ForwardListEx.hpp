#pragma once

#include "Base/Memory/MemoryPoolAllocator.hpp"
#include "Platform.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Container {

template<typename T>
struct SinglyNode
{
    SinglyNode* next = nullptr;
    T object;
};

template<typename T, typename Allocator = Memory::MemoryPoolAllocator<SinglyNode<T>>>
 class ForwardListEx
 { 
 public:
     using alloc_traits = std::allocator_traits<Allocator>;
     using singly_node = SinglyNode<T>;

     struct Iterator
     {
     public:
         using iterator_category = std::forward_iterator_tag;
         using difference_type = std::ptrdiff_t;
         using value_type = T;
         using pointer = T*;
         using reference = T&;

         Iterator(singly_node* node) : node(node) { }

         reference operator*() const { return node->object; }
         pointer operator->() const { return &node->object; }

         Iterator& operator++() { node = node->next; return *this; }
         Iterator operator++(int) { Iterator it(*this); node = node->next; return it; }

         friend bool operator==(const Iterator& lhs, const Iterator& rhs) { return lhs.node == rhs.node; }
         friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return lhs.node != rhs.node; }

     private:
         singly_node* node;
     };

     ForwardListEx() = default;

     ~ForwardListEx() noexcept
     {
         Clear();
     }

     ForwardListEx(const ForwardListEx& other) : allocator(other.allocator), size(other.size)
     {
         CopyFrom(other);
     }

     ForwardListEx& operator=(const ForwardListEx& other)
     {
         Clear();
         CopyFrom(other);
         return *this;
     }

     ForwardListEx(ForwardListEx&& other) noexcept : allocator(std::move(other.allocator)), size(other.size), head(other.head), tail(other.tail)
     {
         other.head = nullptr;
         other.tail = nullptr;
         other.size = 0;
     }

     ForwardListEx& operator=(ForwardListEx&& other) noexcept
     {
         if (this != &other)
         {
             Clear();
             allocator = std::move(other.allocator);
             size = other.size;
             head = other.head;
             tail = other.tail;

             other.head = nullptr;
             other.tail = nullptr;
             other.size = 0;
         }
         return *this;
     }

     inline int32 Size() const noexcept { return size; }
     inline Iterator begin() const noexcept { return Iterator(head); }
     inline Iterator end() const noexcept { return Iterator(nullptr); }
 
     void Clear() noexcept
     {
         size = 0;
         singly_node* node = head;
         while (node != nullptr)
         {
             singly_node* next = node->next;
             node->object.~T();
             allocator.deallocate(node, 1);
             node = next;
         }

         head = nullptr;
         tail = nullptr;
     }

     inline void AddFirst(const T& obj)
     {
         AddFirstInternal(obj);
     }
 
     inline void AddFirst(T&& obj)
     { 
         AddFirstInternal(std::move(obj));
     }
 
     inline void AddLast(const T& obj)
     {
         AddLastInternal(obj);
     }

     inline void AddLast(T&& obj)
     {
         AddLastInternal(std::move(obj));
     }

     void Remove(const T& obj) noexcept
     {
         singly_node* prev = nullptr;
         singly_node* curr = head;
         while (curr != nullptr)
         {
             if (curr->object == obj)
             {
                 Remove(prev, curr);
                 return;
             }

             prev = curr;
             curr = curr->next;
         }
     }
 
     void RemoveFirst() noexcept
     {
         if (head != nullptr)
         {
             Remove(nullptr, head);
         }
     }

     void RemoveLast() noexcept
     {
         if (head == nullptr)
         {
             return;
         }

         singly_node* prev = nullptr;
         singly_node* curr = head;
         while (curr->next != nullptr)
         {
             prev = curr;
             curr = curr->next;
         }
         Remove(prev, curr);
     }

private:
     Allocator allocator;
     int32 size = 0;

     singly_node* head = nullptr;
     singly_node* tail = nullptr;

     void CopyFrom(const ForwardListEx& other)
     {
         if (other.head == nullptr)
         {
             return;
         }

         singly_node** node = &head;
         singly_node* curr = other.head;
         while (curr)
         {
             *node = alloc_traits::allocate(allocator, 1);
             new (&(*node)->object) T(curr->object);
             (*node)->next = nullptr;

             tail = *node;
             node = &(*node)->next;
             curr = curr->next;
         }
     }

     template<typename U>
     void AddFirstInternal(U&& obj)
     {
         singly_node node = alloc_traits::allocate(allocator, 1);
         new (&node->object) T(std::forward<U>(obj));
         node->next = head;

         head = node;
         if (tail == nullptr)
         {
             tail = node;
         }

         ++size;
     }

     template<typename U>
     void AddLastInternal(U&& obj)
     {
         singly_node node = alloc_traits::allocate(allocator, 1);
         new (&node->object) T(std::forward<U>(obj));
         node->next = nullptr;

         if (head == nullptr)
         {
             head = node;
         }
         else
         {
             tail->next = node;
         }

         tail = node;
         ++size;
     }

     inline void Remove(singly_node* prev, singly_node* node) noexcept
     {
         if (node == head)
         {
             head = node->next;
         }
         else
         {
             prev->next = node->next;
         }

         if (node == tail)
         {
             tail = prev;
         }

         node->object.~T();
         alloc_traits::deallocate(allocator, node, 1);
         --size;
     }
 };

} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen