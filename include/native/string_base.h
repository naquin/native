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

#ifndef NATIVE_STRING_BASE_H__
#define NATIVE_STRING_BASE_H__

#include <iosfwd>
#include <memory>
#include <string>
#include <type_traits>

namespace native {

template <typename Ch>
struct basic_string_core;

template <typename Ch>
class basic_istring;

template <typename Ch>
class basic_string_splice;

class string_base;

template <class T>
struct is_string_class {
  enum {
      value = std::is_base_of<string_base, T>::value ||
              std::is_same<T, std::string>::value ||
              std::is_same<T, std::u16string>::value ||
              std::is_same<T, std::u32string>::value ||
              std::is_same<T, std::wstring>::value
    };
};

class string_base {
public:
    void throw_out_of_range() const;
};

inline void string_base::throw_out_of_range() const
{
    throw std::out_of_range("native::basic_istring");
}


} // namespace native

#endif
