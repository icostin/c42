#ifndef _C42_H_
#define _C42_H_

/**
 *  @mainpage C42 - Common C Code Collection v2
 *
 *  This is a freestanding library implementing various generic functions that
 *  do not depend directly on any OS or external APIs.
 *
 *  It also defines a set of simple interfaces through which the user can
 *  pass OS-dependent services to this library.
 *
 *  @warning This library supports only platforms with 8-bit chars and bytes
 *  so this might not be usable on some embedded system.
 *  @warning The implementation also assumes it runs on a machine using
 *  2's complement format for integers
 */

/*
Headers allowed in a conforming C99 freestanding environment:

#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
*/

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#if CHAR_BIT != 8
#error only platforms with 8-bit chars are supported
#endif

/** @defgroup env Target Environment
 *
 *  @{
 */

#if C42_DOXY // fake branch to control doxygen output

/* C42_STATIC ****************************************************************/
/** c42 library is used/compiled statically.
 */
#define C42_STATIC

/* C42_CALL ******************************************************************/
/** Calling convention used by all functions exposed by this library.  */
#define C42_CALL

/* C42_INLINE ****************************************************************/
/** Prefix macro for inline functions (depends on the compiler). */
#define C42_INLINE

/* C42_LOCAL *****************************************************************/
/** Function modifier to signify the function is not to be accessed from
 *  other modules.
 */
#define C42_LOCAL

/* C42_LIB_EXPORT ************************************************************/
/** Function modifier to specify that the function should be exported by the
 *  library.
 */
#define C42_LIB_EXPORT

/* C42_LIB_IMPORT ************************************************************/
/** Function modifier to specify that the function should be imported from
 *  another library.
 */
#define C42_LIB_IMPORT

/* C42_API *******************************************************************/
/**
 * Function modifier for all functions exported by this library.
 * This is set to one of C42_LIB_EXPORT, C42_LIB_IMPORT or empty, depending
 * on the type of compilation (building the dynamic library, using the
 * dynamic library from user code - typical usage, or building or using the
 * static library).
 */
#define C42_API

/* C42_ARM32 *****************************************************************/
/** Macro defined as non-zero int when target architecture is ARM Arch32 */
#define C42_ARM32 0

/* C42_ARM64 *****************************************************************/
/** Macro defined as non-zero int when target architecture is ARM AArch64 */
#define C42_ARM64 0

/* C42_MIPS ******************************************************************/
/** Macro defined as non-zero int when target architecture is MIPS */
#define C42_MIPS 0

/* C42_AMD64 *****************************************************************/
/** Macro defined as non-zero int when target architecture is AMD64 (x86_64) */
#define C42_AMD64 0

/* C42_IA32 ******************************************************************/
/** Macro defined as non-zero int when target architecture is IA32 (x86) */
#define C42_IA32 0

/* C42_BSLE ******************************************************************/
/** Macro defined as non-zero int when target architecture is
 *  byte-swapped little endian.
 *  Example: 32-bit int 0x11223344 is stored as 0x44, 0x33, 0x22, 0x11.
 */
#define C42_BSLE 0

/* C42_BSBE ******************************************************************/
/** Macro defined as non-zero int when target architecture is
 *  byte-swapped big endian.
 *  Example: 32-bit int 0x11223344 is stored as 0x11, 0x22, 0x33, 0x44.
 */
#define C42_BSBE 0

/* C42_WSLE ******************************************************************/
/** Macro defined as non-zero int when target architecture is
 *  word-swapped little endian.
 *  Word refers here to 16-bit ints because this encoding was created for
 *  some 16-bit architectures (such as PDP-11).
 *  Example: 32-bit int 0x11223344 is stored as 0x22, 0x11, 0x44, 0x33.
 */
#define C42_WSLE 0

/* C42_WSBE ******************************************************************/
/** Macro defined as non-zero int when target architecture is
 *  word-swapped big endian.
 *  Word refers here to 16-bit ints because this encoding was created for
 *  some 16-bit architectures (such as Honeywell 316).
 *  Example: 32-bit int 0x11223344 is stored as 0x33, 0x44, 0x11, 0x22.
 */
#define C42_WSBE 0

#else // enough lying to doxygen; real macros follow

#undef C42_ARM32
#undef C42_ARM64
#undef C42_MIPS
#undef C42_AMD64
#undef C42_IA32
#undef C42_ALPHA
#undef C42_SPARC

#undef C42_BSLE
#undef C42_BSBE
#undef C42_WSLE
#undef C42_WSBE

#if __amd64__ || _M_IX64 || _M_AMD64
#   define C42_AMD64 1
#   define C42_BSLE 1
#elif __i386__ || _M_IX86
#   define C42_IA32 1
#   define C42_BSLE 1
#elif __arm__ || _M_ARM
#   define C42_ARM32 1
#   if __armel__ || _M_ARM || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#       define C42_BSLE 1
#   elif __armeh__ || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#       define C42_BSBE 1
#   else
#       error cannot detect arm endianess
#   endif
#elif __aarch64__
#   define C42_ARM64 1
#   define C42_BSLE 1
#elif __mips__
#   define C42_MIPS 1
#   if __mipsel__
#       define C42_BSLE 1
#   elif __mipseh__
#       define C42_BSBE 1
#   else
#       error cannot detect mips endianess
#   endif
#else
#   error unrecognised architecture
#endif

#if _WIN32 && !_WIN64
#   define C42_CALL __fastcall
#elif __GNUC__ >= 3 && __i386__
#   define C42_CALL __attribute__((regparm(3)))
#else
#   define C42_CALL
#endif

#define C42_INLINE static __inline

#if defined(_WIN32) || defined(__CYGWIN__)
#   define C42_LIB_EXPORT __declspec(dllexport)
#   define C42_LIB_IMPORT __declspec(dllimport)
#   define C42_LOCAL
#elif __GNUC__ >= 4
#   define C42_LIB_IMPORT __attribute__ ((visibility ("default")))
#   define C42_LIB_EXPORT __attribute__ ((visibility ("default")))
#   define C42_LOCAL __attribute__ ((visibility ("hidden")))
#else //if defined(__GNUC__)
#   define C42_LIB_IMPORT
#   define C42_LIB_EXPORT
#   define C42_LOCAL
#endif

#if C42_STATIC
#define C42_API
#elif C42_LIB_BUILD
#define C42_API C42_LIB_EXPORT
#else
#define C42_API C42_LIB_IMPORT
#endif

#endif // done with real macros for target environment

/** @} */

/** @defgroup pmv Primitive operations
 *  @{
 */

/* c42_u8a_copy *************************************************************/
/**
 *  Copies a block of bytes.
 *  @warning the arrays must not overlap.
 */
C42_API void C42_CALL c42_u8a_copy
(
    uint8_t * restrict dest,
    uint8_t const * restrict src,
    size_t len
);

/* c42_u8a_set **************************************************************/
/**
 *  Sets all bytes to the value given.
 */
C42_API void C42_CALL c42_u8a_set
(
    uint8_t * restrict dest,
    uint_fast8_t value,
    size_t len
);

/* C42_VAR_CLEAR ************************************************************/
/**
 *  Clears all bits of given variable.
 */
#define C42_VAR_CLEAR(_v) (c42_u8a_set((uint8_t *) &(_v), 0, sizeof((_v))))

/* C42_VAR_SET_BITS *********************************************************/
/**
 *  Sets to 1 all bits of given variable.
 */
#define C42_VAR_SET_BITS(_v) (c42_u8a_set((uint8_t *) &(_v), 0xFF, sizeof((_v))))

/* c42_u8a_cmp **************************************************************/
/**
 *  Compares bytes.
 *  @param a first byte array of length @a len
 *  @param b second byte array of length @a len
 *  @param len number of bytes in each array
 *  Bytes are compared one by one as unsigned 8-bit integers.
 *  @returns    -1  @a a is less than @a b
 *  @returns     0  @a a is equal to @a b
 *  @returns    +1  @a a is greater than @a b
 */
C42_API int C42_CALL c42_u8a_cmp
(
    uint8_t const * a,
    uint8_t const * b,
    size_t len
);

/* c42_u8a_equal ************************************************************/
/**
 *  Returns true if the given byte arrays of same length have the same content.
 */
#define C42_U8A_EQUAL(_a, _b, _l) (c42_u8a_cmp((_a), (_b), (_l)) == 0)

/* C42_U8A_EQLIT ************************************************************/
/**
 *  Returns true if the given byte array starts with the same bytes as the
 *  given string literal without the NUL terminator.
 *  Example: C42_U8A_EQLIT(url, "http://")
 */
#define C42_U8A_EQLIT(_a, _lit) \
    (C42_U8A_EQUAL((_a), (uint8_t const *) (_lit), sizeof(_lit) - 1))

/* c42_digit_from_char ******************************************************/
/**
 *  Decodes the digit from a single ASCII/Unicode character
 *  @returns digit value
 *  @retval -1 character is not a valid digit
 */
C42_INLINE int c42_digit_from_char (uint32_t ch, uint_fast8_t radix)
{
    unsigned int digit;
    if (ch >= '0' && ch <= '9') digit = ch - '0';
    else
    {
        ch |= 0x20;
        if (ch >= 'a' && ch <= 'z') digit = ch - 'a' + 10;
        else return -1;
    }
    if (digit >= radix) return -1;
    return digit;
}

/* c42_u8a_hex **************************************************************/
/**
 *  Converts byte array to hex.
 *  @param dest output array; must be @a len * 2 wide
 *  @param src source byte array
 *  @param len length of @a src
 *  @returns dest
 */
C42_API uint8_t * C42_CALL c42_u8a_hex
(
    uint8_t * dest,
    uint8_t const * src,
    size_t len
);

/* c42_u8a_c_escape_len *****************************************************/
/**
 *  Computes the length needed to store the C-escaped form of the given buffer.
 *  The result does not include the terminating NUL-character.
 */
C42_API size_t C42_CALL c42_u8a_c_escape_len
(
    uint8_t const * data,
    size_t len
);

/* c42_u8a_c_escape *********************************************************/
/**
 *  C-Escapes the given byte array.
 *  @returns number of bytes written without counting the terminating NUL;
 *      you can ensure @a out is large enough to hold the output by calling
 *      c42_u8a_c_escape_len() prior to calling this function.
 *  @note this function converts the given byte array
 */
C42_API size_t C42_CALL c42_u8a_c_escape
(
    uint8_t * out,
    uint8_t const * data,
    size_t len
);

/* c42_u8z_copy *************************************************************/
/**
 *  Copies a NUL-terminated byte string returning the destination address for
 *  the terminating NUL.
 *  @warning @a dest must point to a large enough block, or else....
 */
C42_API uint8_t * C42_CALL c42_u8z_copy
(
    uint8_t * dest,
    uint8_t const * src
);

/* c42_u8z_cmp **************************************************************/
/**
 *  Compares two NUL-terminated byte strings.
 *  Bytes are compared one by one as unsigned 8-bit integers.
 *  @returns    -1  @a a is less than @a b
 *  @returns     0  @a a is equal to @a b
 *  @returns    +1  @a a is greater than @a b
 */
C42_API int C42_CALL c42_u8z_cmp
(
    uint8_t const * a,
    uint8_t const * b
);

/* c42_u8z_len **************************************************************/
/**
 *  Returns the length of the given byte string.
 */
C42_API size_t C42_CALL c42_u8z_len
(
    uint8_t const * a
);

/* c42_u8z_scan *************************************************************/
/**
 *  Searches the given byte inside a NUL-terminated byte string.
 *  @returns pointer to first byte in the string holding @a val
 *  @retval NULL if no byte is @a val
 */
C42_API uint8_t * C42_CALL c42_u8z_scan
(
    uint8_t const * str,
    uint8_t val
);

/* C42_U8Z_EQUAL ************************************************************/
/**
 *  Macro returning true if the two NUL-terminated byte strings are equal.
 */
#define C42_U8Z_EQUAL(_a, _b) (!c42_u8z_cmp((_a), (_b)))

/* C42_U8Z_EQLIT ************************************************************/
/**
 *  Compares a NUL-terminated byte string with a NUL-terminated string literal.
 *  Unlike #C42_U8A_EQLIT this will compare the NUL terminator.
 */
#define C42_U8Z_EQLIT(_a, _b) \
    (!c42_u8a_cmp((_a), (uint8_t const *) (_b), sizeof(_b)))

/* c42_u16z_len *************************************************************/
/**
 *  Finds the length in elements of a zero-terminated 16-bit int array.
 */
C42_API size_t C42_CALL c42_u16z_len
(
    uint16_t const * a
);

/* c42_u64_to_str ***********************************************************/
/**
 *  Converts the given unsigned 64-bit int to ASCII NUL-terminated string.
 *  @param [out]    str     output buffer; caller must ensure is large enough
 *                          for the result
 *  @param [in]     value   value to convert
 *  @param [in]     radix   numeration base; must be in range 2 - 36
 *  @param [in]     width   desired width of the output; this is a guaranteed
 *                          minimum size for the output, not a maximum;
 *                          if the value is large it may cause the output to
 *                          exceed this limit
 *  @param [in]     group   how many digits to group together before using a
 *                          separator char; use 64 or above to disable grouping
 *  @param [in]     sep     separator char
 *  @returns        N       number of chars outputted, not including the
 *                          NUL terminator
 *  @note on DEBUG builds if radix is not in the valid range it will output
 *  the string "BAD_RADIX"
 */
C42_API size_t C42_CALL c42_u64_to_str
(
    uint8_t * str,
    uint64_t value,
    uint_fast8_t radix,
    uint32_t width,
    uint_fast8_t group,
    uint_fast8_t sep
);

#define C42_NO_SIGN 0
    /**< consider the value unsigned (flag for c42_i64_to_str#sign_mode) */

#define C42_SIGN_NEG 1
    /**< only put the sign if the value is negative
     *  (flag for c42_i64_to_str#sign_mode) */

#define C42_SIGN_ALWAYS 2
    /**< use '-' for negative, space for 0 and + for positive numbers
     *  (flag for c42_i64_to_str#sign_mode) */

#define C42_SIGN_ALIGN 3
    /**< use '-' for negative, space for non-negative numbers
     *  (flag for c42_i64_to_str#sign_mode) */

/* c42_i64_to_str ***********************************************************/
/**
 *  Converts the given int to ASCII string.
 *  @param [out]    str     output buffer; caller must ensure is large enough
 *                          for the result
 *  @param [in]     value   value to convert
 *  @param [in]     sign_mode   one of: #C42_NO_SIGN, #C42_SIGN_NEG,
 *                          C42_SIGN_ALWAYS, #C42_SIGN_ALIGN
 *  @param [in]     radix   numeration base; must be in range 2 - 36
 *  @param [in]     prefix  NUL-terminated C-string to be put in front of the
 *                          digits (after the sign, if any); can be NULL
 *  @param [in]     width   desired width of the output; this is a guaranteed
 *                          minimum size for the output, not a maximum;
 *                          if the value is large it may cause the output to
 *                          exceed this limit
 *  @param [in]     group   how many digits to group together before using a
 *                          separator char; use 64 or above to disable grouping
 *  @param [in]     sep     separator char
 *  @returns                number of chars outputted, not including the
 *                          NUL terminator
 */
C42_API size_t C42_CALL c42_i64_to_str
(
    uint8_t * str,
    int64_t value,
    uint_fast8_t sign_mode,
    uint_fast8_t radix,
    uint8_t const * prefix,
    uint32_t width,
    uint_fast8_t group,
    uint_fast8_t sep
);

#define C42_U64_STOP 1 /**< non-digit encountered */
#define C42_U64_OVERFLOW 2 /**< number parsed exceeds uint64_t capacity */
/* c42_u64_from_str *********************************************************/
/**
 *  Converts a string to unsigned int.
 *  @param [in]     str     pointer to data
 *  @param [in]     len     length of input string
 *  @param [in]     radix   numeration base; 0 is magic for autodetect
 *  @param [out]    value   will be filled with the parsed value
 *  @param [out]    used_len filled with number of bytes parsed; can be NULL
 *  @retval 0 parsing successful, full length used
 *  @retval C42_U64_STOP non-digit encountered
 *  @retval C42_U64_OVERFLOW
 */
C42_API uint_fast8_t C42_CALL c42_u64_from_str
(
    uint8_t const * str,
    size_t len,
    uint_fast8_t radix,
    uint64_t * value,
    size_t * used_len
);

/* c42_ucp_is_valid *********************************************************/
/**
 *  Tells if a given int is a valid Unicode code-point.
 */
C42_INLINE int c42_ucp_is_valid (uint32_t ucp)
{
    return ucp < 0xD800 || (ucp >= 0xE000 && ucp < 0x110000);
}

/* c42_ucp_utf8_len *********************************************************/
/**
 *  Returns the number of bytes needed for representing the Unicode codepoint
 *  in UTF8.
 *  @param ucp a valid Unicode codepoint
 */
C42_INLINE unsigned int c42_ucp_utf8_len (uint32_t ucp)
{
    if (ucp < 0x80) return 1;
    if (ucp < 0x800) return 2;
    if (ucp < 0x10000) return 3;
    return 4;
}

/* c42_ucp_to_utf8 **********************************************************/
/**
 *  Encodes a single Unicode codepoint in UTF8.
 *  This works to encode any valid and invalid codepoints below 0x1FFFFF.
 *  @returns number of bytes written.
 *  @warning the output buffer must be large enough to store the encoded
 *  value.
 */
C42_INLINE unsigned int c42_ucp_to_utf8
(
    uint8_t * out,
    uint32_t ucp
)
{
    if (ucp < 0x80) { *out = ucp; return 1; }
    if (ucp < 0x800)
    {
        out[0] = 0xC0 | (ucp >> 6);
        out[1] = 0x80 | (ucp & 0x3F);
        return 2;
    }
    if (ucp < 0x10000)
    {
        out[0] = 0xE0 | (ucp >> 12);
        out[1] = 0x80 | ((ucp >> 6) & 0x3F);
        out[2] = 0x80 | (ucp & 0x3F);
        return 3;
    }
    out[0] = 0xF0 | (ucp >> 18);
    out[1] = 0x80 | ((ucp >> 12) & 0x3F);
    out[2] = 0x80 | ((ucp >> 6) & 0x3F);
    out[3] = 0x80 | (ucp & 0x3F);
    return 4;
}

/* c42_ucp_from_valid_utf8 **************************************************/
/**
 *  Decodes a single Unicode code-point from a valid UTF-8 string.
 *  @warning calling this on an unverified UTF-8 data may lead to accessing
 *  outside the buffer bounds.
 */
C42_INLINE uint32_t c42_ucp_from_valid_utf8
(
    uint8_t const * * data_p
)
{
    uint8_t const * data = *data_p;
    uint32_t ucp = *data++;
    if (ucp < 0x80);
    else if (ucp < 0xE0)
    {
        ucp = ((ucp & 0x1F) << 6);
        ucp |= (*data++) & 0x3F;
    }
    else if (ucp < 0xF0)
    {
        ucp = (ucp & 0x0F) << 12;
        ucp |= ((*data++) & 0x3F) << 6;
        ucp |= ((*data++) & 0x3F);
    }
    else
    {
        ucp = (ucp & 7) << 18;
        ucp |= ((*data++) & 0x3F) << 12;
        ucp |= ((*data++) & 0x3F) << 6;
        ucp |= ((*data++) & 0x3F);
    }
    *data_p = data;
    return ucp;
}

/* C42_TWO_BYTE_NUL *********************************************************/
/**
 *  Flag for converters to UTF-8 to encode NUL as two-byte sequence.
 *  Should be used with any of these functions:
 *  - c42_utf16le_to_utf8_len()
 *  - c42_utf16le_to_utf8()
 */
#define C42_TWO_BYTE_NUL 1

/* C42_ALLOW_UNPAIRED_SURROGATES ********************************************/
/**
 *  Flag for converters to UTF-8 to allow unpaired surrogate codepoints.
 *  Should be used with any of these functions:
 *  - c42_utf16le_to_utf8_len()
 *  - c42_utf16le_to_utf8()
 */
#define C42_ALLOW_UNPAIRED_SURROGATES 2

/* C42_NEVER_PAIR_SURROGATES ************************************************/
/**
 *  Flag for converters to UTF-8 to never combine lead and trail surrogate
 *  codepoints into a single code point.
 *  This together with #C42_TWO_BYTE_NUL cause converters to generate MUTF-8.
 *  Should be used with any of these functions:
 *  - c42_utf16le_to_utf8_len()
 *  - c42_utf16le_to_utf8()
 */
#define C42_NEVER_PAIR_SURROGATES 4

/* c42_utf16le_to_utf8_len **************************************************/
/**
 *  Computes the size of output UTF-8 data after converting from UTF-16-LE.
 *  @param [in] data
 *      UTF-16-LE data
 *  @param [in] len
 *      length in bytes of UTF-16-LE data
 *  @param [in] flags
 *      a bitmask with any combination of:
 *      - #C42_TWO_BYTE_NUL encode '\0' as 2-byte UTF8 ("\xC0\x80")
 *      - #C42_ALLOW_UNPAIRED_SURROGATES to allow as input any 16-bit int array
 *        by potentially producing code points in range D800 - DFFF
 *      - #C42_NEVER_PAIR_SURROGATES to treat each 16-bit code point as a
 *        standalone unit; this together with #C42_TWO_BYTE_NUL esentially
 *        transform the data to MUTF8.
 *  @returns
 *      size in bytes of resulting UTF-8 text
 *  @retval -1
 *      @a len is odd integer
 *  @retval -2
 *      if unpaired surrogates are found and #C42_ALLOW_UNPAIRED_SURROGATES
 *      is not specified in @a flags
 *  @retval -3
 *      output length too large to fit in ptrdiff_t
 */
C42_API ptrdiff_t C42_CALL c42_utf16le_to_utf8_len
(
    uint8_t const * data,
    size_t len,
    unsigned int flags
);

/* c42_utf16le_to_utf8 ******************************************************/
/**
 *  Converts UTF-16-LE to UTF-8.
 *  @param [out] out
 *      pointer to output buffer; the buffer must be large enough to hold
 *      the result; one must call first c42_utf16le_to_utf8_len() to find out
 *      the size needed
 *  @param [in] data
 *      UTF-16-LE data that must be valid according to options passed in flags
 *  @param [in] len
 *      data length in bytes
 *  @param [in] flags
 *      a bitmask with any combination of:
 *      - #C42_TWO_BYTE_NUL encode '\0' as 2-byte UTF8 ("\xC0\x80")
 *      - #C42_ALLOW_UNPAIRED_SURROGATES to allow as input any 16-bit int array
 *        by potentially producing code points in range D800 - DFFF
 *      - #C42_NEVER_PAIR_SURROGATES to treat each 16-bit code point as a
 *        standalone unit; this together with #C42_TWO_BYTE_NUL esentially
 *        transform the data to MUTF8.
 * @returns end output pointer
 */
C42_API uint8_t * C42_CALL c42_utf16le_to_utf8
(
    uint8_t * out,
    uint8_t const * data,
    size_t len,
    unsigned int flags
);

/* c42_ucp_term_width *******************************************************/
/**
 *  Computes the typical width of a Unicode codepoint when displayed in a
 *  character based terminal.
 *  @retval -1 Unicode codepoint invalid or not printable
 *  @retval 0 zero-width character (f.ex. combining characters)
 *  @retval 1 normal width character
 *  @retval 2 double width characted
 */
C42_API int C42_CALL c42_ucp_term_width (uint32_t ucp);

/* c42_utf8_writer_f ********************************************************/
/**
 *  Function pointer for plain UTF-8 text writers.
 *  @param ctx context for the writer (e.g. an I/O object, a buffer, etc)
 *  @param data input text - guaranteed to be valid UTF-8
 *  @param len length of input text; the writer should never be called with 0
 *  @returns number of bytes successfully written; the reason for the error,
 *  if any, is left at the discretion of the implementation (e.g. ctx is a
 *  structure with a status code field)
 */
typedef size_t (C42_CALL * c42_utf8_writer_f)
    (
        uint8_t const * data,
        size_t len,
        void * ctx
    );

/* c42_sbw_t ****************************************************************/
/**
 *  Single-buffer writer context structure.
 */
typedef struct c42_sbw_s c42_sbw_t;
struct c42_sbw_s
{
    uint8_t * data; /**< pointer to buffer. */
    size_t limit; /**< number of bytes available at @a data. */
    size_t size; /**< how much was written to the buffer; this will accurately
                   count the bytes written although anything beyond @a limit
                   will not be stored */
};

/* c42_sbw_init *************************************************************/
/**
 *  Initializes the context structure for sbw writer.
 *  See c42_sbw_t and c42_sbw_write().
 *  @returns @a sbw
 */
C42_API c42_sbw_t * C42_CALL c42_sbw_init
(
    c42_sbw_t * sbw,
    uint8_t * data,
    size_t limit
);

/* c42_sbw_write ************************************************************/
/**
 *  Single-buffer writer processor function.
 *  See c42_sbw_t and #c42_utf8_writer_f.
 */
C42_API size_t C42_CALL c42_sbw_write
(
    uint8_t const * data,
    size_t len,
    void * sbw
);

/* c42_utf8_width_f *********************************************************/
/**
 *  Computes the width (in some arbitrary user-defined integral units) of the
 *  given valid UTF-8 string.
 */
typedef int32_t (C42_CALL * c42_utf8_width_f)
(
    uint8_t const * data,
    size_t len,
    void * ctx
);

/* c42_utf8_validate ********************************************************/
/**
 *  Validates the given UTF-8 string.
 *  @param [in] data    input buffer pointer
 *  @param [in] len     input buffer length
 *  @param [out] ok_len_p   receives the length of the string parsed without
 *                          errors; can be NULL
 *  @retval 0 input string is valid
 *  @retval 1 unterminated string
 *  @retval 2 malformed start byte
 *  @retval 3 malformed continuation byte
 *  @retval 4 surrogate codepoint
 *  @retval 5 overly long encoded codepoint
 */
C42_API uint_fast8_t C42_CALL c42_utf8_validate
(
    uint8_t const * data,
    size_t len,
    size_t * ok_len_p
);

/* c42_utf8_term_width ******************************************************/
/**
 *  Computes the 'terminal' width of the given valid UTF-8 string.
 *  This assumes the terminal is printing normal-width (1) and double-width (2)
 *  characters and supports combining and other zero-width characters.
 *  This function can be used as an implementation for #c42_utf8_width_f.
 *  @param data [in] pointer to valid UTF8 buffer
 *  @param len [in] size of buffer
 *  @param ctx [in] not used
 *  @returns computed width as a non-negative int
 *  @retval -1 some codepoint is invalid or non-printable
 *  @retval -2 width too large to fit in the return type
 */
C42_API int32_t C42_CALL c42_utf8_term_width
(
    uint8_t const * data,
    size_t len,
    void * ctx
);

#define C42_FMT_MALFORMED 1 /**< bad format string error code */
#define C42_FMT_WIDTH_ERROR 2 /**< width function returned error */
#define C42_FMT_WRITE_ERROR 3 /**< write error */
#define C42_FMT_CONV_ERROR 4 /**< conversion error during escaping of some string */
#define C42_FMT_NO_CODE 5 /**< feature not implemented */

/* c42_write_vfmt ***********************************************************/
/**
 *  Writes formatted UTF-8 text (similar to printf formatting).
 *  @retval 0 success
 *  @retval C42_FMT_MALFORMED bad format string
 *  @retval C42_FMT_WIDTH_ERROR
 *  @retval C42_FMT_WRITE_ERROR
 */
C42_API uint_fast8_t C42_CALL c42_write_vfmt
(
    c42_utf8_writer_f writer,
    void * writer_context,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    va_list va
);

/* c42_write_fmt ************************************************************/
/**
 *  Writes formatted UTF-8 text.
 *  See c42_write_vfmt.
 */
C42_API uint_fast8_t C42_CALL c42_write_fmt
(
    c42_utf8_writer_f writer,
    void * writer_context,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    ...
);

#define C42_CLCONV_OK 0
/**< entire input used (return code for #c42_clconv_f) */

#define C42_CLCONV_MALFORMED 1
/**< malformed input; error code for #c42_clconv_f */

#define C42_CLCONV_INCOMPLETE 2
/**< input needs more data for decoding a valid item */

#define C42_CLCONV_FULL 3
/**< output full (error code for #c42_clconv_f) */

/* c42_clconv_f ******************************************************/
/**
 *  Chunked linear converter function.
 *  Implementations must consume as much as possible from input, either
 *  until the entire input is used, either until the output buffer cannot
 *  hold an extra item.
 *  Param @a ctx can be used to store the state if the conversion requires
 *  keeping a state.
 *  After feeding the entire desired input to the converter, users should
 *  call this function again with @a in set to NULL and @a in_len set to 0
 *  to tell the converter to append any terminating sequence.
 */
typedef uint_fast8_t (C42_CALL * c42_clconv_f)
    (
        uint8_t const * in,
        size_t in_len,
        size_t * in_used_len,
        uint8_t * out,
        size_t out_len,
        size_t * out_used_len,
        void * ctx
    );

/* c42_clconv_hex_to_bin ****************************************************/
/**
 *  Hex to bin converter.
 *  @param [in] in input data
 *  @param [in] in_len length of input data
 *  @param [out] in_used_len will be filled in with length of input data
 *              processed
 *  @param [in] out output buffer
 *  @param [in] out_len size of output buffer
 *  @param [out] out_used_len   will be filled in with length of output produced
 *  @param [in] ctx this can be NULL for strict conversion or it should
 *      point to a NUL-terminated byte string containing bytes to skip
 */
C42_API uint_fast8_t C42_CALL c42_clconv_hex_to_bin
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
);

/* c42_clconv_bin_to_hex_line ***********************************************/
/**
 *  Bin to hex (all output on one line without any whitespace).
 *  @param [in] in
 *      input data
 *  @param [in] in_len
 *      length of input data
 *  @param [out] in_used_len
 *      will be filled in with length of input data processed
 *  @param [in] out
 *      output buffer
 *  @param [in] out_len
 *      size of output buffer
 *  @param [out] out_used_len
 *      will be filled in with length of output produced
 *  @param ctx
 *      this should be NULL
 */
C42_API uint_fast8_t C42_CALL c42_clconv_bin_to_hex_line
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
);

/* c42_clconv_c_escape_force_hex ********************************************/
/**
 *  C-Escapes a byte array forcing 2 hex digit '\\x' escapes even when
 *  generating bad string literals.
 *  @param [in] in
 *      input data
 *  @param [in] in_len
 *      length of input data
 *  @param [out] in_used_len
 *      will be filled in with length of input data processed
 *  @param [in] out
 *      output buffer
 *  @param [in] out_len
 *      size of output buffer
 *  @param [out] out_used_len
 *      will be filled in with length of output produced
 *  @param ctx
 *      NULL
 *  @warning This does not produce strings conforming to the C standard;
 *      AB34 will be converted to "\\xAB4" which according to C is a 1 char
 *      string (or invalid literal when char has under 12 bits).
 *      This is the same way python is understanding literals (\\x is always
 *      followed by exactly to hex digits)
 */
C42_API uint_fast8_t C42_CALL c42_clconv_c_escape_force_hex
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
);

/* c42_clconv_c_escape_t ****************************************************/
/**
 *  Context structure for c42_clconv_c_escape().
 */
typedef struct c42_clconv_c_escape_s c42_clconv_c_escape_t;
struct c42_clconv_c_escape_s
{
    int crt_byte; /**< internal field. */
};

/* c42_clconv_c_escape_init *************************************************/
/**
 *  Inits the c-escape converter context.
 */
C42_API void * C42_CALL c42_clconv_c_escape_init
(
    c42_clconv_c_escape_t * ctx
);

/* c42_clconv_c_escape ******************************************************/
/**
 *  C-style escape.
 */
C42_API uint_fast8_t C42_CALL c42_clconv_c_escape
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
);

/** @} */

/* io ***********************************************************************/
/** @defgroup io8 I/O Streams
 *  @{
 */

#define C42_IO8_OK 0 /**< operation completed ok */
#define C42_IO8_INTERRUPTED 1 /**< operation interrupted (by signal / APC) */
#define C42_IO8_WOULD_BLOCK 2 /**< read/write would on non-blocking I/O stream*/
#define C42_IO8_BAD_FILE 3 /**< file descriptor/handle is not valid */
#define C42_IO8_BAD_ADDR 4 /**< bad buffer address */
#define C42_IO8_TOO_BIG 5 /**< operation would exceed transfer limit */
#define C42_IO8_BAD_WRITE 6
    /**< unaligned write for direct I/O stream or no writes allowed for the
     * stream */
#define C42_IO8_IO_ERROR 7 /**< low-level I/O error */
#define C42_IO8_NO_SPACE 8 /**< no space left for writing */
#define C42_IO8_BROKEN_PIPE 9 /**< other end of the pipe is closed */
#define C42_IO8_BAD_SIZE 10 /**< size greater than SSIZE_MAX */
#define C42_IO8_EOF 11 /**< reached end of file */
#define C42_IO8_IS_DIR 12 /**< attempted to read from a directory */
#define C42_IO8_BAD_POS 13
    /**< resulting seek position would be negative or beyond device limit */
#define C42_IO8_POS_OVERFLOW 14
    /**< resulting seek position overflows data type */
#define C42_IO8_NO_SEEK 15
    /**< seeking a non-seekable file */
#define C42_IO8_FMT_MALFORMED 16 /**< bad format string error code */
#define C42_IO8_FMT_WIDTH_ERROR 17 /**< width function returned error */
#define C42_IO8_FMT_WRITE_ERROR 18 /**< write error */
#define C42_IO8_FMT_CONV_ERROR 19 /**< conversion error during escaping of some string */
#define C42_IO8_FMT_NO_CODE 20 /**< formatting code not implemented */
#define C42_IO8_BAD_OP 21 
    /**< bad operation; (e.g. write on a read-only file, seek on stdin, etc) */
#define C42_IO8_NA 22 
    /**< feature not available (close read-side of a regular file) */
#define C42_IO8_NOT_IMPLEMENTED 126 /**< feature not implemented */
#define C42_IO8_OTHER_ERROR 127
    /**< and now for something completely different! */

#define C42_IO8_OP_READ 1
#define C42_IO8_OP_WRITE 2
#define C42_IO8_OP_SEEK 4

/* c42_io8_t ****************************************************************/
/**
 * 8-bit I/O stream.
 */
typedef struct c42_io8_s c42_io8_t;

/* c42_io8_class_t **********************************************************/
/**
 * 8-bit I/O stream class.
 * This structure holds the functions that perform operations on the stream.
 */
typedef struct c42_io8_class_s c42_io8_class_t;
struct c42_io8_class_s
{
    uint_fast8_t (C42_CALL * read)
        (uintptr_t ctx, uint8_t * data, size_t size, size_t * rsize);
    /**< function pointer for the read operation. */

    uint_fast8_t (C42_CALL * write)
        (uintptr_t ctx, uint8_t const * data, size_t size, size_t * wsize);
    /**< function pointer for the write operation. */

    uint_fast8_t (C42_CALL * seek)
        (uintptr_t ctx, ptrdiff_t offset, int anchor, size_t * pos);
    /**< function pointer for seeking with ptrdiff_t offset */

    uint_fast8_t (C42_CALL * seek64)
        (uintptr_t ctx, int64_t offset, int anchor, uint64_t * pos);
    /**< function pointer for seeing with 64-bit int pointer */

    uint_fast8_t (C42_CALL * truncate)
        (uintptr_t ctx);
    /**< function pointer for truncate operation */

    uint_fast8_t (C42_CALL * close)
        (uintptr_t ctx, int mode);
    /**< function pointer for closing the stream.
     *  @param ctx [in] context
     *  @param mode [in] bitmask with flags: C42_IO8_OP_READ | C42_IO8_OP_WRITE
     **/
};

struct c42_io8_s
{
    c42_io8_class_t * io8_class; /**< pointer to the io8 class */
    uintptr_t context; /**< context to be passed to io functions */
};

/* c42_io8_read *************************************************************/
/**
 *  Reads data from an I/O stream.
 *  @returns 0  success
 *  @returns C42_IO8_xxx error
 */
C42_API uint_fast8_t C42_CALL c42_io8_read
(
    c42_io8_t * io,
    uint8_t * data,
    size_t size,
    size_t * rsize
);

/* c42_io8_read_full ********************************************************/
/**
 *  Reads chunks until requested size is transferred.
 *  Ignores interruptions (from signals or APCs).
 *  @returns 0 success; *rsize set to size
 *  @returns C42_IO8_EOF read until end of file
 *  @returns C42_IO8_xxx some error
 */
C42_API uint_fast8_t C42_CALL c42_io8_read_full
(
    c42_io8_t * io,
    uint8_t * data,
    size_t size,
    size_t * rsize
);

/* c42_io8_write ************************************************************/
/**
 *  Writes data to an I/O stream.
 *  @returns 0  success
 */
C42_API uint_fast8_t C42_CALL c42_io8_write
(
    c42_io8_t * io,
    uint8_t const * data,
    size_t size,
    size_t * wsize
);

/* c42_io8_write_full *******************************************************/
/**
 *  Writes full data, ignoring interruptions (signals, APCs).
 *  This function will call c42_io8_write() in a loop if the actual written
 *  size is less than what's requested or if it receives #C42_IO8_INTERRUPTED.
 *  @param io stream to write to
 *  @param data pointer to buffer
 *  @param size number of bytes at @a data
 *  @param wsize pointer where the size written is returned; can be NULL
 *  @returns 0  success
 */
C42_API uint_fast8_t C42_CALL c42_io8_write_full
(
    c42_io8_t * io,
    uint8_t const * data,
    size_t size,
    size_t * wsize
);

/* c42_io8_write_u8z ********************************************************/
/**
 *  Writes the given NUL terminated byte string.
 */
C42_API uint_fast8_t C42_CALL c42_io8_write_u8z
(
    c42_io8_t * io,
    uint8_t const * u8z,
    size_t * wsize
);

/* C42_IO8_WRITE_LIT ********************************************************/
/**
 *  Writes the given string literal.
 */
#define C42_IO8_WRITE_LIT(_io, _lit) \
    (c42_io8_write_full((_io), (uint8_t const *) (_lit), sizeof(_lit) - 1, NULL))

/* c42_io8_wvfmt ************************************************************/
/**
 *  Writes formatted UTF-8 text (similar to printf formatting).
 *  @retval 0 success
 *  @retval C42_FMT_MALFORMED bad format string
 *  @retval C42_FMT_WIDTH_ERROR
 *  @retval C42_FMT_WRITE_ERROR
 */
C42_API uint_fast8_t C42_CALL c42_io8_wvfmt
(
    c42_io8_t * io,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    va_list va
);

/* c42_io8_vfmt *************************************************************/
/**
 *  Writes formatted UTF-8 text using terminal-style width calculations.
 *  @retval 0 success
 *  @retval C42_FMT_MALFORMED bad format string
 *  @retval C42_FMT_WIDTH_ERROR
 *  @retval C42_FMT_WRITE_ERROR
 */
#define c42_io8_vfmt(_io, _fmt, _va) \
    (c42_io8_wvfmt((_io), c42_utf8_term_width, NULL, (_fmt), (_va)))

/* c42_io8_wfmt *************************************************************/
/**
 *  Writes formatted UTF-8 text using terminal-style width calculations.
 *  See c42_write_vfmt.
 *  @returns 0 on success or some C42_IO8_xxx error code
 *  @retval C42_IO8_FMT_MALFORMED bad format string
 *  @retval C42_IO8_FMT_WIDTH_ERROR width computation failed
 *  @retval C42_IO8_FMT_WRITE_ERROR this is returned only if there is a bug
 *          causing no I/O error code to be returned but partial buffer being
 *          written
 *  @retval C42_IO8_FMT_CONV_ERROR conversion error
 *  @retval C42_IO8_FMT_NO_CODE formatting feature not implemented
 */
C42_API uint_fast8_t C42_CALL c42_io8_wfmt
(
    c42_io8_t * io,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    ...
);

/* c42_io8_fmt **************************************************************/
/**
 *  Writes formatted UTF-8 text using terminal-style width calculations.
 */
#define c42_io8_fmt(_io, _fmt, ...) \
    (c42_io8_wfmt((_io), c42_utf8_term_width, NULL, (_fmt), __VA_ARGS__))

/* c42_io8bc_t **************************************************************/
/**
 *  I/O Stream with static buffer and counter.
 *  Check c42_io8bc_init().
 */
typedef struct c42_io8bc_s c42_io8bc_t;
struct c42_io8bc_s
{
    c42_io8_t io8; /**< base io8 object */
    uint8_t * data; /**< data buffer pointer */
    size_t limit; /**< size of buffer */
    size_t size; /**< size used so far */
    size_t offset; /**< current file position */
};

/* c42_io8bc_init ***********************************************************/
/**
 *  Init a buffer with counter stream.
 *  The stream will store buffers written as long as they fit in the given
 *  buffer. Writes beyond @a limit will report success but will not store
 *  any data that would not fit in the buffer.
 *  Read request that include missing data (data previously written at an
 *  offset past @a limit) will report C42_IO8_IO_ERROR.
 *  @returns @a io8bc casted to c42_io8_t.
 */
C42_API c42_io8_t * C42_CALL c42_io8bc_init
(
    c42_io8bc_t * io8bc,
    uint8_t * data,
    size_t limit
);

/** @} */

/* fsa **********************************************************************/
/** @defgroup fsa Filesystem interface
 *  @{
 */

#define C42_FSA_SOME_ERROR 1 /**< exactly! */
#define C42_FSA_BAD_MODE 2 /**< error: bad open mode specified. */
#define C42_FSA_NO_MEM 3 /**< no mem to open the file */

#define C42_FSA_OPEN_EXISTING 0 /**< opens existing file or fails */
#define C42_FSA_OPEN_ALWAYS 1 /**< opens existing file or creates a new one */
#define C42_FSA_CREATE_NEW 2 /**< fails if the file exists, otherwise creates it */
#define C42_FSA_CREATE_ALWAYS 3 /**< truncates existing or creates new */
#define C42_FSA_TRUNC_EXISTING 4 /**< truncates existing or fails */
#define C42_FSA_READ (1 << 3) /**< request read access */
#define C42_FSA_WRITE (1 << 4) /**< request write access */
#define C42_FSA_RW (C42_FSA_READ | C42_FSA_WRITE) /**< request read/write access */
#define C42_FSA_PERM_SHIFT 5 /**< bit shift constant for create permissions */
#define C42_FSA_UR (1 << (C42_FSA_PERM_SHIFT + 8)) /**< user-read permission */
#define C42_FSA_UW (1 << (C42_FSA_PERM_SHIFT + 7)) /**< user-write permission */
#define C42_FSA_UX (1 << (C42_FSA_PERM_SHIFT + 6)) /**< user-execute permission */
#define C42_FSA_GR (1 << (C42_FSA_PERM_SHIFT + 5)) /**< group-read permission */
#define C42_FSA_GW (1 << (C42_FSA_PERM_SHIFT + 4)) /**< group-write permission */
#define C42_FSA_GX (1 << (C42_FSA_PERM_SHIFT + 3)) /**< group-execute permission */
#define C42_FSA_OR (1 << (C42_FSA_PERM_SHIFT + 2)) /**< others-read perm */
#define C42_FSA_OW (1 << (C42_FSA_PERM_SHIFT + 1)) /**< others-write perm */
#define C42_FSA_OX (1 << (C42_FSA_PERM_SHIFT + 0)) /**< others-execute perm */

/* c42_fsa_t ****************************************************************/
/**
 *  File-system interface.
 *
 */
typedef struct c42_fsa_s c42_fsa_t;
struct c42_fsa_s
{
    /** File open function pointer.
     *  @param [out] io
     *      gets the inited io object
     *  @param [in] path
     *      a NUL-terminated byte-array with the path to open
     *  @param [in] mode
     *      one of C42_FSA_OPEN_EXISTING, C42_FSA_OPEN_ALWAYS,
     *      C42_FSA_CREATE_NEW, C42_FSA_CREATE_ALWAYS, C42_FSA_TRUNC_EXISTING
     *      bitwise ORed with one of: C42_FSA_READ, C42_FSA_WRITE, C42_FSA_RW
     *      and if file creation is allowed any bitwise OR mask with
     *      C42_FSA_UR, C42_FSA_UW, C42_FSA_UX,
     *      C42_FSA_GR, C42_FSA_GW, C42_FSA_GX,
     *      C42_FSA_OR, C42_FSA_OW, C42_FSA_OX.
     *  @param context
     *      value stored in c42_fsa_t#file_open_context
     */
    uint_fast8_t (C42_CALL * file_open)
        (
            c42_io8_t * io,
            uint8_t const * path,
            int mode,
            void * context
        );

    /** Context pointer for c42_fsa_t#file_open function. */
    void * file_open_context;
};

/* c42_file_open ************************************************************/
/**
 *  Opens a file using the given filesystem interface.
 */
C42_INLINE uint_fast8_t C42_CALL c42_file_open
(
    c42_fsa_t * fsa,
    c42_io8_t * io,
    uint8_t const * path,
    int mode
)
{
    return fsa->file_open(io, path, mode, fsa->file_open_context);
}

/** @} */

/****************************************************************************/
/** @defgroup ma Memory Allocation
 *  @{
 */

#define C42_MA_OK 0 /**< alloc/realloc/free done ok */
#define C42_MA_NO_MEM 1 /**< not enough mem for alloc/realloc */
#define C42_MA_LIMIT 2 /**< limit reached for alloc/realloc */
#define C42_MA_BAD_ITEM_SIZE 3 /**< item size passed as 0 or larger than the 
                                 max ptrdiff_t value*/
#define C42_MA_SIZE_OVERFLOW 4 /**< item_size * count is too big */
#define C42_MA_CORRUPT 0x7F /**< heap corruption detected; 
                                 callers should attempt to exit */

/* c42_ma_f *****************************************************************/
/**
 *  Memory allocator handler.
 *  This function does alloc/realloc/free.
 */
typedef uint_fast8_t (C42_CALL * c42_ma_f)
    (
        void * * ptr_p,
        size_t old_size,
        size_t new_size,
        void * ctx
    );

/* c42_ma_t *****************************************************************/
/**
 *  Memory allocation interface.
 */
typedef struct c42_ma_s c42_ma_t;
struct c42_ma_s
{
    c42_ma_f handler; /**< handler function */
    void * context; /**< implementation specific context data */
};

/* c42_ma_alloc *************************************************************/
/**
 *  Allocates a new array.
 */
C42_INLINE uint_fast8_t c42_ma_alloc
(
    c42_ma_t * ma_p,
    void * * ptr_p,
    size_t item_size,
    size_t count
)
{
    ptrdiff_t new_size = item_size * count;
    if ((ptrdiff_t) item_size <= 0) return C42_MA_BAD_ITEM_SIZE;
    if (new_size / item_size != count) return C42_MA_SIZE_OVERFLOW;
    return ma_p->handler(ptr_p, 0, new_size, ma_p->context);
}

/* c42_ma_realloc ***********************************************************/
/**
 *  Alloc/realloc/free.
 */
C42_INLINE uint_fast8_t c42_ma_realloc
(
    c42_ma_t * ma_p,
    void * * ptr_p,
    size_t item_size,
    size_t old_count,
    size_t new_count
)
{
    ptrdiff_t new_size = item_size * new_count;
    if ((ptrdiff_t) item_size <= 0) return C42_MA_BAD_ITEM_SIZE;
    if (new_size / item_size != new_count) return C42_MA_SIZE_OVERFLOW;
    return ma_p->handler(ptr_p, item_size * old_count, new_size, ma_p->context);
}

/* c42_ma_free **************************************************************/
/**
 *  Frees an array.
 */
C42_INLINE uint_fast8_t c42_ma_free
(
    c42_ma_t * ma_p,
    void * ptr,
    size_t item_size,
    size_t count
)
{
    return ma_p->handler(&ptr, item_size * count, 0, ma_p->context);
}

/* C42_MA_ARRAY_ALLOC *******************************************************/
/**
 *  Macro that allocates an array given a typed pointer.
 */
#define C42_MA_ARRAY_ALLOC(_ma, _ptr, _len) \
    (c42_ma_alloc((_ma), (void * *) &(_ptr), sizeof(*(_ptr)), (_len)))

/* C42_MA_ARRAY_DOUBLE ******************************************************/
/**
 *  Macro that doubles the size of an array held by a typed array
 */
#define C42_MA_ARRAY_DOUBLE(_ma, _ptr, _cur_len) \
    (c42_ma_realloc((_ma), (void * *) &(_ptr), \
                    sizeof(*(_ptr)), (_cur_len), (_cur_len) << 1))

/** @} */

/****************************************************************************/
/** @defgroup smt Simple Multithreading
 *  @{
 */

#define C42_SMT_OK 0 /**< ok */

/* c42_smt_tid_t ************************************************************/
/**
 *  Thread ID.
 */
typedef uintptr_t c42_smt_tid_t;

/* c42_smt_mutex_t **********************************************************/
/**
 *  Mutex.
 */
typedef struct c42_smt_mutex_s c42_smt_mutex_t;

/* c42_smt_cond_t ***********************************************************/
/**
 *  Condition Variable.
 */
typedef struct c42_smt_cond_s c42_smt_cond_t;

/* c42_smt_thread_f *********************************************************/
/**
 *  Thread procedure.
 */
typedef uint8_t (C42_CALL * c42_smt_thread_f) (void * arg);

/* c42_smt_t ****************************************************************/
/**
 *  Simple Multithreading interface.
 */
typedef struct c42_smt_s c42_smt_t;

struct c42_smt_s
{
    /* thread_create ********************************************************/
    /**
     *  creates a thread.
     */
    uint_fast8_t (C42_CALL * thread_create)
        (
            c42_smt_t * smt_p,
            c42_smt_tid_t * tid_p,
            c42_smt_thread_f func,
            void * arg
        );

    /* thread_join **********************************************************/
    /**
     *  joins a thread.
     */
    uint_fast8_t (C42_CALL * thread_join)
        (
            c42_smt_t * smt_p,
            c42_smt_tid_t tid,
            uint32_t * exit_code_p
        );

    /* mutex_init ***********************************************************/
    /**
     *  Inits a mutex.
     */
    uint_fast8_t (C42_CALL * mutex_init)
        (
            c42_smt_t * smt_p,
            c42_smt_mutex_t * mutex_p
        );
    /* mutex_finish *********************************************************/
    /**
     *  Frees resources allocated for a mutex.
     */
    uint_fast8_t (C42_CALL * mutex_finish)
        (
            c42_smt_t * smt_p,
            c42_smt_mutex_t * mutex_p
        );
    /* mutex_lock ***********************************************************/
    /**
     *  Locks a mutex.
     */
    uint_fast8_t (C42_CALL * mutex_lock)
        (
            c42_smt_t * smt_p,
            c42_smt_mutex_t * mutex_p
        );
    /* mutex_trylock ********************************************************/
    /**
     *  Tries to lock a mutex.
     */
    uint_fast8_t (C42_CALL * mutex_trylock)
        (
            c42_smt_t * smt_p,
            c42_smt_mutex_t * mutex_p
        );
    /* mutex_unlock *********************************************************/
    /**
     *  Unlocks a mutex.
     */
    uint_fast8_t (C42_CALL * mutex_unlock)
        (
            c42_smt_t * smt_p,
            c42_smt_mutex_t * mutex_p
        );

    /* cond_init ************************************************************/
    /**
     *  Inits a condition variable.
     */
    uint_fast8_t (C42_CALL * cond_init)
        (
            c42_smt_t * smt_p,
            c42_smt_cond_t * cond_p
        );
    /* cond_finish **********************************************************/
    /**
     *  Frees resources allocated for a condition variable.
     */
    uint_fast8_t (C42_CALL * cond_finish)
        (
            c42_smt_t * smt_p,
            c42_smt_cond_t * cond_p
        );
    /* cond_signal **********************************************************/
    /**
     *  Signals a condition variable.
     */
    uint_fast8_t (C42_CALL * cond_signal)
        (
            c42_smt_t * smt_p,
            c42_smt_cond_t * cond_p
        );
    /* cond_wait ************************************************************/
    /**
     *  Waits on a condition variable.
     */
    uint_fast8_t (C42_CALL * cond_wait)
        (
            c42_smt_t * smt_p,
            c42_smt_cond_t * cond_p,
            c42_smt_mutex_t * mutex_p
        );

    size_t mutex_size; /**< size of a mutex */
    size_t cond_size; /**< size of a condition variable */

    void * thread_context; /**< context to be passed to thread functions */
    void * mutex_context; /**< context to be passed to mutex functions */
    void * cond_context; /**< context to be passed to condition variable 
                           functions */
};

/* c42_smt_thread_create ****************************************************/
/**
 *  Creates a thread.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_thread_create
    (
        c42_smt_t * smt_p,
        c42_smt_tid_t * tid_p,
        c42_smt_thread_f func,
        void * arg
    )
{
    return smt_p->thread_create(smt_p, tid_p, func, arg);
}

/* c42_smt_thread_join ******************************************************/
/**
 *  Joins a thread.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_thread_join
    (
        c42_smt_t * smt_p,
        c42_smt_tid_t tid_p,
        uint32_t * exit_code_p
    )
{
    return smt_p->thread_join(smt_p, tid_p, exit_code_p);
}

/* c42_smt_mutex_init *******************************************************/
/**
 *  Inits a mutex.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_mutex_init
    (
        c42_smt_t * smt_p,
        c42_smt_mutex_t * mutex_p
    )
{
    return smt_p->mutex_init(smt_p, mutex_p);
}

/* c42_smt_mutex_finish *****************************************************/
/**
 *  Finishes a mutex.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_mutex_finish
    (
        c42_smt_t * smt_p,
        c42_smt_mutex_t * mutex_p
    )
{
    return smt_p->mutex_finish(smt_p, mutex_p);
}

/* c42_smt_mutex_lock *******************************************************/
/**
 *  Locks mutex.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_mutex_lock
    (
        c42_smt_t * smt_p,
        c42_smt_mutex_t * mutex_p
    )
{
    return smt_p->mutex_lock(smt_p, mutex_p);
}

/* c42_smt_mutex_trylock ****************************************************/
/**
 *  Try to lock a mutex.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_mutex_trylock
    (
        c42_smt_t * smt_p,
        c42_smt_mutex_t * mutex_p
    )
{
    return smt_p->mutex_trylock(smt_p, mutex_p);
}

/* c42_smt_mutex_unlock *****************************************************/
/**
 *  Unlocks mutex.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_mutex_unlock
    (
        c42_smt_t * smt_p,
        c42_smt_mutex_t * mutex_p
    )
{
    return smt_p->mutex_unlock(smt_p, mutex_p);
}

/* c42_smt_mutex_create *****************************************************/
/**
 *  Allocates and inits a mutex.
 */
C42_API uint_fast8_t C42_CALL c42_smt_mutex_create
    (
        c42_smt_mutex_t * * mutex_pp,
        c42_smt_t * smt_p,
        c42_ma_t * ma_p
    );

/* c42_smt_mutex_destroy ****************************************************/
/**
 *  Destroys a mutex.
 */
C42_API uint_fast8_t C42_CALL c42_smt_mutex_destroy
    (
        c42_smt_mutex_t * mutex_p,
        c42_smt_t * smt_p,
        c42_ma_t * ma_p
    );

/* c42_smt_cond_init ********************************************************/
/**
 *  Inits a condition variable.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_cond_init
    (
        c42_smt_t * smt_p,
        c42_smt_cond_t * cond_p
    )
{
    return smt_p->cond_init(smt_p, cond_p);
}

/* c42_smt_cond_finish ******************************************************/
/**
 *  Finish a condition variable.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_cond_finish
    (
        c42_smt_t * smt_p,
        c42_smt_cond_t * cond_p
    )
{
    return smt_p->cond_finish(smt_p, cond_p);
}

/* c42_smt_cond_create ******************************************************/
/**
 *  Creates a condition variable.
 */
C42_API uint_fast8_t C42_CALL c42_smt_cond_create
(
    c42_smt_cond_t * * cond_pp,
    c42_smt_t * smt_p,
    c42_ma_t * ma_p
);

/* c42_smt_cond_destroy *****************************************************/
/**
 *  Destroys a condition variable.
 */
C42_API uint_fast8_t C42_CALL c42_smt_cond_destroy
(
    c42_smt_cond_t * cond_p,
    c42_smt_t * smt_p,
    c42_ma_t * ma_p
);

/* c42_smt_cond_signal ******************************************************/
/**
 *  Signal a condition variable.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_cond_signal
    (
        c42_smt_t * smt_p,
        c42_smt_cond_t * cond_p
    )
{
    return smt_p->cond_signal(smt_p, cond_p);
}

/* c42_smt_cond_wait ********************************************************/
/**
 *  Wait for a condition variable.
 */
C42_INLINE uint_fast8_t C42_CALL c42_smt_cond_wait
    (
        c42_smt_t * smt_p,
        c42_smt_cond_t * cond_p,
        c42_smt_mutex_t * mutex_p
    )
{
    return smt_p->cond_wait(smt_p, cond_p, mutex_p);
}

/** @} */

/* Red/Black Tree ***********************************************************/
/** @defgroup rbtree Red/Black Trees
 * @{
 */

#define C42_RBTREE_MAX_DEPTH 0x40 /**< max depth in the tree */
#define C42_RBTREE_NONE 0 /**< reserved value for no node */
#define C42_RBTREE_LESS 0 /**< returned by c42_rbtree_cmp_f implementations 
                        when the key is less than the key in the given node */
#define C42_RBTREE_MORE 1 /**< returned by c42_rbtree_cmp_f implementations 
                        when the key is bigger than the key in the given node */
#define C42_RBTREE_EQUAL 2 /**< returned by c42_rbtree_cmp_f implementations 
                        when the key is equal than the key in the given node */
#define C42_RBTREE_ERROR 3 /**< returned by c42_rbtree_cmp_f implementations 
                        when there is an error */

#define C42_RBTREE_BLACK 0 /**< color for nodes */
#define C42_RBTREE_RED 1 /**< color for nodes */
#define C42_RBTREE_FOUND 0 /**< returned by c42_rbtree_find() 
                                when key is found */
#define C42_RBTREE_NOT_FOUND 1 /**< returned by c42_rbtree_find()
                                    when key is not found */

/* c42_rbtree_node_t ********************************************************/
/**
 *  Red/black tree node.
 */
typedef struct c42_rbtree_node_s c42_rbtree_node_t;

/* c42_rbtree_cmp_f *********************************************************/
/**
 *  Function to compare a key with a node present in the tree.
 *  @retval C42_RBTREE_LESS if the key is less than the node's key
 *  @retval C42_RBTREE_MORE if the key is greater than the node's key
 *  @retval C42_RBTREE_EQUAL if the key is equal to that in the node
 *  @retval C42_RBTREE_ERROR if the comparison failed for some reason
 */
typedef uint_fast8_t (C42_CALL * c42_rbtree_cmp_f) 
    (
        uintptr_t key,
        c42_rbtree_node_t * node,
        void * restrict context
    );

/* c42_rbtree_path_t ********************************************************/
/**
 *  Identifies a path starting from root and reaching down to some node.
 */
typedef struct c42_rbtree_path_s c42_rbtree_path_t;
struct c42_rbtree_path_s
{
    c42_rbtree_node_t * nodes[C42_RBTREE_MAX_DEPTH]; /**< 
        trail of nodes traversed from the root */
    uint8_t sides[C42_RBTREE_MAX_DEPTH]; /**<
        sides taken to reach the nodes in the trail */
    unsigned int last; /**< index of last node in the path */
};

struct c42_rbtree_node_s
{
    c42_rbtree_node_t * links[2]; /**< left/right links */
    uint8_t red; /**< node is red if 1, black if 0 */
    uint8_t extra1;
    uint16_t extra2;
};

/* c42_rbtree_t *************************************************************/
/**
 *  Red/Black tree.
 */
typedef struct c42_rbtree_s c42_rbtree_t;
struct c42_rbtree_s
{
    c42_rbtree_node_t guard; /**< 
        guard node that holds in its left link the root of the tree */
    c42_rbtree_cmp_f cmp; /**< compares a key with a node */
    void * ctx; /**< context data for callbacks */
};

/* c42_rbtree_init **********************************************************/
/**
 *  Inits a red/black tree.
 */
C42_API void C42_CALL c42_rbtree_init
(
    c42_rbtree_t * restrict tree,
    c42_rbtree_cmp_f cmp,
    void * ctx
);

/* c42_rbtree_find **********************************************************/
/**
 *  Finds a node matching the given key.
 */
C42_API uint_fast8_t C42_CALL c42_rbtree_find
(
    c42_rbtree_path_t * restrict path,
    c42_rbtree_t * restrict tree,
    uintptr_t key
);

/* c42_rbtree_insert ********************************************************/
/**
 *  Inserts a node in the tree using a given path.
 *  The path must be populated by calling c42_rbtree_find().
 */
C42_API void C42_CALL c42_rbtree_insert
(
    c42_rbtree_path_t * restrict path,
    c42_rbtree_node_t * restrict node
);

/* c42_rbtree_delete ********************************************************/
/**
 *  Deletes from the tree the last node in the given path.
 */
C42_API void C42_CALL c42_rbtree_delete
(
    c42_rbtree_path_t * restrict path
);

/* c42_rbtree_np ************************************************************/
/**
 *  Moves to next/previous node.
 */
C42_API c42_rbtree_node_t * C42_CALL c42_rbtree_np
(
  c42_rbtree_path_t * restrict path,
  uint8_t side
);


/** @} */

/* Miscellaneous ************************************************************/
/** @defgroup misc Miscellaneous
 *  @{
 */

/** C42 library major version. */
#define C42_MAJOR 0
/** C42 library minor version */
#define C42_MINOR 0

/* c42_svc_t ****************************************************************/
/**
 *  Structure containing pointers to all interfaces for services defined
 *  in this library.
 */
typedef struct c42_svc_s c42_svc_t;
struct c42_svc_s
{
    uint8_t const * provider;
    /**< NUL-terminated UTF-8 string holding the name of the implementation
     *  for the services provided in this structure
     */
    c42_ma_t ma; /**< mem allocator */
    c42_smt_t smt; /**< simple multithreading interface */
    c42_fsa_t fsa; /**< file system interface */
};

/* c42_io8_std_t ************************************************************/
/**
 *  Standard io8 streams.
 */
typedef struct c42_io8_std_s c42_io8_std_t;
struct c42_io8_std_s
{
    c42_io8_t in; /**< program standard input stream */
    c42_io8_t out; /**< program standard output stream */
    c42_io8_t err; /**< program standard error stream */
};

/* c42_clia_t ***************************************************************/
/**
 *  Command line interface application parameters.
 */
typedef struct c42_clia_s c42_clia_t;
struct c42_clia_s
{
    c42_io8_std_t stdio; /**< standard I/O */
    uint8_t const * program; /**< C-style UTF-8 string with program name */
    uint8_t const * const * args;
        /**< array of C-style UTF-8 strings for arguments */
    size_t argc; /**< number of arguments */
};

/* c42_main *****************************************************************/
/**
 *  Prototype for user-defined main function of c42clia programs.
 */
uint_fast8_t C42_CALL c42_main
(
    c42_svc_t * svc,
    c42_clia_t * clia
);

/* c42_lib_name *************************************************************/
/**
 *  Returns the name of the library as static NUL-terminated UTF-8 string.
 */
C42_API uint8_t const * C42_CALL c42_lib_name ();

/** @} */

#endif /* _C42_H_ */

