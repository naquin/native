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

namespace native
{
namespace json
{

struct any::object_key_iterator
{
    using value_type = istring;

    object_key_iterator(any::object::const_iterator element)
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
    any::object::const_iterator _element;
};

struct any::object_value_iterator
{
    using value_type = any;

    object_value_iterator(any::object::const_iterator element)
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
    any::object::const_iterator _element;
};

struct any::object_item_iterator
{
    using value_type = any::object::value_type;

    object_item_iterator(any::object::const_iterator element)
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
    any::object::const_iterator _element;
};

template <typename Iterator>
struct any::object_range
{
    using const_iterator = Iterator;
    using value_type = typename Iterator::value_type;

    object_range(const any::object* object = nullptr)
        : _object(object)
    {
    }

    Iterator begin() const { return _object->begin(); }

    Iterator end() const { return _object->end(); }

private:
    const any::object* _object;
};

inline any::data::data(std::nullptr_t)
    : _null{nullptr}
{
}

inline any::data::data(bool value)
    : _bool{value}
{
}

inline any::data::data(long long value)
    : _int{value}
{
}

inline any::data::data(long double value)
    : _double{value}
{
}

inline any::data::data(const istring& value)
    : _string{value}
{
}

inline any::data::data(const array& value)
    : _array{value}
{
}

inline any::data::data(array&& value)
    : _array{}
{
    _array.reserve(value.size());
    for (auto& element : value)
    {
        _array.emplace_back(std::move(element));
    }
}

inline any::data::data(const object& value)
    : _object{value}
{
}

inline any::data::data(object&& value)
    : _object{std::move(value)}
{
}

inline any::data::data(element_type type, const any::data& right)
{
    assign(type, right);
}

inline any::data::data(element_type type, any::data&& right)
{
    assign(type, std::move(right));
}

inline void any::data::assign(element_type type, const any::data& right)
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
            new (&_string) istring{right._string};
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

inline void any::data::assign(element_type type, any::data&& right)
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
            new (&_string) istring{right._string};
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

any::data::~data() {}

inline any::any() noexcept : _type{json_null}, _data{nullptr} {}

inline any::any(element_type type)
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
            new (&_data._string) istring;
            break;
        case json_array:
            new (&_data._array) array;
            break;
        case json_object:
            new (&_data._object) object;
            break;
    }
}

inline any::any(std::nullptr_t) noexcept : _type{json_null}, _data{nullptr} {}

inline any::any(bool value)
    : _type(json_bool)
    , _data{value}
{
}

inline any::any(short value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(unsigned short value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(int value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(unsigned value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(unsigned long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(long long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(unsigned long long value)
    : _type(json_integer)
    , _data{static_cast<long long>(value)}
{
}

inline any::any(float value)
    : _type{json_real}
    , _data{static_cast<long double>(value)}
{
}

inline any::any(double value)
    : _type{json_real}
    , _data{static_cast<long double>(value)}
{
}

inline any::any(long double value)
    : _type{json_real}
    , _data{value}
{
}

inline any::any(const char* value)
    : any{istring{value}}
{
}

inline any::any(const std::string& value)
    : any{istring{value}}
{
}

inline any::any(const istring& value)
    : _type{json_string}
    , _data{value}
{
}

inline any::any(const array& value)
    : _type{json_array}
    , _data{value}
{
}

inline any::any(array&& value)
    : _type{json_array}
    , _data{std::move(value)}
{
}

inline any::any(const object& value)
    : _type{json_object}
    , _data{value}
{
}

inline any::any(object&& value)
    : _type{json_object}
    , _data{std::move(value)}
{
}

inline any::any(const any& right)
    : _type{right._type}
    , _data{right._type, right._data}
{
}

inline any::any(any&& right) noexcept : _type{right._type},
                                        _data{right._type, right._data}
{
}

inline any& any::operator=(const any& right)
{
    this->~any();
    _type = right._type;
    _data.assign(_type, right._data);
    return *this;
}

inline any& any::operator=(any&& right) noexcept
{
    this->~any();
    _type = right._type;
    _data.assign(_type, std::move(right._data));
    return *this;
}

inline any::~any()
{
    switch (_type)
    {
        case json_null:
        case json_bool:
        case json_integer:
        case json_real:
            break;
        case json_string:
            _data._string.~istring();
            break;
        case json_array:
            _data._array.~array();
            break;
        case json_object:
            _data._object.~object();
            break;
    }
}

inline element_type any::type() const { return _type; }

inline bool any::is_null() const { return _type == json_null; }
inline bool any::is_object() const { return _type == json_object; }
inline bool any::is_array() const { return _type == json_array; }
inline bool any::is_bool() const { return _type == json_bool; }
inline bool any::is_double() const { return _type == json_real; }
inline bool any::is_int() const { return _type == json_integer; }
inline bool any::is_string() const { return _type == json_string; }

inline bool any::is_number() const { return is_int() || is_double(); }

inline istring any::string_value() const
{
    switch (_type)
    {
        case json_null:
        case json_object:
        case json_array:
            return istring{};
        case json_bool:
            return to<istring>(_data._bool);
        case json_real:
            return to<istring>(_data._double);
        case json_integer:
            return to<istring>(_data._int);
        case json_string:
            return _data._string;
    }
}

inline long double any::double_value() const
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

inline long long any::int_value() const
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

inline bool any::bool_value() const
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

inline bool any::empty() const
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

std::size_t any::size() const
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
inline any::const_iterator any::begin() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.begin();
}

inline any::const_iterator any::end() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.end();
}

any& any::front()
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.front();
}

const any& any::front() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.front();
}

any& any::back()
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.back();
}

const any& any::back() const
{
    if (_type != json_array)
    {
        _throw_invalid_type();
    }
    return _data._array.back();
}

inline any& any::operator[](std::size_t i) { return _data._array[i]; }

inline const any& any::operator[](std::size_t i) const
{
    return _data._array[i];
}

inline void any::push_back(const any& value)
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

inline void any::emplace_back(any&& value)
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

void any::pop_back()
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

void any::resize(std::size_t n, const any& defaultValue)
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

any::const_iterator any::erase(const_iterator it)
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
            return any::const_iterator{};
    }
}

any::const_iterator any::erase(const_iterator first, const_iterator last)
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
            return any::const_iterator{};
    }
}

//
// object access
//
inline any& any::operator[](const istring& key) { return _data._object[key]; }

inline const any& any::operator[](const istring& key) const
{
    const auto it = _data._object.find(key);
    if (it == _data._object.end())
    {
        throw std::out_of_range("any::operator[] key out of range");
    }
    return it->second;
}

any::object::const_iterator any::find(const istring& key) const
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
        return object::const_iterator{};
    }
    return it;
}

std::size_t any::erase(const istring& key)
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

any::object::const_iterator any::erase(object::const_iterator it)
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

any::object_range<any::object_key_iterator> any::keys() const
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

any::object_range<any::object_value_iterator> any::values() const
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

any::object_range<any::object_item_iterator> any::items() const
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
inline bool any::operator==(const any& right) const
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

inline bool any::operator!=(const any& right) const { return *this != right; }

inline bool any::operator<(const any& right) const
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

inline bool any::operator>(const any& right) const { return right < *this; }

inline bool any::operator<=(const any& right) const { return !(right < *this); }

inline bool any::operator>=(const any& right) const { return !(*this < right); }

void any::_throw_invalid_type() const
{
    throw std::logic_error("invalid type");
}

template <typename Writer>
void any::_dump(Writer& writer, bool sort_keys) const
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
                std::vector<const object::value_type*> items;
                items.reserve(_data._object.size());
                for (const auto& element : _data._object)
                {
                    items.push_back(&element);
                }

                std::sort(items.begin(), items.end(),
                          [](const object::value_type* left,
                             const object::value_type* right)
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

inline void any::dump(std::ostream& ostr, bool sort_keys,
                      std::size_t indent) const
{
    writer<std::ostream> writer{ostr, indent};
    _dump(writer, sort_keys);
}

inline istring any::dump(bool sort_keys, std::size_t indent) const
{
    string_builder ostr;
    writer<string_builder> writer{ostr, indent};
    _dump(writer, sort_keys);
    return ostr.str();
}

inline void any::dump(std::string& str, bool sort_keys,
                      std::size_t indent) const
{
    std::ostringstream ostr;
    writer<std::ostringstream> writer{ostr, indent};
    _dump(writer, sort_keys);
    str = ostr.str();
}

class any::handler
{
public:
    typedef char char_type;

    handler(any& self)
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
        value(istring{val, length});
    }

    data_type start_array()
    {
        _stack.emplace_back(istring{_key, _key_length}, any{json_array});
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
        _stack.emplace_back(istring{_key, _key_length}, any{json_object});
    }

    void end_object() { end_array(); }

    data_type key(const char_type* key, std::size_t length)
    {
        _key = key;
        _key_length = length;
        return type_unknown;
    }

private:
    any& _root;
    std::vector<std::pair<istring, any>> _stack;
    const char_type* _key = nullptr;
    std::size_t _key_length = 0;
};

template <typename IStream>
any parse_stream(IStream& istr)
{
    any result;
    any::handler handler{result};
    parser{}.parse_stream(istr, handler);
    return result;
}

template <typename String>
typename std::enable_if<is_string_class<String>::value, any>::type
parse(const String& str)
{
    any result;
    any::handler handler{result};
    parser{}.parse(str, handler);
    return result;
}

template <typename Ch>
any parse(const Ch* str, std::size_t length)
{
    any result;
    any::handler handler{result};
    parser{}.parse(str, length, handler);
    return result;
}

template <typename Ch, std::size_t Size>
any parse(const Ch (&str)[Size])
{
    any result;
    any::handler handler{result};
    parser{}.parse(str, Size, handler);
    return result;
}

} // namespace json
} // namespace native
