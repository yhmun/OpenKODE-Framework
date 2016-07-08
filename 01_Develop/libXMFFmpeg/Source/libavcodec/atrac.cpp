/*
 * Atrac common functions
 * Copyright (c) 2006-2008 Maxim Poliakovski
 * Copyright (c) 2006-2008 Benjamin Larsson
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

/**
 * @file
 */

#include "internal.h"
#include "dsputil.h"
#include "atrac.h"

float ff_atrac_sf_table[64];
static float qmf_window[48];

static const float qmf_48tap_half[24] = {
   -0.00001461907f, -0.00009205479f,-0.000056157569f,0.00030117269f,
    0.0002422519f,  -0.00085293897f,-0.0005205574f,  0.0020340169f,
    0.00078333891f, -0.0042153862f, -0.00075614988f, 0.0078402944f,
   -0.000061169922f,-0.01344162f,    0.0024626821f,  0.021736089f,
   -0.007801671f,   -0.034090221f,   0.01880949f,    0.054326009f,
   -0.043596379f,   -0.099384367f,   0.13207909f,    0.46424159f
};

/**
 * Generate common tables
 */

void atrac_generate_tables(void)
{
    int i;
    float s;

    /* Generate scale factors */
    if (!ff_atrac_sf_table[63])
        for (i=0 ; i<64 ; i++)
            ff_atrac_sf_table[i] = pow(2.0, (i - 15) / 3.0);

    /* Generate the QMF window. */
    if (!qmf_window[47])
        for (i=0 ; i<24; i++) {
            s = qmf_48tap_half[i] * 2.0;
            qmf_window[i] = qmf_window[47 - i] = s;
        }
}


/**
 * Quadrature mirror synthesis filter.
 *
 * @param inlo      lower part of spectrum
 * @param inhi      higher part of spectrum
 * @param nIn       size of spectrum buffer
 * @param pOut      out buffer
 * @param delayBuf  delayBuf buffer
 * @param temp      temp buffer
 */


void atrac_iqmf (float *inlo, float *inhi, unsigned int nIn, float *pOut, float *delayBuf, float *temp)
{
    int   i, j;
    float   *p1, *p3;

    memcpy(temp, delayBuf, 46*sizeof(float));

    p3 = temp + 46;

    /* loop1 */
    for(i=0; i<nIn; i+=2){
        p3[2*i+0] = inlo[i  ] + inhi[i  ];
        p3[2*i+1] = inlo[i  ] - inhi[i  ];
        p3[2*i+2] = inlo[i+1] + inhi[i+1];
        p3[2*i+3] = inlo[i+1] - inhi[i+1];
    }

    /* loop2 */
    p1 = temp;
    for (j = nIn; j != 0; j--) {
        float s1 = 0.0;
        float s2 = 0.0;

        for (i = 0; i < 48; i += 2) {
            s1 += p1[i] * qmf_window[i];
            s2 += p1[i+1] * qmf_window[i+1];
        }

        pOut[0] = s2;
        pOut[1] = s1;

        p1 += 2;
        pOut += 2;
    }

    /* Update the delay buffer. */
    memcpy(delayBuf, temp + nIn*2, 46*sizeof(float));
}

