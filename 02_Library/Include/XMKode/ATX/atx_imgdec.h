/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec.h
 *      Description     Functions for decoding an image that is stored in a file.  
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

#ifndef __kd_ATX_imgdec_h_
#define __kd_ATX_imgdec_h_

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KD_IMAGE_FORMAT_RGBA8888_ATX                    121
#define KD_IMAGE_FORMAT_RGB888_ATX                      128
#define KD_IMAGE_FORMAT_RGB565_ATX                      129
#define KD_IMAGE_FORMAT_RGBA5551_ATX                    130
#define KD_IMAGE_FORMAT_RGBA4444_ATX                    131
#define KD_IMAGE_FORMAT_BGRA8888_ATX                    132
#define KD_IMAGE_FORMAT_ALPHA8_ATX                      133
#define KD_IMAGE_FORMAT_LUM8_ATX                        134
#define KD_IMAGE_FORMAT_LUMALPHA88_ATX                  135
#define KD_IMAGE_FORMAT_COMPRESSED_ATX                  122
#define KD_IMAGE_FLAG_FLIP_X_ATX                        1
#define KD_IMAGE_FLAG_FLIP_Y_ATX                        2
#define KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA               4

#define KD_IMAGE_POINTER_BUFFER_ATX                     112

#define KD_IMAGE_WIDTH_ATX                              113
#define KD_IMAGE_HEIGHT_ATX                             114
#define KD_IMAGE_FORMAT_ATX                             115
#define KD_IMAGE_FORMAT_RGB_ATX                         136
#define KD_IMAGE_FORMAT_RGBA_ATX                        137
#define KD_IMAGE_FORMAT_PALETTED_ATX                    138
#define KD_IMAGE_FORMAT_LUMINANCE_ATX                   139
#define KD_IMAGE_FORMAT_LUMALPHA_ATX                    140
#define KD_IMAGE_STRIDE_ATX                             116
#define KD_IMAGE_BITSPERPIXEL_ATX                       117
#define KD_IMAGE_LEVELS_ATX                             118
#define KD_IMAGE_DATASIZE_ATX                           119
#define KD_IMAGE_BUFFEROFFSET_ATX                       120
#define KD_IMAGE_ALPHA_ATX                              141

//****************************************************************************
// XMSoft's Added.
//****************************************************************************

#define KD_IMAGE_TYPE									142

#define	KD_IMAGE_TYPE_PNG								0
#define	KD_IMAGE_TYPE_PVR								1
#define	KD_IMAGE_TYPE_S3TC								2
#define	KD_IMAGE_TYPE_ETC								3
#define KD_IMAGE_TYPE_ATITC								4
#define	KD_IMAGE_TYPE_TIFF								5
#define	KD_IMAGE_TYPE_WEBP								6
#define	KD_IMAGE_TYPE_BMP								7
#define	KD_IMAGE_TYPE_TGA								8
#define	KD_IMAGE_TYPE_JPEG								9

#define	KD_IMAGE_TYPE_NULL								-1

#define KD_MAKE_RGB565(r, g, b)							(KDuint16) ( ( (r) & 0xF8 ) << 8 | ( ( (g) & 0xFC ) << 3 ) | ( ( (b) & 0xF8 ) >> 3 ) )
#define KD_MAKE_RGBA5551(r, g, b, a)					(KDuint16) ( ( (r) & 0xF8 ) << 8 | ( ( (g) & 0xF8 ) << 3 ) | ( ( (b) & 0xF8 ) >> 2 ) | ( ( (a) & 0x80 ) >> 7 ) )
#define KD_MAKE_RGBA4444(r, g, b, a)					(KDuint16) ( ( (r) & 0xF0 ) << 8 | ( ( (g) & 0xF0 ) << 4 ) | ( ( (b) & 0xF0 ) >> 0 ) | ( ( (a) & 0xF0 ) >> 4 ) )


typedef KDvoid* KDImageATX;

// kdGetImageInfoATX, kdGetImageInfoFromStreamATX : Construct an informational image object based on an image in a file or stream.
KD_API KDImageATX KD_APIENTRY kdGetImageInfoATX ( const KDchar* pathname );
KD_API KDImageATX KD_APIENTRY kdGetImageInfoFromStreamATX ( KDFile* file );

// kdGetImageATX, kdGetImageFromStreamATX : Read and decode an image from a file or stream, returning a decoded image object.
KD_API KDImageATX KD_APIENTRY kdGetImageATX ( const KDchar* pathname, KDint format, KDint flags );
KD_API KDImageATX KD_APIENTRY kdGetImageFromStreamATX ( KDFile* file, KDint format, KDint flags );

// kdFreeImageATX : Free image object. 
KD_API KDvoid KD_APIENTRY kdFreeImageATX ( KDImageATX image );

// kdGetImagePointerATX : Get the value of an image pointer attribute.
KD_API KDvoid* KD_APIENTRY kdGetImagePointerATX ( KDImageATX image, KDint attr );

// kdGetImageIntATX, kdGetImageLevelIntATX : Get the value of an image integer attribute.
KD_API KDint KD_APIENTRY kdGetImageIntATX ( KDImageATX image, KDint attr );
KD_API KDint KD_APIENTRY kdGetImageLevelIntATX ( KDImageATX image, KDint attr, KDint level );

KD_API KDint KD_APIENTRY kdSaveImageATX ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );

#ifdef __cplusplus
}
#endif

#endif 

