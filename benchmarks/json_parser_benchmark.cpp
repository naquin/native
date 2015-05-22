//
// Copyright (c) 2015 Mike Naquin. All rights reserved.
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

#include "benchmark.h"

#include "native/json/parser.h"

#include <boost/timer/timer.hpp>

#if defined(RAPID_JSON)
#include "rapidjson/reader.h"
#endif

#include <fstream>

class json_parser_benchmark : public benchmark_test
{
public:
    json_parser_benchmark(const char* fixture, const char* name)
        : benchmark_test{fixture, name, 1000}
    {
    }

    void SetUp() override
    {
        std::ifstream istr(_filename);

        istr.seekg(0, std::ios::seek_dir::end);
        auto size = istr.tellg();

        _text.resize(size);

        istr.seekg(0, std::ios::seek_dir::beg);

        istr.read(&_text[0], size);
    }

    std::string _filename = "sample.json";
    std::string _text;
    std::string _whitespace;
};

BENCHMARK(json_parser_benchmark, parse_string_empty_handler)
{
    native::json::parser parser;
    native::json::handler<> handler;
    auto func = [&]()
    {
        parser.parse(_text, handler);
    };
    benchmark("parse_string_empty_handler", func);
}

BENCHMARK(json_parser_benchmark, parse_iterator_empty_handler)
{
    native::json::parser parser;
    native::json::handler<> handler;
    auto func = [&]()
    {
        parser.parse(_text.cbegin(), _text.cend(), handler);
    };
    benchmark("parse_iterator_empty_handler", func);
}

BENCHMARK(json_parser_benchmark, parse_raw_string_empty_handler)
{
    native::json::parser parser;
    native::json::handler<> handler;
    auto func = [&]()
    {
        parser.parse(_text.c_str(), _text.size(), handler);
    };
    benchmark("parse_raw_string_empty_handler", func);
}

BENCHMARK(json_parser_benchmark, parse_stream_empty_handler)
{
    native::json::parser parser;
    native::json::handler<> handler;
    auto func = [&]()
    {
        std::istringstream istr(_text);
        parser.parse_stream(istr, handler);
    };

    benchmark("parse_stream_empty_handler", func);
}

#if defined(RAPID_JSON)
BENCHMARK(json_parser_benchmark, rapidjson_parse_string_empty_handler)
{
    rapidjson::BaseReaderHandler<> h;
    rapidjson::Reader reader;
    auto func = [&]()
    {
        rapidjson::StringStream s(_text.c_str());
        reader.Parse<0>(s, h);
    };
    benchmark("rapid_json_empty_handler", func);
}
#endif
