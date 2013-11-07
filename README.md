native
======

The Native Library is intended to provide first class libraries for C++11. The
code here is designed from the ground up with C++11 so that we can make smarter
choices and tradeoffs to provide code that will stand the test of time.

Dependencies
------------

- boost 1.54.0

    Hopefully you are already using boost. If not, download and install from http://www.boost.org/.
    
    Lower versions may also work, but are not tested. Use at your own risk.

- googletest (Google C++ Testing Framework)

    Optional.
    
    Download and install to run the unit tests.
    http://code.google.com/p/googletest/

Install
-------

Right now Native only consists of header files, so all that is needed is to
reference the ./include in the include paths for your project.

Requirements
------------

Compiler that supports C++11.

Examples
--------

```
#include "native/istring.h"

using native::istring;
using native::string_slice;

// immutable string
istring s = "some really long string";

// shallow copy, immutable strings can share memory
istring copy = s;

// string::literal performs no allocations!!!
s = istring::literal("this is a string literal");

// substr also performs no allocations, instead returns a string_slice
string_slice slice = s.substr(5, 4);

auto lambda = [s]()
{
    // efficiently copied into lambda
    // immutable and thread safe - return this lambda anywhere
    std::cout << s << std::endl;
};
```
