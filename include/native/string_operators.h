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
// compare basic_string to basic_string
//
template <typename Ch>
bool operator==(const basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string to basic_string_splice
//
template <typename Ch>
bool operator==(const basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string to std::basic_string
//
template <typename Ch>
bool operator==(const basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_splice to basic_string
//
template <typename Ch>
bool operator==(const basic_string_splice<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_splice<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_splice<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_splice<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_splice<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_splice<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_splice to basic_string_splice
//
template <typename Ch>
bool operator==(const basic_string_splice<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_splice<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_splice<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_splice<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_splice<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_splice<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_splice to std::basic_string
//
template <typename Ch>
bool operator==(const basic_string_splice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_splice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_splice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_splice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_splice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_splice<Ch>& lhs, const std::basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ge(lhs, rhs);
}

//
// compare std::basic_string to basic_string
//
template <typename Ch>
bool operator==(const std::basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const std::basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const std::basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const std::basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const std::basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const std::basic_string<Ch>& lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare std::basic_string to basic_string_splice
//
template <typename Ch>
bool operator==(const std::basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const std::basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const std::basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const std::basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const std::basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<std::basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const std::basic_string<Ch>& lhs, const basic_string_splice<Ch>& rhs)
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
// compare basic_string to Ch*
//
template <typename Ch>
bool operator==(const basic_string<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator==(const Ch* lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator!=(const Ch* lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<(const Ch* lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator<=(const Ch* lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>(const Ch* lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string<Ch>>::ge(lhs, rhs);
}
template <typename Ch>
bool operator>=(const Ch* lhs, const basic_string<Ch>& rhs)
{
    return detail::string_common<basic_string<Ch>>::ge(lhs, rhs);
}

//
// compare basic_string_splice to Ch*
//
template <typename Ch>
bool operator==(const basic_string_splice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator==(const Ch* lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::eq(lhs, rhs);
}
template <typename Ch>
bool operator!=(const basic_string_splice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator!=(const Ch* lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ne(lhs, rhs);
}
template <typename Ch>
bool operator<(const basic_string_splice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<(const Ch* lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::lt(lhs, rhs);
}
template <typename Ch>
bool operator<=(const basic_string_splice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator<=(const Ch* lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::le(lhs, rhs);
}
template <typename Ch>
bool operator>(const basic_string_splice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>(const Ch* lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::gt(lhs, rhs);
}
template <typename Ch>
bool operator>=(const basic_string_splice<Ch>& lhs, const Ch* rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ge(lhs, rhs);
}
template <typename Ch>
bool operator>=(const Ch* lhs, const basic_string_splice<Ch>& rhs)
{
    return detail::string_common<basic_string_splice<Ch>>::ge(lhs, rhs);
}

} // namespace native

#endif
