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

#ifndef NATIVE_STRING_SLICE_H__
#define NATIVE_STRING_SLICE_H__

#include "native/string_base.h"
#include "native/string_core.h"

namespace native {

// Splice strings are a way to efficiently refer to a sub-string without having
// to make a copy of the super-string.
//
// istring super_string = "abcdefghijklmnopqrstuvwxyz";
// string_slice sub_string = super_string(13, 16); // just points to "mno"
//
//
// "abcdefghijklmnopqrstuvwxyz" - super-string
//              ^
//             "xyz"            sub-string
//
// As a consequence of this, slices *cannot be null terminated*. A null terminated
// copy can instead be obtained with one of the string conversion methods
// str() or std_str().
template <typename Ch>
class basic_string_slice:
    public string_base {
public:
    using this_type              = basic_string_slice<Ch>;
    using traits_type            = std::char_traits<Ch>;
    using value_type             = typename traits_type::char_type;
    using allocator_type         = std::allocator<Ch>;
    using size_type              = typename allocator_type::size_type;
    using difference_type        = typename allocator_type::difference_type;
    using reference              = typename allocator_type::reference;
    using const_reference        = typename allocator_type::const_reference;
    using pointer                = typename allocator_type::pointer;
    using const_pointer          = typename allocator_type::const_pointer;
    using const_iterator         = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using std_type    = std::basic_string<value_type>;
    using common      = detail::string_common<this_type>;
    using core_type   = basic_string_core<Ch>;
    using slice_type  = basic_string_slice<value_type>;
    using string_type = basic_istring<Ch>;

    static const size_type npos;
    
    // 1) Default constructor. Constructs empty string.
    basic_string_slice() noexcept;

    // 3) Constructs the string with a substring [pos, pos+count) of other. If
    //    the requested substring lasts past the end of the string, or if
    //    count == npos, the resulting substring is [pos, size()). If
    //    pos >= other.size(), std::out_of_range is thrown.
    basic_string_slice(const string_type& str, size_type pos, size_type n = npos);
    basic_string_slice(const std_type& str,    size_type pos, size_type n = npos);
    basic_string_slice(const basic_string_slice& str, size_type pos, size_type n = npos);

    // 4) Constructs the string with the first count characters of character
    //    string pointed to by s. s can contain null characters. s must not be
    //    a NULL pointer.
    basic_string_slice(const_pointer s, size_type n);

    // 5) Constructs the string with the contents of null-terminated character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character. s must not be a NULL pointer.
    basic_string_slice(const_pointer s);

    // 6) Constructs the string with the contents of the range [first, last).
    basic_string_slice(const_pointer first, const_pointer last);

    // 7) Copy constructor. Constructs the string with the copy of the contents
    //    of other.
    basic_string_slice(const basic_string_slice& str);
    basic_string_slice(const string_type& str);
    basic_string_slice(const std_type& str);

    // 8) Move constructor. Constructs the string with the contents of other
    //    using move semantics.
    basic_string_slice(basic_string_slice&& str) noexcept;


    // 1) Replaces the contents with a copy of str
    basic_string_slice& operator=(const basic_string_slice& str);
    basic_string_slice& operator=(const std_type& str);
    basic_string_slice& operator=(const string_type& str);

    // 2) Replaces the contents with those of str using move semantics. str is
    //    in undefined state after the operation.
    basic_string_slice& operator=(basic_string_slice&& str);
    
    // 3) Replaces the contents with those of null-terminated character string
    //    pointed to by s.
    basic_string_slice& operator=(const value_type* s);

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;

    const_iterator         cbegin() const noexcept;
    const_iterator         cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    size_type size() const noexcept;
    size_type length() const noexcept;
    size_type max_size() const noexcept;
    size_type capacity() const noexcept;

    bool empty() const noexcept;

    const_reference operator[](size_type pos) const;

    const_reference at(size_type n) const;

    const_reference front() const;
    const_reference back() const;

    size_type copy(pointer s, size_type n, size_type pos = 0) const;
    template <std::size_t Size>
    size_type copy(value_type (&s)[Size], size_type pos = 0) const;
    this_type substr(size_type pos = 0, size_type n = npos) const;

    string_type str() const { return std::move(string_type(data(), size())); }
    std_type std_str() const;
    // No c_str() on purpose. Splices are NOT NULL TERMINATED!!!
    const_pointer data() const noexcept;

    // 1) Split a string by ch.
    std::vector<basic_string_slice> split(value_type ch) const;

    // 2) Split a string by s.
    std::vector<basic_string_slice> split(const_pointer s) const;

    // 3) Split a string by str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        std::vector<basic_string_slice>
    >::type split(const String& str) const;

    // 1) Finds the first substring equal to str. If str.size() is zero, returns pos.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find(const String& str, size_type pos = 0) const noexcept;

    // 2) Finds the first substring equal to the first count characters of the
    //    character string pointed to by s. s can include null characters. If
    //    count is zero, returns pos.
    size_type find(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the first substring equal to the character string pointed to by
    //    s. The length of the string is determined by the first null character.
    //    If the length of s is zero, returns pos.
    size_type find(const_pointer s, size_type pos = 0) const noexcept;

    // 4) Finds the first character ch.
    size_type find(value_type c, size_type pos = 0) const noexcept;
    
    // 1) Finds the last substring equal to str. If str.size() is zero, returns
    //    pos or size()-1, whichever is smaller.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type rfind(const String& str, size_type pos = npos) const noexcept;

    // 2) Finds the last substring equal to the first count characters of the
    //    character string pointed to by s. s can include null characters. If
    //    count is zero, returns pos or size()-1, whichever is smaller.
    size_type rfind(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the last substring equal to the character string pointed to by
    //    s. The length of the string is determined by the first null character.
    //    If the length of s is zero, returns pos or size()-1, whichever is
    //    smaller.
    size_type rfind(const_pointer s, size_type pos = npos) const noexcept;

    // 4) Finds the last character ch.
    size_type rfind(value_type c, size_type pos = npos) const noexcept;

    // 1) Finds the first character equal to one of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_first_of(const String& str, size_type pos = 0) const noexcept;

    // 2) Finds the first character equal to one of characters in the first
    //    count characters of the character string pointed to by s. s can
    //    include null characters.
    size_type find_first_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the first character equal to one of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_first_of(const_pointer s, size_type pos = 0) const noexcept;

    // 4) Finds the first character equal to ch.
    size_type find_first_of(value_type c, size_type pos = 0) const noexcept;
    
    // 1) Finds the last character equal to one of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_last_of(const String& str, size_type pos = npos) const noexcept;

    // 2) Finds the last character equal to one of characters in the first count
    //    characters of the character string pointed to by s. s can include null
    //    characters.
    size_type find_last_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the last character equal to one of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_last_of(const_pointer s, size_type pos = npos) const noexcept;

    // 4) Finds the last character equal to ch.
    size_type find_last_of(value_type c, size_type pos = npos) const noexcept;

    // 1) Finds the first character equal to none of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_first_not_of(const String& str, size_type pos = 0) const noexcept;

    // 2) Finds the first character equal to none of characters in the first
    //    count characters of the character string pointed to by s. s can
    //    include null characters.
    size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the first character equal to none of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_first_not_of(const_pointer s, size_type pos = 0) const noexcept;

    // 4) Finds the first character not equal to ch.
    size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;
    
    // 1) Finds the last character equal to none of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_last_not_of(const String& str, size_type pos = npos) const noexcept;

    // 2) Finds the last character equal to none of characters in the first
    //    count characters of the character string pointed to by s. s can
    //     include null characters.
    size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the last character equal to none of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_last_not_of(const_pointer s, size_type pos = npos) const noexcept;

    // 4) Finds the last character not equal to ch.
    size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept;


    // 1) Compares this string to str. First, calculates the number of
    //    characters to compare, as if by size_type rlen = std::min(size(),
    //    str.size()). Then compares by calling Traits::compare(data(),
    //    str.data(), rlen). For standard strings this function performs
    //    character-by-character lexicographical comparison.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        int
    >::type compare(const String& str) const noexcept;

    // 2) Compares a [pos1, pos1+count1) substring of this string to str as if
    //    by this_type(*this, pos1, count1).compare(str)
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        int
    >::type compare(size_type pos1, size_type n1, const String& str) const;

    // 3) Compares a [pos1, pos1+count1) substring of this string to a substring
    //    [pos2, pas2+count2) of str as if by
    //    this_type(*this, pos1, count1).compare(this_type(str, pos2, count2))
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        int
    >::type compare(size_type pos1, size_type n1, const String& str,
                size_type pos2, size_type n2) const;

    // 4) Compares this string to the null-terminated character sequence
    //    beginning at the character pointed to by s, as if by
    //    compare(this_type(s))
    int compare(const_pointer s) const noexcept;

    // 5) Compares a [pos1, pos1+count1) substring of this string to the
    //    null-terminated character sequence beginning at the character pointed
    //    to by s, as if by
    //    this_type(*this, pos, count1).compare(this_type(s))
    int compare(size_type pos1, size_type n1, const_pointer s) const;

    // 6) Compares a [pos1, pos1+count1) substring of this string to the first
    //    count2 characters of the character array whose first character is
    //    pointed to by s, as if by
    //    this_type(*this, pos, count1).compare(this_type(s, count2)).
    //    (Note: the characters from s to s+count2 may include null characters))
    int compare(size_type pos1, size_type n1, const_pointer s, size_type n2) const;


private:

    const_pointer _head;
    size_type _length;
};

using string_slice    = basic_string_slice<char>;
using wstring_slice   = basic_string_slice<wchar_t>;
using u16string_slice = basic_string_slice<char16_t>;
using u32string_slice = basic_string_slice<char32_t>;

//
// Implementations
//

template <typename Ch>
constexpr const typename basic_string_slice<Ch>::size_type basic_string_slice<Ch>::npos = -1;

// 1) Default constructor. Constructs empty string.
template <typename Ch>
basic_string_slice<Ch>::basic_string_slice() noexcept:
    _head(),
    _length()
{

}

// 3) Constructs the string with a substring [pos, pos+count) of other. If the
//    requested substring lasts past the end of the string, or if count == npos,
//    the resulting substring is [pos, size()). If pos >= other.size(),
//    std::out_of_range is thrown.
template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(
    const basic_string_slice& str, size_type pos, size_type n)
{
    if (pos >= str.size())
    {
        this->throw_out_of_range();
    }
    else
    {
        n = std::min(n, str.size() - pos);
        _head = str.data() + pos;
        _length = n;
    }
}

template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(
    const std_type& str, size_type pos, size_type n)
{
    if (pos >= str.size())
    {
        this->throw_out_of_range();
    }
    else
    {
        n = std::min(n, str.size() - pos);
        _head = str.data() + pos;
        _length = n;
    }
}

template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(
    const string_type& str, size_type pos, size_type n)
{
    if (pos >= str.size())
    {
        this->throw_out_of_range();
    }
    else
    {
        n = std::min(n, str.size() - pos);
        _head = str.data() + pos;
        _length = n;
    }
}

// 4) Constructs the string with the first count characters of character
//    string pointed to by s. s can contain null characters. s must not be
//    a NULL pointer.
template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(const_pointer s, size_type n):
    _head(s),
    _length(n)
{

}


// 5) Constructs the string with the contents of null-terminated character
//    string pointed to by s. The length of the string is determined by the
//    first null character. s must not be a NULL pointer.
template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(const_pointer s):
    _head(s),
    _length(traits_type::length(s))
{

}

// 6) Constructs the string with the contents of the range [first, last).
template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(
    const_pointer first, const_pointer last):
    _head(first),
    _length(last - first)
{

}

// 7) Copy constructor. Constructs the string with the copy of the contents
//    of other.
template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(const basic_string_slice& str):
    _head(str.data()),
    _length(str.size())
{

}

template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(const string_type& str):
    _head(str.data()),
    _length(str.size())
{

}

template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(const std_type& str):
    _head(str.data()),
    _length(str.size())
{

}

// 8) Move constructor. Constructs the string with the contents of other
//    using move semantics.
template <typename Ch>
basic_string_slice<Ch>::basic_string_slice(
    basic_string_slice&& str) noexcept:
    _head(std::move(str._head)),
    _length(std::move(str._length))
{

}

// 1) Replaces the contents with a copy of str
template <typename Ch>
basic_string_slice<Ch>& basic_string_slice<Ch>::operator=(const basic_string_slice& str)
{
    this->_head   = str._head;
    this->_length = str._length;
    return *this;
}

template <typename Ch>
basic_string_slice<Ch>& basic_string_slice<Ch>::operator=(const std_type& str)
{
    this->_head   = str.data();
    this->_length = str.size();
    return *this;
}

template <typename Ch>
basic_string_slice<Ch>& basic_string_slice<Ch>::operator=(const string_type& str)
{
    this->_head   = str.data();
    this->_length = str.size();
    return *this;
}


// 2) Replaces the contents with those of str using move semantics. str is
//    in undefined state after the operation.
template <typename Ch>
basic_string_slice<Ch>& basic_string_slice<Ch>::operator=(basic_string_slice&& str)
{
    this->_head   = std::move(str._head);
    this->_length = std::move(str._length);
    return *this;
}

// 3) Replaces the contents with those of null-terminated character string pointed to by s.
template <typename Ch>
basic_string_slice<Ch>& basic_string_slice<Ch>::operator=(const value_type* s)
{
    this->_head   = s;
    this->_length = traits_type::length(s);
    return *this;
}

// 1) Split a string by ch.
template <typename Ch>
std::vector<basic_string_slice<Ch>>
    basic_string_slice<Ch>::split(value_type ch) const
{
    std::vector<slice_type> result;
    size_type start = 0, end = find(ch);
    for(;;)
    {
        if (end == npos)
        {
            result.push_back(slice_type(*this, start, size() - start));
            break;
        }

        result.push_back(slice_type(*this, start, end - start));
        start = end + 1;
        end = find(ch, start);
    }

    return std::move(result);
}

// 2) Split a string by s.
template <typename Ch>
std::vector<basic_string_slice<Ch>>
    basic_string_slice<Ch>::split(const_pointer s) const
{
    const size_type n = traits_type::length(s);
    std::vector<slice_type> result;
    if (n == 0)
    {
        result.push_back(slice_type(*this));
    }
    else
    {
        size_type start = 0, end = find(s, 0, n);
        for(;;)
        {
            if (end == npos)
            {
                result.push_back(slice_type(*this, start, size() - start));
                break;
            }

            result.push_back(slice_type(*this, start, end - start));
            start = end + n;
            end = find(s, start, n);
        }
    }

    return std::move(result);
}

// 3) Split a string by str.
template <typename Ch>
template <typename String>
typename std::enable_if<
    is_string_class<String>::value,
    std::vector<basic_string_slice<Ch>>
>::type basic_string_slice<Ch>::split(const String& str) const
{
    std::vector<slice_type> result;
    if (str.empty())
    {
        result.push_back(slice_type(str));
    }
    else
    {
        size_type start = 0, end = find(str);
        for(;;)
        {
            if (end == npos)
            {
                result.push_back(slice_type(*this, start, size() - start));
                break;
            }

            result.push_back(slice_type(*this, start, end - start));
            start = end + str.size();
            end = find(str, start);
        }
    }

    return std::move(result);
}


template <typename Ch>
inline typename basic_string_slice<Ch>::const_iterator
    basic_string_slice<Ch>::begin() const noexcept
{
    return &data()[0];
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_iterator
    basic_string_slice<Ch>::end() const noexcept
{
    return &data()[0] + size();
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_reverse_iterator
    basic_string_slice<Ch>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_reverse_iterator
    basic_string_slice<Ch>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_iterator
    basic_string_slice<Ch>::cbegin() const noexcept
{
    return begin();
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_iterator
    basic_string_slice<Ch>::cend() const noexcept
{
    return end();
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_reverse_iterator
    basic_string_slice<Ch>::crbegin() const noexcept
{
    return rbegin();
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_reverse_iterator
    basic_string_slice<Ch>::crend() const noexcept
{
    return rend();
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::size() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::length() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::max_size() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::capacity() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline bool basic_string_slice<Ch>::empty() const noexcept
{
    return this->_length == 0;
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_reference
    basic_string_slice<Ch>::operator[](size_type pos) const
{
    return data()[pos];
}

template <typename Ch>
typename basic_string_slice<Ch>::const_reference
    basic_string_slice<Ch>::at(size_type n) const
{
    if (n >= this->_length)
    {
        this->throw_out_of_range();
    }
    return data()[n];
}



template <typename Ch>
typename basic_string_slice<Ch>::const_reference
    basic_string_slice<Ch>::front() const
{
    return data()[0];
}

template <typename Ch>
typename basic_string_slice<Ch>::const_reference
    basic_string_slice<Ch>::back() const
{
    return data()[size()-1];
}

template <typename Ch>
typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::copy(pointer s, size_type n, size_type pos) const
{
    if (pos >= size())
    {
        this->throw_out_of_range();
    }
    n = std::min(n, size()-pos);
    for (size_type i = 0; i < n; ++i)
    {
        s[i] = data()[pos++];
    }

    return n;
}

template <typename Ch>
template <std::size_t Size>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::copy(value_type (&s)[Size], size_type pos) const
{
    return copy(s, Size, pos);
}

template <typename Ch>
basic_string_slice<Ch>
    basic_string_slice<Ch>::substr(size_type pos, size_type n) const
{
    return basic_string_slice(*this, pos, n);
}

template <typename Ch>
inline typename basic_string_slice<Ch>::std_type
    basic_string_slice<Ch>::std_str() const
{
    return std::move(std_type(data(), size()));
}

template <typename Ch>
inline typename basic_string_slice<Ch>::const_pointer
    basic_string_slice<Ch>::data() const noexcept
{
    return this->_head;
}



//
// find
//
// 1) Finds the first substring equal to str. If str.size() is zero, returns pos.
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_slice<Ch>::size_type
>::type
    basic_string_slice<Ch>::find(const String& str, size_type pos) const noexcept
{
    return common::find(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find(const_pointer s, size_type pos) const noexcept
{
    return common::find(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find(value_type c, size_type pos) const noexcept
{
    return common::find(*this, c, pos);
}

//
// rfind
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_slice<Ch>::size_type
>::type
    basic_string_slice<Ch>::rfind(const String& str, size_type pos) const noexcept
{
    return common::rfind(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::rfind(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::rfind(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::rfind(const_pointer s, size_type pos) const noexcept
{
    return common::rfind(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::rfind(value_type c, size_type pos) const noexcept
{
    return common::rfind(*this, c, pos);
}

//
// find_first_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_slice<Ch>::size_type
>::type
    basic_string_slice<Ch>::find_first_of(const String& str, size_type pos) const noexcept
{
    return common::find_first_of(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_first_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_first_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_first_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_first_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_first_of(value_type c, size_type pos) const noexcept
{
    return common::find(*this, c, pos);
}

//
// find_last_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_slice<Ch>::size_type
>::type
    basic_string_slice<Ch>::find_last_of(const String& str, size_type pos) const noexcept
{
    return find_last_of(str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_last_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_last_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_last_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_last_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_last_of(value_type c, size_type pos) const noexcept
{
    return common::rfind(*this, c, pos);
}

//
// find_first_not_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_slice<Ch>::size_type
>::type
    basic_string_slice<Ch>::find_first_not_of(const String& str, size_type pos) const noexcept
{
    return find_first_not_of(str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type basic_string_slice<Ch>::find_first_not_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_first_not_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_first_not_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_first_not_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_first_not_of(value_type c, size_type pos) const noexcept
{
    return common::find_first_not_of(*this, c, pos);
}

//
// find_last_not_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_slice<Ch>::size_type
>::type
    basic_string_slice<Ch>::find_last_not_of(const String& str, size_type pos) const noexcept
{
    return common::find_last_not_of(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_last_not_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_last_not_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_slice<Ch>::size_type
    basic_string_slice<Ch>::find_last_not_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_last_not_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
typename basic_string_slice<Ch>::size_type basic_string_slice<Ch>::find_last_not_of(value_type c, size_type pos) const noexcept
{
    return common::find_last_not_of(*this, c, pos);
}

//
// Compare
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    int
>::type
    basic_string_slice<Ch>::compare(const String& str) const noexcept
{
    return common::compare(*this, str);
}

template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    int
>::type
    basic_string_slice<Ch>::compare(
        size_type pos1, size_type n1, const String& str) const
{
    return common::compare(*this, pos1, n1, str);
}

template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    int
>::type
    basic_string_slice<Ch>::compare(
        size_type pos1, size_type n1, const String& str,
                size_type pos2, size_type n2) const
{
    return common::compare(*this, pos1, n1, str, pos2, n2);
}

template <typename Ch>
inline int basic_string_slice<Ch>::compare(const_pointer s) const noexcept
{
    return common::compare(*this, s);
}

template <typename Ch>
inline int basic_string_slice<Ch>::compare(
    size_type pos1, size_type n1, const_pointer s) const
{
    return common::compare(*this, pos1, n1, s);
}

template <typename Ch>
inline int basic_string_slice<Ch>::compare(
    size_type pos1, size_type n1, const_pointer s, size_type n2) const
{
    return common::compare(*this, pos1, n1, s, n2);
}


//
// streams
//
template <typename Ch>
inline std::basic_ostream<Ch>& operator<< (std::basic_ostream<Ch>& ostr, const basic_string_slice<Ch>& s)
{
    ostr.write(s.data(), s.size());
    return ostr;
}

template <typename Ch>
inline std::basic_istream<Ch>& operator<< (std::basic_istream<Ch>& istr, const basic_string_slice<Ch>& s)
{
    std::string tmp;
    istr >> tmp;
    s = istr;
    return istr;
}


} // namespace native

#endif
