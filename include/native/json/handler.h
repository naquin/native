//
// Copyright (c) 2013 Mike Naquin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef NATIVE_JSON_HANDLER_H__
#define NATIVE_JSON_HANDLER_H__

#include "native/config.h"

#include "native/json/types.h"

namespace native
{
namespace json
{

// When JSON input is parsed, the types are passed into the defined handler.
//
// When a key is parsed, the key() method will be called before the value
// is parsed. This gives us the opportunity to check the key for an expected
// data type. Here we can be smart about our type checking. If you expect a
// uint16_t value, it's bounds are also checked.
//
// If the expected type is unknown, then the value will be parsed as either
// a 32-bit integer first. If it's too big to fit as a 32-bit integer, then
// a 64-bit integer is used. The same goes for floating point types.
//
// If a value cannot be converted, then a std::range_error is thrown.
//
// Note that a handler does not have to derive from this handler. This means
// that we can use template methods to pull out the values.
//
//     struct simple_handler
//     {
//         using char_type = char;
//
//         data_type start_array() { return type_unknown; }
//         void end_array() {}
//
//         void start_object() {}
//         void end_object() {}
//
//         data_type key(const char_type* key, std::size_t length)
//         {
//             return type_unknown;
//         }
//
//         void value(const char_type* val, std::size_t length) {}
//         void value(std::nullptr_t) {}
//         void value(bool val) {}
//
//         // parse any integer
//         template <typename T>
//         std::enable_if<std::is_integral<T>::value, void>::type
//         value(T val) {}
//
//         // parse any floating point
//         template <typename T>
//         std::enable_if<std::is_floating_point<T>::value, void>::type
//         value(T val) {}
//     };
template <typename Ch = char>
class handler
{
public:
    using char_type = Ch;

    handler() = default;

    // Return expected data type or type_unknown if not known.
    data_type start_array() { return type_unknown; }
    void end_array() {}

    void start_object() {}
    void end_object() {}

    // This method is called when a key is parsed.
    //
    // The character pointer to key will remain valid until the next key is
    // parsed. This means that it is safe to store for data types, but not
    // object types!
    data_type key(const char_type* key, std::size_t length)
    {
        return type_unknown; // return the expected type
    }

    void value(const char_type* val, std::size_t length) {}
    void value(std::nullptr_t) {}
    void value(bool val) {}
    void value(short val) {}
    void value(unsigned short val) {}
    void value(int val) {}
    void value(unsigned val) {}
    void value(long val) {}
    void value(unsigned long val) {}
    void value(long long val) {}
    void value(unsigned long long val) {}
    void value(float val) {}
    void value(double val) {}
    void value(long double val) {}
};

} // namespace json
} // namespace native

#endif
