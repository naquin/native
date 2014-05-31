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

#ifndef NATIVE_JSON_HANDLER_H__
#define NATIVE_JSON_HANDLER_H__

#include "native/config.h"

#include "native/json/types.h"


namespace native {
namespace json {

template <typename Ch = char>
class handler {
public:
    typedef Ch char_type;

    handler() { }

    // Return expected data type or type_unknown if not known
    data_type start_array() { return type_unknown; }
    void end_array() { }

    void start_object() { }
    void end_object() { }

    // This method is called when a key is parsed.
    //
    // The character pointer to key will remain valid until the next key is parsed.
    // This means that it is safe to store for data types, but not object types!
    data_type key(const char_type* key, std::size_t length)
    {
        return type_unknown; // return the expected type
    }

    void value(const char_type* val, std::size_t length) { }
    void value(std::nullptr_t)         { }
    void value(bool val)               { }
    void value(short val)              { }
    void value(unsigned short val)     { }
    void value(int val)                { }
    void value(unsigned val)           { }
    void value(long val)               { }
    void value(unsigned long val)      { }
    void value(long long val)          { }
    void value(unsigned long long val) { }
    void value(float val)              { }
    void value(double val)             { }
    void value(long double val)        { }
};

} } // namespace native::json

#endif
