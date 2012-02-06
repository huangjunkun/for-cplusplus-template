//---------------------------------------------------------------------------


#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include <map>
#include <algorithm>
#include <vector>
#include <deque>
#include "..\Loki\Functor.h"
#include "..\Loki\AbstractFactory.h"

using namespace std;

#include "unit1.h"



//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    {
        // --- test Acyclic and Cyclic Visitor ----
        cout << " --- test Acyclic and Cyclic Visitor ----\n";
        using namespace MySpace;
        MyConcreteVisitor   visitor1;
        Paragraph       par;
        DocElement* d = &par;
        d->Accpet(visitor1);

        MyConcreteVisitor_   visitor2;
        Paragraph_      par_;
        DocElement_* d_ = &par_;
        d_->Accept(visitor2);
    }
    {
        // --- test namespace testVisitor ----
        cout << " \n";
        using namespace testVisitor;
        DocStats    docVisitor;
        Paragraph   par;
        RasterBitmap    ras;
        DocElement* d = &par;
        d->Accept(docVisitor);
        d = &ras;
        d->Accept(docVisitor);
    }
    {
        /*
            using namespace Loki;
            typedef testAbstractFactory::Soldier Soldier ;
            typedef testAbstractFactory::Monster Monster ;
            typedef testAbstractFactory::SuperMonster SuperMonster ;
            typedef testAbstractFactory::SillySoldier SillySoldier ;
            typedef testAbstractFactory::SillyMonster SillyMonster ;
            typedef testAbstractFactory::SillySuperMonster SillySuperMonster ;
            */
        using namespace testAbstractFactory;
        typedef AbstractFactory
        < TYPELIST_3(testAbstractFactory::Soldier, Monster, SuperMonster) >
        EnemyAbstractFactory;
        // --- test OpNewFactoryUnit ----
        typedef ConcreteFactory
        < EnemyAbstractFactory, //...
        OpNewFactoryUnit,
        TYPELIST_3(SillySoldier, SillyMonster, SillySuperMonster) >
        EasyLevelEnemyFactory1;
        //EasyLevelEnemyFactory   *pELEF = new EasyLevelEnemyFactory;
        // Monster*    pMonster = pELEF->Create<SillyMonster>();
        EnemyAbstractFactory    *pEAF1 = new EasyLevelEnemyFactory1;

        Soldier*    pSoldier1 = pEAF1->Create<Soldier>();//SillySoldier--error
        pSoldier1->showPlayer();
        Monster*    pMonster1 = pEAF1->Create<Monster>();//SillyMonster--error
        pMonster1->showPlayer();
        SuperMonster*    pSuperMonster1 = pEAF1->Create<SuperMonster>();//SillySuperMonster--error
        pSuperMonster1->showPlayer();
        Soldier*    pS = pSoldier1->Clone();//test Clone .okokok
        pS->showPlayer();
        delete pS;

        // --- test PrototypeFactoryUnit ----
        typedef ConcreteFactory
        < EnemyAbstractFactory, //...
        PrototypeFactoryUnit   //MyPFUnit
        //TYPELIST_3(SillySoldier, SillyMonster, SillySuperMonster)
        >
        EasyLevelEnemyFactory2;
        //EasyLevelEnemyFactory   *pELEF;
        EasyLevelEnemyFactory2* pEAF2 = new EasyLevelEnemyFactory2;
        /*    */
        pEAF2->setPrototype<Soldier>(pSoldier1);// error ???
        Soldier* pSoldier2 = pEAF2->Create<Soldier>();
        pSoldier2->showPlayer();
        pEAF2->setPrototype<Monster>(pMonster1); // error ???
        Monster* pMonster2 = pEAF2->Create<Monster>();//
        pMonster2->showPlayer();
        pEAF2->setPrototype<SuperMonster>(pSuperMonster1);
        SuperMonster* pSuperMonster2 = pEAF2->Create<SuperMonster>();//
        pSuperMonster2->showPlayer();

        delete pSoldier1;
        delete pMonster1;
        delete pSuperMonster1;
        delete pEAF1;//...

        delete pSoldier2;
        delete pMonster2;
        delete pSuperMonster2;
        delete pEAF2;
    }
    /*
        {
        using namespace testAbstractFactory;
        typedef AbstractFactory
        < TYPELIST_3(Soldier, Monster, SuperMonster) >
        EnemyAbstractFactory;

        typedef ConcreteFactory
        < EnemyAbstractFactory, //...
          OpNewFactoryUnit,
          TYPELIST_3(SillySoldier, SillyMonster, SillySuperMonster) >
        EasyLevelEnemyFactory;
        EasyLevelEnemyFactory   *pELEF;
        EnemyAbstractFactory    *pEAF;

        //Monster*    pMonster = pEAF->Create<SillyMonster>();
        Monster*    pMonster = pELEF->Create<SillyMonster>();

        }
    /*
        //cout << " X: "<<X <<", Y: "<<Y <<"\n";//X=1, Y=2
        {//// --- test Singleton ----
        using namespace MySpace;
        Singleton   s1 = Singleton::Instance();
        Singleton   s2 = Singleton::Instance();
        if (s1==s1)
            cout << " Singleton is defined OK ! \n";
        }
    /*
        {
        bool iterIsPtr = TypeTraits<vector<int>::iterator>::isPointer;
        cout << " vector<int>::iterator is " << (iterIsPtr ? "fast" : "smart") << '\n';

        iterIsPtr = TypeTraits<deque<int>::iterator>::isPointer;
        cout << " deque<int>::iterator is " << (iterIsPtr ? "fast" : "smart") << '\n';
        }

        { // ---- test Loki::Functor ----
        using namespace Loki;
        Parrot  geronimo;
        Functor<>   f6(&geronimo, &Parrot::Eat),  \
                    f7(&geronimo, &Parrot::Speak);
        f6();
        f7();
        }
        /*
    //----- ƒ£∞Â¿‡ ∂‡Ã¨ --------------
        Base<int>*   ib1 =  new Base<int>() ;
        Derive<int>*    id1 = new Derive<int>();
        cout << " ----------ib1->Print();--------------\n";
        ib1->Print();
        cout << " ----------id1->Print();--------------\n";
        id1->Print();
        cout << " ----------ib1 = id1; ib1->Print();--------------\n";
        ib1 = id1;
        ib1->Print();
        id1 = reinterpret_cast<Derive<int>*>(ib1);
        cout << " ---id1 = reinterpret_cast<Derive<int>*>(ib1);--\n";
        id1->Print();
        const char str[]  = "1111";
        cout << str <<endl;
        //str[0] = '2';
     // ----------------------------------- */
// ---------------------------------------------------------------
    cout << "   Hello, World !" << endl;
    getch();
    return 0;
}
//---------------------------------------------------------------------------

// ---------------------------------------------------------------

