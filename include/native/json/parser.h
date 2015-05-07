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

#ifndef NATIVE_JSON_PARSER_H__
#define NATIVE_JSON_PARSER_H__

#include "native/config.h"

#include "native/json/detail/parser_impl.h"

#include "native/utf.h"

namespace native
{
namespace json
{

template <typename SourceEncoding, typename TargetEncoding>
class basic_parser
{
public:
    typedef SourceEncoding source_encoding_type;
    typedef TargetEncoding target_encoding_type;
    typedef typename source_encoding_type::char_type char_type;

    template <typename Handler, typename String>
    void parse(const String& source, Handler& handler)
    {
        typedef typename String::const_iterator iterator_type;
        typedef iterator_stream<iterator_type> stream_type;
        stream_type stream(source.cbegin(), source.cend());
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse_whole();
    }

    template <typename Handler>
    void parse(const char_type* source, std::size_t length, Handler& handler)
    {
        typedef const char_type* iterator_type;
        typedef iterator_stream<iterator_type> stream_type;

        stream_type stream(source, source + length);
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse_whole();
    }

    template <typename Handler, typename Iterator>
    void parse(Iterator first, Iterator last, Handler& handler)
    {
        typedef Iterator iterator_type;
        typedef iterator_stream<iterator_type> stream_type;
        stream_type stream(first, last);
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse();
    }

    template <typename IStream, typename Handler>
    void parse_stream(IStream& istr, Handler& handler)
    {
        typedef istream_stream<IStream> stream_type;
        stream_type stream(istr);
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse();
    }
};

typedef basic_parser<utf8, utf8> parser;
typedef basic_parser<utf8, utf16> utf8_to_utf16_parser;
typedef basic_parser<utf16, utf8> utf16_to_utf8_parser;
}
} // namespace native::json

#endif
