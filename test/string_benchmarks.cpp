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

#define STRING_100 \
    STRING_10 STRING_10 STRING_10 STRING_10 STRING_10 \
    STRING_10 STRING_10 STRING_10 STRING_10 STRING_10

#define STRING_1000 \
    STRING_100 STRING_100 STRING_100 STRING_100 STRING_100 \
    STRING_100 STRING_100 STRING_100 STRING_100 STRING_100

#define STRING_10000 \
    STRING_1000 STRING_1000 STRING_1000 STRING_1000 STRING_1000 \
    STRING_1000 STRING_1000 STRING_1000 STRING_1000 STRING_1000

static const char TEST_STRING_10[]    = STRING_10;
static const char TEST_STRING_100[]   = STRING_100;
static const char TEST_STRING_1000[]  = STRING_1000;
static const char TEST_STRING_10000[] = STRING_10000;

static const size_t NUM_ASSIGNS = 10;
static const size_t MAX_HASH_LENGTH = 10000000;

BENCHMARK(BenchmarkTest, assign_std_string)
{
    for (std::string s: {TEST_STRING_10, TEST_STRING_100, TEST_STRING_1000})
    {
        for (std::size_t i = 0; i < NUM_ASSIGNS; ++i)
        {
            auto benchmark_name = "assign_std_string," +
                to_string(s.size()) + "," +
                to_string(i+1);
            benchmark(benchmark_name, [&s,i]()
            {
                benchmark_string_copy(s, i+1);
            });
        }
    }
}

BENCHMARK(BenchmarkTest, assign_istring)
{
    for (istring s: {TEST_STRING_10, TEST_STRING_100, TEST_STRING_1000})
    {
        for (std::size_t i = 0; i < NUM_ASSIGNS; ++i)
        {
            auto benchmark_name = "assign_istring," +
                to_string(s.size()) + "," +
                to_string(i+1);
            benchmark(benchmark_name, [&s,i]()
            {
                benchmark_string_copy(s, i+1);
            });
        }
    }
}

BENCHMARK(BenchmarkTest, assign_istring_literal)
{
    auto do_benchmark = [&](const istring& s)
    {
        for (std::size_t i = 0; i < NUM_ASSIGNS; ++i)
        {
            auto benchmark_name = "assign_istring_literal," +
                to_string(s.size()) + "," +
                to_string(i+1);
            benchmark(benchmark_name, [&s,i]()
            {
                benchmark_string_copy(s, i+1);
            });
        }
    };

    do_benchmark(istring::literal(TEST_STRING_10));
    do_benchmark(istring::literal(TEST_STRING_100));
    do_benchmark(istring::literal(TEST_STRING_1000));
}

BENCHMARK(BenchmarkTest, hash_std_string)
{
    auto do_benchmark = [&](size_t size)
    {
        auto benchmark_name = "hash_std_string," +
            to_string(size);

        std::string key(size, '7');
        benchmark(benchmark_name, [&key](){

            size_t hash = std::hash<std::string>()(key);
            if (hash == 0)
            {
                std::cout << "fail" << std::endl;
            }
        });
    };

    for (size_t i = 10; i < MAX_HASH_LENGTH; i *= 10)
    {
        do_benchmark(i);
    }
}

BENCHMARK(BenchmarkTest, hash_istring)
{
    auto do_benchmark = [&](size_t size)
    {
        auto benchmark_name = "hash_istring," + to_string(size);

        istring key(size, '7');
        benchmark(benchmark_name, [&key](){
            size_t hash = std::hash<istring>()(key);
            if (hash == 0)
            {
                std::cout << "fail" << std::endl;
            }
        });
    };

    for (size_t i = 10; i < MAX_HASH_LENGTH; i *= 10)
    {
        do_benchmark(i);
    }
}

BENCHMARK(BenchmarkTest, index_std_string)
{
    std::string s = TEST_STRING_100;
	benchmark([&s]()
    {
        benchmark_string_index(s);
	});
}

BENCHMARK(BenchmarkTest, index_native_string)
{
    istring s = TEST_STRING_100;
	benchmark([&s]()
    {
        benchmark_string_index(s);
	});
}

BENCHMARK(BenchmarkTest, split_std_string)
{
    std::string s = TEST_STRING_100;
    std::vector<std::string> result;
	benchmark([&s, &result]()
    {
        boost::algorithm::split(result, s, boost::algorithm::is_any_of("5"));
	});
}

BENCHMARK(BenchmarkTest, split_native_string)
{
    istring s = TEST_STRING_100;
	benchmark([&s]()
    {
        auto result = string_slice(s).split('5');
	});
}

BENCHMARK(BenchmarkTest, substr_std_string)
{
    std::string s = TEST_STRING_100;
	benchmark([&s]()
    {
        benchmark_string_substr(s);
	});
}

BENCHMARK(BenchmarkTest, substr_native_string)
{
    istring s = TEST_STRING_100;
	benchmark([&s]()
    {
        benchmark_string_substr(s);
	});
}
