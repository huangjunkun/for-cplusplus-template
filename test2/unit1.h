//---------------------------------------------------------------------------



//#include "unit1.h"


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
//---------------------------------------------------------------------------

namespace MySpace
{
// ---- 构造多态 －－－－－－－－
class AbstractBase
{
    //...
    virtual AbstractBase* Clone() = 0;
};
class Concrete : public AbstractBase
{
    //...
    virtual AbstractBase* Clone()
    {
        return new Concrete(*this);
    }
};

// ---- 简单 SmartPtr 实现 －－－－－－－

class NullPointerException : public std::exception
{
public:
    const char* what() const throw()
    {
        return " Error, NullPointerException.";
    }
};

template <typename T>
class SmartPtr
{
private:
    T*  pointee_;
public:

    SmartPtr() : pointee_(NULL) {} //*T(0)
    explicit SmartPtr(T* pointee) : pointee_(pointee) {}
    SmartPtr(const SmartPtr& other) :
        pointee_(new T(*other.pointee_)) {}

    ~SmartPtr()
    {
        cout << " ~SmartPtr() --- Call automatically ";
        if (pointee_!=NULL)
        {
            delete pointee_;
            cout << ",  delete pointee_ automatically---";
        }
        cout << "\n";
    }
    T&   operator * () const
    {
        return  *pointee_;
    }
    T*   operator -> () const
    {
        return  pointee_;
    }
    T** operator&()
    {
        return &pointee_;
    }
    SmartPtr& operator=(SmartPtr& src)
    {
        if (this != &src)
        {
            delete pointee_;
            pointee_ = src.pointee_;
            src.pointee_ = 0;
        }
        return *this;
    }
    SmartPtr& operator=(T* p)
    {
        if (!p)
            throw NullPointerException();

        if (pointee_ != p)
        {
            delete pointee_;
            pointee_ = p;
            cout << " Call operator=,  delete pointee_ automatically---\n";
        }
        return *this;
    }
// Implicit Conversion to Raw Pointer Types ---- 111111111111
// [C++ Error] E2157 Deleting an object requires exactly one conversion to pointer operator
    operator T*() // User-defined conversion to T*
    {
        return pointee_;
    }
    operator void*() // Added conversion to void*
    {
        return pointee_;
    }
// Implicit Conversion to Raw Pointer Types ---- 222222222222
// ---  No delete SamrtPtr
// delete  sp;// sp --- SmartPtr
//[C++ Error] E2247 'SmartPtr<Widget<int> >::Tester::operator delete(void *)' is not accessible
private:
    class Tester
    {
        void    operator delete (void*);
    };
public:
    operator Tester*() const
    {
        if (!pointee_) return 0;
        static Tester test;
        return &test;
    }
// -----------
    operator bool() const
    {
        return pointee_ != NULL;
    }
    bool operator!() const // Enables "if (!sp) ..."
    {
        return pointee_ == 0;
    }
    inline friend bool operator==(const SmartPtr& lhs,
                                  const T* rhs)
    {
        return lhs.pointee_ == rhs;
    }
    inline friend bool operator==(const T* lhs,
                                  const SmartPtr& rhs)
    {
        return lhs == rhs.pointee_;
    }
    inline friend bool operator!=(const SmartPtr& lhs,
                                  const T* rhs)
    {
        return lhs.pointee_ != rhs;
    }
    inline friend bool operator!=(const T* lhs,
                                  const SmartPtr& rhs)
    {
        return lhs != rhs.pointee_;
    }
    inline friend T*  GetImpl(const SmartPtr& p)
    {
        return  p.pointee_;
    }
    // ...
};
template <typename T >
class Widget
{
private:
    T   value_;
public:
    T   value;
    Widget():value_(0), value(0) {}
} ;
// --------------------------------------------------------------------
template <class T, class U>
class Conversion
{
public:
    typedef char   Small;  //     T
    class   Big
    {
        char Array[2];
    };//Small
    static  Small   Test(U);
    static  Big     Test(...);  // T
    static  T       MakeT();
//public:
//[C++ Error] E2514 Cannot (yet) use member overload resolution during template instantiation
    /*
        enum { exists = sizeof(Test(MakeT()))==sizeof(Small) };
        enum { exists2way = exists && Conversion<U, T>::exists };
        enum { sameType = false };   */
    static const bool exists, exists2way, sameType;
    Conversion()
    {
        exists = sizeof(Test(MakeT()))==sizeof(Small);
        exists2way = exists && Conversion<U, T>::exists;
        sameType = false;
    }
};

template <class T>
class Conversion< T, T >
{
public:
    //enum { exists = true, exists2way = true, sameType = true }; // 1,  1, 1
    static const bool exists, exists2way, sameType;
    Conversion< T, T >()
    {
        exists = true, exists2way = true, sameType = true;
    }
} ;
//static_cast<bool>()
#define SUPERSUBCLASS(T,U)  \
    (Conversion<const U*, const T*>::exixsts && \
    !Conversion<const U*, const T*>::sameType )

#define SUPERSUBCLASS_STRICT(T,U)  \
    (SUPERSUBCLASS(T,U) && \
    !Conversion<const U, const T>::sameType )
// ---- Select -----
//enum Bool { Ture = true, False = false };
template <bool flag, typename T, typename U>
struct Select
{
    typedef T Result;
};
template <typename T, typename U>
struct Select<false, T, U>
{
    typedef U Result;
};

}//namespace MySpace
//---------------------------------------------------------------------------
namespace MyTL
{
// ---- 定义：每一个 Typelist 都必须以NullType 结尾--------
class NullType {};

template <class T, class U>
struct Typelist
{
    typedef T Head;
    typedef U Tail;
};

#define TYPELIST_1(T1) ::MyTL::Typelist<T1, ::MyTL::NullType>

#define TYPELIST_2(T1, T2) ::MyTL::Typelist<T1, TYPELIST_1(T2) >

#define TYPELIST_3(T1, T2, T3) ::MyTL::Typelist<T1, TYPELIST_2(T2, T3) >

#define TYPELIST_4(T1, T2, T3, T4) \
    ::MyTL::Typelist<T1, TYPELIST_3(T2, T3, T4) >

#define TYPELIST_5(T1, T2, T3, T4, T5) \
    ::MyTL::Typelist<T1, TYPELIST_4(T2, T3, T4, T5) >

#define TYPELIST_6(T1, T2, T3, T4, T5, T6) \
    ::MyTL::Typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6) >

#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
    ::MyTL::Typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >

#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
    ::MyTL::Typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
    ::MyTL::Typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
    ::MyTL::Typelist<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

// .....
// as : #define TYPELIST_50(...) ...
// --- 计算Typelist 对象 长度 ---------
template <class TList> struct Length;
template <> struct Length<NullType>
{
    enum { value = 0 };
};
template <class T, class U>
struct Length< Typelist<T, U> >
{
    enum { value = 1+Length<U>::value };
};
// --- 索引式访问 indexed Access---------
template <class Head, unsigned int> struct TypeAt;  //  {}
template <class Head, class Tail>
struct TypeAt< Typelist<Head, Tail>, 0 >
{
    typedef Head Result;
};
template <class Head, class Tail, unsigned int i>
struct TypeAt< Typelist<Head, Tail>, i >
{
    typedef typename TypeAt<Tail, i-1>::Result Result;
};
// --- 查找 Typelist ---------
template <class TList, class T> struct IndexOf ;
template <class T>
struct IndexOf<NullType, T>
{
    enum { value = -1 };
};
template <class Tail, class T>
struct IndexOf<Typelist<T, Tail>, T>
{
    enum { value = 0 };
};
template <class Head, class Tail, class T>
struct IndexOf<Typelist<Head, Tail>, T>
{
private:
    enum { temp = IndexOf<Tail, T>::value };
public:
    enum { value = (temp==-1 ? -1 : 1+temp) };
};

// --- 附加元素至 Typelist ---------
template <class TList, class T> struct Append ;
template <>
struct Append<NullType, NullType>
{
    typedef NullType Result;
};
template <class T>
struct Append<NullType, T>
{
    typedef TYPELIST_1(T)   Result;
};
template <class Head, class Tail>
struct Append<NullType, Typelist<Head, Tail> >
{
    typedef Typelist<Head, Tail> Result;
};
template <class Head, class Tail, class T>
struct Append<Typelist<Head, Tail>,T >
{
    typedef Typelist<Head, typename Append<Tail, T>::Result> Result;
};
// ....
// --- 移除 Typelist 中的某个元素 ---------
template <class TList, class T> struct Erase;

template <class T>                         // Specialization 1
struct Erase<NullType, T>
{
    typedef NullType Result;
};

template <class T, class Tail>             // Specialization 2
struct Erase<Typelist<T, Tail>, T>
{
    typedef Tail Result;
};

template <class Head, class Tail, class T> // Specialization 3
struct Erase<Typelist<Head, Tail>, T>
{
    typedef Typelist<Head,
    typename Erase<Tail, T>::Result>
    Result;
};
template <class TList, class T> struct EraseAll;

template <class T>
struct EraseAll<NullType, T>
{
    typedef NullType Result;
};

template <class T, class Tail>
struct EraseAll<Typelist<T, Tail>, T>
{
    // Go all the way down the list removing the type
    typedef typename EraseAll<Tail, T>::Result Result;
};

template <class Head, class Tail, class T>
struct EraseAll<Typelist<Head, Tail>, T>
{
    // Go all the way down the list removing the type
    typedef Typelist<Head,
    typename EraseAll<Tail, T>::Result>
    Result;
};
// --- 移除重复元素 ---------
template <class TList> struct NoDuplicates;

template <> struct NoDuplicates<NullType>
{
    typedef NullType Result;
};

template <class Head, class Tail>
struct NoDuplicates< Typelist<Head, Tail> >
{
private:
    typedef typename NoDuplicates<Tail>::Result L1;
    typedef typename Erase<L1, Head>::Result L2;
public:
    typedef Typelist<Head, L2> Result;
};

// --- 取代（替换） Typelist 中的某个元素 ---------
template <class TList, class T, class U> struct Replace;

template <class T, class U>
struct Replace<NullType, T, U>
{
    typedef NullType Result;
};

template <class T, class Tail, class U>
struct Replace<Typelist<T, Tail>, T, U>
{
    typedef Typelist<U, Tail> Result;
};

template <class Head, class Tail, class T, class U>
struct Replace<Typelist<Head, Tail>, T, U>
{
    typedef Typelist<Head,
    typename Replace<Tail, T, U>::Result>
    Result;
};

// --- 为 Typelist 局部更换次序 ---------
template <class TList, class T> struct MostDerived;

/* !!!!!!!!!!
template <class T>
struct MostDerived<NullType, T>
{
   typedef T Result;
};
template <class Head, class Tail, class T>
struct MostDerived<Typelist<Head, Tail>, T>
{
private:
   typedef typename MostDerived<Tail, T>::Result Candidate;
public:
   typedef typename Select< SUPERSUBCLASS(Candidate, Head),  // false
         Head, Candidate>::Result Result;
};     */ /*  */
template <class TList, class T>
struct MostDerived;

template <class Head, class Tail, class T>
struct MostDerived<Typelist<Head, Tail>, T>
{
private:
    typedef typename MostDerived<Tail, T>::Result Candidate;

public:
    typedef typename Loki::Select<
    Loki::Conversion<const volatile Head *, const volatile Candidate *>::exists==1 &&
    Loki::Conversion<const volatile Candidate *, const volatile void *>::sameType==0,
    Head, Candidate>::Result Result;
};

template <class T>
struct MostDerived<NullType, T>
{
    typedef T Result;
};

//The DerivedToFront algorithm uses MostDerived as a primitive.
// Here is DerivedToFront's implementation:

template <class T> struct DerivedToFront;

template <>
struct DerivedToFront<NullType>
{
    typedef NullType Result;
};

template <class Head, class Tail>
struct DerivedToFront< Typelist<Head, Tail> >
{
private:
    typedef typename MostDerived<Tail, Head>::Result \
    TheMostDerived;
    typedef typename Replace<Tail,
    TheMostDerived, Head>::Result L;
public:
    typedef Typelist<TheMostDerived, L> Result;
};

// --- 运用 Typelist 自动产生 Classes ---------
template <class TList, template <class> class Unit>
class GenScatterHierarchy;

// GenScatterHierarchy specialization: Typelist to Unit
template <class T1, class T2, template <class> class Unit>
class GenScatterHierarchy<Typelist<T1, T2>, Unit>
    : public GenScatterHierarchy<T1, Unit>
    ,  public GenScatterHierarchy<T2, Unit>
{
public:
    typedef Typelist<T1, T2> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase;
    typedef GenScatterHierarchy<T2, Unit> RightBase;
};

// Pass an atomic type (non-typelist) to Unit
template <class AtomicType, template <class> class Unit>
class GenScatterHierarchy : public Unit<AtomicType>
{
    typedef Unit<AtomicType> LeftBase;
};

// Do nothing for NullType
template <template <class> class Unit>
class GenScatterHierarchy<NullType, Unit>
{
};
} //namespace TL

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
