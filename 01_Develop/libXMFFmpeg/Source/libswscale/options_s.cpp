/*
 * Copyright (C) 2001-2003 Michael Niedermayer <michaelni@gmx.at>
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

#include "XMFFmpeg/libavutil/avutil.h"
#include "XMFFmpeg/libavutil/pixfmt.h"
#include "XMFFmpeg/libavutil/opt.h"
#include "swscale_internal.h"

static const char * sws_context_to_name(void * ptr)
{
    return "swscaler";
}

#define OFFSET(x) offsetof(SwsContext, x)
#define DEFAULT 0
#define VE AV_OPT_FLAG_VIDEO_PARAM | AV_OPT_FLAG_ENCODING_PARAM

static const AVOption options[] = {
    { "sws_flags"		, "scaler/cpu flags"				, OFFSET(flags)		, AV_OPT_TYPE_FLAGS	, DEFAULT			, NULL, 0		, UINT_MAX	, VE, "sws_flags" },
    { "fast_bilinear"	, "fast bilinear"					, 0					, AV_OPT_TYPE_CONST	, SWS_FAST_BILINEAR	, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "bilinear"		, "bilinear"						, 0					, AV_OPT_TYPE_CONST	, SWS_BILINEAR		, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "bicubic"			, "bicubic"							, 0					, AV_OPT_TYPE_CONST	, SWS_BICUBIC		, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "experimental"	, "experimental"					, 0					, AV_OPT_TYPE_CONST	, SWS_X				, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "neighbor"		, "nearest neighbor"				, 0					, AV_OPT_TYPE_CONST	, SWS_POINT			, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "area"			, "averaging area"					, 0					, AV_OPT_TYPE_CONST	, SWS_AREA			, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "bicublin"		, "luma bicubic, chroma bilinear"	, 0					, AV_OPT_TYPE_CONST	, SWS_BICUBLIN		, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "gauss"			, "gaussian"						, 0					, AV_OPT_TYPE_CONST	, SWS_GAUSS			, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "sinc"			, "sinc"							, 0					, AV_OPT_TYPE_CONST	, SWS_SINC			, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "lanczos"			, "lanczos"							, 0					, AV_OPT_TYPE_CONST	, SWS_LANCZOS		, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "spline"			, "natural bicubic spline"			, 0					, AV_OPT_TYPE_CONST	, SWS_SPLINE		, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { " PRInt_info"		, " PRInt info"						, 0					, AV_OPT_TYPE_CONST	, SWS_PRINT_INFO	, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "accurate_rnd"	, "accurate rounding"				, 0					, AV_OPT_TYPE_CONST	, SWS_ACCURATE_RND	, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "full_chroma_int"	, "full chroma interpolation"		, 0					, AV_OPT_TYPE_CONST	, SWS_FULL_CHR_H_INT, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "full_chroma_inp"	, "full chroma input"				, 0					, AV_OPT_TYPE_CONST	, SWS_FULL_CHR_H_INP, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },
    { "bitexact"		, ""								, 0					, AV_OPT_TYPE_CONST	, SWS_BITEXACT		, NULL, INT_MIN	, INT_MAX	, VE, "sws_flags" },

    { "srcw"			, "source width"					, OFFSET(srcW)		, AV_OPT_TYPE_INT	, 16				, NULL, 1		, INT_MAX	, VE },
    { "srch"			, "source height"					, OFFSET(srcH)		, AV_OPT_TYPE_INT	, 16				, NULL, 1		, INT_MAX	, VE },
    { "dstw"			, "destination width"				, OFFSET(dstW)		, AV_OPT_TYPE_INT	, 16				, NULL, 1		, INT_MAX	, VE },
    { "dsth"			, "destination height"				, OFFSET(dstH)		, AV_OPT_TYPE_INT	, 16				, NULL, 1		, INT_MAX	, VE },
    { "src_format"		, "source format"					, OFFSET(srcFormat)	, AV_OPT_TYPE_INT	, DEFAULT			, NULL, 0		, PIX_FMT_NB-1, VE },
    { "dst_format"		, "destination format"				, OFFSET(dstFormat)	, AV_OPT_TYPE_INT	, DEFAULT			, NULL, 0		, PIX_FMT_NB-1, VE },
    { "src_range"		, "source range"					, OFFSET(srcRange)	, AV_OPT_TYPE_INT	, DEFAULT			, NULL, 0		, 1			, VE },
    { "dst_range"		, "destination range"				, OFFSET(dstRange)	, AV_OPT_TYPE_INT	, DEFAULT			, NULL, 0		, 1			, VE },
    { "param0"			, "scaler param 0"					, OFFSET(param[0])	, AV_OPT_TYPE_DOUBLE, SWS_PARAM_DEFAULT	, NULL, INT_MIN	, INT_MAX	, VE },
    { "param1"			, "scaler param 1"					, OFFSET(param[1])	, AV_OPT_TYPE_DOUBLE, SWS_PARAM_DEFAULT	, NULL, INT_MIN	, INT_MAX	, VE },

    { NULL }
};

const AVClass sws_context_class = { "SWScaler", sws_context_to_name, options };

const AVClass *sws_get_class(void)
{
    return &sws_context_class;
}
