/*
 * adaptive and fixed codebook vector operations for ACELP-based codecs
 *
 * Copyright (c) 2008 Vladimir Voroshilov
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

#include "internal.h"
#include "acelp_vectors.h"
#include "celp_math.h"

const uint8_t ff_fc_2pulses_9bits_track1[16] =
{
    1,  3,
    6,  8,
    11, 13,
    16, 18,
    21, 23,
    26, 28,
    31, 33,
    36, 38
};
const uint8_t ff_fc_2pulses_9bits_track1_gray[16] =
{
  1,  3,
  8,  6,
  18, 16,
  11, 13,
  38, 36,
  31, 33,
  21, 23,
  28, 26,
};

const uint8_t ff_fc_2pulses_9bits_track2_gray[32] =
{
  0,  2,
  5,  4,
  12, 10,
  7,  9,
  25, 24,
  20, 22,
  14, 15,
  19, 17,
  36, 31,
  21, 26,
  1,  6,
  16, 11,
  27, 29,
  32, 30,
  39, 37,
  34, 35,
};

const uint8_t ff_fc_4pulses_8bits_tracks_13[16] =
{
  0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75,
};

const uint8_t ff_fc_4pulses_8bits_track_4[32] =
{
    3,  4,
    8,  9,
    13, 14,
    18, 19,
    23, 24,
    28, 29,
    33, 34,
    38, 39,
    43, 44,
    48, 49,
    53, 54,
    58, 59,
    63, 64,
    68, 69,
    73, 74,
    78, 79,
};

const float ff_pow_0_7[10] = {
    0.700000f, 0.490000f, 0.343000f, 0.240100f, 0.168070f,
    0.117649f, 0.082354f, 0.057648f, 0.040354f, 0.028248f
};

const float ff_pow_0_75[10] = {
    0.750000f, 0.562500f, 0.421875f, 0.316406f, 0.237305f,
    0.177979f, 0.133484f, 0.100113f, 0.075085f, 0.056314f
};

const float ff_pow_0_55[10] = {
    0.550000f, 0.302500f, 0.166375f, 0.091506f, 0.050328f,
    0.027681f, 0.015224f, 0.008373f, 0.004605f, 0.002533f
};

const float ff_b60_sinc[61] = {
 0.898529f  ,  0.865051f  ,  0.769257f  ,  0.624054f  ,  0.448639f  ,  0.265289f   ,
 0.0959167f , -0.0412598f , -0.134338f  , -0.178986f  , -0.178528f  , -0.142609f   ,
-0.0849304f , -0.0205078f ,  0.0369568f ,  0.0773926f ,  0.0955200f ,  0.0912781f  ,
 0.0689392f ,  0.0357056f ,  0.0f       , -0.0305481f , -0.0504150f , -0.0570068f  ,
-0.0508423f , -0.0350037f , -0.0141602f ,  0.00665283f,  0.0230713f ,  0.0323486f  ,
 0.0335388f ,  0.0275879f ,  0.0167847f ,  0.00411987f, -0.00747681f, -0.0156860f  ,
-0.0193481f , -0.0183716f , -0.0137634f , -0.00704956f,  0.0f       ,  0.00582886f ,
 0.00939941f,  0.0103760f ,  0.00903320f,  0.00604248f,  0.00238037f, -0.00109863f ,
-0.00366211f, -0.00497437f, -0.00503540f, -0.00402832f, -0.00241089f, -0.000579834f,
 0.00103760f,  0.00222778f,  0.00277710f,  0.00271606f,  0.00213623f,  0.00115967f ,
 0.
};

void ff_acelp_fc_pulse_per_track(
        int16_t* fc_v,
        const uint8_t *tab1,
        const uint8_t *tab2,
        int pulse_indexes,
        int pulse_signs,
        int pulse_count,
        int bits)
{
    int mask = (1 << bits) - 1;
    int i;

    for(i=0; i<pulse_count; i++)
    {
        fc_v[i + tab1[pulse_indexes & mask]] +=
                (pulse_signs & 1) ? 8191 : -8192; // +/-1 in (2.13)

        pulse_indexes >>= bits;
        pulse_signs >>= 1;
    }

    fc_v[tab2[pulse_indexes]] += (pulse_signs & 1) ? 8191 : -8192;
}

void ff_decode_10_pulses_35bits(const int16_t *fixed_index,
                                AMRFixed *fixed_sparse,
                                const uint8_t *gray_decode,
                                int half_pulse_count, int bits)
{
    int i;
    int mask = (1 << bits) - 1;

    fixed_sparse->no_repeat_mask = 0;
    fixed_sparse->n = 2 * half_pulse_count;
    for (i = 0; i < half_pulse_count; i++) {
        const int pos1   = gray_decode[fixed_index[2*i+1] & mask] + i;
        const int pos2   = gray_decode[fixed_index[2*i  ] & mask] + i;
        const float sign = (fixed_index[2*i+1] & (1 << bits)) ? -1.0 : 1.0;
        fixed_sparse->x[2*i+1] = pos1;
        fixed_sparse->x[2*i  ] = pos2;
        fixed_sparse->y[2*i+1] = sign;
        fixed_sparse->y[2*i  ] = pos2 < pos1 ? -sign : sign;
    }
}

void ff_acelp_weighted_vector_sum(
        int16_t* out,
        const int16_t *in_a,
        const int16_t *in_b,
        int16_t weight_coeff_a,
        int16_t weight_coeff_b,
        int16_t rounder,
        int shift,
        int length)
{
    int i;

    // Clipping required here; breaks OVERFLOW test.
    for(i=0; i<length; i++)
        out[i] = av_clip_int16((
                 in_a[i] * weight_coeff_a +
                 in_b[i] * weight_coeff_b +
                 rounder) >> shift);
}

void ff_weighted_vector_sumf(float *out, const float *in_a, const float *in_b,
                             float weight_coeff_a, float weight_coeff_b, int length)
{
    int i;

    for(i=0; i<length; i++)
        out[i] = weight_coeff_a * in_a[i]
               + weight_coeff_b * in_b[i];
}

void ff_adaptive_gain_control(float *out, const float *in, float speech_energ,
                              int size, float alpha, float *gain_mem)
{
    int i;
    float postfilter_energ = ff_dot_productf(in, in, size);
    float gain_scale_factor = 1.0;
    float mem = *gain_mem;

    if (postfilter_energ)
        gain_scale_factor = sqrt(speech_energ / postfilter_energ);

    gain_scale_factor *= 1.0 - alpha;

    for (i = 0; i < size; i++) {
        mem = alpha * mem + gain_scale_factor;
        out[i] = in[i] * mem;
    }

    *gain_mem = mem;
}

void ff_scale_vector_to_given_sum_of_squares(float *out, const float *in,
                                             float sum_of_squares, const int n)
{
    int i;
    float scalefactor = ff_dot_productf(in, in, n);
    if (scalefactor)
        scalefactor = sqrt(sum_of_squares / scalefactor);
    for (i = 0; i < n; i++)
        out[i] = in[i] * scalefactor;
}

void ff_set_fixed_vector(float *out, const AMRFixed *in, float scale, int size)
{
    int i;

    for (i=0; i < in->n; i++) {
        int x   = in->x[i], repeats = !((in->no_repeat_mask >> i) & 1);
        float y = in->y[i] * scale;

        if (in->pitch_lag > 0)
            do {
                out[x] += y;
                y *= in->pitch_fac;
                x += in->pitch_lag;
            } while (x < size && repeats);
    }
}

void ff_clear_fixed_vector(float *out, const AMRFixed *in, int size)
{
    int i;

    for (i=0; i < in->n; i++) {
        int x  = in->x[i], repeats = !((in->no_repeat_mask >> i) & 1);

        if (in->pitch_lag > 0)
            do {
                out[x] = 0.0;
                x += in->pitch_lag;
            } while (x < size && repeats);
    }
}
