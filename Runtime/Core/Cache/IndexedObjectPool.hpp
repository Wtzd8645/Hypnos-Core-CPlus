#pragma once

#include <Platform.hpp>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>

namespace Blanketmen {
namespace Hypnos {
namespace Cache {

template <typename T>
class IndexedObjectPool
{
private:
    struct Item
    {
        T object;
        size_t index;
    };

public:
    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(Item* ptr, size_t pos) : ptr(ptr), pos(pos) { }

        reference operator*() const { return ptr[pos].object; }
        pointer operator->() const { return &ptr[pos].object; }

        Iterator& operator++()
        {
            ++pos;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b)
        {
            return a.ptr == b.ptr && a.pos == b.pos;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b)
        {
            return !(a == b);
        }

    private:
        Item* ptr;
        size_t pos;
    };

    IndexedObjectPool(size_t cap = 16, float32 expand_fac = 2.0f) : items(nullptr), capacity(0), size(0), expand_factor(expand_fac)
    {
        Allocate(cap);
    }

    ~IndexedObjectPool()
    {
        if (items != nullptr)
        {
            std::free(items);
        }
    }

    Iterator begin() { return Iterator(items, 0); }
    Iterator end() { return Iterator(items, size); }

    size_t Capacity() const noexcept { return capacity; }
    size_t Size() const noexcept { return size; }

    void Clear()
    {
        size = 0;
    }

    inline T* Acquire()
    {
        if (size >= capacity)
        {
            Expand();
        }

        size_t freeIndex = items[size++].index;
        return &items[freeIndex].object;
    }

    inline void Release(T* obj)
    {
        Item* item = reinterpret_cast<Item*>(obj);
        if (item < items || item >= items + capacity)
        {
            return;
        }

        --size;
        size_t index = item->index;
        item->index = items[size].index;
        items[size].index = index;
        item->object.~T();
    }

    template<typename... Args>
    inline T* Emplace(Args&&... args)
    {
        if (size >= capacity)
        {
            Expand();
        }

        size_t freeIndex = items[size++].index;
        new (&items[freeIndex].object) T(std::forward<Args>(args)...);
        return &items[freeIndex].object;
    }

    inline void Destroy(T* obj)
    {
        Item* item = reinterpret_cast<Item*>(obj);
        if (item < items || item >= items + capacity)
        {
            return;
        }

        --size;
        size_t index = item->index;
        item->index = items[size].index;
        items[size].index = index;
    }

private:
    float32 expand_factor;
    size_t capacity;
    size_t size;
    Item* items;

    void Allocate(size_t cap)
    {
        if (cap <= capacity)
        {
            return;
        }

        Item* new_items = static_cast<Item*>(std::aligned_alloc(alignof(Item), cap * sizeof(Item)));
        if (items != nullptr)
        {
            std::uninitialized_move(items, items + size, new_items);
            std::free(items);
        }

        for (size_t i = capacity; i < cap; ++i)
        {
            new_items[i].index = i;
        }
        capacity = cap;
        items = new_items;
    }

    void Expand()
    {
        size_t new_cap = std::max(size_t(1), static_cast<size_t>(capacity * expand_factor));
        Allocate(new_cap);
    }
};

} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen