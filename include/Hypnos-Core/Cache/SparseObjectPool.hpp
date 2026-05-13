#pragma once

#include <Hypnos-Core/Base/Memory/MemoryUtils.hpp>
#include <cstddef>
#include <cstdlib>
#include <memory>

namespace Blanketmen {

template <typename T>
class SparseObjectPool
{
private:
    struct Element
    {
        size_t key; // Active element key.
        size_t index; // Index in active elements.
        alignas(T) byte storage[sizeof(T)];
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

        Iterator(Element* ptr, size_t count) : elem(ptr), count(count) { }

        reference operator*() const { return *reinterpret_cast<T*>(&elem[elem[count].key].storage); }
        pointer operator->() const { return reinterpret_cast<T*>(&elem[elem[count].key].storage); }

        Iterator& operator++() { ++count; return *this; }
        Iterator operator++(int) { Iterator it = *this; ++(*this); return it; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.count == b.count && a.elem == b.elem; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

    private:
        Element* elem;
        size_t count;
    };

    SparseObjectPool(size_t cap = 16) : capacity(cap), count(0)
    {
        elems = static_cast<Element*>(MemoryUtils::Allocate<Element>(sizeof(Element) * cap));
        for (size_t i = 0; i < cap; ++i)
        {
            Element& elem = elems[i];
            elem.key = i;
            elem.index = i;
            new (&elem.storage) T();
        }
    }

    ~SparseObjectPool()
    {
        MemoryUtils::Deallocate<Element>(elems);
    }

    SparseObjectPool(const SparseObjectPool&) = delete;

    SparseObjectPool& operator=(const SparseObjectPool&) = delete;

    SparseObjectPool(SparseObjectPool&& other) = delete;

    SparseObjectPool& operator=(SparseObjectPool&& other) = delete;

    Iterator begin() { return Iterator(elems, 0); }

    Iterator end() { return Iterator(elems, count); }

    size_t Capacity() const noexcept { return capacity; }

    size_t Count() const noexcept { return count; }

    size_t IndexOf(const T* obj) const
    {
        const Element* elem = reinterpret_cast<const Element*>(reinterpret_cast<const byte*>(obj) - offsetof(Element, storage));
        assert(elem >= elems && elem < elems + capacity);
        return static_cast<size_t>(elem - elems);
    }

    T* TryGet(size_t slot)
    {
        if (slot >= capacity)
        {
            return nullptr;
        }

        Element& elem = elems[slot];
        if (elem.index >= count || elems[elem.index].key != slot)
        {
            return nullptr;
        }

        return reinterpret_cast<T*>(&elem.storage);
    }

    const T* TryGet(size_t slot) const
    {
        if (slot >= capacity)
        {
            return nullptr;
        }

        const Element& elem = elems[slot];
        if (elem.index >= count || elems[elem.index].key != slot)
        {
            return nullptr;
        }

        return reinterpret_cast<const T*>(&elem.storage);
    }

    void Clear()
    {
        for (size_t i = 0; i < count; ++i)
        {
            reinterpret_cast<T*>(&elems[elems[i].key].storage)->~T();
            elems[i].key = i;
            elems[i].index = i;
        }

        for (size_t i = count; i < capacity; ++i)
        {
            elems[i].key = i;
            elems[i].index = i;
        }

        count = 0;
    }

    T* Acquire()
    {
        return count < capacity ? new (&elems[elems[count++].key].storage) T() : nullptr;
    }

    void Release(T* obj)
    {
        obj->~T();
        Element* elem = reinterpret_cast<Element*>(reinterpret_cast<byte*>(obj) - offsetof(Element, storage));
        assert(elem >= elems && elem < elems + capacity);

        --count;
        size_t curr_key = elems[elem->index].key;
        size_t last_key = elems[count].key;
        elems[elem->index].key = last_key;
        elems[count].key = curr_key;

        size_t index = elem->index;
        elem->index = elems[last_key].index;
        elems[last_key].index = index;
    }

private:
    size_t capacity;
    size_t count;
    Element* elems;
};

} // namespace Blanketmen
