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
class any
{
    using array = std::vector<any>;

public:
    using const_iterator = array::const_iterator;
    using value_type = any;
    using object = std::unordered_map<istring, any>;
    struct handler;
    struct object_key_iterator;
    struct object_value_iterator;
    struct object_item_iterator;

    template <typename T>
    struct object_range;

    // Default constructor. Sets as null.
    any() noexcept;

    // nullptr_t overload. Sets as null.
    any(std::nullptr_t) noexcept;

    // Initialize as the given type with that type's default constructor.
    any(element_type type);

    // Initialize with bool value.
    any(bool value);

    // Initialize with an integral value.
    any(short value);
    any(unsigned short value);
    any(int value);
    any(unsigned value);
    any(long value);
    any(unsigned long value);
    any(long long value);
    any(unsigned long long value);

    // Initialize with a floating point value.
    any(float value);
    any(double value);
    any(long double value);

    // Initialize with a string value.
    any(const char* value);
    any(const std::string& value);
    any(const istring& value);

    // Initialize with an array. Works with initializer_list too.
    //    any x{{42, "foo", 5.0, true, nullptr}};
    any(const array& value);
    any(array&& value);

    // Initialize with an object.
    //    any x{{{"foo", 42}, {"bar", 5.0}}};
    any(const object& value);
    any(object&& value);

    any(const any& right);
    any(any&& right) noexcept;

    any& operator=(const any& right);
    any& operator=(any&& right) noexcept;

    ~any();

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
    istring string_value() const;
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

    any& front();
    const any& front() const;

    any& back();
    const any& back() const;

    any& operator[](std::size_t index);
    const any& operator[](std::size_t index) const;

    void push_back(const any& value);
    void emplace_back(any&& value);

    void pop_back();

    void resize(std::size_t n, const any& defaultValue = nullptr);

    const_iterator erase(const_iterator it);
    const_iterator erase(const_iterator first, const_iterator last);

    //
    // Object methods. If this object is not an array, then std::logic_error is
    // thrown.
    //
    any& operator[](const istring& key);
    const any& operator[](const istring& key) const;

    object::const_iterator find(const istring& key) const;

    std::size_t erase(const istring& key);

    object::const_iterator erase(object::const_iterator it);

    object_range<object_key_iterator> keys() const;
    object_range<object_value_iterator> values() const;
    object_range<object_item_iterator> items() const;

    //
    // Deep comparisons.
    //
    bool operator==(const any& right) const;
    bool operator!=(const any& right) const;
    bool operator<(const any& right) const;
    bool operator>(const any& right) const;
    bool operator<=(const any& right) const;
    bool operator>=(const any& right) const;

    // Convert to a JSON string.
    void dump(std::ostream& ostr, bool sort_keys = false,
              std::size_t indent = 0) const;

    istring dump(bool sort_keys = false, std::size_t indent = 0) const;
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
        data(const istring& value);
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
        istring _string;
        array _array;
        object _object;
    } _data;
};

// Initialize by parsing JSON from the given source.
template <typename IStream>
any parse_stream(IStream& istr);

template <typename String>
typename std::enable_if<is_string_class<String>::value, any>::type
parse(const String& str);

template <typename Ch>
any parse(const Ch* str, std::size_t length);

template <typename Ch, std::size_t Size>
any parse(const Ch (&str)[Size]);

} // namespace json
} // namespace native

#include "native/json/detail/any_impl.h"

#endif
