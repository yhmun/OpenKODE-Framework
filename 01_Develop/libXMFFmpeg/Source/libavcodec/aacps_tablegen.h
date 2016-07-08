/*
 * Header file for hardcoded Parametric Stereo tables
 *
 * Copyright (c) 2010 Alex Converse <alex.converse@gmail.com>
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

#ifndef AACPS_TABLEGEN_H
#define AACPS_TABLEGEN_H

#if CONFIG_HARDCODED_TABLES
#define ps_tableinit()
#include "XMFFMpeg/libavcodec/aacps_tables.h"
#else
#include "XMFFMpeg/libavutil/common.h"
#include "XMFFMpeg/libavutil/mathematics.h"
#define NR_ALLPASS_BANDS20 30
#define NR_ALLPASS_BANDS34 50
#define PS_AP_LINKS 3
static float pd_re_smooth[8*8*8];
static float pd_im_smooth[8*8*8];
static float HA[46][8][4];
static float HB[46][8][4];
static float f20_0_8 [ 8][7][2];
static float f34_0_12[12][7][2];
static float f34_1_8 [ 8][7][2];
static float f34_2_4 [ 4][7][2];
static float Q_fract_allpass[2][50][3][2];
static float phi_fract[2][50][2];

static const float g0_Q8[] = {
    0.00746082949812f, 0.02270420949825f, 0.04546865930473f, 0.07266113929591f,
    0.09885108575264f, 0.11793710567217f, 0.125f
};

static const float g0_Q12[] = {
    0.04081179924692f, 0.03812810994926f, 0.05144908135699f, 0.06399831151592f,
    0.07428313801106f, 0.08100347892914f, 0.08333333333333f
};

static const float g1_Q8[] = {
    0.01565675600122f, 0.03752716391991f, 0.05417891378782f, 0.08417044116767f,
    0.10307344158036f, 0.12222452249753f, 0.125f
};

static const float g2_Q4[] = {
    -0.05908211155639f, -0.04871498374946f, 0.0f,   0.07778723915851f,
     0.16486303567403f,  0.23279856662996f, 0.25f
};

static void make_filters_from_proto(float (*filter)[7][2], const float *proto, int bands)
{
    int q, n;
    for (q = 0; q < bands; q++) {
        for (n = 0; n < 7; n++) {
            double theta = 2 * M_PI * (q + 0.5) * (n - 6) / bands;
            filter[q][n][0] = proto[n] *  cos(theta);
            filter[q][n][1] = proto[n] * -sin(theta);
        }
    }
}

static void ps_tableinit(void)
{
    static const float ipdopd_sin[] = { 0, (float)M_SQRT1_2, 1,  (float)M_SQRT1_2,  0, -(float)M_SQRT1_2, -1, -(float)M_SQRT1_2 };
    static const float ipdopd_cos[] = { 1, (float)M_SQRT1_2, 0, -(float)M_SQRT1_2, -1, -(float)M_SQRT1_2,  0,  (float)M_SQRT1_2 };
    int pd0, pd1, pd2;

    static const float iid_par_dequant[] = {
        //iid_par_dequant_default
        0.05623413251903f, 0.12589254117942f, 0.19952623149689f, 0.31622776601684f,
        0.44668359215096f, 0.63095734448019f, 0.79432823472428f, 1,
        1.25892541179417f, 1.58489319246111f, 2.23872113856834f, 3.16227766016838f,
        5.01187233627272f, 7.94328234724282f, 17.7827941003892f,
        //iid_par_dequant_fine
        0.00316227766017f, 0.00562341325190f, 0.01f,             0.01778279410039f,
        0.03162277660168f, 0.05623413251903f, 0.07943282347243f, 0.11220184543020f,
        0.15848931924611f, 0.22387211385683f, 0.31622776601684f, 0.39810717055350f,
        0.50118723362727f, 0.63095734448019f, 0.79432823472428f, 1,
        1.25892541179417f, 1.58489319246111f, 1.99526231496888f, 2.51188643150958f,
        3.16227766016838f, 4.46683592150963f, 6.30957344480193f, 8.91250938133745f,
        12.5892541179417f, 17.7827941003892f, 31.6227766016838f, 56.2341325190349f,
        100,               177.827941003892f, 316.227766016837f,
    };
    static const float icc_invq[] = {
        1, 0.937f,      0.84118f,    0.60092f,    0.36764f,   0,      -0.589f,    -1
    };
    static const float acos_icc_invq[] = {
        0, 0.35685527f, 0.57133466f, 0.92614472f, 1.1943263f, (float)M_PI/2, 2.2006171f, (float)M_PI
    };
    int iid, icc;

    int k, m;
    static const int8_t f_center_20[] = {
        -3, -1, 1, 3, 5, 7, 10, 14, 18, 22,
    };
    static const int8_t f_center_34[] = {
         2,  6, 10, 14, 18, 22, 26, 30,
        34,-10, -6, -2, 51, 57, 15, 21,
        27, 33, 39, 45, 54, 66, 78, 42,
       102, 66, 78, 90,102,114,126, 90,
    };
    static const float fractional_delay_links[] = { 0.43f, 0.75f, 0.347f };
    const float fractional_delay_gain = 0.39f;

    for (pd0 = 0; pd0 < 8; pd0++) {
        float pd0_re = ipdopd_cos[pd0];
        float pd0_im = ipdopd_sin[pd0];
        for (pd1 = 0; pd1 < 8; pd1++) {
            float pd1_re = ipdopd_cos[pd1];
            float pd1_im = ipdopd_sin[pd1];
            for (pd2 = 0; pd2 < 8; pd2++) {
                float pd2_re = ipdopd_cos[pd2];
                float pd2_im = ipdopd_sin[pd2];
                float re_smooth = 0.25f * pd0_re + 0.5f * pd1_re + pd2_re;
                float im_smooth = 0.25f * pd0_im + 0.5f * pd1_im + pd2_im;
                float pd_mag = 1 / sqrt(im_smooth * im_smooth + re_smooth * re_smooth);
                pd_re_smooth[pd0*64+pd1*8+pd2] = re_smooth * pd_mag;
                pd_im_smooth[pd0*64+pd1*8+pd2] = im_smooth * pd_mag;
            }
        }
    }

    for (iid = 0; iid < 46; iid++) {
        float c = iid_par_dequant[iid]; ///< Linear Inter-channel Intensity Difference
        float c1 = (float)M_SQRT2 / sqrtf(1.0f + c*c);
        float c2 = c * c1;
        for (icc = 0; icc < 8; icc++) {
            /*if (PS_BASELINE || ps->icc_mode < 3)*/ {
                float alpha = 0.5f * acos_icc_invq[icc];
                float beta  = alpha * (c1 - c2) * (float)M_SQRT1_2;
                HA[iid][icc][0] = c2 * cosf(beta + alpha);
                HA[iid][icc][1] = c1 * cosf(beta - alpha);
                HA[iid][icc][2] = c2 * sinf(beta + alpha);
                HA[iid][icc][3] = c1 * sinf(beta - alpha);
            } /* else */ {
                float alpha, gamma, mu, rho;
                float alpha_c, alpha_s, gamma_c, gamma_s;
                rho = FFMAX(icc_invq[icc], 0.05f);
                alpha = 0.5f * atan2f(2.0f * c * rho, c*c - 1.0f);
                mu = c + 1.0f / c;
                mu = sqrtf(1 + (4 * rho * rho - 4)/(mu * mu));
                gamma = atanf(sqrtf((1.0f - mu)/(1.0f + mu)));
                if (alpha < 0) alpha += (float)M_PI/2;
                alpha_c = cosf(alpha);
                alpha_s = sinf(alpha);
                gamma_c = cosf(gamma);
                gamma_s = sinf(gamma);
                HB[iid][icc][0] =  M_SQRT2 * alpha_c * gamma_c;
                HB[iid][icc][1] =  M_SQRT2 * alpha_s * gamma_c;
                HB[iid][icc][2] = -M_SQRT2 * alpha_s * gamma_s;
                HB[iid][icc][3] =  M_SQRT2 * alpha_c * gamma_s;
            }
        }
    }

    for (k = 0; k < NR_ALLPASS_BANDS20; k++) {
        double f_center, theta;
        if (k < FF_ARRAY_ELEMS(f_center_20))
            f_center = f_center_20[k] * 0.125;
        else
            f_center = k - 6.5f;
        for (m = 0; m < PS_AP_LINKS; m++) {
            theta = -M_PI * fractional_delay_links[m] * f_center;
            Q_fract_allpass[0][k][m][0] = cos(theta);
            Q_fract_allpass[0][k][m][1] = sin(theta);
        }
        theta = -M_PI*fractional_delay_gain*f_center;
        phi_fract[0][k][0] = cos(theta);
        phi_fract[0][k][1] = sin(theta);
    }
    for (k = 0; k < NR_ALLPASS_BANDS34; k++) {
        double f_center, theta;
        if (k < FF_ARRAY_ELEMS(f_center_34))
            f_center = f_center_34[k] / 24.;
        else
            f_center = k - 26.5f;
        for (m = 0; m < PS_AP_LINKS; m++) {
            theta = -M_PI * fractional_delay_links[m] * f_center;
            Q_fract_allpass[1][k][m][0] = cos(theta);
            Q_fract_allpass[1][k][m][1] = sin(theta);
        }
        theta = -M_PI*fractional_delay_gain*f_center;
        phi_fract[1][k][0] = cos(theta);
        phi_fract[1][k][1] = sin(theta);
    }

    make_filters_from_proto(f20_0_8,  g0_Q8,   8);
    make_filters_from_proto(f34_0_12, g0_Q12, 12);
    make_filters_from_proto(f34_1_8,  g1_Q8,   8);
    make_filters_from_proto(f34_2_4,  g2_Q4,   4);
}
#endif /* CONFIG_HARDCODED_TABLES */

#endif /* AACPS_TABLEGEN_H */
