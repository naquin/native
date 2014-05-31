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

#ifndef NATIVE_JSON_EXCEPTIONS_H__
#define NATIVE_JSON_EXCEPTIONS_H__

#include "native/config.h"

#include <exception>

namespace native {
namespace json {

struct json_exception: public std::runtime_error
{
    json_exception(const std::string& message,
                   std::size_t line,
                   std::size_t column):
        std::runtime_error(message +
                           ". Line " + std::to_string(line) +
                           ", column " + std::to_string(column) + "."),
        _line(line),
        _column(column)
    { }

    std::size_t line() const
    {
        return _line;
    }

    std::size_t column() const
    {
        return _column;
    }

private:
    std::size_t _line;
    std::size_t _column;
};

    
#define NATIVE_JSON_EXCEPTION_DECL(className, messageText)                      \
    struct className: json_exception {                                          \
        className(std::size_t line, std::size_t column):                        \
            json_exception(messageText, line, column)                           \
        { }                                                                     \
    };


NATIVE_JSON_EXCEPTION_DECL(expected_end_of_stream,
                           "Expected end of stream")
NATIVE_JSON_EXCEPTION_DECL(unexpected_end_of_stream,
                           "Unexpected end of stream")
NATIVE_JSON_EXCEPTION_DECL(expected_object_or_array,
                           "Expected object or array")
NATIVE_JSON_EXCEPTION_DECL(expected_colon_after_key,
                           "Expected colon after key")
NATIVE_JSON_EXCEPTION_DECL(expected_comma_or_close_curly_brace,
                           "Expected comma or close curly brace")
NATIVE_JSON_EXCEPTION_DECL(expected_comma_or_close_bracket,
                           "Expected comman or close bracket")
NATIVE_JSON_EXCEPTION_DECL(expected_null_value,
                           "Expected null value")
NATIVE_JSON_EXCEPTION_DECL(expected_true_value,
                           "Expected true value")
NATIVE_JSON_EXCEPTION_DECL(expected_false_value,
                           "Expected false value")
NATIVE_JSON_EXCEPTION_DECL(incorrect_hex_digit,
                           "Incorrect hex digit")
NATIVE_JSON_EXCEPTION_DECL(missing_second_in_surrogate_pair,
                           "Missing second in surrogate pair")
NATIVE_JSON_EXCEPTION_DECL(invalid_second_in_surrogate_pair,
                           "Invalid second in surrogate pair")
NATIVE_JSON_EXCEPTION_DECL(unknown_escape_character,
                           "Unknown escape character")
NATIVE_JSON_EXCEPTION_DECL(missing_start_quote,
                           "Missig start quote")
NATIVE_JSON_EXCEPTION_DECL(missing_end_quote,
                           "Missing end quote")
NATIVE_JSON_EXCEPTION_DECL(incorrect_unescaped_character,
                           "Incorrect unescaped character")
NATIVE_JSON_EXCEPTION_DECL(invalid_encoding,
                           "Invalid encoding")
NATIVE_JSON_EXCEPTION_DECL(expected_value,
                           "Expected value")
NATIVE_JSON_EXCEPTION_DECL(unexpected_signed_value,
                           "The number given was signed when only unsigned was expected")
NATIVE_JSON_EXCEPTION_DECL(number_too_big_for_expected_type,
                           "The number given was too big for the expected type")
NATIVE_JSON_EXCEPTION_DECL(unexpected_decimal_value,
                           "The number given was a decimal instead of an integer")
NATIVE_JSON_EXCEPTION_DECL(at_least_one_digit_in_fraction_part,
                           "At least one digit in fraction part")
NATIVE_JSON_EXCEPTION_DECL(at_least_one_digit_in_exponent,
                           "At least one digit in exponent")
NATIVE_JSON_EXCEPTION_DECL(unexpected_character,
                           "An unexpected character was found")
} } // namespace native::json

#endif
