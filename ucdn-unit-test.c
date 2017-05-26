#include <stdlib.h>
#include <check.h>
#include "ucdn.h"

START_TEST(test_basic)
{
    /* check versio */
    ck_assert_str_eq(ucdn_get_unicode_version(), "9.0.0");

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
    ck_assert_int_eq(ucdn_mirror(0x0028), 0x0029); /* normal case */
    ck_assert_int_eq(ucdn_mirror(0x223d), 0x0223c); /* normal case */
    ck_assert_int_eq(ucdn_mirror(0x0032), 0x0032); /* no mirroring exists */
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
    #if 0
    ck_assert_int_eq(ucdn_get_linebreak_class(0x1f46e), UCDN_LINEBREAK_CLASS_EB); /* outside BMP, unusual class */
    #endif
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
