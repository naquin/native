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

#include "json_parser_test.h"

using namespace native;

TEST(json_parser_test, parse_bool)
{
    std::string str("true");
    numeric_handler<bool> handler;
    auto parser =
        json::detail::make_shared_parser_impl(str.begin(), str.end(), handler);
    parser->parse_true();
    EXPECT_EQ(true, handler.actual);

    str = "false";
    parser =
        json::detail::make_shared_parser_impl(str.begin(), str.end(), handler);
    parser->parse_false();
    EXPECT_EQ(false, handler.actual);
}

TEST(json_parser_test, integers_should_parse_limits)
{
    test_integer_limits<unsigned>();
    test_integer_limits<int>();
    test_integer_limits<unsigned long>();
    test_integer_limits<long>();
    test_integer_limits<long long>();
    test_integer_limits<unsigned long long>();
}

TEST(json_parser_test, integers_should_throw_outside_of_limits)
{
    EXPECT_ANY_THROW(parse_integer<std::uint32_t>("4294967297")); // 2 ^ 32 + 1
    EXPECT_ANY_THROW(parse_integer<std::uint32_t>("-1"));

    EXPECT_ANY_THROW(parse_integer<std::int32_t>("2147483648"));  // 2 ^ 31 + 1
    EXPECT_ANY_THROW(parse_integer<std::int32_t>("-2147483649")); // -2 ^ 31 + 1

    EXPECT_ANY_THROW(
        parse_integer<std::uint64_t>("18446744073709551616")); // 2 ^ 64 + 1
    EXPECT_ANY_THROW(parse_integer<std::uint64_t>("-1"));

    EXPECT_ANY_THROW(
        parse_integer<std::int64_t>("9223372036854775809")); // 2 ^ 63 + 1
    EXPECT_ANY_THROW(
        parse_integer<std::int64_t>("-9223372036854775810")); // -2 ^ 63 + 1
}

TEST(json_parser_test, integers_should_parse)
{
    EXPECT_EQ(0u, parse_integer<unsigned>("0"));
    EXPECT_EQ(42u, parse_integer<unsigned>("42"));

    EXPECT_ANY_THROW(parse_integer<unsigned>("-1"));

    EXPECT_EQ(0, parse_integer<int>("0"));
    EXPECT_EQ(-1, parse_integer<int>("-1"));
    EXPECT_EQ(-42, parse_integer<int>("-42"));
    EXPECT_EQ(-4200, parse_integer<int>("-4200"));
    EXPECT_EQ(-420000, parse_integer<int>("-420000"));

    using Handler = numeric_handler<long long>;
    Handler handler;
    std::string str = "3808159498143417627]";
    auto parser =
        json::detail::make_parser_impl(str.cbegin(), str.cend(), handler);
    parser.parse_number();
    EXPECT_EQ(3808159498143417627, handler.actual);

    EXPECT_EQ(-5514085325291784739,
              parse_integer<long long>("-5514085325291784739"));
    EXPECT_EQ(3808159498143417627,
              parse_integer<long long>("3808159498143417627"));
}

TEST(json_parser_test, parse_real_handler)
{
    EXPECT_EQ(0.0, parse_real<double>("0.0"));
    EXPECT_EQ(1.0, parse_real<double>("1.0"));
    EXPECT_EQ(-1.0, parse_real<double>("-1.0"));
    EXPECT_EQ(1.5, parse_real<double>("1.5"));
    EXPECT_EQ(-1.5, parse_real<double>("-1.5"));
    EXPECT_EQ(3.1416, parse_real<double>("3.1416"));
    EXPECT_EQ(1E10, parse_real<double>("1E10"));
    EXPECT_EQ(1e10, parse_real<double>("1e10"));
    EXPECT_EQ(1E+10, parse_real<double>("1E+10"));
    EXPECT_EQ(1E-10, parse_real<double>("1E-10"));
    EXPECT_EQ(-1E10, parse_real<double>("-1E10"));
    EXPECT_EQ(-1e10, parse_real<double>("-1e10"));
    EXPECT_EQ(-1E+10, parse_real<double>("-1E+10"));
    EXPECT_EQ(-1E-10, parse_real<double>("-1E-10"));
    EXPECT_EQ(1.234E+10, parse_real<double>("1.234E+10"));
    EXPECT_EQ(1.234E-10, parse_real<double>("1.234E-10"));
    EXPECT_EQ(1.79769e+308, parse_real<double>("1.79769e+308"));
    EXPECT_EQ(2.22507e-308, parse_real<double>("2.22507e-308")); // underflow
    EXPECT_EQ(-1.79769e+308, parse_real<double>("-1.79769e+308"));
    EXPECT_EQ(-2.22507e-308, parse_real<double>("-2.22507e-308")); // underflow
    EXPECT_EQ(18446744073709551616.0,
              parse_real<double>("18446744073709551616")); // 2^64 (max of
                                                           // uint64_t + 1,
                                                           // force to use
                                                           // double)
    EXPECT_EQ(-9223372036854775809.0,
              parse_real<double>("-9223372036854775809")); // -2^63 - 1(min of
                                                           // int64_t + 1, force
                                                           // to use double)

    {
        std::string n1e308(309, '0'); // '1' followed by 308 '0'
        n1e308[0] = '1';
        n1e308[309] = '\0';
        EXPECT_EQ(1e308, parse_real<double>(n1e308));
    }

    //    {
    //        std::string n1e309(310, '0');   // '1' followed by 309 '0'
    //        n1e309[0] = '1';
    //        EXPECT_ANY_THROW(parse_real<double>(n1e309));
    //    }
}

TEST(json_parser_test, numbers_should_check_for_bad_input)
{
    EXPECT_ANY_THROW(
        parse_integer<int>("a")); // JSON doesn't support hex or letters

    EXPECT_ANY_THROW(parse_integer<int>(".1")); // JSON needs a leading digit

    EXPECT_ANY_THROW(
        parse_integer<int>("1.")); // JSON needs a digit after decimal

    EXPECT_ANY_THROW(
        parse_integer<int>("1e")); // JSON needs a digit in exponent
}

TEST(json_parser_test, strings_should_parse)
{
    EXPECT_EQ("", parse_string("\"\""));
    EXPECT_EQ("Hello", parse_string("\"Hello\""));
    EXPECT_EQ("Hello\nWorld", parse_string("\"Hello\\nWorld\""));
    EXPECT_EQ("\"\\/\b\f\n\r\t", parse_string("\"\\\"\\\\/\\b\\f\\n\\r\\t\""));
    EXPECT_EQ("\x24", parse_string("\"\\u0024\""));     // Dollar sign U+0024
    EXPECT_EQ("\xC2\xA2", parse_string("\"\\u00A2\"")); // Cents sign U+00A2
    EXPECT_EQ("\xE2\x82\xAC", parse_string("\"\\u20AC\"")); // Euro sign U+20AC
    EXPECT_EQ("\xF0\x9D\x84\x9E",
              parse_string("\"\\uD834\\uDD1E\"")); // G clef sign U+1D11E

    // UTF16
    EXPECT_EQ(u"", parse_string(u"\"\""));
    EXPECT_EQ(u"Hello", parse_string(u"\"Hello\""));
    EXPECT_EQ(u"Hello\nWorld", parse_string(u"\"Hello\\nWorld\""));
    EXPECT_EQ(u"\"\\/\b\f\n\r\t",
              parse_string(u"\"\\\"\\\\/\\b\\f\\n\\r\\t\""));
    EXPECT_EQ(u"\u0024", parse_string(u"\"\\u0024\""));
    EXPECT_EQ(u"\u00A2", parse_string(u"\"\\u00A2\"")); // Cents sign U+00A2
    EXPECT_EQ(u"\u20AC", parse_string(u"\"\\u20AC\"")); // Euro sign U+20AC
    EXPECT_EQ(std::u16string({0xD834, 0xDD1E}),
              parse_string(u"\"\\uD834\\uDD1E\"")); // G clef sign U+1D11E

    // UTF32
    EXPECT_EQ(U"", parse_string(U"\"\""));
    EXPECT_EQ(U"Hello", parse_string(U"\"Hello\""));
    EXPECT_EQ(U"Hello\nWorld", parse_string(U"\"Hello\\nWorld\""));
    EXPECT_EQ(U"\"\\/\b\f\n\r\t",
              parse_string(U"\"\\\"\\\\/\\b\\f\\n\\r\\t\""));
    EXPECT_EQ(U"\u0024", parse_string(U"\"\\u0024\""));
    EXPECT_EQ(U"\u00A2", parse_string(U"\"\\u00A2\"")); // Cents sign U+00A2
    EXPECT_EQ(U"\u20AC", parse_string(U"\"\\u20AC\"")); // Euro sign U+20AC
    EXPECT_EQ(U"\U0001D11E",
              parse_string(U"\"\\uD834\\uDD1E\"")); // G clef sign U+1D11E
}

TEST(json_parser_test, parse_string_transcoding)
{
    const std::string x = "\"Hello\"";
    const std::u16string expected = u"Hello";
    using Handler = string_handler<char16_t>;
    Handler handler;

    std::istringstream istr(x);
    using stream_type = json::istream_stream<std::istringstream>;
    auto stream = stream_type(istr);

    json::detail::parser_impl<stream_type, Handler, utf8, utf16> parser(
        std::move(stream), handler);
    parser.parse_string();

    EXPECT_EQ(expected, handler.actual);
    EXPECT_EQ(expected.size(), handler.actual.size());
}

TEST(json_parser_test, parser_should_parse_around_encoding_errors)
{
    EXPECT_THROW(parse_any("[\"\\a\"]"), json::unknown_escape_character);

    EXPECT_THROW(parse_any("[\"\\uABCG\"]"), json::incorrect_hex_digit);

    EXPECT_THROW(parse_any("[\"\\uD800X\"]"),
                 json::missing_second_in_surrogate_pair);

    EXPECT_THROW(parse_any("[\"\\uD800\\uFFFF\"]"),
                 json::invalid_second_in_surrogate_pair);

    // lacks ending quotation before the end of string
    EXPECT_THROW(parse_any("[\"Test]"), json::missing_end_quote);

    // http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

    // 3  Malformed sequences

    // 3.1 Unexpected continuation bytes
    {
        std::string expected{'\"', 0, '\"'};
        for (unsigned char c = 0x80u; c <= 0xBFu; ++c)
        {
            expected[1] = c;
            const auto actual = parse_string(expected);
            EXPECT_EQ(c, static_cast<unsigned char>(actual[0]));
        }
    }

    // 3.2 Lonely start characters, 3.5 Impossible bytes
    {
        for (unsigned ch = 0xC0u; ch <= 0xFFu; ++ch)
        {
            const std::string expected = {'\"', (char)ch, ' ', '\"'};
            const auto actual = parse_string(expected);
            EXPECT_EQ(ch, static_cast<unsigned char>(actual[0]));
        }
    }

    // 4  Overlong sequences

    auto narrow = [](const std::initializer_list<unsigned short> value)
    {
        std::string result;
        for (const auto c : value)
        {
            result.push_back(c);
        }
        return result;
    };

    // 4.1  Examples of an overlong ASCII character
    EXPECT_EQ("\uC0AF", parse_string("\"\uC0AF\""));

    EXPECT_EQ(narrow({0xE0, 0x80, 0xAF}),
              parse_string<char>(narrow({'\"', 0xE0, 0x80, 0xAF, '\"'})));

    EXPECT_EQ(narrow({0xF0, 0x80, 0x80, 0xAF}),
              parse_string<char>(narrow({'\"', 0xF0, 0x80, 0x80, 0xAF, '\"'})));

    // 4.2  Maximum overlong sequences
    EXPECT_EQ(narrow({0xC1, 0xBF}),
              parse_string<char>(narrow({'\"', 0xC1, 0xBF, '\"'})));

    EXPECT_EQ(narrow({0xE0, 0x9F, 0xBF}),
              parse_string<char>(narrow({'\"', 0xE0, 0x9F, 0xBF, '\"'})));

    EXPECT_EQ(narrow({0xF0, 0x8F, 0xBF, 0xBF}),
              parse_string<char>(narrow({'\"', 0xF0, 0x8F, 0xBF, 0xBF, '\"'})));

    // 4.3  Overlong representation of the NUL character
    EXPECT_EQ(narrow({0xC0, 0x80}),
              parse_string<char>(narrow({'\"', 0xC0, 0x80, '\"'})));
    EXPECT_EQ(narrow({0xE0, 0x80, 0x80}),
              parse_string<char>(narrow({'\"', 0xE0, 0x80, 0x80, '\"'})));
    EXPECT_EQ(narrow({0xF0, 0x80, 0x80, 0x80}),
              parse_string<char>(narrow({'\"', 0xF0, 0x80, 0x80, 0x80, '\"'})));

    // 5  Illegal code positions

    // 5.1 Single UTF-16 surrogates
    EXPECT_EQ(narrow({0xED, 0xA0, 0x80}),
              parse_string<char>(narrow({'\"', 0xED, 0xA0, 0x80, '\"'})));

    EXPECT_EQ(narrow({0xED, 0xAD, 0xBF}),
              parse_string<char>(narrow({'\"', 0xED, 0xAD, 0xBF, '\"'})));

    EXPECT_EQ(narrow({0xED, 0xAE, 0x80}),
              parse_string<char>(narrow({'\"', 0xED, 0xAE, 0x80, '\"'})));

    EXPECT_EQ(narrow({0xED, 0xAF, 0xBF}),
              parse_string<char>(narrow({'\"', 0xED, 0xAF, 0xBF, '\"'})));

    EXPECT_EQ(narrow({0xED, 0xB0, 0x80}),
              parse_string<char>(narrow({'\"', 0xED, 0xB0, 0x80, '\"'})));

    EXPECT_EQ(narrow({0xED, 0xBE, 0x80}),
              parse_string<char>(narrow({'\"', 0xED, 0xBE, 0x80, '\"'})));

    EXPECT_EQ(narrow({0xED, 0xBF, 0xBF}),
              parse_string<char>(narrow({'\"', 0xED, 0xBF, 0xBF, '\"'})));
}

TEST(json_parser_test, parser_should_parse_empty_array)
{
    const std::string str("[ ] ");

    using Handler = numeric_array_handler<int, utf8>;
    Handler handler;
    auto parser =
        json::detail::make_parser_impl(str.begin(), str.end(), handler);
    parser.parse();

    EXPECT_EQ(1u, handler.start_count);
    EXPECT_EQ(1u, handler.end_count);
}

TEST(json_parser_test, parser_should_parse_numeric_array)
{
    EXPECT_EQ(std::vector<bool>({true, false, false, true}),
              parse_array<bool>("[true, false, false, true]"));

    EXPECT_EQ(std::vector<int>({1, 2, 3, 4}), parse_array<int>("[1, 2, 3, 4]"));
    EXPECT_EQ(std::vector<unsigned>({1, 2, 3, 4}),
              parse_array<unsigned>("[1, 2, 3, 4]"));

    EXPECT_EQ(std::vector<short>({1, 2, 3, 4}),
              parse_array<short>("[1, 2, 3, 4]"));
    EXPECT_EQ(std::vector<unsigned short>({1, 2, 3, 4}),
              parse_array<unsigned short>("[1, 2, 3, 4]"));

    EXPECT_EQ(std::vector<long>({1, 2, 3, 4}),
              parse_array<long>("[1, 2, 3, 4]"));
    EXPECT_EQ(std::vector<unsigned long>({1, 2, 3, 4}),
              parse_array<unsigned long>("[1, 2, 3, 4]"));

    EXPECT_EQ(std::vector<long long>({1, 2, 3, 4}),
              parse_array<long long>("[1, 2, 3, 4]"));
    EXPECT_EQ(std::vector<unsigned long long>({1, 2, 3, 4}),
              parse_array<unsigned long long>("[1, 2, 3, 4]"));

    EXPECT_EQ(std::vector<float>({1, 2, 3, 4}),
              parse_array<float>("[1, 2, 3, 4]"));
    EXPECT_EQ(std::vector<double>({1, 2, 3, 4}),
              parse_array<double>("[1, 2, 3, 4]"));
    EXPECT_EQ(std::vector<long double>({1, 2, 3, 4}),
              parse_array<long double>("[1, 2, 3, 4]"));
}

TEST(json_parser_test, parser_should_detect_malformed_arrays)
{
    // Must be a comma or ']' after an array element.
    EXPECT_THROW(parse_array<int>("["), std::range_error);
    EXPECT_THROW(parse_array<int>("[}"), std::range_error);
    EXPECT_THROW(parse_array<int>("[1 2]"),
                 json::expected_comma_or_close_bracket);
}

TEST(json_parser_test, parser_should_parse_object)
{
    const std::string str = "{ "
                            "\"string\" : \"hello\", "
                            "\"true\" : true , "
                            "\"false\" : false, "
                            "\"null\": null, "
                            "\"int\":123, "
                            "\"double\": 3.1416, "
                            "\"array\":[1, 2, 3] "
                            "} ";

    const auto handler = parse_object(str);

    EXPECT_EQ(1, handler.start_count);
    EXPECT_EQ(1, handler.end_count);

    EXPECT_EQ(1, handler.nulls.size());
    EXPECT_EQ(4, handler.nulls[0]);

    EXPECT_EQ(2, handler.bools.size());
    EXPECT_EQ(std::make_pair(std::size_t(2), true), handler.bools[0]);
    EXPECT_EQ(std::make_pair(std::size_t(3), false), handler.bools[1]);

    EXPECT_EQ(4, handler.ints.size());
    EXPECT_EQ(std::make_pair(std::size_t(5), 123), handler.ints[0]);
    EXPECT_EQ(std::make_pair(std::size_t(7), 1), handler.ints[1]);
    EXPECT_EQ(std::make_pair(std::size_t(8), 2), handler.ints[2]);
    EXPECT_EQ(std::make_pair(std::size_t(9), 3), handler.ints[3]);

    EXPECT_EQ(1, handler.doubles.size());
    EXPECT_EQ(std::make_pair(std::size_t(6), 3.1416), handler.doubles[0]);

    EXPECT_EQ(1, handler.strings.size());
    EXPECT_EQ(std::make_pair(std::size_t(1), std::string("hello")),
              handler.strings[0]);

    EXPECT_EQ(7, handler.key_actual.size());
    EXPECT_EQ("string", handler.key_actual[0]);
    EXPECT_EQ("true", handler.key_actual[1]);
    EXPECT_EQ("false", handler.key_actual[2]);
    EXPECT_EQ("null", handler.key_actual[3]);
    EXPECT_EQ("int", handler.key_actual[4]);
    EXPECT_EQ("double", handler.key_actual[5]);
    EXPECT_EQ("array", handler.key_actual[6]);
}

TEST(json_parser_test, parse_empty_object)
{
    const std::string str("{ } ");
    const auto handler = parse_object(str);

    EXPECT_EQ(1, handler.start_count);
    EXPECT_EQ(1, handler.end_count);

    EXPECT_TRUE(handler.nulls.empty());
    EXPECT_TRUE(handler.bools.empty());
    EXPECT_TRUE(handler.shorts.empty());
    EXPECT_TRUE(handler.ushorts.empty());
    EXPECT_TRUE(handler.ints.empty());
    EXPECT_TRUE(handler.uints.empty());
    EXPECT_TRUE(handler.longs.empty());
    EXPECT_TRUE(handler.ulongs.empty());
    EXPECT_TRUE(handler.long_longs.empty());
    EXPECT_TRUE(handler.ulong_longs.empty());
    EXPECT_TRUE(handler.floats.empty());
    EXPECT_TRUE(handler.doubles.empty());
    EXPECT_TRUE(handler.long_doubles.empty());
}

TEST(json_parser_test, parser_shold_detect_object_errors)
{
    // Name of an object member must be a string
    EXPECT_THROW(parse_object("{null:1}"), json::missing_start_quote);
    EXPECT_THROW(parse_object("{true:1}"), json::missing_start_quote);
    EXPECT_THROW(parse_object("{false:1}"), json::missing_start_quote);
    EXPECT_THROW(parse_object("{1:1}"), json::missing_start_quote);
    EXPECT_THROW(parse_object("{[]:1}"), json::missing_start_quote);
    EXPECT_THROW(parse_object("{{}:1}"), json::missing_start_quote);
    EXPECT_THROW(parse_object("{xyz:1}"), json::missing_start_quote);

    // There must be a colon after the name of object member
    EXPECT_THROW(parse_object("{\"a\" 1}"), json::expected_colon_after_key);
    EXPECT_THROW(parse_object("{\"a\",1}"), json::expected_colon_after_key);

    // Must be a comma or '}' after an object member
    EXPECT_THROW(parse_object("{]"), json::missing_start_quote);
    EXPECT_THROW(parse_object("{\"a\":1]"),
                 json::expected_comma_or_close_curly_brace);
}

TEST(json_parser_test, parser_should_dectect_format_errors)
{
    // Text only contains white space(s)
    EXPECT_THROW(parse_any(""), json::expected_object_or_array);
    EXPECT_THROW(parse_any(" "), json::expected_object_or_array);
    EXPECT_THROW(parse_any(" \n"), json::expected_object_or_array);

    // Expect either an object or array at root
    EXPECT_THROW(parse_any("null"), json::expected_object_or_array);
    EXPECT_THROW(parse_any("true"), json::expected_object_or_array);
    EXPECT_THROW(parse_any("false"), json::expected_object_or_array);
    EXPECT_THROW(parse_any("\"s\""), json::expected_object_or_array);
    EXPECT_THROW(parse_any("0"), json::expected_object_or_array);

    // Parser should stop at the end of an object or array
    EXPECT_NO_THROW(parse_any("[] 0"));
    EXPECT_NO_THROW(parse_any("{} 0"));

    // Invalid value
    EXPECT_THROW(parse_null("nu"), json::expected_null_value);
    EXPECT_THROW(parse_null("nulL"), json::expected_null_value);
    EXPECT_THROW(parse_bool("truE", true), json::expected_true_value);
    EXPECT_THROW(parse_bool("falsE", false), json::expected_false_value);
}
