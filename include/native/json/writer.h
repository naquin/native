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

#ifndef NATIVE_JSON_WRITER_H__
#define NATIVE_JSON_WRITER_H__

#include "native/config.h"

#include "native/istring.h"

#include <type_traits>

namespace native
{
namespace json
{

template <typename Stream>
class writer
{
public:
    writer(Stream& ostr, std::size_t indent = 0);

    void open_array();
    void close_array();

    template <typename T>
    void append(T&& value);

    void open_object();
    void close_object();

    template <typename T>
    void key(T&& key);

    template <typename T, typename U>
    void append(T&& key, U&& value);

private:
    struct state
    {
        enum t : unsigned char
        {
            array,
            object,
        } type;
        bool has_elements = false;

        state(t type)
            : type{type}
        {
        }
    };

    void _comma();
    void _indent();
    void _close();

    void _write(std::nullptr_t);
    void _write(bool value);
    void _write(const char* value);
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value ||
                                std::is_floating_point<T>::value,
                            void>::type
    _write(T value);
    template <typename T>
    typename std::enable_if<is_string_class<T>::value, void>::type
    _write(const T& value);

    Stream& _ostr;
    istring _indent_string;
    std::vector<state> _state;
};

template <typename Stream>
writer<Stream>::writer(Stream& ostr, std::size_t indent)
    : _ostr{ostr}
    , _indent_string(indent, ' ')
{
}

template <typename Stream>
void writer<Stream>::open_array()
{
    _ostr.put('[');
    _state.push_back(state::array);
}

template <typename Stream>
void writer<Stream>::close_array()
{
    _close();
}

template <typename Stream>
void writer<Stream>::open_object()
{
    if (!_state.empty())
    {
        if (_state.back().type == state::array && _state.back().has_elements)
        {
            _comma();
        }
        _indent();
    }
    _ostr.put('{');
    _state.push_back(state::object);
}

template <typename Stream>
void writer<Stream>::close_object()
{
    _close();
}

template <typename Stream>
template <typename T>
void writer<Stream>::key(T&& key)
{
    if (!_state.empty() && _state.back().has_elements)
    {
        _comma();
    }
    _indent();
    _write(key);
    _ostr.put(':');
    if (!_indent_string.empty())
    {
        _ostr.put(' ');
    }
}

template <typename Stream>
template <typename T, typename U>
void writer<Stream>::append(T&& label, U&& value)
{
    key(label);
    append(value);
}

template <typename Stream>
template <typename T>
void writer<Stream>::append(T&& value)
{
    if (!_state.empty())
    {
        if (_state.back().type == state::array)
        {
            if (_state.back().has_elements)
            {
                _comma();
            }
            _indent();
        }

        _state.back().has_elements = true;
    }
    _write(value);
}

template <typename Stream>
template <typename T>
typename std::enable_if<
    std::is_integral<T>::value || std::is_floating_point<T>::value, void>::type
writer<Stream>::_write(T value)
{
    _ostr << value;
}

template <typename Stream>
void writer<Stream>::_write(std::nullptr_t)
{
    _ostr.write("null", 4);
}

template <typename Stream>
void writer<Stream>::_write(bool value)
{
    if (value)
    {
        _ostr.write("true", 4);
    }
    else
    {
        _ostr.write("false", 5);
    }
}

template <typename Stream>
void writer<Stream>::_write(const char* value)
{
    _ostr.put('"');
    _ostr.write(value, std::strlen(value));
    _ostr.put('"');
}

template <typename Stream>
template <typename T>
typename std::enable_if<is_string_class<T>::value, void>::type
writer<Stream>::_write(const T& value)
{
    _ostr.put('"');
    _ostr.write(value.data(), value.size());
    _ostr.put('"');
}

template <typename Stream>
void writer<Stream>::_comma()
{
    _ostr.put(',');
}

template <typename Stream>
void writer<Stream>::_indent()
{
    if (_indent_string.empty())
    {
        return;
    }
    _ostr.put('\n');
    for (size_t i = 0; i < _state.size(); ++i)
    {
        _ostr.write(_indent_string.data(), _indent_string.size());
    }
}

template <typename Stream>
void writer<Stream>::_close()
{
    if (_state.empty())
    {
        return;
    }

    const auto current_state = _state.back();
    _state.pop_back();
    if (current_state.has_elements)
    {
        _indent();
    }
    switch (current_state.type)
    {
        case state::array:
            _ostr.put(']');
            break;
        case state::object:
            _ostr.put('}');
            break;
    }
    if (!_state.empty() && _state.back().type == state::object)
    {
        _state.back().has_elements = true;
    }
}

} // namespace json
} // namespace native

#endif
