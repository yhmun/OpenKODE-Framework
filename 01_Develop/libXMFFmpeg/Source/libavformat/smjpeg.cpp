/*
 * SMJPEG common code
 * Copyright (c) 2011-2012 Paul B Mahol
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
 * SMJPEG common code
 */

#include "internal.h"
#include "smjpeg.h"

extern const AVCodecTag ff_codec_smjpeg_video_tags[] = {
    { CODEC_ID_MJPEG,             MKTAG('J', 'F', 'I', 'F') },
    { CODEC_ID_NONE, 0 },
};

extern const AVCodecTag ff_codec_smjpeg_audio_tags[] = {
    { CODEC_ID_ADPCM_IMA_SMJPEG,  MKTAG('A', 'P', 'C', 'M') },
    { CODEC_ID_PCM_S16LE,         MKTAG('N', 'O', 'N', 'E') },
    { CODEC_ID_NONE, 0 },
};
