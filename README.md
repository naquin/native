native
======

The Native Library is intended to provide first class libraries for C++11. The
code here is designed from the ground up with C++11 so that we can make smarter
choices and tradeoffs to provide code that will stand the test of time.

Requirements
------------

Compiler that supports C++11.

Install
-------

This library is header-only, so just reference the ./include in the include paths for your project.

Unit Test Dependencies
----------------------

- boost >= 1.55.0

    [Download](http://www.boost.org/) and install for benchmarking.

    Lower versions may also work, but are not tested. Use at your own risk.

- googletest (Google C++ Testing Framework)

    [Download](http://code.google.com/p/googletest/) and install to run the unit tests.

Docs
----
- [native::istring](docs/Strings.md)
- [native::json](docs/JSON.md)
