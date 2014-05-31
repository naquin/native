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

#ifndef NATIVE_REAL_H__
#define NATIVE_REAL_H__

#include "native/config.h"

#include "native/json/detail/integers.h"

#include "strtod.h" // double-conversion


namespace native {
namespace json {
namespace detail {

template <typename T>
T string_to_real(const number_parse<T>& attribs);

       
template <>
inline long double string_to_real<long double>(const number_parse<long double>& attribs)
{
    using namespace double_conversion;
    double converted = Strtod(Vector<const char>(attribs.buffer, attribs.length), attribs.exponent);
    return attribs.sign ? -converted: converted;
}

template <>
inline double string_to_real<double>(const number_parse<double>& attribs)
{
    using namespace double_conversion;
    double converted = Strtod(Vector<const char>(attribs.buffer, attribs.length), attribs.exponent);
    return attribs.sign ? -converted: converted;
}

template <>
inline float string_to_real<float>(const number_parse<float>& attribs)
{
    using namespace double_conversion;
    float converted = Strtof(Vector<const char>(attribs.buffer, attribs.length), attribs.exponent);
    return attribs.sign ? -converted: converted;
}

} } } // namespace native::json::detail

#endif
