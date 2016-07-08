/*
 * Copyright (c) 2001 Fabrice Bellard
 * Copyright (c) 2002-2004 Michael Niedermayer <michaelni@gmx.at>
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
 * Options definition for AVCodecContext.
 */

#include "internal.h"
#include "XMFFmpeg/libavutil/avassert.h"
#include "XMFFmpeg/libavutil/opt.h"

static const char* context_to_name(void* ptr) {
    AVCodecContext *avc= (AVCodecContext *)ptr;

    if(avc && avc->codec && avc->codec->name)
        return avc->codec->name;
    else
        return "NULL";
}

static void *codec_child_next(void *obj, void *prev)
{
    AVCodecContext *s = (AVCodecContext *)obj;
    if (!prev && s->codec && s->codec->priv_class && s->priv_data)
        return s->priv_data;
    return NULL;
}

static const AVClass *codec_child_class_next(const AVClass *prev)
{
    AVCodec *c = NULL;

    /* find the codec that corresponds to prev */
    while (prev && (c = av_codec_next(c)))
        if (c->priv_class == prev)
            break;

    /* find next codec with priv options */
    while (c = av_codec_next(c))
        if (c->priv_class)
            return c->priv_class;
    return NULL;
}

#define OFFSET(x) offsetof(AVCodecContext,x)
#define DEFAULT 0 //should be NAN but it does not work as it is not a constant in glibc as required by ANSI/ISO C
//these names are too long to be readable
#define V AV_OPT_FLAG_VIDEO_PARAM
#define A AV_OPT_FLAG_AUDIO_PARAM
#define S AV_OPT_FLAG_SUBTITLE_PARAM
#define E AV_OPT_FLAG_ENCODING_PARAM
#define D AV_OPT_FLAG_DECODING_PARAM

#define AV_CODEC_DEFAULT_BITRATE 200*1000

static const AVOption options[]={
{"b", "set bitrate (in bits/s)", OFFSET(bit_rate), AV_OPT_TYPE_INT, AV_CODEC_DEFAULT_BITRATE , NULL, INT_MIN, INT_MAX, A|V|E},
{"ab", "set bitrate (in bits/s)", OFFSET(bit_rate), AV_OPT_TYPE_INT, 128*1000 , NULL, INT_MIN, INT_MAX, A|E},
{"bt", "set video bitrate tolerance (in bits/s)", OFFSET(bit_rate_tolerance), AV_OPT_TYPE_INT, AV_CODEC_DEFAULT_BITRATE*20, NULL, 1, INT_MAX, V|E},
{"flags", NULL, OFFSET(flags), AV_OPT_TYPE_FLAGS, DEFAULT, NULL, 0, UINT_MAX, V|A|E|D, "flags"},
{"mv4", "use four motion vector by macroblock (mpeg4)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_4MV , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"obmc", "use overlapped block motion compensation (h263+)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_OBMC , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#endif
{"qpel", "use 1/4 pel motion compensation", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_QPEL , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"loop", "use loop filter", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_LOOP_FILTER , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"qscale", "use fixed qscale", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_QSCALE , NULL, INT_MIN, INT_MAX, 0, "flags"},
{"gmc", "use gmc", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_GMC , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"mv0", "always try a mb with mv=<0,0>", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_MV0 , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"part", "use data partitioning", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_PART , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#endif
{"input_preserved", NULL, 0, AV_OPT_TYPE_CONST, CODEC_FLAG_INPUT_PRESERVED , NULL, INT_MIN, INT_MAX, 0, "flags"},
{"pass1", "use internal 2pass ratecontrol in first  pass mode", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_PASS1 , NULL, INT_MIN, INT_MAX, 0, "flags"},
{"pass2", "use internal 2pass ratecontrol in second pass mode", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_PASS2 , NULL, INT_MIN, INT_MAX, 0, "flags"},
#if FF_API_MJPEG_GLOBAL_OPTS
{"extern_huff", "use external huffman table (for mjpeg)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_EXTERN_HUFF , NULL, INT_MIN, INT_MAX, 0, "flags"},
#endif
{"gray", "only decode/encode grayscale", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_GRAY , NULL, INT_MIN, INT_MAX, V|E|D, "flags"},
{"emu_edge", "don't draw edges", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_EMU_EDGE , NULL, INT_MIN, INT_MAX, 0, "flags"},
{"psnr", "error[?] variables will be set during encoding", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_PSNR , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"truncated", NULL, 0, AV_OPT_TYPE_CONST, CODEC_FLAG_TRUNCATED , NULL, INT_MIN, INT_MAX, 0, "flags"},
{"naq", "normalize adaptive quantization", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_NORMALIZE_AQP , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"ildct", "use interlaced dct", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_INTERLACED_DCT , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"low_delay", "force low delay", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_LOW_DELAY , NULL, INT_MIN, INT_MAX, V|D|E, "flags"},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"alt", "enable alternate scantable (mpeg2/mpeg4)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_ALT_SCAN , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#endif
{"global_header", "place global headers in extradata instead of every keyframe", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_GLOBAL_HEADER , NULL, INT_MIN, INT_MAX, V|A|E, "flags"},
{"bitexact", "use only bitexact stuff (except (i)dct)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_BITEXACT , NULL, INT_MIN, INT_MAX, A|V|S|D|E, "flags"},
{"aic", "h263 advanced intra coding / mpeg4 ac prediction", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_AC_PRED , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"umv", "use unlimited motion vectors", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_H263P_UMV , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#endif
{"cbp", "use rate distortion optimization for cbp", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_CBP_RD , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"qprd", "use rate distortion optimization for qp selection", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_QP_RD , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"aiv", "h263 alternative inter vlc", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_H263P_AIV , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"slice", NULL, 0, AV_OPT_TYPE_CONST, CODEC_FLAG_H263P_SLICE_STRUCT , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#endif
{"ilme", "interlaced motion estimation", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_INTERLACED_ME , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"scan_offset", "will reserve space for svcd scan offset user data", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_SVCD_SCAN_OFFSET , NULL, INT_MIN, INT_MAX, V|E, "flags"},
#endif
{"cgop", "closed gop", 0, AV_OPT_TYPE_CONST, CODEC_FLAG_CLOSED_GOP , NULL, INT_MIN, INT_MAX, V|E, "flags"},
{"fast", "allow non spec compliant speedup tricks", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_FAST , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"sgop", "strictly enforce gop size", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_STRICT_GOP , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"noout", "skip bitstream encoding", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_NO_OUTPUT , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"local_header", "place global headers at every keyframe instead of in extradata", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_LOCAL_HEADER , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"showall", "Show all frames before the first keyframe", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_SHOW_ALL , NULL, INT_MIN, INT_MAX, V|D, "flags2"},
{"sub_id", NULL, OFFSET(sub_id), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"me_method", "set motion estimation method", OFFSET(me_method), AV_OPT_TYPE_INT, ME_EPZS , NULL, INT_MIN, INT_MAX, V|E, "me_method"},
{"zero", "zero motion estimation (fastest)", 0, AV_OPT_TYPE_CONST, ME_ZERO , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"full", "full motion estimation (slowest)", 0, AV_OPT_TYPE_CONST, ME_FULL , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"epzs", "EPZS motion estimation (default)", 0, AV_OPT_TYPE_CONST, ME_EPZS , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"esa", "esa motion estimation (alias for full)", 0, AV_OPT_TYPE_CONST, ME_FULL , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"tesa", "tesa motion estimation", 0, AV_OPT_TYPE_CONST, ME_TESA , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"dia", "dia motion estimation (alias for epzs)", 0, AV_OPT_TYPE_CONST, ME_EPZS , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"log", "log motion estimation", 0, AV_OPT_TYPE_CONST, ME_LOG , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"phods", "phods motion estimation", 0, AV_OPT_TYPE_CONST, ME_PHODS , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"x1", "X1 motion estimation", 0, AV_OPT_TYPE_CONST, ME_X1 , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"hex", "hex motion estimation", 0, AV_OPT_TYPE_CONST, ME_HEX , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"umh", "umh motion estimation", 0, AV_OPT_TYPE_CONST, ME_UMH , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"iter", "iter motion estimation", 0, AV_OPT_TYPE_CONST, ME_ITER , NULL, INT_MIN, INT_MAX, V|E, "me_method" },
{"extradata_size", NULL, OFFSET(extradata_size), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"time_base", NULL, OFFSET(time_base), AV_OPT_TYPE_RATIONAL, 0, NULL, INT_MIN, INT_MAX},
{"g", "set the group of picture size", OFFSET(gop_size), AV_OPT_TYPE_INT, 12 , NULL, INT_MIN, INT_MAX, V|E},
{"ar", "set audio sampling rate (in Hz)", OFFSET(sample_rate), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, A|D|E},
{"ac", "set number of audio channels", OFFSET(channels), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, A|D|E},
{"cutoff", "set cutoff bandwidth", OFFSET(cutoff), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, A|E},
{"frame_size", NULL, OFFSET(frame_size), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, A|E},
{"frame_number", NULL, OFFSET(frame_number), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"delay", NULL, OFFSET(delay), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"qcomp", "video quantizer scale compression (VBR)", OFFSET(qcompress), AV_OPT_TYPE_FLOAT, 0.5, NULL, -FLT_MAX, FLT_MAX, V|E},
{"qblur", "video quantizer scale blur (VBR)", OFFSET(qblur), AV_OPT_TYPE_FLOAT, 0.5, NULL, -1, FLT_MAX, V|E},
{"qmin", "min video quantizer scale (VBR)", OFFSET(qmin), AV_OPT_TYPE_INT, 2, NULL, -1, 69, V|E},
{"qmax", "max video quantizer scale (VBR)", OFFSET(qmax), AV_OPT_TYPE_INT, 31, NULL, -1, 69, V|E},
{"qdiff", "max difference between the quantizer scale (VBR)", OFFSET(max_qdiff), AV_OPT_TYPE_INT, 3 , NULL, INT_MIN, INT_MAX, V|E},
{"bf", "use 'frames' B frames", OFFSET(max_b_frames), AV_OPT_TYPE_INT, DEFAULT, NULL, -1, FF_MAX_B_FRAMES, V|E},
{"b_qfactor", "qp factor between p and b frames", OFFSET(b_quant_factor), AV_OPT_TYPE_FLOAT, 1.25, NULL, -FLT_MAX, FLT_MAX, V|E},
{"rc_strategy", "ratecontrol method", OFFSET(rc_strategy), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"b_strategy", "strategy to choose between I/P/B-frames", OFFSET(b_frame_strategy), AV_OPT_TYPE_INT, 0 , NULL, INT_MIN, INT_MAX, V|E},
#if FF_API_X264_GLOBAL_OPTS
{"wpredp", "weighted prediction analysis method", OFFSET(weighted_p_pred), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, V|E},
#endif
{"ps", "rtp payload size in bytes", OFFSET(rtp_payload_size), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"mv_bits", NULL, OFFSET(mv_bits), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"header_bits", NULL, OFFSET(header_bits), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"i_tex_bits", NULL, OFFSET(i_tex_bits), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"p_tex_bits", NULL, OFFSET(p_tex_bits), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"i_count", NULL, OFFSET(i_count), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"p_count", NULL, OFFSET(p_count), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"skip_count", NULL, OFFSET(skip_count), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"misc_bits", NULL, OFFSET(misc_bits), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"frame_bits", NULL, OFFSET(frame_bits), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"codec_tag", NULL, OFFSET(codec_tag), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"bug", "workaround not auto detected encoder bugs", OFFSET(workaround_bugs), AV_OPT_TYPE_FLAGS, FF_BUG_AUTODETECT , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"autodetect", NULL, 0, AV_OPT_TYPE_CONST, FF_BUG_AUTODETECT , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"old_msmpeg4", "some old lavc generated msmpeg4v3 files (no autodetection)", 0, AV_OPT_TYPE_CONST, FF_BUG_OLD_MSMPEG4 , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"xvid_ilace", "Xvid interlacing bug (autodetected if fourcc==XVIX)", 0, AV_OPT_TYPE_CONST, FF_BUG_XVID_ILACE , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"ump4", "(autodetected if fourcc==UMP4)", 0, AV_OPT_TYPE_CONST, FF_BUG_UMP4 , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"no_padding", "padding bug (autodetected)", 0, AV_OPT_TYPE_CONST, FF_BUG_NO_PADDING , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"amv", NULL, 0, AV_OPT_TYPE_CONST, FF_BUG_AMV , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"ac_vlc", "illegal vlc bug (autodetected per fourcc)", 0, AV_OPT_TYPE_CONST, FF_BUG_AC_VLC , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"qpel_chroma", NULL, 0, AV_OPT_TYPE_CONST, FF_BUG_QPEL_CHROMA , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"std_qpel", "old standard qpel (autodetected per fourcc/version)", 0, AV_OPT_TYPE_CONST, FF_BUG_STD_QPEL , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"qpel_chroma2", NULL, 0, AV_OPT_TYPE_CONST, FF_BUG_QPEL_CHROMA2 , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"direct_blocksize", "direct-qpel-blocksize bug (autodetected per fourcc/version)", 0, AV_OPT_TYPE_CONST, FF_BUG_DIRECT_BLOCKSIZE , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"edge", "edge padding bug (autodetected per fourcc/version)", 0, AV_OPT_TYPE_CONST, FF_BUG_EDGE , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"hpel_chroma", NULL, 0, AV_OPT_TYPE_CONST, FF_BUG_HPEL_CHROMA , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"dc_clip", NULL, 0, AV_OPT_TYPE_CONST, FF_BUG_DC_CLIP , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"ms", "workaround various bugs in microsofts broken decoders", 0, AV_OPT_TYPE_CONST, FF_BUG_MS , NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"trunc", "trancated frames", 0, AV_OPT_TYPE_CONST, FF_BUG_TRUNCATED, NULL, INT_MIN, INT_MAX, V|D, "bug"},
{"lelim", "single coefficient elimination threshold for luminance (negative values also consider dc coefficient)", OFFSET(luma_elim_threshold), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"celim", "single coefficient elimination threshold for chrominance (negative values also consider dc coefficient)", OFFSET(chroma_elim_threshold), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"strict", "how strictly to follow the standards", OFFSET(strict_std_compliance), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, A|V|D|E, "strict"},
{"very", "strictly conform to a older more strict version of the spec or reference software", 0, AV_OPT_TYPE_CONST, FF_COMPLIANCE_VERY_STRICT , NULL, INT_MIN, INT_MAX, V|D|E, "strict"},
{"strict", "strictly conform to all the things in the spec no matter what consequences", 0, AV_OPT_TYPE_CONST, FF_COMPLIANCE_STRICT , NULL, INT_MIN, INT_MAX, V|D|E, "strict"},
{"normal", NULL, 0, AV_OPT_TYPE_CONST, FF_COMPLIANCE_NORMAL , NULL, INT_MIN, INT_MAX, V|D|E, "strict"},
{"unofficial", "allow unofficial extensions", 0, AV_OPT_TYPE_CONST, FF_COMPLIANCE_UNOFFICIAL , NULL, INT_MIN, INT_MAX, V|D|E, "strict"},
{"experimental", "allow non standardized experimental things", 0, AV_OPT_TYPE_CONST, FF_COMPLIANCE_EXPERIMENTAL , NULL, INT_MIN, INT_MAX, V|D|E, "strict"},
{"b_qoffset", "qp offset between P and B frames", OFFSET(b_quant_offset), AV_OPT_TYPE_FLOAT, 1.25, NULL, -FLT_MAX, FLT_MAX, V|E},
#if FF_API_ER
{"er", "set error detection aggressivity", OFFSET(error_recognition), AV_OPT_TYPE_INT, FF_ER_CAREFUL , NULL, INT_MIN, INT_MAX, A|V|D, "er"},
{"careful", NULL, 0, AV_OPT_TYPE_CONST, FF_ER_CAREFUL , NULL, INT_MIN, INT_MAX, V|D, "er"},
{"compliant", NULL, 0, AV_OPT_TYPE_CONST, FF_ER_COMPLIANT , NULL, INT_MIN, INT_MAX, V|D, "er"},
{"aggressive", NULL, 0, AV_OPT_TYPE_CONST, FF_ER_AGGRESSIVE , NULL, INT_MIN, INT_MAX, V|D, "er"},
{"very_aggressive", NULL, 0, AV_OPT_TYPE_CONST, FF_ER_VERY_AGGRESSIVE , NULL, INT_MIN, INT_MAX, V|D, "er"},
{"explode", "abort decoding on error recognition", 0, AV_OPT_TYPE_CONST, FF_ER_EXPLODE , NULL, INT_MIN, INT_MAX, V|D, "er"},
#endif /* FF_API_ER */
{"err_detect", "set error detection flags", OFFSET(err_recognition), AV_OPT_TYPE_FLAGS, AV_EF_CRCCHECK , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"crccheck", "verify embedded CRCs", 0, AV_OPT_TYPE_CONST, AV_EF_CRCCHECK , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"bitstream", "detect bitstream specification deviations", 0, AV_OPT_TYPE_CONST, AV_EF_BITSTREAM , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"buffer", "detect improper bitstream length", 0, AV_OPT_TYPE_CONST, AV_EF_BUFFER , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"explode", "abort decoding on minor error detection", 0, AV_OPT_TYPE_CONST, AV_EF_EXPLODE , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"careful",    "consider things that violate the spec and have not been seen in the wild as errors", 0, AV_OPT_TYPE_CONST, AV_EF_CAREFUL , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"compliant",  "consider all spec non compliancies as errors", 0, AV_OPT_TYPE_CONST, AV_EF_COMPLIANT , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"aggressive", "consider things that a sane encoder shouldnt do as an error", 0, AV_OPT_TYPE_CONST, AV_EF_AGGRESSIVE , NULL, INT_MIN, INT_MAX, A|V|D, "err_detect"},
{"has_b_frames", NULL, OFFSET(has_b_frames), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"block_align", NULL, OFFSET(block_align), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
#if FF_API_PARSE_FRAME
{"parse_only", NULL, OFFSET(parse_only), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
#endif
{"mpeg_quant", "use MPEG quantizers instead of H.263", OFFSET(mpeg_quant), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"stats_out", NULL, OFFSET(stats_out), AV_OPT_TYPE_STRING, 0, NULL, CHAR_MIN, CHAR_MAX},
{"stats_in", NULL, OFFSET(stats_in), AV_OPT_TYPE_STRING, 0, NULL, CHAR_MIN, CHAR_MAX},
{"qsquish", "how to keep quantizer between qmin and qmax (0 = clip, 1 = use differentiable function)", OFFSET(rc_qsquish), AV_OPT_TYPE_FLOAT, DEFAULT, NULL, 0, 99, V|E},
{"rc_qmod_amp", "experimental quantizer modulation", OFFSET(rc_qmod_amp), AV_OPT_TYPE_FLOAT, DEFAULT, NULL, -FLT_MAX, FLT_MAX, V|E},
{"rc_qmod_freq", "experimental quantizer modulation", OFFSET(rc_qmod_freq), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"rc_override_count", NULL, OFFSET(rc_override_count), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"rc_eq", "set rate control equation", OFFSET(rc_eq), AV_OPT_TYPE_STRING, 0, NULL, CHAR_MIN, CHAR_MAX, V|E},
{"maxrate", "set max video bitrate tolerance (in bits/s)", OFFSET(rc_max_rate), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"minrate", "set min video bitrate tolerance (in bits/s)", OFFSET(rc_min_rate), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"bufsize", "set ratecontrol buffer size (in bits)", OFFSET(rc_buffer_size), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, A|V|E},
{"rc_buf_aggressivity", "currently useless", OFFSET(rc_buffer_aggressivity), AV_OPT_TYPE_FLOAT, 1.0, NULL, -FLT_MAX, FLT_MAX, V|E},
{"i_qfactor", "qp factor between P and I frames", OFFSET(i_quant_factor), AV_OPT_TYPE_FLOAT, -0.8, NULL, -FLT_MAX, FLT_MAX, V|E},
{"i_qoffset", "qp offset between P and I frames", OFFSET(i_quant_offset), AV_OPT_TYPE_FLOAT, 0.0, NULL, -FLT_MAX, FLT_MAX, V|E},
{"rc_init_cplx", "initial complexity for 1-pass encoding", OFFSET(rc_initial_cplx), AV_OPT_TYPE_FLOAT, DEFAULT, NULL, -FLT_MAX, FLT_MAX, V|E},
{"dct", "DCT algorithm", OFFSET(dct_algo), AV_OPT_TYPE_INT, DEFAULT, NULL, 0, INT_MAX, V|E, "dct"},
{"auto", "autoselect a good one (default)", 0, AV_OPT_TYPE_CONST, FF_DCT_AUTO , NULL, INT_MIN, INT_MAX, V|E, "dct"},
{"fastint", "fast integer", 0, AV_OPT_TYPE_CONST, FF_DCT_FASTINT , NULL, INT_MIN, INT_MAX, V|E, "dct"},
{"int", "accurate integer", 0, AV_OPT_TYPE_CONST, FF_DCT_INT , NULL, INT_MIN, INT_MAX, V|E, "dct"},
{"mmx", NULL, 0, AV_OPT_TYPE_CONST, FF_DCT_MMX , NULL, INT_MIN, INT_MAX, V|E, "dct"},
{"mlib", NULL, 0, AV_OPT_TYPE_CONST, FF_DCT_MLIB , NULL, INT_MIN, INT_MAX, V|E, "dct"},
{"altivec", NULL, 0, AV_OPT_TYPE_CONST, FF_DCT_ALTIVEC , NULL, INT_MIN, INT_MAX, V|E, "dct"},
{"faan", "floating point AAN DCT", 0, AV_OPT_TYPE_CONST, FF_DCT_FAAN , NULL, INT_MIN, INT_MAX, V|E, "dct"},
{"lumi_mask", "compresses bright areas stronger than medium ones", OFFSET(lumi_masking), AV_OPT_TYPE_FLOAT, 0, NULL, -FLT_MAX, FLT_MAX, V|E},
{"tcplx_mask", "temporal complexity masking", OFFSET(temporal_cplx_masking), AV_OPT_TYPE_FLOAT, 0, NULL, -FLT_MAX, FLT_MAX, V|E},
{"scplx_mask", "spatial complexity masking", OFFSET(spatial_cplx_masking), AV_OPT_TYPE_FLOAT, 0, NULL, -FLT_MAX, FLT_MAX, V|E},
{"p_mask", "inter masking", OFFSET(p_masking), AV_OPT_TYPE_FLOAT, 0, NULL, -FLT_MAX, FLT_MAX, V|E},
{"dark_mask", "compresses dark areas stronger than medium ones", OFFSET(dark_masking), AV_OPT_TYPE_FLOAT, 0, NULL, -FLT_MAX, FLT_MAX, V|E},
{"idct", "select IDCT implementation", OFFSET(idct_algo), AV_OPT_TYPE_INT, DEFAULT, NULL, 0, INT_MAX, V|E|D, "idct"},
{"auto", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_AUTO , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"int", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_INT , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"simple", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SIMPLE , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"simplemmx", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SIMPLEMMX , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"libmpeg2mmx", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_LIBMPEG2MMX , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"ps2", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_PS2 , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"mlib", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_MLIB , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"arm", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_ARM , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"altivec", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_ALTIVEC , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"sh4", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SH4 , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"simplearm", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SIMPLEARM , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"simplearmv5te", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SIMPLEARMV5TE , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"simplearmv6", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SIMPLEARMV6 , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"simpleneon", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SIMPLENEON , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"simplealpha", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_SIMPLEALPHA , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"h264", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_H264 , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"vp3", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_VP3 , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"ipp", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_IPP , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"xvidmmx", NULL, 0, AV_OPT_TYPE_CONST, FF_IDCT_XVIDMMX , NULL, INT_MIN, INT_MAX, V|E|D, "idct"},
{"faani", "floating point AAN IDCT", 0, AV_OPT_TYPE_CONST, FF_IDCT_FAAN , NULL, INT_MIN, INT_MAX, V|D|E, "idct"},
{"slice_count", NULL, OFFSET(slice_count), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"ec", "set error concealment strategy", OFFSET(error_concealment), AV_OPT_TYPE_FLAGS, 3 , NULL, INT_MIN, INT_MAX, V|D, "ec"},
{"guess_mvs", "iterative motion vector (MV) search (slow)", 0, AV_OPT_TYPE_CONST, FF_EC_GUESS_MVS , NULL, INT_MIN, INT_MAX, V|D, "ec"},
{"deblock", "use strong deblock filter for damaged MBs", 0, AV_OPT_TYPE_CONST, FF_EC_DEBLOCK , NULL, INT_MIN, INT_MAX, V|D, "ec"},
{"bits_per_coded_sample", NULL, OFFSET(bits_per_coded_sample), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"pred", "prediction method", OFFSET(prediction_method), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "pred"},
{"left", NULL, 0, AV_OPT_TYPE_CONST, FF_PRED_LEFT , NULL, INT_MIN, INT_MAX, V|E, "pred"},
{"plane", NULL, 0, AV_OPT_TYPE_CONST, FF_PRED_PLANE , NULL, INT_MIN, INT_MAX, V|E, "pred"},
{"median", NULL, 0, AV_OPT_TYPE_CONST, FF_PRED_MEDIAN , NULL, INT_MIN, INT_MAX, V|E, "pred"},
{"aspect", "sample aspect ratio", OFFSET(sample_aspect_ratio), AV_OPT_TYPE_RATIONAL, 0, NULL, 0, 10, V|E},
{"debug", "print specific debug info", OFFSET(debug), AV_OPT_TYPE_FLAGS, DEFAULT, NULL, 0, INT_MAX, V|A|S|E|D, "debug"},
{"pict", "picture info", 0, AV_OPT_TYPE_CONST, FF_DEBUG_PICT_INFO , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"rc", "rate control", 0, AV_OPT_TYPE_CONST, FF_DEBUG_RC , NULL, INT_MIN, INT_MAX, V|E, "debug"},
{"bitstream", NULL, 0, AV_OPT_TYPE_CONST, FF_DEBUG_BITSTREAM , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"mb_type", "macroblock (MB) type", 0, AV_OPT_TYPE_CONST, FF_DEBUG_MB_TYPE , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"qp", "per-block quantization parameter (QP)", 0, AV_OPT_TYPE_CONST, FF_DEBUG_QP , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"mv", "motion vector", 0, AV_OPT_TYPE_CONST, FF_DEBUG_MV , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"dct_coeff", NULL, 0, AV_OPT_TYPE_CONST, FF_DEBUG_DCT_COEFF , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"skip", NULL, 0, AV_OPT_TYPE_CONST, FF_DEBUG_SKIP , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"startcode", NULL, 0, AV_OPT_TYPE_CONST, FF_DEBUG_STARTCODE , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"pts", NULL, 0, AV_OPT_TYPE_CONST, FF_DEBUG_PTS , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"er", "error recognition", 0, AV_OPT_TYPE_CONST, FF_DEBUG_ER , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"mmco", "memory management control operations (H.264)", 0, AV_OPT_TYPE_CONST, FF_DEBUG_MMCO , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"bugs", NULL, 0, AV_OPT_TYPE_CONST, FF_DEBUG_BUGS , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"vis_qp", "visualize quantization parameter (QP), lower QP are tinted greener", 0, AV_OPT_TYPE_CONST, FF_DEBUG_VIS_QP , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"vis_mb_type", "visualize block types", 0, AV_OPT_TYPE_CONST, FF_DEBUG_VIS_MB_TYPE , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"buffers", "picture buffer allocations", 0, AV_OPT_TYPE_CONST, FF_DEBUG_BUFFERS , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"thread_ops", "threading operations", 0, AV_OPT_TYPE_CONST, FF_DEBUG_THREADS , NULL, INT_MIN, INT_MAX, V|D, "debug"},
{"vismv", "visualize motion vectors (MVs)", OFFSET(debug_mv), AV_OPT_TYPE_INT, DEFAULT, NULL, 0, INT_MAX, V|D, "debug_mv"},
{"pf", "forward predicted MVs of P-frames", 0, AV_OPT_TYPE_CONST, FF_DEBUG_VIS_MV_P_FOR , NULL, INT_MIN, INT_MAX, V|D, "debug_mv"},
{"bf", "forward predicted MVs of B-frames", 0, AV_OPT_TYPE_CONST, FF_DEBUG_VIS_MV_B_FOR , NULL, INT_MIN, INT_MAX, V|D, "debug_mv"},
{"bb", "backward predicted MVs of B-frames", 0, AV_OPT_TYPE_CONST, FF_DEBUG_VIS_MV_B_BACK , NULL, INT_MIN, INT_MAX, V|D, "debug_mv"},
{"cmp", "full pel me compare function", OFFSET(me_cmp), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"subcmp", "sub pel me compare function", OFFSET(me_sub_cmp), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"mbcmp", "macroblock compare function", OFFSET(mb_cmp), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"ildctcmp", "interlaced dct compare function", OFFSET(ildct_cmp), AV_OPT_TYPE_INT, FF_CMP_VSAD , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"dia_size", "diamond type & size for motion estimation", OFFSET(dia_size), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"last_pred", "amount of motion predictors from the previous frame", OFFSET(last_predictor_count), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"preme", "pre motion estimation", OFFSET(pre_me), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"precmp", "pre motion estimation compare function", OFFSET(me_pre_cmp), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"sad", "sum of absolute differences, fast (default)", 0, AV_OPT_TYPE_CONST, FF_CMP_SAD , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"sse", "sum of squared errors", 0, AV_OPT_TYPE_CONST, FF_CMP_SSE , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"satd", "sum of absolute Hadamard transformed differences", 0, AV_OPT_TYPE_CONST, FF_CMP_SATD , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"dct", "sum of absolute DCT transformed differences", 0, AV_OPT_TYPE_CONST, FF_CMP_DCT , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"psnr", "sum of squared quantization errors (avoid, low quality)", 0, AV_OPT_TYPE_CONST, FF_CMP_PSNR , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"bit", "number of bits needed for the block", 0, AV_OPT_TYPE_CONST, FF_CMP_BIT , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"rd", "rate distortion optimal, slow", 0, AV_OPT_TYPE_CONST, FF_CMP_RD , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"zero", "0", 0, AV_OPT_TYPE_CONST, FF_CMP_ZERO , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"vsad", "sum of absolute vertical differences", 0, AV_OPT_TYPE_CONST, FF_CMP_VSAD , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"vsse", "sum of squared vertical differences", 0, AV_OPT_TYPE_CONST, FF_CMP_VSSE , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"nsse", "noise preserving sum of squared differences", 0, AV_OPT_TYPE_CONST, FF_CMP_NSSE , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
#if CONFIG_SNOW_ENCODER
{"w53", "5/3 wavelet, only used in snow", 0, AV_OPT_TYPE_CONST, FF_CMP_W53 , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"w97", "9/7 wavelet, only used in snow", 0, AV_OPT_TYPE_CONST, FF_CMP_W97 , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
#endif
{"dctmax", NULL, 0, AV_OPT_TYPE_CONST, FF_CMP_DCTMAX , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"chroma", NULL, 0, AV_OPT_TYPE_CONST, FF_CMP_CHROMA , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"pre_dia_size", "diamond type & size for motion estimation pre-pass", OFFSET(pre_dia_size), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"subq", "sub pel motion estimation quality", OFFSET(me_subpel_quality), AV_OPT_TYPE_INT, 8 , NULL, INT_MIN, INT_MAX, V|E},
{"dtg_active_format", NULL, OFFSET(dtg_active_format), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"me_range", "limit motion vectors range (1023 for DivX player)", OFFSET(me_range), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"ibias", "intra quant bias", OFFSET(intra_quant_bias), AV_OPT_TYPE_INT, FF_DEFAULT_QUANT_BIAS , NULL, INT_MIN, INT_MAX, V|E},
{"pbias", "inter quant bias", OFFSET(inter_quant_bias), AV_OPT_TYPE_INT, FF_DEFAULT_QUANT_BIAS , NULL, INT_MIN, INT_MAX, V|E},
{"color_table_id", NULL, OFFSET(color_table_id), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"global_quality", NULL, OFFSET(global_quality), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|A|E},
{"coder", NULL, OFFSET(coder_type), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "coder"},
{"vlc", "variable length coder / huffman coder", 0, AV_OPT_TYPE_CONST, FF_CODER_TYPE_VLC , NULL, INT_MIN, INT_MAX, V|E, "coder"},
{"ac", "arithmetic coder", 0, AV_OPT_TYPE_CONST, FF_CODER_TYPE_AC , NULL, INT_MIN, INT_MAX, V|E, "coder"},
{"raw", "raw (no encoding)", 0, AV_OPT_TYPE_CONST, FF_CODER_TYPE_RAW , NULL, INT_MIN, INT_MAX, V|E, "coder"},
{"rle", "run-length coder", 0, AV_OPT_TYPE_CONST, FF_CODER_TYPE_RLE , NULL, INT_MIN, INT_MAX, V|E, "coder"},
{"deflate", "deflate-based coder", 0, AV_OPT_TYPE_CONST, FF_CODER_TYPE_DEFLATE , NULL, INT_MIN, INT_MAX, V|E, "coder"},
{"context", "context model", OFFSET(context_model), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"slice_flags", NULL, OFFSET(slice_flags), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"xvmc_acceleration", NULL, OFFSET(xvmc_acceleration), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"mbd", "macroblock decision algorithm (high quality mode)", OFFSET(mb_decision), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "mbd"},
{"simple", "use mbcmp (default)", 0, AV_OPT_TYPE_CONST, FF_MB_DECISION_SIMPLE , NULL, INT_MIN, INT_MAX, V|E, "mbd"},
{"bits", "use fewest bits", 0, AV_OPT_TYPE_CONST, FF_MB_DECISION_BITS , NULL, INT_MIN, INT_MAX, V|E, "mbd"},
{"rd", "use best rate distortion", 0, AV_OPT_TYPE_CONST, FF_MB_DECISION_RD , NULL, INT_MIN, INT_MAX, V|E, "mbd"},
{"stream_codec_tag", NULL, OFFSET(stream_codec_tag), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"sc_threshold", "scene change threshold", OFFSET(scenechange_threshold), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"lmin", "min lagrange factor (VBR)", OFFSET(lmin), AV_OPT_TYPE_INT,  2*FF_QP2LAMBDA, NULL, 0, INT_MAX, V|E},
{"lmax", "max lagrange factor (VBR)", OFFSET(lmax), AV_OPT_TYPE_INT, 31*FF_QP2LAMBDA, NULL, 0, INT_MAX, V|E},
{"nr", "noise reduction", OFFSET(noise_reduction), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"rc_init_occupancy", "number of bits which should be loaded into the rc buffer before decoding starts", OFFSET(rc_initial_buffer_occupancy), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"inter_threshold", NULL, OFFSET(inter_threshold), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
#if FF_API_X264_GLOBAL_OPTS
#define X264_DEFAULTS CODEC_FLAG2_FASTPSKIP|CODEC_FLAG2_PSY|CODEC_FLAG2_MBTREE
#else
#define X264_DEFAULTS 0
#endif
#if FF_API_LAME_GLOBAL_OPTS
#define LAME_DEFAULTS CODEC_FLAG2_BIT_RESERVOIR
#else
#define LAME_DEFAULTS 0
#endif
{"flags2", NULL, OFFSET(flags2), AV_OPT_TYPE_FLAGS, X264_DEFAULTS|LAME_DEFAULTS, NULL, 0, UINT_MAX, V|A|E|D, "flags2"},
{"error", NULL, OFFSET(error_rate), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
#if FF_API_ANTIALIAS_ALGO
{"antialias", "MP3 antialias algorithm", OFFSET(antialias_algo), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|D, "aa"},
{"auto", NULL, 0, AV_OPT_TYPE_CONST, FF_AA_AUTO , NULL, INT_MIN, INT_MAX, V|D, "aa"},
{"fastint", NULL, 0, AV_OPT_TYPE_CONST, FF_AA_FASTINT , NULL, INT_MIN, INT_MAX, V|D, "aa"},
{"int", NULL, 0, AV_OPT_TYPE_CONST, FF_AA_INT , NULL, INT_MIN, INT_MAX, V|D, "aa"},
{"float", NULL, 0, AV_OPT_TYPE_CONST, FF_AA_FLOAT , NULL, INT_MIN, INT_MAX, V|D, "aa"},
#endif
{"qns", "quantizer noise shaping", OFFSET(quantizer_noise_shaping), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"threads", NULL, OFFSET(thread_count), AV_OPT_TYPE_INT, 1, NULL, 0, INT_MAX, V|E|D, "threads"},
{"auto", "detect a good number of threads", 0, AV_OPT_TYPE_CONST, 0 , NULL, INT_MIN, INT_MAX, V|E|D, "threads"},
{"me_threshold", "motion estimaton threshold", OFFSET(me_threshold), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"mb_threshold", "macroblock threshold", OFFSET(mb_threshold), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"dc", "intra_dc_precision", OFFSET(intra_dc_precision), AV_OPT_TYPE_INT, 0 , NULL, INT_MIN, INT_MAX, V|E},
{"nssew", "nsse weight", OFFSET(nsse_weight), AV_OPT_TYPE_INT, 8 , NULL, INT_MIN, INT_MAX, V|E},
{"skip_top", "number of macroblock rows at the top which are skipped", OFFSET(skip_top), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|D},
{"skip_bottom", "number of macroblock rows at the bottom which are skipped", OFFSET(skip_bottom), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|D},
{"profile", NULL, OFFSET(profile), AV_OPT_TYPE_INT, FF_PROFILE_UNKNOWN , NULL, INT_MIN, INT_MAX, V|A|E, "profile"},
{"unknown", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_UNKNOWN , NULL, INT_MIN, INT_MAX, V|A|E, "profile"},
{"aac_main", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_AAC_MAIN , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"aac_low", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_AAC_LOW , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"aac_ssr", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_AAC_SSR , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"aac_ltp", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_AAC_LTP , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"dts", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_DTS , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"dts_es", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_DTS_ES , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"dts_96_24", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_DTS_96_24 , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"dts_hd_hra", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_DTS_HD_HRA , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"dts_hd_ma", NULL, 0, AV_OPT_TYPE_CONST, FF_PROFILE_DTS_HD_MA , NULL, INT_MIN, INT_MAX, A|E, "profile"},
{"level", NULL, OFFSET(level), AV_OPT_TYPE_INT, FF_LEVEL_UNKNOWN , NULL, INT_MIN, INT_MAX, V|A|E, "level"},
{"unknown", NULL, 0, AV_OPT_TYPE_CONST, FF_LEVEL_UNKNOWN , NULL, INT_MIN, INT_MAX, V|A|E, "level"},
{"lowres", "decode at 1= 1/2, 2=1/4, 3=1/8 resolutions", OFFSET(lowres), AV_OPT_TYPE_INT, 0, NULL, 0, INT_MAX, V|A|D},
{"skip_threshold", "frame skip threshold", OFFSET(frame_skip_threshold), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"skip_factor", "frame skip factor", OFFSET(frame_skip_factor), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"skip_exp", "frame skip exponent", OFFSET(frame_skip_exp), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
{"skipcmp", "frame skip compare function", OFFSET(frame_skip_cmp), AV_OPT_TYPE_INT, FF_CMP_DCTMAX , NULL, INT_MIN, INT_MAX, V|E, "cmp_func"},
{"border_mask", "increases the quantizer for macroblocks close to borders", OFFSET(border_masking), AV_OPT_TYPE_FLOAT, DEFAULT, NULL, -FLT_MAX, FLT_MAX, V|E},
{"mblmin", "min macroblock lagrange factor (VBR)", OFFSET(mb_lmin), AV_OPT_TYPE_INT, FF_QP2LAMBDA * 2, NULL, 1, FF_LAMBDA_MAX, V|E},
{"mblmax", "max macroblock lagrange factor (VBR)", OFFSET(mb_lmax), AV_OPT_TYPE_INT, FF_QP2LAMBDA * 31, NULL, 1, FF_LAMBDA_MAX, V|E},
{"mepc", "motion estimation bitrate penalty compensation (1.0 = 256)", OFFSET(me_penalty_compensation), AV_OPT_TYPE_INT, 256 , NULL, INT_MIN, INT_MAX, V|E},
{"skip_loop_filter", NULL, OFFSET(skip_loop_filter), AV_OPT_TYPE_INT, AVDISCARD_DEFAULT , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"skip_idct"       , NULL, OFFSET(skip_idct)       , AV_OPT_TYPE_INT, AVDISCARD_DEFAULT , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"skip_frame"      , NULL, OFFSET(skip_frame)      , AV_OPT_TYPE_INT, AVDISCARD_DEFAULT , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"none"            , NULL, 0, AV_OPT_TYPE_CONST, AVDISCARD_NONE    , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"default"         , NULL, 0, AV_OPT_TYPE_CONST, AVDISCARD_DEFAULT , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"noref"           , NULL, 0, AV_OPT_TYPE_CONST, AVDISCARD_NONREF  , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"bidir"           , NULL, 0, AV_OPT_TYPE_CONST, AVDISCARD_BIDIR   , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"nokey"           , NULL, 0, AV_OPT_TYPE_CONST, AVDISCARD_NONKEY  , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"all"             , NULL, 0, AV_OPT_TYPE_CONST, AVDISCARD_ALL     , NULL, INT_MIN, INT_MAX, V|D, "avdiscard"},
{"bidir_refine", "refine the two motion vectors used in bidirectional macroblocks", OFFSET(bidir_refine), AV_OPT_TYPE_INT, 1, NULL, 0, 4, V|E},
{"brd_scale", "downscales frames for dynamic B-frame decision", OFFSET(brd_scale), AV_OPT_TYPE_INT, DEFAULT, NULL, 0, 10, V|E},
#if FF_API_X264_GLOBAL_OPTS
{"crf", "enables constant quality mode, and selects the quality (x264/VP8)", OFFSET(crf), AV_OPT_TYPE_FLOAT, DEFAULT, NULL, 0, 63, V|E},
{"cqp", "constant quantization parameter rate control method", OFFSET(cqp), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, V|E},
#endif
{"keyint_min", "minimum interval between IDR-frames", OFFSET(keyint_min), AV_OPT_TYPE_INT, 25 , NULL, INT_MIN, INT_MAX, V|E},
{"refs", "reference frames to consider for motion compensation", OFFSET(refs), AV_OPT_TYPE_INT, 1 , NULL, INT_MIN, INT_MAX, V|E},
{"chromaoffset", "chroma qp offset from luma", OFFSET(chromaoffset), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
#if FF_API_X264_GLOBAL_OPTS
{"bframebias", "influences how often B-frames are used", OFFSET(bframebias), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|E},
#endif
{"trellis", "rate-distortion optimal quantization", OFFSET(trellis), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX, V|A|E},
#if FF_API_X264_GLOBAL_OPTS
{"directpred", "direct mv prediction mode - 0 (none), 1 (spatial), 2 (temporal), 3 (auto)", OFFSET(directpred), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, V|E},
{"bpyramid", "allows B-frames to be used as references for predicting", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_BPYRAMID , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"wpred", "weighted biprediction for b-frames (H.264)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_WPRED , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"mixed_refs", "one reference per partition, as opposed to one reference per macroblock", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_MIXED_REFS , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"dct8x8", "high profile 8x8 transform (H.264)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_8X8DCT , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"fastpskip", "fast pskip (H.264)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_FASTPSKIP , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"aud", "access unit delimiters (H.264)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_AUD , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
#endif
{"skiprd", "RD optimal MB level residual skipping", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_SKIP_RD , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
#if FF_API_X264_GLOBAL_OPTS
{"complexityblur", "reduce fluctuations in qp (before curve compression)", OFFSET(complexityblur), AV_OPT_TYPE_FLOAT, -1, NULL, -1, FLT_MAX, V|E},
{"deblockalpha", "in-loop deblocking filter alphac0 parameter", OFFSET(deblockalpha), AV_OPT_TYPE_INT, DEFAULT, NULL, -6, 6, V|E},
{"deblockbeta", "in-loop deblocking filter beta parameter", OFFSET(deblockbeta), AV_OPT_TYPE_INT, DEFAULT, NULL, -6, 6, V|E},
{"partitions", "macroblock subpartition sizes to consider", OFFSET(partitions), AV_OPT_TYPE_FLAGS, DEFAULT , NULL, INT_MIN, INT_MAX, V|E, "partitions"},
{"parti4x4", NULL, 0, AV_OPT_TYPE_CONST, X264_PART_I4X4 , NULL, INT_MIN, INT_MAX, V|E, "partitions"},
{"parti8x8", NULL, 0, AV_OPT_TYPE_CONST, X264_PART_I8X8 , NULL, INT_MIN, INT_MAX, V|E, "partitions"},
{"partp4x4", NULL, 0, AV_OPT_TYPE_CONST, X264_PART_P4X4 , NULL, INT_MIN, INT_MAX, V|E, "partitions"},
{"partp8x8", NULL, 0, AV_OPT_TYPE_CONST, X264_PART_P8X8 , NULL, INT_MIN, INT_MAX, V|E, "partitions"},
{"partb8x8", NULL, 0, AV_OPT_TYPE_CONST, X264_PART_B8X8 , NULL, INT_MIN, INT_MAX, V|E, "partitions"},
#endif
{"sc_factor", "multiplied by qscale for each frame and added to scene_change_score", OFFSET(scenechange_factor), AV_OPT_TYPE_INT, 6, NULL, 0, INT_MAX, V|E},
{"mv0_threshold", NULL, OFFSET(mv0_threshold), AV_OPT_TYPE_INT, 256, NULL, 0, INT_MAX, V|E},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"ivlc", "intra vlc table", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_INTRA_VLC , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
#endif
{"b_sensitivity", "adjusts sensitivity of b_frame_strategy 1", OFFSET(b_sensitivity), AV_OPT_TYPE_INT, 40, NULL, 1, INT_MAX, V|E},
{"compression_level", NULL, OFFSET(compression_level), AV_OPT_TYPE_INT, FF_COMPRESSION_DEFAULT , NULL, INT_MIN, INT_MAX, V|A|E},
{"min_prediction_order", NULL, OFFSET(min_prediction_order), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, A|E},
{"max_prediction_order", NULL, OFFSET(max_prediction_order), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, A|E},
#if FF_API_FLAC_GLOBAL_OPTS
{"lpc_coeff_precision", "deprecated, use flac-specific options", OFFSET(lpc_coeff_precision), AV_OPT_TYPE_INT, DEFAULT, NULL, 0, INT_MAX, A|E},
{"prediction_order_method", "deprecated, use flac-specific options", OFFSET(prediction_order_method), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, A|E},
{"min_partition_order", "deprecated, use flac-specific options", OFFSET(min_partition_order), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, A|E},
{"max_partition_order", "deprecated, use flac-specific options", OFFSET(max_partition_order), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, A|E},
#endif
{"timecode_frame_start", "GOP timecode frame start number, in non drop frame format", OFFSET(timecode_frame_start), AV_OPT_TYPE_INT64, 0, NULL, 0, (double)INT64_MAX, V|E},
#if FF_API_MPEGVIDEO_GLOBAL_OPTS
{"drop_frame_timecode", NULL, 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_DROP_FRAME_TIMECODE , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"non_linear_q", "use non linear quantizer", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_NON_LINEAR_QUANT , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
#endif
#if FF_API_REQUEST_CHANNELS
{"request_channels", "set desired number of audio channels", OFFSET(request_channels), AV_OPT_TYPE_INT, DEFAULT, NULL, 0, INT_MAX, A|D},
#endif
#if FF_API_DRC_SCALE
{"drc_scale", "percentage of dynamic range compression to apply", OFFSET(drc_scale), AV_OPT_TYPE_FLOAT, 0.0, NULL, 0.0, 1.0, A|D},
#endif
#if FF_API_LAME_GLOBAL_OPTS
{"reservoir", "use bit reservoir", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_BIT_RESERVOIR , NULL, INT_MIN, INT_MAX, A|E, "flags2"},
#endif
#if FF_API_X264_GLOBAL_OPTS
{"mbtree", "use macroblock tree ratecontrol (x264 only)", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_MBTREE , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
#endif
{"bits_per_raw_sample", NULL, OFFSET(bits_per_raw_sample), AV_OPT_TYPE_INT, DEFAULT , NULL, INT_MIN, INT_MAX},
{"channel_layout", NULL, OFFSET(channel_layout), AV_OPT_TYPE_INT64, DEFAULT, NULL, 0, (double)INT64_MAX, A|E|D, "channel_layout"},
{"request_channel_layout", NULL, OFFSET(request_channel_layout), AV_OPT_TYPE_INT64, DEFAULT, NULL, 0, (double)INT64_MAX, A|D, "request_channel_layout"},
{"rc_max_vbv_use", NULL, OFFSET(rc_max_available_vbv_use), AV_OPT_TYPE_FLOAT, 1.0/3, NULL, 0.0, FLT_MAX, V|E},
{"rc_min_vbv_use", NULL, OFFSET(rc_min_vbv_overflow_use),  AV_OPT_TYPE_FLOAT, 3, NULL,     0.0, FLT_MAX, V|E},
{"ticks_per_frame", NULL, OFFSET(ticks_per_frame), AV_OPT_TYPE_INT, 1, NULL, 1, INT_MAX, A|V|E|D},
{"color_primaries", NULL, OFFSET(color_primaries), AV_OPT_TYPE_INT, AVCOL_PRI_UNSPECIFIED, NULL, 1, AVCOL_PRI_NB-1, V|E|D},
{"color_trc", NULL, OFFSET(color_trc), AV_OPT_TYPE_INT, AVCOL_TRC_UNSPECIFIED, NULL, 1, AVCOL_TRC_NB-1, V|E|D},
{"colorspace", NULL, OFFSET(colorspace), AV_OPT_TYPE_INT, AVCOL_SPC_UNSPECIFIED, NULL, 1, AVCOL_SPC_NB-1, V|E|D},
{"color_range", NULL, OFFSET(color_range), AV_OPT_TYPE_INT, AVCOL_RANGE_UNSPECIFIED, NULL, 0, AVCOL_RANGE_NB-1, V|E|D},
{"chroma_sample_location", NULL, OFFSET(chroma_sample_location), AV_OPT_TYPE_INT, AVCHROMA_LOC_UNSPECIFIED, NULL, 0, AVCHROMA_LOC_NB-1, V|E|D},
#if FF_API_X264_GLOBAL_OPTS
{"psy", "use psycho visual optimization", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_PSY , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"psy_rd", "specify psycho visual strength", OFFSET(psy_rd), AV_OPT_TYPE_FLOAT, -1.0, NULL, -1, FLT_MAX, V|E},
{"psy_trellis", "specify psycho visual trellis", OFFSET(psy_trellis), AV_OPT_TYPE_FLOAT, -1, NULL, -1, FLT_MAX, V|E},
{"aq_mode", "specify aq method", OFFSET(aq_mode), AV_OPT_TYPE_INT, -1, NULL, -1, INT_MAX, V|E},
{"aq_strength", "specify aq strength", OFFSET(aq_strength), AV_OPT_TYPE_FLOAT, -1.0, NULL, -1, FLT_MAX, V|E},
{"rc_lookahead", "specify number of frames to look ahead for frametype", OFFSET(rc_lookahead), AV_OPT_TYPE_INT, -1, NULL, -1, INT_MAX, V|E},
{"ssim", "ssim will be calculated during encoding", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_SSIM , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"intra_refresh", "use periodic insertion of intra blocks instead of keyframes", 0, AV_OPT_TYPE_CONST, CODEC_FLAG2_INTRA_REFRESH , NULL, INT_MIN, INT_MAX, V|E, "flags2"},
{"crf_max", "in crf mode, prevents vbv from lowering quality beyond this point", OFFSET(crf_max), AV_OPT_TYPE_FLOAT, DEFAULT, NULL, 0, 51, V|E},
#endif
{"log_level_offset", "set the log level offset", OFFSET(log_level_offset), AV_OPT_TYPE_INT, 0 , NULL, INT_MIN, INT_MAX },
#if FF_API_FLAC_GLOBAL_OPTS
{"lpc_type", "deprecated, use flac-specific options", OFFSET(lpc_type), AV_OPT_TYPE_INT, AV_LPC_TYPE_DEFAULT, NULL, AV_LPC_TYPE_DEFAULT, AV_LPC_TYPE_NB-1, A|E},
{"none",     NULL, 0, AV_OPT_TYPE_CONST, AV_LPC_TYPE_NONE, NULL,     INT_MIN, INT_MAX, A|E, "lpc_type"},
{"fixed",    NULL, 0, AV_OPT_TYPE_CONST, AV_LPC_TYPE_FIXED, NULL,    INT_MIN, INT_MAX, A|E, "lpc_type"},
{"levinson", NULL, 0, AV_OPT_TYPE_CONST, AV_LPC_TYPE_LEVINSON , NULL, INT_MIN, INT_MAX, A|E, "lpc_type"},
{"cholesky", NULL, 0, AV_OPT_TYPE_CONST, AV_LPC_TYPE_CHOLESKY , NULL, INT_MIN, INT_MAX, A|E, "lpc_type"},
{"lpc_passes", "deprecated, use flac-specific options", OFFSET(lpc_passes), AV_OPT_TYPE_INT, -1 , NULL, INT_MIN, INT_MAX, A|E},
#endif
{"slices", "number of slices, used in parallelized encoding", OFFSET(slices), AV_OPT_TYPE_INT, 0, NULL, 0, INT_MAX, V|E},
{"thread_type", "select multithreading type", OFFSET(thread_type), AV_OPT_TYPE_FLAGS, FF_THREAD_SLICE|FF_THREAD_FRAME, NULL, 0, INT_MAX, V|E|D, "thread_type"},
{"slice", NULL, 0, AV_OPT_TYPE_CONST, FF_THREAD_SLICE , NULL, INT_MIN, INT_MAX, V|E|D, "thread_type"},
{"frame", NULL, 0, AV_OPT_TYPE_CONST, FF_THREAD_FRAME , NULL, INT_MIN, INT_MAX, V|E|D, "thread_type"},
{"audio_service_type", "audio service type", OFFSET(audio_service_type), AV_OPT_TYPE_INT, AV_AUDIO_SERVICE_TYPE_MAIN, NULL, 0, AV_AUDIO_SERVICE_TYPE_NB-1, A|E, "audio_service_type"},
{"ma", "Main Audio Service", 0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_MAIN, NULL,              INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"ef", "Effects",            0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_EFFECTS, NULL,           INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"vi", "Visually Impaired",  0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_VISUALLY_IMPAIRED , NULL, INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"hi", "Hearing Impaired",   0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_HEARING_IMPAIRED, NULL,  INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"di", "Dialogue",           0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_DIALOGUE, NULL,          INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"co", "Commentary",         0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_COMMENTARY, NULL,        INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"em", "Emergency",          0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_EMERGENCY, NULL,         INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"vo", "Voice Over",         0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_VOICE_OVER, NULL,        INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"ka", "Karaoke",            0, AV_OPT_TYPE_CONST, AV_AUDIO_SERVICE_TYPE_KARAOKE, NULL,           INT_MIN, INT_MAX, A|E, "audio_service_type"},
{"request_sample_fmt", "sample format audio decoders should prefer", OFFSET(request_sample_fmt), AV_OPT_TYPE_INT, AV_SAMPLE_FMT_NONE, NULL, AV_SAMPLE_FMT_NONE, AV_SAMPLE_FMT_NB-1, A|D, "request_sample_fmt"},
{"u8" , "8-bit unsigned integer", 0, AV_OPT_TYPE_CONST, AV_SAMPLE_FMT_U8  , NULL, INT_MIN, INT_MAX, A|D, "request_sample_fmt"},
{"s16", "16-bit signed integer",  0, AV_OPT_TYPE_CONST, AV_SAMPLE_FMT_S16 , NULL, INT_MIN, INT_MAX, A|D, "request_sample_fmt"},
{"s32", "32-bit signed integer",  0, AV_OPT_TYPE_CONST, AV_SAMPLE_FMT_S32 , NULL, INT_MIN, INT_MAX, A|D, "request_sample_fmt"},
{"flt", "32-bit float",           0, AV_OPT_TYPE_CONST, AV_SAMPLE_FMT_FLT , NULL, INT_MIN, INT_MAX, A|D, "request_sample_fmt"},
{"dbl", "64-bit double",          0, AV_OPT_TYPE_CONST, AV_SAMPLE_FMT_DBL , NULL, INT_MIN, INT_MAX, A|D, "request_sample_fmt"},
{NULL},
};

#undef A
#undef V
#undef S
#undef E
#undef D
#undef DEFAULT

static const AVClass av_codec_context_class = {
    /*.class_name					= */	"AVCodecContext",
    /*.item_name					= */	context_to_name,
    /*.option						= */	options,
    /*.version						= */	LIBAVUTIL_VERSION_INT,
    /*.log_level_offset_offset		= */	OFFSET(log_level_offset),
	/*.parent_log_context_offset	= */	0,
    /*.child_next					= */	codec_child_next,
    /*.child_class_next				= */	codec_child_class_next,
};

#if FF_API_ALLOC_CONTEXT
void avcodec_get_context_defaults2(AVCodecContext *s, enum AVMediaType codec_type){
    AVCodec c= {0};
    c.type= codec_type;
    avcodec_get_context_defaults3(s, &c);
}
#endif

int avcodec_get_context_defaults3(AVCodecContext *s, AVCodec *codec){
    int flags=0;
    memset(s, 0, sizeof(AVCodecContext));

    s->av_class = &av_codec_context_class;

    s->codec_type = codec ? codec->type : AVMEDIA_TYPE_UNKNOWN;
    if(s->codec_type == AVMEDIA_TYPE_AUDIO)
        flags= AV_OPT_FLAG_AUDIO_PARAM;
    else if(s->codec_type == AVMEDIA_TYPE_VIDEO)
        flags= AV_OPT_FLAG_VIDEO_PARAM;
    else if(s->codec_type == AVMEDIA_TYPE_SUBTITLE)
        flags= AV_OPT_FLAG_SUBTITLE_PARAM;
    av_opt_set_defaults2(s, flags, flags);

    s->time_base           = av_rational(0,1);
    s->get_buffer          = avcodec_default_get_buffer;
    s->release_buffer      = avcodec_default_release_buffer;
    s->get_format          = avcodec_default_get_format;
    s->execute             = avcodec_default_execute;
    s->execute2            = avcodec_default_execute2;
    s->sample_aspect_ratio = av_rational(0,1);
    s->pix_fmt             = PIX_FMT_NONE;
    s->sample_fmt          = AV_SAMPLE_FMT_NONE;
    s->timecode_frame_start = -1;

    s->reget_buffer        = avcodec_default_reget_buffer;
    s->reordered_opaque    = AV_NOPTS_VALUE;
    if(codec && codec->priv_data_size){
        if(!s->priv_data){
            s->priv_data= av_mallocz(codec->priv_data_size);
            if (!s->priv_data) {
                return AVERROR(ENOMEM);
            }
        }
        if(codec->priv_class){
            *(const AVClass**)s->priv_data = codec->priv_class;
            av_opt_set_defaults(s->priv_data);
        }
    }
    if (codec && codec->defaults) {
        int ret;
        const AVCodecDefault *d = codec->defaults;
        while (d->key) {
            ret = av_opt_set(s, (const char *)d->key, (const char *)d->value, 0);
            av_assert0(ret >= 0);
            d++;
        }
    }
    return 0;
}

AVCodecContext *avcodec_alloc_context3(AVCodec *codec){
    AVCodecContext *avctx= (AVCodecContext *)av_malloc(sizeof(AVCodecContext));

    if(avctx==NULL) return NULL;

    if(avcodec_get_context_defaults3(avctx, codec) < 0){
        av_free(avctx);
        return NULL;
    }

    return avctx;
}

#if FF_API_ALLOC_CONTEXT
AVCodecContext *avcodec_alloc_context2(enum AVMediaType codec_type){
    AVCodecContext *avctx= (AVCodecContext *)av_malloc(sizeof(AVCodecContext));

    if(avctx==NULL) return NULL;

    avcodec_get_context_defaults2(avctx, codec_type);

    return avctx;
}

void avcodec_get_context_defaults(AVCodecContext *s){
    avcodec_get_context_defaults2(s, AVMEDIA_TYPE_UNKNOWN);
}

AVCodecContext *avcodec_alloc_context(void){
    return avcodec_alloc_context2(AVMEDIA_TYPE_UNKNOWN);
}
#endif

int avcodec_copy_context(AVCodecContext *dest, const AVCodecContext *src)
{
    if (avcodec_is_open(dest)) { // check that the dest context is uninitialized
        av_log(dest, AV_LOG_ERROR,
               "Tried to copy AVCodecContext %p into already-initialized %p\n",
               src, dest);
        return AVERROR(EINVAL);
    }
    memcpy(dest, src, sizeof(*dest));

    /* set values specific to opened codecs back to their default state */
    dest->priv_data       = NULL;
    dest->codec           = NULL;
    dest->slice_offset    = NULL;
    dest->hwaccel         = NULL;
    dest->thread_opaque   = NULL;
    dest->internal        = NULL;

    /* reallocate values that should be allocated separately */
    dest->rc_eq           = NULL;
    dest->extradata       = NULL;
    dest->intra_matrix    = NULL;
    dest->inter_matrix    = NULL;
    dest->rc_override     = NULL;
    if (src->rc_eq) {
        dest->rc_eq = av_strdup(src->rc_eq);
        if (!dest->rc_eq)
            return AVERROR(ENOMEM);
    }

#define alloc_and_copy_or_fail(cast, obj, size, pad) \
    if (src->obj && size > 0) { \
        dest->obj = (cast)av_malloc(size + pad); \
        if (!dest->obj) \
            goto fail; \
        memcpy(dest->obj, src->obj, size); \
        if (pad) \
            memset(((uint8_t *) dest->obj) + size, 0, pad); \
    }
    alloc_and_copy_or_fail(uint8_t *, extradata,    src->extradata_size,
                           FF_INPUT_BUFFER_PADDING_SIZE);
    alloc_and_copy_or_fail(uint16_t *, intra_matrix, 64 * sizeof(int16_t), 0);
    alloc_and_copy_or_fail(uint16_t *, inter_matrix, 64 * sizeof(int16_t), 0);
    alloc_and_copy_or_fail(RcOverride *, rc_override,  src->rc_override_count * sizeof(*src->rc_override), 0);
#undef alloc_and_copy_or_fail

    return 0;

fail:
    av_freep(&dest->rc_override);
    av_freep(&dest->intra_matrix);
    av_freep(&dest->inter_matrix);
    av_freep(&dest->extradata);
    av_freep(&dest->rc_eq);
    return AVERROR(ENOMEM);
}

const AVClass *avcodec_get_class(void)
{
    return &av_codec_context_class;
}

#define FOFFSET(x) offsetof(AVFrame,x)

static const AVOption frame_options[]={
{"best_effort_timestamp", "", FOFFSET(best_effort_timestamp), AV_OPT_TYPE_INT64, (double)AV_NOPTS_VALUE, NULL, (double)INT64_MIN, (double)INT64_MAX, 0},
{"pkt_pos", "", FOFFSET(pkt_pos), AV_OPT_TYPE_INT64, -1, NULL, (double)INT64_MIN, (double)INT64_MAX, 0},
{"sample_aspect_ratio", "", FOFFSET(sample_aspect_ratio), AV_OPT_TYPE_RATIONAL, 0, NULL, 0, INT_MAX, 0},
{"width", "", FOFFSET(width), AV_OPT_TYPE_INT, 0, NULL, 0, INT_MAX, 0},
{"height", "", FOFFSET(height), AV_OPT_TYPE_INT, 0, NULL, 0, INT_MAX, 0},
{"format", "", FOFFSET(format), AV_OPT_TYPE_INT, -1, NULL, 0, INT_MAX, 0},
{NULL},
};

static const AVClass av_frame_class = {
    /*.class_name              = */	"AVFrame",
    /*.item_name               = */	NULL,
    /*.option                  = */	frame_options,
    /*.version                 = */	LIBAVUTIL_VERSION_INT,
};

const AVClass *avcodec_get_frame_class(void)
{
    return &av_frame_class;
}
