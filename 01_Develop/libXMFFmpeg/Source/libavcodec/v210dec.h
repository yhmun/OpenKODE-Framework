/*
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_V210DEC_H
#define AVCODEC_V210DEC_H

#include "XMFFmpeg/libavutil/log.h"
#include "XMFFmpeg/libavutil/opt.h"

typedef struct {
    AVClass *av_class;
    int custom_stride;
    int aligned_input;
    void (*unpack_frame)(const uint32_t *src, uint16_t *y, uint16_t *u, uint16_t *v, int width);
} V210DecContext;

void v210_x86_init(V210DecContext *s);

#endif /* AVCODEC_V210DEC_H */
