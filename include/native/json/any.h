//
// Copyright (c) 2015 Mike Naquin. All rights reserved.
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

#ifndef NATIVE_JSON_ANY_H__
#define NATIVE_JSON_ANY_H__

#include "native/config.h"

#include "native/istring.h"

#include "native/json/conversion.h"
#include "native/json/types.h"

#include <unordered_map>
#include <vector>

namespace native
{
namespace json
{

// A JSON-like value class.
template <typename String>
class basic_any
{
    using this_type = basic_any<String>;
    using array = std::vector<this_type>;

public:
    using string_type = String;
    using const_iterator = typename array::const_iterator;
    using value_type = this_type;
    using object = std::unordered_map<string_type, this_type>;
    struct handler;
    struct object_key_iterator;
    struct object_value_iterator;
    struct object_item_iterator;

    template <typename T>
    struct object_range;

    // Default constructor. Sets as null.
    basic_any() noexcept;

    // nullptr_t overload. Sets as null.
    basic_any(std::nullptr_t) noexcept;

    // Initialize as the given type with that type's default constructor.
    basic_any(element_type type);

    // Initialize with bool value.
    basic_any(bool value);

    // Initialize with an integral value.
    basic_any(short value);
    basic_any(unsigned short value);
    basic_any(int value);
    basic_any(unsigned value);
    basic_any(long value);
    basic_any(unsigned long value);
    basic_any(long long value);
    basic_any(unsigned long long value);

    // Initialize with a floating point value.
    basic_any(float value);
    basic_any(double value);
    basic_any(long double value);

    // Initialize with a string value.
    basic_any(const char* value);
    basic_any(const string_type& value);

    // Initialize with an array. Works with initializer_list too.
    //    basic_any x{{42, "foo", 5.0, true, nullptr}};
    basic_any(const array& value);
    basic_any(array&& value);

    // Initialize with an object.
    //    basic_any x{{{"foo", 42}, {"bar", 5.0}}};
    basic_any(const object& value);
    basic_any(object&& value);

    basic_any(const basic_any& right);
    basic_any(basic_any&& right) noexcept;

    basic_any& operator=(const basic_any& right);
    basic_any& operator=(basic_any&& right) noexcept;

    ~basic_any();

    // Returns the data type of this object.
    element_type type() const;

    // Easy type checks.
    bool is_null() const;
    bool is_object() const;
    bool is_array() const;
    bool is_bool() const;
    bool is_double() const;
    bool is_int() const;
    bool is_string() const;

    // Returns true if this object is an integral or a floating point.
    bool is_number() const;

    // Return the value of this object as a useful type. If the value of the
    // object
    // can be converted, then a conversion is performed on the specified type.
    //
    // If no conversion can be performed, then std::logic_error is thrown.
    string_type string_value() const;
    long double double_value() const;
    long long int_value() const;
    bool bool_value() const;

    // If this object is a container type (string, array, object) and is empty
    // then true is returned. False otherwise.
    bool empty() const;

    // If this object is a container type, then the number of elements is
    // returned.
    //
    // Otherwise std::logic_error is thrown.
    std::size_t size() const;

    //
    // Array methods. If this object is not an array, then std::logic_error is
    // thrown.
    //
    const_iterator begin() const;
    const_iterator end() const;

    basic_any& front();
    const basic_any& front() const;

    basic_any& back();
    const basic_any& back() const;

    basic_any& operator[](std::size_t index);
    const basic_any& operator[](std::size_t index) const;

    void push_back(const basic_any& value);
    void emplace_back(basic_any&& value);

    void pop_back();

    void resize(std::size_t n, const basic_any& defaultValue = nullptr);

    const_iterator erase(const_iterator it);
    const_iterator erase(const_iterator first, const_iterator last);

    //
    // Object methods. If this object is not an array, then std::logic_error is
    // thrown.
    //
    basic_any& operator[](const string_type& key);
    const basic_any& operator[](const string_type& key) const;

    typename object::const_iterator find(const string_type& key) const;

    std::size_t erase(const string_type& key);

    typename object::const_iterator erase(typename object::const_iterator it);

    object_range<object_key_iterator> keys() const;
    object_range<object_value_iterator> values() const;
    object_range<object_item_iterator> items() const;

    //
    // Deep comparisons.
    //
    bool operator==(const basic_any& right) const;
    bool operator!=(const basic_any& right) const;
    bool operator<(const basic_any& right) const;
    bool operator>(const basic_any& right) const;
    bool operator<=(const basic_any& right) const;
    bool operator>=(const basic_any& right) const;

    // Convert to a JSON string.
    void dump(std::ostream& ostr, bool sort_keys = false,
              std::size_t indent = 0) const;

    string_type dump(bool sort_keys = false, std::size_t indent = 0) const;
    void dump(std::string& str, bool sort_keys = false,
              std::size_t indent = 0) const;

private:
    template <typename Writer>
    void _dump(Writer& writer, bool sort_keys) const;

    void _throw_invalid_type() const;

    element_type _type;
    union data
    {
        data(std::nullptr_t);
        data(bool value);
        data(long long value);
        data(long double value);
        data(const string_type& value);
        data(const array& value);
        data(array&& value);
        data(const object& value);
        data(object&& value);
        data(element_type type, const data& right);
        data(element_type type, data&& right);
        ~data();

        void assign(element_type, const data& right);
        void assign(element_type, data&& right);

        std::nullptr_t _null;
        bool _bool;
        long long _int;
        long double _double;
        string_type _string;
        array _array;
        object _object;
    } _data;
};

using any = basic_any<istring>;

// Initialize by parsing JSON from the given source.
template <typename String, typename IStream>
basic_any<String> parse_stream(IStream& istr);

template <typename String = istring>
typename std::enable_if<is_string_class<String>::value, basic_any<String>>::type
parse(const String& str);

template <typename String = istring, typename Ch = typename String::char_type>
basic_any<String> parse(const Ch* str, std::size_t length);

template <typename String = istring, typename Ch = typename String::char_type,
          std::size_t Size>
basic_any<String> parse(const Ch (&str)[Size]);

} // namespace json
} // namespace native

#include "native/json/detail/any_impl.h"

#endif
