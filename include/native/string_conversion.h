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

#ifndef NATIVE_STRING_CONVERSION_H__
#define NATIVE_STRING_CONVERSION_H__

#include "native/config.h"

#include <type_traits>

namespace native {

// similar to http://stackoverflow.com/a/24000041/766172
namespace detail
{

template<uint8_t... digits>
struct unsigned_to_chars
{
    static const char value[];
};

template<uint8_t... digits>
const char unsigned_to_chars<digits...>::value[] = {('0' + digits)..., 0};


template<uint8_t... digits>
struct signed_to_chars
{
    static const char value[];
};

template<uint8_t... digits>
const char signed_to_chars<digits...>::value[] = {'-', ('0' + digits)..., 0};

template<bool neg, uint8_t... digits>
struct to_chars: unsigned_to_chars<digits...>
{
};

template<uint8_t... digits>
struct to_chars<true, digits...>: signed_to_chars<digits...>
{

};

template<bool neg, uintmax_t rem, uint8_t... digits>
struct expand: expand<neg, rem / 10, rem % 10, digits...>
{

};

template<bool neg, uint8_t... digits>
struct expand<neg, 0, digits...>: to_chars<neg, digits...>
{

};

template<typename T>
constexpr uintmax_t const_abs(T num)
{
    return (num < 0) ? -num : num;
}

} // namespace detail


template <typename Integer, Integer num>
struct to_string_literal: detail::expand<(num < 0), detail::const_abs(num)>
{

};

} // namespace native

#endif
