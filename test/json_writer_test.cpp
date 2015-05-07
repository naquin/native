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

#include "test.h"

#include "native/json/writer.h"

#include <sstream>

using namespace native;

TEST(json_writer_test, write_object)
{
    std::ostringstream ostr;
    json::writer<std::ostringstream> writer(ostr, 2);
    writer.open_object();
    writer.append("foo", 42);
    writer.append("bar", "string");
    writer.append("bool", true);
    writer.close_object();

    EXPECT_EQ(R"json({
  "foo": 42,
  "bar": "string",
  "bool": true
})json",
              ostr.str());
}

TEST(json_writer_test, write_array)
{
    std::ostringstream ostr;
    json::writer<std::ostringstream> writer(ostr, 2);
    writer.open_array();
    writer.append("foo");
    writer.append(42);
    writer.close_array();

    EXPECT_EQ(R"json([
  "foo",
  42
])json",
              ostr.str());
}

TEST(json_writer_test, write_complex_objects)
{
    std::ostringstream ostr;
    json::writer<std::ostringstream> writer(ostr, 2);
    writer.open_object();
    writer.append("foo", 42);
    writer.append("bar", "string");
    writer.key("array");
    writer.open_array();
    writer.append("foo");
    writer.append(42);
    writer.open_object();
    writer.append("foo", 42);
    writer.append("bar", "string");
    writer.close_object();
    writer.close_array();
    writer.close_object();

    EXPECT_EQ(R"json({
  "foo": 42,
  "bar": "string",
  "array": [
    "foo",
    42,
    {
      "foo": 42,
      "bar": "string"
    }
  ]
})json",
              ostr.str());
}

TEST(json_writer_test, write_utf_8)
{
    {
        std::ostringstream ostr;
        json::writer<std::ostringstream> writer(ostr, 2);
        writer.append("κόσμε");

        EXPECT_EQ(R"json("\u03ba\u03cc\u03c3\u03bc\u03b5")json", ostr.str());
    }

    {
        std::ostringstream ostr;
        json::writer<std::ostringstream> writer(ostr, 2);

        writer.open_object();
        writer.append("dollar", "\x24");
        writer.append("cents", "\xC2\xA2");
        writer.append("euro", "\xE2\x82\xAC");
        writer.append("G clef", "\xF0\x9D\x84\x9E");
        writer.close_object();

        EXPECT_EQ(R"json({
  "dollar": "$",
  "cents": "\u00a2",
  "euro": "\u20ac",
  "G clef": "\ud834\udd1e"
})json",
              ostr.str());
    }
}
