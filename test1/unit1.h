

#ifndef UNIT1_H_
#define UNIT1_H_

#include <iostream>
#include <vector>

#include <exception>
#include <stdexcept>

#include "..\Loki\Typelist.h"
#include "..\Loki\TypeManip.h"
#include "..\Loki\HierarchyGenerators.h"

using namespace std;


//extern int  X;
//int     Y = X + 1;
//---------------------------------------------------------------------------
// --- template Æ«ÌØ»¯  ---------
template <class Window, class Controller >
class Widget
{
    // ..... generic implementation .....
};
class ModalDialog
{
};
class MyController
{
};
template <>
class Widget<ModalDialog, MyController>
{
    // .... specialized implementation ......
};
// -----------------
// ---- Test TypeTraits -------
// -----1 .  --------------------------
namespace MySpace
{
struct NullType
{
};
template <typename T >
class TypeTraits
{
private:
    template <typename U>
    struct PointerTraits
    {
        enum { result = false };
        typedef NullType PointerType;
    };
    template <typename U>
    struct PointerTraits<U*>
    {
        enum { result = true };
        typedef U PointerType;
    };
    template <typename U>
    struct PToMTraits
    {
        enum { result = false };
    };
    template <typename U, typename V>
    struct PToMTraits<U V::*>
    {
        enum { result = true };
    };
    template <typename U>
    struct UnConst
    {
        typedef U Result;
    };
    template <typename U>
    struct UnConst <const U>
    {
        typedef U Result;
    };
public:
    enum { isPointer = PointerTraits<T>::result };
    typedef typename PointerTraits<T>::PointerType PointerType;
    enum { isMemberPointer = PToMTraits<T>::result };
    typedef typename UnConst<T>::Result  NonConstType;
    // ..... other policies ......

};
}// namespace MySpace

// -----2 .  --------------------------

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
// ---------------------------------------------------------------
namespace MySpace
{
// --- test Conversion  SUPERSUBCLASS SUPERSUBCLASS_STRICT -----
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
    enum { exists = sizeof(Test(MakeT()))==sizeof(Small) };
    enum { exists2way = exists && Conversion<U, T>::exists };
    enum { sameType = false };
};
template <class T>
class Conversion< T, T >
{
public:
    enum { exists = 1, exists2way = 1, sameType = 1 };
} ;
// warning --- define no namespace  restriction ???
#define SUPERSUBCLASS(T,U)  \
        (Conversion<const U*, const T*>::exixsts && \
        !Conversion<const U*, const T*>::sameType )

#define SUPERSUBCLASS_STRICT(T,U)  \
        (SUPERSUBCLASS(T,U) && \
        !Conversion<const U, const T>::sameType )
// --- test Singleton -----
// template<typename T>
class   Singleton
{
    //protected:
private:
    static Singleton*   _pInstance;
    static bool         _destoryed;
    Singleton() {}
    //Singleton(const Singleton& single) {}//...
    Singleton& operator = (const Singleton& single)
    {
        return    *this;    // ...
    }
    static void Create()
    {
        static Singleton theInstance;
        _pInstance = &theInstance;
    }
    static void OnDeadReference()
    {
        throw std::runtime_error("Dead Refrence Deteted ");
    }
    //virtual ~Singleton()
    //{ _pInstance = 0; _destoryed = false; }
public:
    static Singleton& Instance()
    {
        if (!_pInstance)
            if (_destoryed)
                OnDeadReference();
            else
                Create();
        return  *_pInstance;
    }
    bool operator == (const Singleton& s2)
    {
        return  _pInstance == &s2.Instance();
    }
}; // class Singleton


// --- test Acyclic Visitor ----
// -- Visitor part ---
class   BaseVisitor
{
public:
    virtual ~BaseVisitor() { }
};
template <class T, typename R = void >
class   Visitor
{
public:
    typedef R ReturnType;
    virtual ReturnType Visit(T&) = 0; // {}
};
// --- visitable part ----
template <typename R = void>
class   BaseVisitable
{
public:
    typedef R ReturnType;
    virtual ~BaseVisitable() {}
    virtual ReturnType Accpet(BaseVisitor&) = 0; //ReturnType-->> R

protected:
    template <class T>
    static ReturnType AcceptImpl(T& visited, BaseVisitor& guest)//ReturnType-->> R
    {
        // Apply the Acyclic Visitor
        if (Visitor<T>* p = dynamic_cast<Visitor<T>*>(&guest) )
            return  p->Visit(visited);
        return  ReturnType();
    }
};
#define DEFINE_VISITABLE() \
    virtual ReturnType Accpet(BaseVisitor& guest) \
    { return AcceptImpl(*this, guest); }

// --- test Visitor ----
class DocElement : public BaseVisitable<>
{
public:
    DEFINE_VISITABLE();
};
class Paragraph : public DocElement
{
public:
    DEFINE_VISITABLE();
};
class MyConcreteVisitor :
    public BaseVisitor,
    public Visitor<DocElement>,
    public Visitor<Paragraph>
{
public:
    void Visit(DocElement&)
    {
        std::cout << " void Visit(DocElement&)\n";
    }
    void Visit(Paragraph&)
    {
        std::cout << " void Visit(Paragraph&)\n";
    }
};

// --- test CyclicVisitor ----
////////////////////////////////////////////////////////////////////////////////

template <class Head, class Tail, typename R>
class Visitor<Loki::Typelist<Head, Tail>, R>
    : public Visitor<Head, R>, public Visitor<Tail, R>
{
public:
    typedef R ReturnType;
    // using Visitor<Head, R>::Visit;
    // using Visitor<Tail, R>::Visit;
};

template <class Head, typename R>
class Visitor<Loki::Typelist<Head, NullType>, R> : public Visitor<Head, R>
{
public:
    typedef R ReturnType;
    using Visitor<Head, R>::Visit;
};

///////////////////////////////////////////////////////////////////////////////

template <typename R, class TList>
class CyclicVisitor : public Visitor<TList, R>
{
public:
    typedef R ReturnType;
    // using Visitor<TList, R>::Visit;

    template <class Visited>
    ReturnType GenericVisit(Visited& host)
    {
        Visitor<Visited, ReturnType>& subObj = *this;
        return subObj.Visit(host);
    }
};
#define DEFINE_CYCLIC_VISITABLE(SomeVisitor) \
    virtual SomeVisitor::ReturnType Accept(SomeVisitor& guest) \
    { return guest.GenericVisit(*this); }
class DocElement_;
class Paragraph_;
typedef CyclicVisitor < void,
TYPELIST_2(DocElement_, Paragraph_) >  MyCyclicVisitor;

class DocElement_
{
public:
    void virtual Accept(MyCyclicVisitor&) = 0;
};
class Paragraph_ : public DocElement_
{
public:
    DEFINE_CYCLIC_VISITABLE(MyCyclicVisitor);
};
class   MyConcreteVisitor_ : public MyCyclicVisitor
{
public:
    void Visit(Loki::NullType&) // ...
    {
        std::cout << " void Visit(NullType&)\n";
    }
    void Visit(DocElement_&)
    {
        std::cout << " void Visit(DocElement_&)\n";
    }
    void Visit(Paragraph_&)
    {
        std::cout << " void Visit(Paragraph_&)\n";
    }


};
} //namespace MySpace

// ---------------------------------------------------------------
namespace   testVisitor //
{
// --- must declaration ----
class DocElement;
class Paragraph;
class RasterBitmap;
// ------------  class DocElementVisitor  ---------------------------------
class DocElementVisitor
{
    // ....
public:
    virtual void Visit(Paragraph&) = 0;//
    virtual void Visit(RasterBitmap&) = 0;
    // catch-all class XXX : public DocElement {}
    //virtual void Visit(DocElement&) = 0;// ...
};

// ------------  class DocStats  ---------------------------------
class DocStats : public DocElementVisitor
{
private:
    unsigned int chars_, nonBlankChars_, words_, images_;
    // ...
public:
    DocStats()
        :chars_(0), nonBlankChars_(0), words_(0), images_(0)
    {}
    void    addChars(unsigned int addNum)
    {
        chars_ += addNum;
    }
    // ...addWords, addImages ....
    void    addWords(unsigned int addNum)
    {
        words_ += addNum;
    }
    void    addImages(unsigned int addNum)
    {
        images_ += addNum;
    }
    // Dispaly ....
    void    dispaly()
    {
        std::cout << "chars_ :" << chars_
                  << ", nonBlankChars_ :" << nonBlankChars_
                  << ", words_ :" << words_
                  << ", images_ :" << images_ << "\n";
    }
    // ...
    void Visit(Paragraph& par)//virtual
    {
        chars_ += 1; //par.numChars()
        words_ += 1; //par.numWords()
        std::cout << " void Visit(Paragraph& par) \n";
    }
    void Visit(RasterBitmap& ras)//virtual
    {
        images_ += 1;//ras.numImages()
        std::cout << " void Visit(RasterBitmap& ras) \n";
    }

};
// ------------  class DocElement  ---------------------------------
class DocElement
{
public:
    // ....
    virtual void updateStats(DocStats& statistics) = 0;
    //...
    virtual void Accept(DocElementVisitor&) = 0;
};
#define DEFINE_ACCEPTIMPL() \
    void Accept(DocElementVisitor& v) \
    {   v.Visit(*this); }
// ------------  class Paragrap  ---------------------------------
class Paragraph : public DocElement
{
private:
    unsigned int chars_, words_ ;
public:
    Paragraph() : chars_(1), words_(1) {}
    unsigned int numChars()
    {
        return chars_;
    }
    unsigned int numWords()
    {
        return words_;
    }
    // ....
    void updateStats(DocStats& statistics)// ...
    {
        statistics.addChars(chars_);
        statistics.addWords(words_);
    }
    DEFINE_ACCEPTIMPL();
};
// ------------  class RasterBitmap  ---------------------------------
class RasterBitmap : public DocElement
{
private:
    unsigned int images_;
public:
    RasterBitmap() : images_(1) {}
    unsigned int numImages()
    {
        return images_;
    }
public:
    // ....
    void updateStats(DocStats& statistics)// ...
    {
        statistics.addImages(1);
    }
    DEFINE_ACCEPTIMPL();
};
// .... class XXXX : public DocElement {// ....
// .... DEFINE_VISITIMPL(); // ....     }     ....

// ------------  class Document  ---------------------------------
class Document
{
private:
    vector<DocElement*>    vecDocElement;//std::
public:
    void    dispalyStatistics()
    {
        DocStats    statistics;//...
        for (std::size_t i=0; i<vecDocElement.size(); i++)
        {
            vecDocElement[i]->updateStats(statistics);
            // or:
            vecDocElement[i]->Accept(statistics) ;
        }
        statistics.dispaly();
    }
};
}
// ---------------------------------------------------------------
namespace testAbstractFactory
{

template <class T>
class AFUnit//AbstractFactoryUnit
{
public:
    virtual T* DoCreate(Loki::Type2Type<T>) = 0;
    virtual ~AFUnit() { }
};
template <class TList,
template <class> class Unit = AFUnit >
class AbstractFactory : public Loki::GenScatterHierarchy<TList, Unit>
{
public:
    typedef TList   ProductList;
    template <class T>
    T*  Create()
    {
        Unit<T>&    unit = *this;
        return  unit.DoCreate(Loki::Type2Type<T>());
    }
};
// --- OpNewFactoryUnit ---
template <class ConcreteProduct, class Base>
class OpNewFactoryUnit : public Base
{
private:
    typedef typename Base::ProductList BaseProductList;
protected:
    typedef typename BaseProductList::Tail ProductList;
public:
    typedef typename BaseProductList::Head  AbstractProduct;
    ConcreteProduct* DoCreate(Loki::Type2Type<AbstractProduct>)
    {
        return  new ConcreteProduct;
    }
};
// --- PrototypeFactoryUnit ---
template <class ConcreteProduct, class Base>
class PrototypeFactoryUnit : public Base
{
private:
    typedef typename Base::ProductList BaseProductList;
protected:
    typedef typename BaseProductList::Tail ProductList;
public:
    typedef typename BaseProductList::Head  AbstractProduct;
    //private:
protected:
    AbstractProduct*    pPrototype_;

public:
    PrototypeFactoryUnit(AbstractProduct* p=0)
        : pPrototype_(p) {}
    /* --- error, name hiding issues---------------------------
    AbstractProduct* getPrototype() const
    {   return pPrototype_; }
    template <class T>
    void setPrototype(T* pObj)
    {   pPrototype_ = pObj; }
    /*     */
////////////////////////////////////////////////////////////////////////////////
// class template PrototypeFactoryUnit
// Creates an object by cloning a prototype
// There is a difference between the implementation herein and the one described
//     in the book: GetPrototype and SetPrototype use the helper friend
//     functions DoGetPrototype and DoSetPrototype. The friend functions avoid
//     name hiding issues. Plus, GetPrototype takes a reference to pointer
//     instead of returning the pointer by value.
////////////////////////////////////////////////////////////////////////////////
    friend void DoGetPrototype(const PrototypeFactoryUnit& me,
                               AbstractProduct*& pPrototype)
    {
        pPrototype = me.pPrototype_;
    }

    friend void DoSetPrototype(PrototypeFactoryUnit& me,
                               AbstractProduct* pObj)
    {
        me.pPrototype_ = pObj;
    }

    template <class U>
    void getPrototype(AbstractProduct*& p)
    {
        return DoGetPrototype(*this, p);
    }

    template <class U>
    void setPrototype(U* pObj)
    {
        DoSetPrototype(*this, pObj);
    }

    AbstractProduct* DoCreate(Loki::Type2Type<AbstractProduct>)
    {
        assert(pPrototype_); // ...
        return pPrototype_->Clone();
    }
};
// --- test PrototypeFactoryUnit ---
template <class AbstractProduct, class Base>
class MyPFUnit : public PrototypeFactoryUnit<AbstractProduct, Base>
{
public:
    using PrototypeFactoryUnit<AbstractProduct, Base>::pPrototype_;
    AbstractProduct* DoCreate(Loki::Type2Type<AbstractProduct>)
    {
        return pPrototype_ ? pPrototype_->Clone() : 0;
    }
};
// ---- ConcreteFactory ConcreteFactory ------------------
template
<
class AbstractFactory,
template <class, class> class Creator = OpNewFactoryUnit,
class TList = typename AbstractFactory::ProductList
>
class ConcreteFactory
    : public Loki::GenLinearHierarchy<typename Loki::TL::Reverse<TList>::Result,
    Creator, AbstractFactory >
{
public:
    typedef typename AbstractFactory::ProductList ProductList;
    typedef TList ConcreteProductList;
};
// --- test Classes  ----------------------
class Enemy //PlayRole
{
public:
    virtual void showPlayer() = 0;
    virtual Enemy* Clone() = 0;//copy constructor ...
    virtual ~Enemy() {};
};
class Soldier : public Enemy
{
public:
    virtual Soldier* Clone() = 0;//copy constructor ...
};
class SillySoldier : public Soldier
{
public:
    void showPlayer()
    {
        cout << " SillySoldier\n";
    }
    SillySoldier* Clone()
    {
        return new SillySoldier;
    }
};
class BadSoldier : public Soldier
{
public:
    void showPlayer()
    {
        cout << " BadSoldier\n";
    }
};
class Monster : public Enemy
{
public:
    virtual Monster* Clone() = 0;//copy constructor ...
};
class SillyMonster : public Monster
{
public:
    void showPlayer()
    {
        cout << " SillyMonster\n";
    }
    SillyMonster* Clone()
    {
        return new SillyMonster;
    }
};
class BadMonster : public Monster
{
public:
    void showPlayer()
    {
        cout << " BadMonster\n";
    }
};
class SuperMonster : public Enemy
{
public:
    virtual SuperMonster* Clone() = 0;//copy constructor ...
};
class SillySuperMonster : public SuperMonster
{
public:
    void showPlayer()
    {
        cout << " SillySuperMonster\n";
    }
    SillySuperMonster* Clone()
    {
        return new SillySuperMonster;
    }
};
class BadSuperMonster : public SuperMonster
{
public:
    void showPlayer()
    {
        cout << " BadSuperMonster\n";
    }
};

}// namespace testAbstractFactory
// ---------------------------------------------------------------
// ---------------------------------------------------------------

#endif //UNIT1_H_
