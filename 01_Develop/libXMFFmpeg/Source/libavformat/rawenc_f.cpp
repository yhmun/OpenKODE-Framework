/*
 * RAW muxers
 * Copyright (c) 2001 Fabrice Bellard
 * Copyright (c) 2005 Alex Beregszaszi
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
#include "rawenc.h"

int ff_raw_write_packet(AVFormatContext *s, AVPacket *pkt)
{
    avio_write(s->pb, pkt->data, pkt->size);
    avio_flush(s->pb);
    return 0;
}

/* Note: Do not forget to add new entries to the Makefile as well. */

#if CONFIG_AC3_MUXER
AVOutputFormat ff_ac3_muxer = {
    /*.name              = */	"ac3",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw AC-3"),
    /*.mime_type         = */	"audio/x-ac3",
    /*.extensions        = */	"ac3",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_AC3,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_ADX_MUXER
AVOutputFormat ff_adx_muxer = {
    /*.name              = */	"adx",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("CRI ADX"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"adx",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_ADPCM_ADX,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_DIRAC_MUXER
AVOutputFormat ff_dirac_muxer = {
    /*.name              = */	"dirac",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw Dirac"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"drc",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_DIRAC,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_DNXHD_MUXER
AVOutputFormat ff_dnxhd_muxer = {
    /*.name              = */	"dnxhd",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw DNxHD (SMPTE VC-3)"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"dnxhd",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_DNXHD,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_DTS_MUXER
AVOutputFormat ff_dts_muxer = {
    /*.name              = */	"dts",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw DTS"),
    /*.mime_type         = */	"audio/x-dca",
    /*.extensions        = */	"dts",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_DTS,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_EAC3_MUXER
AVOutputFormat ff_eac3_muxer = {
    /*.name              = */	"eac3",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw E-AC-3"),
    /*.mime_type         = */	"audio/x-eac3",
    /*.extensions        = */	"eac3",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_EAC3,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_G722_MUXER
AVOutputFormat ff_g722_muxer = {
    /*.name              = */	"g722",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw G.722"),
    /*.mime_type         = */	"audio/G722",
    /*.extensions        = */	"g722",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_ADPCM_G722,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_G723_1_MUXER
AVOutputFormat ff_g723_1_muxer = {
    /*.name              = */	"g723_1",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw G.723.1"),
    /*.mime_type         = */	"audio/g723",
    /*.extensions        = */	"tco,rco",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_G723_1,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_H261_MUXER
AVOutputFormat ff_h261_muxer = {
    /*.name              = */	"h261",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw H.261"),
    /*.mime_type         = */	"video/x-h261",
    /*.extensions        = */	"h261",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_H261,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_H263_MUXER
AVOutputFormat ff_h263_muxer = {
    /*.name              = */	"h263",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw H.263"),
    /*.mime_type         = */	"video/x-h263",
    /*.extensions        = */	"h263",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_H263,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_H264_MUXER
AVOutputFormat ff_h264_muxer = {
    /*.name              = */	"h264",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw H.264 video format"),
    /*.mime_type         = */	"video/x-h263",
    /*.extensions        = */	"h264",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_H264,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_CAVSVIDEO_MUXER
AVOutputFormat ff_cavsvideo_muxer = {
    /*.name              = */	"cavsvideo",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw Chinese AVS video"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"cavs",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_CAVS,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_M4V_MUXER
AVOutputFormat ff_m4v_muxer = {
    /*.name              = */	"m4v",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw MPEG-4 video format"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"m4v",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_MPEG4,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_MJPEG_MUXER
AVOutputFormat ff_mjpeg_muxer = {
    /*.name              = */	"mjpeg",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw MJPEG video"),
    /*.mime_type         = */	"video/x-mjpeg",
    /*.extensions        = */	"mjpg,mjpeg",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_MJPEG,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_MLP_MUXER
AVOutputFormat ff_mlp_muxer = {
    /*.name              = */	"mlp",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw MLP"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"mlp",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_MLP,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_SRT_MUXER
AVOutputFormat ff_srt_muxer = {
    /*.name              = */	"srt",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("SubRip subtitle format"),
    /*.mime_type         = */	"application/x-subrip",
    /*.extensions        = */	"srt",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
	/*.dummy			 = */	0,
	/*.interleave_packet = */	0,
	/*.codec_tag		 = */	0,
	/*.subtitle_codec	 = */	CODEC_ID_SRT,
};
#endif

#if CONFIG_TRUEHD_MUXER
AVOutputFormat ff_truehd_muxer = {
    /*.name              = */	"truehd",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw TrueHD"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"thd",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_TRUEHD,
    /*.video_codec       = */	CODEC_ID_NONE,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_MPEG1VIDEO_MUXER
AVOutputFormat ff_mpeg1video_muxer = {
    /*.name              = */	"mpeg1video",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw MPEG-1 video"),
    /*.mime_type         = */	"video/x-mpeg",
    /*.extensions        = */	"mpg,mpeg,m1v",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_MPEG1VIDEO,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_MPEG2VIDEO_MUXER
AVOutputFormat ff_mpeg2video_muxer = {
    /*.name              = */	"mpeg2video",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw MPEG-2 video"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"m2v",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_MPEG2VIDEO,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif

#if CONFIG_RAWVIDEO_MUXER
AVOutputFormat ff_rawvideo_muxer = {
    /*.name              = */	"rawvideo",
    /*.long_name         = */	NULL_IF_CONFIG_SMALL("raw video format"),
    /*.mime_type         = */	0,
    /*.extensions        = */	"yuv,rgb",
	/*.priv_data_size	 = */	0,
    /*.audio_codec       = */	CODEC_ID_NONE,
    /*.video_codec       = */	CODEC_ID_RAWVIDEO,
    /*.write_header      = */	0,
    /*.write_packet      = */	ff_raw_write_packet,
    /*.write_trailer     = */	0,
	/*.flags			 = */	AVFMT_NOTIMESTAMPS,
};
#endif
