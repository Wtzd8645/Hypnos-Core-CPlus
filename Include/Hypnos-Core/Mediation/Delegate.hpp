#pragma once

namespace Blanketmen {
namespace Hypnos {

template<typename... TArgs>
class Delegate
{
public:
    class FunctionBase
    {
    public:
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

    Delegate() : function(nullptr) { }
    Delegate(void(* const funcPtr)(TArgs...)) : function(new StaticFunction(funcPtr)) { }
    template <typename TObj>
    Delegate(TObj* const objPtr, void(TObj::* const mtdPtr)(TArgs...)) : function(new ObjectFunction<TObj>(objPtr, mtdPtr)) { }

    Delegate(const Delegate&) = delete;
    Delegate(Delegate&& other) noexcept : function(other.function) { other.function = nullptr; }

    ~Delegate()
    {
        if (function != nullptr)
        {
            delete function;
        }
    }

    void operator()(TArgs... args) const
    {
        if (function != nullptr)
        {
            (*function)(std::forward<TArgs>(args)...);
        }
    }

    Delegate& operator=(const Delegate&) = delete;

    Delegate& operator=(void(* const funcPtr)(TArgs...))
    {
        if (function != nullptr)
        {
            delete function;
            function = nullptr;
        }

        function = new StaticFunction(funcPtr);
        return *this;
    }

    template<typename TObj>
    void Assign(typename ObjectFunction<TObj>::ObjectPtr objPtr, typename ObjectFunction<TObj>::MethodPtr mtdPtr)
    {
        if (function != nullptr)
        {
            delete function;
            function = nullptr;
        }

        if (objPtr != nullptr && mtdPtr != nullptr)
        {
            function = new ObjectFunction<TObj>(objPtr, mtdPtr);
        }
    }

private:
    FunctionBase* function;
};

} // namespace Hypnos
} // namespace Blanketmen