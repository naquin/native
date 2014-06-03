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

#ifndef native_integers_h__
#define native_integers_h__

#include "native/config.h"

#include "native/json/detail/number_parse.h"

#include "native/string_conversion.h"

#include <cassert>

namespace native {
namespace json {
namespace detail {

//
// Integer conversions adapted from https://github.com/facebook/folly
//

// Lookup tables that converts from a decimal character value to an integral
// binary value, shifted by a decimal "shift" multiplier.
// For all character values in the range '0'..'9', the table at those
// index locations returns the actual decimal value shifted by the multiplier.
// For all other values, the lookup table returns an invalid OOR value.
//
// Out-of-range flag value, larger than the largest value that can fit in
// four decimal bytes (9999), but four of these added up together should
// still not overflow uint16_t.
constexpr std::int32_t OOR = 10000;

__attribute__((aligned(16))) constexpr std::uint16_t shift1[] =
{
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 0-9
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  10
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  20
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  30
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, 0,         //  40
    1, 2, 3, 4, 5, 6, 7, 8, 9, OOR, OOR,
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  60
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  70
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  80
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  90
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 100
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 110
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 120
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 130
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 140
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 150
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 160
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 170
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 180
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 190
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 200
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 210
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 220
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 230
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 240
    OOR, OOR, OOR, OOR, OOR, OOR                       // 250
};

__attribute__((aligned(16))) constexpr uint16_t shift10[] =
{
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 0-9
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  10
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  20
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  30
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, 0,         //  40
    10, 20, 30, 40, 50, 60, 70, 80, 90, OOR, OOR,
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  60
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  70
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  80
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  90
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 100
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 110
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 120
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 130
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 140
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 150
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 160
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 170
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 180
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 190
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 200
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 210
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 220
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 230
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 240
    OOR, OOR, OOR, OOR, OOR, OOR                       // 250
};

__attribute__((aligned(16))) constexpr uint16_t shift100[] =
{
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 0-9
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  10
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  20
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  30
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, 0,         //  40
    100, 200, 300, 400, 500, 600, 700, 800, 900, OOR, OOR,
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  60
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  70
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  80
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  90
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 100
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 110
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 120
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 130
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 140
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 150
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 160
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 170
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 180
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 190
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 200
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 210
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 220
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 230
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 240
    OOR, OOR, OOR, OOR, OOR, OOR                       // 250
};

__attribute__((aligned(16))) constexpr uint16_t shift1000[] =
{
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 0-9
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  10
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  20
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  30
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, 0,         //  40
    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, OOR, OOR,
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  60
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  70
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  80
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  //  90
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 100
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 110
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 120
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 130
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 140
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 150
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 160
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 170
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 180
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 190
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 200
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 210
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 220
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 230
    OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR, OOR,  // 240
    OOR, OOR, OOR, OOR, OOR, OOR                       // 250
};

    
// Maximum value of number when represented as a string
template <class T> struct max_string
{
    constexpr static const char* const value =
        to_string_literal<T, std::numeric_limits<T>::max()>::value;
};

template <typename T>
T string_to_unsigned(const char* b, const std::size_t size)
{
    static_assert(!std::is_signed<T>::value, "Unsigned type expected");
    
    const char* e = b + size;

    if ( *b == '0' && size > 1 )
    {
        throw std::range_error("Leading zeros are not allowed");
    }
    
    /* Although the string is entirely made of digits, we still need to
     * check for overflow.
     */
    if (size >= std::numeric_limits<T>::digits10 + 1)
    {
        if ( !(size == std::numeric_limits<T>::digits10 + 1 &&
            strncmp(b, max_string<T>::value, size) <= 0) )
        {
            throw std::range_error("Numeric overflow upon conversion");
        }
    }
    
    // Here we know that the number won't overflow when
    // converted. Proceed without checks.
    
    T result = 0;
    for (; e - b >= 4; b += 4)
    {
        result *= 10000;
        const int32_t r0 = shift1000[static_cast<size_t>(b[0])];
        const int32_t r1 = shift100[static_cast<size_t>(b[1])];
        const int32_t r2 = shift10[static_cast<size_t>(b[2])];
        const int32_t r3 = shift1[static_cast<size_t>(b[3])];
        const auto sum = r0 + r1 + r2 + r3;
        assert(sum < OOR && "Assumption: string only has digits");
        result += sum;
    }

    switch (e - b)
    {
        case 3:
        {
            const int32_t r0 = shift100[static_cast<size_t>(b[0])];
            const int32_t r1 = shift10[static_cast<size_t>(b[1])];
            const int32_t r2 = shift1[static_cast<size_t>(b[2])];
            const auto sum = r0 + r1 + r2;
            assert(sum < OOR && "Assumption: string only has digits");
            return result * 1000 + sum;
        }
        case 2:
        {
            const int32_t r0 = shift10[static_cast<size_t>(b[0])];
            const int32_t r1 = shift1[static_cast<size_t>(b[1])];
            const auto sum = r0 + r1;
            assert(sum < OOR && "Assumption: string only has digits");
            return result * 100 + sum;
        }
        case 1:
        {
            const int32_t sum = shift1[static_cast<size_t>(b[0])];
            assert(sum < OOR && "Assumption: string only has digits");
            return result * 10 + sum;
        }
    }
    
    assert(b == e);
    return result;
}

    
template <typename T, typename U>
typename std::enable_if<std::numeric_limits<T>::is_signed, T>::type
    string_to_integer(const number_parse<U>& attribs)
{
    if (attribs.sign)
    {
        T number = -string_to_unsigned<typename std::make_unsigned<T>::type>(attribs.buffer, attribs.length);
        if ( !(number <= 0 ) )
        {
            throw std::range_error("Negative overflow.");
        }
        return number;
    }
    else
    {
        T number = string_to_unsigned<typename std::make_unsigned<T>::type>(attribs.buffer, attribs.length);
        if ( !(number >= 0) )
        {
            throw std::range_error("Overflow.");
        }
        return number;
    }
}


template <typename T, typename U>
typename std::enable_if<!std::numeric_limits<T>::is_signed, T>::type
    string_to_integer(const number_parse<U>& attribs)
{
    return string_to_unsigned<T>(attribs.buffer, attribs.length);
}

} } } // namespace native::json::detail

#endif
