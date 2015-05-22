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

#ifndef NATIVE_JSON_DETAIL_STREAM_TYPE_H__
#define NATIVE_JSON_DETAIL_STREAM_TYPE_H__

#include "native/config.h"

#include "native/istring.h"

namespace native
{
namespace detail
{

template <typename T>
struct stream_type;

template <>
struct stream_type<istring>
{
    using type = string_builder;
};

template <>
struct stream_type<std::string>
{
    using type = std::ostringstream;
};

} // namespace detail
} // namespace native

#endif
