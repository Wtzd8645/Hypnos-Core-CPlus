#pragma once

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
        size_t index;
        size_t pos;
        T value;
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

        reference operator*() const { return elem[elem[count].index].value; }
        pointer operator->() const { return &elem[elem[count].index].value; }

        Iterator& operator++() { ++count; return *this; }
        Iterator operator++(int) { Iterator it = *this; ++(*this); return it; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.count == b.count && a.elem == b.elem; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

    private:
        Element* elem;
        size_t count;
    };

    SparseObjectPool(size_t cap = 16) : capacity(cap), count(0), elems(new Element[cap])
    {
        for (size_t i = 0; i < cap; i++)
        {
            elems[i].index = i;
            elems[i].pos = i;
        }
    }

    ~SparseObjectPool()
    {
        delete[] elems;
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
        return count < capacity ? &elems[elems[count++].index].value : nullptr;
    }

    void Release(T* obj)
    {
        Element* elem = reinterpret_cast<Element*>(reinterpret_cast<char*>(obj) - offsetof(Element, value));
        if (elem < elems || elem >= elems + capacity)
        {
            Logging::Error("[SparseObjectPool] Release failed. Please check whether the pool is used correctly.");
            return;
        }

        size_t curr_index = elems[elem->pos].index;
        size_t last_index = elems[--count].index;
        elems[elem->pos].index = last_index;
        elems[count].index = curr_index;

        size_t pos = elem->pos;
        elem->pos = elems[last_index].pos;
        elems[last_index].pos = pos;
    }

private:
    size_t capacity;
    size_t count;
    Element* elems;
};

} // namespace Blanketmen