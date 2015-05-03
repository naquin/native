native
======

The Native Library is intended to provide first class libraries for C++11. The
code here is designed from the ground up with C++11 so that we can make smarter
choices and tradeoffs to provide code that will stand the test of time.

Dependencies
------------

- boost 1.55.0

    Optional. [Download](http://www.boost.org/) and install for benchmarking.

    Lower versions may also work, but are not tested. Use at your own risk.

- googletest (Google C++ Testing Framework)

    Optional. [Download](http://code.google.com/p/googletest/) and install to run the unit tests.

Install
-------

Right now Native only consists of header files, so all that is needed is to
reference the ./include in the include paths for your project.

Requirements
------------

Compiler that supports C++11.

Examples
========

Immutable String
----------------

```
#include "native/istring.h"

using native::istring; // 'i' for immutable

// immutable string
istring s = "some really long string";

// shallow copy, immutable strings can share memory
istring copy = s;

// istring::literal performs no allocations!!!
s = istring::literal("this is a string literal");

auto lambda = [s]()
{
    // efficiently copied into lambda
    // immutable and thread safe - return this lambda anywhere
    std::cout << s << std::endl;
};
```

String Slices
-------------

```
istring s = "foobar";

// take a slice of a string from characters 3 to 6
string_splice splice_s = s(3,6); // "bar"
```

Easy C++11 JSON (DOM-style)
---------------------------

```
auto any = native::json::parse(R"json({
  "array": [
    2,
    "c++",
    7,
    {
      "color": "orange",
      "problems": 99
    }
  ],
  "bar": "string",
  "foo": 42
})json");

any.is_object(); // == true
any["array"][2].string_value(); // == "c++";
any["foo"].int_value(); // == 42;

any.dump(std::cout); // write formatted json to stream

// initialization
json::any object{{{"foo", 42}, {"bar", "string"}}};
json::any array{{42, "hello", true, nullptr}};
```

Super Fast C++11 JSON Parser (SAX-style)
----------------------------------------

```
struct person
{
    string name;
    unsigned short age;
};

struct myhandler: native::json::handler<>
{
    person& _p;

    myhandler(person& p): _p(p) { }

    void value(unsigned short val) { _p.age = value; }
    void value(const char* val, size_t len) { _p.name = val; }
};

auto input = istring::literal("{\"name\": \"jack\", \"age\": 5");
person jack;
myhandler handler(jack);

native::json::parser parser;
parser.parse(input, jack);

jack.name == "jack";
jack.age == 5;
```


Benchmarks
==========

- Mac OS 10.9 2.4 GHz Intel Core i5
- Results taken from fastest performance in 100,000 iterations

![](https://raw.github.com/syvex/native-wiki/master/benchmark-assign.png)

String hashes are cached in `istring` for values that do not fit within
the small string optimization. Therefore, we do not have to recalculate them.

Hashes for strings created with `istring::literal` are computed at compile time.

![](https://raw.github.com/syvex/native-wiki/master/benchmark-istring-hash.png)
