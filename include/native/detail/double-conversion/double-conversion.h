// !!!
// generated from /native/deps/double-conversion/setup.py
// !!!
// easy compile for double-conversion

// ** double-conversion.h **
// Copyright 2012 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef DOUBLE_CONVERSION_DOUBLE_CONVERSION_H_
#define DOUBLE_CONVERSION_DOUBLE_CONVERSION_H_

#include "native/detail/double-conversion/utils.h"

namespace native
{
namespace double_conversion
{

class DoubleToStringConverter
{
public:
    // When calling ToFixed with a double > 10^kMaxFixedDigitsBeforePoint
    // or a requested_digits parameter > kMaxFixedDigitsAfterPoint then the
    // function returns false.
    static const int kMaxFixedDigitsBeforePoint = 60;
    static const int kMaxFixedDigitsAfterPoint = 60;

    // When calling ToExponential with a requested_digits
    // parameter > kMaxExponentialDigits then the function returns false.
    static const int kMaxExponentialDigits = 120;

    // When calling ToPrecision with a requested_digits
    // parameter < kMinPrecisionDigits or requested_digits > kMaxPrecisionDigits
    // then the function returns false.
    static const int kMinPrecisionDigits = 1;
    static const int kMaxPrecisionDigits = 120;

    enum Flags
    {
        NO_FLAGS = 0,
        EMIT_POSITIVE_EXPONENT_SIGN = 1,
        EMIT_TRAILING_DECIMAL_POINT = 2,
        EMIT_TRAILING_ZERO_AFTER_POINT = 4,
        UNIQUE_ZERO = 8
    };

    // Flags should be a bit-or combination of the possible Flags-enum.
    //  - NO_FLAGS: no special flags.
    //  - EMIT_POSITIVE_EXPONENT_SIGN: when the number is converted into
    //  exponent
    //    form, emits a '+' for positive exponents. Example: 1.2e+2.
    //  - EMIT_TRAILING_DECIMAL_POINT: when the input number is an integer and
    //  is
    //    converted into decimal format then a trailing decimal point is
    //    appended.
    //    Example: 2345.0 is converted to "2345.".
    //  - EMIT_TRAILING_ZERO_AFTER_POINT: in addition to a trailing decimal
    //  point
    //    emits a trailing '0'-character. This flag requires the
    //    EXMIT_TRAILING_DECIMAL_POINT flag.
    //    Example: 2345.0 is converted to "2345.0".
    //  - UNIQUE_ZERO: "-0.0" is converted to "0.0".
    //
    // Infinity symbol and nan_symbol provide the string representation for
    // these
    // special values. If the string is NULL and the special value is
    // encountered
    // then the conversion functions return false.
    //
    // The exponent_character is used in exponential representations. It is
    // usually 'e' or 'E'.
    //
    // When converting to the shortest representation the converter will
    // represent input numbers in decimal format if they are in the interval
    // [10^decimal_in_shortest_low; 10^decimal_in_shortest_high[
    //    (lower boundary included, greater boundary excluded).
    // Example: with decimal_in_shortest_low = -6 and
    //               decimal_in_shortest_high = 21:
    //   ToShortest(0.000001)  -> "0.000001"
    //   ToShortest(0.0000001) -> "1e-7"
    //   ToShortest(111111111111111111111.0)  -> "111111111111111110000"
    //   ToShortest(100000000000000000000.0)  -> "100000000000000000000"
    //   ToShortest(1111111111111111111111.0) -> "1.1111111111111111e+21"
    //
    // When converting to precision mode the converter may add
    // max_leading_padding_zeroes before returning the number in exponential
    // format.
    // Example with max_leading_padding_zeroes_in_precision_mode = 6.
    //   ToPrecision(0.0000012345, 2) -> "0.0000012"
    //   ToPrecision(0.00000012345, 2) -> "1.2e-7"
    // Similarily the converter may add up to
    // max_trailing_padding_zeroes_in_precision_mode in precision mode to avoid
    // returning an exponential representation. A zero added by the
    // EMIT_TRAILING_ZERO_AFTER_POINT flag is counted for this limit.
    // Examples for max_trailing_padding_zeroes_in_precision_mode = 1:
    //   ToPrecision(230.0, 2) -> "230"
    //   ToPrecision(230.0, 2) -> "230."  with EMIT_TRAILING_DECIMAL_POINT.
    //   ToPrecision(230.0, 2) -> "2.3e2" with EMIT_TRAILING_ZERO_AFTER_POINT.
    DoubleToStringConverter(int flags, const char* infinity_symbol,
                            const char* nan_symbol, char exponent_character,
                            int decimal_in_shortest_low,
                            int decimal_in_shortest_high,
                            int max_leading_padding_zeroes_in_precision_mode,
                            int max_trailing_padding_zeroes_in_precision_mode)
        : flags_(flags)
        , infinity_symbol_(infinity_symbol)
        , nan_symbol_(nan_symbol)
        , exponent_character_(exponent_character)
        , decimal_in_shortest_low_(decimal_in_shortest_low)
        , decimal_in_shortest_high_(decimal_in_shortest_high)
        , max_leading_padding_zeroes_in_precision_mode_(
              max_leading_padding_zeroes_in_precision_mode)
        , max_trailing_padding_zeroes_in_precision_mode_(
              max_trailing_padding_zeroes_in_precision_mode)
    {
        // When 'trailing zero after the point' is set, then 'trailing point'
        // must be set too.
        ASSERT(((flags & EMIT_TRAILING_DECIMAL_POINT) != 0) ||
               !((flags & EMIT_TRAILING_ZERO_AFTER_POINT) != 0));
    }

    // Returns a converter following the EcmaScript specification.
    static const DoubleToStringConverter& EcmaScriptConverter();

    // Computes the shortest string of digits that correctly represent the input
    // number. Depending on decimal_in_shortest_low and decimal_in_shortest_high
    // (see constructor) it then either returns a decimal representation, or an
    // exponential representation.
    // Example with decimal_in_shortest_low = -6,
    //              decimal_in_shortest_high = 21,
    //              EMIT_POSITIVE_EXPONENT_SIGN activated, and
    //              EMIT_TRAILING_DECIMAL_POINT deactived:
    //   ToShortest(0.000001)  -> "0.000001"
    //   ToShortest(0.0000001) -> "1e-7"
    //   ToShortest(111111111111111111111.0)  -> "111111111111111110000"
    //   ToShortest(100000000000000000000.0)  -> "100000000000000000000"
    //   ToShortest(1111111111111111111111.0) -> "1.1111111111111111e+21"
    //
    // Note: the conversion may round the output if the returned string
    // is accurate enough to uniquely identify the input-number.
    // For example the most precise representation of the double 9e59 equals
    // "899999999999999918767229449717619953810131273674690656206848", but
    // the converter will return the shorter (but still correct) "9e59".
    //
    // Returns true if the conversion succeeds. The conversion always succeeds
    // except when the input value is special and no infinity_symbol or
    // nan_symbol has been given to the constructor.
    bool ToShortest(double value, StringBuilder* result_builder) const
    {
        return ToShortestIeeeNumber(value, result_builder, SHORTEST);
    }

    // Same as ToShortest, but for single-precision floats.
    bool ToShortestSingle(float value, StringBuilder* result_builder) const
    {
        return ToShortestIeeeNumber(value, result_builder, SHORTEST_SINGLE);
    }

    // Computes a decimal representation with a fixed number of digits after the
    // decimal point. The last emitted digit is rounded.
    //
    // Examples:
    //   ToFixed(3.12, 1) -> "3.1"
    //   ToFixed(3.1415, 3) -> "3.142"
    //   ToFixed(1234.56789, 4) -> "1234.5679"
    //   ToFixed(1.23, 5) -> "1.23000"
    //   ToFixed(0.1, 4) -> "0.1000"
    //   ToFixed(1e30, 2) -> "1000000000000000019884624838656.00"
    //   ToFixed(0.1, 30) -> "0.100000000000000005551115123126"
    //   ToFixed(0.1, 17) -> "0.10000000000000001"
    //
    // If requested_digits equals 0, then the tail of the result depends on
    // the EMIT_TRAILING_DECIMAL_POINT and EMIT_TRAILING_ZERO_AFTER_POINT.
    // Examples, for requested_digits == 0,
    //   let EMIT_TRAILING_DECIMAL_POINT and EMIT_TRAILING_ZERO_AFTER_POINT be
    //    - false and false: then 123.45 -> 123
    //                             0.678 -> 1
    //    - true and false: then 123.45 -> 123.
    //                            0.678 -> 1.
    //    - true and true: then 123.45 -> 123.0
    //                           0.678 -> 1.0
    //
    // Returns true if the conversion succeeds. The conversion always succeeds
    // except for the following cases:
    //   - the input value is special and no infinity_symbol or nan_symbol has
    //     been provided to the constructor,
    //   - 'value' > 10^kMaxFixedDigitsBeforePoint, or
    //   - 'requested_digits' > kMaxFixedDigitsAfterPoint.
    // The last two conditions imply that the result will never contain more
    // than
    // 1 + kMaxFixedDigitsBeforePoint + 1 + kMaxFixedDigitsAfterPoint characters
    // (one additional character for the sign, and one for the decimal point).
    bool ToFixed(double value, int requested_digits,
                 StringBuilder* result_builder) const;

    // Computes a representation in exponential format with requested_digits
    // after the decimal point. The last emitted digit is rounded.
    // If requested_digits equals -1, then the shortest exponential
    // representation
    // is computed.
    //
    // Examples with EMIT_POSITIVE_EXPONENT_SIGN deactivated, and
    //               exponent_character set to 'e'.
    //   ToExponential(3.12, 1) -> "3.1e0"
    //   ToExponential(5.0, 3) -> "5.000e0"
    //   ToExponential(0.001, 2) -> "1.00e-3"
    //   ToExponential(3.1415, -1) -> "3.1415e0"
    //   ToExponential(3.1415, 4) -> "3.1415e0"
    //   ToExponential(3.1415, 3) -> "3.142e0"
    //   ToExponential(123456789000000, 3) -> "1.235e14"
    //   ToExponential(1000000000000000019884624838656.0, -1) -> "1e30"
    //   ToExponential(1000000000000000019884624838656.0, 32) ->
    //                     "1.00000000000000001988462483865600e30"
    //   ToExponential(1234, 0) -> "1e3"
    //
    // Returns true if the conversion succeeds. The conversion always succeeds
    // except for the following cases:
    //   - the input value is special and no infinity_symbol or nan_symbol has
    //     been provided to the constructor,
    //   - 'requested_digits' > kMaxExponentialDigits.
    // The last condition implies that the result will never contain more than
    // kMaxExponentialDigits + 8 characters (the sign, the digit before the
    // decimal point, the decimal point, the exponent character, the
    // exponent's sign, and at most 3 exponent digits).
    bool ToExponential(double value, int requested_digits,
                       StringBuilder* result_builder) const;

    // Computes 'precision' leading digits of the given 'value' and returns them
    // either in exponential or decimal format, depending on
    // max_{leading|trailing}_padding_zeroes_in_precision_mode (given to the
    // constructor).
    // The last computed digit is rounded.
    //
    // Example with max_leading_padding_zeroes_in_precision_mode = 6.
    //   ToPrecision(0.0000012345, 2) -> "0.0000012"
    //   ToPrecision(0.00000012345, 2) -> "1.2e-7"
    // Similarily the converter may add up to
    // max_trailing_padding_zeroes_in_precision_mode in precision mode to avoid
    // returning an exponential representation. A zero added by the
    // EMIT_TRAILING_ZERO_AFTER_POINT flag is counted for this limit.
    // Examples for max_trailing_padding_zeroes_in_precision_mode = 1:
    //   ToPrecision(230.0, 2) -> "230"
    //   ToPrecision(230.0, 2) -> "230."  with EMIT_TRAILING_DECIMAL_POINT.
    //   ToPrecision(230.0, 2) -> "2.3e2" with EMIT_TRAILING_ZERO_AFTER_POINT.
    // Examples for max_trailing_padding_zeroes_in_precision_mode = 3, and no
    //    EMIT_TRAILING_ZERO_AFTER_POINT:
    //   ToPrecision(123450.0, 6) -> "123450"
    //   ToPrecision(123450.0, 5) -> "123450"
    //   ToPrecision(123450.0, 4) -> "123500"
    //   ToPrecision(123450.0, 3) -> "123000"
    //   ToPrecision(123450.0, 2) -> "1.2e5"
    //
    // Returns true if the conversion succeeds. The conversion always succeeds
    // except for the following cases:
    //   - the input value is special and no infinity_symbol or nan_symbol has
    //     been provided to the constructor,
    //   - precision < kMinPericisionDigits
    //   - precision > kMaxPrecisionDigits
    // The last condition implies that the result will never contain more than
    // kMaxPrecisionDigits + 7 characters (the sign, the decimal point, the
    // exponent character, the exponent's sign, and at most 3 exponent digits).
    bool ToPrecision(double value, int precision,
                     StringBuilder* result_builder) const;

    enum DtoaMode
    {
        // Produce the shortest correct representation.
        // For example the output of 0.299999999999999988897 is (the less
        // accurate
        // but correct) 0.3.
        SHORTEST,
        // Same as SHORTEST, but for single-precision floats.
        SHORTEST_SINGLE,
        // Produce a fixed number of digits after the decimal point.
        // For instance fixed(0.1, 4) becomes 0.1000
        // If the input number is big, the output will be big.
        FIXED,
        // Fixed number of digits (independent of the decimal point).
        PRECISION
    };

    // The maximal number of digits that are needed to emit a double in base 10.
    // A higher precision can be achieved by using more digits, but the shortest
    // accurate representation of any double will never use more digits than
    // kBase10MaximalLength.
    // Note that DoubleToAscii null-terminates its input. So the given buffer
    // should be at least kBase10MaximalLength + 1 characters long.
    static const int kBase10MaximalLength = 17;

    // Converts the given double 'v' to ascii. 'v' must not be NaN, +Infinity,
    // or
    // -Infinity. In SHORTEST_SINGLE-mode this restriction also applies to 'v'
    // after it has been casted to a single-precision float. That is, in this
    // mode static_cast<float>(v) must not be NaN, +Infinity or -Infinity.
    //
    // The result should be interpreted as buffer * 10^(point-length).
    //
    // The output depends on the given mode:
    //  - SHORTEST: produce the least amount of digits for which the internal
    //   identity requirement is still satisfied. If the digits are printed
    //   (together with the correct exponent) then reading this number will give
    //   'v' again. The buffer will choose the representation that is closest to
    //   'v'. If there are two at the same distance, than the one farther away
    //   from 0 is chosen (halfway cases - ending with 5 - are rounded up).
    //   In this mode the 'requested_digits' parameter is ignored.
    //  - SHORTEST_SINGLE: same as SHORTEST but with single-precision.
    //  - FIXED: produces digits necessary to print a given number with
    //   'requested_digits' digits after the decimal point. The produced digits
    //   might be too short in which case the caller has to fill the remainder
    //   with '0's.
    //   Example: toFixed(0.001, 5) is allowed to return buffer="1", point=-2.
    //   Halfway cases are rounded towards +/-Infinity (away from 0). The call
    //   toFixed(0.15, 2) thus returns buffer="2", point=0.
    //   The returned buffer may contain digits that would be truncated from the
    //   shortest representation of the input.
    //  - PRECISION: produces 'requested_digits' where the first digit is not
    //  '0'.
    //   Even though the length of produced digits usually equals
    //   'requested_digits', the function is allowed to return fewer digits, in
    //   which case the caller has to fill the missing digits with '0's.
    //   Halfway cases are again rounded away from 0.
    // DoubleToAscii expects the given buffer to be big enough to hold all
    // digits and a terminating null-character. In SHORTEST-mode it expects a
    // buffer of at least kBase10MaximalLength + 1. In all other modes the
    // requested_digits parameter and the padding-zeroes limit the size of the
    // output. Don't forget the decimal point, the exponent character and the
    // terminating null-character when computing the maximal output size.
    // The given length is only used in debug mode to ensure the buffer is big
    // enough.
    static void DoubleToAscii(double v, DtoaMode mode, int requested_digits,
                              char* buffer, int buffer_length, bool* sign,
                              int* length, int* point);

private:
    // Implementation for ToShortest and ToShortestSingle.
    bool ToShortestIeeeNumber(double value, StringBuilder* result_builder,
                              DtoaMode mode) const;

    // If the value is a special value (NaN or Infinity) constructs the
    // corresponding string using the configured infinity/nan-symbol.
    // If either of them is NULL or the value is not special then the
    // function returns false.
    bool HandleSpecialValues(double value, StringBuilder* result_builder) const;
    // Constructs an exponential representation (i.e. 1.234e56).
    // The given exponent assumes a decimal point after the first decimal digit.
    void CreateExponentialRepresentation(const char* decimal_digits, int length,
                                         int exponent,
                                         StringBuilder* result_builder) const;
    // Creates a decimal representation (i.e 1234.5678).
    void CreateDecimalRepresentation(const char* decimal_digits, int length,
                                     int decimal_point, int digits_after_point,
                                     StringBuilder* result_builder) const;

    const int flags_;
    const char* const infinity_symbol_;
    const char* const nan_symbol_;
    const char exponent_character_;
    const int decimal_in_shortest_low_;
    const int decimal_in_shortest_high_;
    const int max_leading_padding_zeroes_in_precision_mode_;
    const int max_trailing_padding_zeroes_in_precision_mode_;

    DISALLOW_IMPLICIT_CONSTRUCTORS(DoubleToStringConverter);
};

class StringToDoubleConverter
{
public:
    // Enumeration for allowing octals and ignoring junk when converting
    // strings to numbers.
    enum Flags
    {
        NO_FLAGS = 0,
        ALLOW_HEX = 1,
        ALLOW_OCTALS = 2,
        ALLOW_TRAILING_JUNK = 4,
        ALLOW_LEADING_SPACES = 8,
        ALLOW_TRAILING_SPACES = 16,
        ALLOW_SPACES_AFTER_SIGN = 32
    };

    // Flags should be a bit-or combination of the possible Flags-enum.
    //  - NO_FLAGS: no special flags.
    //  - ALLOW_HEX: recognizes the prefix "0x". Hex numbers may only be
    //  integers.
    //      Ex: StringToDouble("0x1234") -> 4660.0
    //          In StringToDouble("0x1234.56") the characters ".56" are trailing
    //          junk. The result of the call is hence dependent on
    //          the ALLOW_TRAILING_JUNK flag and/or the junk value.
    //      With this flag "0x" is a junk-string. Even with ALLOW_TRAILING_JUNK,
    //      the string will not be parsed as "0" followed by junk.
    //
    //  - ALLOW_OCTALS: recognizes the prefix "0" for octals:
    //      If a sequence of octal digits starts with '0', then the number is
    //      read as octal integer. Octal numbers may only be integers.
    //      Ex: StringToDouble("01234") -> 668.0
    //          StringToDouble("012349") -> 12349.0  // Not a sequence of octal
    //                                               // digits.
    //          In StringToDouble("01234.56") the characters ".56" are trailing
    //          junk. The result of the call is hence dependent on
    //          the ALLOW_TRAILING_JUNK flag and/or the junk value.
    //          In StringToDouble("01234e56") the characters "e56" are trailing
    //          junk, too.
    //  - ALLOW_TRAILING_JUNK: ignore trailing characters that are not part of
    //      a double literal.
    //  - ALLOW_LEADING_SPACES: skip over leading whitespace, including spaces,
    //                          new-lines, and tabs.
    //  - ALLOW_TRAILING_SPACES: ignore trailing whitespace.
    //  - ALLOW_SPACES_AFTER_SIGN: ignore whitespace after the sign.
    //       Ex: StringToDouble("-   123.2") -> -123.2.
    //           StringToDouble("+   123.2") -> 123.2
    //
    // empty_string_value is returned when an empty string is given as input.
    // If ALLOW_LEADING_SPACES or ALLOW_TRAILING_SPACES are set, then a string
    // containing only spaces is converted to the 'empty_string_value', too.
    //
    // junk_string_value is returned when
    //  a) ALLOW_TRAILING_JUNK is not set, and a junk character (a character not
    //     part of a double-literal) is found.
    //  b) ALLOW_TRAILING_JUNK is set, but the string does not start with a
    //     double literal.
    //
    // infinity_symbol and nan_symbol are strings that are used to detect
    // inputs that represent infinity and NaN. They can be null, in which case
    // they are ignored.
    // The conversion routine first reads any possible signs. Then it compares
    // the
    // following character of the input-string with the first character of
    // the infinity, and nan-symbol. If either matches, the function assumes,
    // that
    // a match has been found, and expects the following input characters to
    // match
    // the remaining characters of the special-value symbol.
    // This means that the following restrictions apply to special-value
    // symbols:
    //  - they must not start with signs ('+', or '-'),
    //  - they must not have the same first character.
    //  - they must not start with digits.
    //
    // Examples:
    //  flags = ALLOW_HEX | ALLOW_TRAILING_JUNK,
    //  empty_string_value = 0.0,
    //  junk_string_value = NaN,
    //  infinity_symbol = "infinity",
    //  nan_symbol = "nan":
    //    StringToDouble("0x1234") -> 4660.0.
    //    StringToDouble("0x1234K") -> 4660.0.
    //    StringToDouble("") -> 0.0  // empty_string_value.
    //    StringToDouble(" ") -> NaN  // junk_string_value.
    //    StringToDouble(" 1") -> NaN  // junk_string_value.
    //    StringToDouble("0x") -> NaN  // junk_string_value.
    //    StringToDouble("-123.45") -> -123.45.
    //    StringToDouble("--123.45") -> NaN  // junk_string_value.
    //    StringToDouble("123e45") -> 123e45.
    //    StringToDouble("123E45") -> 123e45.
    //    StringToDouble("123e+45") -> 123e45.
    //    StringToDouble("123E-45") -> 123e-45.
    //    StringToDouble("123e") -> 123.0  // trailing junk ignored.
    //    StringToDouble("123e-") -> 123.0  // trailing junk ignored.
    //    StringToDouble("+NaN") -> NaN  // NaN string literal.
    //    StringToDouble("-infinity") -> -inf.  // infinity literal.
    //    StringToDouble("Infinity") -> NaN  // junk_string_value.
    //
    //  flags = ALLOW_OCTAL | ALLOW_LEADING_SPACES,
    //  empty_string_value = 0.0,
    //  junk_string_value = NaN,
    //  infinity_symbol = NULL,
    //  nan_symbol = NULL:
    //    StringToDouble("0x1234") -> NaN  // junk_string_value.
    //    StringToDouble("01234") -> 668.0.
    //    StringToDouble("") -> 0.0  // empty_string_value.
    //    StringToDouble(" ") -> 0.0  // empty_string_value.
    //    StringToDouble(" 1") -> 1.0
    //    StringToDouble("0x") -> NaN  // junk_string_value.
    //    StringToDouble("0123e45") -> NaN  // junk_string_value.
    //    StringToDouble("01239E45") -> 1239e45.
    //    StringToDouble("-infinity") -> NaN  // junk_string_value.
    //    StringToDouble("NaN") -> NaN  // junk_string_value.
    StringToDoubleConverter(int flags, double empty_string_value,
                            double junk_string_value,
                            const char* infinity_symbol, const char* nan_symbol)
        : flags_(flags)
        , empty_string_value_(empty_string_value)
        , junk_string_value_(junk_string_value)
        , infinity_symbol_(infinity_symbol)
        , nan_symbol_(nan_symbol)
    {
    }

    // Performs the conversion.
    // The output parameter 'processed_characters_count' is set to the number
    // of characters that have been processed to read the number.
    // Spaces than are processed with ALLOW_{LEADING|TRAILING}_SPACES are
    // included
    // in the 'processed_characters_count'. Trailing junk is never included.
    double StringToDouble(const char* buffer, int length,
                          int* processed_characters_count) const;

    // Same as StringToDouble above but for 16 bit characters.
    double StringToDouble(const uc16* buffer, int length,
                          int* processed_characters_count) const;

    // Same as StringToDouble but reads a float.
    // Note that this is not equivalent to
    // static_cast<float>(StringToDouble(...))
    // due to potential double-rounding.
    float StringToFloat(const char* buffer, int length,
                        int* processed_characters_count) const;

    // Same as StringToFloat above but for 16 bit characters.
    float StringToFloat(const uc16* buffer, int length,
                        int* processed_characters_count) const;

private:
    const int flags_;
    const double empty_string_value_;
    const double junk_string_value_;
    const char* const infinity_symbol_;
    const char* const nan_symbol_;

    template <class Iterator>
    double StringToIeee(Iterator start_pointer, int length, bool read_as_double,
                        int* processed_characters_count) const;

    DISALLOW_IMPLICIT_CONSTRUCTORS(StringToDoubleConverter);
};

} // namespace double_conversion
} // namespace native

// ** double-conversion.cc **
// Copyright 2010 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <limits.h>
#include <math.h>

#include "native/detail/double-conversion/double-conversion.h"

#include "native/detail/double-conversion/bignum-dtoa.h"
#include "native/detail/double-conversion/fast-dtoa.h"
#include "native/detail/double-conversion/fixed-dtoa.h"
#include "native/detail/double-conversion/ieee.h"
#include "native/detail/double-conversion/strtod.h"
#include "native/detail/double-conversion/utils.h"

namespace native
{
namespace double_conversion
{

inline const DoubleToStringConverter&
DoubleToStringConverter::EcmaScriptConverter()
{
    int flags = UNIQUE_ZERO | EMIT_POSITIVE_EXPONENT_SIGN;
    static DoubleToStringConverter converter(flags, "Infinity", "NaN", 'e', -6,
                                             21, 6, 0);
    return converter;
}

inline bool DoubleToStringConverter::HandleSpecialValues(
    double value, StringBuilder* result_builder) const
{
    Double double_inspect(value);
    if (double_inspect.IsInfinite())
    {
        if (infinity_symbol_ == NULL)
            return false;
        if (value < 0)
        {
            result_builder->AddCharacter('-');
        }
        result_builder->AddString(infinity_symbol_);
        return true;
    }
    if (double_inspect.IsNan())
    {
        if (nan_symbol_ == NULL)
            return false;
        result_builder->AddString(nan_symbol_);
        return true;
    }
    return false;
}

inline void DoubleToStringConverter::CreateExponentialRepresentation(
    const char* decimal_digits, int length, int exponent,
    StringBuilder* result_builder) const
{
    ASSERT(length != 0);
    result_builder->AddCharacter(decimal_digits[0]);
    if (length != 1)
    {
        result_builder->AddCharacter('.');
        result_builder->AddSubstring(&decimal_digits[1], length - 1);
    }
    result_builder->AddCharacter(exponent_character_);
    if (exponent < 0)
    {
        result_builder->AddCharacter('-');
        exponent = -exponent;
    }
    else
    {
        if ((flags_ & EMIT_POSITIVE_EXPONENT_SIGN) != 0)
        {
            result_builder->AddCharacter('+');
        }
    }
    if (exponent == 0)
    {
        result_builder->AddCharacter('0');
        return;
    }
    ASSERT(exponent < 1e4);
    const int kMaxExponentLength = 5;
    char buffer[kMaxExponentLength + 1];
    buffer[kMaxExponentLength] = '\0';
    int first_char_pos = kMaxExponentLength;
    while (exponent > 0)
    {
        buffer[--first_char_pos] = '0' + (exponent % 10);
        exponent /= 10;
    }
    result_builder->AddSubstring(&buffer[first_char_pos],
                                 kMaxExponentLength - first_char_pos);
}

inline void DoubleToStringConverter::CreateDecimalRepresentation(
    const char* decimal_digits, int length, int decimal_point,
    int digits_after_point, StringBuilder* result_builder) const
{
    // Create a representation that is padded with zeros if needed.
    if (decimal_point <= 0)
    {
        // "0.00000decimal_rep".
        result_builder->AddCharacter('0');
        if (digits_after_point > 0)
        {
            result_builder->AddCharacter('.');
            result_builder->AddPadding('0', -decimal_point);
            ASSERT(length <= digits_after_point - (-decimal_point));
            result_builder->AddSubstring(decimal_digits, length);
            int remaining_digits =
                digits_after_point - (-decimal_point) - length;
            result_builder->AddPadding('0', remaining_digits);
        }
    }
    else if (decimal_point >= length)
    {
        // "decimal_rep0000.00000" or "decimal_rep.0000"
        result_builder->AddSubstring(decimal_digits, length);
        result_builder->AddPadding('0', decimal_point - length);
        if (digits_after_point > 0)
        {
            result_builder->AddCharacter('.');
            result_builder->AddPadding('0', digits_after_point);
        }
    }
    else
    {
        // "decima.l_rep000"
        ASSERT(digits_after_point > 0);
        result_builder->AddSubstring(decimal_digits, decimal_point);
        result_builder->AddCharacter('.');
        ASSERT(length - decimal_point <= digits_after_point);
        result_builder->AddSubstring(&decimal_digits[decimal_point],
                                     length - decimal_point);
        int remaining_digits = digits_after_point - (length - decimal_point);
        result_builder->AddPadding('0', remaining_digits);
    }
    if (digits_after_point == 0)
    {
        if ((flags_ & EMIT_TRAILING_DECIMAL_POINT) != 0)
        {
            result_builder->AddCharacter('.');
        }
        if ((flags_ & EMIT_TRAILING_ZERO_AFTER_POINT) != 0)
        {
            result_builder->AddCharacter('0');
        }
    }
}

inline bool DoubleToStringConverter::ToShortestIeeeNumber(
    double value, StringBuilder* result_builder,
    DoubleToStringConverter::DtoaMode mode) const
{
    ASSERT(mode == SHORTEST || mode == SHORTEST_SINGLE);
    if (Double(value).IsSpecial())
    {
        return HandleSpecialValues(value, result_builder);
    }

    int decimal_point;
    bool sign;
    const int kDecimalRepCapacity = kBase10MaximalLength + 1;
    char decimal_rep[kDecimalRepCapacity];
    int decimal_rep_length;

    DoubleToAscii(value, mode, 0, decimal_rep, kDecimalRepCapacity, &sign,
                  &decimal_rep_length, &decimal_point);

    bool unique_zero = (flags_ & UNIQUE_ZERO) != 0;
    if (sign && (value != 0.0 || !unique_zero))
    {
        result_builder->AddCharacter('-');
    }

    int exponent = decimal_point - 1;
    if ((decimal_in_shortest_low_ <= exponent) &&
        (exponent < decimal_in_shortest_high_))
    {
        CreateDecimalRepresentation(
            decimal_rep, decimal_rep_length, decimal_point,
            Max(0, decimal_rep_length - decimal_point), result_builder);
    }
    else
    {
        CreateExponentialRepresentation(decimal_rep, decimal_rep_length,
                                        exponent, result_builder);
    }
    return true;
}

inline bool
DoubleToStringConverter::ToFixed(double value, int requested_digits,
                                 StringBuilder* result_builder) const
{
    ASSERT(kMaxFixedDigitsBeforePoint == 60);
    const double kFirstNonFixed = 1e60;

    if (Double(value).IsSpecial())
    {
        return HandleSpecialValues(value, result_builder);
    }

    if (requested_digits > kMaxFixedDigitsAfterPoint)
        return false;
    if (value >= kFirstNonFixed || value <= -kFirstNonFixed)
        return false;

    // Find a sufficiently precise decimal representation of n.
    int decimal_point;
    bool sign;
    // Add space for the '\0' byte.
    const int kDecimalRepCapacity =
        kMaxFixedDigitsBeforePoint + kMaxFixedDigitsAfterPoint + 1;
    char decimal_rep[kDecimalRepCapacity];
    int decimal_rep_length;
    DoubleToAscii(value, FIXED, requested_digits, decimal_rep,
                  kDecimalRepCapacity, &sign, &decimal_rep_length,
                  &decimal_point);

    bool unique_zero = ((flags_ & UNIQUE_ZERO) != 0);
    if (sign && (value != 0.0 || !unique_zero))
    {
        result_builder->AddCharacter('-');
    }

    CreateDecimalRepresentation(decimal_rep, decimal_rep_length, decimal_point,
                                requested_digits, result_builder);
    return true;
}

inline bool
DoubleToStringConverter::ToExponential(double value, int requested_digits,
                                       StringBuilder* result_builder) const
{
    if (Double(value).IsSpecial())
    {
        return HandleSpecialValues(value, result_builder);
    }

    if (requested_digits < -1)
        return false;
    if (requested_digits > kMaxExponentialDigits)
        return false;

    int decimal_point;
    bool sign;
    // Add space for digit before the decimal point and the '\0' character.
    const int kDecimalRepCapacity = kMaxExponentialDigits + 2;
    ASSERT(kDecimalRepCapacity > kBase10MaximalLength);
    char decimal_rep[kDecimalRepCapacity];
    int decimal_rep_length;

    if (requested_digits == -1)
    {
        DoubleToAscii(value, SHORTEST, 0, decimal_rep, kDecimalRepCapacity,
                      &sign, &decimal_rep_length, &decimal_point);
    }
    else
    {
        DoubleToAscii(value, PRECISION, requested_digits + 1, decimal_rep,
                      kDecimalRepCapacity, &sign, &decimal_rep_length,
                      &decimal_point);
        ASSERT(decimal_rep_length <= requested_digits + 1);

        for (int i = decimal_rep_length; i < requested_digits + 1; ++i)
        {
            decimal_rep[i] = '0';
        }
        decimal_rep_length = requested_digits + 1;
    }

    bool unique_zero = ((flags_ & UNIQUE_ZERO) != 0);
    if (sign && (value != 0.0 || !unique_zero))
    {
        result_builder->AddCharacter('-');
    }

    int exponent = decimal_point - 1;
    CreateExponentialRepresentation(decimal_rep, decimal_rep_length, exponent,
                                    result_builder);
    return true;
}

inline bool
DoubleToStringConverter::ToPrecision(double value, int precision,
                                     StringBuilder* result_builder) const
{
    if (Double(value).IsSpecial())
    {
        return HandleSpecialValues(value, result_builder);
    }

    if (precision < kMinPrecisionDigits || precision > kMaxPrecisionDigits)
    {
        return false;
    }

    // Find a sufficiently precise decimal representation of n.
    int decimal_point;
    bool sign;
    // Add one for the terminating null character.
    const int kDecimalRepCapacity = kMaxPrecisionDigits + 1;
    char decimal_rep[kDecimalRepCapacity];
    int decimal_rep_length;

    DoubleToAscii(value, PRECISION, precision, decimal_rep, kDecimalRepCapacity,
                  &sign, &decimal_rep_length, &decimal_point);
    ASSERT(decimal_rep_length <= precision);

    bool unique_zero = ((flags_ & UNIQUE_ZERO) != 0);
    if (sign && (value != 0.0 || !unique_zero))
    {
        result_builder->AddCharacter('-');
    }

    // The exponent if we print the number as x.xxeyyy. That is with the
    // decimal point after the first digit.
    int exponent = decimal_point - 1;

    int extra_zero = ((flags_ & EMIT_TRAILING_ZERO_AFTER_POINT) != 0) ? 1 : 0;
    if ((-decimal_point + 1 > max_leading_padding_zeroes_in_precision_mode_) ||
        (decimal_point - precision + extra_zero >
         max_trailing_padding_zeroes_in_precision_mode_))
    {
        // Fill buffer to contain 'precision' digits.
        // Usually the buffer is already at the correct length, but
        // 'DoubleToAscii'
        // is allowed to return less characters.
        for (int i = decimal_rep_length; i < precision; ++i)
        {
            decimal_rep[i] = '0';
        }

        CreateExponentialRepresentation(decimal_rep, precision, exponent,
                                        result_builder);
    }
    else
    {
        CreateDecimalRepresentation(
            decimal_rep, decimal_rep_length, decimal_point,
            Max(0, precision - decimal_point), result_builder);
    }
    return true;
}

inline BignumDtoaMode
DtoaToBignumDtoaMode(DoubleToStringConverter::DtoaMode dtoa_mode)
{
    switch (dtoa_mode)
    {
        case DoubleToStringConverter::SHORTEST:
            return BIGNUM_DTOA_SHORTEST;
        case DoubleToStringConverter::SHORTEST_SINGLE:
            return BIGNUM_DTOA_SHORTEST_SINGLE;
        case DoubleToStringConverter::FIXED:
            return BIGNUM_DTOA_FIXED;
        case DoubleToStringConverter::PRECISION:
            return BIGNUM_DTOA_PRECISION;
        default:
            UNREACHABLE();
    }
}

inline void DoubleToStringConverter::DoubleToAscii(
    double v, DtoaMode mode, int requested_digits, char* buffer,
    int buffer_length, bool* sign, int* length, int* point)
{
    Vector<char> vector(buffer, buffer_length);
    ASSERT(!Double(v).IsSpecial());
    ASSERT(mode == SHORTEST || mode == SHORTEST_SINGLE ||
           requested_digits >= 0);

    if (Double(v).Sign() < 0)
    {
        *sign = true;
        v = -v;
    }
    else
    {
        *sign = false;
    }

    if (mode == PRECISION && requested_digits == 0)
    {
        vector[0] = '\0';
        *length = 0;
        return;
    }

    if (v == 0)
    {
        vector[0] = '0';
        vector[1] = '\0';
        *length = 1;
        *point = 1;
        return;
    }

    bool fast_worked;
    switch (mode)
    {
        case SHORTEST:
            fast_worked =
                FastDtoa(v, FAST_DTOA_SHORTEST, 0, vector, length, point);
            break;
        case SHORTEST_SINGLE:
            fast_worked = FastDtoa(v, FAST_DTOA_SHORTEST_SINGLE, 0, vector,
                                   length, point);
            break;
        case FIXED:
            fast_worked =
                FastFixedDtoa(v, requested_digits, vector, length, point);
            break;
        case PRECISION:
            fast_worked = FastDtoa(v, FAST_DTOA_PRECISION, requested_digits,
                                   vector, length, point);
            break;
        default:
            fast_worked = false;
            UNREACHABLE();
    }
    if (fast_worked)
        return;

    // If the fast dtoa didn't succeed use the slower bignum version.
    BignumDtoaMode bignum_mode = DtoaToBignumDtoaMode(mode);
    BignumDtoa(v, bignum_mode, requested_digits, vector, length, point);
    vector[*length] = '\0';
}

// Consumes the given substring from the iterator.
// Returns false, if the substring does not match.
template <class Iterator>
inline bool ConsumeSubString(Iterator* current, Iterator end,
                             const char* substring)
{
    ASSERT(**current == *substring);
    for (substring++; *substring != '\0'; substring++)
    {
        ++*current;
        if (*current == end || **current != *substring)
            return false;
    }
    ++*current;
    return true;
}

// Maximum number of significant digits in decimal representation.
// The longest possible double in decimal representation is
// (2^53 - 1) * 2 ^ -1074 that is (2 ^ 53 - 1) * 5 ^ 1074 / 10 ^ 1074
// (768 digits). If we parse a number whose first digits are equal to a
// mean of 2 adjacent doubles (that could have up to 769 digits) the result
// must be rounded to the bigger one unless the tail consists of zeros, so
// we don't need to preserve all the digits.
const int kMaxSignificantDigits = 772;

static const char kWhitespaceTable7[] = {32, 13, 10, 9, 11, 12};
static const int kWhitespaceTable7Length = ARRAY_SIZE(kWhitespaceTable7);

static const uc16 kWhitespaceTable16[] = {
    160,  8232, 8233, 5760, 6158, 8192, 8193, 8194, 8195,  8196,
    8197, 8198, 8199, 8200, 8201, 8202, 8239, 8287, 12288, 65279};
static const int kWhitespaceTable16Length = ARRAY_SIZE(kWhitespaceTable16);

inline bool isWhitespace(int x)
{
    if (x < 128)
    {
        for (int i = 0; i < kWhitespaceTable7Length; i++)
        {
            if (kWhitespaceTable7[i] == x)
                return true;
        }
    }
    else
    {
        for (int i = 0; i < kWhitespaceTable16Length; i++)
        {
            if (kWhitespaceTable16[i] == x)
                return true;
        }
    }
    return false;
}

// Returns true if a nonspace found and false if the end has reached.
template <class Iterator>
inline bool AdvanceToNonspace(Iterator* current, Iterator end)
{
    while (*current != end)
    {
        if (!isWhitespace(**current))
            return true;
        ++*current;
    }
    return false;
}

inline bool isDigit(int x, int radix)
{
    return (x >= '0' && x <= '9' && x < '0' + radix) ||
           (radix > 10 && x >= 'a' && x < 'a' + radix - 10) ||
           (radix > 10 && x >= 'A' && x < 'A' + radix - 10);
}

inline double SignedZero(bool sign) { return sign ? -0.0 : 0.0; }

// Returns true if 'c' is a decimal digit that is valid for the given radix.
//
// The function is small and could be inlined, but VS2012 emitted a warning
// because it constant-propagated the radix and concluded that the last
// condition was always true. By moving it into a separate function the
// compiler wouldn't warn anymore.
inline bool IsDecimalDigitForRadix(int c, int radix)
{
    return '0' <= c && c <= '9' && (c - '0') < radix;
}

// Returns true if 'c' is a character digit that is valid for the given radix.
// The 'a_character' should be 'a' or 'A'.
//
// The function is small and could be inlined, but VS2012 emitted a warning
// because it constant-propagated the radix and concluded that the first
// condition was always false. By moving it into a separate function the
// compiler wouldn't warn anymore.
inline bool IsCharacterDigitForRadix(int c, int radix, char a_character)
{
    return radix > 10 && c >= a_character && c < a_character + radix - 10;
}

// Parsing integers with radix 2, 4, 8, 16, 32. Assumes current != end.
template <int radix_log_2, class Iterator>
inline double RadixStringToIeee(Iterator* current, Iterator end, bool sign,
                                bool allow_trailing_junk,
                                double junk_string_value, bool read_as_double,
                                bool* result_is_junk)
{
    ASSERT(*current != end);

    const int kDoubleSize = Double::kSignificandSize;
    const int kSingleSize = Single::kSignificandSize;
    const int kSignificandSize = read_as_double ? kDoubleSize : kSingleSize;

    *result_is_junk = true;

    // Skip leading 0s.
    while (**current == '0')
    {
        ++(*current);
        if (*current == end)
        {
            *result_is_junk = false;
            return SignedZero(sign);
        }
    }

    int64_t number = 0;
    int exponent = 0;
    const int radix = (1 << radix_log_2);

    do
    {
        int digit;
        if (IsDecimalDigitForRadix(**current, radix))
        {
            digit = static_cast<char>(**current) - '0';
        }
        else if (IsCharacterDigitForRadix(**current, radix, 'a'))
        {
            digit = static_cast<char>(**current) - 'a' + 10;
        }
        else if (IsCharacterDigitForRadix(**current, radix, 'A'))
        {
            digit = static_cast<char>(**current) - 'A' + 10;
        }
        else
        {
            if (allow_trailing_junk || !AdvanceToNonspace(current, end))
            {
                break;
            }
            else
            {
                return junk_string_value;
            }
        }

        number = number * radix + digit;
        int overflow = static_cast<int>(number >> kSignificandSize);
        if (overflow != 0)
        {
            // Overflow occurred. Need to determine which direction to round the
            // result.
            int overflow_bits_count = 1;
            while (overflow > 1)
            {
                overflow_bits_count++;
                overflow >>= 1;
            }

            int dropped_bits_mask = ((1 << overflow_bits_count) - 1);
            int dropped_bits = static_cast<int>(number) & dropped_bits_mask;
            number >>= overflow_bits_count;
            exponent = overflow_bits_count;

            bool zero_tail = true;
            for (;;)
            {
                ++(*current);
                if (*current == end || !isDigit(**current, radix))
                    break;
                zero_tail = zero_tail && **current == '0';
                exponent += radix_log_2;
            }

            if (!allow_trailing_junk && AdvanceToNonspace(current, end))
            {
                return junk_string_value;
            }

            int middle_value = (1 << (overflow_bits_count - 1));
            if (dropped_bits > middle_value)
            {
                number++; // Rounding up.
            }
            else if (dropped_bits == middle_value)
            {
                // Rounding to even to consistency with decimals: half-way case
                // rounds
                // up if significant part is odd and down otherwise.
                if ((number & 1) != 0 || !zero_tail)
                {
                    number++; // Rounding up.
                }
            }

            // Rounding up may cause overflow.
            if ((number & ((int64_t)1 << kSignificandSize)) != 0)
            {
                exponent++;
                number >>= 1;
            }
            break;
        }
        ++(*current);
    } while (*current != end);

    ASSERT(number < ((int64_t)1 << kSignificandSize));
    ASSERT(static_cast<int64_t>(static_cast<double>(number)) == number);

    *result_is_junk = false;

    if (exponent == 0)
    {
        if (sign)
        {
            if (number == 0)
                return -0.0;
            number = -number;
        }
        return static_cast<double>(number);
    }

    ASSERT(number != 0);
    return Double(DiyFp(number, exponent)).value();
}

template <class Iterator>
inline double
StringToDoubleConverter::StringToIeee(Iterator input, int length,
                                      bool read_as_double,
                                      int* processed_characters_count) const
{
    Iterator current = input;
    Iterator end = input + length;

    *processed_characters_count = 0;

    const bool allow_trailing_junk = (flags_ & ALLOW_TRAILING_JUNK) != 0;
    const bool allow_leading_spaces = (flags_ & ALLOW_LEADING_SPACES) != 0;
    const bool allow_trailing_spaces = (flags_ & ALLOW_TRAILING_SPACES) != 0;
    const bool allow_spaces_after_sign =
        (flags_ & ALLOW_SPACES_AFTER_SIGN) != 0;

    // To make sure that iterator dereferencing is valid the following
    // convention is used:
    // 1. Each '++current' statement is followed by check for equality to 'end'.
    // 2. If AdvanceToNonspace returned false then current == end.
    // 3. If 'current' becomes equal to 'end' the function returns or goes to
    // 'parsing_done'.
    // 4. 'current' is not dereferenced after the 'parsing_done' label.
    // 5. Code before 'parsing_done' may rely on 'current != end'.
    if (current == end)
        return empty_string_value_;

    if (allow_leading_spaces || allow_trailing_spaces)
    {
        if (!AdvanceToNonspace(&current, end))
        {
            *processed_characters_count = static_cast<int>(current - input);
            return empty_string_value_;
        }
        if (!allow_leading_spaces && (input != current))
        {
            // No leading spaces allowed, but AdvanceToNonspace moved forward.
            return junk_string_value_;
        }
    }

    // The longest form of simplified number is: "-<significant
    // digits>.1eXXX\0".
    const int kBufferSize = kMaxSignificantDigits + 10;
    char buffer[kBufferSize]; // NOLINT: size is known at compile time.
    int buffer_pos = 0;

    // Exponent will be adjusted if insignificant digits of the integer part
    // or insignificant leading zeros of the fractional part are dropped.
    int exponent = 0;
    int significant_digits = 0;
    int insignificant_digits = 0;
    bool nonzero_digit_dropped = false;

    bool sign = false;

    if (*current == '+' || *current == '-')
    {
        sign = (*current == '-');
        ++current;
        Iterator next_non_space = current;
        // Skip following spaces (if allowed).
        if (!AdvanceToNonspace(&next_non_space, end))
            return junk_string_value_;
        if (!allow_spaces_after_sign && (current != next_non_space))
        {
            return junk_string_value_;
        }
        current = next_non_space;
    }

    if (infinity_symbol_ != NULL)
    {
        if (*current == infinity_symbol_[0])
        {
            if (!ConsumeSubString(&current, end, infinity_symbol_))
            {
                return junk_string_value_;
            }

            if (!(allow_trailing_spaces || allow_trailing_junk) &&
                (current != end))
            {
                return junk_string_value_;
            }
            if (!allow_trailing_junk && AdvanceToNonspace(&current, end))
            {
                return junk_string_value_;
            }

            ASSERT(buffer_pos == 0);
            *processed_characters_count = static_cast<int>(current - input);
            return sign ? -Double::Infinity() : Double::Infinity();
        }
    }

    if (nan_symbol_ != NULL)
    {
        if (*current == nan_symbol_[0])
        {
            if (!ConsumeSubString(&current, end, nan_symbol_))
            {
                return junk_string_value_;
            }

            if (!(allow_trailing_spaces || allow_trailing_junk) &&
                (current != end))
            {
                return junk_string_value_;
            }
            if (!allow_trailing_junk && AdvanceToNonspace(&current, end))
            {
                return junk_string_value_;
            }

            ASSERT(buffer_pos == 0);
            *processed_characters_count = static_cast<int>(current - input);
            return sign ? -Double::NaN() : Double::NaN();
        }
    }

    bool leading_zero = false;
    if (*current == '0')
    {
        ++current;
        if (current == end)
        {
            *processed_characters_count = static_cast<int>(current - input);
            return SignedZero(sign);
        }

        leading_zero = true;

        // It could be hexadecimal value.
        if ((flags_ & ALLOW_HEX) && (*current == 'x' || *current == 'X'))
        {
            ++current;
            if (current == end || !isDigit(*current, 16))
            {
                return junk_string_value_; // "0x".
            }

            bool result_is_junk;
            double result = RadixStringToIeee<4>(
                &current, end, sign, allow_trailing_junk, junk_string_value_,
                read_as_double, &result_is_junk);
            if (!result_is_junk)
            {
                if (allow_trailing_spaces)
                    AdvanceToNonspace(&current, end);
                *processed_characters_count = static_cast<int>(current - input);
            }
            return result;
        }

        // Ignore leading zeros in the integer part.
        while (*current == '0')
        {
            ++current;
            if (current == end)
            {
                *processed_characters_count = static_cast<int>(current - input);
                return SignedZero(sign);
            }
        }
    }

    bool octal = leading_zero && (flags_ & ALLOW_OCTALS) != 0;

    // Copy significant digits of the integer part (if any) to the buffer.
    while (*current >= '0' && *current <= '9')
    {
        if (significant_digits < kMaxSignificantDigits)
        {
            ASSERT(buffer_pos < kBufferSize);
            buffer[buffer_pos++] = static_cast<char>(*current);
            significant_digits++;
            // Will later check if it's an octal in the buffer.
        }
        else
        {
            insignificant_digits++; // Move the digit into the exponential part.
            nonzero_digit_dropped = nonzero_digit_dropped || *current != '0';
        }
        octal = octal && *current < '8';
        ++current;
        if (current == end)
            goto parsing_done;
    }

    if (significant_digits == 0)
    {
        octal = false;
    }

    if (*current == '.')
    {
        if (octal && !allow_trailing_junk)
            return junk_string_value_;
        if (octal)
            goto parsing_done;

        ++current;
        if (current == end)
        {
            if (significant_digits == 0 && !leading_zero)
            {
                return junk_string_value_;
            }
            else
            {
                goto parsing_done;
            }
        }

        if (significant_digits == 0)
        {
            // octal = false;
            // Integer part consists of 0 or is absent. Significant digits start
            // after
            // leading zeros (if any).
            while (*current == '0')
            {
                ++current;
                if (current == end)
                {
                    *processed_characters_count =
                        static_cast<int>(current - input);
                    return SignedZero(sign);
                }
                exponent--; // Move this 0 into the exponent.
            }
        }

        // There is a fractional part.
        // We don't emit a '.', but adjust the exponent instead.
        while (*current >= '0' && *current <= '9')
        {
            if (significant_digits < kMaxSignificantDigits)
            {
                ASSERT(buffer_pos < kBufferSize);
                buffer[buffer_pos++] = static_cast<char>(*current);
                significant_digits++;
                exponent--;
            }
            else
            {
                // Ignore insignificant digits in the fractional part.
                nonzero_digit_dropped =
                    nonzero_digit_dropped || *current != '0';
            }
            ++current;
            if (current == end)
                goto parsing_done;
        }
    }

    if (!leading_zero && exponent == 0 && significant_digits == 0)
    {
        // If leading_zeros is true then the string contains zeros.
        // If exponent < 0 then string was [+-]\.0*...
        // If significant_digits != 0 the string is not equal to 0.
        // Otherwise there are no digits in the string.
        return junk_string_value_;
    }

    // Parse exponential part.
    if (*current == 'e' || *current == 'E')
    {
        if (octal && !allow_trailing_junk)
            return junk_string_value_;
        if (octal)
            goto parsing_done;
        ++current;
        if (current == end)
        {
            if (allow_trailing_junk)
            {
                goto parsing_done;
            }
            else
            {
                return junk_string_value_;
            }
        }
        char exponen_sign = '+';
        if (*current == '+' || *current == '-')
        {
            exponen_sign = static_cast<char>(*current);
            ++current;
            if (current == end)
            {
                if (allow_trailing_junk)
                {
                    goto parsing_done;
                }
                else
                {
                    return junk_string_value_;
                }
            }
        }

        if (current == end || *current < '0' || *current > '9')
        {
            if (allow_trailing_junk)
            {
                goto parsing_done;
            }
            else
            {
                return junk_string_value_;
            }
        }

        const int max_exponent = INT_MAX / 2;
        ASSERT(-max_exponent / 2 <= exponent && exponent <= max_exponent / 2);
        int num = 0;
        do
        {
            // Check overflow.
            int digit = *current - '0';
            if (num >= max_exponent / 10 &&
                !(num == max_exponent / 10 && digit <= max_exponent % 10))
            {
                num = max_exponent;
            }
            else
            {
                num = num * 10 + digit;
            }
            ++current;
        } while (current != end && *current >= '0' && *current <= '9');

        exponent += (exponen_sign == '-' ? -num : num);
    }

    if (!(allow_trailing_spaces || allow_trailing_junk) && (current != end))
    {
        return junk_string_value_;
    }
    if (!allow_trailing_junk && AdvanceToNonspace(&current, end))
    {
        return junk_string_value_;
    }
    if (allow_trailing_spaces)
    {
        AdvanceToNonspace(&current, end);
    }

parsing_done:
    exponent += insignificant_digits;

    if (octal)
    {
        double result;
        bool result_is_junk;
        char* start = buffer;
        result = RadixStringToIeee<3>(&start, buffer + buffer_pos, sign,
                                      allow_trailing_junk, junk_string_value_,
                                      read_as_double, &result_is_junk);
        ASSERT(!result_is_junk);
        *processed_characters_count = static_cast<int>(current - input);
        return result;
    }

    if (nonzero_digit_dropped)
    {
        buffer[buffer_pos++] = '1';
        exponent--;
    }

    ASSERT(buffer_pos < kBufferSize);
    buffer[buffer_pos] = '\0';

    double converted;
    if (read_as_double)
    {
        converted = Strtod(Vector<const char>(buffer, buffer_pos), exponent);
    }
    else
    {
        converted = Strtof(Vector<const char>(buffer, buffer_pos), exponent);
    }
    *processed_characters_count = static_cast<int>(current - input);
    return sign ? -converted : converted;
}

inline double
StringToDoubleConverter::StringToDouble(const char* buffer, int length,
                                        int* processed_characters_count) const
{
    return StringToIeee(buffer, length, true, processed_characters_count);
}

inline double
StringToDoubleConverter::StringToDouble(const uc16* buffer, int length,
                                        int* processed_characters_count) const
{
    return StringToIeee(buffer, length, true, processed_characters_count);
}

inline float
StringToDoubleConverter::StringToFloat(const char* buffer, int length,
                                       int* processed_characters_count) const
{
    return static_cast<float>(
        StringToIeee(buffer, length, false, processed_characters_count));
}

inline float
StringToDoubleConverter::StringToFloat(const uc16* buffer, int length,
                                       int* processed_characters_count) const
{
    return static_cast<float>(
        StringToIeee(buffer, length, false, processed_characters_count));
}

} // namespace double_conversion
} // namespace native
#endif // DOUBLE_CONVERSION_DOUBLE_CONVERSION_H_
