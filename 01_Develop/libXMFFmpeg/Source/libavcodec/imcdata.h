/*
 * IMC compatible decoder
 * Copyright (c) 2002-2004 Maxim Poliakovski
 * Copyright (c) 2006 Benjamin Larsson
 * Copyright (c) 2006 Konstantin Shishkov
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

#ifndef AVCODEC_IMCDATA_H
#define AVCODEC_IMCDATA_H

#include "XMFFmpeg/type.h"

static const uint16_t band_tab[33] = {
      0,   3,   6,   9,  12,  16,  20,  24,  29,  34,  40,
     46,  53,  60,  68,  76,  84,  93, 102, 111, 121, 131,
    141, 151, 162, 173, 184, 195, 207, 219, 231, 243, 256,
};


static const int8_t cyclTab[32] = {
    1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
   12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23,
   24, 25, 26, 27, 28, 29, 30, 31, 32, 32,
};

static const int8_t cyclTab2[32] = {
   -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
 12, 13, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22,
23, 24, 25, 26, 27, 28, 29};

static const float imc_weights1[31] = {
    0.119595f, 0.123124f, 0.129192f, 9.97377e-2f, 8.1923e-2f, 9.61153e-2f, 8.77885e-2f, 8.61174e-2f,
    9.00882e-2f, 9.91658e-2f, 0.112991f, 0.131126f, 0.152886f, 0.177292f, 0.221782f, 0.244917f, 0.267386f,
    0.306816f, 0.323046f, 0.33729f, 0.366773f, 0.392557f, 0.398076f, 0.403302f, 0.42451f, 0.444777f,
    0.449188f, 0.455445f, 0.477853f, 0.500669f, 0.510395f};

static const float imc_weights2[31] = {
    3.23466e-3f, 3.49886e-3f, 3.98413e-3f, 1.98116e-3f, 1.16465e-3f, 1.79283e-3f, 1.40372e-3f, 1.33274e-3f,
    1.50523e-3f, 1.95064e-3f, 2.77472e-3f, 4.14725e-3f, 6.2776e-3f, 9.36401e-3f, 1.71397e-2f, 2.24052e-2f,
    2.83971e-2f, 4.11689e-2f, 4.73165e-2f, 5.31631e-2f, 6.66614e-2f, 8.00824e-2f, 8.31588e-2f, 8.61397e-2f,
    9.89229e-2f, 0.112197f, 0.115227f, 0.119613f, 0.136174f, 0.15445f, 0.162685f};

static const float imc_quantizer1[4][8] = {
    { 8.4431201e-1f, 4.7358301e-1f, 1.448354f, 2.7073899e-1f, 7.4449003e-1f, 1.241991f,  1.845484f,  0.0f},
    { 8.6876702e-1f, 4.7659001e-1f, 1.478224f, 2.5672799e-1f, 7.55777e-1f,   1.3229851f, 2.03438f,   0.0f},
    { 7.5891501e-1f, 6.2272799e-1f, 1.271322f, 3.47904e-1f,   7.5317699e-1f, 1.150767f,  1.628476f,  0.0f},
    { 7.65257e-1f,   6.44647e-1f,   1.263824f, 3.4548101e-1f, 7.6384902e-1f, 1.214466f,  1.7638789f, 0.0f},
};

static const float imc_quantizer2[2][56] = {
    { 1.39236e-1f, 3.50548e-1f, 5.9547901e-1f, 8.5772401e-1f, 1.121545f, 1.3882281f, 1.695882f, 2.1270809f,
      7.2221003e-2f, 1.85177e-1f, 2.9521701e-1f, 4.12568e-1f, 5.4068601e-1f, 6.7679501e-1f, 8.1196898e-1f, 9.4765198e-1f,
      1.0779999f, 1.203415f, 1.337265f, 1.481871f, 1.639982f, 1.814766f, 2.0701399f, 2.449862f,
      3.7533998e-2f, 1.02722e-1f, 1.6021401e-1f, 2.16043e-1f, 2.7231601e-1f, 3.3025399e-1f, 3.9022601e-1f, 4.52849e-1f,
      5.1794899e-1f, 5.8529502e-1f, 6.53956e-1f, 7.2312802e-1f, 7.9150802e-1f, 8.5891002e-1f, 9.28141e-1f, 9.9706203e-1f,
      1.062153f, 1.12564f, 1.189834f, 1.256122f, 1.324469f, 1.3955311f, 1.468906f, 1.545084f,
      1.6264729f, 1.711524f, 1.802705f, 1.91023f, 2.0533991f, 2.22333f, 2.4830019f, 3.253329f },
    { 1.11654e-1f, 3.54469e-1f, 6.4232099e-1f, 9.6128798e-1f, 1.295053f, 1.61777f, 1.989839f, 2.51107f,
      5.7721999e-2f, 1.69879e-1f, 2.97589e-1f, 4.3858799e-1f, 5.9039903e-1f, 7.4934798e-1f, 9.1628098e-1f, 1.087297f,
      1.262751f, 1.4288321f, 1.6040879f, 1.79067f, 2.000668f, 2.2394669f, 2.649332f, 5.2760072f,
      2.9722e-2f, 8.7316997e-2f, 1.4445201e-1f, 2.04247e-1f, 2.6879501e-1f, 3.3716801e-1f, 4.08811e-1f, 4.8306999e-1f,
      5.6049401e-1f, 6.3955498e-1f, 7.2044599e-1f, 8.0427998e-1f, 8.8933599e-1f, 9.7537601e-1f, 1.062461f, 1.1510431f,
      1.240236f, 1.326715f, 1.412513f, 1.500502f, 1.591749f, 1.686413f, 1.785239f, 1.891233f,
      2.0051291f, 2.127681f, 2.2709141f, 2.475826f, 2.7219379f, 3.101985f, 4.686213f, 6.2287788f},
};


static const float xTab[14] = {7.6f, 3.6f, 4.4f, 3.7f, 6.1f, 5.1f, 2.3f, 1.6f, 6.2f, 1.5f, 1.8f, 1.2f, 0, 0}; //10014048

/* precomputed table for 10^(i/4), i=-15..16 */
static const float imc_exp_tab[32] = {
    1.778280e-4f, 3.162278e-4f, 5.623413e-4f, 1.000000e-3f,
    1.778280e-3f, 3.162278e-3f, 5.623413e-3f, 1.000000e-2f,
    1.778280e-2f, 3.162278e-2f, 5.623413e-2f, 1.000000e-1f,
    1.778280e-1f, 3.162278e-1f, 5.623413e-1f, 1.000000e00f,
    1.778280e00f, 3.162278e00f, 5.623413e00f, 1.000000e01f,
    1.778280e01f, 3.162278e01f, 5.623413e01f, 1.000000e02f,
    1.778280e02f, 3.162278e02f, 5.623413e02f, 1.000000e03f,
    1.778280e03f, 3.162278e03f, 5.623413e03f, 1.000000e04f
};
static const float * const imc_exp_tab2 = imc_exp_tab + 8;


static const uint8_t imc_cb_select[4][32] = {
    { 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2 },
    { 0, 2, 0, 3, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3,
      3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2 },
    { 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
      3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

static const uint8_t imc_huffman_sizes[4] = {
    17,  17,  18,  18
};

static const uint8_t imc_huffman_lens[4][4][18] = {
    {
        { 16, 15, 13, 11,  8,  5,  3,  1,  2,  4,  6,  9, 10, 12, 14, 16,  7,  0 },
        { 10,  8,  7,  6,  4,  4,  3,  2,  2,  3,  4,  6,  7,  9, 11, 11,  7,  0 },
        { 15, 15, 14, 11,  8,  6,  4,  2,  1,  4,  5,  7,  9, 10, 12, 13,  4,  0 },
        { 13, 11, 10,  8,  6,  4,  2,  2,  2,  3,  5,  7,  9, 12, 15, 15, 14,  0 },
    },
    {
        { 14, 12, 10,  8,  7,  4,  2,  2,  2,  3,  5,  7,  9, 11, 13, 14,  7,  0 },
        { 14, 13, 11,  8,  6,  4,  3,  2,  2,  3,  5,  7,  9, 10, 12, 14,  3,  0 },
        { 13, 12, 10,  7,  5,  4,  3,  2,  2,  3,  4,  6,  8,  9, 11, 13,  4,  0 },
        { 13, 12, 10,  7,  5,  4,  3,  2,  2,  3,  4,  6,  8,  9, 11, 13,  4,  0 },
    },
    {
        { 16, 14, 12, 10,  8,  5,  3,  1,  2,  4,  7,  9, 11, 13, 15, 17,  6, 17 },
        { 15, 13, 11,  8,  6,  4,  2,  2,  2,  3,  5,  7, 10, 12, 14, 16,  9, 16 },
        { 14, 12, 11,  9,  8,  6,  3,  1,  2,  5,  7, 10, 13, 15, 16, 17,  4, 17 },
        { 16, 14, 12,  9,  7,  5,  2,  2,  2,  3,  4,  6,  8, 11, 13, 15, 10, 16 },
    },
    {
        { 13, 11, 10,  8,  7,  5,  2,  2,  2,  4,  6,  9, 12, 14, 15, 16,  3, 16 },
        { 11, 11, 10,  9,  8,  7,  5,  4,  3,  3,  3,  3,  3,  3,  4,  5,  6,  5 },
        {  9,  9,  7,  6,  5,  4,  3,  3,  2,  3,  4,  5,  4,  5,  5,  6,  8,  6 },
        { 13, 12, 10,  8,  5,  3,  3,  2,  2,  3,  4,  7,  9, 11, 14, 15,  6, 15 },
    }
};

static const uint16_t imc_huffman_bits[4][4][18] = {
    {
        { 0xCC32, 0x6618, 0x1987, 0x0660, 0x00CD, 0x0018, 0x0007, 0x0000, 0x0002, 0x000D, 0x0032, 0x0199, 0x0331, 0x0CC2, 0x330D, 0xCC33, 0x0067, 0x0000 },
        { 0x02FE, 0x00BE, 0x005E, 0x002D, 0x000A, 0x0009, 0x0003, 0x0003, 0x0000, 0x0002, 0x0008, 0x002C, 0x005D, 0x017E, 0x05FE, 0x05FF, 0x005C, 0x0000 },
        { 0x5169, 0x5168, 0x28B5, 0x0517, 0x00A3, 0x0029, 0x0008, 0x0003, 0x0000, 0x0009, 0x0015, 0x0050, 0x0144, 0x028A, 0x0A2C, 0x145B, 0x000B, 0x0000 },
        { 0x1231, 0x048D, 0x0247, 0x0090, 0x0025, 0x0008, 0x0001, 0x0003, 0x0000, 0x0005, 0x0013, 0x0049, 0x0122, 0x0919, 0x48C3, 0x48C2, 0x2460, 0x0000 },
    },
    {
        { 0x2D1D, 0x0B46, 0x02D0, 0x00B5, 0x0059, 0x000A, 0x0003, 0x0001, 0x0000, 0x0004, 0x0017, 0x005B, 0x0169, 0x05A2, 0x168F, 0x2D1C, 0x0058, 0x0000 },
        { 0x1800, 0x0C01, 0x0301, 0x0061, 0x0019, 0x0007, 0x0004, 0x0003, 0x0000, 0x0005, 0x000D, 0x0031, 0x00C1, 0x0181, 0x0601, 0x1801, 0x0002, 0x0000 },
        { 0x1556, 0x0AAA, 0x02AB, 0x0054, 0x0014, 0x000B, 0x0002, 0x0003, 0x0000, 0x0003, 0x0008, 0x002B, 0x00AB, 0x0154, 0x0554, 0x1557, 0x0009, 0x0000 },
        { 0x1556, 0x0AAA, 0x02AB, 0x0054, 0x0014, 0x000B, 0x0002, 0x0003, 0x0000, 0x0003, 0x0008, 0x002B, 0x00AB, 0x0154, 0x0554, 0x1557, 0x0009, 0x0000 },
    },
    {
        { 0x2993, 0x0A65, 0x0298, 0x00A7, 0x0028, 0x0004, 0x0000, 0x0001, 0x0001, 0x0003, 0x0015, 0x0052, 0x014D, 0x0533, 0x14C8, 0x5324, 0x000B, 0x5325 },
        { 0x09B8, 0x026F, 0x009A, 0x0012, 0x0005, 0x0000, 0x0001, 0x0002, 0x0003, 0x0001, 0x0003, 0x0008, 0x004C, 0x0136, 0x04DD, 0x1373, 0x0027, 0x1372 },
        { 0x0787, 0x01E0, 0x00F1, 0x003D, 0x001F, 0x0006, 0x0001, 0x0001, 0x0001, 0x0002, 0x000E, 0x0079, 0x03C2, 0x0F0D, 0x1E19, 0x3C30, 0x0000, 0x3C31 },
        { 0x4B06, 0x12C0, 0x04B1, 0x0097, 0x0024, 0x0008, 0x0002, 0x0003, 0x0000, 0x0003, 0x0005, 0x0013, 0x004A, 0x0259, 0x0961, 0x2582, 0x012D, 0x4B07 },
    },
    {
        { 0x0A5A, 0x0297, 0x014A, 0x0053, 0x0028, 0x000B, 0x0003, 0x0000, 0x0002, 0x0004, 0x0015, 0x00A4, 0x052C, 0x14B7, 0x296C, 0x52DB, 0x0003, 0x52DA },
        { 0x0193, 0x0192, 0x00C8, 0x0065, 0x0033, 0x0018, 0x0007, 0x0004, 0x0000, 0x0004, 0x0005, 0x0007, 0x0006, 0x0003, 0x0005, 0x0005, 0x000D, 0x0004 },
        { 0x0012, 0x0013, 0x0005, 0x0003, 0x0000, 0x0003, 0x0005, 0x0004, 0x0003, 0x0003, 0x0005, 0x0005, 0x0004, 0x0004, 0x0003, 0x0005, 0x0008, 0x0004 },
        { 0x0D66, 0x06B2, 0x01AD, 0x006A, 0x000C, 0x0005, 0x0004, 0x0000, 0x0003, 0x0002, 0x0007, 0x0034, 0x00D7, 0x0358, 0x1ACF, 0x359C, 0x001B, 0x359D },
    }
};

#endif /* AVCODEC_IMCDATA_H */
