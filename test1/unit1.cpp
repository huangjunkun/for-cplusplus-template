

//---------------------------------------------------------------------------

#include <typeinfo>
#include "unit1.h"

//---------------------------------------------------------------------------

// ---  ----
MySpace::Singleton*   MySpace::Singleton::_pInstance;
bool         MySpace::Singleton::_destoryed;


//---------------------------------------------------------------------------

template<typename T>
void foo()
{
    std::cout << "Here I am!\n";
}
//Second Re --- [Linker Error] Unresolved external 'void foo<int>()' referenced from F:\C++\TEMPLE TEST\UNITMAIN.OBJ
template void foo<int>();  //

template<typename T>
extern void foo();
//First Re --- [Linker Error] Unresolved external 'void foo<int>()' referenced from F:\C++\TEMPLE TEST\UNITMAIN.OBJ

//using  namespace std;
/*
template<typename T>
void foo()
{
  std::cout << "   Foo, Here I am!\n";
}  */
template<typename T> class Foo;  // pre-declare the template class itself
template<typename T> Foo<T> operator+ (const Foo<T>& lhs, const Foo<T>& rhs);
template<typename T> std::ostream& operator<< (std::ostream& o, const Foo<T>& x);

template<typename T>
class Foo
{
public:
    Foo(const T& value = T());
    friend Foo<T> operator+ <> (const Foo<T>& lhs, const Foo<T>& rhs);
    friend std::ostream& operator<< <> (std::ostream& o, const Foo<T>& x);
private:
    T value_;
};
/* /// ---First method ---    */
template<typename T>
Foo<T>::Foo(const T& value )  //   = T()
    : value_(value)
{  } // value_ = value;

template<typename T>
Foo<T> operator+ (const Foo<T>& lhs, const Foo<T>& rhs)
{
    return Foo<T>(lhs.value_ + rhs.value_);
}

template<typename T>
std::ostream& operator<< (std::ostream& o, const Foo<T>& x)
{
    return o << x.value_;
}

template<typename T>
class Base
{
// private:
protected:
    T   val;
public:
    Base() : val(T(0)) {}
    Base(const T& x) : val(x) {}
    virtual void Print()
    {
        cout << " Base::val = " << val <<endl;
    }
} ;
template<typename T>
class Derive : public Base<T>
{
public:
    using Base<T>::val;

    Derive() : Base<T>() {}
    Derive(const T& x) : Base<T>(x) {}
    virtual void Print()
    {
        cout << " Derive::val = " << val <<endl;
    }
};

// ---------------------------------------------------------------
template <typename T > //  = {int, char}
void rttiTester( T testVar)
{
    if ( typeid(testVar) == typeid(int) )
        cout << "Is integer" <<endl;
    else
        cout << "Is character"<<endl;
}
// ---------------------------------------------------------------
// --- templates are overloading ---
template<typename T> void foo(T* x)
{
    std::cout << "foo<" << typeid(T).name() << ">(T*)\n";
}

void foo(int x)
{
    std::cout << "foo(int)\n";
}

void foo(double x)
{
    std::cout << "foo(double)\n";
}
// ---------------------------------------------------------------
const int values[] = { 1,2,3,4,5 };
const int NVALS = sizeof values / sizeof (int);

// ---------------------------------------------------------------
template<typename T>
class D;

class Xyz
{
    template<typename T>
    friend class D;

    Xyz()
    {
        cout << "global (namespace scope) type -- Xyz()" << endl;
    }
    void    Show()
    {
        cout << "type nested in class B<T>" << endl;
    }
};  //¡û global ("namespace scope") type

void f()
{
    cout << "global (namespace scope) type -- f()" << endl;
}        //¡û global ("namespace scope") function
template<typename T>
class B
{
public:
    class Xyz
    {
        /* [C++ Error] E2247 'B<int>::Xyz::Xyz()' is not accessible
        Xyz()
        { cout << "type nested in class B<T>" << endl; }  */
        void    Show()
        {
            cout << "type nested in class B<T>" << endl;
        }
    };  //¡û type nested in class B<T>
    void f()
    {
        cout << "member of class B<T> -- f()" << endl;
    }         //¡û member of class B<T>
};

template<typename T>
class D : public B<T>
{
public:
    void g()
    {
        Xyz x;  //¡û suprise: you get the global Xyz!!! ????
        //x.Show();
        f();    //¡û suprise: you get member of class B<T>'s f(), not the global's !!
    }
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
