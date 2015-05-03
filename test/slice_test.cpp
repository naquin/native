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

TEST(splice_string, compare) { test_string_compare<string_slice>(); }

TEST(splice_string, constructors)
{
    istring ctor2(5, 'a');

    string_slice ctor1;
    string_slice ctor3c(ctor2, 1);
    string_slice ctor3a(ctor3c, 1);
    string_slice ctor3b(ctor2.std_str(), 1);
    string_slice ctor4a("foobar", 3);
    string_slice ctor4b("foobar", 3);
    string_slice ctor5("foobar");
    string_slice ctor7a(ctor5);
    string_slice ctor7b(ctor5.std_str());
    //    String ctor7c((string(ctor5)));
    string_slice ctor8Original("foobar");
    string_slice ctor8(std::move(ctor8Original));
    string_slice ctor9({'f', 'o', 'o', 'b', 'a', 'r'});

    EXPECT_EQ("", ctor1);
    EXPECT_EQ("aaaaa", ctor2);
    EXPECT_EQ("aaaa", ctor3a);
    EXPECT_EQ("aaaa", ctor3b);
    EXPECT_EQ("aaaa", ctor3c);
    EXPECT_EQ("foo", ctor4a);
    EXPECT_EQ("foo", ctor4b);
    EXPECT_EQ("foobar", ctor5);
    EXPECT_EQ(ctor5, ctor7a);
    EXPECT_EQ(ctor5, ctor7b);
    //    EXPECT_EQ(ctor5, ctor7c);
    EXPECT_EQ("foobar", ctor8Original);
    EXPECT_EQ("foobar", ctor8);
    EXPECT_EQ("foobar", ctor9);

    istring ctor7c((istring(ctor2)));

    EXPECT_EQ("aaaa", ctor3c);
    EXPECT_EQ(ctor2, ctor7c);
}

TEST(splice_string, assignment_operators)
{
    test_string_assignment_operators<string_slice>();

    string_slice slice_expected("lions");
    istring s = slice_expected;
    EXPECT_EQ(slice_expected, s);
}

TEST(splice_string, iterators) { test_string_iterators<string_slice>(); }

TEST(splice_string, attributes)
{
    test_string_attributes<string_slice>();
    istring s("foobar");
    EXPECT_EQ(0, strcmp("foobar", s.c_str()));
}

TEST(splice_string, find) { test_string_find<string_slice>(); }

TEST(splice_string, rfind) { test_string_rfind<string_slice>(); }

TEST(splice_string, find_first_of)
{
    test_string_find_first_of<string_slice>();
}

TEST(splice_string, find_last_of) { test_string_find_last_of<string_slice>(); }

TEST(splice_string, find_first_not_of)
{
    test_string_find_first_not_of<string_slice>();
}

TEST(splice_string, find_last_not_of)
{
    test_string_find_last_not_of<string_slice>();
}

TEST(splice_string, split) { test_string_split<string_slice>(); }

TEST(splice_string, ostream) { test_string_ostream<string_slice>(); }
