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

#ifndef NATIVE_TYPES_H__
#define NATIVE_TYPES_H__

#include <cstdint>

namespace native {

using int8  = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using int_fast8  = std::int_fast8_t;
using int_fast16 = std::int_fast16_t;
using int_fast32 = std::int_fast32_t;
using int_fast64 = std::int_fast64_t;

using int_least8  = std::int_least8_t;
using int_least16 = std::int_least16_t;
using int_least32 = std::int_least32_t;
using int_least64 = std::int_least64_t;

using intmax = std::intmax_t;
using intptr = std::intptr_t;

using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using uint_fast8 = std::uint_fast8_t;
using uint_fast16 = std::uint_fast16_t;
using uint_fast32 = std::uint_fast32_t;
using uint_fast64 = std::uint_fast64_t;

using uint_least8 = std::uint_least8_t;
using uint_least16 = std::uint_least16_t;
using uint_least32 = std::uint_least32_t;
using uint_least64 = std::uint_least64_t;

using uintmax = std::uintmax_t;
using uintptr = std::uintptr_t;

using std::size_t;

} // namespace native

#endif
