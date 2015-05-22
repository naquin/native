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

#ifndef NATIVE_JSON_INPUT_STREAMS_H__
#define NATIVE_JSON_INPUT_STREAMS_H__

#include "native/config.h"

#include <type_traits>

namespace native
{
namespace json
{

// input_stream interface
template <typename Ch>
class input_stream
{
public:
    using char_type = Ch;

    // Return the current line number.
    inline std::size_t line() const;

    // Return the current column number.
    inline std::size_t column() const;

    // Return the current position.
    inline std::size_t position() const;

    // Return true if this is the end of the stream.
    inline bool eof() const;

    // Return the current character.
    inline char_type get() const;

    // Move ahead one position in the stream.
    inline void next();

    // Return the current character, then increment ahead one position.
    inline char_type get();

    // Increment the line number when a new line is found.
    inline void increment_line();
};

template <typename Iterator>
class iterator_stream
{
public:
    using iterator_type = Iterator;
    using char_type = typename std::remove_cv<
        typename std::remove_reference<decltype(*iterator_type())>::type>::type;

    iterator_stream(iterator_type first, iterator_type last)
        : _head(first)
        , _first(first)
        , _last(last)
        , _line(1)
        , _col_start(first)
    {
    }

    inline std::size_t line() const { return _line; }

    inline std::size_t column() const
    {
        return static_cast<std::size_t>(_first - _col_start);
    }

    inline std::size_t position() const
    {
        return static_cast<std::size_t>(_first - _head);
    }

    inline bool eof() const { return _first == _last; }

    inline char_type peek() const { return *_first; }

    inline void next() { ++_first; }

    inline char_type get()
    {
        const auto ch = peek();
        next();
        return ch;
    }

    inline void increment_line()
    {
        ++_line;
        _col_start = _first;
    }

private:
    iterator_type _head;
    iterator_type _first;
    iterator_type _last;
    std::size_t _line;
    iterator_type _col_start;
};

template <typename IStream>
class istream_stream
{
public:
    using istream_type = IStream;
    using char_type = typename istream_type::char_type;
    using pos_type = typename istream_type::pos_type;

    istream_stream(istream_type& istr)
        : _istr(istr)
        , _line()
        , _current(istr.get())
        , _col_start()
    {
    }

    inline std::size_t line() const { return _line; }

    inline std::size_t column() const { return _istr.tellg() - _col_start; }

    inline bool eof() const { return _istr.eof(); }

    inline char_type peek() const { return _current; }

    inline void next() { _current = _istr.get(); }

    inline char_type get()
    {
        const auto ch = peek();
        next();
        return ch;
    }

    inline void increment_line()
    {
        ++_line;
        _col_start = _istr.tellg();
    }

private:
    istream_type& _istr;
    char_type _current;
    std::size_t _line;
    pos_type _col_start;
};

template <typename Iterator>
iterator_stream<Iterator> make_parser_range_iterator(Iterator first,
                                                     Iterator last)
{
    return std::move(iterator_stream<Iterator>(first, last));
}

template <typename IStream>
istream_stream<IStream> make_parser_range_string(IStream& istr)
{
    return std::move(istream_stream<IStream>(istr));
}
}
} // namespace native::json

#endif
