//---------------------------------------------------------------------------

#ifndef UNIT1_H_
#define UNIT1_H_

#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <assert.h>
#include <memory>
#include <map>

using namespace std;

#include "..\loki\NullType.h"
#include "..\loki\EmptyType.h"
#include "..\loki\Typelist.h"
#include "..\loki\SmallObj.h"
#include "..\loki\TypeInfo.h"
#include "..\loki\AssocVector.h"
#include "..\loki\Functor.h"

//using namespace Loki;
//using namespace TL;

//using namespace MyTL;

namespace TestFunctor
{
// --- class FunctorImpl; ----
template <typename R, class TList>
class FunctorImpl;  /*
template <typename R,
          template <class> class ThreadingModel = DEFAULT_THREADING>
class FunctorImpl;  */

template <typename R>
class FunctorImpl<R, Loki::NullType>
{
public:
    typedef R ResultType;
    virtual R operator () () = 0;
    virtual FunctorImpl* Clone() const = 0;
    virtual ~FunctorImpl() {}
};
template <typename R, typename P1>
class FunctorImpl<R, TYPELIST_1(P1)>
{
public:
    typedef R ResultType;
    virtual R operator () (P1) = 0;
    virtual FunctorImpl* Clone() const = 0;
    virtual ~FunctorImpl() {}
};
template <typename R, typename P1, typename P2>
class FunctorImpl<R, TYPELIST_2(P1, P2) >
{
public:
    typedef R ResultType;
    virtual R operator() (P1, P2) = 0;
    virtual FunctorImpl* Clone() const = 0;
    virtual ~FunctorImpl() {}
};
template <typename R, typename P1, typename P2, typename P3>
class FunctorImpl<R, TYPELIST_3(P1, P2, P3) >
{
public:
    typedef R ResultType;
    virtual R operator() (P1, P2, P3) = 0;
    virtual FunctorImpl* Clone() const = 0;
    virtual ~FunctorImpl() {}
};
template <typename R, typename P1, typename P2, typename P3, typename P4>
class FunctorImpl<R, TYPELIST_4(P1, P2, P3, P4) >
{
public:
    typedef R ResultType;
    virtual R operator() (P1, P2, P3, P4) = 0;
    virtual FunctorImpl* Clone() const = 0;
    virtual ~FunctorImpl() {}
};
// .....

// --- class FunctorHandler ----
template <class ParentFunctor, typename Fun>
class FunctorHandler : public FunctorImpl
    < typename ParentFunctor::ResultType,
    typename ParentFunctor::ParmList >
{
public:
    typedef typename ParentFunctor::ResultType ResultType;
    FunctorHandler(const Fun& fun):fun_(fun) {}
    FunctorHandler* Clone() const
    {
        return new FunctorHandler(*this);
    }
    ResultType operator() ()
    {
        return fun_();
    }
    ResultType operator() (typename ParentFunctor::Parm1 p1)
    {
        return fun_(p1);
    }
    ResultType operator() (typename ParentFunctor::Parm1 p1,
                           typename ParentFunctor::Parm2 p2 )
    {
        return fun_(p1, p2);
    }
    ResultType operator() (typename ParentFunctor::Parm1 p1,
                           typename ParentFunctor::Parm2 p2,
                           typename ParentFunctor::Parm3 p3 )
    {
        return fun_(p1, p2, p3);
    }
    ResultType operator() (typename ParentFunctor::Parm1 p1,
                           typename ParentFunctor::Parm2 p2,
                           typename ParentFunctor::Parm3 p3,
                           typename ParentFunctor::Parm4 p4 )
    {
        return fun_(p1, p2, p3, p4);
    }
    // .....
private:
    Fun fun_;
}; // FunctorHandler

// --- class MemFunHandle ----
template <class ParentFunctor, typename PointerToObj,
typename PointerToMemFn>
class MemFunHandle : public FunctorImpl
    < typename ParentFunctor::ResultType,
    typename ParentFunctor::ParmList >
{
private:
    PointerToObj    pObj_;
    PointerToMemFn  pMemFn_;
public:
    typedef typename ParentFunctor::ResultType ResultType;
    typedef typename ParentFunctor::Parm1 Parm1;
    typedef typename ParentFunctor::Parm2 Parm2;
    typedef typename ParentFunctor::Parm3 Parm3;
    typedef typename ParentFunctor::Parm4 Parm4;

    MemFunHandle(const PointerToObj& pObj, PointerToMemFn pMemFn)
        : pObj_(pObj), pMemFn_(pMemFn) {}
    MemFunHandle* Clone() const
    {
        return new MemFunHandle(*this);
    }

    ResultType operator()()
    {
        return ((*pObj_).*pMemFn_)();
    }
    ResultType operator()(Parm1 p1)
    {
        return ((*pObj_).*pMemFn_)(p1);
    }

    ResultType operator()(Parm1 p1, Parm2 p2)
    {
        return ((*pObj_).*pMemFn_)(p1, p2);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3)
    {
        return ((*pObj_).*pMemFn_)(p1, p2, p3);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
    {
        return ((*pObj_).*pMemFn_)(p1, p2, p3, p4);
    }
    // .....
}; //MemFunHandle

// --- class Functor  ----
template <typename R = void, class TList = Loki::NullType>
//template<class> class ThreadingModel = DEFAULT_THREADING>
class Functor
{
private:
    typedef FunctorImpl<R, TList> Impl;
    std::auto_ptr<Impl> spImpl_;
public:
    Functor() : spImpl_(0) {}
    Functor(const Functor& rhs) : spImpl_(Impl::Clone(rhs.spImpl_.get()))
    {}
    Functor(std::auto_ptr<Impl> spImpl) : spImpl_(spImpl)
    {}
    // ÉùÃ÷ --- declaration constructor ---------
    template <typename Fun>
    Functor(Fun fun);

    template <class PtrObj, typename MemFn>
    Functor(const PtrObj& p, MemFn memFn);
    //    : spImpl_(new MemFunHandler<Functor, PtrObj, MemFn>(p, memFn)) {}

    typedef R ResultType;
    typedef TList ParmList;
    typedef typename Loki::TL::TypeAtNonStrict<TList, 0, Loki::EmptyType>::Result Parm1;
    typedef typename Loki::TL::TypeAtNonStrict<TList, 1, Loki::EmptyType>::Result Parm2;
    typedef typename Loki::TL::TypeAtNonStrict<TList, 2, Loki::EmptyType>::Result Parm3;
    typedef typename Loki::TL::TypeAtNonStrict<TList, 3, Loki::EmptyType>::Result Parm4;
    //...
public:
    R operator () ()
    {
        return (*spImpl_)();
    }
    R operator ()(Parm1 p1)
    {
        return (*spImpl_)(p1);
    }
    R operator ()(Parm1 p1, Parm2 p2)
    {
        return (*spImpl_)(p1, p2);
    }
    R operator ()(Parm1 p1, Parm2 p2, Parm3 p3)
    {
        return (*spImpl_)(p1, p2, p3);
    }
    R operator ()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
    {
        return (*spImpl_)(p1, p2, p3, p4);
    }
    // ....
};
/* */
template <typename R, class TList>
template <typename Fun>
Functor<R, TList>::Functor(Fun fun)// (const Fun& fun) ---- error
    : spImpl_(new FunctorHandler<Functor, Fun>(fun) )
{
}   /*  */
template <typename R, class TList>
template <class PtrObj, typename MemFn>
Functor<R, TList>::Functor(const PtrObj& p, MemFn memFn)//
    : spImpl_(new Loki::MemFunHandler<Functor, PtrObj, MemFn>(p, memFn))
{
}
//[C++ Error] E2285 Could not find a match for 'auto_ptr<FunctorImpl<void> >::auto_ptr(int *)'
// ---
template <class OriginalFunctor>
class BinderFirst
    : public FunctorImpl<typename OriginalFunctor::ResultType,
    typename OriginalFunctor::ParmList >
{
    typedef typename OriginalFunctor::ResultType ResultType;

    typedef typename OriginalFunctor::Parm1 BoundType;

    typedef typename OriginalFunctor::Parm2 Parm1;
    typedef typename OriginalFunctor::Parm3 Parm2;
    typedef typename OriginalFunctor::Parm4 Parm3;
public:
    BinderFirst(const OriginalFunctor& fun, BoundType bound)
        : f_(fun), b_(bound)
    {}

    //DEFINE_CLONE_FUNCTORIMPL(BinderFirst)

    BinderFirst* Clone() const
    {
        return new BinderFirst(*this);
    }

    // operator() implementations for up to 15 arguments

    ResultType operator()()
    {
        return f_(b_);
    }

    ResultType operator()(Parm1 p1)
    {
        return f_(b_, p1);
    }

    ResultType operator()(Parm1 p1, Parm2 p2)
    {
        return f_(b_, p1, p2);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3)
    {
        return f_(b_, p1, p2, p3);
    }
private:
    OriginalFunctor f_;
    BoundType b_;
};
//---------------------------------------------------------------------------
// ---- test Functor call functor, function -----------
struct TestFunctor1
{
    double operator () (int i, double d)
    {
        return i*d;
    }
};
static void    TestFunction2(int i, double d)
{
    cout << " TestFunctor::operator() (" <<i << ", " <<d<<") called. \n";
}
struct TestFunctor2
{
    void operator () (int i, double d)
    {
        cout << " TestFunctor::operator() (" <<i << ", " <<d<<") called. \n";
    }
};
static string  stringCat(const string& s1, const string& s2)
{
	//return  string(const_cast<string&>(s1))+s2;
	string ret(s1);
    return ret.append(s2);
}
struct StringCat
{
    string operator ()(const string& s1, const string& s2)
    {
        //return ( string(s1)+s2);
        //return s1+s2;
		string ret(s1);
		return ret.append(s2);
    }
};
// --- test MemFunHandle ------
class Parrot
{
public:
    void Eat()
    {
        cout << " Eat(): Tsk, knick, tsk .... \n";
    }
    void Speak()
    {
        cout << " Speak(): Oh, Caption, my Caption ! \n";
    }
    int ReturnInt()
    {
        return 1;
    }
};
}//namespace TestFunctor


namespace TestFactory
{
// ---- DefaultFactoryError  -----
template <class IdentifierType, class ProductType>
class DefaultFactoryError
{
public:
    class Exception : public std::exception
    {
    private:
        IdentifierType unknownId_;
    public:
        Exception(const IdentifierType& unknownId) : unknownId_(unknownId) {}
        virtual ~Exception() throw() {}
        virtual const char* what()
        {
            return " Unknown Object type passed to Factory .";
        }
        const IdentifierType getId()
        {
            return unknownId_;
        }
    };
protected:
    static ProductType* OnUnknownType(const IdentifierType& unknownId)
    {
        throw Exception(unknownId);
    }
};

// ---  Factory ----
template < class AbstractProduct,
typename IdentifierType,
typename ProductCreator = AbstractProduct* (*)(), //Functor<AbstractProduct* (*)()>  ...AbstractProduct::*
template <typename, class>
class FactoryErrorPolicy = DefaultFactoryError >
class Factory
    : public FactoryErrorPolicy<IdentifierType, AbstractProduct>
{
private:
    typedef map<IdentifierType, ProductCreator> IdToProductMap;
    IdToProductMap associations_;
public:
    //typedef ProductCreator ProductCreator; //### added for convenience, Pavel

    bool Register(const IdentifierType& id, ProductCreator creator)
    {
        return (associations_.insert(std::make_pair(id, creator))).second;
    }
    bool Unregister(const IdentifierType& id)
    {
        return associations_.erase(id)==1;
    }
    AbstractProduct* CreateObject(const IdentifierType& id)
    {
        typename IdToProductMap::const_iterator i = associations_.find(id);
        if (i!=associations_.end())
            return (i->second)();
        // else handle error
        return OnUnknownType(id);
    }
};

// ---  CloneFactory ----
template < class AbstractProduct,
typename ProductCreator = AbstractProduct* (*)(const AbstractProduct*), //Functor<AbstractProduct* (*)()>
template <typename, class>
class FactoryErrorPolicy = DefaultFactoryError >
class CloneFactory
    : public FactoryErrorPolicy<Loki::TypeInfo, AbstractProduct>
{
private:
    typedef Loki::TypeInfo TypeInfo;
    typedef map<Loki::TypeInfo, ProductCreator> IdToProductMap;
    IdToProductMap associations_;
public:
    using FactoryErrorPolicy<Loki::TypeInfo, AbstractProduct>::OnUnknownType;

    bool Register(const TypeInfo& id, ProductCreator creator)
    {
        return (associations_.insert(std::make_pair(id, creator))).second;
    }
    bool Unregister(const TypeInfo& id)
    {
        return associations_.erase(id)==1;
    }
    AbstractProduct* CreateObject(const AbstractProduct* model)
    {
        if (model == 0) return 0;
        typename IdToProductMap::const_iterator i = associations_.find(typeid(*model));
        if (i!=associations_.end())
            return (i->second)(model);
        // else handle error
        return OnUnknownType(typeid(*model));
    }
};
class Shape
{
public:
    virtual void Draw() = 0;
    virtual void Clear() = 0;
    virtual Shape* Clone()const = 0;
    Shape* Create()
    {
        return DoCreate();
    }
    virtual ~Shape()
    {
        cout << " deconstructor: ~Shape() ... \n";
    }
private:
    virtual Shape* DoCreate() = 0;
};
class Line : public Shape
{
public:
    void Draw()
    {
        cout << " Line Draw ... \n";
    }
    void Clear()
    {
        cout << " Line Clear ... \n";
    }
    Shape* Clone()const
    {
        cout << " Line Clone ... \n";
        return new Line(*this);
    }
private:
    Shape* DoCreate()
    {
        cout << " Line Clone ... \n";
        return new Line;
    }
};
class Polygon : public Shape
{
public:
    void Draw()
    {
        cout << " Polygon Draw ... \n";
    }
    void Clear()
    {
        cout << " Polygon Clear ... \n";
    }
    Shape* Clone()const
    {
        cout << " Polygon Clone ... \n";
        return new Polygon(*this);
    }
private:
    Shape* DoCreate()
    {
        cout << " Polygon Clone ... \n";
        return new Polygon;
    }
};
class Circle : public Shape
{
public:
    void Draw()
    {
        cout << " Circle Draw ... \n";
    }
    void Clear()
    {
        cout << " Circle Clear ... \n";
    }
    Shape* Clone()const
    {
        cout << " Circle Clone ... \n";
        return new Circle(*this);
    }
private:
    Shape* DoCreate()
    {
        cout << " Circle Clone ... \n";
        return new Circle;
    }
};
class Rectangle : public Shape
{
public:
    void Draw()
    {
        cout << " Rectangle Draw ... \n";
    }
    void Clear()
    {
        cout << " Rectangle Clear ... \n";
    }
    Shape* Clone()const
    {
        cout << " Rectangle Clone ... \n";
        return new Rectangle(*this);
    }
private:
    Shape* DoCreate()
    {
        cout << " Rectangle Clone ... \n";
        return new Rectangle;
    }
};

}// namespace TestFactory

namespace TestFactory
{

static  Shape* CreateLine()
{
    return new Line;
}
static  Shape* CreatePolygon()
{
    return new Polygon;
}
static Shape* CreateCircle()
{
    return new Circle;
}
static Shape* ShapeClone(const Shape* shape)
{
    return shape->Clone();
}

}// namespace TestFactory
// --- test MultiMethods ----------------------------------
namespace TestMultiMethods
{
namespace Private
{
template <class SomeLhs, class SomeRhs,
class Executor, typename ResultType>
struct InvocationTraits
{
    static ResultType
    DoDispatch(SomeLhs& lhs, SomeRhs& rhs,
               Executor& exec, Loki::Int2Type<false>)
    {
        return exec.Fire(lhs, rhs);
    }
    static ResultType
    DoDispatch(SomeLhs& lhs, SomeRhs& rhs,
               Executor& exec, Loki::Int2Type<true>)
    {
        return exec.Fire(rhs, lhs);
    }
};
}

////////////////////////////////////////////////////////////////////////////////
// class template StaticDispatcher
// Implements an automatic static double dispatcher based on two typelists
////////////////////////////////////////////////////////////////////////////////

template
<
class Executor,
class BaseLhs,
class TypesLhs,
bool symmetric = true,
class BaseRhs = BaseLhs,
class TypesRhs = TypesLhs,
typename ResultType = void
>
class StaticDispatcher
{
    template <class SomeLhs>
    static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs,
                                  Executor exec, Loki::NullType)
    {
        return exec.OnError(lhs, rhs);
    }

    template <class Head, class Tail, class SomeLhs>
    static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs,
                                  Executor exec, Loki::Typelist<Head, Tail>)
    {
        if (Head* p2 = dynamic_cast<Head*>(&rhs))
        {
            Loki::Int2Type<(symmetric &&
                      int(Loki::TL::IndexOf<TypesRhs, Head>::value) <
                      int(Loki::TL::IndexOf<TypesLhs, SomeLhs>::value))> i2t;

            typedef Private::InvocationTraits<
            SomeLhs, Head, Executor, ResultType> CallTraits;

            return CallTraits::DoDispatch(lhs, *p2, exec, i2t);
        }
        return DispatchRhs(lhs, rhs, exec, Tail());
    }

    static ResultType DispatchLhs(BaseLhs& lhs, BaseRhs& rhs,
                                  Executor exec, Loki::NullType)
    {
        return exec.OnError(lhs, rhs);
    }

    template <class Head, class Tail>
    static ResultType DispatchLhs(BaseLhs& lhs, BaseRhs& rhs,
                                  Executor exec, Loki::Typelist<Head, Tail>)
    {
        if (Head* p1 = dynamic_cast<Head*>(&lhs))
        {
            return DispatchRhs(*p1, rhs, exec, TypesRhs());
        }
        return DispatchLhs(lhs, rhs, exec, Tail());
    }

public:
    static ResultType Go(BaseLhs& lhs, BaseRhs& rhs,
                         Executor exec)
    {
        return DispatchLhs(lhs, rhs, exec, TypesLhs());
    }
};
/*
    template < class Executor,
               class BaseLhs,
               class TypesLhs,
               class BaseRhs ,
               class TypesRhs,
               typename ResultType = void >
    class StaticDispatcher ;

    template < class Executor,
               class BaseLhs,
               class TypesLhs,
               class BaseRhs = BaseLhs, //
               class TypesRhs = TypesLhs, //
               typename ResultType >
    class StaticDispatcher
    {
    private:
        typedef typename TypesLhs::Head Head;
        typedef typename TypesLhs::Tail Tail;
    public:
        template <class SomeLhs>
        static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs, Executor exec)
        {
            if (Head* p2 = dynamic_cast<Head*>(&rhs))
            {
                return exec.Fire(lhs, *p2);

            }
            else
                return StaticDispatcher<Executor, SomeLhs, NullType,  \
                    BaseRhs, Tail>::DispatchRhs(lhs, rhs, exec);
        }
        static ResultType Go(BaseLhs& lhs, BaseRhs& rhs, Executor exec)
        {
            if (Head* p1 = dynamic_cast<Head*>(&lhs))
            {
                return StaticDispatcher<Executor, BaseLhs, NullType,  \
                    BaseRhs, TypesRhs>::DispatchRhs(*p1, rhs, exec);

            }
            else
                return StaticDispatcher<Executor, BaseLhs, Tail,  \
                    BaseRhs, TypesRhs>::Go(lhs, rhs, exec);
        }

    };
    template < class Executor,
               class BaseLhs,
               class TypesLhs,
               class BaseRhs ,
               class TypesRhs,
               typename ResultType >
    class StaticDispatcher<Executor, BaseLhs, NullType,  \
                    BaseRhs, TypesRhs, ResultType>
    {
        static ResultType Go(BaseLhs& lhs, BaseRhs& rhs, Executor exec)
        {
            exec.OnError(lhs, rhs);
        }

    };
    template < class Executor,
               class BaseLhs,
               class TypesLhs,
               class BaseRhs ,
               class TypesRhs,
               typename ResultType >
    class StaticDispatcher<Executor, BaseLhs, TypesLhs,  \
                    BaseRhs, NullType, ResultType>
    {
        static ResultType DispatchRhs(BaseLhs& lhs, BaseRhs& rhs, Executor exec)
        {
            exec.OnError(lhs, rhs);
        }

    }; /*********************/
////////////////////////////////////////////////////////////////////////////////
// class template BasicDispatcher
// Implements a logarithmic double dispatcher for functors (or functions)
// Doesn't offer automated casts or symmetry
////////////////////////////////////////////////////////////////////////////////

template
<
class BaseLhs,
class BaseRhs = BaseLhs,
typename ResultType = void,
typename CallbackType = ResultType (*)(BaseLhs&, BaseRhs&)
>
class BasicDispatcher
{
    typedef Loki::TypeInfo TypeInfo;
    typedef std::pair<TypeInfo,TypeInfo> KeyType;
    typedef CallbackType MappedType;
    typedef Loki::AssocVector<KeyType, MappedType> MapType;
    MapType callbackMap_;

    void DoAdd(TypeInfo lhs, TypeInfo rhs, CallbackType fun);
    bool DoRemove(TypeInfo lhs, TypeInfo rhs);

public:
    template <class SomeLhs, class SomeRhs>
    void Add(CallbackType fun)
    {
        DoAdd(typeid(SomeLhs), typeid(SomeRhs), fun);
    }

    template <class SomeLhs, class SomeRhs>
    bool Remove()
    {
        return DoRemove(typeid(SomeLhs), typeid(SomeRhs));
    }

    ResultType Go(BaseLhs& lhs, BaseRhs& rhs);
};

// Non-inline to reduce compile time overhead...
template <class BaseLhs, class BaseRhs,
typename ResultType, typename CallbackType>
void BasicDispatcher<BaseLhs,BaseRhs,ResultType,CallbackType>
::DoAdd(TypeInfo lhs, TypeInfo rhs, CallbackType fun)
{
    callbackMap_[KeyType(lhs, rhs)] = fun;
}

template <class BaseLhs, class BaseRhs,
typename ResultType, typename CallbackType>
bool BasicDispatcher<BaseLhs,BaseRhs,ResultType,CallbackType>
::DoRemove(TypeInfo lhs, TypeInfo rhs)
{
    return callbackMap_.erase(KeyType(lhs, rhs)) == 1;
}

template <class BaseLhs, class BaseRhs,
typename ResultType, typename CallbackType>
ResultType BasicDispatcher<BaseLhs,BaseRhs,ResultType,CallbackType>
::Go(BaseLhs& lhs, BaseRhs& rhs)
{
    typename MapType::key_type k(typeid(lhs),typeid(rhs));
    typename MapType::iterator i = callbackMap_.find(k);
    if (i == callbackMap_.end())
    {
        throw std::runtime_error("Function not found");
    }
    return (i->second)(lhs, rhs);
}
// ... test class Shape and its Derived class ....
class Shape
{
public:
    virtual void Draw() = 0;
    virtual ~Shape() {}
};
class Rectangle : public Shape
{
public:
    void Draw()
    {
        cout << " Rectangle Draw ... \n";
    }
};
class Poly : public Shape
{
public:
    void Draw()
    {
        cout << " Poly Draw ... \n";
    }
};
class Ellipse : public Shape
{
public:
    void Draw()
    {
        cout << " Ellipse Draw ... \n";
    }
};
class Round : public Shape
{
public:
    void Draw()
    {
        cout << " Round Draw ... \n";
    }
};
// --- HatchingExecutor ----
class HatchingExecutor
{
    // test StaticDispatcher ------------------
public:
    void Fire(Rectangle&, Rectangle&)
    {
        cout << " Fire(Rectangle&, Rectangle&) ... \n";
    }
    void Fire(Poly&, Poly&)
    {
        cout << " Fire(Poly&, Poly&) ... \n";
    }
    void Fire(Ellipse&, Ellipse&)
    {
        cout << " Fire(Ellipse&, Ellipse&) ... \n";
    }
    void Fire(Rectangle&, Poly&)
    {
        cout << " Fire(Rectangle&, Poly&) ... \n";
    }
    void Fire(Rectangle&, Ellipse&)
    {
        cout << " Fire(Rectangle&, Ellipse&) ... \n";
    }
    void Fire(Ellipse&, Poly&)
    {
        cout << " Fire(Ellipse&, Rectangle&) ... \n";
    }
    //void Fire(Poly&, Ellipse&) { cout << " Fire(Poly&, Ellipse&) ... \n"; }

    void OnError(Shape&, Shape&)
    {
        cout << " OnError(Shape&, Shape&) ... \n";
    }
};
// test BasicDispatcher ------------------
static void HatchRectangleEllipse(Shape& lhs, Shape& rhs)
{
    //dynamic_cast<T>(t) error-throw
    Rectangle& rc = dynamic_cast<Rectangle&>(lhs);
    Ellipse& el = dynamic_cast<Ellipse&>(rhs);
    //if (rc!=NULL||el!=NULL) //error
    cout << " HatchRectangleEllipse(Rectangle&, Ellipse&) ... \n";
}
static void HatchEllipseRectangle(Shape& lhs, Shape& rhs)
{
    //dynamic_cast<T>(t) error-throw
    Rectangle& rc = dynamic_cast<Rectangle&>(rhs);
    Ellipse& el = dynamic_cast<Ellipse&>(lhs);
    cout << " HatchEllipseRectangle(Ellipse&, Rectangle&) ... \n";
}
static void HatchRectangleRound(Shape& lhs, Shape& rhs)
{
    //dynamic_cast<T>(t) error-throw
    Rectangle& rc = dynamic_cast<Rectangle&>(lhs);
    Round& el = dynamic_cast<Round&>(rhs);
    cout << " HatchRectangleRound(Rectangle&, Round&) ... \n";
}
namespace
{
// test BasicDispatcher ------------------
void Fire(Rectangle&, Rectangle&)
{
    cout << " Fire(Rectangle&, Rectangle&) ... \n";
}
void Fire(Poly&, Poly&)
{
    cout << " Fire(Poly&, Poly&) ... \n";
}
void Fire(Ellipse&, Ellipse&)
{
    cout << " Fire(Ellipse&, Ellipse&) ... \n";
}
void Fire(Rectangle&, Poly&)
{
    cout << " Fire(Rectangle&, Poly&) ... \n";
}
void RectangleEllipse(Rectangle&, Ellipse&)
{
    cout << " Fire(Rectangle&, Ellipse&) ... \n";
}
void Fire(Ellipse&, Poly&)
{
    cout << " Fire(Ellipse&, Rectangle&) ... \n";
}
//void Fire(Poly&, Ellipse&) { cout << " Fire(Poly&, Ellipse&) ... \n"; }

}

} //namespace TestMultiMethods
//---------------------------------------------------------------------------
#endif //UNIT1_H_
