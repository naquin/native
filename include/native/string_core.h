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

#ifndef NATIVE_STRING_CORE_H__
#define NATIVE_STRING_CORE_H__

#include <atomic>
#include <cassert>
#include <cstdlib>

namespace native {

template <typename Ch>
struct basic_string_core {
    typedef std::char_traits<Ch> traits_type;
    typedef typename traits_type::char_type value_type;
    typedef std::allocator<Ch> allocator_type;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static const value_type empty_value[1];

    enum class storage: unsigned char {
        small    = 0x0,
        static_  = 0x1,
        dynamic  = 0x2,
    };

    basic_string_core()
    {
        _static = static_data();
    }

    basic_string_core(const_pointer ptr,
                      size_type length,
                      storage type = storage::dynamic)
    {
        if (type == storage::static_)
        {
            _static = static_data(ptr, length);
        }
        else if (length < max_small_size)
        {
            _small = small_data(length);
            std::copy(ptr, ptr + length, &_small.head[0]);
            _small.head[length] = '\0'; // null terminate
        }
        else
        {
            _dynamic = dynamic_data(length);
            std::copy(ptr, ptr + length, _dynamic.head);
        }
    }

    basic_string_core(const_pointer ptr,
                      storage type = storage::dynamic):
        basic_string_core(ptr, traits_type::length(ptr), type)
    {

    }

    basic_string_core(size_type length, value_type c)
    {
        if (length < max_small_size)
        {
            _small = small_data(length);
            for (size_type i = 0; i < length; ++i)
            {
                _small.head[i] = c;
            }
            _small.head[length] = '\0'; // null terminate
        }
        else
        {
            _dynamic = dynamic_data(length);
            for (size_type i = 0; i < length; ++i)
            {
                _dynamic.head[i] = c;
            }
        }
    }

    template <typename InputIterator>
    basic_string_core(InputIterator first, InputIterator last, size_type length)
    {
        if (length < max_small_size)
        {
            _small = small_data(length);
            std::copy(first, last, &_small.head[0]);
            _small.head[length] = '\0'; // null terminate
        }
        else
        {
            _dynamic = dynamic_data(length);
            std::copy(first, last, _dynamic.head);
        }
    }

    // TODO: specialize for random access iterators
    template <typename InputIterator>
    static size_type iterator_length(InputIterator first, InputIterator last)
    {
        size_type length = 0;
        for (auto it = first; it != last; ++it)
        {
            ++length;
        }

        return length;
    }

    template <typename InputIterator>
    basic_string_core(InputIterator first, InputIterator last):
        basic_string_core(first, last, iterator_length(first, last))
    {

    }

    basic_string_core(const basic_string_core& rhs)
    {
        switch (rhs.type())
        {
        case storage::static_:
            _static = rhs._static;
            break;
        case storage::small:
            _small = rhs._small;
            break;
        case storage::dynamic:
            _dynamic = rhs._dynamic;
            _dynamic.increment_shared_count();
            break;
        }
    }

    basic_string_core(basic_string_core&& rhs)
    {
        switch (rhs.type())
        {
        case storage::static_:
            _static = rhs._static;
            break;
        case storage::small:
            _small = rhs._small;
            break;
        case storage::dynamic:
            _dynamic = rhs._dynamic;
            rhs.clear();
            break;
        }
    }

    basic_string_core& operator=(const basic_string_core& rhs)
    {
        switch (this->type())
        {
        case storage::static_:
        case storage::small:
            break;
        case storage::dynamic:
            _dynamic.decrement_shared_count();
            break;
        }

        switch (rhs.type())
        {
        case storage::static_:
            _static = rhs._static;
            break;
        case storage::small:
            _small = rhs._small;
            break;
        case storage::dynamic:
            _dynamic = rhs._dynamic;
            _dynamic.increment_shared_count();
            break;
        }
        return *this;
    }

    ~basic_string_core()
    {
        switch (this->type())
        {
        case storage::static_:
        case storage::small:
            break;
        case storage::dynamic:
            _dynamic.decrement_shared_count();
            break;
        }
    }

    size_type size() const
    {
        switch (this->type())
        {
        case storage::static_:
            return _static.length;
        case storage::small:
            return _small.attribs.length;
        case storage::dynamic:
            return _dynamic.length;
        }
    }

    const_pointer data() const
    {
        switch (this->type())
        {
        case storage::static_:
            return _static.head;
        case storage::small:
            return _small.head;
        case storage::dynamic:
            return _dynamic.head;
        }
    }

    storage type() const
    {
        const auto t = _static.type;
        return static_cast<storage>(t);
    }

public:

    void clear()
    {
        _static = static_data();
    }

    enum {
        type_bits = 2,
        length_bits = sizeof(size_type)*8-type_bits
    };

    // smart_pointer is similar to what's found under the hood in fbstring
    // from https://github.com/facebook/folly, available under Apache 2.0
    // license.
    struct smart_pointer {
        std::atomic<std::size_t> shared_count;
        value_type               head[1];
    };

    struct dynamic_data {
        size_type type:type_bits;
        size_type length:length_bits;
        pointer head;

        dynamic_data(size_type length):
            type(static_cast<unsigned char>(storage::dynamic)),
            length(length),
            head(create(length))
        {
            head[length] = '\0'; // null terminate
        }

        smart_pointer* get()
        {
            auto ptr =
                reinterpret_cast<smart_pointer*>(
                reinterpret_cast<unsigned char*>(head) - sizeof(std::atomic<std::size_t>)
            );
            return ptr;
        }

        std::size_t reference_count()
        {
            return get()->shared_count.load(std::memory_order_acquire);
        }

        void increment_shared_count()
        {
            get()->shared_count.fetch_add(1, std::memory_order_acq_rel);
        }

        void decrement_shared_count()
        {
            const auto ptr = get();
            std::size_t previousCount =
                ptr->shared_count.fetch_sub(1, std::memory_order_acq_rel);
            if (previousCount == 1)
            {
                free(ptr);
            }
        }

        static pointer create(std::size_t size)
        {
            const std::size_t allocSize =
                sizeof(dynamic_data) + (size * sizeof(value_type));
            auto result = reinterpret_cast<smart_pointer*>(std::malloc(allocSize));
            result->shared_count.store(1, std::memory_order_release);
            return &result->head[0];
        }
    };

    enum {
        max_small_size = sizeof(dynamic_data) - sizeof(value_type),
    };

    struct small_data {
        struct attrib_type {
            unsigned char type:type_bits;
            unsigned char length:sizeof(unsigned char)*8-type_bits;
        };
        union {
            attrib_type attribs;
            value_type  padding;
        };
        value_type head[max_small_size];
        
        small_data(size_type length)
        {
            attribs.type = 0;
            attribs.length = length;
        }
    };

    struct static_data {
        size_type     type:type_bits;
        size_type     length:length_bits;
        const_pointer head;
        
        static_data():
            type(static_cast<unsigned char>(storage::static_)),
            length(),
            head(empty_value)
        {
        
        }

        static_data(const_pointer ptr, size_type length):
            type(static_cast<unsigned char>(storage::static_)),
            length(length),
            head(ptr)
        {
        
        }
    };

    static_assert(sizeof(small_data) == sizeof(static_data),
        "sizes should match");
    static_assert(sizeof(small_data) == sizeof(dynamic_data),
        "sizes should match");
    static_assert(sizeof(static_data) == sizeof(dynamic_data),
        "sizes should match");

    union {
        small_data    _small;
        static_data   _static;
        dynamic_data  _dynamic;
    };

};

template <typename Ch>
const typename basic_string_core<Ch>::value_type
    basic_string_core<Ch>::empty_value[1] = {0};

} // namespace native

#endif
