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

// basic_parser takes an input source and parses the JSON data into the given
// handler.
//
// See native/json/exceptions.h for specific exception types that are thrown.
template <typename SourceEncoding, typename TargetEncoding>
class basic_parser
{
public:
    using source_encoding_type = SourceEncoding;
    using target_encoding_type = TargetEncoding;
    using char_type = typename source_encoding_type::char_type;

    // Parses JSON source as a string with the given handler.
    //
    // Throws json_exception on error,
    template <typename Handler, typename String>
    void parse(const String& source, Handler& handler)
    {
        using iterator_type = typename String::const_iterator;
        using stream_type = iterator_stream<iterator_type>;
        stream_type stream(source.cbegin(), source.cend());
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse_whole();
    }

    // Parses JSON source as a const char* with the given handler.
    //
    // Throws json_exception on error,
    template <typename Handler>
    void parse(const char_type* source, std::size_t length, Handler& handler)
    {
        using iterator_type = const char_type*;
        using stream_type = iterator_stream<iterator_type>;

        stream_type stream(source, source + length);
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse_whole();
    }

    // Parses JSON from an iterator range with the given handler.
    //
    // Throws json_exception on error,
    template <typename Handler, typename Iterator>
    void parse(Iterator first, Iterator last, Handler& handler)
    {
        using iterator_type = Iterator;
        using stream_type = iterator_stream<iterator_type>;
        stream_type stream(first, last);
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse();
    }

    // Parses JSON from an input stream with the given handler.
    //
    // Throws json_exception on error,
    template <typename IStream, typename Handler>
    void parse_stream(IStream& istr, Handler& handler)
    {
        using stream_type = istream_stream<IStream>;
        stream_type stream(istr);
        detail::parser_impl<stream_type, Handler, source_encoding_type,
                            target_encoding_type> parser(std::move(stream),
                                                         handler);
        parser.parse();
    }
};

using parser = basic_parser<utf8, utf8>;

} // namespace json
} // namespace native

#endif
