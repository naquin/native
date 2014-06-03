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

#ifndef NATIVE_JSON_CONVERSION_H__
#define NATIVE_JSON_CONVERSION_H__

#include "native/config.h"

#include "native/json/input_streams.h"

#include "native/json/detail/number_parse.h"
#include "native/json/detail/integers.h"
#include "native/json/detail/real.h"

#include <boost/implicit_cast.hpp>

#include <string>
#include <type_traits>

namespace native {
namespace json {

// integer to integer
template <typename T, typename U>
typename std::enable_if<std::is_integral<U>::value && std::is_integral<T>::value, T>::type
    to(U value)
{
    // no range check
    return static_cast<T>(value);
}

// real to real
template <typename T, typename U>
typename std::enable_if<std::is_floating_point<U>::value && std::is_floating_point<T>::value, T>::type
    to(U value)
{
    // no range check
    return boost::implicit_cast<T>(value);
}

// integer to real
template <typename T, typename U>
typename std::enable_if<std::is_integral<U>::value && std::is_floating_point<T>::value, T>::type
    to(U value)
{
    // no range check
    return boost::implicit_cast<T>(value);
}

// real to integer
template <typename T, typename U>
typename std::enable_if<std::is_floating_point<U>::value && std::is_integral<T>::value, T>::type
    to(U value)
{
    // no range check
    return boost::implicit_cast<T>(value);
}

// integer to string
template <typename T, typename U>
typename std::enable_if<std::is_same<T, std::string>::value && std::is_integral<U>::value, T>::type
    to(U value)
{
    return std::to_string(value);
}

// real to string
template <typename T, typename U>
typename std::enable_if<std::is_same<T, std::string>::value && std::is_floating_point<U>::value, T>::type
    to(U value)
{
    return std::to_string(value);
}

// bool to string
template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, T>::type
    to(bool value)
{
    return value ? "true" : "false";
}

// string to string
template <typename T>
const typename std::enable_if<std::is_same<T, std::string>::value, T>::type&
    to(const std::string& value)
{
    return value;
}

// string to bool
template <typename T>
typename std::enable_if<std::is_same<T, bool>::value, T>::type
    to(const std::string& value)
{
    if ( value.empty() )
    {
        return false;
    }
    else
    {
        return value[0] == 't'; // lazy
    }
}

// string to integer
template <typename T>
typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type
    to(const std::string& value)
{
    detail::number_parse<T> p;
    auto stream = make_parser_range_iterator(value.begin(), value.end());
    p.to_buffer(stream);
    return detail::string_to_integer<T>(p);
}

// string to real
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
    to(const std::string& value)
{
    detail::number_parse<T> p;
    auto stream = make_parser_range_iterator(value.begin(), value.end());
    p.to_buffer(stream);
    return detail::string_to_real<T>(p);
}
    
} } // namespace native::json

#endif
