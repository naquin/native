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

#ifndef native_json_parser_test_h__
#define native_json_parser_test_h__

#include "test.h"

#include "native/json/parser.h"

#include <sstream>

template <typename T>
struct numeric_handler:
    native::json::handler<>
{
    typedef T value_type;
    
    numeric_handler(): actual() {}
    void value(T val)
    {
        actual = val;
    }
    
    T actual;
};

template <typename Ch = char>
struct string_handler:
    native::json::handler<Ch>
{
    typedef Ch char_type;
    
    void value(const char_type* val, std::size_t length)
    {
        actual.assign(val, length);
    }
    
    std::basic_string<char_type> actual;
};


template <typename T>
T parse_integer(const std::string& number)
{
    typedef numeric_handler<T> Handler;
    Handler handler;
    std::string str = number;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.template parse_integer<T>();
    return handler.actual;
};


bool parse_bool(const std::string& number, bool expected)
{
    typedef numeric_handler<bool> Handler;
    Handler handler;
    std::string str = number;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    if ( expected )
    {
        parser.parse_true();
    }
    else
    {
        parser.parse_false();
    }
    return handler.actual;
}

void parse_null(const std::string& number)
{
    typedef numeric_handler<bool> Handler;
    Handler handler;
    std::string str = number;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.parse_null();
//    return handler.actual;
}

template <typename T>
T parse_real(const std::string& number)
{
    typedef numeric_handler<T> Handler;
    Handler handler;
    std::string str = number;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.template parse_real<typename Handler::value_type>();
    return handler.actual;
};


template <typename T>
void test_integer_limits()
{
    const auto max = std::numeric_limits<T>::max();
    const auto min = std::numeric_limits<T>::min();
    // put dummy bracket at the end to protect against eof
    EXPECT_EQ(max, parse_integer<T>(std::to_string(max)));
    EXPECT_EQ(min, parse_integer<T>(std::to_string(min)));
}


template <typename Ch>
std::basic_string<Ch> parse_string(const std::basic_string<Ch>& str)
{
    typedef typename native::encoding<Ch>::type Encoding;
    typedef string_handler<Ch> Handler;
    Handler handler;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.parse_string();
    return handler.actual;
}


template <typename Ch, std::size_t Size>
std::basic_string<Ch> parse_string(const Ch (&str)[Size])
{
    return parse_string(std::basic_string<Ch>(str, Size-1));
}


template <typename Ch>
void parse_any(const std::basic_string<Ch>& str)
{
    typedef typename native::encoding<Ch>::type Encoding;
    typedef native::json::handler<Ch> Handler;
    Handler handler;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.parse();
}


template <typename Ch, std::size_t Size>
void parse_any(const Ch (&str)[Size])
{
    return parse_any(std::basic_string<Ch>(str, Size-1));
}


template <typename T, typename Encoding = native::utf8>
struct numeric_array_handler:
    native::json::handler<typename Encoding::char_type>
{
    typedef native::json::handler<typename Encoding::char_type> base_type;
    typedef Encoding encoding_type;
    typedef typename encoding_type::char_type char_type;
    
    numeric_array_handler():
    start_count(), end_count(), actual()
    { }
    
    native::json::data_type start_array()
    {
        ++start_count;
        return native::json::type_mapper<T>::value;
    }
    
    native::json::data_type key(const char_type* key, std::size_t length)
    {
        return native::json::type_mapper<T>::value;
    }
    
    void end_array()
    {
        ++end_count;
    }
    
    using base_type::value;
    
    void value(T val)
    {
        actual.push_back(val);
    }
    
    std::size_t start_count, end_count;
    std::vector<T> actual;
};


template <typename T, std::size_t Size, typename Ch>
std::vector<T> parse_array(const Ch (&raw_str)[Size])
{
    typedef typename native::encoding<Ch>::type Encoding;
    typedef numeric_array_handler<T, Encoding> Handler;
    std::string str(raw_str, Size-1);
    Handler handler;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.parse();
    return std::move(handler.actual);
}


template <typename Ch>
struct object_handler:
    native::json::handler<Ch>
{
    typedef Ch char_type;
    typedef native::json::handler<char_type> base_type;
    typedef std::basic_string<char_type> string_type;
    
    object_handler():
        start_count(), end_count(), step()
    { }
    
    void start_object()
    {
        ++start_count;
    }

    native::json::data_type start_array()
    {
        return native::json::type_int;
    }

    native::json::data_type key(const char_type* key, std::size_t length)
    {
        key_actual.push_back(key);
        switch ( key[0] )
        {
        case 'a':
        case 'i':
            return native::json::type_int;
        default:
            return native::json::type_unknown;
        }
    }

    void end_object()
    {
        ++end_count;
    }
    
    using base_type::value;
    
    void value(std::nullptr_t)
    {
        ++step;
        nulls.emplace_back(step);
    }
    
    void value(bool val)
    {
        ++step;
        bools.emplace_back(step, val);
    }

    void value(short val)
    {
        ++step;
        shorts.emplace_back(step, val);
    }

    void value(unsigned short val)
    {
        ++step;
        ushorts.emplace_back(step, val);
    }

    void value(int val)
    {
        ++step;
        ints.emplace_back(step, val);
    }

    void value(long val)
    {
        ++step;
        longs.emplace_back(step, val);
    }

    void value(long long val)
    {
        ++step;
        long_longs.emplace_back(step, val);
    }

    void value(unsigned val)
    {
        ++step;
        uints.emplace_back(step, val);
    }

    void value(unsigned long val)
    {
        ++step;
        ulongs.emplace_back(step, val);
    }

    void value(unsigned long long val)
    {
        ++step;
        ulong_longs.emplace_back(step, val);
    }

    void value(float val)
    {
        ++step;
        floats.emplace_back(step, val);
    }

    void value(double val)
    {
        ++step;
        doubles.emplace_back(step, val);
    }

    void value(long double val)
    {
        ++step;
        long_doubles.emplace_back(step, val);
    }

    void value(const char_type* val, std::size_t length)
    {
        ++step;
        strings.emplace_back(step, string_type(val, length));
    }
    
    std::size_t start_count, end_count, step;
    std::vector<string_type>                                key_actual;
    std::vector<std::size_t>                                nulls;
    std::vector<std::pair<std::size_t, bool>>               bools;
    std::vector<std::pair<std::size_t, short>>              shorts;
    std::vector<std::pair<std::size_t, unsigned short>>     ushorts;
    std::vector<std::pair<std::size_t, int>>                ints;
    std::vector<std::pair<std::size_t, unsigned>>           uints;
    std::vector<std::pair<std::size_t, long>>               longs;
    std::vector<std::pair<std::size_t, unsigned long>>      ulongs;
    std::vector<std::pair<std::size_t, long long>>          long_longs;
    std::vector<std::pair<std::size_t, unsigned long long>> ulong_longs;
    std::vector<std::pair<std::size_t, float>>              floats;
    std::vector<std::pair<std::size_t, double>>             doubles;
    std::vector<std::pair<std::size_t, long double>>        long_doubles;
    std::vector<std::pair<std::size_t, string_type>>        strings;
};


template <typename Ch>
object_handler<Ch> parse_object(const std::basic_string<Ch>& str)
{
    typedef object_handler<Ch> Handler;
    Handler handler;
    auto parser = native::json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.parse();
    return std::move(handler);
}

template <typename Ch>
object_handler<Ch> parse_object(const Ch* str)
{
    return parse_object(std::basic_string<Ch>(str));
}

#endif
