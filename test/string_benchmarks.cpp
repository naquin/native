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
#include "string_benchmarks.h"

#include "benchmark.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <sstream>
#include <unordered_map>

using namespace native;
using std::to_string;

#define STRING_10 "0123456789"

#define STRING_100                                                             \
    STRING_10 STRING_10 STRING_10 STRING_10 STRING_10 STRING_10 STRING_10      \
        STRING_10 STRING_10 STRING_10

#define STRING_1000                                                            \
    STRING_100 STRING_100 STRING_100 STRING_100 STRING_100 STRING_100          \
        STRING_100 STRING_100 STRING_100 STRING_100

#define STRING_10000                                                           \
    STRING_1000 STRING_1000 STRING_1000 STRING_1000 STRING_1000 STRING_1000    \
        STRING_1000 STRING_1000 STRING_1000 STRING_1000

#define STRING_100000                                                          \
    STRING_10000 STRING_10000 STRING_10000 STRING_10000 STRING_10000           \
        STRING_10000 STRING_10000 STRING_10000 STRING_10000 STRING_10000

#define STRING_1000000                                                         \
    STRING_100000 STRING_100000 STRING_100000 STRING_100000 STRING_100000      \
        STRING_100000 STRING_100000 STRING_100000 STRING_100000 STRING_100000

static const size_t NUM_ASSIGNS = 10;

static std::vector<const char*> SMALL_STRINGS = {STRING_10, STRING_100,
                                                 STRING_1000};
static std::vector<const char*> LARGE_STRINGS = {
    STRING_10,    STRING_100,    STRING_1000,
    STRING_10000, STRING_100000, STRING_1000000,
};

BENCHMARK(benchmark_test, assign_std_string)
{
    for (std::string s : SMALL_STRINGS)
    {
        for (std::size_t i = 0; i < NUM_ASSIGNS; ++i)
        {
            auto benchmark_name = "assign_std_string," + to_string(s.size()) +
                                  "," + to_string(i + 1);
            benchmark(benchmark_name, [&s, i]()
                      {
                benchmark_string_copy(s, i + 1);
            });
        }
    }
}

BENCHMARK(benchmark_test, assign_istring)
{
    for (istring s : SMALL_STRINGS)
    {
        for (std::size_t i = 0; i < NUM_ASSIGNS; ++i)
        {
            auto benchmark_name = "assign_istring," + to_string(s.size()) +
                                  "," + to_string(i + 1);
            benchmark(benchmark_name, [&s, i]()
                      {
                benchmark_string_copy(s, i + 1);
            });
        }
    }
}

BENCHMARK(benchmark_test, assign_istring_literal)
{
    auto do_benchmark = [&](const istring& s)
    {
        for (std::size_t i = 0; i < NUM_ASSIGNS; ++i)
        {
            auto benchmark_name = "assign_istring_literal," +
                                  to_string(s.size()) + "," + to_string(i + 1);
            benchmark(benchmark_name, [&s, i]()
                      {
                benchmark_string_copy(s, i + 1);
            });
        }
    };

    do_benchmark(istring::literal(STRING_10));
    do_benchmark(istring::literal(STRING_100));
    do_benchmark(istring::literal(STRING_1000));
}

BENCHMARK(benchmark_test, hash_std_string)
{
    auto do_benchmark = [&](const std::string& s)
    {
        auto benchmark_name = "hash_std_string," + to_string(s.size());

        benchmark(benchmark_name, [&s]()
                  {
            benchmark_string_hash(s);
        });
    };

    for (std::string s : LARGE_STRINGS)
    {
        do_benchmark(s);
    }
}

BENCHMARK(benchmark_test, hash_istring)
{
    auto do_benchmark = [&](const istring& s)
    {
        auto benchmark_name = "hash_istring," + to_string(s.size());

        benchmark(benchmark_name, [&s]()
                  {
            benchmark_string_hash(s);
        });
    };

    for (istring s : LARGE_STRINGS)
    {
        do_benchmark(s);
    }
}

BENCHMARK(benchmark_test, hash_istring_literal)
{
    auto do_benchmark = [&](const istring& s)
    {
        auto benchmark_name = "hash_istring_literal," + to_string(s.size());

        benchmark(benchmark_name, [&s]()
                  {
            benchmark_string_hash(s);
        });
    };

    do_benchmark(istring::literal(STRING_10));
    do_benchmark(istring::literal(STRING_100));
    do_benchmark(istring::literal(STRING_1000));
    do_benchmark(istring::literal(STRING_10000));
    do_benchmark(istring::literal(STRING_100000));
    do_benchmark(istring::literal(STRING_1000000));
}

BENCHMARK(benchmark_test, index_std_string)
{
    std::string s = STRING_100;
    benchmark([&s]()
              {
                  benchmark_string_index(s);
              });
}

BENCHMARK(benchmark_test, index_native_string)
{
    istring s = STRING_100;
    benchmark([&s]()
              {
                  benchmark_string_index(s);
              });
}

BENCHMARK(benchmark_test, split_std_string)
{
    std::string s = STRING_100;
    std::vector<std::string> result;
    benchmark([&s, &result]()
              {
                  boost::algorithm::split(result, s,
                                          boost::algorithm::is_any_of("5"));
              });
}

BENCHMARK(benchmark_test, split_native_string)
{
    istring s = STRING_100;
    benchmark([&s]()
              {
                  auto result = string_slice(s).split('5');
              });
}

BENCHMARK(benchmark_test, substr_std_string)
{
    std::string s = STRING_100;
    benchmark([&s]()
              {
                  benchmark_string_substr(s);
              });
}

BENCHMARK(benchmark_test, substr_native_string)
{
    istring s = STRING_100;
    benchmark([&s]()
              {
                  benchmark_string_substr(s);
              });
}

BENCHMARK(benchmark_test, unordered_map_std_string)
{
    const auto keys = benchmark_create_unordered_map_keys<std::string>();
    const auto map = benchmark_create_unordered_map<std::string>(keys);
    benchmark([&keys, &map]()
              {
                  benchmark_unordered_map(keys, map);
              });
}

BENCHMARK(benchmark_test, unordered_map_native_string)
{
    const auto keys = benchmark_create_unordered_map_keys<istring>();
    const auto map = benchmark_create_unordered_map<istring>(keys);
    benchmark([&keys, &map]()
              {
                  benchmark_unordered_map(keys, map);
              });
}
