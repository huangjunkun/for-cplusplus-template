//---------------------------------------------------------------------------


#include <iostream>
#include <string>
#include "unit1.h"
#include "unit2.h"
#include "unit3.h"

#include "..\loki\Singleton.h"

using namespace std;

int main(int argc, char* argv[])
{

    // ---- test  safe_reinterpret_cast<...> (...) ---
    {
    using namespace MySpace;
//    //-- ok ---
//   int     iCast = 100;
//   double dCast = safe_reinterpret_cast<double>(iCast);
//    //[C++ Error] E2031 Cannot cast from 'int' to 'double'
//    double dCast2 = 100;
//    int iCast2 = safe_reinterpret_cast<int>(dCast2);

//     //[C++ Error] E2031 Cannot cast from 'ERROR_Destination_Type_Too_Narrow' to 'CompileTimeChecker<0>'
//     void* somePtr;
//     char ch = safe_reinterpret_cast<char>(somePtr);

    }
    /**
    // --- test AutoCast ---------
    {
    using namespace MySpace;
    AutoCast<int>   iTest1, iTest2(1);
    AutoCast<int>::value_type iValue = iTest2;
    cout << " iValue = " << iValue << "\n";
    if (iTest1)
        cout << " operator bool () --- return true;\n";
    else
        cout << " operator bool () --- return false;\n";
    if (iTest2)
        cout << " operator bool () --- return true;\n";
    else
        cout << " operator bool () --- return false;\n";
    }
    /** --- test SmartPtr ---------
    {
    using namespace MySpace;
    SmartPtr<int>   iSPtr(new int(1000) ); // iSPtr = new int(1000);
    cout << " SmartPtr<int>   iSPtr(new int(1000) ); *iSPtr = " << *iSPtr << endl;
    int*    iPtr = new int(999);
    iSPtr = iPtr;
    cout << " &iSPtr = " << &iSPtr << ", &iPtr = " << &iPtr << endl;
    cout << " *iSPtr = " <<*iSPtr << ", *iPtr = " << *iPtr << endl;
    SmartPtr<Widget<int> >    wSPtr(new Widget<int> );
    cout << " wSPtr->value = " << wSPtr->value << endl;
    cout << " (*wSPtr).value = " << (*wSPtr).value << endl;
    //delete  GetImpl(wSPtr);
    //delete  wSPtr; ///error: ambiguous default type conversion from 'MySpace::SmartPtr<MySpace::Widget<int> >'|

    }
    /**
    // --- test Mapping Integral Constants Types ----
    // ---- NiftyContainer_if_else, NiftyContainer -------
    {
    using namespace MySpace;
    NiftyContainer_if_else<Int,false>   test1; // <int, false> --error
    Int*     ITemp = new Int(999);
    test1.DoSomething(ITemp);
    delete ITemp;
    NiftyContainer<int,false>   test2;
    int*     iTemp = new int(999);
    test2.DoSomething(iTemp);
    delete iTemp;
    }
    // ---- test Local Class and  Interface ----
    {
    using namespace MySpace;
    Interface*  PPtr;
    PPtr = MakeAdapter(100, 100);
    PPtr->Fun();
    }
    /**---
    {
    using namespace MySpace;
    // test Create(...),型别对型别的映射,
    // template<...> friend operator << (...), template class inherit
    MyPair<string>  myPair("huangjunkun");
    MyPair<string>* MPPtr;
    cout << myPair << "\n";  // !!!!!!!!!!!!

    MPPtr = Create(myPair, Type2Type<MyPair<string> >() ); // OKOK
    cout << *MPPtr << "\n";
    delete MPPtr;

    string* strPtr;
    strPtr = Create("Hello, world !", Type2Type<string>() );// okok
    cout << " *strPtr : " << *strPtr << "\n";
    delete strPtr;
    }

    /**
    { // --using namespace TestFunctor; ==========
    using namespace TestFunctor;
    TestFunctor1    t1;
    TestFunctor::Functor<double, TYPELIST_2(int, double)> f1(t1);
    cout << " Functor<double, TYPELIST_2(int, double)> f1(t1);\n";
    cout << f1(9, 9.9 ) << "\n";

    TestFunctor2    t2;
    TestFunctor::Functor<void, TYPELIST_2(int, double)> f2(t2);
    cout << " Functor<void, TYPELIST_2(int, double)> f2(t2);\n";
    f2(9, 9.9);
    TestFunctor::Functor<void, TYPELIST_2(int, double)> f3(TestFunction2);
    cout << " Functor<void, TYPELIST_2(int, double)> f3(TestFunction2);\n";
    f3(9, 9.9);

    TestFunctor::Functor<string, TYPELIST_2(string, string)> f4(*(new StringCat()) );// --ok
    cout << " Functor<string, TYPELIST_2(string, string)> f4(*(new StringCat()) );\n";
    cout << f4("Hello ", "only !") << "\n";
    TestFunctor::Functor<string, TYPELIST_2(string, string)> f5(stringCat);// --ok
    cout << " Functor<string, TYPELIST_2(string, string)> f5(stringCat);\n";
    cout << f5("Hello ", "only !") << "\n";

    //[C++ Error] E2285 Could not find a match for 'auto_ptr<FunctorImpl<void> >::auto_ptr(int *)'
    Parrot  geronimo;
    TestFunctor::Functor<>   f6(&geronimo, &Parrot::Eat),  \
                f7(&geronimo, &Parrot::Speak);
    Parrot  p;
    TestFunctor::Functor<>   f6(&p, &Parrot::ReturnInt);
    //f6();
    //f7();
    }

    /**
    // --- test namespace TestFactory ------------
    {
    using namespace TestFactory;
    typedef Factory<Shape, std::string> ShapeFactory;  //
    typedef CloneFactory<Shape> ShapeCloneFactory;
    ShapeFactory    shapeFactory;
    shapeFactory.Register(typeid(Line).name(), CreateLine);// &Line::Create
    shapeFactory.Register(typeid(TestFactory::Polygon).name(), CreatePolygon);//&TestFactory::Polygon::Create
    shapeFactory.Register(typeid(Circle).name(), CreateCircle);//&Circle::Create

    Shape   *shape1, *shape2, *shape3;
    shape1 = shapeFactory.CreateObject(typeid(Line).name() );
    shape1->Draw();
    shape2 = shapeFactory.CreateObject(typeid(TestFactory::Polygon).name() );
    shape2->Draw();
    shape3 = shapeFactory.CreateObject(typeid(Circle).name() );
    shape3->Draw();

    ShapeCloneFactory   shapeCloneFactory;
    shapeCloneFactory.Register(typeid(Line),ShapeClone);//
    shapeCloneFactory.Register(typeid(TestFactory::Polygon), ShapeClone);//
    shapeCloneFactory.Register(typeid(Circle), ShapeClone);//
    Shape   *shape4, *shape5, *shape6;
    shape4 =  shapeCloneFactory.CreateObject(shape1);
    shape5 =  shapeCloneFactory.CreateObject(shape2);
    shape6 =  shapeCloneFactory.CreateObject(shape3);
    shape4->Draw();
    shape5->Draw();
    shape6->Draw();

    delete shape1;
    delete shape2;Loki::
    delete shape3;
    delete shape4;
    delete shape5;
    delete shape6;
    // test DefaultFactoryError ===================
    Shape   *shape7, *shape8 = new TestFactory::Rectangle;
    try
    {
        shape7 = shapeFactory.CreateObject(typeid(TestFactory::Rectangle).name() );
        shape7->Draw();
        delete shape7;
    }
    catch(exception& e)
    {
        cout << " catch(exception& e): "<< e.what() << "\n";
    }
    try
    {
        shape7 = shapeCloneFactory.CreateObject(shape8);
        shape7->Draw();
        delete shape7;
    }
    catch(exception& e)
    {
        cout << " catch(exception& e): "<< e.what() << "\n";
    }
    }
/**
    {
    cout << " --- test namespace TestMultiMethods --------\n";
    using namespace TestMultiMethods;
    typedef TestMultiMethods::Shape Shape;
    typedef TestMultiMethods::Ellipse Ellipse;
    typedef TestMultiMethods::Rectangle Rectangle;
    Shape* shape1 = new Rectangle;
    Shape* shape2 = new Ellipse;
    Shape* shape3 = new Round;

    {
    cout << " --- test StaticDispatcher -------------\n";

    typedef StaticDispatcher<HatchingExecutor, Shape,
    TYPELIST_3(Rectangle, Ellipse, Poly)> ShapeDispatcher;

    HatchingExecutor    exec;
    ShapeDispatcher::Go(*shape1, *shape2, exec);
    ShapeDispatcher::Go(*shape2, *shape1, exec); //okokok
    ShapeDispatcher::Go(*shape1, *shape3, exec); //OnError

    }// --- test StaticDispatcher ----
    {
    cout << "  --- test BasicDispatcher -------------\n";
    typedef BasicDispatcher<Shape> ShapeDispatcher;
    ShapeDispatcher shapeDispatcher;
    shapeDispatcher.Add<Rectangle,Ellipse>(HatchRectangleEllipse);
    shapeDispatcher.Add<Ellipse,Rectangle>(HatchEllipseRectangle);
    //shapeDispatcher.Add<Rectangle,Round>(HatchRectangleRound);
    try
    {
        shapeDispatcher.Go(*shape1, *shape2);
        shapeDispatcher.Go(*shape2, *shape1);
        shapeDispatcher.Go(*shape1, *shape3);//error-throw
    }
    catch(exception& e)
    {
        cout << " catch(exception& e): "<< e.what() << "\n";
    }

    } // --- test BasicDispatcher ----

    delete shape1;
    delete shape2;
    delete shape3;
    }// --- test namespace TestMultiMethods ---

    /*--------------------------------------------------------------*/
    //cout.fill('-');
    cout << " ------ Hello, World ! \n ";
    getch();
    return 0;
}
//---------------------------------------------------------------------------



