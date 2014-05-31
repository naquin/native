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

#include "native/json/detail/integers.h"

namespace native {
namespace json {
namespace detail {
    
template <> const char *const max_string<std::uint8_t>::value = "255";
template <> const char *const max_string<std::uint16_t>::value = "65535";
template <> const char *const max_string<std::uint32_t>::value = "4294967295";
#if __SIZEOF_LONG__ == 4
template <> const char *const max_string<unsigned long>::value =
"4294967295";
#else
template <> const char *const max_string<unsigned long>::value =
"18446744073709551615";
#endif
static_assert(sizeof(unsigned long) >= 4,
              "Wrong value for max_string<unsigned long>::value,"
              " please update.");
template <> const char *const max_string<unsigned long long>::value =
"18446744073709551615";
static_assert(sizeof(unsigned long long) >= 8,
              "Wrong value for max_string<unsigned long long>::value"
              ", please update.");



} } } // namespace native::json::detail
