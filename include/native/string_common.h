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

#ifndef NATIVE_STRING_COMMON_H__
#define NATIVE_STRING_COMMON_H__

#include "native/string_base.h"

namespace native {
namespace detail {

// Non-trivial string member implementations that can be shared with differing
// string types.
//
// Find implementations inspired by libc++, available under MIT license.


template <typename String>
struct string_common {
    using string_type = String;

    using traits_type            = typename string_type::traits_type;
    using value_type             = typename string_type::value_type;
    using allocator_type         = typename string_type::allocator_type;
    using size_type              = typename string_type::size_type;
    using difference_type        = typename string_type::difference_type;
    using reference              = typename string_type::reference;
    using const_reference        = typename string_type::const_reference;
    using pointer                = typename string_type::pointer;
    using const_pointer          = typename string_type::const_pointer;
    using const_iterator         = typename string_type::const_pointer;
    using const_reverse_iterator = typename string_type::const_reverse_iterator;

    //
    // find
    //
    static size_type find(const string_type& self,
                          const_pointer s,
                          size_type pos,
                          size_type n) noexcept
    {
        size_type length = self.size();
        if (pos > length || length - pos < n)
        {
            return self.npos;
        }
        if (n == 0)
        {
            return pos;
        }
        const_pointer p = self.data();
        const_pointer r = std::search(p + pos, p + length, s, s + n, traits_type::eq);
        if (r == p + length)
        {
            return self.npos;
        }
        return static_cast<size_type>(r - p);
    }

    static size_type find(const string_type& self,
                          value_type c,
                          size_type pos) noexcept
    {
        size_type length = self.size();
        if (pos >= length)
        {
            return self.npos;
        }
        const_pointer p = self.data();
        const_pointer r = traits_type::find(p + pos, length - pos, c);
        if (r == 0)
        {
            return self.npos;
        }

        return static_cast<size_type>(r - p);
    }

    //
    // rfind
    //
    static size_type rfind(const string_type& self,
                           const_pointer s,
                           size_type pos,
                           size_type n) noexcept
    {
        size_type length = self.size();
        pos = std::min(pos, length);
        if (n < length - pos)
        {
            pos += n;
        }
        else
        {
            pos = length;
        }

        const_pointer p = self.data();
        const_pointer r = std::find_end(p, p + pos, s, s + n, traits_type::eq);

        if (n > 0 && r == p + pos)
        {
            return self.npos;
        }

        return static_cast<size_type>(r - p);
    }

    static size_type rfind(const string_type& self,
                           value_type c,
                           size_type pos) noexcept
    {
        size_type length = self.size();
        if (length)
        {
            if (pos < length)
            {
                ++pos;
            }
            else
            {
                pos = length;
            }
            const_pointer p = self.data();
            for (auto ps = p + pos; ps != p;)
            {
                if (traits_type::eq(*--ps, c))
                {
                    return static_cast<size_type>(ps - p);
                }
            }
        }
        return self.npos;
    }

    //
    // find_first_of
    //
    static size_type find_first_of(const string_type& self,
                                   const_pointer s,
                                   size_type pos,
                                   size_type n) noexcept
    {
        size_type length = self.size();
        if (pos >= length || n == 0)
        {
            return self.npos;
        }
        const_pointer p = self.data();
        const_pointer r = std::find_first_of(p + pos, p + length, s, s + n, traits_type::eq);
        if (r == p + length)
        {
            return self.npos;
        }
        return static_cast<size_type>(r - p);
    }

    //
    // find_last_of
    //
    static size_type find_last_of(const string_type& self,
                                  const_pointer s,
                                  size_type pos,
                                  size_type n) noexcept
    {
        if (n != 0)
        {
            size_type length = self.size();
            if (pos < length)
            {
                ++pos;
            }
            else
            {
                pos = length;
            }
            const_pointer p = self.data();
            for (auto ps = p + pos; ps != p;)
            {
                const_pointer r = traits_type::find(s, n, *--ps);
                if (r)
                {
                    return static_cast<size_type>(ps - p);
                }
            }
        }
        return self.npos;
    }

    //
    // find_first_not_of
    //
    static size_type find_first_not_of(const string_type& self,
                                       const_pointer s,
                                       size_type pos,
                                       size_type n) noexcept
    {
        size_type length = self.size();
        if (pos < length)
        {
            const_pointer p = self.data();
            const_pointer pe = p + length;
            for (auto ps = p + pos; ps != pe; ++ps)
            {
                if (traits_type::find(s, n, *ps) == 0)
                {
                    return static_cast<size_type>(ps - p);
                }
            }
        }
        return self.npos;
    }

    static size_type find_first_not_of(const string_type& self,
                                       value_type c,
                                       size_type pos) noexcept
    {
        size_type length = self.size();
        if (pos < length)
        {
            const_pointer p = self.data();
            const_pointer pe = p + length;
            for (auto ps = p + pos; ps != pe; ++ps)
            {
                if (!traits_type::eq(*ps, c))
                {
                    return static_cast<size_type>(ps - p);
                }
            }
        }
        return self.npos;
    }

    //
    // find_last_not_of
    //
    static size_type find_last_not_of(const string_type& self,
                                      const_pointer s,
                                      size_type pos,
                                      size_type n) noexcept
    {
        size_type length = self.size();
        if (pos < length)
        {
            ++pos;
        }
        else
        {
            pos = length;
        }
        const_pointer p = self.data();
        for (const_pointer ps = p + pos; ps != p;)
        {
            if (traits_type::find(s, n, *--ps) == 0)
            {
                return static_cast<size_type>(ps - p);
            }
        }
        return self.npos;
    }

    static size_type find_last_not_of(const string_type& self,
                                      value_type c,
                                      size_type pos) noexcept
    {
        size_type length = self.size();
        if (pos < length)
        {
            ++pos;
        }
        else
        {
            pos = length;
        }
        const_pointer p = self.data();
        for (auto ps = p + pos; ps != p;)
        {
            if (!traits_type::eq(*--ps, c))
            {
                return static_cast<size_type>(ps - p);
            }
        }
        return self.npos;
    }

    //
    // Compare
    //
    template <typename RightString>
    static int compare(const string_type& self, const RightString& str) noexcept
    {
        auto lhs_length = self.size();
        auto rhs_length = str.size();
        const auto result = traits_type::compare(self.data(), str.data(),
            std::min(lhs_length, rhs_length)
        );
        
        if (result != 0)
        {
            return result;
        }
        if (lhs_length < rhs_length)
        {
            return -1;
        }
        if (lhs_length > rhs_length)
        {
            return 1;
        }
        return 0;
    }

    template <typename RightString>
    static inline int compare(const string_type& self,
        size_type pos1, size_type n1, const RightString& str)
    {
        return compare(self, pos1, n1, str.data(), str.size());
    }

    static inline int compare(
        const string_type& self,
        size_type pos1, size_type n1,
        const string_type& str,
        size_type pos2, size_type n2)
    {
        return compare(self, pos1, n1, str, pos2, n2);
    }

    static inline int compare(const string_type& self, const_pointer s) noexcept
    {
        return compare(self, 0, self.npos, s, traits_type::length(s));
    }

    static inline int compare(
        const string_type& self,
        size_type pos1, size_type n1, const_pointer s)
    {
        return compare(self, pos1, n1, s, traits_type::length(s));
    }

    template <typename RightString>
    static int compare(const string_type& self,
                       size_type pos1,
                       size_type n1,
                       const RightString& str,
                       size_type pos2,
                       size_type n2)
    {
        size_type length = str.size();
        if (pos2 > length)
        {
            self.throw_out_of_range();
        }
        return compare(self, pos1, n1, str.data() + pos2, std::min(n2, length - pos2));
    }

    static int compare(const string_type& self,
                       size_type pos1,
                       size_type n1,
                       const_pointer s,
                       size_type n2)
    {
        size_type length = self.size();
        if (pos1 > length || n2 == self.npos)
        {
            self.throw_out_of_range();
        }
        size_type rhs_length = std::min(n1, length - pos1);
        int result = traits_type::compare(self.data() + pos1, s, std::min(rhs_length, n2));
        if (result == 0)
        {
            if (rhs_length < n2)
            {
                return -1;
            }
            else if (rhs_length > n2)
            {
                return 1;
            }
        }
        return result;
    }

    //
    // Compare two string objects
    //
    template <typename RightString>
    static inline bool eq(const string_type& lhs, const RightString& rhs)
    {
        return compare(lhs, rhs) == 0;
    }

    template <typename RightString>
    static inline bool ne(const string_type& lhs, const RightString& rhs)
    {
        return compare(lhs, rhs) != 0;
    }

    template <typename RightString>
    static inline bool lt(const string_type& lhs, const RightString& rhs)
    {
        return compare(lhs, rhs) <= 0;
    }

    template <typename RightString>
    static inline bool le(const string_type& lhs, const RightString& rhs)
    {
        return compare(lhs, rhs) <= 0;
    }

    template <typename RightString>
    static inline bool gt(const string_type& lhs, const RightString& rhs)
    {
        return compare(lhs, rhs) > 0;
    }

    template <typename RightString>
    static inline bool ge(const string_type& lhs, const RightString& rhs)
    {
        return compare(lhs, rhs) >= 0;
    }


    //
    // Compare a string object and null-terminated array of Ch
    //
    template <typename RightString>
    static inline bool eq(typename RightString::const_pointer lhs, const RightString& rhs)
    {
        return RightString::traits_type::compare(lhs, rhs.data(), rhs.size()) == 0;
    }

    static inline bool eq(const string_type& lhs, const_pointer rhs)
    {
        return compare(lhs, rhs) == 0;
    }

    template <typename RightString>
    static inline bool ne(typename RightString::const_pointer lhs, const RightString& rhs)
    {
        return RightString::traits_type::compare(lhs, rhs.data(), rhs.size()) != 0;
    }

    static inline bool ne(const string_type& lhs, const_pointer rhs)
    {
        return compare(lhs, rhs) != 0;
    }

    template <typename RightString>
    static inline bool lt(typename RightString::const_pointer lhs, const RightString& rhs)
    {
        return RightString::traits_type::compare(lhs, rhs.data(), rhs.size()) < 0;
    }

    static inline bool lt(const string_type& lhs, const_pointer rhs)
    {
        return compare(lhs, rhs) < 0;
    }

    template <typename RightString>
    static inline bool le(typename RightString::const_pointer lhs, const RightString& rhs)
    {
        return RightString::traits_type::compare(lhs, rhs.data(), rhs.size()) <= 0;
    }

    static inline bool le(const string_type& lhs, const_pointer rhs)
    {
        return compare(lhs, rhs) <= 0;
    }

    template <typename RightString>
    static inline bool gt(typename RightString::const_pointer lhs, const RightString& rhs)
    {
        return RightString::traits_type::compare(lhs, rhs.data(), rhs.size()) > 0;
    }

    static inline bool gt(const string_type& lhs, const_pointer rhs)
    {
        return compare(lhs, rhs) > 0;
    }

    template <typename RightString>
    static inline bool ge(typename RightString::const_pointer lhs, const RightString& rhs)
    {
        return RightString::traits_type::compare(lhs, rhs.data(), rhs.size()) >= 0;
    }

    static inline bool ge(const string_type& lhs, const_pointer rhs)
    {
        return compare(lhs, rhs) >= 0;
    }


    //
    // streams
    //
    static inline std::basic_ostream<value_type>&
        write(std::basic_ostream<value_type>& ostr, const string_type& s)
    {
        ostr.write(s.c_str(), s.size());
        return ostr;
    }

    static inline std::basic_istream<value_type>&
        read(std::basic_istream<value_type>& istr, string_type& s)
    {
        std::string tmp;
        istr >> tmp;
        s = tmp;
        return istr;
    }
};

} } // namespace native

#endif
