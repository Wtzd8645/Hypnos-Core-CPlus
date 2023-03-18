#ifndef DELEGATE_HPP_
#define DELEGATE_HPP_

#include <utility>

namespace Blanketmen {
namespace Hypnos {

template<typename... TArgs>
class FunctionBase
{
public:
    FunctionBase<TArgs...>* nextNode = nullptr;

    virtual ~FunctionBase() { }

    virtual void operator()(TArgs&&... args) = 0;
};

template<typename... TArgs>
class StaticFunction : public FunctionBase<TArgs...>
{
public:
    typedef void (*FunctionPtr)(TArgs...);

public:
    FunctionPtr function;

    StaticFunction(FunctionPtr func) : function(func) { }

    void operator()(TArgs&&... args) override final
    {
        (*function)(std::forward<TArgs...>(args...));
    }
};

template<typename T, typename... TArgs>
class ObjectFunction : public FunctionBase<TArgs...>
{
public:
    typedef void (T::* MemberFunctionPtr)(TArgs...);
    typedef T* ObjectPtr;

public:
    MemberFunctionPtr function;
    ObjectPtr object;

    ObjectFunction(MemberFunctionPtr func, T* obj) : function(func), object(obj) { }

    void operator()(TArgs&&... args) override final
    {
        (object->*function)(std::forward<TArgs...>(args...));
    }
};

template<typename... TArgs>
class Delegate
{
public:
    ~Delegate()
    {
        Clear();
    }

    void operator()(TArgs... args)
    {
        FunctionBase<TArgs...>* currNode = headNode;
        while (currNode != nullptr)
        {
            (*currNode)(std::forward<TArgs...>(args...));
            currNode = currNode->nextNode;
        }
    }

    inline void Clear()
    {
        FunctionBase<TArgs...>* currNode = headNode;
        FunctionBase<TArgs...>* nextNode = nullptr;
        while (currNode != nullptr)
        {
            nextNode = currNode->nextNode;
            delete currNode;
            currNode = nextNode;
        }
        headNode = nullptr;
        tailNode = nullptr;
    }

    void Add(void(* const funcPtr)(TArgs...))
    {
        FunctionBase<TArgs...>* func = new StaticFunction<TArgs...>(funcPtr);
        if (headNode == nullptr)
        {
            headNode = func;
            tailNode = func;
        }
        else
        {
            tailNode->nextNode = func;
            tailNode = func;
        }
    }

    void Remove(void(* const funcPtr)(TArgs...))
    {
        FunctionBase<TArgs...>* prevNode = nullptr;
        FunctionBase<TArgs...>* currNode = headNode;
        while (currNode != nullptr)
        {
            StaticFunction<TArgs...>* func = reinterpret_cast<StaticFunction<TArgs...>*>(currNode);
            if (func->function == funcPtr)
            {
                // Remove node.
                if (currNode == headNode)
                {
                    headNode = currNode->nextNode; // NOTE: NextNode can't be self.
                }
                else
                {
                    prevNode->nextNode = currNode->nextNode;
                }

                // Set last node.
                if (currNode == tailNode)
                {
                    tailNode = prevNode;
                }

                delete currNode;
                break;
            }
            prevNode = currNode;
            currNode = currNode->nextNode;
        }
    }

    template<typename TObj>
    void Add(void(TObj::* const funcPtr)(TArgs...), TObj* const obj)
    {
        FunctionBase<TArgs...>* objfunc = new ObjectFunction<TObj, TArgs...>(funcPtr, obj);
        if (headNode == nullptr)
        {
            headNode = objfunc;
            tailNode = objfunc;
        }
        else
        {
            tailNode->nextNode = objfunc;
            tailNode = objfunc;
        }
    }

    template<typename TObj>
    void Remove(void(TObj::* const funcPtr)(TArgs...), TObj* const obj)
    {
        FunctionBase<TArgs...>* prevNode = nullptr;
        FunctionBase<TArgs...>* currNode = headNode;
        while (currNode != nullptr)
        {
            ObjectFunction<TObj, TArgs...>* objFunc = reinterpret_cast<ObjectFunction<TObj, TArgs...>*>(currNode);
            if (objFunc->object == obj && objFunc->function == funcPtr)
            {
                // Remove node.
                if (currNode == headNode)
                {
                    headNode = currNode->nextNode; // NOTE: NextNode can't be self.
                }
                else
                {
                    prevNode->nextNode = currNode->nextNode;
                }

                // Set last node.
                if (currNode == tailNode)
                {
                    tailNode = prevNode;
                }

                delete currNode;
                break;
            }
            prevNode = currNode;
            currNode = currNode->nextNode;
        }
    }

private:
    FunctionBase<TArgs...>* headNode = nullptr;
    FunctionBase<TArgs...>* tailNode = nullptr;
};

} // namespace Hypnos
} // namespace Blanketmen

#endif // DELEGATE_HPP_