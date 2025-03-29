#pragma once

#include <utility>

namespace Blanketmen {
namespace Hypnos {

template<typename... TArgs>
class MulticastDelegate
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

    MulticastDelegate() : functions(nullptr) { }
    MulticastDelegate(const MulticastDelegate&) = delete;

    ~MulticastDelegate()
    {
        Clear();
    }

    void Clear()
    {
        FunctionBase* curr = functions;
        FunctionBase* next = nullptr;
        while (curr != nullptr)
        {
            next = curr->next;
            delete curr;
            curr = next;
        }
        functions = nullptr;
    }

    void operator()(TArgs... args) const
    {
        FunctionBase* curr = functions;
        while (curr != nullptr)
        {
            (*curr)(std::forward<TArgs>(args)...);
            curr = curr->next;
        }
    }

    MulticastDelegate& operator=(const MulticastDelegate&) = delete;

    MulticastDelegate& operator=(void(* const funcPtr)(TArgs...))
    {
        Clear();
        functions = new StaticFunction(funcPtr);
        return *this;
    }

    MulticastDelegate& operator+=(void(* const funcPtr)(TArgs...))
    {
        FunctionBase** curr = &functions;
        while (*curr != nullptr)
        {
            curr = &(*curr)->next;
        }
        *curr = new StaticFunction(funcPtr);
        return *this;
    }

    MulticastDelegate& operator-=(void(* const funcPtr)(TArgs...))
    {
        FunctionBase** curr = &functions;
        while (*curr != nullptr)
        {
            StaticFunction* func = dynamic_cast<StaticFunction*>(*curr);
            if (func == nullptr)
            {
                continue;
            }

            if (func->functionPtr == funcPtr)
            {
                *curr = (*curr)->next;
                delete func;
                break;
            }

            curr = &(*curr)->next;
        }
        return *this;
    }

    template<typename TObj>
    void Assign(typename ObjectFunction<TObj>::ObjectPtr objPtr, typename ObjectFunction<TObj>::MethodPtr mtdPtr)
    {
        Clear();
        functions = new ObjectFunction<TObj>(objPtr, mtdPtr);
    }

    template<typename TObj>
    void Register(typename ObjectFunction<TObj>::ObjectPtr objPtr, typename ObjectFunction<TObj>::MethodPtr mtdPtr)
    {
        FunctionBase** curr = &functions;
        while (*curr != nullptr)
        {
            curr = &(*curr)->next;
        }
        *curr = new ObjectFunction<TObj>(objPtr, mtdPtr);
    }

    template<typename TObj>
    void Unregister(typename ObjectFunction<TObj>::ObjectPtr objPtr, typename ObjectFunction<TObj>::MethodPtr mtdPtr)
    {
        FunctionBase** curr = &functions;
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
    FunctionBase* functions = nullptr;
};

} // namespace Hypnos
} // namespace Blanketmen