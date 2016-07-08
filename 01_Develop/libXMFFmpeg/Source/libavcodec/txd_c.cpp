/*
 * Renderware TeXture Dictionary (.txd) image decoder
 * Copyright (c) 2007 Ivo van Poorten
 *
 * See also: http://wiki.multimedia.cx/index.php?title=TXD
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

#include "XMFFmpeg/libavutil/intreadwrite.h"
#include "XMFFmpeg/libavutil/imgutils.h"
#include "bytestream.h"
#include "internal.h"
#include "s3tc.h"

typedef struct TXDContext {
    AVFrame picture;
} TXDContext;

static av_cold int txd_init(AVCodecContext *avctx) {
    TXDContext *s = (TXDContext *)avctx->priv_data;

    avcodec_get_frame_defaults(&s->picture);
    avctx->coded_frame = &s->picture;

    return 0;
}

static int txd_decode_frame(AVCodecContext *avctx, void *data, int *data_size,
                            AVPacket *avpkt) {
    const uint8_t *buf = avpkt->data;
    const uint8_t *buf_end = avpkt->data + avpkt->size;
    TXDContext * const s = (TXDContext * const)avctx->priv_data;
    AVFrame *picture = (AVFrame *)data;
    AVFrame * const p = &s->picture;
    unsigned int version, w, h, d3d_format, depth, stride, mipmap_count, flags;
    unsigned int y, v;
    uint8_t *ptr;
    const uint8_t *cur = buf;
    const uint32_t *palette = (const uint32_t *)(cur + 88);
    uint32_t *pal;

    if (buf_end - cur < 92)
        return AVERROR_INVALIDDATA;
    version         = AV_RL32(cur);
    d3d_format      = AV_RL32(cur+76);
    w               = AV_RL16(cur+80);
    h               = AV_RL16(cur+82);
    depth           = AV_RL8 (cur+84);
    mipmap_count    = AV_RL8 (cur+85);
    flags           = AV_RL8 (cur+87);
    cur            += 92;

    if (version < 8 || version > 9) {
        av_log(avctx, AV_LOG_ERROR, "texture data version %i is unsupported\n",
                                                                    version);
        return -1;
    }

    if (depth == 8) {
        avctx->pix_fmt = PIX_FMT_PAL8;
        if (buf_end - cur < 1024)
            return AVERROR_INVALIDDATA;
        cur += 1024;
    } else if (depth == 16 || depth == 32)
        avctx->pix_fmt = PIX_FMT_RGB32;
    else {
        av_log(avctx, AV_LOG_ERROR, "depth of %i is unsupported\n", depth);
        return -1;
    }

    if (p->data[0])
        avctx->release_buffer(avctx, p);

    if (av_image_check_size(w, h, 0, avctx))
        return -1;
    if (w != avctx->width || h != avctx->height)
        avcodec_set_dimensions(avctx, w, h);
    if (avctx->get_buffer(avctx, p) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return -1;
    }

    p->pict_type = AV_PICTURE_TYPE_I;

    ptr    = p->data[0];
    stride = p->linesize[0];

    if (depth == 8) {
        pal = (uint32_t *) p->data[1];
        for (y=0; y<256; y++) {
            v = AV_RB32(palette+y);
            pal[y] = (v>>8) + (v<<24);
        }
        if (buf_end - cur < w * h)
            return AVERROR_INVALIDDATA;
        for (y=0; y<h; y++) {
            memcpy(ptr, cur, w);
            ptr += stride;
            cur += w;
        }
    } else if (depth == 16) {
        switch (d3d_format) {
        case 0:
            if (!(flags & 1))
                goto unsupported;
        case FF_S3TC_DXT1:
            if (buf_end - cur < (w/4) * (h/4) * 8)
                return AVERROR_INVALIDDATA;
            ff_decode_dxt1(cur, ptr, w, h, stride);
            break;
        case FF_S3TC_DXT3:
            if (buf_end - cur < (w/4) * (h/4) * 16)
                return AVERROR_INVALIDDATA;
            ff_decode_dxt3(cur, ptr, w, h, stride);
            break;
        default:
            goto unsupported;
        }
    } else if (depth == 32) {
        switch (d3d_format) {
        case 0x15:
        case 0x16:
            if (buf_end - cur < h * w * 4)
                return AVERROR_INVALIDDATA;
            for (y=0; y<h; y++) {
                memcpy(ptr, cur, w*4);
                ptr += stride;
                cur += w*4;
            }
            break;
        default:
            goto unsupported;
        }
    }

    for (; mipmap_count > 1 && buf_end - cur >= 4; mipmap_count--) {
        uint32_t length = bytestream_get_le32(&cur);
        if (buf_end - cur < length)
            break;
        cur += length;
    }

    *picture   = s->picture;
    *data_size = sizeof(AVPicture);

    return cur - buf;

unsupported:
    av_log(avctx, AV_LOG_ERROR, "unsupported d3d format (%08x)\n", d3d_format);
    return -1;
}

static av_cold int txd_end(AVCodecContext *avctx) {
    TXDContext *s = (TXDContext *)avctx->priv_data;

    if (s->picture.data[0])
        avctx->release_buffer(avctx, &s->picture);

    return 0;
}

AVCodec ff_txd_decoder = {
    /*.name					= */	"txd",
    /*.type					= */	AVMEDIA_TYPE_VIDEO,
    /*.id					= */	CODEC_ID_TXD,
    /*.priv_data_size		= */	sizeof(TXDContext),
    /*.init					= */	txd_init,
    /*.encode				= */	0,
    /*.close				= */	txd_end,
    /*.decode				= */	txd_decode_frame,
    /*.capabilities			= */	CODEC_CAP_DR1,
	/*.next					= */	0,
	/*.flush				= */	0,
	/*.supported_framerates	= */	0,
	/*.pix_fmts				= */	0,
    /*.long_name			= */	NULL_IF_CONFIG_SMALL("Renderware TXD (TeXture Dictionary) image"),
};
