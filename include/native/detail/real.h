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

#include "native/detail/integers.h"

#include "native/detail/double-conversion/strtod.h"
#include "native/detail/double-conversion/double-conversion.h"

namespace native {
namespace detail {

template <typename T>
T string_to_real(const number_parse<T>& attribs);

template <>
inline long double
string_to_real<long double>(const number_parse<long double>& attribs) {
    using namespace double_conversion;
    double converted = Strtod(
        Vector<const char>(attribs.buffer, attribs.length), attribs.exponent);
    return attribs.sign ? -converted : converted;
}

template <>
inline double string_to_real<double>(const number_parse<double>& attribs) {
    using namespace double_conversion;
    double converted = Strtod(
        Vector<const char>(attribs.buffer, attribs.length), attribs.exponent);
    return attribs.sign ? -converted : converted;
}

template <>
inline float string_to_real<float>(const number_parse<float>& attribs) {
    using namespace double_conversion;
    float converted = Strtof(Vector<const char>(attribs.buffer, attribs.length),
                             attribs.exponent);
    return attribs.sign ? -converted : converted;
}

constexpr int kConvMaxDecimalInShortestLow = -6;
constexpr int kConvMaxDecimalInShortestHigh = 21;

// Wrapper around DoubleToStringConverter
template <class Stream, class Source>
typename std::enable_if<std::is_floating_point<Source>::value>::type
stream_append(Stream& stream, Source value,
              double_conversion::DoubleToStringConverter::DtoaMode mode,
              unsigned int numDigits) {
    using namespace double_conversion;
    DoubleToStringConverter conv(DoubleToStringConverter::NO_FLAGS, "infinity",
                                 "NaN", 'E', kConvMaxDecimalInShortestLow,
                                 kConvMaxDecimalInShortestHigh,
                                 6,  // max leading padding zeros
                                 1); // max trailing padding zeros
    char buffer[256];
    StringBuilder builder(buffer, sizeof(buffer));
    switch (mode) {
        case DoubleToStringConverter::SHORTEST:
            conv.ToShortest(value, &builder);
            break;
        case DoubleToStringConverter::SHORTEST_SINGLE:
            conv.ToShortestSingle(value, &builder);
            break;
        case DoubleToStringConverter::FIXED:
            conv.ToFixed(value, numDigits, &builder);
            break;
        case DoubleToStringConverter::PRECISION:
            conv.ToPrecision(value, numDigits, &builder);
            break;
    }
    const size_t length = builder.position();
    builder.Finalize();
    stream.write(buffer, length);
}

// As above, but for floating point
template <class Stream, class Source>
typename std::enable_if<std::is_floating_point<Source>::value>::type
stream_append(Stream& stream, Source value) {
    stream_append(stream, value,
                  double_conversion::DoubleToStringConverter::SHORTEST, 0);
}

} // namespace native::detail
} // namespace native

#endif
