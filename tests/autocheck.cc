#include <xdrpp/autocheck.h>
#include <xdrpp/printer.h>
#include <xdrpp/marshal.h>
#include "tests/xdrtest.hh"

using namespace std;
using namespace xdr;

int
main()
{

  autocheck::generator<bool>{}(100);

  {
    autocheck::generator<test_recursive> g;
    for (size_t i = 0; i < 100; i++) {
      auto x = g(i);
      string name = "size " + to_string(i);
      // cout << xdr_to_string(x, name.c_str());
    }
  }

  {
    autocheck::generator<testns::numerics> g;
    for (size_t i = 0; i < 100; i++) {
      auto x = g(i), y = g(i);
      assert (x == x);
      assert (y == y);
      auto m1 = xdr_to_msg(x), m2 = xdr_to_msg(y);
      bool eq = (m1->size() == m2->size()
		 && !memcmp(m1->data(), m2->data(), m1->size()));
      cout << boolalpha << eq << ':' << endl
	   << xdr_to_string(x, "x", 4)
	   << xdr_to_string(y, "y", 4);
      assert (eq == (x == y));
    }
  }
  
  return 0;
}
