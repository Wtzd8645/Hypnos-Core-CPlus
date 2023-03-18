#ifndef DELEGATE_HPP_
#define DELEGATE_HPP_

#include <utility>

namespace Blanketmen {
namespace Hypnos {

template<typename... TArgs>
class Delegate
{
public:
    class FunctionBase
    {
    public:
        FunctionBase* next = nullptr;

        virtual ~FunctionBase() { }

        virtual void operator()(TArgs&&... args) const = 0;
    };

    class StaticFunction : public FunctionBase
    {
    public:
        typedef void (*FunctionPtr)(TArgs...);

        FunctionPtr functionPtr;

        StaticFunction(FunctionPtr funcPtr) : functionPtr(funcPtr) { }

        void operator()(TArgs&&... args) const override final
        {
            (*functionPtr)(std::forward<TArgs>(args)...);
        }
    };

    template<typename TObj>
    class ObjectFunction : public FunctionBase
    {
    public:
        typedef TObj* const ObjectPtr;
        typedef void (TObj::* const MethodPtr)(TArgs...);

        ObjectPtr objectPtr;
        MethodPtr methodPtr;

        ObjectFunction(ObjectPtr objPtr, MethodPtr mtdPtr) : objectPtr(objPtr), methodPtr(mtdPtr) { }

        void operator()(TArgs&&... args) const override final
        {
            (objectPtr->*methodPtr)(std::forward<TArgs>(args)...);
        }
    };

    Delegate() : head(nullptr) { }

    Delegate(const Delegate&) = delete;

    ~Delegate()
    {
        Clear();
    }

    void Clear()
    {
        FunctionBase* curr = head;
        FunctionBase* next = nullptr;
        while (curr != nullptr)
        {
            next = curr->next;
            delete curr;
            curr = next;
        }
        head = nullptr;
    }

    void operator()(TArgs... args) const
    {
        FunctionBase* curr = head;
        while (curr != nullptr)
        {
            (*curr)(std::forward<TArgs>(args)...);
            curr = curr->next;
        }
    }

    Delegate& operator=(const Delegate&) = delete;

    Delegate& operator=(void(* const funcPtr)(TArgs...))
    {
        Clear();
        head = new StaticFunction(funcPtr);
        return *this;
    }

    Delegate& operator+=(void(* const funcPtr)(TArgs...))
    {
        FunctionBase** curr = &head;
        while (*curr != nullptr)
        {
            curr = &(*curr)->next;
        }
        *curr = new StaticFunction(funcPtr);
        return *this;
    }

    Delegate& operator-=(void(* const funcPtr)(TArgs...))
    {
        FunctionBase** curr = &head;
        while (*curr != nullptr)
        {
            StaticFunction* func = dynamic_cast<StaticFunction*>(*curr);
            if (func->functionPtr != funcPtr)
            {
                curr = &(*curr)->next;
                continue;
            }

            *curr = (*curr)->next;
            delete func;
            return *this;
        }
        return *this;
    }

    template<typename TObj>
    void Assign(typename ObjectFunction<TObj>::ObjectPtr objPtr, typename ObjectFunction<TObj>::MethodPtr mtdPtr)
    {
        Clear();
        head = new ObjectFunction<TObj>(objPtr, mtdPtr);
    }

    template<typename TObj>
    void Add(typename ObjectFunction<TObj>::ObjectPtr objPtr, typename ObjectFunction<TObj>::MethodPtr mtdPtr)
    {
        FunctionBase** curr = &head;
        while (*curr != nullptr)
        {
            curr = &(*curr)->next;
        }
        *curr = new ObjectFunction<TObj>(objPtr, mtdPtr);
    }

    template<typename TObj>
    void Remove(typename ObjectFunction<TObj>::ObjectPtr objPtr, typename ObjectFunction<TObj>::MethodPtr mtdPtr)
    {
        FunctionBase** curr = &head;
        while (*curr != nullptr)
        {
            ObjectFunction<TObj>* func = dynamic_cast<ObjectFunction<TObj>*>(*curr);
            if (func == nullptr
                || func->objectPtr != objPtr
                || func->methodPtr != mtdPtr)
            {
                curr = &(*curr)->next;
                continue;
            }

            *curr = (*curr)->next;
            delete func;
            return;
        }
    }

private:
    FunctionBase* head = nullptr;
};

} // namespace Hypnos
} // namespace Blanketmen

#endif // DELEGATE_HPP_