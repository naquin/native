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
#include "native/string_builder.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <sstream>
#include <unordered_map>

using namespace native;
using std::to_string;

template <typename StringBuilder>
void test_append()
{
    StringBuilder sb;
    EXPECT_EQ("", sb.slice());
    
    sb << "foo";
    EXPECT_EQ("foo", sb.slice());

    sb << "bar";
    EXPECT_EQ("foobar", sb.slice());

    sb.clear();
    sb << "abcdefghijklmnopqrstuvwxyz";
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", sb.slice());
    
    sb << "0123456789";
    sb << "0123456789";
    sb << "0123456789";
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz012345678901234567890123456789", sb.slice());

    sb.clear();
    sb << "abcdefghijklmnopqrstuvwxyz";
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", sb.slice());
    
    sb.clear();
    sb << 4294967296u;
    EXPECT_EQ("4294967296", sb.slice());

    sb.clear();
    sb << 18446744073709551615u;
    EXPECT_EQ("18446744073709551615", sb.slice());

    sb.clear();
    sb << -32767;
    EXPECT_EQ("-32767", sb.slice());
    
    sb.clear();
    sb << -2147483647;
    EXPECT_EQ("-2147483647", sb.slice());
    
    sb.clear();
    sb << -9223372036854775807;
    EXPECT_EQ("-9223372036854775807", sb.slice());

    sb.clear();
    sb << 1.2345f;
    EXPECT_EQ("1.234500", sb.slice());

    sb.clear();
    sb << 123456.789;
    EXPECT_EQ("123456.789000", sb.slice());

    sb.clear();
    sb << 'f' << 'o' << 'o' << 'b' << 'a' << 'r';
    EXPECT_EQ("foobar", sb.slice());

    istring move_it(std::move(sb));
    EXPECT_EQ("foobar", move_it);

    auto syntax = (string_builder() << "The" << " quick" << " brown" << " fox" << " jumps" << " over" << " the" << " lazy" << " dog").str();
    EXPECT_EQ("The quick brown fox jumps over the lazy dog", syntax);
}

TEST(StringBuilder, Append)
{
    test_append<string_builder>();
}

TEST(StringBuilder, str)
{
    string_builder sb;
    sb << "foo";
    sb << "bar";
    
    auto s = sb.str();
    EXPECT_EQ("foobar", s);
    EXPECT_EQ("", sb.slice());
}
