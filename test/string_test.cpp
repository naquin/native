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
#include "string_test.h"

#include "benchmark.h"

#include "native/istring.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <sstream>

using namespace native;

TEST(String, Compare)
{
    test_string_compare<string>();
}

TEST(String, Constructors)
{
    test_string_constructors<string>();

    string ctor2(5, 'a');
    string ctor3c(string_splice(ctor2), 1);
    string ctor7c((string_splice(ctor2)));

    EXPECT_EQ("aaaaa", ctor2);
    EXPECT_EQ("aaaa", ctor3c);
    EXPECT_EQ(ctor2, ctor7c);
}

TEST(String, AssignmentOperators)
{
    test_string_assignment_operators<string>();

    string_splice splice_expected("lions");
    string s = splice_expected;
    EXPECT_EQ(splice_expected, s);
}

TEST(String, Iterators)
{
    test_string_iterators<string>();
}

TEST(String, Literal)
{
    auto literal = string::literal("literal");
    string literalCopy = literal;

    EXPECT_EQ("literal", literal);
    EXPECT_EQ("literal", literalCopy);
    EXPECT_EQ(literal, literalCopy);
    EXPECT_EQ(literalCopy, literal);
}

TEST(String, Attributes)
{
    test_string_attributes<string>();
    string s("foobar");
    EXPECT_EQ(0, strcmp("foobar", s.c_str()));
}

TEST(String, find)
{
    test_string_find<string>();
}

TEST(String, rfind)
{
    test_string_rfind<string>();
}

TEST(String, find_first_of)
{
    test_string_find_first_of<string>();
}

TEST(String, find_last_of)
{
    test_string_find_last_of<string>();
}

TEST(String, find_first_not_of)
{
    test_string_find_first_not_of<string>();
}

TEST(String, find_last_not_of)
{
    test_string_find_last_not_of<string>();
}

TEST(String, split)
{
    test_string_split<string>();
}

TEST(String, OStream)
{
    test_string_ostream<string>();
}

static const char TEST_STRING[] = "012345678901234567890123456789012345678901234567890123456789";

BENCHMARK(BenchmarkTest, assign_std_string)
{
    std::string s = TEST_STRING;
	benchmark([&s]()
    {
        benchmark_string_copy(s);
	});
}

BENCHMARK(BenchmarkTest, assign_native_string)
{
    string s = TEST_STRING;
	benchmark([&s]()
    {
        benchmark_string_copy(s);
	});
}

BENCHMARK(BenchmarkTest, assign_native_string_literal)
{
    string s = string::literal(TEST_STRING);
	benchmark([&s]()
    {
        benchmark_string_copy(s);
    });
}

BENCHMARK(BenchmarkTest, index_std_string)
{
    std::string s = TEST_STRING;
	benchmark([&s]()
    {
        benchmark_string_index(s);
	});
}

BENCHMARK(BenchmarkTest, index_native_string)
{
    string s = TEST_STRING;
	benchmark([&s]()
    {
        benchmark_string_index(s);
	});
}

BENCHMARK(BenchmarkTest, split_std_string)
{
    std::string s = TEST_STRING;
    std::vector<std::string> result;
	benchmark([&s, &result]()
    {
        boost::algorithm::split(result, s, boost::algorithm::is_any_of("5"));
	});
}

BENCHMARK(BenchmarkTest, split_native_string)
{
    string s = TEST_STRING;
	benchmark([&s]()
    {
        auto result = s.split('5');
	});
}

BENCHMARK(BenchmarkTest, substr_std_string)
{
    std::string s = TEST_STRING;
	benchmark([&s]()
    {
        benchmark_string_substr(s);
	});
}

BENCHMARK(BenchmarkTest, substr_native_string)
{
    string s = TEST_STRING;
	benchmark([&s]()
    {
        benchmark_string_substr(s);
	});
}
