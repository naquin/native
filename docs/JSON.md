C++11 JSON
==========

Many C++ JSON libraries are either not completely type safe or too slow (or
both). native::json aims to provide a modern C++11 interface with
uncompromising performance.

DOM-style
---------

native::json::any is a very simple and generic interface for JSON parsing
and writing.

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

SAX-style
---------

For better performance and complete type safety, use the SAX-style parser.

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
