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

#ifndef NATIVE_STRING_BENCHMARKS_H__
#define NATIVE_STRING_BENCHMARKS_H__

#include "test.h"
#include "native/istring.h"

template <typename String>
void benchmark_string_copy(String s, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        String s2 = s;
    }
}

template <typename String>
void benchmark_string_index(String& s)
{
    typename String::value_type ch = s[0];
    for (std::size_t i = 1; i < s.size(); ++i)
    {
        ch = std::max(ch, s[i]);
    }
}

template <typename String>
void benchmark_string_substr(const String& s)
{
    auto sub = s.substr(5, 30);
}

#endif
