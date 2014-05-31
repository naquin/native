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

#ifndef NATIVE_JSON_TYPES_H__
#define NATIVE_JSON_TYPES_H__

#include "native/config.h"

#include <string>

namespace native
{
namespace json
{

// enumeration for JSON-types
enum element_type : unsigned char
{
    json_null,
    json_array,
    json_bool,
    json_integer,
    json_real,
    json_object,
    json_string,
};

// enumeration for C++ types
enum data_type : unsigned char
{
    type_unknown,
    type_object,
    type_array,
    type_string,
    type_bool,
    type_short,
    type_unsigned_short,
    type_int,
    type_long,
    type_long_long,
    type_unsigned,
    type_unsigned_long,
    type_unsigned_long_long,
    type_float,
    type_double,
    type_long_double,
};

template <typename T>
struct type_mapper;

template <>
struct type_mapper<const char*>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<std::basic_string<char>>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<const char16_t*>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<std::basic_string<char16_t>>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<const char32_t*>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<std::basic_string<char32_t>>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<const wchar_t*>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<std::basic_string<wchar_t>>
{
    static constexpr data_type value = type_string;
};

template <>
struct type_mapper<bool>
{
    static constexpr data_type value = type_bool;
};

template <>
struct type_mapper<short>
{
    static constexpr data_type value = type_short;
};

template <>
struct type_mapper<unsigned short>
{
    static constexpr data_type value = type_unsigned_short;
};

template <>
struct type_mapper<int>
{
    static constexpr data_type value = type_int;
};

template <>
struct type_mapper<long>
{
    static constexpr data_type value = type_long;
};

template <>
struct type_mapper<long long>
{
    static constexpr data_type value = type_long_long;
};

template <>
struct type_mapper<unsigned>
{
    static constexpr data_type value = type_unsigned;
};

template <>
struct type_mapper<unsigned long>
{
    static constexpr data_type value = type_unsigned_long;
};

template <>
struct type_mapper<unsigned long long>
{
    static constexpr data_type value = type_unsigned_long_long;
};

template <>
struct type_mapper<float>
{
    static constexpr data_type value = type_float;
};

template <>
struct type_mapper<double>
{
    static constexpr data_type value = type_double;
};

template <>
struct type_mapper<long double>
{
    static constexpr data_type value = type_long_double;
};
}
} // namespace native::json

#endif
