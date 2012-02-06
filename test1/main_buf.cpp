


// ---------------------------------------------------------------

// ---------------------------------------------------------------
/*  How can any human hope to understand these overly verbose template-based error messages?
    using namespace std;
    typedef map<int, double> valmap;
    valmap m;
    for (int i = 0; i < NVALS; i++)
        m.insert(make_pair(values[i], pow(values[i], .5)));

    valmap::iterator it = 100;              // error
    valmap::iterator it2(100);              // error
    m.insert(1,2);                          // error      */
/*
    D<int>   *d = new D<int>();
    d->g();
    delete d;
    int i;
    const type_info& info = typeid(i);
    cout << "int i, typeid(i) :Class name: " << info.name() << endl;   */
/*
    foo<int>();
    Foo<int> lhs(1);
    Foo<int> rhs(2);
    Foo<int> result = lhs + rhs;
    std::cout << result << endl;

    using namespace std;
    cout << "" << Conversion<double, int>::exists << endl
         << "" << Conversion<char, char*>::exists << endl
         << "" << Conversion<size_t, vector<int>>::exists << endl;

    cout << "" << Conversion<double, int>::exists << endl;  */
 /*
    int test1;
    char test2;
    cout << "Enter integer: ";
    cin >> test1;
    cin.ignore(255, '\n');
    cout << "Enter character: ";
    cin >> test2;
    cin.ignore(255, '\n');
    rttiTester(test1);
    rttiTester(test2);

    foo(42);        // matches foo(int) exactly
    foo(42.0);      // matches foo(double) exactly
    foo("abcdef");  // matches foo<T>(T*) with T = char         */
// ---------------------------------------------------------------
/*
  #include   "iostream"
  #include   "vector"
  #include   "cassert"
  #include   "algorithm"
  #include   "string.h"
  using   namespace   std;

  template <typename Container>
  Container make(const char s[])
  {
    return   Container(&s[0],&s[strlen(s)]);
  }

  void   main()
  {
    cout<<"Using   reverse   algorithm   with   a   vector"<<endl;
    vector<char>   vector1=make<vector<char>   >("mark   twain");
    reverse(vector1.begin(),vector1.end());
    //assert(vector1==make<vector<char>>("niawt   kram"));
    cout<<"ok"<<endl;
  }
*/
/*
#include <iostream>
#include <typeinfo>
using namespace std; 
template <typename T = {int, char}> 
void rttiTester( T testVar)
{	
  if ( typeid(testVar) == typeid(int) )		
      cout << "Is integer";
  else
      cout << "Is character";
}
void main()
{	
    int test1;
    char test2;
    cout << "Enter integer: ";
    cin >> test1;
    cin.ignore(255, '\n');
    cout << "Enter character: ";
    cin >> test2;
    cin.ignore(255, '\n');
    rttiTester(test1);
    rttiTester(test2);
}
*/
// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------

// ---------------------------------------------------------------
