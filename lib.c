#include "c42.h"

static char const digit_char_table[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* c42_lib_name *************************************************************/
C42_API uint8_t const * C42_CALL c42_lib_name ()
{
    return (uint8_t const *) "c42-v0000"

#if C42_ARM32
        "-arm32"
#elif C42_ARM64
        "-arm64"
#elif C42_MIPS
        "-mips"
#elif C42_AMD64
        "-amd64"
#elif C42_IA32
        "-ia32"
#else
        "-unknown_arch"
#endif

#if C42_BSLE
        "-bsle"
#elif C42_BSBE
        "-bsbe"
#elif C42_WSLE
        "-wsle"
#elif C42_WSBE
        "-wsbe"
#endif

#if C42_STATIC
        "-static"
#else
        "-dynamic"
#endif

#if _DEBUG
        "-debug"
#else
        "-release"
#endif
        ;
}

/* c42_u8a_copy *************************************************************/
C42_API void C42_CALL c42_u8a_copy
(
    uint8_t * restrict dest,
    uint8_t const * restrict src,
    size_t len
)
{
    while (len--) *dest++ = *src++;
}

/* c42_u8a_set **************************************************************/
C42_API void C42_CALL c42_u8a_set
(
    uint8_t * restrict dest,
    uint_fast8_t value,
    size_t len
)
{
    while (len--) *dest++ = value;
}

/* c42_u8a_cmp **************************************************************/
C42_API int C42_CALL c42_u8a_cmp
(
    uint8_t const * a,
    uint8_t const * b,
    size_t len
)
{
    size_t i;
    for (i = 0; i < len; ++i)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : +1;
    return 0;
}

/* c42_u8a_hex **************************************************************/
C42_API uint8_t * C42_CALL c42_u8a_hex
(
    uint8_t * dest,
    uint8_t const * src,
    size_t len
)
{
    uint8_t * out = dest;
    uint8_t const * e = src + len;
    for (; src < e; ++src)
    {
        *out++ = digit_char_table[*src >> 4];
        *out++ = digit_char_table[*src & 15];
    }

    return dest;
}

static uint32_t cet[8] =
{
    0xFFFFFFFF, 0x00000084, 0x10000000, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

static char ec[] = "abtnvfr";

/* byte_c_escape_len ********************************************************/
static unsigned int byte_c_escape_len
(
    uint8_t v
)
{
    return (cet[v >> 5] & (1 << (v & 31))) == 0 ? 1
        : ((v >= 7 && v <= 13) || v >= 32 ? 2: 4);
}

/* c42_u8a_c_escape_len *****************************************************/
C42_API size_t C42_CALL c42_u8a_c_escape_len
(
    uint8_t const * data,
    size_t len
)
{
    uint8_t const * end = data + len;
    size_t elen;
    for (elen = 0; data < end; ++data)
        elen += byte_c_escape_len(*data);
    return elen;
}

/* c42_u8a_c_escape *********************************************************/
C42_API size_t C42_CALL c42_u8a_c_escape
(
    uint8_t * out,
    uint8_t const * data,
    size_t len
)
{
    uint8_t * o;
    uint8_t const * end = data + len;

    for (o = out; data < end; ++data)
    {
        if ((cet[*data >> 5] & (1 << (*data & 0x1F))) == 0) *o++ = *data;
        else
        {
            *o++ = '\\';
            if (*data >= 7 && *data <= 13) *o++ = ec[*data - 7];
            else if (*data == '\\' || *data == '\'' || *data == '"')
                *o++ = *data;
            else
            {
                if (data + 1 == end ||
                    ((data[1] >= '0' && data[1] < '9') ||
                     (data[1] >= 'A' && data[1] < 'F') ||
                     (data[1] >= 'a' && data[1] < 'f')))
                {
                    *o++ = '0' + (*data >> 6);
                    *o++ = '0' + ((*data >> 3) & 7);
                    *o++ = '0' + (*data & 7);
                }
                else
                {
                    *o++ = 'x';
                    *o++ = digit_char_table[*data >> 4];
                    *o++ = digit_char_table[*data & 15];
                }
            }
        }
    }
    *o = 0;

    return o - out;
}

/* c42_u8z_copy *************************************************************/
C42_API uint8_t * C42_CALL c42_u8z_copy
(
    uint8_t * dest,
    uint8_t const * src
)
{
    while (*src) *dest++ = *src++;
    *dest = 0;
    return dest;
}

/* c42_u8z_cmp **************************************************************/
C42_API int C42_CALL c42_u8z_cmp
(
    uint8_t const * a,
    uint8_t const * b
)
{
    for (; *a == *b; a++, b++)
        if (*a == 0) return 0;
    return *a < *b ? -1 : +1;
}

/* c42_u8z_len **************************************************************/
C42_API size_t C42_CALL c42_u8z_len
(
    uint8_t const * a
)
{
    size_t len;
    for (len = 0; a[len]; ++len);
    return len;
}

/* c42_u8z_scan *************************************************************/
C42_API uint8_t * C42_CALL c42_u8z_scan
(
    uint8_t const * str,
    uint8_t val
)
{
    for (; *str; str++)
        if (*str == val) return (uint8_t *) str;
    return NULL;
}

/* c42_u16z_len *************************************************************/
C42_API size_t C42_CALL c42_u16z_len
(
    uint16_t const * a
)
{
    size_t n;
    for (n = 0; a[n]; n++);
    return n;
}

/* c42_u64_to_str ***********************************************************/
C42_API size_t C42_CALL c42_u64_to_str
(
    uint8_t * str,
    uint64_t value,
    uint_fast8_t radix,
    uint32_t width,
    uint_fast8_t group,
    uint_fast8_t sep
)
{
    unsigned int g, a, b, i;

#if _DEBUG
    if (radix < 2 || radix > 36)
    {
        static char bad_radix[] = "BAD_RADIX";
        c42_u8a_copy(str, (uint8_t const *) bad_radix, sizeof bad_radix);
        return sizeof bad_radix - 1;
    }

#endif

    for (i = g = 0; i < width || value; i++)
    {
        uint8_t digit = value % radix;
        value /= radix;
        str[i] = digit_char_table[digit];
        if (++g == group)
        {
            str[++i] = sep;
            g = 0;
        }
    }
    str[i] = 0;
    for (a = 0, b = i - 1; a < b; a++, b--)
    {
        uint8_t ch = str[a];
        str[a] = str[b];
        str[b] = ch;
    }
    return i;
}

/* c42_i64_to_str ***********************************************************/
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
)
{
    uint8_t * start = str;
    size_t w;

    switch (sign_mode)
    {
    case C42_NO_SIGN:
        break;
    case C42_SIGN_NEG:
        if (value >= 0) break;
        value = -value;
        *str++ = '-';
        break;
    case C42_SIGN_ALWAYS:
        if (value < 0)
        {
            *str++ = '-';
            value -= value;
            break;
        }
        *str++ = value ? '+' : ' ';
        break;
    case C42_SIGN_ALIGN:
        if (value < 0)
        {
            *str++ = '-';
            value -= value;
            break;
        }
        *str++ = ' ';
        break;
    }
    if (prefix) str = c42_u8z_copy(str, prefix);
    w = str - start;
    if (w > width) width = 0;
    else width -= w;
    return w + c42_u64_to_str(str, (uint64_t) value, radix, width, group, sep);
}

/* c42_u64_from_str *********************************************************/
C42_API uint_fast8_t C42_CALL c42_u64_from_str
(
    uint8_t const * str,
    size_t len,
    uint_fast8_t radix,
    uint64_t * value,
    size_t * used_len
)
{
    size_t i;
    uint64_t v;
    uint_fast8_t r = 0;

    if (len == 0) return C42_U64_STOP;
    i = 0;
    if (radix == 0)
    {
        if (*str != '0' || len == 1) radix = 10;
        else
        {
            switch (str[1])
            {
            case 'b': radix = 2; i = 2; break;
            case 'o': radix = 8; i = 2; break;
            case 'd': radix = 10; i = 2; break;
            case 'x': radix = 16; i = 2; break;
            default:
                radix = 10;
            }
        }
    }

    for (v = 0; i < len; ++i)
    {
        uint8_t digit;
        uint64_t w;
        if (str[i] >= '0' && str[i] <= '9') digit = str[i] - '0';
        else
        {
            digit = str[i] | 0x20;
            if (digit >= 'a' && digit <= 'z') digit -= ('a' - 10);
            else digit = radix;
        }
        if (digit >= radix) { r = C42_U64_STOP; break; }
        w = v * radix;
        if (w / radix != v) { r = C42_U64_OVERFLOW; break; }
        v = w + digit;
        if (v < w) { r = C42_U64_OVERFLOW; break; }
    }
    *value = v;
    if (used_len) *used_len = i;
    return r;
}

extern uint8_t c42_ucp_width_ofs_a[];
extern uint8_t c42_ucp_width_val_a[];

/* c42_ucp_term_width *******************************************************/
C42_API int C42_CALL c42_ucp_term_width (uint32_t ucp)
{
    return ((int) (c42_ucp_width_val_a[
        ((unsigned int) c42_ucp_width_ofs_a[ucp >> 8] << 6)
        + ((ucp >> 2) & 0x3F)] >> (2 * (ucp & 3))) & 3) - 1;
}

/* c42_utf8_validate ********************************************************/
C42_API uint_fast8_t C42_CALL c42_utf8_validate
(
    uint8_t const * data,
    size_t len,
    size_t * ok_len_p
)
{
    uint8_t const * start = data;
    uint8_t const * end = data + len;

    while (data < end)
    {
        if (data[0] < 0x80) { data++; continue; }
        if (data[0] < 0xC0) { *ok_len_p = data - start; return 2; }
        if (data[0] < 0xE0)
        {
            if ((data[1] & 0xC0) != 0x80)
            {
                if (ok_len_p) *ok_len_p = data - start;
                return 3;
            }
            if (data[0] < 0xC2)
            {
                if (ok_len_p) *ok_len_p = data - start;
                return 5;
            }
            data += 2;
            continue;
        }
        if (data[0] < 0xF0)
        {
            if (((data[1] & 0xC0) != 0x80) || ((data[2] & 0xC0) != 0x80))
            {
                if (ok_len_p) *ok_len_p = data - start;
                return 3;
            }
            if (data[0] == 0xE0 && data[1] < 0xA0)
            {
                if (ok_len_p) *ok_len_p = data - start;
                return 5;
            }
            if (data[0] == 0xED && data[1] >= 0xA0)
            {
                if (ok_len_p) *ok_len_p = data - start;
                return 4;
            }
            data += 3;
            continue;
        }
        if (data[0] < 0xF8)
        {
            if (((data[1] & 0xC0) != 0x80)
                || ((data[2] & 0xC0) != 0x80)
                || ((data[3] & 0xC0) != 0x80))
            {
                if (ok_len_p) *ok_len_p = data - start;
                return 3;
            }
            if (data[0] == 0xF0 && data[1] < 0x90)
            {
                if (ok_len_p) *ok_len_p = data - start;
                return 5;
            }
            data += 4;
            continue;
        }
        if (ok_len_p) *ok_len_p = data - start;
        return 2;
    }
    if (ok_len_p) *ok_len_p = len;
    return 0;
}

/* c42_utf8_term_width ******************************************************/
C42_API int32_t C42_CALL c42_utf8_term_width
(
    uint8_t const * data,
    size_t len,
    void * ctx
)
{
    int32_t w = 0;
    uint8_t const * end = data + len;
    (void) ctx;
    while (data != end)
    {
        uint32_t ucp = c42_ucp_from_valid_utf8(&data);
        int cw = c42_ucp_term_width(ucp);
        if (cw < 0) return -1;
        w += cw;
        if (w < 0) return -2;
    }
    return w;
}

/* c42_utf16le_to_utf8_len **************************************************/
C42_API ptrdiff_t C42_CALL c42_utf16le_to_utf8_len
(
    uint8_t const * data,
    size_t len,
    unsigned int flags
)
{
    uint32_t ucp;
    ptrdiff_t o;
    uint8_t const * end = data + len;
    if ((len & 1) != 0) return -1;
    for (o = 0; data < end && o >= 0; )
    {
        ucp = *data++;
        ucp |= (*data++) << 8;
        if ((ucp & 0xF800) == 0xD800)
        {
            /* surrogate code point */
            if ((flags & C42_NEVER_PAIR_SURROGATES)) o += 3;
            else if (ucp < 0xDC00 && data < end && (data[1] & 0xFC) == 0xDC)
            {
                o += 4;
                data += 2;
            }
            else
            {
                /* hi-value surrogate in lead position - invalid UTF-16 */
                if ((flags & C42_ALLOW_UNPAIRED_SURROGATES) == 0) return -2;
                o += 3;
            }
        }
        else
        {
            /* non-surrogate code point */
            o += ucp >= 0x800 || (!ucp && (flags & C42_TWO_BYTE_NUL)) ? 2 : 1;
        }
    }
    return (o >= 0) ? o : -3;
}

/* c42_utf16le_to_utf8 ******************************************************/
C42_API uint8_t * C42_CALL c42_utf16le_to_utf8
(
    uint8_t * out,
    uint8_t const * data,
    size_t len,
    unsigned int flags
)
{
    uint32_t ucp;
    uint8_t const * end = data + len;
    for (; data < end; )
    {
        ucp = *data++;
        ucp |= (uint32_t) (*data++) << 8;
        if ((ucp & 0xFC00) == 0xD800
            && !(flags & C42_NEVER_PAIR_SURROGATES)
            && data < end
            && (data[1] & 0xFC) == 0xDC)
        {
            /* check for valid surrogate pair */
            ucp = (ucp & 0x3FF) << 10;
            ucp |= *data++;
            ucp |= (*data++ & 3) << 8;
            ucp += 0x10000;
        }
        else if (ucp == 0 && (flags & C42_TWO_BYTE_NUL))
        {
            *out++ = 0xC0;
            *out++ = 0x80;
            continue;
        }
        out += c42_ucp_to_utf8(out, ucp);
    }
    return out;
}
/* c42_sbw_init *************************************************************/
C42_API c42_sbw_t * C42_CALL c42_sbw_init
(
    c42_sbw_t * sbw,
    uint8_t * data,
    size_t limit
)
{
    sbw->data = data;
    sbw->limit = limit;
    sbw->size = 0;
    return sbw;
}

/* c42_sbw_write ************************************************************/
C42_API size_t C42_CALL c42_sbw_write
(
    uint8_t const * data,
    size_t len,
    void * sbw
)
{
    c42_sbw_t * s = sbw;
    if (s->size + len < len) return 0; // int overflow
    if (s->size < s->limit)
    {
        size_t left = s->limit - s->size;
        if (left > len) left = len;
        c42_u8a_copy(s->data + s->size, data, left);
    }
    s->size += len;
    return len;
}

/* c42_write_vfmt ***********************************************************/
#define CMD_NONE 0
#define CMD_BUF 1
#define CMD_STR 2
#define CMD_CONV 3
#define ALIGN_DEFAULT 0
#define ALIGN_LEFT 1
#define ALIGN_RIGHT 2
#define STR_ESC_NONE 0
#define STR_ESC_C 1
#define STR_ESC_HEX 2
C42_API uint_fast8_t C42_CALL c42_write_vfmt
(
    c42_utf8_writer_f writer,
    void * writer_context,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    va_list va
)
{
    static uint8_t const nul = 0;
    static uint8_t const empty_spaces[] =  // what are we living for?
        "                                                                ";
    uint8_t buffer[0x400];
    uint8_t const * f = (uint8_t const *) fmt;
    uint8_t const * str = NULL;
    uint8_t const * num_pfx;
    c42_clconv_f conv = NULL;
    void * conv_ctx;
    uint_fast8_t sep;
    uint_fast8_t group_len;
    char zero_fill;
    char align_mode;
    char sign_mode;
    size_t z, req_width, arg_len, prec;
    int32_t arg_width;
    uint32_t ucp;
    uint8_t radix;
    int64_t i64;
    char cmd;
    char esc_mode;
    size_t ofs, in_len, out_len;
    uint_fast8_t cc;
    c42_clconv_c_escape_t cectx;

    for (;;)
    {
        uint8_t const * sfmt = f;

        while (*f && *f != '$') f++;
        z = f - sfmt;
        if (z && writer(sfmt, z, writer_context) != z)
            return C42_FMT_WRITE_ERROR;
        if (*f == 0) break;
        zero_fill = 0;
        req_width = 0;
        sign_mode = C42_SIGN_NEG;
        align_mode = ALIGN_DEFAULT;
        esc_mode = STR_ESC_NONE;
        radix = 0;
        prec = SIZE_MAX;
        num_pfx = NULL;
        sep = '_';
        group_len = 64;
        arg_width = 0;
        arg_len = 0;
        f++;
        for (cmd = CMD_NONE; cmd == CMD_NONE; ++f)
        {
            switch (*f)
            {
            case '0':
                zero_fill = 1;
                continue;
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                for (req_width = 0; *f >= '0' && *f <= '9'; ++f)
                    req_width = req_width * 10 + *f - '0';
                --f;
                break;
            case 'c':
                ucp = va_arg(va, uint32_t);
                arg_len = c42_ucp_to_utf8(buffer, ucp);
                arg_width = width_func(buffer, arg_len, width_context);
                if (arg_width < 0) return C42_FMT_WIDTH_ERROR;
                cmd = CMD_BUF;
                if (align_mode == ALIGN_DEFAULT) align_mode = ALIGN_LEFT;
                break;
            case 'b':
                sign_mode = C42_NO_SIGN;
                i64 = (uint8_t) va_arg(va, int);
            l_int:
                if (radix == 0) radix = 10;
                arg_len = c42_i64_to_str(buffer, i64, sign_mode, radix, num_pfx,
                                zero_fill ? req_width : 1, group_len, sep);
                arg_width = width_func(buffer, arg_len, width_context);
                if (arg_width < 0) return C42_FMT_WIDTH_ERROR;
                cmd = CMD_BUF;
                if (align_mode == ALIGN_DEFAULT) align_mode = ALIGN_RIGHT;
                break;
            case 'B':
                i64 = (int8_t) va_arg(va, int);
                goto l_int;
            case 'w':
                i64 = (uint16_t) va_arg(va, int);
                goto l_int;
            case 'W':
                i64 = (int16_t) va_arg(va, int);
                goto l_int;
            case 'd':
                i64 = va_arg(va, uint32_t);
                goto l_int;
            case 'D':
                i64 = va_arg(va, int32_t);
                goto l_int;
            case 'q':
                i64 = va_arg(va, uint64_t);
                goto l_int;
            case 'Q':
                i64 = va_arg(va, int64_t);
                goto l_int;
            case 'i':
                i64 = va_arg(va, unsigned int);
                goto l_int;
            case 'I':
                i64 = va_arg(va, signed int);
                goto l_int;
            case 'l':
                i64 = va_arg(va, unsigned long int);
                goto l_int;
            case 'L':
                i64 = va_arg(va, signed long int);
                goto l_int;
            case 'h':
                i64 = (unsigned short int) va_arg(va, int);
                goto l_int;
            case 'H':
                i64 = (signed short int) va_arg(va, int);
                goto l_int;
            case 'z':
                i64 = va_arg(va, size_t);
                goto l_int;
            case 'Z':
                i64 = va_arg(va, ptrdiff_t);
                goto l_int;
            case 'p':
                i64 = va_arg(va, uintptr_t);
                goto l_int;
            case 'P':
                i64 = va_arg(va, intptr_t);
                if (radix == 0) radix = 16;
                goto l_int;
            case 'y':
                num_pfx = (uint8_t const *) "0b";
            case 'Y':
                radix = 2;
                break;
            case 'o':
                num_pfx = (uint8_t const *) "0o";
            case 'O':
                radix = 8;
                break;
            case 'n':
                num_pfx = (uint8_t const *) "0d";
            case 'N':
                radix = 10;
                break;
            case 'x':
                num_pfx = (uint8_t const *) "0x";
            case 'X':
                esc_mode = STR_ESC_HEX;
                radix = 16;
                break;
            case 's':
                str = va_arg(va, uint8_t const *);
                if (prec == SIZE_MAX) arg_len = c42_u8z_len(str);
                else arg_len = prec;
                switch (esc_mode)
                {
                case STR_ESC_NONE:
                    cmd = CMD_STR;
                    arg_width = width_func(str, arg_len, width_context);
                    if (arg_width < 0) return C42_FMT_WIDTH_ERROR;
                    break;
                case STR_ESC_HEX:
                    cmd = CMD_CONV;
                    conv = c42_clconv_bin_to_hex_line;
                    conv_ctx = NULL;
                    break;
                case STR_ESC_C:
                    cmd = CMD_CONV;
                    conv = c42_clconv_c_escape;
                    conv_ctx = c42_clconv_c_escape_init(&cectx);
                    break;
                default:
                    return C42_FMT_NO_CODE;
                }
                if (cmd == CMD_CONV && req_width)
                {
                    int32_t width;
                    for (arg_width = 0, ofs = 0; ofs < arg_len; ofs += in_len)
                    {
                        cc = conv(str + ofs, arg_len - ofs, &in_len,
                                  buffer, sizeof buffer, &out_len, conv_ctx);
                        if (cc && cc != C42_CLCONV_FULL)
                            return C42_FMT_CONV_ERROR;
                        width = width_func(buffer, out_len, width_context);
                        if (width < 0) return C42_FMT_WIDTH_ERROR;
                        arg_width += width;
                    }
                    cc = conv(NULL, 0, &in_len, buffer, sizeof buffer,
                              &out_len, conv_ctx);
                    width = width_func(buffer, out_len, width_context);
                    if (width < 0) return C42_FMT_WIDTH_ERROR;
                    arg_width += width;
                }
                break;
            case '.':
                ++f;
                if (*f == '*')
                {
                    prec = va_arg(va, size_t);
                }
                else
                {
                    for (prec = 0; *f >= '0' && *f <= '9'; ++f)
                        prec = prec * 10 + *f - '0';
                    --f;
                }
                break;
            case '<':
                align_mode = ALIGN_LEFT;
                break;
            case '>':
                align_mode = ALIGN_RIGHT;
                break;
            case '/':
                if (f[1] < '0' || f[1] > '9' ||
                    f[2] == 0) return C42_FMT_MALFORMED;
                group_len = f[1] - '0';
                sep = f[2];
                f += 2;
                break;
            case 'e':
                esc_mode = STR_ESC_C;
                break;
            default:
                return C42_FMT_MALFORMED;
            }
        }

        if ((size_t) arg_width < req_width && align_mode == ALIGN_RIGHT)
        {
            size_t pad_width = req_width - arg_width;
            size_t clen;
            for (; pad_width; pad_width -= clen)
            {
                clen = pad_width;
                if (clen > sizeof(empty_spaces) - 1)
                    clen = sizeof(empty_spaces) - 1;
                if (writer(empty_spaces, clen, writer_context) != clen)
                    return C42_FMT_WRITE_ERROR;
            }
        }

        switch (cmd)
        {
        case CMD_BUF:
            z = writer(buffer, arg_len, writer_context);
            if (z != arg_len) return C42_FMT_WRITE_ERROR;
            break;
        case CMD_STR:
            z = writer(str, arg_len, writer_context);
            if (z != arg_len) return C42_FMT_WRITE_ERROR;
            break;
        case CMD_CONV:
            switch (esc_mode)
            {
            case STR_ESC_C:
                conv_ctx = c42_clconv_c_escape_init(&cectx);
                break;
            default:
                conv_ctx = NULL;
            }

            for (ofs = 0; ofs < arg_len; ofs += in_len)
            {
                cc = conv(str + ofs, arg_len - ofs, &in_len,
                          buffer, sizeof buffer, &out_len, conv_ctx);
                if (cc && cc != C42_CLCONV_FULL) return C42_FMT_CONV_ERROR;
                if (writer(buffer, out_len, writer_context) != out_len)
                    return C42_FMT_WRITE_ERROR;
            }
            cc = conv(NULL, 0, &in_len,
                      buffer, sizeof buffer, &out_len, conv_ctx);
            if (cc) return C42_FMT_CONV_ERROR;
            if (writer(buffer, out_len, writer_context) != out_len)
                return C42_FMT_WRITE_ERROR;
        }

        if ((size_t) arg_width < req_width && align_mode == ALIGN_LEFT)
        {
            size_t pad_width = req_width - arg_width;
            size_t clen;
            for (; pad_width; pad_width -= clen)
            {
                clen = pad_width;
                if (clen > sizeof(empty_spaces) - 1)
                    clen = sizeof(empty_spaces) - 1;
                if (writer(empty_spaces, clen, writer_context) != clen)
                    return C42_FMT_WRITE_ERROR;
            }
        }

    }
    if (writer(&nul, 1, writer_context) != 1) return C42_FMT_WRITE_ERROR;

    return 0;
}
#undef CMD_NONE
#undef CMD_BUF
#undef CMD_STR
#undef CMD_CONV
#undef ALIGN_DEFAULT
#undef ALIGN_LEFT
#undef ALIGN_RIGHT
#undef STR_ESC_NONE
#undef STR_ESC_C
#undef STR_ESC_HEX

/* c42_write_fmt ************************************************************/
C42_API uint_fast8_t C42_CALL c42_write_fmt
(
    c42_utf8_writer_f writer,
    void * writer_context,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    ...
)
{
    va_list va;
    uint_fast8_t rc;

    va_start(va, fmt);
    rc = c42_write_vfmt(writer, writer_context, width_func, width_context,
                        fmt, va);
    va_end(va);
    return rc;
}

/* c42_io8_read *************************************************************/
C42_API uint_fast8_t C42_CALL c42_io8_read
(
    c42_io8_t * io,
    uint8_t * data,
    size_t size,
    size_t * rsize
)
{
    size_t tmp;
    if (rsize == NULL) rsize = &tmp;
    if (size > (SIZE_MAX >> 1)) return C42_IO8_BAD_SIZE;
    if (size == 0) return 0;
#if _DEBUG
    if (io->io8_class->read == NULL) return C42_IO8_NOT_IMPLEMENTED;
#endif
    return io->io8_class->read(io, data, size, rsize);
}

/* c42_io8_read_full ********************************************************/
C42_API uint_fast8_t C42_CALL c42_io8_read_full
(
    c42_io8_t * io,
    uint8_t * data,
    size_t size,
    size_t * rsize
)
{
    size_t r;
    uint8_t * rdata;
    uint8_t * edata;
    uint_fast8_t ioe;

    for (rdata = data, edata = data + size; rdata < edata; rdata += r)
    {
        ioe = c42_io8_read(io, rdata, edata - rdata, &r);
        if (ioe != 0)
        {
            if (ioe != C42_IO8_INTERRUPTED)
            {
                if (rsize) *rsize = rdata - data;
                return ioe;
            }
            r = 0;
        }
        else
        {
            if (rsize) *rsize = rdata - data;
            return C42_IO8_EOF;
        }
    }

    if (rsize) *rsize = rdata - data;
    return 0;
}

/* c42_io8_write ************************************************************/
C42_API uint_fast8_t C42_CALL c42_io8_write
(
    c42_io8_t * io,
    uint8_t const * data,
    size_t size,
    size_t * wsize
)
{
    size_t tmp;
    if (wsize == NULL) wsize = &tmp;
    if (size > (SIZE_MAX >> 1)) return C42_IO8_BAD_SIZE;
    if (size == 0) return 0;
#if _DEBUG
    if (io->io8_class->write == NULL) return C42_IO8_NOT_IMPLEMENTED;
#endif
    return io->io8_class->write(io, data, size, wsize);
}

/* c42_io8_write_full *******************************************************/
C42_API uint_fast8_t C42_CALL c42_io8_write_full
(
    c42_io8_t * io,
    uint8_t const * data,
    size_t size,
    size_t * wsize
)
{
    size_t w;
    uint8_t const * wdata;
    uint8_t const * edata;
    uint_fast8_t ioe;

    for (wdata = data, edata = data + size; wdata < edata; wdata += w)
    {
        ioe = c42_io8_write(io, wdata, edata - wdata, &w);
        if (ioe != 0)
        {
            if (ioe != C42_IO8_INTERRUPTED)
            {
                if (wsize) *wsize = wdata - data;
                return ioe;
            }
            w = 0;
        }
    }

    if (wsize) *wsize = wdata - data;
    return 0;
}

/* c42_io8_write_u8z ********************************************************/
C42_API uint_fast8_t C42_CALL c42_io8_write_u8z
(
    c42_io8_t * io,
    uint8_t const * u8z,
    size_t * wsize
)
{
    size_t size;
    size = c42_u8z_len(u8z);
    return c42_io8_write_full(io, u8z, size, wsize);
}

/* io8_utf8_writer_ctx_t ****************************************************/
typedef struct io8_utf8_writer_ctx_s
{
    c42_io8_t * io;
    uint8_t ioe;
} io8_utf8_writer_ctx_t;

/* io8_utf8_writer **********************************************************/
static size_t C42_CALL io8_utf8_writer
(
    uint8_t const * data,
    size_t len,
    void * ctx
)
{
    io8_utf8_writer_ctx_t * x = ctx;
    uint_fast8_t e;
    size_t w;

    e = c42_io8_write_full(x->io, data, len, &w);
    if (e) x->ioe = e;
    return w;
}

/* c42_io8_wvfmt ************************************************************/
C42_API uint_fast8_t C42_CALL c42_io8_wvfmt
(
    c42_io8_t * io,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    va_list va
)
{
    (void) io;
    (void) width_func;
    (void) width_context;
    (void) fmt;
    (void) va;
    io8_utf8_writer_ctx_t c;
    uint_fast8_t e;
    c.io = io;
    c.ioe = 0;
    e = c42_write_vfmt(io8_utf8_writer, &c, width_func, width_context, fmt, va);
    if (e)
    {
        if (c.ioe) return c.ioe;
        return C42_IO8_FMT_MALFORMED - C42_FMT_MALFORMED + e;
    }
    return 0;
}

/* c42_io8_wfmt *************************************************************/
C42_API uint_fast8_t C42_CALL c42_io8_wfmt
(
    c42_io8_t * io,
    c42_utf8_width_f width_func,
    void * width_context,
    char const * fmt,
    ...
)
{
    va_list va;
    uint_fast8_t rc;
    va_start(va, fmt);
    rc = c42_io8_wvfmt(io, width_func, width_context, fmt, va);
    va_end(va);
    return rc;
}


/* io8bc_write **************************************************************/
static uint_fast8_t C42_CALL io8bc_write
(
    c42_io8_t * io,
    uint8_t const * data,
    size_t size,
    size_t * wsize
)
{
    c42_io8bc_t * a = (c42_io8bc_t *) io;
    if (a->offset + size < size)
    {
        *wsize = 0;
        return C42_IO8_NO_SPACE;
    }
    if (a->offset < a->limit)
    {
        size_t c = a->limit - a->offset;
        if (c > size) c = size;
        c42_u8a_copy(a->data + a->offset, data, c);
    }
    a->offset += size;
    if (a->offset > a->size) a->size = a->offset;
    *wsize = size;
    return 0;
}

/* io8bc_class **************************************************************/
static c42_io8_class_t io8bc_class =
{
    NULL,
    io8bc_write,
    NULL,
    NULL,
    NULL,
    NULL
};

/* c42_io8bc_init ***********************************************************/
C42_API c42_io8_t * C42_CALL c42_io8bc_init
(
    c42_io8bc_t * io8bc,
    uint8_t * data,
    size_t limit
)
{
    io8bc->io8.io8_class = &io8bc_class;
    io8bc->data = data;
    io8bc->limit = limit;
    io8bc->offset = 0;
    io8bc->size = 0;
    return &io8bc->io8;
}

/* c42_clconv_bin_to_hex_line ***********************************************/
C42_API uint_fast8_t C42_CALL c42_clconv_bin_to_hex_line
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
)
{
    size_t i, o, ol;
    (void) ctx;
    ol = out_len & -2;
    for (i = o = 0; i < in_len && o < ol; i++)
    {
        out[o++] = digit_char_table[in[i] >> 4];
        out[o++] = digit_char_table[in[i] & 15];
    }
    *in_used_len = i;
    *out_used_len = o;
    return (i == in_len) ? C42_CLCONV_OK : C42_CLCONV_FULL;
}

/* c42_clconv_hex_to_bin ****************************************************/
C42_API uint_fast8_t C42_CALL c42_clconv_hex_to_bin
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
)
{
    uint8_t const * skip_list = ctx;
    size_t i, o;
    uint_fast8_t r;

    for (r = 0, i = o = 0; i < in_len; i++)
    {
        int lo, hi = c42_digit_from_char(in[i], 16);
        if (hi < 0)
        {
            if (skip_list && c42_u8z_scan(skip_list, in[i])) continue;
            r = C42_CLCONV_MALFORMED;
            break;
        }
        if (++i == in_len)
        {
            r = C42_CLCONV_INCOMPLETE;
            --i;
            break;
        }
        lo = c42_digit_from_char(in[i], 16);
        if (lo < 0)
        {
            r = C42_CLCONV_MALFORMED;
            break;
        }
        if (o == out_len)
        {
            r = C42_CLCONV_FULL;
            --i;
            break;
        }
        out[o++] = (hi << 4) | lo;
    }
    *in_used_len = i;
    *out_used_len = o;
    return r;
}

/* c42_clconv_c_escape_force_hex ********************************************/
C42_API uint_fast8_t C42_CALL c42_clconv_c_escape_force_hex
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
)
{
    size_t i, o;
    uint_fast8_t r;
    (void) ctx;
    for (r = C42_CLCONV_OK, i = o = 0; i < in_len; ++i)
    {
        uint8_t v = in[i];
        unsigned int cl = byte_c_escape_len(v);
        if (o + cl > out_len) { r = C42_CLCONV_FULL; break; }
        if (cl == 1) out[o++] = v;
        else
        {
            out[o++] = '\\';

            if (cl == 4)
            {
                out[o++] = 'x';
                out[o++] = digit_char_table[v >> 4];
                out[o++] = digit_char_table[v & 15];
            }
            else if (v >= 7 && v <= 13) out[o++] = ec[v - 7];
            else out[o++] = v;
        }
    }
    *in_used_len = i;
    *out_used_len = o;
    return r;
}

/* c42_clconv_c_escape_init *************************************************/
C42_API void * C42_CALL c42_clconv_c_escape_init
(
    c42_clconv_c_escape_t * ctx
)
{
    ctx->crt_byte = -1;
    return ctx;
}

/* c42_clconv_c_escape ******************************************************/
C42_API uint_fast8_t C42_CALL c42_clconv_c_escape
(
    uint8_t const * in,
    size_t in_len,
    size_t * in_used_len,
    uint8_t * out,
    size_t out_len,
    size_t * out_used_len,
    void * ctx
)
{
    c42_clconv_c_escape_t * cectx = ctx;
    int v = cectx->crt_byte;
    size_t i, o;
    uint_fast8_t r;

    if (in)
    {
        if (in_len == 0) return 0;
        i = o = 0;
        if (v < 0) v = in[i++];
        for (r = 0;; v = in[i++])
        {
            if (cet[v >> 5] & (1 << (v & 31)))
            {
                // escaping needed
                if (v < 7 || (v >= 14 && v < 32) || v >= 0x7F)
                {
                    // should escape with hex or octal form
                    if (o + 4 > out_len) { r = C42_CLCONV_FULL; break; }
                    if (i == in_len) { r = C42_CLCONV_OK; break; }
                    out[o++] = '\\';
                    if (c42_digit_from_char(in[i], 16) < 0)
                    {
                        out[o++] = 'x';
                        out[o++] = digit_char_table[v >> 4];
                        out[o++] = digit_char_table[v & 15];
                    }
                    else
                    {
                        out[o++] = digit_char_table[v >> 6];
                        out[o++] = digit_char_table[(v >> 3) & 7];
                        out[o++] = digit_char_table[v & 7];
                    }
                }
                else
                {
                    // short escape
                    if (o + 2 > out_len) { r = C42_CLCONV_FULL; break; }
                    out[o++] = '\\';
                    out[o++] = v < 32 ? ec[v - 7] : v;
                    if (i == in_len) { v = -1; r = C42_CLCONV_FULL; break; }
                }
            }
            else
            {
                // non-escaped char
                if (o == out_len) { r = C42_CLCONV_FULL; break; }
                out[o++] = v;
                if (i == in_len) { v = -1; r = C42_CLCONV_FULL; break; }
            }
        }
        cectx->crt_byte = v;
        *in_used_len = i;
        *out_used_len = o;
        return r;
    }
    else
    {
        if (v == -1)
        {
            *out_used_len = 0;
            return 0;
        }
        if (out_len < 4) return C42_CLCONV_FULL;
        out[0] = '\\';
        out[1] = 'x';
        out[2] = digit_char_table[v >> 4];
        out[3] = digit_char_table[v & 15];
        *out_used_len = 4;
        return 0;
    }
}

#define OTHER_SIDE(_s) ((_s) ^ 1)
#define IS_RED(_n) ((_n) && (_n)->red)
#define IS_BLACK(_n) (!(IS_RED(_n)))
/* c42_rbtree_init **********************************************************/
/**
 *  Inits a red/black tree.
 */
C42_API void C42_CALL c42_rbtree_init
(
    c42_rbtree_t * restrict tree,
    c42_rbtree_cmp_f cmp,
    void * ctx
)
{
    tree->cmp = cmp;
    tree->ctx = ctx;
    C42_VAR_CLEAR(tree->guard);
}


/* c42_rbtree_find **********************************************************/
/**
 *  Finds a node matching the given key.
 */
C42_API uint_fast8_t C42_CALL c42_rbtree_find
(
    c42_rbtree_path_t * restrict path,
    c42_rbtree_t * restrict tree,
    uintptr_t key
)
{
    c42_rbtree_cmp_f cmp = tree->cmp;
    void * ctx = tree->ctx;
    c42_rbtree_node_t * n;
    unsigned int i;
    int cr;

    path->nodes[0] = &tree->guard;
    path->sides[0] = 0;
    for (i = 0, n = tree->guard.links[0]; n;)
    {
        path->nodes[++i] = n;
        cr = cmp(key, n + 1, ctx);
        switch (cr)
        {
        case C42_RBTREE_LESS:
        case C42_RBTREE_MORE:
            path->sides[i] = cr;
            n = n->links[cr];
            break;
        case C42_RBTREE_EQUAL:
            path->sides[i] = cr;
            path->last = i;
            return C42_RBTREE_FOUND;
        default:
            path->last = i;
            return C42_RBTREE_ERROR;
        }
    }
    path->last = i;
    return C42_RBTREE_NOT_FOUND;
}

/* c42_rbtree_np ************************************************************/
/**
 *  Moves to next/previous node.
 */
C42_API c42_rbtree_node_t * C42_CALL c42_rbtree_np
(
  c42_rbtree_path_t * restrict path,
  uint8_t side
)
{
  c42_rbtree_node_t * n;
  c42_rbtree_node_t * m;
  unsigned int d;

  d = path->last;
  n = path->nodes[d];
  m = n->links[side];
  if (!m) return NULL;
  path->sides[d] = side;
  side = OTHER_SIDE(side);
  for (; m; m = m->links[side])
  {
    ++d;
    path->nodes[d] = m;
    path->sides[d] = side;
  }
  path->sides[d] = C42_RBTREE_EQUAL;
  path->last = d;
  return path->nodes[d];
}


/* c42_rbtree_insert ********************************************************/
/**
 *  Inserts a node in the tree using a given path.
 *  The path must be populated by calling c42_rbtree_find().
 */
C42_API void C42_CALL c42_rbtree_insert
(
    c42_rbtree_path_t * restrict path,
    c42_rbtree_node_t * restrict node
)
{
  unsigned int i, gs, ps, ts;
  c42_rbtree_node_t * parent;
  c42_rbtree_node_t * grandpa;
  c42_rbtree_node_t * uncle;
  c42_rbtree_node_t * tmp;

  i = path->last;
  node->links[0] = node->links[1] = NULL;
  node->red = 1;
  ts = path->sides[i];
  path->nodes[i]->links[ts] = node;

  while (i)
  {
    parent = path->nodes[i];
    if (!parent->red) return; // 0;
    // parent is red (thus it is not the root)
    grandpa = path->nodes[--i];
    uncle = grandpa->links[OTHER_SIDE(path->sides[i])];
    if (uncle && uncle->red)
    {
      grandpa->red = 1;
      parent->red = 0;
      uncle->red = 0;
      node = grandpa;
      --i;
      continue;
    }
    // parent is red, uncle is black or NULL
    if ((gs = path->sides[i]) != (ps = path->sides[i + 1]))
    {
      // node and parent are on different sides
      grandpa->links[gs] = node;
      parent->links[ps] = tmp = node->links[gs];
      node->links[gs] = parent;

      tmp = node;
      node = parent;
      parent = tmp;
    }
    else ps = OTHER_SIDE(gs);
    // node and parent are on same side: gs; ps is set to the 'other' side
    tmp = parent->links[ps];
    parent->links[ps] = grandpa;
    parent->red = 0;
    grandpa->links[gs] = tmp;
    grandpa->red = 1;
    tmp = path->nodes[i - 1];
    ts = path->sides[i - 1];
    tmp->links[ts] = parent;
    return;
  }
  // if we processed 'til the top of the path then root is changed
  node->red = 0;
  return;
}

/* c42_rbtree_delete ********************************************************/
/**
 *  Deletes from the tree the last node in the given path.
 */
C42_API void C42_CALL c42_rbtree_delete
(
    c42_rbtree_path_t * restrict path
)
{
  c42_rbtree_node_t * o;
  c42_rbtree_node_t * d;
  c42_rbtree_node_t * p;
  c42_rbtree_node_t * c;
  c42_rbtree_node_t * n;
  c42_rbtree_node_t * s;
  c42_rbtree_node_t * sl;
  c42_rbtree_node_t * sr;
  c42_rbtree_node_t * t;
  c42_rbtree_node_t tmp;
  unsigned int cs, ds, od, dd, ns, pd, ps, ss, os;

  od = path->last;
  o = path->nodes[od]; // the node we want to delete

  if (o->links[0] && o->links[1])
  {
    ds = path->sides[od - 1];
    d = c42_rbtree_np(path, (uint8_t) ds);
    // now must delete d which has at most 1 non-null child
    dd = path->last;
    tmp = *o;
    *o = *d;
    *d = tmp;
    path->nodes[od] = d;
    t = path->nodes[od - 1];
    os = path->sides[od - 1];
    t->links[os] = d;
  }
  else { dd = od; }

  d = o;

  cs = d->links[0] ? C42_RBTREE_LESS : C42_RBTREE_MORE;
  c = d->links[cs]; // c is the only child of d or NULL

  ds = path->sides[dd - 1];
  p = path->nodes[dd - 1];
  if (d->red)
  {
    // d has no children since it has at most 1 non-null child and
    // both paths must have same ammount of black nodes
    p->links[ds] = NULL;
    return;
  }
  // d is black; it has either no children, or a single red child with no
  // children of its own

  if (c) // same as: (c && c->red)
  {
    p->links[ds] = c;
    c->red = 0;
    return;
  }

  // d is black and has no children (if c is black and its sibling is NULL then
  // it must be NULL itself)

  p->links[ds] = n = NULL;

  for (pd = dd - 1; pd; p = path->nodes[--pd])
  {
    // n is always black (in first iteration it is NULL)
    ns = path->sides[pd];
    ss = OTHER_SIDE(ns);
    s = p->links[ss]; // this is a non-NULL node
    if (s->red) // implies p is black
    {
      /* del case 2 */
      t = path->nodes[pd - 1];
      ps = path->sides[pd - 1];
      t->links[ps] = s;
      p->links[ss] = sl = s->links[ns];
      p->red = 1;
      s->links[ns] = p;
      s->red = 0;
      path->nodes[pd] = s;
      ++pd;
      s = sl; /* new s is black */
    }
    else
    {
      // s is black
      if (!p->red && IS_BLACK(s->links[0]) && IS_BLACK(s->links[1]))
      {
        /* del case 3 */
        s->red = 1;
        continue;
      }
    }
    // s must be black
    sl = s->links[ns];
    sr = s->links[ss];
    if (p->red && IS_BLACK(sl) && IS_BLACK(sr))
    {
      /* del case 4 */
      p->red = 0;
      s->red = 1;
      break;
    }
    if (IS_RED(sl) && IS_BLACK(sr))
    {
      p->links[ss] = sl;
      s->links[ns] = sl->links[ss];
      s->red = 1;
      sl->links[ss] = s;
      sl->red = 0;
      s = sl; // again new s is black
      sl = s->links[ns];
      sr = s->links[ss];
    }
    if (IS_RED(sr))
    {
      t = path->nodes[pd - 1];
      ps = path->sides[pd - 1];
      t->links[ps] = s;
      p->links[ss] = sl;
      s->red = p->red;
      p->red = 0;
      sr->red = 0;
      s->links[ns] = p;
    }
    break;
  }

  return;
}
#undef OTHER_SIDE
#undef IS_RED
#undef IS_BLACK

