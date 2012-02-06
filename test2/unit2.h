//---------------------------------------------------------------------------

#ifndef UNIT2_H_
#define UNIT2_H_

#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <assert.h>
#include <memory>
#include <map>

using namespace std;
//---------------------------------------------------------------------------

#include "..\loki\NullType.h"
#include "..\loki\EmptyType.h"
#include "..\loki\Typelist.h"
#include "..\loki\SmallObj.h"
#include "..\loki\TypeInfo.h"
#include "..\loki\AssocVector.h"
#include "..\loki\Functor.h"

//---------------------------------------------------------------------------

namespace MySpace
{

// ----- 模板类 ，利用operator 实现自动类型转换机制（bool ，T） －－－－
template <typename T>
class AutoCast
{
private:
    T   value;
public:
    typedef T value_type;
    operator bool ()
    {
        return value != T(0);
    }/* */
    operator T ()
    {
        return value;
    }
    AutoCast() : value(T(0)) {}
    AutoCast(T v) : value(v) {}
    AutoCast(const AutoCast<T>& a)
    {
        value = a.value;
    }
};

// --- 安全类型转换 ----
template <class To, class From>
To safe_reinterpret_cast_(From from)
{
    assert( sizeof(From)<=sizeof(To));
    return reinterpret_cast<To>(from);
}
template<bool> 
struct CompileTimeChecker
{
	CompileTimeChecker(...) {} ; // 这时C/C++支持的非定量任意参数表
};

template<> 
struct CompileTimeChecker<false> 
{};

#define STATIC_CHECK(expr, msg) \
{\
    class ERROR_##msg {}; \
    (void) sizeof(CompileTimeChecker<(expr)>(ERROR_##msg()) );\
} //// ## 是个罕被使用的C++操作符

template <class To, class From>
To safe_reinterpret_cast(From from)
{
    STATIC_CHECK(sizeof(From) <= sizeof(To), Destination_Type_Too_Narrow);
    //return  reinterpret_cast<To>(from);
	return (To)(from);
};

// --- 常整数映射为型别
// --- if-else --- 编译失败，完全编译if-else 情况 ---
class Int
{
private:
    int*    pointee_;
public:
    Int(){ pointee_ = new int(); }
    Int(int* ptr){ pointee_ = new int(*ptr); }
    Int(int i){ pointee_ = new int(i); }
    ~Int() { delete pointee_;}
    Int* Clone() { return new Int(pointee_);}
    friend ostream& operator << (ostream& os, const Int& I);
};
ostream& operator << (ostream& os, const Int& I)
{
    os << "[" << *I.pointee_ << "]";
    return  os;
}
template <typename T, bool isPolymorphic>
class   NiftyContainer_if_else
{
public:
    void    DoSomething(T* Obj)
    {
        // ...
        T*  pNewObj;
        if (isPolymorphic)
        {
            pNewObj = Obj->Clone();
        // ... non-poltmorphic algorithm ...多态算法
        }
        else
        {
            pNewObj = new T(*Obj);
            cout << " *pNewObj = " << *pNewObj << "\n";
            delete pNewObj;
        // ... non-poltmorphic algorithm ...非多态算法
        }
    }
};
// --- not if-else --- Int2Type --
template <int I>
struct Int2Type
{
    enum { value = I};
};
template <typename T, bool isPolymorphic>
class   NiftyContainer
{
private:
    void DoSomething(T* pObj, Int2Type<true>)
    {
        //T*  pNewObj = pObj->Clone();
        // ... poltmorphic algorithm ...
    }
    void DoSomething(T* pObj, Int2Type<false>)
    {
        T*  pNewObj = new T(*pObj);
        // added by myself ---
        pNewObj = new T(*pObj);
        cout << " *pNewObj = " << *pNewObj << "\n";
        delete pNewObj;
        // ... non-poltmorphic algorithm ...
    }
public:
    void    DoSomething(T* pObj)
    {
        DoSomething(pObj, Int2Type<isPolymorphic>() );
    }
};

//---------------------------------------------------------------------------
// ---- test Local Classes -----------
// --- 1 . in function --------
//template <class T> //const T& lhs, const T& rhs
void Fun()
{ // 当函数内部数据繁多，处理复杂时 ，局部类用途颇大 -----
   class Local
   {
      //... member variables ...
      //... member function definitions ...
   };
   //... code using Local ...
}
// --- 2 . inherit one base class ----------
class Interface
{
public:
   virtual void Fun() = 0;
   //...
};
template <class T, class P>
Interface* MakeAdapter(const T& obj, const P& arg)
{
   class Local : public Interface
   {
   public:
      Local(const T& obj, const P& arg)
         : obj_(obj), arg_(arg) {}
      virtual void Fun()
      {
         //obj_.Call(arg_);
         cout << " obj_: " << obj_ << ", arg_: " << arg_ <<endl;
      }
   private:
      T obj_;
      P arg_;
   };
   return new Local(obj, arg);
}
//---------------------------------------------------------------------------


// --- 型别对型别的映射 ----
template <typename T>
struct Type2Type
{
    typedef T OriginalType;
};

template <typename T, typename U>
T*  Create(const U& arg, Type2Type<T>)
{
    return  new T(arg);
}
 // template  偏特化 ---
template <typename U>
class MyPair : public std::pair<U, int>
{
public:
    MyPair(const U& arg):pair<U,int>(arg, 198867) {}


};

// !!!!!!!!!
template <class T>
ostream& operator << (ostream& os, const MyPair<T>& pair)
{
    os << " pair.first: " << pair.first
       << ", pair.second: " << pair.second;

    return  os;
}

template <typename U>
MyPair<U>*   Create(const U& arg, Type2Type<MyPair<U> >)
{
    return  new MyPair<U>(arg);
}

} //namespace testSpace

#endif //UNIT2_H_
