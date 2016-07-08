/*
 * Copyright (c) 2009 Baptiste Coudurier <baptiste.coudurier@gmail.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <time.h>
#include "XMFFmpeg/libavutil/internal.h"
#include "XMFFmpeg/libavutil/timer.h"
#include "XMFFmpeg/libavutil/random_seed.h"

#if defined (WINCE)
#define clock()			KD_GET_UST2MSEC
#endif

static int read_random(uint32_t *dst, const char *file)
{
    int fd = open(file, O_RDONLY, 0);
    int err = -1;

    if (fd == -1)
        return -1;
    err = read(fd, dst, sizeof(*dst));
    close(fd);

    return err;
}

static uint32_t get_generic_seed(void)
{
    clock_t last_t  = 0;
    int bits        = 0;
    uint64_t random = 0;
    unsigned i;
    float s = 0.000000000001f;

    for (i = 0; bits < 64; i++) {
        clock_t t = clock();
        if (last_t && fabs((float)(t - last_t)) > s || t == (clock_t) -1) {
            if (i < 10000 && s < (1 << 24)) {
                s += s;
                i = t = 0;
            } else {
                random = 2 * random + (i & 1);
                bits++;
            }
        }
        last_t = t;
    }
#ifdef AV_READ_TIME
    random ^= AV_READ_TIME();
#else
    random ^= clock();
#endif

    random += random >> 32;

    return (uint32_t)random;
}

uint32_t av_get_random_seed(void)
{
    uint32_t seed;

    if (read_random(&seed, "/dev/urandom") == sizeof(seed))
        return seed;
    if (read_random(&seed, "/dev/random")  == sizeof(seed))
        return seed;
    return get_generic_seed();
}
