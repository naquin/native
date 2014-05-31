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

#ifndef native_number_parse_h__
#define native_number_parse_h__

#include "native/config.h"

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <limits>
#include <type_traits>

namespace native {
namespace json {
namespace detail {

template <typename T>
struct number_parse
{
    typedef T value_type;

    typedef std::numeric_limits<T> numeric_limitis;
    
    constexpr static std::uint16_t max_significant_digits =
        2 + numeric_limitis::digits - numeric_limitis::min_exponent;

    constexpr static std::uint16_t max_buffer_length = max_significant_digits + 10;

    constexpr static std::uint16_t buffer_size = max_significant_digits + 10;
    char buffer[buffer_size];  // NOLINT: size is known at compile time.
    std::uint32_t length                = 0;
    std::int32_t  exponent              = 0;
    std::int32_t  significant_digits    = 0;
    std::int32_t  insignificant_digits  = 0;
    bool nonzero_digit_dropped          = false;
    bool sign                           = false;

    bool is_real() const
    {
        return !(exponent == 0 );
    }
    
    // Adapted from V8 double-conversion
    template <typename Stream>
    void to_buffer(Stream& stream)
    {
        // The longest form of simplified number is: "-<significant digits>.1eXXX\0".

        // Exponent will be adjusted if insignificant digits of the integer part
        // or insignificant leading zeros of the fractional part are dropped.

        if ( stream.peek() == '-')
        {
            sign = true;
            stream.next();
            if ( stream.peek() < '0' || stream.peek() > '9' )
            {
                throw std::range_error("Expected a digit after minus sign");
            }
        }

        const bool leading_zero = stream.peek() == '0';
        if (leading_zero)
        {
            stream.next();

            switch ( stream.peek() )
            {
            case 'e':
            case 'E': // exponent
            case '.': // this is a decimal
                // A single leading zero is only allowed for decimals
                break;
            case ']':
            case ',':
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                return; // we've reached the end of the number
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                throw std::range_error("Leading zeros are not allowed.");
            }
        }

        // Copy significant digits of the integer part (if any) to the buffer.
        while (stream.peek() >= '0' && stream.peek() <= '9')
        {
            if (significant_digits < max_significant_digits)
            {
                assert(length < buffer_size);
                buffer[length++] = static_cast<char>(stream.peek());
                ++significant_digits;
            }
            else
            {
                ++insignificant_digits;  // Move the digit into the exponential part.
                nonzero_digit_dropped = nonzero_digit_dropped || stream.peek() != '0';
            }
            stream.next();
        }

        if (!leading_zero && significant_digits == 0)
        {
            throw std::range_error("Expected a digit");
        }

        if (stream.peek() == '.')
        {
            stream.next();

            switch ( stream.peek() ) // check for digit after dot
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            default:
                throw std::range_error("Expected digit after decimal.");
            }

            if (significant_digits == 0)
            {
                // Integer part consists of 0 or is absent. Significant digits start after
                // leading zeros (if any).
                while (stream.peek() == '0')
                {
                    stream.next();
                    --exponent;  // Move this 0 into the exponent.
                }
            }
            
            // There is a fractional part.
            // We don't emit a '.', but adjust the exponent instead.
            while (stream.peek() >= '0' && stream.peek() <= '9')
            {
                if (significant_digits < max_significant_digits)
                {
                    assert(length < buffer_size);
                    buffer[length++] = static_cast<char>(stream.peek());
                    ++significant_digits;
                    --exponent;
                }
                else
                {
                    // Ignore insignificant digits in the fractional part.
                    nonzero_digit_dropped = nonzero_digit_dropped || stream.peek() != '0';
                }
                stream.next();
            }
        }

        if (!leading_zero && exponent == 0 && significant_digits == 0)
        {
            // If leading_zeros is true then the string contains zeros.
            // If exponent < 0 then string was [+-]\.0*...
            // If significant_digits != 0 the string is not equal to 0.
            // Otherwise there are no digits in the string.
            return;
        }

        // Parse exponential part.
        if (stream.peek() == 'e' || stream.peek() == 'E')
        {
            stream.next();
            char sign = '+';
            if (stream.peek() == '+' || stream.peek() == '-')
            {
                sign = static_cast<char>(stream.peek());
                stream.next();
            }
            
            if (stream.peek() < '0' || stream.peek() > '9')
            {
                throw std::range_error("Expected digit after exponent start");
            }

            const int max_exponent = std::numeric_limits<T>::max_exponent;
            assert(-max_exponent / 2 <= exponent && exponent <= max_exponent / 2);
            int num = 0;
            do
            {
                // Check overflow.
                int digit = stream.peek() - '0';
                if (num >= max_exponent / 10
                    && !(num == max_exponent / 10 && digit <= max_exponent % 10))
                {
                    num = max_exponent;
                }
                else
                {
                    num = num * 10 + digit;
                }
                stream.next();
            }
            while (stream.peek() >= '0' && stream.peek() <= '9');
            
            exponent += (sign == '-' ? -num : num);
        }

        exponent += insignificant_digits;

        if (nonzero_digit_dropped)
        {
            buffer[length++] = '1';
            exponent--;
        }
        
        assert(length < buffer_size);
        buffer[length] = '\0';
    }
};


} } } // namespace native::json::detail

#endif
