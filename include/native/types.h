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

typedef std::int8_t  int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;

typedef std::int_fast8_t  int_fast8;
typedef std::int_fast16_t int_fast16;
typedef std::int_fast32_t int_fast32;
typedef std::int_fast64_t int_fast64;

typedef std::int_least8_t  int_least8;
typedef std::int_least16_t int_least16;
typedef std::int_least32_t int_least32;
typedef std::int_least64_t int_least64;

typedef std::intmax_t intmax;
typedef std::intptr_t intptr;

typedef std::uint8_t  uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;

typedef std::uint_fast8_t  uint_fast8;
typedef std::uint_fast16_t uint_fast16;
typedef std::uint_fast32_t uint_fast32;
typedef std::uint_fast64_t uint_fast64;

typedef std::uint_least8_t  uint_least8;
typedef std::uint_least16_t uint_least16;
typedef std::uint_least32_t uint_least32;
typedef std::uint_least64_t uint_least64;

typedef std::uintmax_t uintmax;
typedef std::uintptr_t uintptr;

using std::size_t;

} // namespace native

#endif
