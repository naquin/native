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

Docs
----
- [native::istring](docs/Strings.md)
- [native::json](docs/JSON.md)
