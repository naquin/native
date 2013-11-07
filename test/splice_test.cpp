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

#include "native/istring.h"

#include <sstream>

using namespace native;

TEST(SpliceString, Compare)
{
    test_string_compare<string_splice>();
}

TEST(SpliceString, Constructors)
{
    test_string_constructors<string_splice>();

    istring ctor2(5, 'a');
    istring ctor3c(istring(ctor2), 1);
    istring ctor7c((istring(ctor2)));

    EXPECT_EQ("aaaaa", ctor2);
    EXPECT_EQ("aaaa", ctor3c);
    EXPECT_EQ(ctor2, ctor7c);
}

TEST(SpliceString, AssignmentOperators)
{
    test_string_assignment_operators<string_splice>();

    string_splice splice_expected("lions");
    istring s = splice_expected;
    EXPECT_EQ(splice_expected, s);
}

TEST(SpliceString, Iterators)
{
    test_string_iterators<string_splice>();
}

TEST(SpliceString, Attributes)
{
    test_string_attributes<string_splice>();
    istring s("foobar");
    EXPECT_EQ(0, strcmp("foobar", s.c_str()));
}

TEST(SpliceString, find)
{
    test_string_find<string_splice>();
}

TEST(SpliceString, rfind)
{
    test_string_rfind<string_splice>();
}

TEST(SpliceString, find_first_of)
{
    test_string_find_first_of<string_splice>();
}

TEST(SpliceString, find_last_of)
{
    test_string_find_last_of<string_splice>();
}

TEST(SpliceString, find_first_not_of)
{
    test_string_find_first_not_of<string_splice>();
}

TEST(SpliceString, find_last_not_of)
{
    test_string_find_last_not_of<string_splice>();
}

TEST(SpliceString, split)
{
    test_string_split<string_splice>();
}

TEST(SpliceString, OStream)
{
    test_string_ostream<string_splice>();
}
