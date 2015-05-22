// Adapted from http://www.boost.org/doc/libs/1_56_0/boost/implicit_cast.hpp

// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

namespace native
{
namespace detail
{

template <class T>
struct icast_identity
{
    using type = T;
};

} // namespace detail

// implementation originally suggested by C. Green in
// http://lists.boost.org/MailArchives/boost/msg00886.php

// The use of identity creates a non-deduced form, so that the
// explicit template argument must be supplied
template <typename T>
inline T implicit_cast(typename detail::icast_identity<T>::type x)
{
    return x;
}

// incomplete return type now is here
// template <typename T>
// void implicit_cast (...);

} // namespace native
