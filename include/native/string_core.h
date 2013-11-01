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

    enum class storage : unsigned char {
        static_  = 0x00,
        dynamic  = 0x01,
        splice   = 0x02
    };

    basic_string_core():
        _storage(storage::static_),
        _length()
    {
        _static = static_data{empty_value};
    }

    basic_string_core(const_pointer ptr,
                      size_type length,
                      storage type = storage::dynamic):
        _storage(type),
        _length(length)
    {
        switch (_storage)
        {
        case storage::static_:
            _static = static_data{ptr};
            break;
        case storage::dynamic:
            _dynamic = dynamic_data::create(_length);
            std::copy(ptr, ptr + _length, &_dynamic->head[0]);
            null_terminate();
            break;
        case storage::splice:
            assert(!"Cannot have splice in this constructor.");
            break;
        }
    }

    basic_string_core(const_pointer ptr,
                      storage type = storage::dynamic):
        _storage(type),
        _length(traits_type::length(ptr))
    {
        switch (_storage)
        {
        case storage::static_:
            _static = static_data{ptr};
            break;
        case storage::dynamic:
            _dynamic = dynamic_data::create(_length);
            std::copy(ptr, ptr + _length, &_dynamic->head[0]);
            null_terminate();
            break;
        case storage::splice:
            assert(!"Cannot have splice in this constructor.");
            break;
        }
    }
    
    basic_string_core(size_type n, value_type c):
        _storage(storage::dynamic),
        _length(n)
    {
        _dynamic = dynamic_data::create(_length);
        for (size_type i = 0; i < n; ++i)
        {
            _dynamic->head[i] = c;
        }
        null_terminate();
    }

    template <typename InputIterator>
    basic_string_core(InputIterator first, InputIterator last):
        _storage(storage::dynamic),
        _length(0)
    {
        for (auto it = first; it != last; ++it)
        {
            ++_length;
        }
        // TODO: specialize for random access iterators
//        _length = last - first; 
        _dynamic = dynamic_data::create(_length);
        std::copy(first, last, &_dynamic->head[0]);
        null_terminate();
    }

    basic_string_core(const basic_string_core& original, size_type start, size_type length):
        _storage(storage::splice),
        _length(length)
    {
        switch (original._storage)
        {
        case storage::static_:
            _storage     = storage::static_;
            _static      = original._static;
            _static.head = original.data() + start;
            break;
        case storage::dynamic:
            _splice = splice_data{original._dynamic, original.data() + start};
            _splice.dynamic->increment_shared_count();
            break;
        case storage::splice:
            _splice = splice_data{original._splice.dynamic, original.data() + start};
            _splice.dynamic->increment_shared_count();
            break;
        }
    }

    basic_string_core(const basic_string_core& rhs):
        _storage(rhs._storage),
        _length(rhs._length)
    {
        switch (_storage)
        {
        case storage::static_:
            _static = rhs._static;
            break;
        case storage::dynamic:
            _dynamic = rhs._dynamic;
            _dynamic->increment_shared_count();
            break;
        case storage::splice:
            _splice = rhs._splice;
            _splice.dynamic->increment_shared_count();
            break;
        }
    }

    basic_string_core(basic_string_core&& rhs):
        _storage(rhs._storage),
        _length(rhs._length)
    {
        switch (_storage)
        {
        case storage::static_:
            _static = rhs._static;
            break;
        case storage::dynamic:
            _dynamic = rhs._dynamic;
            rhs.clear();
            break;
        case storage::splice:
            _splice = rhs._splice;
            rhs.clear();
            break;
        }
    }

    basic_string_core& operator=(const basic_string_core& rhs)
    {
        switch (_storage)
        {
        case storage::static_:
            break;
        case storage::dynamic:
            _dynamic->decrement_shared_count();
            break;
        case storage::splice:
            _splice.dynamic->decrement_shared_count();
            break;
        }

        _storage = rhs._storage;
        _length = rhs._length;
        switch (_storage)
        {
        case storage::static_:
            _static = rhs._static;
            break;
        case storage::dynamic:
            _dynamic = rhs._dynamic;
            _dynamic->increment_shared_count();
            break;
        case storage::splice:
            _splice = rhs._splice;
            _splice.dynamic->increment_shared_count();
            break;
        }
        return *this;
    }


    ~basic_string_core()
    {
        switch (_storage)
        {
        case storage::static_:
            break;
        case storage::dynamic:
            _dynamic->decrement_shared_count();
            break;
        case storage::splice:
            _splice.dynamic->decrement_shared_count();
            break;
        }
    }

    size_type size() const
    {
        return _length;
    }

    const_pointer data() const
    {
        switch (_storage)
        {
        case storage::static_:
            return _static.head;
        case storage::dynamic:
            return _dynamic->head;
        case storage::splice:
            return _splice.head;
        }
//        return _storage == storage::dynamic ? _dynamic->head :
//               _storage == storage::static_ ? _static.head :
//               _splice.head;
    }

    inline void null_terminate()
    {
//        assert(!_is_static);
        _dynamic->head[_length] = 0;
    }

private:

    void clear()
    {
        _storage = storage::static_;
        _length = 0;
        _static = static_data{empty_value};
    }

    struct dynamic_data {
        // dynamic_data is similar to what's found under the hood in fbstring
        // from https://github.com/facebook/folly, available under Apache 2.0 license.
        std::size_t reference_count()
        {
            return shared_count.load(std::memory_order_acquire);
        }

        void increment_shared_count()
        {
            shared_count.fetch_add(1, std::memory_order_acq_rel);
        }

        void decrement_shared_count()
        {
            std::size_t previousCount =
                shared_count.fetch_sub(1, std::memory_order_acq_rel);
            if (previousCount == 1)
            {
                free(this);
            }
        }

        static dynamic_data* create(std::size_t size)
        {
            const std::size_t allocSize = sizeof(dynamic_data) + (size * sizeof(value_type));
            auto result = reinterpret_cast<dynamic_data*>(std::malloc(allocSize));
            result->shared_count.store(1, std::memory_order_release);
//        std::size_t resultSize = (allocSize - sizeof(rep)) / sizeof(value_type);
            return result;
        }

        std::atomic<std::size_t> shared_count;
//        std::size_t              hash;
        value_type               head[1];
    };

    struct splice_data {
        dynamic_data* dynamic;
        const_pointer head;
//        std::size_t   hash;
    };

    struct static_data {
        const_pointer head;
//        std::size_t   hash;
    };

    storage   _storage;
    size_type _length;

    union {
        static_data   _static;
        dynamic_data* _dynamic;
        splice_data   _splice;
    };

};

template <typename Ch>
const typename basic_string_core<Ch>::value_type
    basic_string_core<Ch>::empty_value[1] = {0};

} // namespace native

#endif
