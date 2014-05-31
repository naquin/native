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

#ifndef NATIVE_ENCODING_H__
#define NATIVE_ENCODING_H__

namespace native {

//
// encodings adapted from RapidJSON
// http://code.google.com/p/rapidjson/
//

// UTF-8 encoding
// http://en.wikipedia.org/wiki/UTF-8
// http://tools.ietf.org/html/rfc3629
struct utf8 {
	typedef char char_type;
    typedef unsigned char uchar_type;

    template <typename String>
	static void encode_string(String& str, unsigned codepoint)
	{
        static_assert(sizeof(typename String::value_type) == 1, "");

		if (codepoint <= 0x7F)
		{
			str.push_back(codepoint & 0xFF);
		}
		else if (codepoint <= 0x7FF)
		{
			str.push_back(0xC0 | ((codepoint >> 6) & 0xFF));
			str.push_back(0x80 | ((codepoint & 0x3F)));
		}
		else if (codepoint <= 0xFFFF)
		{
			str.push_back(0xE0 | ((codepoint >> 12) & 0xFF));
			str.push_back(0x80 | ((codepoint >> 6) & 0x3F));
			str.push_back(0x80 | (codepoint & 0x3F));
		}
		else
		{
			assert(codepoint <= 0x10FFFF);
			str.push_back(0xF0 | ((codepoint >> 18) & 0xFF));
			str.push_back(0x80 | ((codepoint >> 12) & 0x3F));
			str.push_back(0x80 | ((codepoint >> 6) & 0x3F));
			str.push_back(0x80 | (codepoint & 0x3F));
		}
	}

    template <typename OStream>
	static void encode_stream(OStream& ostr, unsigned codepoint)
	{
        static_assert(sizeof(typename OStream::char_type) == 1, "");

		if (codepoint <= 0x7F)
		{
			ostr.put(codepoint & 0xFF);
		}
		else if (codepoint <= 0x7FF)
		{
			ostr.put(0xC0 | ((codepoint >> 6) & 0xFF));
			ostr.put(0x80 | ((codepoint & 0x3F)));
		}
		else if (codepoint <= 0xFFFF)
		{
			ostr.put(0xE0 | ((codepoint >> 12) & 0xFF));
			ostr.put(0x80 | ((codepoint >> 6) & 0x3F));
			ostr.put(0x80 | (codepoint & 0x3F));
		}
		else
		{
			assert(codepoint <= 0x10FFFF);
			ostr.put(0xF0 | ((codepoint >> 18) & 0xFF));
			ostr.put(0x80 | ((codepoint >> 12) & 0x3F));
			ostr.put(0x80 | ((codepoint >> 6) & 0x3F));
			ostr.put(0x80 | (codepoint & 0x3F));
		}
	}

    
    template <typename Stream>
	static std::uint32_t decode(Stream& stream)
	{
//        static_assert(sizeof(typename IStream::char_type) == 1, "");

        std::uint32_t codepoint;
		uchar_type c = static_cast<char_type>(stream.get());
		if (!(c & 0x80))
        {
			codepoint = c;
			return codepoint;
		}

		uchar_type type = get_range(c);
		codepoint = (0xFF >> type) & c;
		switch (type) {
		case 2:
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            break;
		case 3:
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            break;
		case 4:
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x50);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            break;
		case 5:
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x10);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            break;
		case 6:
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            break;
		case 10:
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x20);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            break;
		case 11:
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x60);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            c = static_cast<char_type>(stream.get());
            codepoint = (codepoint << 6) | (c & 0x3Fu);
            check_range(c & 0x70);
            break;
		default:
            // TODO: throw?
            return 0;
		}

        return codepoint;
	}

    static void check_range(uchar_type c)
	{
        if ( !get_range(c) )
        {
            // TODO: throw?
        }
	}

	static uchar_type get_range(uchar_type c)
	{
		// Referring to DFA of http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
		// With new mapping 1 -> 0x10, 7 -> 0x20, 9 -> 0x40, such that AND operation can test multiple types.
		static const uchar_type type[] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
			0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
			0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
			0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
			8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
		};
        return type[c];
	}

	template <typename InputByteStream>
	static char_type take_bom(InputByteStream& istr)
    {
		static_assert(sizeof(typename InputByteStream::char_type) == 1, "Expected a 1-byte character");
		char_type c = take(istr);
		if (static_cast<uchar_type>(c) != 0xEFu) return c;
		c = istr.get();
		if ((uchar_type)c != 0xBBu) return c;
		c = istr.get();
		if ((uchar_type)c != 0xBFu) return c;
		c = istr.get();
		return c;
	}

	template <typename InputByteStream>
	static char_type take(InputByteStream& istr)
    {
		static_assert(sizeof(typename InputByteStream::char_type) == 1, "Expected a 1-byte character");
		return istr.get();
	}

	template <typename OutputByteStream>
	static void put_bom(OutputByteStream& ostr)
    {
		static_assert(sizeof(typename OutputByteStream::char_type) == 1, "Expected a 1-byte character");
		ostr.put(0xEFu); ostr.put(0xBBu); ostr.put(0xBFu);
	}

	template <typename OutputByteStream>
	static void put(OutputByteStream& ostr, char_type c)
    {
		static_assert(sizeof(typename OutputByteStream::char_type) == 1, "Expected a 1-byte character");
		ostr.put(static_cast<typename OutputByteStream::char_type>(c));
	}
};

// UTF-16 encoding
// http://en.wikipedia.org/wiki/UTF-16
// http://tools.ietf.org/html/rfc2781
struct utf16 {
    typedef char16_t char_type;

    static_assert(sizeof(char_type) >= 2, "UTF-16 needs at least 2 byte character");


    template <typename String>
    static void encode_string(String& str, unsigned codepoint)
    {
        if (codepoint <= 0xFFFF)
        {
            assert(codepoint < 0xD800 || codepoint > 0xDFFF); // Code point itself cannot be surrogate pair
            str.push_back(static_cast<char_type>(codepoint));
        }
        else
        {
            assert(codepoint <= 0x10FFFF);
            unsigned v = codepoint - 0x10000;
            str.push_back(static_cast<char_type>((v >> 10) | 0xD800));
            str.push_back((v & 0x3FF) | 0xDC00);
        }
    }

    template <typename OStream>
    static void encode_stream(OStream& ostr, unsigned codepoint)
    {
        if (codepoint <= 0xFFFF)
        {
            assert(codepoint < 0xD800 || codepoint > 0xDFFF); // Code point itself cannot be surrogate pair
            ostr.put(static_cast<char_type>(codepoint));
        }
        else
        {
            assert(codepoint <= 0x10FFFF);
            unsigned v = codepoint - 0x10000;
            ostr.put(static_cast<char_type>((v >> 10) | 0xD800));
            ostr.put((v & 0x3FF) | 0xDC00);
        }
    }

    template <typename IStream>
    static std::uint32_t decode(IStream& istr)
    {
        static_assert(sizeof(typename IStream::char_type) == 2, "");
        std::uint32_t codepoint;
        char_type c = istr.get();
        if (c < 0xD800 || c > 0xDFFF)
        {
            codepoint = c;
        }
        else if (c <= 0xDBFF)
        {
            codepoint = (c & 0x3FF) << 10;
            c = istr.get();
            codepoint |= (c & 0x3FF);
            codepoint += 0x10000;
            if ( !(c >= 0xDC00 && c <= 0xDFFF ) )
            {
                // TODO: throw?
            }
        }
        else
        {
            // TODO: throw?
            codepoint = 0;
        }
        return codepoint;
    }
 
};

// UTF-16 little endian encoding.
struct utf16le : utf16 {
    typedef char16_t char_type;

    template <typename InputByteStream>
    static char_type take_bom(InputByteStream& istr)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = take(istr);
        return static_cast<std::uint16_t>(c) == 0xFEFFu ? take(istr) : c;
    }

    template <typename InputByteStream>
    static char_type take(InputByteStream& istr)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = static_cast<unsigned char>(istr.get());
        c |= static_cast<unsigned char>(istr.get()) << 8;
        return c;
    }

    template <typename OutputByteStream>
    static void put_bom(OutputByteStream& ostr)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put(0xFFu);
        ostr.put(0xFEu);
    }

    template <typename OutputByteStream>
    static void put(OutputByteStream& ostr, char_type c)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put(c & 0xFFu);
        ostr.put((c >> 8) & 0xFFu);
    }
};



// UTF-16 big endian encoding.
struct utf16be : utf16 {
    typedef char16_t char_type;

    template <typename InputByteStream>
    static char_type take_bom(InputByteStream& istr)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = take(istr);
        return static_cast<unsigned short>(c == 0xFEFFu ? take(istr) : c);
    }

    template <typename InputByteStream>
    static char_type take(InputByteStream& istr)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = static_cast<unsigned char>(istr.get()) << 8;
        c |= static_cast<unsigned char>(istr.get());
        return c;
    }

    template <typename OutputByteStream>
    static void put_bom(OutputByteStream& ostr)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put(0xFEu);
        ostr.put(0xFFu);
    }

    template <typename OutputByteStream>
    static void put(OutputByteStream& ostr, char_type c)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put((c >> 8) & 0xFFu);
        ostr.put(c & 0xFFu);
    }
};


// UTF-32 encoding.
struct utf32 {
	typedef char32_t char_type;

    template <typename String>
    static void encode_string(String& str, unsigned codepoint)
    {
        assert(codepoint <= 0x10FFFF);
        str.push_back(codepoint);
    }

    template <typename OStream>
    static void encode_stream(OStream& ostr, unsigned codepoint)
    {
        assert(codepoint <= 0x10FFFF);
        ostr.put(codepoint);
    }

    template <typename IStream>
    static std::uint32_t decode(IStream& istr)
    {
        static_assert(sizeof(typename IStream::char_type) == 4, "");
        char_type c = istr.get();
        if ( !(c <= 0x10FFFF) )
        {
            // TODO: throw?
            return 0;
        }

        return c;
    }
};

// UTF-32 little endian enocoding.
struct utf32le : utf32 {
	typedef char32_t char_type;

    template <typename InputByteStream>
    static char_type take_bom(InputByteStream& istr)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = take(istr);
        return static_cast<std::uint32_t>(c) == 0x0000FEFFu ? take(istr) : c;
    }

    template <typename InputByteStream>
    static char_type take(InputByteStream& istr)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = static_cast<unsigned char>(istr.get());
        c |= static_cast<unsigned char>(istr.get()) << 8;
        c |= static_cast<unsigned char>(istr.get()) << 16;
        c |= static_cast<unsigned char>(istr.get()) << 24;
        return c;
    }

    template <typename OutputByteStream>
    static void put_bom(OutputByteStream& ostr)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put(0xFFu);
        ostr.put(0xFEu);
        ostr.put(0x00u);
        ostr.put(0x00u);
    }

    template <typename OutputByteStream>
    static void put(OutputByteStream& ostr, char_type c)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put(c & 0xFFu);
        ostr.put((c >> 8) & 0xFFu);
        ostr.put((c >> 16) & 0xFFu);
        ostr.put((c >> 24) & 0xFFu);
    }
};

// UTF-32 big endian encoding.
struct utf32be : utf32 {
	typedef char32_t char_type;

    template <typename InputByteStream>
    static char_type take_bom(InputByteStream& is)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = take(is);
        return static_cast<std::uint32_t>(c) == 0x0000FEFFu ? take(is) : c;
    }

    template <typename InputByteStream>
    static char_type take(InputByteStream& istr)
    {
        static_assert(sizeof(typename InputByteStream::char_type) == 1, "");
        char_type c = static_cast<unsigned char>(istr.get()) << 24;
        c |= static_cast<unsigned char>(istr.get()) << 16;
        c |= static_cast<unsigned char>(istr.get()) << 8;
        c |= static_cast<unsigned char>(istr.get());
        return c;
    }

    template <typename OutputByteStream>
    static void put_bom(OutputByteStream& ostr)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put(0x00u);
        ostr.put(0x00u);
        ostr.put(0xFEu);
        ostr.put(0xFFu);
    }

    template <typename OutputByteStream>
    static void put(OutputByteStream& ostr, char_type c)
    {
        static_assert(sizeof(typename OutputByteStream::char_type) == 1, "");
        ostr.put((c >> 24) & 0xFFu);
        ostr.put((c >> 16) & 0xFFu);
        ostr.put((c >> 8) & 0xFFu);
        ostr.put(c & 0xFFu);
    }
};


// unicode_converter
template<typename SourceEncoding, typename TargetEncoding>
struct unicode_converter {
    typedef SourceEncoding source_encoding_type;
	typedef typename source_encoding_type::char_type source_char_type;

    typedef TargetEncoding target_encoding_type;
	typedef typename target_encoding_type::char_type target_char_type;

	/// Take one Unicode codepoint from source encoding, convert it to target encoding and put it to the output stream.
    template <typename String, typename Stream>
	inline static void convert(String& destination, Stream& stream)
    {
        const auto codepoint =
            source_encoding_type::decode(stream);

		target_encoding_type::encode_string(destination, codepoint);
	}
};

// no need to convert the same encoding
template<typename Encoding>
struct unicode_converter<Encoding, Encoding> {

    template <typename String, typename Stream>
	inline static void convert(String& destination, Stream& stream)
    {
        const auto ch = stream.peek();
        destination.push_back(ch);
        stream.next();
	}
};
    
// deduce encoding type...
template <typename T>
struct encoding;
    
template <>
struct encoding<char>
{
    typedef utf8 type;
};

template <>
struct encoding<char16_t>
{
    typedef utf16 type;
};

template <>
struct encoding<char32_t>
{
    typedef utf32 type;
};

} // namespace native

#endif
