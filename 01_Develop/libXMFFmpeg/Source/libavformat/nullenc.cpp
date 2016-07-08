/*
 * RAW null muxer
 * Copyright (c) 2002 Fabrice Bellard
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

static int null_write_packet(struct AVFormatContext *s, AVPacket *pkt)
{
    return 0;
}

AVOutputFormat ff_null_muxer = {
    /*.name              = */	"null",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw null video format"),
    /*.mime_type         = */	0,
    /*.extensions        = */	0,
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	AV_NE(CODEC_ID_PCM_S16BE, CODEC_ID_PCM_S16LE),
    /*.video_codec       = */	CODEC_ID_RAWVIDEO,
    /*.write_header      = */	0,
    /*.write_packet      = */	null_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOFILE | AVFMT_NOTIMESTAMPS | AVFMT_RAWPICTURE,
};
