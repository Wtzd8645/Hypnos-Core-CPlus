#pragma once

#include <Platform.hpp>
#include <cstddef>

namespace Blanketmen {

template <typename T>
class TrackedObjectPool
{
private:
    struct Element
    {
        AlignedStorage<T> storage;
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

        Iterator(Element* ptr, size_t pos) : elem(ptr), pos(pos) { }

        reference operator*() const { return *reinterpret_cast<T*>(&elem[elem[pos].index].storage); }
        pointer operator->() const { return reinterpret_cast<T*>(&elem[elem[pos].index].storage); }

        Iterator& operator++() { ++pos; return *this; }
        Iterator operator++(int) { Iterator it = *this; ++(*this); return it; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.elem == b.elem && a.pos == b.pos; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

    private:
        Element* elem;
        size_t pos;
    };

    TrackedObjectPool(size_t cap = 16) : capacity(cap)
    {
        Allocate(cap);
    }

    ~TrackedObjectPool()
    {
        if (elems != nullptr)
        {
            std::free(elems);
        }
    }

    Iterator begin() { return Iterator(elems, 0); }

    Iterator end() { return Iterator(elems, count); }

    size_t Capacity() const noexcept { return capacity; }

    size_t Count() const noexcept { return count; }

    void Clear()
    {
        count = 0;
    }

    T* Acquire()
    {
        if (count >= capacity)
        {
            Expand();
        }

        size_t index = elems[count++].index;
        return reinterpret_cast<T*>(&elems[index]);
    }

    void Release(T* obj)
    {
        Element* elem = reinterpret_cast<Element*>(obj);
        if (elem < elems || elem >= elems + capacity)
        {
            Logging::Error("[TrackedObjectPool] Release failed. Please check whether the pool is used correctly.");
            return;
        }

        size_t index = elem->index;
        elem->index = elems[--count].index;
        elems[count].index = index;
    }

    template<typename... Args>
    T* Emplace(Args&&... args)
    {
        if (count >= capacity)
        {
            Expand();
        }

        size_t index = elems[count++].index;
        return new (&elems[index]) T(std::forward<Args>(args)...);
    }

    void Destroy(T* obj)
    {
        Element* item = reinterpret_cast<Element*>(obj);
        if (item < elems || item >= elems + capacity)
        {
            Logging::Error("[TrackedObjectPool] Destroy failed. Please check whether the pool is used correctly.");
            return;
        }

        obj->~T();
        size_t index = item->index;
        item->index = elems[--count].index;
        elems[count].index = index;
    }

private:
    size_t capacity;
    size_t count = 0;
    Element* elems = nullptr;

    void Allocate(size_t cap)
    {
        if (capacity > 0 && cap <= capacity)
        {
            Logging::Warning("[TrackedObjectPool] Allocate failed. Capacity is already sufficient.");
            return;
        }

        Element* new_elems = static_cast<Element*>(std::aligned_alloc(alignof(Element), cap * sizeof(Element)));
        if (elems != nullptr)
        {
            std::uninitialized_move(elems, elems + count, new_elems);
            std::free(elems);
        }

        for (size_t i = capacity; i < cap; ++i)
        {
            new_elems[i].index = i;
        }
        capacity = cap;
        elems = new_elems;
    }

    void Expand()
    {
        Allocate(std::max(size_t(1), capacity * 2));
    }
};

} // namespace Blanketmen