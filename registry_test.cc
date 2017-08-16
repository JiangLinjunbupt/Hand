#include "registry.h"
using namespace kurff;
#include <iostream>
using namespace std;
class Foo {
 public:
  explicit Foo(int x) { cout << "Foo " << x <<endl; }
};

DECLARE_REGISTRY(FooRegistry, Foo, int);
DEFINE_REGISTRY(FooRegistry, Foo, int);
#define REGISTER_FOO(clsname) \
  REGISTER_CLASS(FooRegistry, clsname, clsname)

class Bar : public Foo {
 public:
  explicit Bar(int x) : Foo(x) { cout << "Bar " << x << endl; }
};
REGISTER_FOO(Bar);

class AnotherBar : public Foo {
 public:
  explicit AnotherBar(int x) : Foo(x) {
    cout << "AnotherBar " << x << endl;
  }
};
REGISTER_FOO(AnotherBar);

int main(){
    unique_ptr<Foo> ptr = FooRegistry()->Create("Bar",1);

    unique_ptr<Foo> p = FooRegistry()->Create("AnotherBar",4);
    return 0;
}

