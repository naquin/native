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

#ifndef NATIVE_HASH_H__
#define NATIVE_HASH_H__

#include "native/string_base.h"
#include "native/types.h"

namespace native {

//
// http://www.isthe.com/chongo/tech/comp/fnv/
//

template <typename T, size_t size>
struct fnv_traits;

template <typename T>
struct fnv_traits<T, 4>
{
    typedef T size_type;

    static constexpr size_type prime = 16777619UL;
    static constexpr size_type basis = 2166136261UL;

    static inline size_type multiply_by_prime(size_type value)
    {
        return (value << 1) +
               (value << 4) +
               (value << 7) +
               (value << 8) +
               (value << 24);
    }

};

template <typename T>
struct fnv_traits<T, 8>
{
    typedef T size_type;

    static constexpr size_type prime = 1099511628211ULL;
    static constexpr size_type basis = 14695981039346656037ULL;

    static inline size_type multiply_by_prime(size_type value)
    {
        return (value << 1) +
               (value << 4) +
               (value << 5) +
               (value << 7) +
               (value << 8) +
               (value << 40);
    }
};



//
// hashes are from http://www.isthe.com/chongo/tech/comp/fnv/
//
template <typename T>
struct basic_fnv1_hasher
{
    typedef fnv_traits<T, sizeof(T)> traits_type;

    typedef typename traits_type::size_type size_type;

    static constexpr size_type prime = traits_type::prime;
    static constexpr size_type basis = traits_type::basis;

    static constexpr size_type static_hash(unsigned char ch,
                                           const char* s,
                                           size_t size,
                                           size_type value)
    {
        return size == 0 ? value : static_hash(s[0], s + 1, size - 1, (value * prime) ^ ch);
    }

    static constexpr size_type static_hash(const char* s, size_t size)
    {
        return static_hash(s[0], s + 1, size, basis);
    }

    static size_type hash(const char* s)
    {
        size_type value = basis;
        for (; *s; ++s)
        {
            // multiply by the FNV magic prime
            value += traits_type::multiply_by_prime(value);

            // xor the bottom with the current octet
            value ^= static_cast<unsigned char>(*s);
        }

        return value;
    }

    static size_type hash(const void* buf, size_t size)
    {
        size_type value = basis;
        const auto* char_buf = reinterpret_cast<const unsigned char*>(buf);
        for (size_t i = 0; i < size; ++i)
        {
            // multiply by the FNV magic prime
            value += traits_type::multiply_by_prime(value);

            // xor the bottom with the current octet
            value ^= char_buf[i];
        }

        return value;
    }

    template <typename String>
    static typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type hash(const String& s)
    {
        return hash(s.data(), s.size());
    }
};


template <typename T>
struct basic_fnv1a_hasher
{
    typedef fnv_traits<T, sizeof(T)> traits_type;
    
    typedef typename traits_type::size_type size_type;

    static constexpr size_type prime = traits_type::prime;
    static constexpr size_type basis = traits_type::basis;

    static constexpr size_type static_hash(unsigned char ch,
                                           const char* s,
                                           size_t size,
                                           size_type value)
    {
        return size == 0 ? value : static_hash(s[0], s + 1, size - 1, (value ^ ch) * prime);
    }

    static constexpr size_type static_hash(const char* s, size_t size)
    {
        return static_hash(s[0], s + 1, size, basis);
    }

    static size_type hash(const char* s)
    {
        size_type value = basis;
        for (; *s; ++s)
        {
            // xor the bottom with the current octet
            value ^= static_cast<unsigned char>(*s);

            // multiply by the FNV magic prime
            value += traits_type::multiply_by_prime(value);
        }

        return value;
    }

    static size_type hash(const void* buf, size_t size)
    {
        size_type value = basis;
        const auto* char_buf = reinterpret_cast<const unsigned char*>(buf);

        for (size_t i = 0; i < size; ++i)
        {
            // xor the bottom with the current octet
            value ^= char_buf[i];

            // multiply by the FNV magic prime
            value += traits_type::multiply_by_prime(value);
        }

        return value;
    }

    template <typename String>
    static typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type hash(const String& s)
    {
        return hash(s.data(), s.size());
    }
};


typedef basic_fnv1_hasher<size_t> fnv1_hasher;
typedef basic_fnv1_hasher<uint32> fnv1_32_hasher;
typedef basic_fnv1_hasher<uint64> fnv1_64_hasher;

typedef basic_fnv1a_hasher<size_t> fnv1a_hasher;
typedef basic_fnv1a_hasher<uint32> fnv1a_32_hasher;
typedef basic_fnv1a_hasher<uint64> fnv1a_64_hasher;

//
// user-defined literals
//
constexpr size_t operator ""_fnv1(const char* s, size_t size)
{
    return fnv1_hasher::static_hash(s, size);
}

constexpr size_t operator ""_fnv1a(const char* s, size_t size)
{
    return fnv1a_hasher::static_hash(s, size);
}

// fnv1 (32-bit)
constexpr size_t operator ""_fnv32(const char* s, size_t size)
{
    return fnv1_32_hasher::static_hash(s, size);
}

constexpr size_t operator ""_fnv64(const char* s, size_t size)
{
    return fnv1_64_hasher::static_hash(s, size);
}

// fnv1a (32-bit)
constexpr size_t operator ""_fnv32a(const char* s, size_t size)
{
    return fnv1a_32_hasher::static_hash(s, size);
}

constexpr size_t operator ""_fnv64a(const char* s, size_t size)
{
    return fnv1a_64_hasher::static_hash(s, size);
}

//
// generic static hash
//
constexpr size_t operator ""_hash(const char* s, size_t size)
{
    return fnv1a_hasher::static_hash(s, size);
}

//
// convenience functions
//

// fnv1 (size_t)
inline size_t fnv1(const char* s)
{
    return fnv1_hasher::hash(s);
}

inline size_t fnv1(const void* buf, size_t size)
{
    return fnv1_hasher::hash(buf, size);
}

template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    size_t
>::type fnv1(const String& s)
{
    return fnv1_hasher::hash(s);
}


// fnv1a (size_t)
inline size_t fnv1a(const char* s)
{
    return fnv1_hasher::hash(s);
}

inline size_t fnv1a(const void* buf, size_t size)
{
    return fnv1_hasher::hash(buf, size);
}

template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    size_t
>::type fnv1a(const String& s)
{
    return fnv1a_hasher::hash(s);
}

// fnv1 (32 bit)
inline uint32 fnv32(const char* s)
{
    return fnv1_32_hasher::hash(s);
}

inline uint32 fnv32(const void* buf, size_t size)
{
    return fnv1_32_hasher::hash(buf, size);
}

template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    uint32
>::type fnv32(const String& s)
{
    return fnv1_32_hasher::hash(s);
}

// fnv1 (64 bit)
inline uint64 fnv64(const char* s)
{
    return fnv1_64_hasher::hash(s);
}

inline uint64 fnv64(const void* buf, size_t size)
{
    return fnv1_64_hasher::hash(buf, size);
}

template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    uint64
>::type fnv64(const String& s)
{
    return fnv1_64_hasher::hash(s);
}

// fnv1a (32 bit)
inline uint32 fnv32a(const char* s)
{
    return fnv1a_32_hasher::hash(s);
}

inline uint32 fnv32a(const void* buf, size_t size)
{
    return fnv1a_32_hasher::hash(buf, size);
}

template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    uint32
>::type fnv32a(const String& s)
{
    return fnv1a_32_hasher::hash(s);
}

// fnv1a (64 bit)
inline uint64 fnv64a(const char* s)
{
    return fnv1a_64_hasher::hash(s);
}

inline uint64 fnv64a(const void* buf, size_t size)
{
    return fnv1a_64_hasher::hash(buf, size);
}

template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    uint64
>::type fnv64a(const String& s)
{
    return fnv1a_64_hasher::hash(s);
}

} // namespace native

#endif
