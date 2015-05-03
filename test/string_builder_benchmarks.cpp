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

#include "test.h"

#include "benchmark.h"

#include "native/string_builder.h"

#include <sstream>

using namespace native;
using std::to_string;

template <typename Stream>
void test_strings()
{
    Stream ostr;
    ostr << "The" << ' ' << "quick" << ' ' << "brown" << ' ' << "fox" << ' '
         << "jumps" << ' ' << "over" << ' ' << "the" << ' ' << "lazy" << ' '
         << "dog";
    ostr.str();
}

template <typename Stream>
void test_integers()
{
    Stream ostr;
    ostr << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    ostr.str();
}

template <typename Stream>
void test_floats()
{
    Stream ostr;
    ostr << 1.1 << 2.2 << 3.3 << 4.4 << 5.5 << 6.6 << 7.7 << 8.8 << 9.9;
    ostr.str();
}

BENCHMARK(benchmark_test, build_std_stringstream)
{

    benchmark([]()
              {
                  test_strings<std::ostringstream>();
              });
}

BENCHMARK(benchmark_test, build_native_string_builder)
{
    benchmark([]()
              {
                  test_strings<string_builder>();
              });
}

BENCHMARK(benchmark_test, build_integers_std_stringstream)
{

    benchmark([]()
              {
                  test_integers<std::ostringstream>();
              });
}

BENCHMARK(benchmark_test, build_integers_native_string_builder)
{
    benchmark([]()
              {
                  test_integers<string_builder>();
              });
}

BENCHMARK(benchmark_test, build_floats_std_stringstream)
{

    benchmark([]()
              {
                  test_floats<std::ostringstream>();
              });
}

BENCHMARK(benchmark_test, build_floats_native_string_builder)
{
    benchmark([]()
              {
                  test_floats<string_builder>();
              });
}
