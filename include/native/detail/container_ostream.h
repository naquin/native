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

#ifndef NATIVE_DETAIL_CONTAINER_OSTREAM_H__
#define NATIVE_DETAIL_CONTAINER_OSTREAM_H__

#include "native/config.h"

#include <string>

namespace native
{
namespace detail
{

template <typename Container>
class container_ostream
{
public:
    using container_type = Container;
    using char_type = typename container_type::value_type;

    container_ostream(Container& container)
        : _container{container}
    {
    }

    inline void put(char_type value) { _container.push_back(value); }

private:
    container_type& _container;
};

} // namespace detail
} // namespace native

#endif
