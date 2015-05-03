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

#include "native/json.h"

using namespace native;

TEST(json_any_test, pod_types) {
    json::any null_value{};
    EXPECT_TRUE(null_value.is_null());
    EXPECT_TRUE(null_value.empty());
    EXPECT_EQ("", null_value.string_value());
    EXPECT_EQ(0.0, null_value.double_value());
    EXPECT_EQ(0, null_value.int_value());
    EXPECT_FALSE(null_value.bool_value());

    json::any bool_value{true};
    EXPECT_TRUE(bool_value.is_bool());
    EXPECT_FALSE(bool_value.empty());
    EXPECT_EQ("true", bool_value.string_value());
    EXPECT_EQ(1.0, bool_value.double_value());
    EXPECT_EQ(1, bool_value.int_value());
    EXPECT_TRUE(bool_value.bool_value());

    json::any int_value{-42};
    EXPECT_TRUE(int_value.is_int());
    EXPECT_FALSE(int_value.empty());
    EXPECT_EQ("-42", int_value.string_value());
    EXPECT_EQ(-42.0, int_value.double_value());
    EXPECT_EQ(-42, int_value.int_value());
    EXPECT_TRUE(int_value.bool_value());

    json::any double_value{-42.0};
    EXPECT_TRUE(double_value.is_double());
    EXPECT_FALSE(double_value.empty());
    EXPECT_EQ("-42.000000", double_value.string_value());
    EXPECT_EQ(-42.0, double_value.double_value());
    EXPECT_EQ(-42, double_value.int_value());
    EXPECT_TRUE(double_value.bool_value());

    json::any sting_value{"hello"};
    EXPECT_TRUE(sting_value.is_string());
    EXPECT_FALSE(sting_value.empty());
    EXPECT_EQ("hello", sting_value.string_value());
    EXPECT_THROW(sting_value.double_value(), std::range_error);
    EXPECT_THROW(sting_value.int_value(), std::range_error);
    EXPECT_FALSE(sting_value.bool_value());
}

TEST(json_any_test, array) {
    json::any value{{42, "hello", true}};
    EXPECT_TRUE(value.is_array());
    EXPECT_EQ(42, value[0].int_value());
    EXPECT_EQ("hello", value[1].string_value());
    EXPECT_EQ(true, value[2].bool_value());

    EXPECT_EQ(3, value.size());
    EXPECT_FALSE(value.empty());

    json::any copy = value;
    EXPECT_TRUE(copy.is_array());
    EXPECT_EQ(42, copy[0].int_value());
    EXPECT_EQ("hello", copy[1].string_value());
    EXPECT_EQ(true, copy[2].bool_value());

    json::any move = std::move(copy);
    EXPECT_TRUE(move.is_array());
    EXPECT_EQ(42, move[0].int_value());
    EXPECT_EQ("hello", move[1].string_value());
    EXPECT_EQ(true, move[2].bool_value());

    value.push_back("next");
    value.push_back(false);
    value.push_back(5.5);
    
    EXPECT_EQ("next", value[3].string_value());
    EXPECT_FALSE(value[4].bool_value());
    EXPECT_EQ(5.5, value[5].double_value());

    EXPECT_EQ(6, value.size());

    EXPECT_EQ(42, value.begin()->int_value());
    EXPECT_EQ(5.5, (value.end()-1)->double_value());
    
    value[4] = "blue";
    EXPECT_EQ("blue", value[4].string_value());
    
    value.pop_back();
    
    EXPECT_EQ(5, value.size());
    EXPECT_EQ(42, value.front().int_value());
    EXPECT_EQ("blue", value.back().string_value());
    
    value.emplace_back(std::move(move));
    
    EXPECT_TRUE(value.back().is_array());
    
    EXPECT_EQ(42, value.back().front().int_value());
    EXPECT_EQ("hello", value.back()[1].string_value());
    
    value.resize(10);
    
    EXPECT_EQ(10, value.size());
    
    EXPECT_TRUE(value.back().is_null());
    
    auto it = value.erase(value.begin()+1);
    EXPECT_EQ(value.begin()+1, it);
    
    it = value.erase(value.begin()+1, value.end());
    EXPECT_EQ(value.end(), it);
    EXPECT_EQ(1, value.size());
}

TEST(json_any_test, object) {
    json::any value{{{"foo", 42}, {"bar", "string"}}};
    EXPECT_TRUE(value.is_object());
    EXPECT_EQ(42, value["foo"].int_value());
    EXPECT_EQ("string", value["bar"].string_value());

    EXPECT_EQ(2, value.size());
    EXPECT_FALSE(value.empty());

    json::any copy = value;
    EXPECT_TRUE(copy.is_object());
    EXPECT_EQ(42, copy["foo"].int_value());
    EXPECT_EQ("string", copy["bar"].string_value());

    json::any move = std::move(copy);
    EXPECT_TRUE(move.is_object());
    EXPECT_EQ(42, move["foo"].int_value());
    EXPECT_EQ("string", move["bar"].string_value());

    value["three"] = 3;
    value["four"] = 4.0;

    EXPECT_EQ(4, value.size());

    EXPECT_EQ(3, value["three"].int_value());
    EXPECT_EQ(4.0, value["four"].double_value());

    EXPECT_EQ("string", move.find("bar")->second.string_value());
    
    EXPECT_EQ(1, value.erase("four"));
    EXPECT_EQ(3, value.size());
    
    auto it = value.find("three");
    it = value.erase(it);
    EXPECT_EQ(2, value.size());

    auto keys = value.keys();
    EXPECT_NE(keys.end(), std::find(keys.begin(), keys.end(), "foo"));
    EXPECT_NE(keys.end(), std::find(keys.begin(), keys.end(), "bar"));
    
    auto values = value.values();
    EXPECT_NE(values.end(), std::find(values.begin(), values.end(), "string"));
    EXPECT_NE(values.end(), std::find(values.begin(), values.end(), 42));
    
    value["array"] = json::any{{2, "c++", 7.0}};
    EXPECT_TRUE(value["array"].is_array());
    EXPECT_EQ(2, value["array"][0].int_value());
    EXPECT_EQ("c++", value["array"][1].string_value());
    EXPECT_EQ(7.0, value["array"][2].double_value());
    value["array"].push_back(native::json::any::object{{"problems", 99}, {"color", "orange"}});
    EXPECT_EQ(99, value["array"].back()["problems"].int_value());
    EXPECT_EQ("orange", value["array"].back()["color"].string_value());
}

TEST(json_any_test, dump) {
    json::any value{{{"foo", 42}, {"bar", "string"}, {"null", nullptr}}};
    auto json = value.dump(true, 2);
    EXPECT_EQ(
R"json({
  "bar": "string",
  "foo": 42,
  "null": null
})json", json);
    
    value["array"] = json::any{{2, "c++", 1.5}};
    value["array"].push_back(json::any::object{{"problems", 99}, {"color", "orange"}});
    json = value.dump(true, 2);
    EXPECT_EQ(R"json({
  "array": [
    2,
    "c++",
    1.5,
    {
      "color": "orange",
      "problems": 99
    }
  ],
  "bar": "string",
  "foo": 42,
  "null": null
})json", json);
}

TEST(json_any_test, parse) {
    auto json = istring::literal(R"json({
  "array": [
    2,
    "c++",
    1.5,
    {
      "color": "orange",
      "problems": 99
    }
  ],
  "bar": "string",
  "foo": 42
})json");

    auto value = json::parse(json);

    EXPECT_EQ(json, value.dump(true, 2));
}
