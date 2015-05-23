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

#include "native/json/any.h"

#include <boost/timer/timer.hpp>

#include <fstream>
#include <sstream>

#if defined(JSON11)
#include "json11.hpp"
#endif

class json_any_benchmark : public benchmark_test
{
public:
    json_any_benchmark(const char* fixture, const char* name)
        : benchmark_test{fixture, name, 10}
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
};

BENCHMARK(json_any_benchmark, parse_string)
{
    auto func = [&]()
    {
        native::json::parse(_text);
    };
    benchmark("parse_string", func);
}

BENCHMARK(json_any_benchmark, parse_istringstream)
{
    auto func = [&]()
    {
        std::istringstream istr(_text);
        native::json::parse_stream(istr);
    };

    benchmark("parse_stream", func);
}

BENCHMARK(json_any_benchmark, parse_ifstream)
{
    auto func = [&]()
    {
        std::ifstream istr(_filename);
        native::json::parse_stream(istr);
    };
    
    benchmark("parse_stream", func);
}

#if defined(JSON11)
BENCHMARK(json_any_benchmark, json11_parse_string)
{
    auto func = [&]()
    {
        json11::Json json;
        std::string error;
        json.parse(_text, error);
        EXPECT_EQ("", error);
    };
    benchmark("parse_string", func);
}

#endif