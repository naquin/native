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

#include "native/json/writer.h"
#include "native/json/parser.h"
#include "native/string_builder.h"
#include "native/detail/stream_type.h"

namespace native
{
namespace json
{

template <typename String>
struct basic_any<String>::object_key_iterator
{
    using value_type = String;

    object_key_iterator(
        typename basic_any<String>::object::const_iterator element)
        : _element{element}
    {
    }

    const value_type& operator*() const { return _element->first; }
    const value_type* const operator->() const { return &_element->first; }

    object_key_iterator& operator++(int)
    {
        ++_element;
        return *this;
    }

    object_key_iterator operator++()
    {
        const auto result = *this;
        ++_element;
        return result;
    }

    bool operator==(const object_key_iterator& right) const
    {
        return _element == right._element;
    }

    bool operator!=(const object_key_iterator& right) const
    {
        return _element != right._element;
    }

private:
    typename basic_any<String>::object::const_iterator _element;
};

template <typename String>
struct basic_any<String>::object_value_iterator
{
    using value_type = basic_any;

    object_value_iterator(
        typename basic_any<String>::object::const_iterator element)
        : _element{element}
    {
    }

    const value_type& operator*() const { return _element->second; }
    const value_type* const operator->() const { return &_element->second; }

    object_value_iterator& operator++(int)
    {
        ++_element;
        return *this;
    }

    object_value_iterator operator++()
    {
        const auto result = *this;
        ++_element;
        return result;
    }

    bool operator==(const object_value_iterator& right) const
    {
        return _element == right._element;
    }

    bool operator!=(const object_value_iterator& right) const
    {
        return _element != right._element;
    }

private:
    typename basic_any<String>::object::const_iterator _element;
};

template <typename String>
struct basic_any<String>::object_item_iterator
{
    using value_type = typename basic_any<String>::object::value_type;

    object_item_iterator(
        typename basic_any<String>::object::const_iterator element)
        : _element{element}
    {
    }

    const value_type& operator*() const { return *_element; }
    const value_type* const operator->() const { return &(*_element); }

    object_item_iterator& operator++(int)
    {
        ++_element;
        return *this;
    }

    object_item_iterator operator++()
    {
        const auto result = *this;
        ++_element;
        return result;
    }

    bool operator==(const object_item_iterator& right) const
    {
        return _element == right._element;
    }

    bool operator!=(const object_item_iterator& right) const
    {
        return _element != right._element;
    }

private:
    typename basic_any<String>::object::const_iterator _element;
};

template <typename String>
template <typename Iterator>
struct basic_any<String>::object_range
{
    using const_iterator = Iterator;
    using value_type = typename Iterator::value_type;

    object_range(const basic_any<String>::object* object = nullptr)
        : _object(object)
    {
    }

    Iterator begin() const { return _object->begin(); }

    Iterator end() const { return _object->end(); }

private:
    const basic_any<String>::object* _object;
};

template <typename String>
inline basic_any<String>::data::data(std::nullptr_t)
    : _null{nullptr}
{
}

template <typename String>
inline basic_any<String>::data::data(bool value)
    : _bool{value}
{
}

template <typename String>
inline basic_any<String>::data::data(long long value)
    : _int{value}
{
}

template <typename String>
inline basic_any<String>::data::data(long double value)
    : _double{value}
{
}

template <typename String>
inline basic_any<String>::data::data(const String& value)
    : _string{value}
{
}

template <typename String>
inline basic_any<String>::data::data(const array& value)
    : _array{value}
{
}

template <typename String>
inline basic_any<String>::data::data(array&& value)
    : _array{}
{
    _array.reserve(value.size());
    for (auto& element : value)
    {
        _array.emplace_back(std::move(element));
    }
}

template <typename String>
inline basic_any<String>::data::data(const object& value)
    : _object{value}
{
}

template <typename String>
inline basic_any<String>::data::data(object&& value)
    : _object{std::move(value)}
{
}

template <typename String>
inline basic_any<String>::data::data(element_type type,
                                     const basic_any<String>::data& right)
{
    assign(type, right);
}

template <typename String>
inline basic_any<String>::data::data(element_type type,
                                     basic_any<String>::data&& right)
{
    assign(type, std::move(right));
}

template <typename String>
void basic_any<String>::data::assign(element_type type,
                                     const basic_any<String>::data& right)
{
    switch (type)
    {
        case json_null:
            _null = nullptr;
            break;
        case json_bool:
            _bool = right._bool;
            break;
        case json_integer:
            _int = right._int;
            break;
        case json_real:
            _double = right._double;
            break;
        case json_string:
            new (&_string) string_type{right._string};
            break;
        case json_array:
            new (&_array) array;
            _array.assign(right._array.begin(), right._array.end());
            break;
        case json_object:
            new (&_object) object{right._object};
            break;
    }
}

template <typename String>
void basic_any<String>::data::assign(element_type type,
                                     basic_any<String>::data&& right)
{
    switch (type)
    {
        case json_null:
            _null = nullptr;
            break;
        case json_bool:
            _bool = right._bool;
            break;
        case json_integer:
            _int = right._int;
            break;
        case json_real:
            _double = right._double;
            break;
        case json_string:
            new (&_string) string_type{right._string};
            break;
        case json_array:
            new (&_array) array;
            _array.reserve(right._array.size());
            for (auto& element : right._array)
            {
                _array.emplace_back(std::move(element));
            }
            break;
        case json_object:
            new (&_object) object{std::move(right._object)};
            break;
    }
}

template <typename String>
basic_any<String>::data::~data()
{
}

template <typename String>
inline basic_any<String>::basic_any() noexcept : _type{json_null},
                                                 _data{nullptr}
{
}

template <typename String>
basic_any<String>::basic_any(element_type type)
    : _type{type}
    , _data{nullptr}
{
    switch (type)
    {
        case json_null:
            _data._null = nullptr;
            break;
        case json_bool:
            _data._bool = false;
            break;
        case json_integer:
            _data._int = 0;
            break;
        case json_real:
            _data._double = 0.0;
            break;
        case json_string:
            new (&_data._string) string_type;
            break;
        case json_array:
            new (&_data._array) array;
            break;
        case json_object:
            new (&_data._object) object;
            break;
    }
}

template <typename String>
inline basic_any<String>::basic_any(std::nullptr_t) noexcept : _type{json_null},
                                                               _data{nullptr}
{
}

template <typename String>
inline basic_any<String>::basic_any(bool value)
    : _type(json_bool)
    , _data{value}
{
}

template <typename String>
inline basic_any<String>::basic_any(short value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(unsigned short value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(int value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(unsigned value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(unsigned long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(long long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(unsigned long long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(float value)
    : _type{json_real}
    , _data{static_cast<long double>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(double value)
    : _type{json_real}
    , _data{static_cast<long double>(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(long double value)
    : _type{json_real}
    , _data{value}
{
}

template <typename String>
inline basic_any<String>::basic_any(const char* value)
    : basic_any{string_type{value}}
{
}

template <typename String>
inline basic_any<String>::basic_any(const string_type& value)
    : _type{json_string}
    , _data{value}
{
}

template <typename String>
inline basic_any<String>::basic_any(const array& value)
    : _type{json_array}
    , _data{value}
{
}

template <typename String>
inline basic_any<String>::basic_any(array&& value)
    : _type{json_array}
    , _data{std::move(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(const object& value)
    : _type{json_object}
    , _data{value}
{
}

template <typename String>
inline basic_any<String>::basic_any(object&& value)
    : _type{json_object}
    , _data{std::move(value)}
{
}

template <typename String>
inline basic_any<String>::basic_any(const basic_any<String>& right)
    : _type{right._type}
    , _data{right._type, right._data}
{
}

template <typename String>
inline basic_any<String>::basic_any(basic_any<String>&& right) noexcept
    : _type{right._type},
      _data{right._type, right._data}
{
}

template <typename String>
inline basic_any<String>& basic_any<String>::
operator=(const basic_any<String>& right)
{
    this->~basic_any();
    _type = right._type;
    _data.assign(_type, right._data);
    return *this;
}

template <typename String>
inline basic_any<String>& basic_any<String>::
operator=(basic_any<String>&& right) noexcept
{
    this->~basic_any();
    _type = right._type;
    _data.assign(_type, std::move(right._data));
    return *this;
}

template <typename String>
basic_any<String>::~basic_any()
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
            break;
        case json_string:
            _data._string.~string_type();
            break;
        case json_array:
            _data._array.~array();
            break;
        case json_object:
            _data._object.~object();
            break;
    }
}

template <typename String>
inline element_type basic_any<String>::type() const
{
    return _type;
}

template <typename String>
inline bool basic_any<String>::is_null() const
{
    return _type == json_null;
}
template <typename String>
inline bool basic_any<String>::is_object() const
{
    return _type == json_object;
}

template <typename String>
inline bool basic_any<String>::is_array() const
{
    return _type == json_array;
}

template <typename String>
inline bool basic_any<String>::is_bool() const
{
    return _type == json_bool;
}

template <typename String>
inline bool basic_any<String>::is_double() const
{
    return _type == json_real;
}

template <typename String>
inline bool basic_any<String>::is_int() const
{
    return _type == json_integer;
}

template <typename String>
inline bool basic_any<String>::is_string() const
{
    return _type == json_string;
}

template <typename String>
inline bool basic_any<String>::is_number() const
{
    return is_int() || is_double();
}

template <typename String>
String basic_any<String>::string_value() const
{
    switch (_type)
    {
        case json_null:
        case json_object:
        case json_array:
            return string_type{};
        case json_bool:
            return to<string_type>(_data._bool);
        case json_real:
            return to<string_type>(_data._double);
        case json_integer:
            return to<string_type>(_data._int);
        case json_string:
            return _data._string;
    }
}

template <typename String>
long double basic_any<String>::double_value() const
{
    switch (_type)
    {
        case json_null:
            return 0.0;
        case json_string:
            return to<long double>(_data._string);
        case json_bool:
            return _data._bool;
        case json_real:
            return _data._double;
        case json_integer:
            return _data._int;
        case json_array:
        case json_object:
            _throw_invalid_type();
            return 0;
    }
}

template <typename String>
long long basic_any<String>::int_value() const
{
    switch (_type)
    {
        case json_null:
            return 0;
        case json_string:
            return to<long long>(_data._string);
        case json_bool:
            return _data._bool;
        case json_real:
            return to<long long>(_data._double);
        case json_integer:
            return _data._int;
        case json_array:
        case json_object:
            _throw_invalid_type();
            return 0;
    }
}

template <typename String>
bool basic_any<String>::bool_value() const
{
    switch (_type)
    {
        case json_null:
            return false;
        case json_bool:
            return _data._bool;
        case json_integer:
            return to<bool>(_data._int);
        case json_real:
            return to<bool>(_data._double);
        case json_string:
            return to<bool>(_data._string);
        case json_array:
        case json_object:
            _throw_invalid_type();
            return false;
    }
}

template <typename String>
bool basic_any<String>::empty() const
{
    switch (_type)
    {
        case json_null:
            return true;
        case json_bool:
        case json_integer:
        case json_real:
            return false;
        case json_string:
            return _data._string.empty();
        case json_array:
            return _data._array.empty();
        case json_object:
            return _data._object.empty();
    }
}

template <typename String>
std::size_t basic_any<String>::size() const
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
            _throw_invalid_type();
        case json_string:
            return _data._string.size();
        case json_array:
            return _data._array.size();
        case json_object:
            return _data._object.size();
    }
}

//
// array
//
template <typename String>
inline typename basic_any<String>::const_iterator
basic_any<String>::begin() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.begin();
}

template <typename String>
inline typename basic_any<String>::const_iterator basic_any<String>::end() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.end();
}

template <typename String>
basic_any<String>& basic_any<String>::front()
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.front();
}

template <typename String>
const basic_any<String>& basic_any<String>::front() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.front();
}

template <typename String>
basic_any<String>& basic_any<String>::back()
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.back();
}

template <typename String>
const basic_any<String>& basic_any<String>::back() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.back();
}

template <typename String>
inline basic_any<String>& basic_any<String>::operator[](std::size_t i)
{
    return _data._array[i];
}

template <typename String>
inline const basic_any<String>& basic_any<String>::
operator[](std::size_t i) const
{
    return _data._array[i];
}

template <typename String>
void basic_any<String>::push_back(const basic_any<String>& value)
{
    switch (_type)
    {
        case json_array:
            return _data._array.push_back(value);
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_object:
            _throw_invalid_type();
    }
}

template <typename String>
void basic_any<String>::emplace_back(basic_any<String>&& value)
{
    switch (_type)
    {
        case json_array:
            return _data._array.emplace_back(std::move(value));
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_object:
            _throw_invalid_type();
    }
}

template <typename String>
void basic_any<String>::pop_back()
{
    switch (_type)
    {
        case json_array:
            return _data._array.pop_back();
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_object:
            _throw_invalid_type();
    }
}

template <typename String>
void basic_any<String>::resize(std::size_t n,
                               const basic_any<String>& defaultValue)
{
    switch (_type)
    {
        case json_array:
            return _data._array.resize(n, defaultValue);
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_object:
            _throw_invalid_type();
    }
}

template <typename String>
typename basic_any<String>::const_iterator
basic_any<String>::erase(const_iterator it)
{
    switch (_type)
    {
        case json_array:
            return _data._array.erase(it);
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_object:
            _throw_invalid_type();
            return basic_any<String>::const_iterator{};
    }
}

template <typename String>
typename basic_any<String>::const_iterator
basic_any<String>::erase(const_iterator first, const_iterator last)
{
    switch (_type)
    {
        case json_array:
            return _data._array.erase(first, last);
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_object:
            _throw_invalid_type();
            return basic_any<String>::const_iterator{};
    }
}

//
// object access
//
template <typename String>
basic_any<String>& basic_any<String>::operator[](const String& key)
{
    return _data._object[key];
}

template <typename String>
const basic_any<String>& basic_any<String>::operator[](const String& key) const
{
    const auto it = _data._object.find(key);
    if (it == _data._object.end())
    {
        throw std::out_of_range(
            "basic_any<String>::operator[] key out of range");
    }
    return it->second;
}

template <typename String>
typename basic_any<String>::object::const_iterator
basic_any<String>::find(const String& key) const
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_array:
            _throw_invalid_type();
        case json_object:
            break;
    }

    auto it = _data._object.find(key);
    if (it == _data._object.end())
    {
        return typename object::const_iterator{};
    }
    return it;
}

template <typename String>
std::size_t basic_any<String>::erase(const String& key)
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_array:
            _throw_invalid_type();
        case json_object:
            return _data._object.erase(key);
    }
}

template <typename String>
typename basic_any<String>::object::const_iterator
basic_any<String>::erase(typename object::const_iterator it)
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_array:
            _throw_invalid_type();
        case json_object:
            return _data._object.erase(it);
    }
}

template <typename String>
basic_any<String>::object_range<typename basic_any<String>::object_key_iterator>
basic_any<String>::keys() const
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_array:
            _throw_invalid_type();
        case json_object:
            return object_range<object_key_iterator>{&_data._object};
    }
}

template <typename String>
basic_any<String>::object_range<
    typename basic_any<String>::object_value_iterator>
basic_any<String>::values() const
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_array:
            _throw_invalid_type();
        case json_object:
            return object_range<object_value_iterator>{&_data._object};
    }
}

template <typename String>
basic_any<String>::object_range<
    typename basic_any<String>::object_item_iterator>
basic_any<String>::items() const
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
        case json_string:
        case json_array:
            _throw_invalid_type();
        case json_object:
            return object_range<object_item_iterator>{&_data._object};
    }
}

//
// operators
//
template <typename String>
bool basic_any<String>::operator==(const basic_any<String>& right) const
{
    if (_type != right._type)
    {
        return false;
    }
    switch (_type)
    {
        case json_null:
            return true;
        case json_bool:
            return _data._bool == right._data._bool;
        case json_integer:
            return _data._int == right._data._int;
        case json_real:
            return _data._double == right._data._double;
        case json_string:
            return _data._string == right._data._string;
        case json_array:
            return _data._array == right._data._array;
        case json_object:
            return _data._object == right._data._object;
    }
}

template <typename String>
inline bool basic_any<String>::operator!=(const basic_any<String>& right) const
{
    return *this != right;
}

template <typename String>
bool basic_any<String>::operator<(const basic_any<String>& right) const
{
    if (_type != right._type)
    {
        return _type < right._type;
    }

    switch (_type)
    {
        case json_null:
            return false;
        case json_bool:
            return _data._bool < right._data._bool;
        case json_integer:
            return _data._int < right._data._int;
        case json_real:
            return _data._double < right._data._double;
        case json_string:
            return _data._string < right._data._string;
        case json_array:
            return _data._array < right._data._array;
        case json_object:
            return false;
    }
}

template <typename String>
inline bool basic_any<String>::operator>(const basic_any<String>& right) const
{
    return right < *this;
}

template <typename String>
inline bool basic_any<String>::operator<=(const basic_any<String>& right) const
{
    return !(right < *this);
}

template <typename String>
inline bool basic_any<String>::operator>=(const basic_any<String>& right) const
{
    return !(*this < right);
}

template <typename String>
void basic_any<String>::_throw_invalid_type() const
{
    throw std::logic_error("invalid type");
}

template <typename String>
template <typename Writer>
void basic_any<String>::_dump(Writer& writer, bool sort_keys) const
{
    switch (_type)
    {
        case json_null:
            writer.append(nullptr);
            break;
        case json_bool:
            writer.append(_data._bool);
            break;
        case json_integer:
            writer.append(_data._int);
            break;
        case json_real:
            writer.append(_data._double);
            break;
        case json_string:
            writer.append(_data._string);
            break;
        case json_array:
            writer.open_array();
            for (const auto& element : _data._array)
            {
                element._dump(writer, sort_keys);
            }
            writer.close_array();
            break;
        case json_object:
            writer.open_object();
            if (sort_keys)
            {
                std::vector<const typename object::value_type*> items;
                items.reserve(_data._object.size());
                for (const auto& element : _data._object)
                {
                    items.push_back(&element);
                }

                std::sort(items.begin(), items.end(),
                          [](const typename object::value_type* left,
                             const typename object::value_type* right)
                          {
                    return left->first < right->first;
                });
                for (const auto& element : items)
                {
                    writer.key(element->first);
                    element->second._dump(writer, sort_keys);
                }
            }
            else
            {
                for (const auto& element : _data._object)
                {
                    writer.key(element.first);
                    element.second._dump(writer, sort_keys);
                }
            }
            writer.close_object();
            break;
    }
}

template <typename String>
void basic_any<String>::dump(std::ostream& ostr, bool sort_keys,
                             std::size_t indent) const
{
    writer<std::ostream> writer{ostr, indent};
    _dump(writer, sort_keys);
}

template <typename String>
String basic_any<String>::dump(bool sort_keys, std::size_t indent) const
{
    using stream_type = typename ::native::detail::stream_type<String>::type;
    stream_type ostr;
    writer<stream_type> writer{ostr, indent};
    _dump(writer, sort_keys);
    return ostr.str();
}

template <typename String>
void basic_any<String>::dump(std::string& str, bool sort_keys,
                             std::size_t indent) const
{
    std::ostringstream ostr;
    writer<std::ostringstream> writer{ostr, indent};
    _dump(writer, sort_keys);
    str = ostr.str();
}

template <typename String>
class basic_any<String>::handler
{
public:
    using char_type = char;

    handler(basic_any<String>& self)
        : _root{self}
    {
    }

    template <typename T>
    void value(T&& value)
    {
        auto& target = _stack.empty() ? _root : _stack.back().second;

        switch (target.type())
        {
            case json_null:
            case json_bool:
            case json_integer:
            case json_real:
            case json_string:
                target = value;
                break;
            case json_array:
                target.push_back(value);
                break;
            case json_object:
                target[_key] = value;
                break;
        }
    }

    void value(const char_type* val, std::size_t length)
    {
        value(string_type{val, length});
    }

    data_type start_array()
    {
        _stack.emplace_back(string_type{_key, _key_length},
                            basic_any{json_array});
        return type_unknown;
    }

    void end_array()
    {
        auto val = std::move(_stack.back());
        _stack.pop_back();
        _key = val.first.data();
        _key_length = val.first.size();
        value(std::move(val.second));
    }

    void start_object()
    {
        _stack.emplace_back(string_type{_key, _key_length},
                            basic_any{json_object});
    }

    void end_object() { end_array(); }

    data_type key(const char_type* key, std::size_t length)
    {
        _key = key;
        _key_length = length;
        return type_unknown;
    }

private:
    basic_any<String>& _root;
    std::vector<std::pair<string_type, basic_any<String>>> _stack;
    const char_type* _key = nullptr;
    std::size_t _key_length = 0;
};

template <typename any_type, typename IStream>
any_type parse_stream(IStream& istr)
{
    any_type result;
    typename any_type::handler handler{result};
    parser{}.parse_stream(istr, handler);
    return result;
}

template <typename any_type>
any_type parse(const typename any_type::string_type& str)
{
    any_type result;
    typename any_type::handler handler{result};
    parser{}.parse(str, handler);
    return result;
}

template <typename any_type, typename Ch>
any_type parse(const Ch* str, std::size_t length)
{
    any_type result;
    typename any_type::handler handler{result};
    parser{}.parse(str, length, handler);
    return result;
}

template <typename any_type, typename Ch, std::size_t Size>
any_type parse(const Ch (&str)[Size])
{
    any_type result;
    typename any_type::handler handler{result};
    parser{}.parse(str, Size, handler);
    return result;
}

} // namespace json
} // namespace native
