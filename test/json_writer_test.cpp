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

TEST(JsonWriterTest, ObjectTypes_Should_Work) {
    std::ostringstream ostr;
    json::writer<std::ostringstream> writer(ostr);
    writer.open_object();
    writer.append("foo", 42);
    writer.append("bar", "string");
    writer.append("bool", true);
    writer.close_object();

    EXPECT_EQ("{\n  \"foo\": 42,\n  \"bar\": \"string\",\n  \"bool\": true\n}" , ostr.str());
}

TEST(JsonWriterTest, ArrayTypes_Should_Work) {
    std::ostringstream ostr;
    json::writer<std::ostringstream> writer(ostr);
    writer.open_array();
    writer.append("foo");
    writer.append(42);
    writer.close_array();
    
    EXPECT_EQ("[\n  \"foo\",\n  42\n]" , ostr.str());
}

TEST(JsonWriterTest, ComplexTypes_Should_Work) {
    std::ostringstream ostr;
    json::writer<std::ostringstream> writer(ostr);
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
    
    EXPECT_EQ("{\n  \"foo\": 42,\n  \"bar\": \"string\",\n  \"array\": \n  [\n    \"foo\",\n    42,\n    {\n      \"foo\": 42,\n      \"bar\": \"string\"\n    }\n  ]\n}" , ostr.str());
}
