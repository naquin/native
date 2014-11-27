//
// Copyright (c) 2014 Mike Naquin. All rights reserved.
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

#ifndef NATIVE_STRING_BUILDER_CORE_H__
#define NATIVE_STRING_BUILDER_CORE_H__

#include "native/hash.h"

#include "native/detail/string_core.h"

namespace native {

template <typename Ch>
struct string_builder_core : string_core_base<Ch> {
    using base_type = string_core_base<Ch>;
    
    using traits_type            = std::char_traits<Ch>;
    using value_type             = typename traits_type::char_type;
    using allocator_type         = std::allocator<Ch>;
    using size_type              = typename allocator_type::size_type;
    using difference_type        = typename allocator_type::difference_type;
    using reference              = typename allocator_type::reference;
    using const_reference        = typename allocator_type::const_reference;
    using pointer                = typename allocator_type::pointer;
    using const_pointer          = typename allocator_type::const_pointer;
    using const_iterator         = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    
    using dynamic_data = typename base_type::dynamic_data;

    inline constexpr string_builder_core(size_type capacity):
        _dynamic(capacity)
    {

    }

    inline string_builder_core(const_pointer ptr, size_type length) : _dynamic(ptr, length)
    {

    }

    inline string_builder_core(const_pointer ptr):
        string_builder_core(ptr, traits_type::length(ptr))
    {

    }

    inline string_builder_core(size_type length, value_type c) : _dynamic(length, c) { }

    template <typename InputIterator>
    string_builder_core(InputIterator first, InputIterator last, size_type length) : _dynamic(first, last, length) { }

    template <typename InputIterator>
    inline string_builder_core(InputIterator first, InputIterator last):
        string_builder_core(first, last, iterator_length(first, last))
    {

    }

    ~string_builder_core()
    {
        if (_dynamic.head)
        {
            _dynamic.decrement_shared_count();
        }
    }

    //
    // immutable methods
    //
    inline size_type     size() const     { return _dynamic.length; }
    inline size_type     capacity() const { return _dynamic.capacity(); }
    inline const_pointer data() const     { return _dynamic.head; }

    //
    // mutable methods
    //
    pointer mutable_data()
    {
        if (_dynamic.reference_count() > 1)
        {
            size_t effective_capacity = _dynamic.capacity();
            const auto new_head = dynamic_data::create(effective_capacity);
            base_type::raw_copy(_dynamic.head, _dynamic.head + _dynamic.length + 1, new_head);
            _dynamic.decrement_shared_count();
            _dynamic.head = new_head;
        }
        return _dynamic.head;
    }

    void reserve(size_t capacity)
    {
        if (_dynamic.head)
        {
            if (_dynamic.capacity() >= capacity)
            {
                return; // has enough capacity
            }

            dynamic_data new_dynamic{&_dynamic.head[0], _dynamic.length, capacity};
            _dynamic.decrement_shared_count();
            _dynamic = std::move(new_dynamic);
        }
        else
        {
            _dynamic.assign(size_type{0}, capacity);
        }
    }

    void resize(size_t n)
    {
        if (!_dynamic.head) {
            _dynamic.assign(n, n);
            return;
        }

        if (n > capacity())
        {
            reserve(n);
        }

        _dynamic.length = n;
        _dynamic.null_terminate();
    }

    void push_back(value_type ch)
    {
        if (!_dynamic.head)
        {
            _dynamic.assign(size_type{0}, size_type{16});
        }

        const auto current_size = _dynamic.length;
        if (current_size == _dynamic.capacity())
        {
            reserve(1 + current_size * 3 / 2);
        }

        ++_dynamic.length;
        _dynamic.head[current_size] = ch;
    }

    void clear()
    {
        if (_dynamic.head)
        {
            _dynamic.length = 0;
            _dynamic.null_terminate();
        }
    }

    void null_terminate()
    {
        if (_dynamic.head)
        {
            _dynamic.null_terminate();
        }
    }

    void detach(dynamic_data& data)
    {
        if (_dynamic.head)
        {
            data = std::move(_dynamic);
            _dynamic.length = 0;
            _dynamic.head = nullptr;
        }
        else
        {
            data.length = 0;
            data.null_terminate();
        }
    }

    void detach(dynamic_data& data, size_type new_capacity)
    {
        if (_dynamic.head)
        {
            data = std::move(_dynamic);
        }
        else
        {
            data.length = 0;
            data.null_terminate();            
        }

        _dynamic.create(new_capacity);
    }

private:
    dynamic_data _dynamic;
};

} // namespace native

#endif
