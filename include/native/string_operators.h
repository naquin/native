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

#ifndef NATIVE_STRING_OPERATORS_H__
#define NATIVE_STRING_OPERATORS_H__

#include "native/string_base.h"

namespace native {

//
// compare basic_istring to basic_istring
//
template <typename Ch>
bool operator==(const basic_istring<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_istring<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_istring<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_istring<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_istring<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_istring<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ge(lhs, rhs);
}

//
// compare basic_istring to basic_string_slice
//
template <typename Ch>
bool operator==(const basic_istring<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_istring<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_istring<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_istring<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_istring<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_istring<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ge(lhs, rhs);
}

//
// compare basic_istring to basic_string_builder
//
template <typename Ch>
bool operator==(const basic_istring<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_istring<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_istring<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_istring<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_istring<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_istring<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ge(lhs, rhs);
}

//
// compare basic_istring to std::basic_string
//
template <typename Ch>
bool operator==(const basic_istring<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_istring<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_istring<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_istring<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_istring<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_istring<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_slice to basic_istring
//
template <typename Ch>
bool operator==(const basic_string_slice<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_slice<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_slice<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_slice<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_slice<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_slice<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_slice to basic_string_slice
//
template <typename Ch>
bool operator==(const basic_string_slice<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_slice<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_slice<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_slice<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_slice<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_slice<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_slice to std::basic_string
//
template <typename Ch>
bool operator==(const basic_string_slice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_slice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_slice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_slice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_slice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_slice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ge(lhs, rhs);
}

//
// compare std::basic_string to basic_istring
//
template <typename Ch>
bool operator==(const std::basic_string<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const std::basic_string<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const std::basic_string<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const std::basic_string<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const std::basic_string<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const std::basic_string<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare std::basic_string to basic_string_slice
//
template <typename Ch>
bool operator==(const std::basic_string<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const std::basic_string<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const std::basic_string<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const std::basic_string<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const std::basic_string<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const std::basic_string<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare std::basic_string to basic_string_builder
//
template <typename Ch>
bool operator==(const std::basic_string<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const std::basic_string<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const std::basic_string<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const std::basic_string<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const std::basic_string<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const std::basic_string<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare std::basic_string to std::basic_string
//
template <typename Ch>
bool operator==(const std::basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const std::basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const std::basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const std::basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const std::basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const std::basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare basic_istring to Ch*
//
template <typename Ch>
bool operator==(const basic_istring<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_istring<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator==(const Ch* lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_istring<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_istring<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator!=(const Ch* lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_istring<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_istring<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<(const Ch* lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_istring<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_istring<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator<=(const Ch* lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_istring<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_istring<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>(const Ch* lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_istring<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_istring<Ch>>::ge(lhs, rhs);
}
template <typename Ch>
bool operator>=(const Ch* lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_istring<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_slice to Ch*
//
template <typename Ch>
bool operator==(const basic_string_slice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator==(const Ch* lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_slice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator!=(const Ch* lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_slice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<(const Ch* lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_slice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator<=(const Ch* lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_slice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>(const Ch* lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_slice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ge(lhs, rhs);
}
template <typename Ch>
bool operator>=(const Ch* lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_builder to Ch*
//
template <typename Ch>
bool operator==(const basic_string_builder<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator==(const Ch* lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_builder<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator!=(const Ch* lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_builder<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<(const Ch* lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_builder<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator<=(const Ch* lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_builder<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>(const Ch* lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_builder<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ge(lhs, rhs);
}
template <typename Ch>
bool operator>=(const Ch* lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_slice<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_builder to basic_string_builder
//
template <typename Ch>
bool operator==(const basic_string_builder<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_builder<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_builder<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_builder<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_builder<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_builder<Ch>& lhs, const basic_string_builder<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_builder to basic_string_slice
//
template <typename Ch>
bool operator==(const basic_string_builder<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_builder<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_builder<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_builder<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_builder<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_builder<Ch>& lhs, const basic_string_slice<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_builder to basic_istring
//
template <typename Ch>
bool operator==(const basic_string_builder<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_builder<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_builder<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_builder<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_builder<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_builder<Ch>& lhs, const basic_istring<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_builder to basic_string
//
template <typename Ch>
bool operator==(const basic_string_builder<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_builder<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_builder<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_builder<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_builder<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_builder<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_builder<Ch>>::ge(lhs, rhs);
}

} // namespace native

#endif
