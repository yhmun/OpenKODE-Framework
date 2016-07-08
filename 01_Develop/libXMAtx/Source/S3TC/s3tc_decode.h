/* --------------------------------------------------------------------------
 *
 *      File            s3tc_decode.h
 *      Author          Young-Hwan Mun
 *                      Jin-Su Park
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

#ifndef __s3tc_decode_h__
#define __s3tc_decode_h__

#include "../atx_library.h"

/*****************************************************************************
 * Useful structs
 *****************************************************************************/

typedef struct 
{
	KDuint32				colorSpaceLowValue;
	KDuint32				colorSpaceHighValue;
} DDColorKey;

typedef struct
{
	KDuint32				caps;
	KDuint32				caps2;
	KDuint32				caps3;
	KDuint32				caps4;
} DDSCaps;

typedef struct
{
	KDuint32				size;
	KDuint32				flags;
	KDuint32				fourCC;
	KDuint32				RGBBitCount;
	KDuint32				RBitMask;
	KDuint32				GBitMask;
	KDuint32				BBitMask;
	KDuint32				ABitMask;
} DDIPixexFormat;

typedef struct
{
	KDuint32				size;
	KDuint32				flags;
	KDuint32				height;
	KDuint32				width;

	union
	{
		KDuint32			pitch;
		KDuint32			linearSize;
	} DUMMYUNIONNAMEN1;

	union
	{
		KDuint32			backBufferCount;
		KDuint32			depth;
	} DUMMYUNIONNAMEN5;

	union
	{
		KDuint32			mipMapCount;
		KDuint32			refreshRate;
		KDuint32			srcVBHandle;
	} DUMMYUNIONNAMEN2;

	KDuint32				alphtBitDepth;
	KDuint32				reserved;
	KDuint32				surface;

	union
	{
		DDColorKey			ddckCKDestOverlay;
		KDuint32			emptyFaceColor;
	} DUMMYUNIONNAMEN3;

	DDColorKey				ddckCKDestBlt;
	DDColorKey				ddckCKSrcOverlay;
	DDColorKey				ddckCKSrcBlt;

	union
	{
		DDIPixexFormat		ddpfPixelFormat;
		KDuint32			FVF;
	} DUMMYUNIONNAMEN4;

	DDSCaps					ddsCaps;
	KDuint32				textureStage;
} DDSURFACEDESC2;

typedef enum 
{
	DXT1 = 1,
	DXT3 = 3,
	DXT5 = 5,
} S3TCDecodeFlag;

KDvoid		s3tc_decode_block ( KDuint8** blockData, KDuint32* decodeBlockData, KDuint32 stride, KDbool oneBitAlphaFlag, KDuint64 alpha, S3TCDecodeFlag decodeFlag );

// Decode S3TC encode data to RGB32
KDvoid		s3tc_decode ( KDuint8* encodeData, KDuint8* decodeData, const KDint pixelsWidth, const KDint pixelsHeight, S3TCDecodeFlag decodeFlag );

KDuint32	makeFourCC ( KDchar ch0, KDchar ch1, KDchar ch2, KDchar ch3 );

#endif	// __s3tc_decode_h__