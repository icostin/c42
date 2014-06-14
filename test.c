#include <stdio.h>
#include <c42.h>

#define T(_cond) \
    if ((_cond)) ; else \
        do { printf("Error: test failed: (%s) - line %u\n", #_cond, __LINE__); \
            return 1; } while (0)

int main ()
{
    uint8_t buf[0x400];
    unsigned int z;
    static uint8_t const s[] = "\a\b\t'\n\"\\\v\f\r\033\[0m\xAB\x43\xCD";
    c42_sbw_t sbw;

    printf("c42: %s\n", c42_lib_name());

    z = c42_u64_to_str(buf, 0x123456789ABCDEFLL, 16, 40, 4, '_');
    printf("a: %s (%u)\n", buf, z);
    z = c42_u64_to_str(buf, 0x123456789ABCDEFLL, 10, 40, 3, ',');
    printf("a: %s (%u)\n", buf, z);
    z = c42_u64_to_str(buf, 0x123456789ABCDEFLL, 2, 40, 1, ':');
    printf("a: %s (%u)\n", buf, z);
    z = c42_i64_to_str(buf, 0x123456789ABCDEFLL, C42_SIGN_NEG, 10, (uint8_t const *) "0d", 40, 3, ',');
    printf("a: %s (%u)\n", buf, z);
    z = c42_i64_to_str(buf, 0x123456789ABCDEFLL, C42_SIGN_ALIGN, 10, (uint8_t const *) "0d", 40, 3, ',');
    printf("a: %s (%u)\n", buf, z);
    z = c42_i64_to_str(buf, 0x123456789ABCDEFLL, C42_SIGN_ALWAYS, 10, (uint8_t const *) "0d", 40, 3, ',');
    printf("a: %s (%u)\n", buf, z);
    z = c42_i64_to_str(buf, 0xFEDCBA9876543210LL, C42_SIGN_NEG, 2, (uint8_t const *) "0b", 40, 1, ':');
    printf("a: %s (%u)\n", buf, z);
    z = c42_i64_to_str(buf, 0xFEDCBA9876543210LL, C42_SIGN_ALWAYS, 16, (uint8_t const *) "0x", 40, 4, ':');
    printf("a: %s (%u)\n", buf, z);
    z = c42_i64_to_str(buf, 0xFEDCBA9876543210LL, C42_NO_SIGN, 16, (uint8_t const *) "0x", 39, 4, ':');
    printf("a: %s (%u)\n", buf, z);
    z = c42_u8a_c_escape(buf, s, sizeof(s));
    printf("ce: \"%s\"\n", buf);

    c42_write_fmt(c42_sbw_write, c42_sbw_init(&sbw, buf, sizeof(buf)),
                  c42_utf8_term_width, NULL, 
                  "It happens$4c Rama ding ding! b=$05B s=$>5.3s\n", '!', 0xCC, "cucu");
    printf("wf: %s\n", buf);

    c42_write_fmt(c42_sbw_write, c42_sbw_init(&sbw, buf, sizeof(buf)),
                  c42_utf8_term_width, NULL, 
                  "d=$d xd=$Xd D=$/3,D XD=$X/4:D",
                  0xABCDEF01, 0xABCDEF01, 0xABCDEF01, 0xABCDEF01);
    printf("wf: %s\n", buf);
    c42_write_fmt(c42_sbw_write, c42_sbw_init(&sbw, buf, sizeof(buf)),
                  c42_utf8_term_width, NULL, 
                  "hex=$>20es", "\0033-rahat\n\xAB");
    printf("wf: %s\n", buf);

    T(c42_utf16le_to_utf8_len((uint8_t const *) "\x00\xD8", 1, 0) == -1);
    T(c42_utf16le_to_utf8_len((uint8_t const *) "\x00\xD8", 2, 0) == -2);
    T(c42_utf16le_to_utf8_len((uint8_t const *) "\x00\xD8", 2, 
                              C42_ALLOW_UNPAIRED_SURROGATES) == 3);
    T(c42_utf16le_to_utf8_len((uint8_t const *) "\x45\xDB\x0C\xDC", 4, 0) == 4);
    T(c42_utf16le_to_utf8_len((uint8_t const *) "\x45\xDB\x0C\xDC", 4, 
                              C42_NEVER_PAIR_SURROGATES) == 6);
    T(c42_utf16le_to_utf8_len((uint8_t const *) "\x0C\xDC\x45\xDB", 4, 0) == -2);
    printf("all done!\n");
    return 0;
}

