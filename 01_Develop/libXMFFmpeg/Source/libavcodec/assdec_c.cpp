/*
 * SSA/ASS decoder
 * Copyright (c) 2010  Aurelien Jacobs <aurel@gnuage.org>
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
#include "ass.h"
#include "ass_split.h"

static av_cold int ass_decode_init(AVCodecContext *avctx)
{
    avctx->subtitle_header = (uint8_t *)av_malloc(avctx->extradata_size);
    if (!avctx->extradata)
        return AVERROR(ENOMEM);
    memcpy(avctx->subtitle_header, avctx->extradata, avctx->extradata_size);
    avctx->subtitle_header_size = avctx->extradata_size;
    avctx->priv_data = ff_ass_split((const char *)avctx->extradata);
    return 0;
}

static int ass_decode_frame(AVCodecContext *avctx, void *data, int *got_sub_ptr,
                            AVPacket *avpkt)
{
    const char *ptr = (const char *)avpkt->data;
    int len, size = avpkt->size;

    while (size > 0) {
        ASSDialog *dialog = ff_ass_split_dialog((ASSSplitContext *)avctx->priv_data, ptr, 0, NULL);
        int duration = dialog->end - dialog->start;
        len = ff_ass_add_rect((AVSubtitle *)data, ptr, 0, duration, 1);
        if (len < 0)
            return len;
        ptr  += len;
        size -= len;
    }

    *got_sub_ptr = avpkt->size > 0;
    return avpkt->size;
}

static int ass_decode_close(AVCodecContext *avctx)
{
    ff_ass_split_free((ASSSplitContext *)avctx->priv_data);
    avctx->priv_data = NULL;
    return 0;
}

AVCodec ff_ass_decoder = {
    /*.name					= */	"ass",
    /*.type					= */	AVMEDIA_TYPE_SUBTITLE,
    /*.id					= */	CODEC_ID_SSA,
    /*.priv_data_size		= */	0,
    /*.init					= */	ass_decode_init,
    /*.encode				= */	0,
    /*.close				= */	ass_decode_close,
    /*.decode				= */	ass_decode_frame,
    /*.capabilities			= */	0,
	/*.next					= */	0,
	/*.flush				= */	0,
	/*.supported_framerates	= */	0,
	/*.pix_fmts				= */	0,
    /*.long_name			= */	NULL_IF_CONFIG_SMALL("Advanced SubStation Alpha subtitle"),
};
