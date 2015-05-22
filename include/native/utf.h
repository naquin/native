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

#ifndef NATIVE_UTF_H__
#define NATIVE_UTF_H__

#include "native/config.h"

namespace native
{

// UTF-8 encoding
// http://en.wikipedia.org/wiki/UTF-8
// http://tools.ietf.org/html/rfc3629
//
// Bits
//    First     Last       Byte 1   Byte 2   Byte 3   Byte 4   Byte 5   Byte 6
// 7  U+0000    U+007F     0xxxxxxx
// 11 U+0080    U+07FF     110xxxxx 10xxxxxx
// 16 U+0800    U+FFFF     1110xxxx 10xxxxxx 10xxxxxx
// 21 U+10000   U+1FFFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
// 26 U+200000  U+3FFFFFF  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// 31 U+4000000 U+7FFFFFFF 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
struct utf8
{
    using char_type = char;

    template <typename OStream>
    static void encode(OStream& ostr, char32_t codepoint)
    {
        static_assert(sizeof(typename OStream::char_type) == 1, "");

        if (codepoint <= 0x7f)
        {
            ostr.put(codepoint & 0xff);
        }
        else if (codepoint <= 0x7ff)
        {
            ostr.put(0xc0 | ((codepoint >> 6) & 0xff));
            ostr.put(0x80 | ((codepoint & 0x3f)));
        }
        else if (codepoint <= 0xffff)
        {
            ostr.put(0xe0 | ((codepoint >> 12) & 0xff));
            ostr.put(0x80 | ((codepoint >> 6) & 0x3f));
            ostr.put(0x80 | (codepoint & 0x3f));
        }
        else if (codepoint <= 0x10ffff)
        {
            ostr.put(0xf0 | ((codepoint >> 18) & 0xff));
            ostr.put(0x80 | ((codepoint >> 12) & 0x3f));
            ostr.put(0x80 | ((codepoint >> 6) & 0x3f));
            ostr.put(0x80 | (codepoint & 0x3f));
        }
        else
        {
            throw std::runtime_error("invalid utf-8 codepoint");
        }
    }

    template <typename IStream>
    static char32_t decode(IStream& istr)
    {
        static_assert(sizeof(typename IStream::char_type) == 1, "");

        static const std::uint32_t byte_1_map[] = {
            //      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b, c, d, e, f
            /* 0 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 1 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 2 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 3 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 4 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 6 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            /* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            /* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            /* a */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            /* b */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            /* c */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            /* d */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            /* e */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            /* f */ 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6,
        };

        static const std::uint8_t byte_1_mask[] = {
            0,    // invalid
            0xff, // 1
            0x3f, // 2
            0x1f, // 3
            0x0f, // 4
            0x07, // 5
            0x03, // 6
        };

        const auto first_byte = static_cast<std::uint8_t>(istr.get());
        const auto num_bytes = byte_1_map[first_byte];

        const auto read_byte = [](IStream& istr)
        {
            const auto ch = static_cast<std::uint8_t>(istr.get());
            if ((ch & 0xc0) != 0x80)
            {
                throw std::runtime_error("invalid utf-8 sequence");
            }
            return ch;
        };

        char32_t codepoint = first_byte & byte_1_mask[num_bytes];
        switch (num_bytes)
        {
            case 6:
                codepoint = (codepoint << 6) | (read_byte(istr) & 0x3fu);
            case 5:
                codepoint = (codepoint << 6) | (read_byte(istr) & 0x3fu);
            case 4:
                codepoint = (codepoint << 6) | (read_byte(istr) & 0x3fu);
            case 3:
                codepoint = (codepoint << 6) | (read_byte(istr) & 0x3fu);
            case 2:
                codepoint = (codepoint << 6) | (read_byte(istr) & 0x3fu);
            case 1:
                return codepoint;
            default:
                throw std::runtime_error("invalid utf-8 sequence");
        }
    }
};

// UTF-16 encoding
// http://en.wikipedia.org/wiki/UTF-16
// http://tools.ietf.org/html/rfc2781
struct utf16
{
    using char_type = char16_t;

    static_assert(sizeof(char_type) >= 2,
                  "UTF-16 needs at least 2 byte character");

    template <typename OStream>
    static void encode(OStream& ostr, char32_t codepoint)
    {
        if (codepoint < 0x10000)
        {
            if (codepoint >= 0xd800 && codepoint <= 0xdfff)
            {
                throw std::runtime_error("invalid utf-16 codepoint");
            }
            ostr.put(static_cast<char_type>(codepoint));
        }
        else if (codepoint <= 0x10ffff)
        {
            ostr.put(static_cast<char_type>((codepoint >> 10) + 0xd7c0));
            ostr.put((codepoint & 0x3ff) + 0xdc00);
        }
        else
        {
            throw std::runtime_error("invalid utf-16 codepoint");
        }
    }

    template <typename IStream>
    static char32_t decode(IStream& istr)
    {
        static_assert(sizeof(typename IStream::char_type) == 2, "");
        const char_type first_code_unit = istr.get();
        if (first_code_unit < 0xd800 || first_code_unit > 0xdfff)
        {
            return first_code_unit;
        }
        else if (first_code_unit <= 0xdbff)
        {
            const char_type second_code_unit = istr.get();
            if (second_code_unit < 0xdc00 || second_code_unit > 0xdfff)
            {
                throw std::runtime_error("invalid utf-16 sequence");
            }
            return (first_code_unit << 10) + second_code_unit - 0x35fdc00;
        }
        else
        {
            throw std::runtime_error("invalid utf-16 sequence");
        }
    }
};

// UTF-32 encoding.
struct utf32
{
    using char_type = char32_t;

    template <typename OStream>
    static void encode(OStream& ostr, char32_t codepoint)
    {
        if (codepoint > 0x10ffff)
        {
            throw std::runtime_error("invalid utf-32 sequence");
        }
        ostr.put(codepoint);
    }

    template <typename IStream>
    static char32_t decode(IStream& istr)
    {
        static_assert(sizeof(typename IStream::char_type) == 4, "");
        char_type ch = istr.get();
        if (ch > 0x10ffff)
        {
            throw std::runtime_error("invalid utf-32 sequence");
        }

        return ch;
    }
};

// deduce encoding type...
template <typename T>
struct encoding;

template <>
struct encoding<char>
{
    using type = utf8;
};

template <>
struct encoding<char16_t>
{
    using type = utf16;
};

template <>
struct encoding<char32_t>
{
    using type = utf32;
};

} // namespace native

#endif
