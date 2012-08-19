/*
 * Copyright (C) 2012 Grigori Goronzy <greg@kinoho.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ucdn.h"

typedef struct {
    const unsigned char category;
    const unsigned char combining;
    const unsigned char bidi_class;
    const unsigned char mirrored;
    const unsigned char east_asian_width;
    const unsigned char normalization_check;
    const unsigned char script;
} UCDRecord;

typedef struct {
    unsigned short from, to;
} MirrorPair;

typedef struct {
    int start;
    short count, index;
} Reindex;

#include "unicodedata_db.h"

static UCDRecord *get_ucd_record(uint32_t code)
{
    int index;

    if (code >= 0x110000)
        index = 0;
    else {
        index = index1[(code>>SHIFT)];
        index = index2[(index<<SHIFT) + (code & ((1<<SHIFT) - 1))];
    }

    return &ucd_records[index];
}

static unsigned int *get_decomp_record(uint32_t code)
{
    int index;

    if (code >= 0x110000)
        index = 0;
    else {
        index = decomp_index1[(code>>DECOMP_SHIFT)];
        index = decomp_index2[(index<<DECOMP_SHIFT) + (code&((1<<DECOMP_SHIFT) - 1))];
    }

    return &decomp_data[index];
}

static int get_comp_index(uint32_t code, Reindex *idx)
{
    int i;

    for (i = 0; idx[i].start; i++) {
        Reindex *cur = &idx[i];
        if (code < cur->start)
            return -1;
        if (code <= cur->start + cur->count) {
            return cur->index + (code - cur->start);
        }
    }

    return -1;
}

static int compare_mp(const void *a, const void *b)
{
    MirrorPair *mpa = (MirrorPair *)a;
    MirrorPair *mpb = (MirrorPair *)b;
    return mpa->from - mpb->from;
}

const char *ucdn_get_unicode_version(void)
{
    return UNIDATA_VERSION;
}

int ucdn_get_combining_class(uint32_t code)
{
    return get_ucd_record(code)->combining;
}

int ucdn_get_east_asian_width(uint32_t code)
{
    return get_ucd_record(code)->east_asian_width;
}

int ucdn_get_general_category(uint32_t code)
{
    return get_ucd_record(code)->category;
}

int ucdn_get_bidi_class(uint32_t code)
{
    return get_ucd_record(code)->bidi_class;
}

int ucdn_get_mirrored(uint32_t code)
{
    return get_ucd_record(code)->mirrored;
}

uint32_t ucdn_mirror(uint32_t code)
{
    if (get_ucd_record(code)->mirrored == 0)
        return code;

    MirrorPair mp = {code, 0};
    MirrorPair *res;
    res = bsearch(&mp, mirror_pairs, BIDI_MIRROR_LEN, sizeof(MirrorPair),
            compare_mp);

    if (res == NULL)
        return code;
    else
        return res->to;
}

int ucdn_get_script(uint32_t code)
{
    return get_ucd_record(code)->script;
}

int ucdn_decompose(uint32_t code, uint32_t *a, uint32_t *b)
{
    unsigned int *rec = get_decomp_record(code);
    int len = rec[0] >> 8;

    if ((rec[0] & 0xff) != 0 || len == 0)
        return 0;

    /* TODO: Hangul Jamo decomposition */

    *a = rec[1];
    if (len > 1)
        *b = rec[2];
    else
        *b = 0;

    return 1;
}

int ucdn_compose(uint32_t *code, uint32_t a, uint32_t b)
{
    int l, r, index, indexi;

    l = get_comp_index(a, nfc_first);
    r = get_comp_index(b, nfc_last);

    if (l < 0 || r < 0)
        return 0;

    index = l * TOTAL_LAST + r;
    indexi = comp_index[(index>>COMP_SHIFT)];
    *code = comp_data[(indexi<<COMP_SHIFT) + (index & ((1<<COMP_SHIFT) - 1))];

    return *code != 0;
}

int ucdn_compat_decompose(uint32_t code, uint32_t *decomposed)
{
    int i, len;
    unsigned int *rec = get_decomp_record(code);
    len = rec[0] >> 8;

    if (len == 0)
        return 0;

    /* TODO: Hangul Jamo decomposition */

    for (i = 0; i < len; i++)
        decomposed[i] = rec[i + 1];

    return len;
}
