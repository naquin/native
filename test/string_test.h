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

#ifndef NATIVE_STRING_TEST_H__
#define NATIVE_STRING_TEST_H__

#include "test.h"
#include "native/nstring.h"

template <typename String>
void test_string_compare()
{
    String foo("foo");
    String fooCopy = foo;
    String fooOther("foo");
    String bar("bar");

    EXPECT_EQ(0, strcmp("foo", foo.data()));
    EXPECT_EQ("foo", foo);
    EXPECT_EQ(foo, "foo");
    EXPECT_EQ("foo", foo.std_str());
    EXPECT_EQ(foo.std_str(), "foo");
    EXPECT_EQ(foo, fooCopy);
    EXPECT_EQ(fooCopy, foo);
    EXPECT_EQ(foo, fooOther);
    EXPECT_EQ(fooOther, foo);

    EXPECT_NE(0, strcmp("bar", foo.data()));
    EXPECT_NE("bar", foo);
    EXPECT_NE(foo, "bar");
    EXPECT_NE("bar", foo.std_str());
    EXPECT_NE(foo.std_str(), "bar");
    EXPECT_NE(bar, foo);
    EXPECT_NE(foo, bar);

    EXPECT_LT("bar", foo);
    EXPECT_LT("bar", foo.std_str());
    EXPECT_LT(bar, foo);

    EXPECT_GT(foo, "bar");
    EXPECT_GT(foo.std_str(), "bar");
    EXPECT_GT(foo, bar);

    EXPECT_LE("bar", foo);
    EXPECT_LE("bar", foo.std_str());
    EXPECT_LE(bar, foo);
    EXPECT_LE("foo", foo);
    EXPECT_LE(foo, "foo");
    EXPECT_LE("foo", foo.std_str());
    EXPECT_LE(foo.std_str(), "foo");
    EXPECT_LE(foo, fooCopy);
    EXPECT_LE(fooCopy, foo);
    EXPECT_LE(foo, fooOther);
    EXPECT_LE(fooOther, foo);

    EXPECT_GE(foo, "bar");
    EXPECT_GE(foo.std_str(), "bar");
    EXPECT_GE(foo, bar);
    EXPECT_GE("foo", foo);
    EXPECT_GE(foo, "foo");
    EXPECT_GE("foo", foo.std_str());
    EXPECT_GE(foo.std_str(), "foo");
    EXPECT_GE(foo, fooCopy);
    EXPECT_GE(fooCopy, foo);
    EXPECT_GE(foo, fooOther);
    EXPECT_GE(fooOther, foo);
}

template <typename String>
void test_string_constructors()
{
    std::vector<char> vector_of_chars = {'f', 'o', 'o', 'b', 'a', 'r'};

    String ctor1;
    String ctor2(5, 'a');
    String ctor3a(ctor2, 1);
    String ctor3b(ctor2.std_str(), 1);
//    String ctor3c(string(ctor2), 1);
    String ctor4a("foobar", 3);
    String ctor4b("foobar", 3);
    String ctor5("foobar");
    String ctor6(vector_of_chars.begin(), vector_of_chars.end());
    String ctor7a(ctor5);
    String ctor7b(ctor5.std_str());
//    String ctor7c((string(ctor5)));
    String ctor8Original("foobar");
    String ctor8(std::move(ctor8Original));
    String ctor9({'f', 'o', 'o', 'b', 'a', 'r'});

    EXPECT_EQ("", ctor1);
    EXPECT_EQ("aaaaa", ctor2);
    EXPECT_EQ("aaaa", ctor3a);
    EXPECT_EQ("aaaa", ctor3b);
//    EXPECT_EQ("aaaa", ctor3c);
    EXPECT_EQ("foo", ctor4a);
    EXPECT_EQ("foo", ctor4b);
    EXPECT_EQ("foobar", ctor5);
    EXPECT_EQ("foobar", ctor6);
    EXPECT_EQ(ctor5, ctor7a);
    EXPECT_EQ(ctor5, ctor7b);
//    EXPECT_EQ(ctor5, ctor7c);
    EXPECT_EQ("foobar", ctor8Original);
    EXPECT_EQ("foobar", ctor8);
    EXPECT_EQ("foobar", ctor9);
}

template <typename String>
void test_string_assignment_operators()
{
    String s;
    EXPECT_EQ("", s);

    String expected("foobar");
    s = expected;
    EXPECT_EQ(expected, s);

    std::string std_expected("tigers");
    s = std_expected;
    EXPECT_EQ(std_expected, s);

    s = "literal";
    EXPECT_EQ("literal", s);
}

template <typename String>
void test_string_iterators()
{
    String s("foobar");
    
    auto it = s.begin();
    EXPECT_EQ('f', *it); ++it;
    EXPECT_EQ('o', *it); ++it;
    EXPECT_EQ('o', *it); ++it;
    EXPECT_EQ('b', *it); ++it;
    EXPECT_EQ('a', *it); ++it;
    EXPECT_EQ('r', *it); ++it;
    EXPECT_EQ(it, s.end());
    --it; --it;
    EXPECT_EQ('a', *it);

    it = s.cbegin();
    EXPECT_EQ('f', *it); ++it;
    EXPECT_EQ('o', *it); ++it;
    EXPECT_EQ('o', *it); ++it;
    EXPECT_EQ('b', *it); ++it;
    EXPECT_EQ('a', *it); ++it;
    EXPECT_EQ('r', *it); ++it;
    EXPECT_EQ(it, s.end());
    --it; --it;
    EXPECT_EQ('a', *it);

    auto reverseIt = s.rbegin();
    EXPECT_EQ('r', *reverseIt); ++reverseIt;
    EXPECT_EQ('a', *reverseIt); ++reverseIt;
    EXPECT_EQ('b', *reverseIt); ++reverseIt;
    EXPECT_EQ('o', *reverseIt); ++reverseIt;
    EXPECT_EQ('o', *reverseIt); ++reverseIt;
    EXPECT_EQ('f', *reverseIt); ++reverseIt;
    EXPECT_EQ(reverseIt, s.rend());
    --reverseIt;
    --reverseIt;
    --reverseIt;
    --reverseIt;
    EXPECT_EQ('b', *reverseIt);

    reverseIt = s.crbegin();
    EXPECT_EQ('r', *reverseIt); ++reverseIt;
    EXPECT_EQ('a', *reverseIt); ++reverseIt;
    EXPECT_EQ('b', *reverseIt); ++reverseIt;
    EXPECT_EQ('o', *reverseIt); ++reverseIt;
    EXPECT_EQ('o', *reverseIt); ++reverseIt;
    EXPECT_EQ('f', *reverseIt); ++reverseIt;
    EXPECT_EQ(reverseIt, s.rend());
    --reverseIt;
    --reverseIt;
    --reverseIt;
    --reverseIt;
    EXPECT_EQ('b', *reverseIt);
}

template <typename String>
void test_string_attributes()
{
    String s("foobar");
    EXPECT_EQ(6, s.size());
    EXPECT_EQ(6, s.length());
    EXPECT_EQ(6, s.max_size());
    EXPECT_EQ(6, s.capacity());
    EXPECT_EQ('b', s[3]);
    EXPECT_EQ('b', s.at(3));
    EXPECT_ANY_THROW(s.at(7));
    EXPECT_EQ('f', s.front());
    EXPECT_EQ('r', s.back());
    EXPECT_EQ("oob", s.substr(1, 3));
    EXPECT_EQ("foobar", s.std_str());
    EXPECT_EQ(0, strcmp("foobar", s.data()));

    EXPECT_FALSE(s.empty());

    s = "";

    EXPECT_TRUE(s.empty());
    
    s = "foobar";
    
    char copy[7];
    EXPECT_EQ(6, s.copy(copy));
    EXPECT_EQ(0, strcmp("foobar", copy));
}

template <typename String>
void test_string_find()
{
    String s("foobar");
    String search("oob");

    // 1
    EXPECT_EQ(1, s.find(search));

    // 2
    EXPECT_EQ(s.npos, s.find("oo", 2));

    // 3
    EXPECT_EQ(0, s.find("f"));
    EXPECT_EQ(1, s.find("o"));
    EXPECT_EQ(1, s.find("oo"));
    EXPECT_EQ(3, s.find("bar"));

    // 4
    EXPECT_EQ(4, s.find('a'));
    EXPECT_EQ(s.npos, s.find('a', 6));
}

template <typename String>
void test_string_rfind()
{
    String s("foobar");
    String search("oob");

    // 1
    EXPECT_EQ(1, s.rfind(search));

    // 2
    EXPECT_EQ(1, s.rfind("oo", 2));

    // 3
    EXPECT_EQ(0, s.rfind("f"));
    EXPECT_EQ(2, s.rfind("o"));
    EXPECT_EQ(1, s.rfind("oo"));
    EXPECT_EQ(3, s.rfind("bar"));

    // 4
    EXPECT_EQ(4, s.rfind('a'));
    EXPECT_EQ(4, s.rfind('a', 6));
}

template <typename String>
void test_string_find_first_of()
{
    String s("foobar");
    String search("oob");

    // 1
    EXPECT_EQ(1, s.find_first_of(search));

    // 2
    EXPECT_EQ(2, s.find_first_of("oo", 2));

    // 3
    EXPECT_EQ(0, s.find_first_of("f"));
    EXPECT_EQ(1, s.find_first_of("o"));
    EXPECT_EQ(1, s.find_first_of("oo"));
    EXPECT_EQ(3, s.find_first_of("rab"));

    // 4
    EXPECT_EQ(4, s.find_first_of('a'));
    EXPECT_EQ(s.npos, s.find_first_of('a', 6));
}

template <typename String>
void test_string_find_last_of()
{
    String s("foobar");
    String search("oob");

    // 1
    EXPECT_EQ(3, s.find_last_of(search));

    // 2
    EXPECT_EQ(2, s.find_last_of("oo", 2));

    // 3
    EXPECT_EQ(0, s.find_last_of("f"));
    EXPECT_EQ(2, s.find_last_of("o"));
    EXPECT_EQ(2, s.find_last_of("oo"));
    EXPECT_EQ(5, s.find_last_of("rab"));

    // 4
    EXPECT_EQ(4, s.find_last_of('a'));
    EXPECT_EQ(4, s.find_last_of('a', 6));
}

template <typename String>
void test_string_find_first_not_of()
{
    String s("foobar");
    String search("oob");

    // 1
    EXPECT_EQ(0, s.find_first_not_of(search));

    // 2
    EXPECT_EQ(3, s.find_first_not_of("oo", 2));

    // 3
    EXPECT_EQ(1, s.find_first_not_of("f"));
    EXPECT_EQ(0, s.find_first_not_of("o"));
    EXPECT_EQ(0, s.find_first_not_of("oo"));
    EXPECT_EQ(0, s.find_first_not_of("rab"));

    // 4
    EXPECT_EQ(0, s.find_first_not_of('a'));
    EXPECT_EQ(s.npos, s.find_first_not_of('a', 6));
}

template <typename String>
void test_string_find_last_not_of()
{
    String s("foobar");
    String search("oob");

    // 1
    EXPECT_EQ(5, s.find_last_not_of(search));

    // 2
    EXPECT_EQ(0, s.find_last_not_of("oo", 2));

    // 3
    EXPECT_EQ(5, s.find_last_not_of("f"));
    EXPECT_EQ(5, s.find_last_not_of("o"));
    EXPECT_EQ(5, s.find_last_not_of("oo"));
    EXPECT_EQ(2, s.find_last_not_of("rab"));

    // 4
    EXPECT_EQ(5, s.find_last_not_of('a'));
    EXPECT_EQ(5, s.find_last_not_of('a', 6));
}

template <typename String>
void test_string_split()
{
    String s("one two three four");
    
    auto split = s.split(' ');
    EXPECT_EQ(4, split.size());
    EXPECT_EQ("one",   split[0]);
    EXPECT_EQ("two",   split[1]);
    EXPECT_EQ("three", split[2]);
    EXPECT_EQ("four",  split[3]);

    split = s.split("ee");
    EXPECT_EQ(2, split.size());
    EXPECT_EQ("one two thr",   split[0]);
    EXPECT_EQ(" four",  split[1]);

    split = s.split("");
    EXPECT_EQ(1, split.size());
    EXPECT_EQ(s,   split[0]);

    split = s.split(String("two"));
    EXPECT_EQ(2, split.size());
    EXPECT_EQ("one ",   split[0]);
    EXPECT_EQ(" three four",  split[1]);

}

template <typename String>
void test_string_ostream()
{
    String s("foobar");

    std::ostringstream ostr;
    ostr << s;

    EXPECT_EQ("foobar", ostr.str());
}

template <typename String>
void benchmark_string_copy(String s)
{
    String s2 = s;
    String s3 = s2;
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
    String sub1 = s.substr(5, 30);
    String sub2 = s.substr(15, 30);
    String sub3 = s.substr(0, 30);
}

#endif
