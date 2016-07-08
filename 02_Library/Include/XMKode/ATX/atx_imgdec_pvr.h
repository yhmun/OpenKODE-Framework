/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_pvr.h
 *      Description     The ATX_imgdec extension the ability to decode Imagination
 *                      Technologies PVR files containing PVRTC and DXT compressed 
 *                      images, as well as a subset of the uncompressed formats 
 *                      supported by PVR files. 
 *      Version         5, 2009-02-04
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#ifndef __kd_ATX_imgdec_pvr_h_
#define __kd_ATX_imgdec_pvr_h_

#include <ATX/atx_imgdec.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KD_IMAGE_FORMAT_DXT1_ATX							123
#define KD_IMAGE_FORMAT_DXT3_ATX							124
#define KD_IMAGE_FORMAT_DXT5_ATX							125
#define KD_IMAGE_FORMAT_PVRTC2_ATX							126
#define KD_IMAGE_FORMAT_PVRTC4_ATX							127

#define KD_IMAGE_FORMAT_ETC1_ATX							128
#define KD_IMAGE_FORMAT_ATC_RGB								129
#define KD_IMAGE_FORMAT_ATC_RGBA_EXPLICIT_ALPHA_AMD			130
#define KD_IMAGE_FORMAT_ATC_RGBA_INTERPOLATED_ALPHA_AMD		131

#define	KD_IMAGE_SURFACES_ATX								201

#ifdef __cplusplus
}
#endif

#endif

