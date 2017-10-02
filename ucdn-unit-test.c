#include <stdlib.h>
#include <check.h>
#include "ucdn.h"

typedef struct {
    uint32_t input;
    uint32_t comp;
} TestTuple;

/*
 * Test cases adapted from HarfBuzz tests/api/test-unicode.c
 */
static const TestTuple combining_class_tests[] = {
    {   0x0020, 0 },
    {   0x0334, 1 },
    {   0x093C, 7 },
    {   0x3099, 8 },
    {   0x094D, 9 },
    {   0x05B0, 10 },
    {   0x05B1, 11 },
    {   0x05B2, 12 },
    {   0x05B3, 13 },
    {   0x05B4, 14 },
    {   0x05B5, 15 },
    {   0x05B6, 16 },
    {   0x05B7, 17 },
    {   0x05B8, 18 },
    {   0x05B9, 19 },
    {   0x05BB, 20 },
    {   0x05BC, 21 },
    {   0x05BD, 22 },
    {   0x05BF, 23 },
    {   0x05C1, 24 },
    {   0x05C2, 25 },
    {   0xFB1E, 26 },
    {   0x064B, 27 },
    {   0x064C, 28 },
    {   0x064D, 29 },
    {   0x05AE, 228 },
    {   0x0300, 230 },
    {   0x302C, 232 },
    {   0x0362, 233 },
    {   0x0360, 234 },
    {   0x0345, 240 },
    {   0x1DCD, 234 },
    {   0xA8E0, 230 },
    {   0x135D, 230 },
    { 0x200000, 0 }
};

/*
 * Test cases adapted from HarfBuzz tests/api/test-unicode.c
 */
static const TestTuple eastasian_width_tests[] =
{
    /* Neutral */
    {   0x0000, UCDN_EAST_ASIAN_N },
    {   0x0483, UCDN_EAST_ASIAN_N },
    {   0x0641, UCDN_EAST_ASIAN_N },
    {   0xFFFC, UCDN_EAST_ASIAN_N },
    {  0x10000, UCDN_EAST_ASIAN_N },
    {  0xE0001, UCDN_EAST_ASIAN_N },

    /* Narrow */
    {   0x0020, UCDN_EAST_ASIAN_NA },
    {   0x0041, UCDN_EAST_ASIAN_NA },
    {   0x27E6, UCDN_EAST_ASIAN_NA },

    /* Halfwidth */
    {   0x20A9, UCDN_EAST_ASIAN_H },
    {   0xFF61, UCDN_EAST_ASIAN_H },
    {   0xFF69, UCDN_EAST_ASIAN_H },
    {   0xFFEE, UCDN_EAST_ASIAN_H },

    /* Ambiguous */
    {   0x00A1, UCDN_EAST_ASIAN_A },
    {   0x00D8, UCDN_EAST_ASIAN_A },
    {   0x02DD, UCDN_EAST_ASIAN_A },
    {  0xE0100, UCDN_EAST_ASIAN_A },
    { 0x100000, UCDN_EAST_ASIAN_A },

    /* Fullwidth */
    {   0x3000, UCDN_EAST_ASIAN_F },
    {   0xFF60, UCDN_EAST_ASIAN_F },

    /* Wide */
    {   0x115F, UCDN_EAST_ASIAN_W },
    {   0x2329, UCDN_EAST_ASIAN_W },
    {   0x3001, UCDN_EAST_ASIAN_W },
    {   0xFE69, UCDN_EAST_ASIAN_W },
    {  0x30000, UCDN_EAST_ASIAN_W },
    {  0x3FFFD, UCDN_EAST_ASIAN_W },
    {  0x2B740, UCDN_EAST_ASIAN_W },
    {  0x1B000, UCDN_EAST_ASIAN_W },

    /* Default Wide blocks */
    {   0x4DBF, UCDN_EAST_ASIAN_W },
    {   0x9FFF, UCDN_EAST_ASIAN_W },
    {   0xFAFF, UCDN_EAST_ASIAN_W },
    {  0x2A6DF, UCDN_EAST_ASIAN_W },
    {  0x2B73F, UCDN_EAST_ASIAN_W },
    {  0x2B81F, UCDN_EAST_ASIAN_W },
    {  0x2FA1F, UCDN_EAST_ASIAN_W },

    /* outside Unicode */
    { 0x200000, UCDN_EAST_ASIAN_N }
};

/*
 * Test cases adapted from HarfBuzz tests/api/test-unicode.c
 */
static const TestTuple general_category_tests[] =
{
    {   0x000D, UCDN_GENERAL_CATEGORY_CC },
    {   0x200E, UCDN_GENERAL_CATEGORY_CF },
    {   0x0378, UCDN_GENERAL_CATEGORY_CN },
    {   0xE000, UCDN_GENERAL_CATEGORY_CO },
    {   0xD800, UCDN_GENERAL_CATEGORY_CS },
    {   0x0061, UCDN_GENERAL_CATEGORY_LL },
    {   0x02B0, UCDN_GENERAL_CATEGORY_LM },
    {   0x3400, UCDN_GENERAL_CATEGORY_LO },
    {   0x01C5, UCDN_GENERAL_CATEGORY_LT },
    {   0xFF21, UCDN_GENERAL_CATEGORY_LU },
    {   0x0903, UCDN_GENERAL_CATEGORY_MC },
    {   0x20DD, UCDN_GENERAL_CATEGORY_ME },
    {   0xA806, UCDN_GENERAL_CATEGORY_MN },
    {   0xFF10, UCDN_GENERAL_CATEGORY_ND },
    {   0x16EE, UCDN_GENERAL_CATEGORY_NL },
    {   0x17F0, UCDN_GENERAL_CATEGORY_NO },
    {   0x005F, UCDN_GENERAL_CATEGORY_PC },
    {   0x058A, UCDN_GENERAL_CATEGORY_PD },
    {   0x0F3B, UCDN_GENERAL_CATEGORY_PE },
    {   0x2019, UCDN_GENERAL_CATEGORY_PF },
    {   0x2018, UCDN_GENERAL_CATEGORY_PI },
    {   0x2016, UCDN_GENERAL_CATEGORY_PO },
    {   0x0F3A, UCDN_GENERAL_CATEGORY_PS },
    {   0x20A0, UCDN_GENERAL_CATEGORY_SC },
    {   0x309B, UCDN_GENERAL_CATEGORY_SK },
    {   0xFB29, UCDN_GENERAL_CATEGORY_SM },
    {   0x00A6, UCDN_GENERAL_CATEGORY_SO },
    {   0x2028, UCDN_GENERAL_CATEGORY_ZL },
    {   0x2029, UCDN_GENERAL_CATEGORY_ZP },
    {   0x202F, UCDN_GENERAL_CATEGORY_ZS },
    {   0x20BF, UCDN_GENERAL_CATEGORY_SC },

    { 0x111111, UCDN_GENERAL_CATEGORY_CN }
};

/*
 * Test cases adapted from HarfBuzz tests/api/test-unicode.c
 */
static const TestTuple script_tests[] =
{
    {   0x002A, UCDN_SCRIPT_COMMON },
    {   0x0670, UCDN_SCRIPT_INHERITED },
    {   0x060D, UCDN_SCRIPT_ARABIC },
    {   0x0559, UCDN_SCRIPT_ARMENIAN },
    {   0x09CD, UCDN_SCRIPT_BENGALI },
    {   0x31B6, UCDN_SCRIPT_BOPOMOFO },
    {   0x13A2, UCDN_SCRIPT_CHEROKEE },
    {   0x2CFD, UCDN_SCRIPT_COPTIC },
    {   0x0482, UCDN_SCRIPT_CYRILLIC },
    {  0x10401, UCDN_SCRIPT_DESERET },
    {   0x094D, UCDN_SCRIPT_DEVANAGARI },
    {   0x1258, UCDN_SCRIPT_ETHIOPIC },
    {   0x10FC, UCDN_SCRIPT_GEORGIAN },
    {  0x10341, UCDN_SCRIPT_GOTHIC },
    {   0x0375, UCDN_SCRIPT_GREEK },
    {   0x0A83, UCDN_SCRIPT_GUJARATI },
    {   0x0A3C, UCDN_SCRIPT_GURMUKHI },
    {   0x3005, UCDN_SCRIPT_HAN },
    {   0x1100, UCDN_SCRIPT_HANGUL },
    {   0x05BF, UCDN_SCRIPT_HEBREW },
    {   0x309F, UCDN_SCRIPT_HIRAGANA },
    {   0x0CBC, UCDN_SCRIPT_KANNADA },
    {   0x30FF, UCDN_SCRIPT_KATAKANA },
    {   0x17DD, UCDN_SCRIPT_KHMER },
    {   0x0EDD, UCDN_SCRIPT_LAO },
    {   0x0061, UCDN_SCRIPT_LATIN },
    {   0x0D3D, UCDN_SCRIPT_MALAYALAM },
    {   0x1843, UCDN_SCRIPT_MONGOLIAN },
    {   0x1031, UCDN_SCRIPT_MYANMAR },
    {   0x169C, UCDN_SCRIPT_OGHAM },
    {  0x10322, UCDN_SCRIPT_OLD_ITALIC },
    {   0x0B3C, UCDN_SCRIPT_ORIYA },
    {   0x16EF, UCDN_SCRIPT_RUNIC },
    {   0x0DBD, UCDN_SCRIPT_SINHALA },
    {   0x0711, UCDN_SCRIPT_SYRIAC },
    {   0x0B82, UCDN_SCRIPT_TAMIL },
    {   0x0C03, UCDN_SCRIPT_TELUGU },
    {   0x07B1, UCDN_SCRIPT_THAANA },
    {   0x0E31, UCDN_SCRIPT_THAI },
    {   0x0FD4, UCDN_SCRIPT_TIBETAN },
    {   0x1401, UCDN_SCRIPT_CANADIAN_ABORIGINAL },
    {   0xA015, UCDN_SCRIPT_YI },
    {   0x1700, UCDN_SCRIPT_TAGALOG },
    {   0x1720, UCDN_SCRIPT_HANUNOO },
    {   0x1740, UCDN_SCRIPT_BUHID },
    {   0x1760, UCDN_SCRIPT_TAGBANWA },

    /* Unicode-4.0 additions */
    {   0x2800, UCDN_SCRIPT_BRAILLE },
    {  0x10808, UCDN_SCRIPT_CYPRIOT },
    {   0x1932, UCDN_SCRIPT_LIMBU },
    {  0x10480, UCDN_SCRIPT_OSMANYA },
    {  0x10450, UCDN_SCRIPT_SHAVIAN },
    {  0x10000, UCDN_SCRIPT_LINEAR_B },
    {   0x1950, UCDN_SCRIPT_TAI_LE },
    {  0x1039F, UCDN_SCRIPT_UGARITIC },

    /* Unicode-4.1 additions */
    {   0x1980, UCDN_SCRIPT_NEW_TAI_LUE },
    {   0x1A1F, UCDN_SCRIPT_BUGINESE },
    {   0x2C00, UCDN_SCRIPT_GLAGOLITIC },
    {   0x2D6F, UCDN_SCRIPT_TIFINAGH },
    {   0xA800, UCDN_SCRIPT_SYLOTI_NAGRI },
    {  0x103D0, UCDN_SCRIPT_OLD_PERSIAN },
    {  0x10A3F, UCDN_SCRIPT_KHAROSHTHI },

    /* Unicode-5.0 additions */
    {   0x0378, UCDN_SCRIPT_UNKNOWN },
    {   0x1B04, UCDN_SCRIPT_BALINESE },
    {  0x12000, UCDN_SCRIPT_CUNEIFORM },
    {  0x10900, UCDN_SCRIPT_PHOENICIAN },
    {   0xA840, UCDN_SCRIPT_PHAGS_PA },
    {   0x07C0, UCDN_SCRIPT_NKO },

    /* Unicode-5.1 additions */
    {   0xA900, UCDN_SCRIPT_KAYAH_LI },
    {   0x1C00, UCDN_SCRIPT_LEPCHA },
    {   0xA930, UCDN_SCRIPT_REJANG },
    {   0x1B80, UCDN_SCRIPT_SUNDANESE },
    {   0xA880, UCDN_SCRIPT_SAURASHTRA },
    {   0xAA00, UCDN_SCRIPT_CHAM },
    {   0x1C50, UCDN_SCRIPT_OL_CHIKI },
    {   0xA500, UCDN_SCRIPT_VAI },
    {  0x102A0, UCDN_SCRIPT_CARIAN },
    {  0x10280, UCDN_SCRIPT_LYCIAN },
    {  0x1093F, UCDN_SCRIPT_LYDIAN },

    /* Unicode-5.2 additions */
    {  0x10B00, UCDN_SCRIPT_AVESTAN },
    {   0xA6A0, UCDN_SCRIPT_BAMUM },
    {  0x13000, UCDN_SCRIPT_EGYPTIAN_HIEROGLYPHS },
    {  0x10840, UCDN_SCRIPT_IMPERIAL_ARAMAIC },
    {  0x10B60, UCDN_SCRIPT_INSCRIPTIONAL_PAHLAVI },
    {  0x10B40, UCDN_SCRIPT_INSCRIPTIONAL_PARTHIAN },
    {   0xA980, UCDN_SCRIPT_JAVANESE },
    {  0x11082, UCDN_SCRIPT_KAITHI },
    {   0xA4D0, UCDN_SCRIPT_LISU },
    {   0xABE5, UCDN_SCRIPT_MEETEI_MAYEK },
    {  0x10A60, UCDN_SCRIPT_OLD_SOUTH_ARABIAN },
    {  0x10C00, UCDN_SCRIPT_OLD_TURKIC },
    {   0x0800, UCDN_SCRIPT_SAMARITAN },
    {   0x1A20, UCDN_SCRIPT_TAI_THAM },
    {   0xAA80, UCDN_SCRIPT_TAI_VIET },

    /* Unicode-6.0 additions */
    {   0x1BC0, UCDN_SCRIPT_BATAK },
    {  0x11000, UCDN_SCRIPT_BRAHMI },
    {   0x0840, UCDN_SCRIPT_MANDAIC },

    /* Unicode-5.2 character additions */
    {   0x1CED, UCDN_SCRIPT_INHERITED },
    {   0x1400, UCDN_SCRIPT_CANADIAN_ABORIGINAL },

    /* Unicode-10.0 additions */
    {   0x0860, UCDN_SCRIPT_SYRIAC },
    {  0x11a00, UCDN_SCRIPT_ZANABAZAR_SQUARE },
    {  0x11a50, UCDN_SCRIPT_SOYOMBO },
    {  0x11d00, UCDN_SCRIPT_MASARAM_GONDI },
    {  0x1b170, UCDN_SCRIPT_NUSHU },
    {  0x2ceb0, UCDN_SCRIPT_HAN },
    {  0x1f92e, UCDN_SCRIPT_COMMON },
    {   0x20bf, UCDN_SCRIPT_COMMON },

    { 0x111111, UCDN_SCRIPT_UNKNOWN }
};

START_TEST(test_basic)
{
    /* check versio */
    ck_assert_str_eq(ucdn_get_unicode_version(), "10.0.0");

    /* one sample check inside BMP for each property */
    ck_assert_int_eq(ucdn_get_general_category(0x0040), 21);
    ck_assert_int_eq(ucdn_get_script(0x0122), 1);
    ck_assert_int_eq(ucdn_get_bidi_class(0x0032), 8);
    ck_assert_int_eq(ucdn_get_east_asian_width(0x4000), 2);
    ck_assert_int_eq(ucdn_get_linebreak_class(0xfeff), 22);
    ck_assert_int_eq(ucdn_get_mirrored(0x0028), 1);
    ck_assert_int_eq(ucdn_get_combining_class(0), 0);

    /* check validity in various blocks and planes outside BMP */
    ck_assert_int_eq(ucdn_get_script(0x103a0), 59); /* SMP, Old Persian */
    ck_assert_int_eq(ucdn_get_script(0x14400), 127); /* SMP, Anatolian Hieroglyphs */
    ck_assert_int_eq(ucdn_get_script(0x1e910), 132); /* SMP, Adlam */
    ck_assert_int_eq(ucdn_get_script(0x20100), 35); /* SIP, CJK Unified Ideographs */
    ck_assert_int_eq(ucdn_get_script(0x28100), 35); /* SIP, CJK Unified Ideographs */
    ck_assert_int_eq(ucdn_get_script(0x2f810), 35); /* SIP, CJK Compatibility Ideographs */
    ck_assert_int_eq(ucdn_get_general_category(0xe0020), 1); /* SSP, Tags */

    /* check error behavior */
    ck_assert_int_eq(ucdn_get_general_category(0xfefe), 2); /* unassigned */
    ck_assert_int_eq(ucdn_get_general_category(0x200000), 2);  /* outside Unicode */

    /* boundaries */
    ck_assert_int_eq(ucdn_get_general_category(0x10FFFF), 2); /* last valid codepoint */
    ck_assert_int_eq(ucdn_get_general_category(0x110000), 2); /* first invalid codepoint */
}
END_TEST

START_TEST(test_general_category)
{
    ck_assert_int_eq(ucdn_get_general_category(general_category_tests[_i].input), general_category_tests[_i].comp);
}
END_TEST

START_TEST(test_script)
{
    ck_assert_int_eq(ucdn_get_script(script_tests[_i].input), script_tests[_i].comp);
}
END_TEST

START_TEST(test_combining_class)
{
    ck_assert_int_eq(ucdn_get_combining_class(combining_class_tests[_i].input), combining_class_tests[_i].comp);
}
END_TEST

START_TEST(test_eastasian_width)
{
    ck_assert_int_eq(ucdn_get_east_asian_width(eastasian_width_tests[_i].input), eastasian_width_tests[_i].comp);
}
END_TEST

START_TEST(test_decompose_basic)
{
    int ret;
    uint32_t a, b;

    ret = ucdn_decompose(0x00c4, &a, &b); ck_assert(ret && a == 0x0041 && b == 0x0308); /* normal case */
    ret = ucdn_decompose(0xfb01, &a, &b); ck_assert(ret == 0); /* no decomposition (only compatibility) */
    ret = ucdn_decompose(0x0065, &a, &b); ck_assert(ret == 0); /* no decomposition */

    /* multi-part sequence */
    ret = ucdn_decompose(0xfb2c, &a, &b);
    ck_assert(ret && a == 0xfb49 && b == 0x05c1);
    ret = ucdn_decompose(a, &a, &b);
    ck_assert(ret && a == 0x05e9 && b == 0x05bc);
}
END_TEST

START_TEST(test_decompose_hangul)
{
    int ret;
    uint32_t a, b;

    ret = ucdn_decompose(0xac01, &a, &b); ck_assert(ret && a == 0xac00 && b == 0x11a8); /* normal case (LV,T) */
    ret = ucdn_decompose(0xd7a3, &a, &b); ck_assert(ret && a == 0xd788 && b == 0x11c2); /* normal case (LV,T) */
    ret = ucdn_decompose(0xac00, &a, &b); ck_assert(ret && a == 0x1100 && b == 0x1161); /* normal case (L,V) */
    ret = ucdn_decompose(0xd7a4, &a, &b); ck_assert(ret == 0); /* invalid Jamo (unassigned) */
}
END_TEST

START_TEST(test_decompose_compat)
{
    int ret;
    uint32_t decomp[18] = {0};

    ret = ucdn_compat_decompose(0x00c4, decomp); ck_assert(ret == 2 && decomp[0] == 0x0041 && decomp[1] == 0x0308); /* normal case */
    ret = ucdn_compat_decompose(0x0065, decomp); ck_assert(ret == 0); /* no decomposition */
}
END_TEST

START_TEST(test_decompose_special)
{
    int ret;
    uint32_t a, b;
    uint32_t decomp[18] = {0};
    uint32_t long_cmp[] = {0x0635, 0x0644, 0x0649, 0x0020, 0x0627, 0x0644, 0x0644, 0x0647, 0x0020, 0x0639, 0x0644, 0x064A, 0x0647, 0x0020, 0x0648, 0x0633, 0x0644, 0x0645};

    /* outside BMP */
    ret = ucdn_decompose(0x1109a, &a, &b); ck_assert(ret && a == 0x11099 && b == 0x110ba);
    ret = ucdn_compat_decompose(0x2fa1d, decomp); ck_assert(ret == 1 && decomp[0] == 0x2a600);

    /* very long decomposition */
    ret = ucdn_compat_decompose(0xfdfa, decomp); ck_assert(ret == 18 && memcmp(decomp, long_cmp, 18) == 0);

    /* very short (single character) decomposition */
    ret = ucdn_decompose(0x212b, &a, &b); ck_assert(ret && a == 0x00c5 && b == 0);

    /* outside Unicode */
    ret = ucdn_compat_decompose(0x200000, decomp); ck_assert(ret == 0); /* no decomposition */
}
END_TEST

START_TEST(test_compose_basic)
{
    int ret;
    uint32_t a;

    ret = ucdn_compose(&a, 0x0041, 0x0308); ck_assert(ret == 1 && a == 0x00c4); /* normal case */
    ret = ucdn_compose(&a, 0x0066, 0x0069); ck_assert(ret == 0); /* compatibility decomposition => no recomposition */
    ret = ucdn_compose(&a, 0x0028, 0x0028); ck_assert(ret == 0); /* no composition exists */
    ret = ucdn_compose(&a, 0x200000, 0x0028); ck_assert(ret == 0); /* outside Unicode */

    /* multi-part sequence */
    ret = ucdn_compose(&a, 0x0041, 0x0308);
    ck_assert(ret == 1 && a == 0x00c4);
    ret = ucdn_compose(&a, a, 0x0304);
    ck_assert(ret == 1 && a == 0x01de);
}
END_TEST

START_TEST(test_compose_hangul)
{
    int ret;
    uint32_t a;

    ret = ucdn_compose(&a, 0xac00, 0x11a8); ck_assert(ret == 1 && a == 0xac01); /* normal case (LV,T) */
    ret = ucdn_compose(&a, 0x1100, 0x1161); ck_assert(ret == 1 && a == 0xac00); /* normal case (L,V) */
    ret = ucdn_compose(&a, 0xd788, 0x11a3); ck_assert(ret == 0); /* invalid Jamo combination (LV,T with invalid T) */
}
END_TEST

START_TEST(test_compose_special)
{
    int ret;
    uint32_t a;

    /* excluded compositions */
    ret = ucdn_compose(&a, 0xfb49, 0x05c1); ck_assert(ret == 0);
    ret = ucdn_compose(&a, 0x2add, 0x0338); ck_assert(ret == 0);

    /* outside BMP */
    ret = ucdn_compose(&a, 0x11099, 0x110ba); ck_assert(ret == 1 && a == 0x1109a);
}
END_TEST

START_TEST(test_mirror)
{
    /* normal, characters that mirror */
    ck_assert_int_eq(ucdn_mirror(0x0028), 0x0029);
    ck_assert_int_eq(ucdn_mirror(0x0029), 0x0028);
    ck_assert_int_eq(ucdn_mirror(0x00ab), 0x00bb);
    ck_assert_int_eq(ucdn_mirror(0x223d), 0x223c);
    ck_assert_int_eq(ucdn_mirror(0x223c), 0x223d);
    ck_assert_int_eq(ucdn_mirror(0x22f0), 0x22f1);
    ck_assert_int_eq(ucdn_mirror(0x22f1), 0x22f0);
    ck_assert_int_eq(ucdn_mirror(0x2aa1), 0x2aa2);
    ck_assert_int_eq(ucdn_mirror(0x2aa2), 0x2aa1);
    ck_assert_int_eq(ucdn_mirror(0xff62), 0xff63);
    ck_assert_int_eq(ucdn_mirror(0xff63), 0xff62);
    ck_assert_int_eq(ucdn_mirror(0x223d), 0x223c);
    ck_assert_int_eq(ucdn_mirror(0x223d), 0x223c);

    /* no mirroring exists */
    ck_assert_int_eq(ucdn_mirror(0x0020), 0x0020);
    ck_assert_int_eq(ucdn_mirror(0x0032), 0x0032);
    ck_assert_int_eq(ucdn_mirror(0x0041), 0x0041);
    ck_assert_int_eq(ucdn_mirror(0x00f0), 0x00f0);
    ck_assert_int_eq(ucdn_mirror(0x27cc), 0x27cc);
    ck_assert_int_eq(ucdn_mirror(0xe01ef), 0xe01ef);
    ck_assert_int_eq(ucdn_mirror(0x1d7c3), 0x1d7c3);

    /* special cases */
    ck_assert_int_eq(ucdn_mirror(0x200000), 0x200000); /* outside Unicode */

}
END_TEST

START_TEST(test_bidi_bracket)
{
    ck_assert_int_eq(ucdn_paired_bracket(0x0028), 0x0029); /* normal case */
    ck_assert_int_eq(ucdn_paired_bracket(0xff08), 0xff09); /* normal case */
    ck_assert_int_eq(ucdn_paired_bracket(0x00ab), 0x00ab); /* mirrored, but not a bracket */
    ck_assert_int_eq(ucdn_paired_bracket(0x200000), 0x200000); /* outside Unicode */

    ck_assert_int_eq(ucdn_paired_bracket_type(0x0028), UCDN_BIDI_PAIRED_BRACKET_TYPE_OPEN); /* normal case */
    ck_assert_int_eq(ucdn_paired_bracket_type(0x0029), UCDN_BIDI_PAIRED_BRACKET_TYPE_CLOSE); /* normal case */
    ck_assert_int_eq(ucdn_paired_bracket_type(0x0020), UCDN_BIDI_PAIRED_BRACKET_TYPE_NONE); /* normal case */
    ck_assert_int_eq(ucdn_paired_bracket_type(0x200000), UCDN_BIDI_PAIRED_BRACKET_TYPE_NONE); /* outside Unicode */
}
END_TEST

START_TEST(test_linebreak_class)
{
    ck_assert_int_eq(ucdn_get_linebreak_class(0x0020), UCDN_LINEBREAK_CLASS_SP); /* normal case */
    ck_assert_int_eq(ucdn_get_linebreak_class(0xffef), UCDN_LINEBREAK_CLASS_XX); /* unassigned */
    ck_assert_int_eq(ucdn_get_linebreak_class(0xd800), UCDN_LINEBREAK_CLASS_SG); /* surrogate */
    ck_assert_int_eq(ucdn_get_linebreak_class(0x3400), UCDN_LINEBREAK_CLASS_ID); /* not part of database */
    ck_assert_int_eq(ucdn_get_linebreak_class(0x1f46e), UCDN_LINEBREAK_CLASS_EB); /* outside BMP, unusual class */
    ck_assert_int_eq(ucdn_get_linebreak_class(0x200000), UCDN_LINEBREAK_CLASS_XX); /* outside of Unicode */
}
END_TEST

START_TEST(test_resolved_linebreak_class)
{
    /* cases that resolve to AL */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0xa7), UCDN_LINEBREAK_CLASS_AL); /* AI */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0xd801), UCDN_LINEBREAK_CLASS_AL); /* SG */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0xffef), UCDN_LINEBREAK_CLASS_AL); /* XX */

    /* cases that resolve to CM/AL */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0x0e31), UCDN_LINEBREAK_CLASS_CM); /* category MN */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0x1a55), UCDN_LINEBREAK_CLASS_CM); /* category MC */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0x19da), UCDN_LINEBREAK_CLASS_AL); /* other category */

    /* cases that resolve to NS */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0x3041), UCDN_LINEBREAK_CLASS_NS); /* CJ */

    /* cases that resolve to B2 */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0xfffc), UCDN_LINEBREAK_CLASS_B2); /* CB */

    /* cases that resolve to BK */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0x0085), UCDN_LINEBREAK_CLASS_BK); /* NL */

    /* other, not further resolved */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0x0066a), UCDN_LINEBREAK_CLASS_PO); /* PO */

    /* error case */
    ck_assert_int_eq(ucdn_get_resolved_linebreak_class(0x200000), UCDN_LINEBREAK_CLASS_AL); /* outside of Unicode */
}
END_TEST

int main(int argc, char **argv)
{
    SRunner *sr;
    int nfailed;

    Suite *s = suite_create("all_tests");
    TCase *t = tcase_create("all_tests");
    tcase_add_test(t, test_basic);
    tcase_add_loop_test(t, test_general_category, 0, sizeof(general_category_tests) / sizeof(TestTuple));
    tcase_add_loop_test(t, test_script, 0, sizeof(script_tests) / sizeof(TestTuple));
    tcase_add_loop_test(t, test_combining_class, 0, sizeof(combining_class_tests) / sizeof(TestTuple));
    tcase_add_loop_test(t, test_eastasian_width, 0, sizeof(eastasian_width_tests) / sizeof(TestTuple));
    tcase_add_test(t, test_decompose_basic);
    tcase_add_test(t, test_decompose_hangul);
    tcase_add_test(t, test_decompose_compat);
    tcase_add_test(t, test_decompose_special);
    tcase_add_test(t, test_compose_basic);
    tcase_add_test(t, test_compose_hangul);
    tcase_add_test(t, test_compose_special);
    tcase_add_test(t, test_mirror);
    tcase_add_test(t, test_bidi_bracket);
    tcase_add_test(t, test_linebreak_class);
    tcase_add_test(t, test_resolved_linebreak_class);
    suite_add_tcase(s, t);
    
    sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    nfailed = srunner_ntests_failed(sr);
    return (nfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
