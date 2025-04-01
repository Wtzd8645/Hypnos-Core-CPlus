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
        size_t index;
        AlignedStorage<T> storage;
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

        Iterator(Item* ptr, size_t pos) : item(ptr), pos(pos) { }

        reference operator*() const { return *reinterpret_cast<T*>(&item[pos].storage); }
        pointer operator->() const { return reinterpret_cast<T*>(&item[pos].storage); }

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
            return a.item == b.item && a.pos == b.pos;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b)
        {
            return !(a == b);
        }

    private:
        Item* item;
        size_t pos;
    };

    IndexedObjectPool(size_t cap = 16) : capacity(cap)
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
    Iterator end() { return Iterator(items, count); }

    size_t Capacity() const noexcept { return capacity; }
    size_t Count() const noexcept { return count; }

    void Clear()
    {
        count = 0;
    }

    inline T* Acquire()
    {
        if (count >= capacity)
        {
            Expand();
        }

        size_t index = items[count++].index;
        return reinterpret_cast<T*>(&items[index].storage);
    }

    inline void Release(T* obj)
    {
        Item* item = reinterpret_cast<Item*>(obj);
        if (item < items || item >= items + capacity)
        {
            return;
        }

        size_t index = item->index;
        item->index = items[--count].index;
        items[count].index = index;
    }

    template<typename... Args>
    inline T* Emplace(Args&&... args)
    {
        if (count >= capacity)
        {
            Expand();
        }

        size_t index = items[count++].index;
        return new (&items[index].storage) T(std::forward<Args>(args)...);
    }

    inline void Destroy(T* obj)
    {
        Item* item = reinterpret_cast<Item*>(obj);
        if (item < items || item >= items + capacity)
        {
            return;
        }

        obj->~T();
        size_t index = item->index;
        item->index = items[--count].index;
        items[count].index = index;
    }

private:
    size_t capacity;
    size_t count = 0;
    Item* items = nullptr;

    void Allocate(size_t cap)
    {
        if (capacity > 0 && cap <= capacity)
        {
            return;
        }

        Item* new_items = static_cast<Item*>(std::aligned_alloc(alignof(Item), cap * sizeof(Item)));
        if (items != nullptr)
        {
            std::uninitialized_move(items, items + count, new_items);
            std::free(items);
        }

        for (size_t i = capacity; i < cap; i++)
        {
            new_items[i].index = i;
        }
        capacity = cap;
        items = new_items;
    }

    inline void Expand()
    {
        Allocate(std::max(size_t(1), capacity * 2));
    }
};

} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen