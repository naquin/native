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

#include "native/hash.h"

#include <atomic>
#include <cassert>
#include <cstdlib>

namespace native {

template <typename Ch>
struct string_core_base {
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

    enum class storage: unsigned char {
        dynamic  = 0x0,
        small    = 0x1,
        static_  = 0x2,
    };

    enum {
        type_bits = 2,
        length_bits = sizeof(size_type)*8-type_bits
    };

    struct dynamic_data {
        // smart_pointer is similar to what's found under the hood in fbstring
        // from https://github.com/facebook/folly, available under Apache 2.0
        // license.
        struct smart_pointer {
            std::atomic<size_t>    shared_count;
            std::atomic<size_type> hash;
            size_type              capacity;
            value_type             head[1];
        };

        // no need for bit packing here since we need the size of
        // dynmaic_data to be the same as static and small
        size_type type;
        size_type length;
        pointer head;

        inline dynamic_data(size_type capacity) :
            dynamic_data(size_type{0}, capacity)
        { }

        inline dynamic_data(size_type length, size_type capacity) :
            type(static_cast<unsigned char>(storage::dynamic)),
            length(length),
            head(create(capacity))
        {
            null_terminate();
        }

        inline dynamic_data(const_pointer ptr, size_type length) : dynamic_data(length, length)
        {
            raw_copy(ptr, ptr + length, head);
        }

        inline dynamic_data(const_pointer ptr, size_type length, size_type capacity) : dynamic_data(length, capacity)
        {
            raw_copy(ptr, ptr + length, head);
        }

        inline dynamic_data(size_type length, value_type c) : dynamic_data(length, length)
        {
            for (size_type i = 0; i < length; ++i)
            {
                head[i] = c;
            }
        }

        template <typename InputIterator>
        inline dynamic_data(InputIterator first, InputIterator last, size_type length) : dynamic_data(length, length)
        {
            std::copy(first, last, head);
        }

        // move
        dynamic_data(dynamic_data&& right) :
            type(right.type),
            length(right.length),
            head(right.head)
        { }

        dynamic_data& operator=(dynamic_data&& right)
        {
            type   = right.type;
            length = right.length;
            head   = right.head;
            return *this;
        }

        // disable copy
        dynamic_data(const dynamic_data&) = delete;
        dynamic_data& operator=(const dynamic_data&) = delete;

        // assign/move/copy help with union access
        inline void assign(size_type length, size_type capacity)
        {
            this->type   = static_cast<unsigned char>(storage::dynamic);
            this->length = length;
            this->head   = create(capacity);
            null_terminate();
        }
    
        inline void assign(const_pointer ptr, size_type length)
        {
            assign(length, length);
            raw_copy(ptr, ptr + length, head);
        }
        
        inline void assign(const_pointer ptr, size_type length, size_type capacity)
        {
            assign(length, capacity);
            raw_copy(ptr, ptr + length, head);
        }

        inline void assign(size_type length, value_type c)
        {
            assign(length, length);
            for (size_type i = 0; i < length; ++i)
            {
                head[i] = c;
            }
        }
        
        template <typename InputIterator>
        inline void assign(InputIterator first, InputIterator last, size_type length)
        {
            assign(length, length);
            raw_copy(first, last, head);
        }
        
        inline void copy(const dynamic_data& right)
        {
            type   = right.type;
            length = right.length;
            head   = right.head;
            increment_shared_count();
        }
        
        inline smart_pointer* get() const
        {
            return
                reinterpret_cast<smart_pointer*>(
                reinterpret_cast<unsigned char*>(head) -
                    sizeof(smart_pointer) + sizeof(size_type)
            );
        }

        inline size_type capacity() const {
            return get()->capacity;
        }

        size_type hash() const
        {
            auto ptr = get();
            size_type value = ptr->hash.load(std::memory_order_relaxed);
            if (!value)
            {
                value = ::native::hash(head, length * sizeof(value_type));
                ptr->hash.store(value, std::memory_order_relaxed);
            }
            return value;
        }

        inline std::size_t reference_count()
        {
            return get()->shared_count.load(std::memory_order_acquire);
        }

        inline void increment_shared_count()
        {
            get()->shared_count.fetch_add(1, std::memory_order_acq_rel);
        }

        void decrement_shared_count()
        {
            smart_pointer* ptr = get();
            std::size_t previousCount =
                ptr->shared_count.fetch_sub(1, std::memory_order_acq_rel);
            if (previousCount == 1)
            {
                free(ptr);
            }
        }

        inline void null_terminate()
        {
            head[length] = '\0';
        }

        static pointer create(std::size_t capacity)
        {
            const std::size_t allocSize =
                sizeof(dynamic_data) +
                sizeof(smart_pointer) - sizeof(value_type) +
                (capacity * sizeof(value_type));

            auto result = reinterpret_cast<smart_pointer*>(std::malloc(allocSize));
            result->shared_count.store(1, std::memory_order_release);
            result->hash.store(0, std::memory_order_relaxed);
            result->capacity = capacity;
            return &result->head[0];
        }
    };

    struct small_data {
        enum {
            max_size = (sizeof(dynamic_data) - sizeof(value_type) - sizeof(size_type)) / sizeof(value_type),
        };

        static_assert(max_size >= 3, "");

        struct attrib_type {
            unsigned char type:type_bits;
            unsigned char length:sizeof(unsigned char)*8-type_bits;
        };

        union {
            attrib_type attribs;
            value_type  padding;
        };

        value_type head[max_size];
        size_type _hash;

        inline small_data(size_type length = 0) : _hash{}
        {
            attribs.type   = static_cast<unsigned char>(storage::small);
            attribs.length = length;
            null_terminate();
        }

        inline small_data(size_type length, value_type c) : small_data(length)
        {
            for (size_type i = 0; i < length; ++i)
            {
                head[i] = c;
            }
        }

        inline small_data(const_pointer ptr, size_type length) : small_data(length)
        {
            raw_copy(ptr, ptr + length, &head[0]);
        }

        template <typename InputIterator>
        inline small_data(InputIterator first, InputIterator last, size_type length) : small_data(length)
        {
            std::copy(first, last, &head[0]);
        }

        inline size_type hash()
        {
            if (!_hash)
            {
                _hash = ::native::hash(head, attribs.length  * sizeof(value_type));
            }
            return _hash;
        }
        
        inline void null_terminate()
        {
            head[attribs.length] = 0;
        }
    };

    struct static_data {
        size_type     type:type_bits;
        size_type     length:length_bits;
        size_type     hash;
        const_pointer head;

        static const value_type empty_value[1];

        inline static_data():
            type(static_cast<unsigned char>(storage::static_)),
            length(),
            head(empty_value)
        {
        
        }

        inline static_data(const_pointer ptr, size_type length, size_type hash):
            type(static_cast<unsigned char>(storage::static_)),
            length(length),
            hash(hash),
            head(ptr)
        {
        
        }
    };

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

    static inline void raw_copy(const value_type* first, const value_type* last, value_type* destination) {
        std::memcpy(destination, first, (last - first) * sizeof(value_type));
    }
};

template <typename Ch>
struct string_builder_core;

template <typename Ch>
struct string_core : string_core_base<Ch> {
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

    using storage      = typename base_type::storage;
    using small_data   = typename base_type::small_data;
    using static_data  = typename base_type::static_data;
    using dynamic_data = typename base_type::dynamic_data;

    inline string_core() : _small{} { }

    string_core(const_pointer ptr, size_type length)
    {
        if (length < small_data::max_size)
        {
            _small = small_data{ptr, length};
        }
        else
        {
            _dynamic.assign(ptr, length);
        }
    }

    // static string only -- hash is pre-computed
    inline constexpr string_core(const_pointer ptr, size_type length, size_type hash):
        _static{ptr, length, hash}
    {

    }

    inline string_core(const_pointer ptr):
        string_core(ptr, traits_type::length(ptr))
    {

    }

    string_core(size_type length, value_type c)
    {
        if (length < small_data::max_size)
        {
            _small = small_data{length, c};
        }
        else
        {
            _dynamic.assign(length, c);
        }
    }

    template <typename InputIterator>
    string_core(InputIterator first, InputIterator last, size_type length)
    {
        if (length < small_data::max_size)
        {
            _small = small_data{first, last, length};
        }
        else
        {
            _dynamic = dynamic_data{first, last, length};
        }
    }

    template <typename InputIterator>
    inline string_core(InputIterator first, InputIterator last):
        string_core(first, last, base_type::iterator_length(first, last))
    {

    }

    string_core(const string_core& rhs)
    {
        copy(rhs);
    }

    string_core& operator=(const string_core& rhs)
    {
        copy(rhs);
        return *this;
    }

    string_core(string_core&& rhs)
    {
        copy(rhs);
    }

    string_core& operator= (string_core&& rhs)
    {
        copy(rhs);
        return *this;
    }

    string_core(string_builder_core<Ch>&& core)
    {
        core.detach(_dynamic);
    }

    ~string_core()
    {
        free();
    }

    inline void copy(const string_core& rhs)
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
            _dynamic.copy(rhs._dynamic);
            break;
        }
    }

    inline void free()
    {
        if (this->type() == storage::dynamic)
        {
            _dynamic.decrement_shared_count();
        }
    }

    //
    // assign
    //
    void assign(const_pointer ptr, size_type length)
    {
        free();

        if (length < small_data::max_size)
        {
            _small = small_data{ptr, length};
        }
        else
        {
            _dynamic.assign(ptr, length);
        }
    }

    inline void assign(const_pointer ptr)
    {
        assign(ptr, traits_type::length(ptr));
    }

    void assign(size_type length, value_type c)
    {
        free();

        if (length < small_data::max_size)
        {
            _small = small_data{length, c};
        }
        else
        {
            _dynamic.assign(length, c);
        }
    }

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, size_type length)
    {
        free();

        if (length < small_data::max_size)
        {
            _small = small_data{first, last, length};
        }
        else
        {
            _dynamic = dynamic_data{first, last, length};
        }
    }

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        assign(first, last, base_type::iterator_length(first, last));
    }

    //
    // immutable methods
    //
    inline storage type() const
    {
        return static_cast<storage>(_static.type);
    }

    inline size_type size() const
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

    inline size_type capacity() const
    {
        switch (this->type())
        {
        case storage::static_:
            return _static.length;
        case storage::small:
            return small_data::max_size;
        case storage::dynamic:
            return _dynamic.capacity();
        }
    }

    inline const_pointer data() const
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

    inline size_t hash() const
    {
        switch (this->type())
        {
        case storage::static_:
            return _static.hash;
        case storage::small:
            return _small.hash();
        case storage::dynamic:
            return _dynamic.hash();
        }
    }
    
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
const typename string_core_base<Ch>::value_type
    string_core_base<Ch>::static_data::empty_value[1] = {0};

} // namespace native

#endif
