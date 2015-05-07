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

#ifndef NATIVE_JSON_PARSER_IMPL_H__
#define NATIVE_JSON_PARSER_IMPL_H__

#include "native/config.h"

#include "native/json/exceptions.h"
#include "native/json/handler.h"
#include "native/json/input_streams.h"

#include "native/json/detail/real.h"
#include "native/json/detail/integers.h"

#include "native/utf.h"

#include "native/detail/container_ostream.h"

#include <cstdint>
#include <cmath>
#include <limits>

namespace native
{
namespace json
{
namespace detail
{

// clang-format off
static constexpr char escape[256] = {
    /* 0 */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* 1 */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* 2 */ 0, 0, '\"', 0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    '/',
    /* 3 */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* 4 */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* 5 */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, '\\', 0, 0,    0,
    /* 6 */ 0, 0, '\b', 0, 0,    0, '\f', 0, 0, 0, 0, 0, 0,    0, '\n', 0,
    /* 7 */ 0, 0, '\r', 0, '\t', 0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* 8 */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* 9 */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* a */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* b */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* c */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* d */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* e */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
    /* f */ 0, 0, 0,    0, 0,    0, 0,    0, 0, 0, 0, 0, 0,    0, 0,    0,
};
// clang-format on

template <typename T>
typename std::enable_if<sizeof(T) == 1, bool>::type
is_escape_character(const T ch)
{
    return escape[static_cast<unsigned char>(ch)];
}

template <typename T>
typename std::enable_if<sizeof(T) != 1, bool>::type
is_escape_character(const T ch)
{
    return ch < 256 && escape[static_cast<unsigned char>(ch)];
}

template <typename SourceEncoding, typename TargetEncoding>
struct codepoint_converter
{
    // Take one Unicode codepoint from source encoding, convert it to target
    // encoding and put it to the output stream.
    template <typename OStream, typename IStream>
    inline static void convert_next(IStream& source, OStream& destination)
    {
        TargetEncoding::encode(destination, SourceEncoding::decode(source));
    }
};

// no need to convert the same encoding
template <typename Encoding>
struct codepoint_converter<Encoding, Encoding>
{
    template <typename OStream, typename IStream>
    inline static void convert_next(IStream& source, OStream& destination)
    {
        destination.put(source.get());
    }
};

//
// parser originally adapted from RapidJSON
// http://code.google.com/p/rapidjson/
//

template <typename Stream, typename Handler,
          typename SourceEncoding =
              typename encoding<typename Handler::char_type>::type,
          typename TargetEncoding = SourceEncoding>
class parser_impl
{
public:
    typedef Stream stream_type;
    typedef Handler handler_type;
    typedef SourceEncoding source_encoding_type;
    typedef TargetEncoding target_encoding_type;

    typedef typename target_encoding_type::char_type char_type;
    typedef std::vector<char_type> buffer_type;

    parser_impl(stream_type&& stream, handler_type& handler)
        : stream(std::move(stream))
        , handler(handler)
        , expected_type(type_unknown)
    {
        key_buffer.reserve(1024);
        string_buffer.reserve(1024);
    }

    // parse the first object or array without checking for trailing characters
    void parse()
    {
        ignore_whitespace();
        switch (stream.peek())
        {
            case '{':
                parse_object();
                break;
            case '[':
                parse_array();
                break;
            default:
                throw expected_object_or_array(stream.line(), stream.column());
        }
    }

    // parse the whole stream and check for junk at the end
    void parse_whole()
    {
        parse();

        ignore_whitespace();
        if (!stream.eof())
        {
            throw expected_end_of_stream(stream.line(), stream.column());
        }
    }

    void ignore_whitespace()
    {
        for (;; stream.next())
        {
            switch (stream.peek())
            {
                case '\n':
                    stream.increment_line();
                case ' ':
                case '\r':
                case '\t':
                    break;
                default:
                    return;
            }
        }
    }

    void parse_object()
    {
        assert(stream.peek() == '{');
        stream.next();
        handler.start_object();

        ignore_whitespace();

        if (stream.peek() == '}') // check for empty object
        {
            stream.next();
            handler.end_object();
            return;
        }

        for (;;)
        {
            parse_key();
            parse_value();
            ignore_whitespace();

            switch (stream.get())
            {
                case ',':
                    ignore_whitespace();
                    break;
                case '}':
                    handler.end_object();
                    return;
                default:
                    throw expected_comma_or_close_curly_brace(stream.line(),
                                                              stream.column());
                    return;
            }
        }
    }

    void parse_array()
    {
        assert(stream.peek() == '[');
        stream.next();
        expected_type = handler.start_array();
        ignore_whitespace();
        if (stream.peek() == ']')
        {
            handler.end_array();
            stream.next();
            return;
        }

        for (;;)
        {
            parse_value();
            ignore_whitespace();

            switch (stream.get())
            {
                case ',':
                    ignore_whitespace();
                    break;
                case ']':
                    handler.end_array();
                    return;
                default:
                    throw expected_comma_or_close_bracket(stream.line(),
                                                          stream.column());
            }
        }
    }

    void parse_null()
    {
        stream.next();
        if (stream.get() == 'u' && stream.get() == 'l' && stream.get() == 'l')
        {
            handler.value(nullptr);
        }
        else
        {
            throw expected_null_value(stream.line(), stream.column());
        }
    }

    void parse_true()
    {
        stream.next();
        if (stream.get() == 'r' && stream.get() == 'u' && stream.get() == 'e')
        {
            handler.value(true);
        }
        else
        {
            throw expected_true_value(stream.line(), stream.column());
        }
    }

    void parse_false()
    {
        stream.next();
        if (stream.get() == 'a' && stream.get() == 'l' && stream.get() == 's' &&
            stream.get() == 'e')
        {
            handler.value(false);
        }
        else
        {
            throw expected_false_value(stream.line(), stream.column());
        }
    }

    std::uint32_t parse_hex4()
    {
        std::uint32_t codepoint = 0;
        for (std::uint32_t i = 0; i < 4; ++i)
        {
            auto ch = stream.get();

            codepoint <<= 4;
            codepoint += ch;

            switch (ch)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    codepoint -= '0';
                    break;
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                    codepoint -= 'A' - 10;
                    break;
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                    codepoint -= 'a' - 10;
                    break;
                default:
                    throw incorrect_hex_digit(stream.line(), stream.column());
            }
        }

        return codepoint;
    }

    void parse_string_impl(buffer_type& buffer)
    {
        buffer.clear();
        ::native::detail::container_ostream<buffer_type> buffer_stream{buffer};
        if (stream.peek() != '"')
        {
            throw missing_start_quote(stream.line(), stream.column());
        }
        stream.next(); // skip '"'

        for (;;)
        {
            const char_type ch = stream.peek();

            if (ch == '\\') // escape character
            {
                stream.next();

                const char_type escaped_ch = stream.get();
                if (is_escape_character(escaped_ch))
                {
                    buffer.push_back(
                        escape[static_cast<unsigned char>(escaped_ch)]);
                }
                else if (escaped_ch == 'u') // unicode
                {
                    std::uint32_t codepoint = parse_hex4();
                    if (codepoint >= 0xd800 && codepoint <= 0xdbff)
                    {
                        // handle utf-16 surrogate pair
                        if (stream.get() != '\\' || stream.get() != 'u')
                        {
                            throw missing_second_in_surrogate_pair(
                                stream.line(), stream.column());
                        }
                        std::uint32_t codepoint2 = parse_hex4();
                        if (codepoint2 < 0xdc00 || codepoint2 > 0xdfff)
                        {
                            throw invalid_second_in_surrogate_pair(
                                stream.line(), stream.column());
                        }
                        codepoint = (((codepoint - 0xd800) << 10) |
                                     (codepoint2 - 0xdc00)) +
                                    0x10000;
                    }
                    target_encoding_type::encode(buffer_stream, codepoint);
                }
                else
                {
                    throw unknown_escape_character(stream.line(),
                                                   stream.column());
                }
            }
            else if (ch == '"')
            {
                stream.next();
                buffer.push_back(0);
                return;
            }
            else if (stream.eof()) // reached the end
            {
                throw missing_end_quote(stream.line(), stream.column());
            }
            // RFC 4627: unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
            else if (static_cast<std::uint32_t>(ch) < 0x20)
            {
                throw incorrect_unescaped_character(stream.line(),
                                                    stream.column());
            }
            else
            {
                codepoint_converter<
                    source_encoding_type,
                    target_encoding_type>::convert_next(stream, buffer_stream);
            }
        }
    }

    void parse_string()
    {
        parse_string_impl(string_buffer);
        handler.value(&string_buffer[0], string_buffer.size() - 1);
    }

    void parse_key()
    {
        parse_string_impl(key_buffer);

        ignore_whitespace();
        if (stream.get() != ':') // check for colon after key
        {
            throw expected_colon_after_key(stream.line(), stream.column());
        }
        ignore_whitespace();

        expected_type = handler.key(&key_buffer[0], key_buffer.size() - 1);
    }

    void parse_number()
    {
        // first check to see if there is an expected type...
        switch (expected_type)
        {
            case type_short:
                parse_integer<short>();
                return;
            case type_unsigned_short:
                parse_integer<unsigned short>();
                return;
            case type_int:
                parse_integer<int>();
                return;
            case type_long:
                parse_integer<long>();
                return;
            case type_long_long:
                parse_integer<long long>();
                return;
            case type_unsigned:
                parse_integer<unsigned>();
                return;
            case type_unsigned_long:
                parse_integer<unsigned long>();
                return;
            case type_unsigned_long_long:
                parse_integer<unsigned long long>();
                return;
            case type_float:
                parse_real<float>();
                return;
            case type_double:
                parse_real<double>();
                return;
            case type_long_double:
                parse_real<long double>();
                return;
            case type_object:
            case type_array:
            case type_string:
            case type_bool:
                //            throw unexpected_type(stream.line(),
                //            stream.column());
                return; // unexpected type...
            case type_unknown:
                break;
        }

        detail::number_parse<long double> attribs;
        attribs.to_buffer(stream);

        // if type is not expected, convert based off of best guess
        if (attribs.is_real())
        {
            double number = string_to_real(attribs);
            handler.value(number);
        }
        else if (attribs.sign) // negative
        {
            switch (attribs.length)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9: // std::numeric_limits<std::int32_t>::digits10:
                {
                    const auto number =
                        detail::string_to_integer<std::int32_t>(attribs);
                    handler.value(number);
                    break;
                }
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18: // std::numeric_limits<std::int64_t>::digits10:
                case 19: // 2 to grow on
                case 20:
                {
                    const auto number =
                        detail::string_to_integer<std::int64_t>(attribs);
                    handler.value(number);
                    break;
                }
                default:
                {
                    throw number_too_big_for_expected_type(stream.line(),
                                                           stream.column());
                }
            }
        }
        else // non-negative unsigned
        {
            switch (attribs.length)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9: // std::numeric_limits<std::uint32_t>::digits10:
                {
                    const auto number =
                        detail::string_to_integer<std::uint32_t>(attribs);
                    handler.value(number);
                    break;
                }
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19: // std::numeric_limits<std::uint64_t>::digits10:
                case 20: // 1 to grow on
                {
                    const auto number =
                        detail::string_to_integer<std::uint64_t>(attribs);
                    handler.value(number);
                    break;
                }
                default:
                    throw number_too_big_for_expected_type(stream.line(),
                                                           stream.column());
            }
        }
    }

    template <typename T>
    void parse_real()
    {
        detail::number_parse<T> attribs;
        attribs.to_buffer(stream);
        T number = string_to_real(attribs);
        handler.value(number);
    }

    template <typename T>
    inline typename std::enable_if<std::numeric_limits<T>::is_signed,
                                   void>::type
    parse_integer()
    {
        detail::number_parse<T> attribs;
        attribs.to_buffer(stream);
        handler.value(detail::string_to_integer<T>(attribs));
    }

    template <typename T>
    inline typename std::enable_if<!std::numeric_limits<T>::is_signed,
                                   void>::type
    parse_integer()
    {
        detail::number_parse<T> attribs;
        attribs.to_buffer(stream);
        if (attribs.sign)
        {
            throw unexpected_signed_value(stream.line(), stream.column());
        }

        // if negative, then read as signed and convert...hope for the best?
        handler.value(detail::string_to_integer<T>(attribs));
    }

    void parse_value()
    {
        switch (stream.peek())
        {
            case 'n':
                parse_null();
                break;
            case 't':
                parse_true();
                break;
            case 'f':
                parse_false();
                break;
            case '"':
                parse_string();
                break;
            case '{':
                parse_object();
                break;
            case '[':
                parse_array();
                break;
            default:
                parse_number();
                break;
        }
    }

    stream_type stream;
    handler_type& handler;
    data_type expected_type;
    std::vector<char_type> key_buffer;
    std::vector<char_type> string_buffer;
};

template <typename Handler, typename Iterator>
parser_impl<iterator_stream<Iterator>, Handler,
            typename encoding<typename Handler::char_type>::type,
            typename encoding<typename Handler::char_type>::type>
make_parser_impl(Iterator first, Iterator last, Handler& handler)
{
    typedef Iterator iterator_type;
    typedef iterator_stream<iterator_type> stream_type;
    typedef typename encoding<typename Handler::char_type>::type
        source_encoding;
    typedef source_encoding target_encoding;
    typedef parser_impl<stream_type, Handler, source_encoding, target_encoding>
        impl_type;
    stream_type stream(first, last);
    return std::move(impl_type(std::move(stream), handler));
}

template <typename Handler, typename Stream>
parser_impl<Stream, Handler,
            typename encoding<typename Handler::char_type>::type,
            typename encoding<typename Handler::char_type>::type>
make_parser_impl(Stream&& stream, Handler& handler)
{
    typedef Stream stream_type;
    typedef typename encoding<typename Handler::char_type>::type
        source_encoding;
    typedef source_encoding target_encoding;
    typedef parser_impl<stream_type, Handler, source_encoding, target_encoding>
        impl_type;
    return std::move(impl_type(std::move(stream), handler));
}

template <typename Handler, typename Iterator>
std::shared_ptr<
    parser_impl<iterator_stream<Iterator>, Handler,
                typename encoding<typename Handler::char_type>::type,
                typename encoding<typename Handler::char_type>::type>>
make_shared_parser_impl(Iterator first, Iterator last, Handler& handler)
{
    typedef Iterator iterator_type;
    typedef iterator_stream<iterator_type> stream_type;
    typedef typename encoding<typename Handler::char_type>::type
        source_encoding;
    typedef source_encoding target_encoding;
    typedef parser_impl<stream_type, Handler, source_encoding, target_encoding>
        impl_type;
    stream_type stream(first, last);
    return std::make_shared<impl_type>(std::move(stream), handler);
}
} // namespace detail
} // namespace json
} // namespace native

#endif
