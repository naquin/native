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
#include "native/string_conversion.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <sstream>
#include <unordered_map>

using namespace native;
using std::to_string;

TEST(String, Compare)
{
    test_string_compare<istring>();
}

TEST(String, Constructors)
{
    test_string_constructors<istring>();

    istring ctor2(5, 'a');
    istring ctor3c(string_slice(ctor2), 1);
    istring ctor7c((string_slice(ctor2)));

    EXPECT_EQ("aaaaa", ctor2);
    EXPECT_EQ("aaaa", ctor3c);
    EXPECT_EQ(ctor2, ctor7c);
}

TEST(String, AssignmentOperators)
{
    test_string_assignment_operators<istring>();

    string_slice slice_expected("lions");
    istring s = slice_expected;
    EXPECT_EQ(slice_expected, s);
}

TEST(String, Iterators)
{
    test_string_iterators<istring>();
}

TEST(String, Literal)
{
    auto literal = istring::literal("literal");
    istring literalCopy = literal;

    EXPECT_EQ("literal", literal);
    EXPECT_EQ("literal", literalCopy);
    EXPECT_EQ(literal, literalCopy);
    EXPECT_EQ(literalCopy, literal);
}

TEST(String, Attributes)
{
    test_string_attributes<istring>();
    istring s("foobar");
    EXPECT_EQ(0, strcmp("foobar", s.c_str()));
}

TEST(String, Slice)
{
    istring s("foobar");

    auto slice = s(1,2);
    EXPECT_EQ("oo", slice);

    slice = s(3,6);
    EXPECT_EQ("bar", slice);
}

TEST(String, hash)
{
    istring short_s("foobar");
    EXPECT_EQ("foobar"_hash, short_s.hash());

    istring dynamic_s("this string needs to be longer than 23 characters");
    EXPECT_EQ("this string needs to be longer than 23 characters"_hash, dynamic_s.hash());
    
    istring static_s = istring::literal("foobar");
    EXPECT_EQ("foobar"_hash, short_s.hash());
}

TEST(String, find)
{
    test_string_find<istring>();
}

TEST(String, rfind)
{
    test_string_rfind<istring>();
}

TEST(String, find_first_of)
{
    test_string_find_first_of<istring>();
}

TEST(String, find_last_of)
{
    test_string_find_last_of<istring>();
}

TEST(String, find_first_not_of)
{
    test_string_find_first_not_of<istring>();
}

TEST(String, find_last_not_of)
{
    test_string_find_last_not_of<istring>();
}

TEST(String, split)
{
    test_string_split<istring>();
}

TEST(String, OStream)
{
    test_string_ostream<istring>();
}

TEST(String, StaticCoversion)
{
    {
        constexpr auto actual = to_string_literal<unsigned, 1>::value;
        static_assert(sizeof(to_string_literal<unsigned, 1>::value) == 2, "");
        std::string expected = "1";
        EXPECT_EQ(actual, expected);
    }

    {
        constexpr auto actual = to_string_literal<unsigned, 12>::value;
        static_assert(sizeof(to_string_literal<unsigned, 12>::value) == 3, "");
        std::string expected = "12";
        EXPECT_EQ(actual, expected);
    }

    {
        constexpr auto actual = to_string_literal<unsigned, 123>::value;
        static_assert(sizeof(to_string_literal<unsigned, 123>::value) == 4, "");
        std::string expected = "123";
        EXPECT_EQ(actual, expected);
    }

    {
        constexpr auto actual = to_string_literal<int, -1>::value;
        static_assert(sizeof(to_string_literal<int, -1>::value) == 3, "");
        std::string expected = "-1";
        EXPECT_EQ(actual, expected);
    }

    {
        constexpr auto actual = to_string_literal<int, -12>::value;
        static_assert(sizeof(to_string_literal<int, -12>::value) == 4, "");
        std::string expected = "-12";
        EXPECT_EQ(actual, expected);
    }

    {
        constexpr auto actual = to_string_literal<int, -123>::value;
        static_assert(sizeof(to_string_literal<int, -123>::value) == 5, "");
        std::string expected = "-123";
        EXPECT_EQ(actual, expected);
    }
}

