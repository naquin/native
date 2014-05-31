//
// Copyright (c) 2014 Mike Naquin. All rights reserved.
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

#ifndef NATIVE_STRING_BUILDER_H__
#define NATIVE_STRING_BUILDER_H__

#include "native/config.h"

#include "native/string_base.h"

#include "native/detail/string_builder_core.h"
#include "native/detail/integers.h"
#include "native/detail/real.h"

#include "native/istring.h"

namespace native
{

template <typename Ch>
class basic_string_builder : public string_base
{
public:
    using this_type = basic_string_builder<Ch>;
    using traits_type = std::char_traits<Ch>;
    using value_type = typename traits_type::char_type;
    using allocator_type = std::allocator<Ch>;
    using size_type = typename allocator_type::size_type;
    using difference_type = typename allocator_type::difference_type;
    using reference = typename allocator_type::reference;
    using const_reference = typename allocator_type::const_reference;
    using pointer = typename allocator_type::pointer;
    using const_pointer = typename allocator_type::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using std_type = std::basic_string<value_type>;
    using common = detail::string_common<this_type>;
    using core_type = string_builder_core<Ch>;
    using slice_type = basic_string_slice<value_type>;
    using string_type = basic_string_builder<Ch>;
    using istring_type = basic_istring<Ch>;

    static const size_type npos = -1;

    // 1) Default constructor. Constructs empty string. The buffer capacity
    //    can be preallocated.
    constexpr basic_string_builder(size_type capacity = 1024) noexcept;

    // 2) Constructs the string with count copies of character ch.
    basic_string_builder(size_type n, value_type c);

    // 3) Constructs the string with a substring [pos, pos+count) of other. If
    //    the requested substring lasts past the end of the string, or if
    //    count == npos, the resulting substring is [pos, size()). If
    //    pos >= other.size(), std::out_of_range is thrown.
    basic_string_builder(const basic_string_builder& str, size_type pos,
                         size_type n = npos);
    basic_string_builder(const istring_type& str, size_type pos,
                         size_type n = npos);
    basic_string_builder(const std_type& str, size_type pos,
                         size_type n = npos);
    basic_string_builder(const slice_type& str, size_type pos,
                         size_type n = npos);

    // 4) Constructs the string with the first count characters of character
    //    string pointed to by s. s can contain null characters. s must not be
    //    a NULL pointer.
    constexpr basic_string_builder(const_pointer s, size_type n);

    // 5) Constructs the string with the contents of null-terminated character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character. s must not be a NULL pointer.
    constexpr basic_string_builder(const_pointer s);

    // 6) Constructs the string with the contents of the range [first, last).
    template <class InputIterator>
    basic_string_builder(InputIterator begin, InputIterator end);

    // 7) Copy constructor. Constructs the string with the copy of the contents
    //    of other.
    basic_string_builder(const basic_string_builder& str);
    basic_string_builder(const istring_type& str);
    basic_string_builder(const std_type& str);
    basic_string_builder(const slice_type& str);

    // 8) Move constructor. Constructs the string with the contents of other
    //    using move semantics.
    basic_string_builder(basic_string_builder&& str) noexcept;

    // 9) Constructs the string with the contents of the initializer list init.
    explicit basic_string_builder(std::initializer_list<value_type>);

    // Destructor
    ~basic_string_builder();

    // append
    basic_string_builder& operator<<(short value);
    basic_string_builder& operator<<(unsigned short value);
    basic_string_builder& operator<<(int value);
    basic_string_builder& operator<<(unsigned int value);
    basic_string_builder& operator<<(long value);
    basic_string_builder& operator<<(unsigned long value);
    basic_string_builder& operator<<(long long value);
    basic_string_builder& operator<<(unsigned long long value);
    basic_string_builder& operator<<(float value);
    basic_string_builder& operator<<(double value);
    basic_string_builder& operator<<(long double value);
    basic_string_builder& operator<<(bool value);
    basic_string_builder& operator<<(const_pointer value);

    basic_string_builder& put(value_type ch);
    basic_string_builder& write(const_pointer s, size_type length);

    size_type size() const noexcept;
    size_type length() const noexcept;
    size_type max_size() const noexcept;
    size_type capacity() const noexcept;

    void reserve(size_t n);
    void resize(size_t n);
    void clear() noexcept;

    bool empty() const noexcept;

    slice_type slice() const noexcept;

    slice_type operator()(size_type start) const;
    slice_type operator()(size_type start, size_type end) const;

    // Moves the contents into an istring_type.
    // The builder will be empty afterwards.
    istring_type str();

private:
    friend basic_istring<Ch>;

    // helper for constructing substrings
    template <typename String>
    size_type _substr_length(const String& str, size_type pos, size_type n);

    core_type _core;
};

using string_builder = basic_string_builder<char>;
using wstring_builder = basic_string_builder<wchar_t>;
using u16string_builder = basic_string_builder<char16_t>;
using u32string_builder = basic_string_builder<char32_t>;

//
// implementation
//

// 1) Default constructor. Constructs empty string.
template <typename Ch>
constexpr basic_string_builder<Ch>::basic_string_builder(
    size_type capacity) noexcept : _core(capacity)
{
}

// 2) Constructs the string with count copies of character ch.
template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(size_type n, value_type c)
    : _core(n, c)
{
}

// 3) Constructs the string with a substring [pos, pos+count) of other. If the
//    requested substring lasts past the end of the string, or if count == npos,
//    the resulting substring is [pos, size()). If pos >= other.size(),
//    std::out_of_range is thrown.
template <typename Ch>
template <typename String>
inline typename basic_string_builder<Ch>::size_type
basic_string_builder<Ch>::_substr_length(const String& str, size_type pos,
                                         size_type n)
{
    if (pos >= str.size())
    {
        this->throw_out_of_range();
    }

    return std::min(n, str.size());
}

template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const basic_string_builder& str,
                                               size_type pos, size_type n)
    : _core(str.data() + pos, _substr_length(str, pos, n))
{
}

template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const istring_type& str,
                                               size_type pos, size_type)
    : _core(str._core)
{
}

template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const std_type& str,
                                               size_type pos, size_type n)
    : _core(str.data() + pos, _substr_length(str, pos, n))
{
}

template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const slice_type& str,
                                               size_type pos, size_type n)
    : _core(str.data() + pos, _substr_length(str, pos, n))
{
}

// 4) Constructs the string with the first count characters of character string
//    pointed to by s. s can contain null characters. s must not be a NULL
//    pointer.
template <typename Ch>
constexpr basic_string_builder<Ch>::basic_string_builder(const_pointer s,
                                                         size_type n)
    : _core(s, n)
{
}

// 5) Constructs the string with the contents of null-terminated character
// string
//    pointed to by s. The length of the string is determined by the first null
//    character. s must not be a NULL pointer.
template <typename Ch>
constexpr basic_string_builder<Ch>::basic_string_builder(const_pointer first)
    : _core(first)
{
}

// 6) Constructs the string with the contents of the range [first, last).
template <typename Ch>
template <class InputIterator>
basic_string_builder<Ch>::basic_string_builder(InputIterator begin,
                                               InputIterator end)
    : _core(begin, end)
{
}

// 7) Copy constructor. Constructs the string with the copy of the contents of
// other.
template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const basic_string_builder& str)
    : _core(str._core)
{
}

template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const istring_type& str)
    : _core(str._core)
{
}

template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const std_type& str)
    : _core(str.data(), str.size())
{
}

template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(const slice_type& str)
    : _core(str.data(), str.size())
{
}

// 8) Move constructor. Constructs the string with the contents of other using
// move semantics.
template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(
    basic_string_builder&& str) noexcept : _core(std::move(str._core))
{
}

// 9) Constructs the string with the contents of the initializer list init.
template <typename Ch>
basic_string_builder<Ch>::basic_string_builder(
    std::initializer_list<value_type> init)
    : _core(init.begin(), init.end())
{
}

// Destructor
template <typename Ch>
basic_string_builder<Ch>::~basic_string_builder()
{
}

template <typename Ch>
inline typename basic_string_builder<Ch>::size_type
basic_string_builder<Ch>::size() const noexcept
{
    return this->_core.size();
}

template <typename Ch>
inline typename basic_string_builder<Ch>::size_type
basic_string_builder<Ch>::length() const noexcept
{
    return this->_core.size();
}

template <typename Ch>
inline typename basic_string_builder<Ch>::size_type
basic_string_builder<Ch>::max_size() const noexcept
{
    return this->_core.max_size();
}

template <typename Ch>
inline typename basic_string_builder<Ch>::size_type
basic_string_builder<Ch>::capacity() const noexcept
{
    return this->_core.capacity();
}

template <typename Ch>
inline void basic_string_builder<Ch>::reserve(size_t n)
{
    this->_core.reserve(n);
}

template <typename Ch>
inline void basic_string_builder<Ch>::resize(size_t n)
{
    this->_core.resize(n);
}

template <typename Ch>
inline void basic_string_builder<Ch>::clear() noexcept
{
    this->_core.clear();
}

template <typename Ch>
inline bool basic_string_builder<Ch>::empty() const noexcept
{
    return this->_core.size() == 0;
}

template <typename Ch>
typename basic_string_builder<Ch>::istring_type basic_string_builder<Ch>::str()
{
    _core.null_terminate();
    return istring_type(std::move(_core));
}

template <typename Ch>
typename basic_string_builder<Ch>::slice_type
basic_string_builder<Ch>::slice() const noexcept
{
    return slice_type(this->_core.data(),
                      this->_core.data() + this->_core.size());
}

//
// append
//

// char
template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(short value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(unsigned short value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(int value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(unsigned int value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(long value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(unsigned long value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(long long value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(unsigned long long value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(float value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(double value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <>
inline basic_string_builder<char>& basic_string_builder<char>::
operator<<(long double value)
{
    detail::stream_append(*this, value);
    return *this;
}

template <typename Ch>
basic_string_builder<Ch>& basic_string_builder<Ch>::
operator<<(const_pointer ptr)
{
    write(ptr, traits_type::length(ptr));
    return *this;
}

// wchar_t
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(short value)
//{
//    _append_string(std::swprintf, L"%hd", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(unsigned short value)
//{
//    _append_string(std::swprintf, L"%hu", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(int
// value)
//{
//    _append_string(std::swprintf, L"%d", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(unsigned int value)
//{
//    _append_string(std::swprintf, L"%u", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(long
// value)
//{
//    _append_string(std::swprintf, L"%ld", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder&
// basic_string_builder<wchar_t>::operator<<(unsigned long
// value)
//{
//    _append_string(std::swprintf, L"%lu", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(long
// long value)
//{
//    _append_string(std::swprintf, L"%lld", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(unsigned long long value)
//{
//    _append_string(std::swprintf, L"%llu", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(float value)
//{
//    _append_string(std::swprintf, L"%f", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(double value)
//{
//    _append_string(std::swprintf, L"%f", value);
//    return *this;
//}
//
// template <>
// inline basic_string_builder<wchar_t>&
// basic_string_builder<wchar_t>::operator<<(long
// double value)
//{
//    _append_string(std::swprintf, L"%Lf", value);
//    return *this;
//}

template <typename Ch>
basic_string_builder<Ch>& basic_string_builder<Ch>::operator<<(bool value)
{
    return put(value ? '1' : '0');
}

template <typename Ch>
basic_string_builder<Ch>& basic_string_builder<Ch>::put(value_type ch)
{
    _core.push_back(ch);
    return *this;
}

template <typename Ch>
basic_string_builder<Ch>& basic_string_builder<Ch>::write(const_pointer s,
                                                          size_type length)
{
    const auto size = _core.size();
    _core.resize(size + length);
    std::copy(s, s + length, _core.mutable_data() + size);
    return *this;
}

//
// streams
//
template <typename Ch>
basic_string_builder<Ch>& operator<<(basic_string_builder<Ch>& s, Ch ch)
{
    s.put(ch);
    return s;
}

template <typename Ch>
basic_string_builder<Ch>&
operator<<(basic_string_builder<Ch>& s,
           const typename basic_string_builder<Ch>::string_type& str)
{
    s.write(str.data(), str.length());
    return s;
}

template <typename Ch>
basic_string_builder<Ch>&
operator<<(basic_string_builder<Ch>& s,
           const typename basic_string_builder<Ch>::istring_type& str)
{
    s.write(str.data(), str.length());
    return s;
}

template <typename Ch>
basic_string_builder<Ch>&
operator<<(basic_string_builder<Ch>& s,
           const typename basic_string_builder<Ch>::std_type& str)
{
    s.write(str.data(), str.length());
    return s;
}

template <typename Ch>
basic_string_builder<Ch>&
operator<<(basic_string_builder<Ch>& s,
           const typename basic_string_builder<Ch>::slice_type& str)
{
    s.write(str.data(), str.length());
    return s;
}

} // namespace native

namespace std
{

template <typename Ch>
struct hash<native::basic_string_builder<Ch>>
{
    std::size_t operator()(const native::basic_string_builder<Ch>& s) const
        noexcept
    {
        return hash(s.data(), s.length() * sizeof(Ch));
    }
};

} // namespace

#endif
