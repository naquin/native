Strings
========

What's wrong with std::string?
------------------------------

While std::string is perfectly servicable, there are many drawbacks to it being
a mutable container. Many people have written customized string classes over
the years--for various reasons--but chiefly among them is efficiency.

C++ developers don't use C++ to write quick disposable code. C++ code is meant to
maximize the use of your hardware at a high level of abstraction.

More than any other container, it seems that std::string falls short of this
ideal.

Why should we use an immutable string?
--------------------------------------

Many other less efficient languages define their string types as immutable for
very good reasons.

- *Thread safety:* Since immutable strings are thread safe, they are safe to be
  used in other threads without having to make a deep copy. No need to worry
  about moving a string, or wrapping it in a std::shared_ptr to go between
  lambdas.
- *Less overhead:* Without needing to worry about reallocating memory when the
  string grows, the implementation can be more efficient. Less memory is used
  per string (no need to worry about capacity and such), and there's no need
  to allocate more memory than necessary.
- *Hashing:* Since the string will never change, we can compute the hash once
  and save ourselves the need to re-hash every time we need it. For longer
  strings this is a hugh win. Also, as an added benefit, the hash of a string
  literal is known _at compile time_.
- *Slicing or substrings:* Given their immutability, strings can be efficiently
  "sliced" without having to allocate extra memory and copy over the relevant
  data. We can take a substring almost for free!

What good is native::string_builder over std::string or std::ostringstream?
---------------------------------------------------------------------------

Since we are using an immutable string, we can't compose a string within one
class like std::string does. That means that `operator+=` overloads won't do
us any good.

Also, since we want to avoid unnecessary copies and other inefficiencies,
we want to avoid having to copy values from std::ostringstream.

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

Hashing
-------

```
std::unordered_map<native::istring, int> hash_table{{
  // ...
  {"some key", 42},
  // ...
}};

// hash for istring "some key" only done once
auto value1 = hash_table["some key"];
auto value2 = hash_table["some key"];
auto value3 = hash_table["some key"];


// even better - the key is only hashed once
native::istring key{"some key"};

value1 = hash_table["some key"];
value2 = hash_table["some key"];
value3 = hash_table["some key"];

// best - the key is only hashed at compile time
auto key = native::istring::literal("some key");

value1 = hash_table["some key"];
value2 = hash_table["some key"];
value3 = hash_table["some key"];
```

String Builder
--------------


Benchmarks
==========

- Mac OS 10.9 2.4 GHz Intel Core i5
- Results taken from fastest performance in 100,000 iterations

![](https://raw.github.com/syvex/native-wiki/master/benchmark-assign.png)

String hashes are cached in `istring` for values that do not fit within
the small string optimization. Therefore, we do not have to recalculate them.

Hashes for strings created with `istring::literal` are computed at compile time.

![](https://raw.github.com/syvex/native-wiki/master/benchmark-istring-hash.png)
