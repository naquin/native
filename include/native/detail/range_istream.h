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

#ifndef NATIVE_RANGE_STREAM_H__
#define NATIVE_RANGE_STREAM_H__

#include "native/config.h"

namespace native
{
namespace detail
{

template <typename Iterator>
class range_istream
{
public:
    using iterator_type = Iterator;
    using char_type = typename std::remove_cv<
        typename std::remove_reference<decltype(*iterator_type())>::type>::type;

    range_istream(iterator_type first, iterator_type last)
        : _head{first}
        , _first{first}
        , _last{last}
    {
    }

    inline std::size_t tellg() const
    {
        return static_cast<std::size_t>(_first - _head);
    }

    inline bool good() const { return _first != _last; }

    inline bool fail() const { return _first == _last; }

    inline bool eof() const { return _first == _last; }

    inline bool bad() const { return _first == _last; }

    inline operator bool() const { return _first != _last; }

    inline bool operator!() const { return _first == _last; }

    inline char_type peek() const { return *_first; }

    inline void next() { ++_first; }

    inline char_type get()
    {
        const auto ch = peek();
        next();
        return ch;
    }

private:
    iterator_type _head;
    iterator_type _first;
    iterator_type _last;
};

} // namespace detail
} // namespace native

#endif
