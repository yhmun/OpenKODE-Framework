/* --------------------------------------------------------------------------
 *
 *      File            atx_dxtcomp.h
 *      Description     Functions for compressing RGBA image data to a DXT format. 
 *                      It extends the functionality of the image decoder extensions
 *                      KD_ATX_imgdec and KD_ATX_imgdec_pvr 
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

#ifndef __kd_ATX_dxtcomp_h_
#define __kd_ATX_dxtcomp_h_

#include <ATX/atx_imgdec_pvr.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KD_DXTCOMP_TYPE_DXT1_ATX                    144
#define KD_DXTCOMP_TYPE_DXT1A_ATX                   145
#define KD_DXTCOMP_TYPE_DXT3_ATX                    146
#define KD_DXTCOMP_TYPE_DXT5_ATX                    147

// kdDXTCompressImageATX, kdDXTCompressBufferATX : Compresses an image into a DXT format.
KD_API KDImageATX KD_APIENTRY kdDXTCompressImageATX ( KDImageATX image, KDint32 comptype );
KD_API KDImageATX KD_APIENTRY kdDXTCompressBufferATX ( const KDvoid* buffer, KDint32 width, KDint32 height, KDint32 compType, KDint32 levels );

#ifdef __cplusplus
}
#endif

#endif 

