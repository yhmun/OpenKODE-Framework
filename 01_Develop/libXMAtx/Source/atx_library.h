/* --------------------------------------------------------------------------
 *
 *      File            atx_library.h
 *      Description     Library-specific definitions and functions for ATX Library
 *      Author          Y.H Mun
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

#ifndef __atx_library_h__
#define __atx_library_h__

#include <kode.h>
#include <stdio.h>

#ifndef SHP
#include <setjmp.h>
#endif

typedef struct	XMBGR           XMBGR;
typedef	struct	XMBGRA          XMBGRA;
typedef struct	XMRGB           XMRGB;
typedef	struct	XMRGBA          XMRGBA;

typedef	struct	XMImage         XMImage;
typedef struct  XMTile          XMTile;
typedef KDvoid*					XMInfo;
typedef KDvoid*					XMDecode;

#pragma pack(1)

struct XMBGR
{
	KDuint8         b;
	KDuint8         g;
	KDuint8         r;
};

struct XMBGRA
{
	KDuint8         b;
	KDuint8         g;
	KDuint8         r;
	KDuint8         a;
};

struct XMRGB
{
	KDuint8         r;
	KDuint8         g;
	KDuint8         b;
};

struct XMRGBA
{
	KDuint8         r;
	KDuint8         g;
	KDuint8         b;
	KDuint8         a;
};


struct XMTile
{
	KDint32         width;			 
	KDint32         height;
	KDint32         offset;
	KDint32         size;	
	KDint32			stride;
};

struct XMImage
{
	KDint32         type;
	KDint32         format;
	KDint32         bpp;
	KDint32         alpha;
	KDint32         level;
	KDint32         width;
	KDint32         height;
	KDint32         num_surf;
	KDint32         bitmask[4];
	KDint32         flip_x;
	KDint32         flip_y;	
	KDint32			premult_alpha;
	KDuint8*        pixels;
	KDuint8*        rows;
	XMTile*         ptr_tile;
	XMTile*         tiles;
	XMInfo*         info;
	XMDecode*       decode;
	XMBGRA*         palette;
};

#pragma pack()

KDint		xmGetInfoPVR		( KDFile* file, XMImage* image );
KDint		xmGetInfoBMP		( KDFile* file, XMImage* image );
KDint		xmGetInfoTGA		( KDFile* file, XMImage* image );
KDint		xmGetInfoPNG		( KDFile* file, XMImage* image );
KDint		xmGetInfoJPEG		( KDFile* file, XMImage* image );
KDint		xmGetInfoS3TC		( KDFile* file, XMImage* image );
KDint		xmGetInfoETC		( KDFile* file, XMImage* image );
KDint		xmGetInfoATITC		( KDFile* file, XMImage* image );
KDint		xmGetInfoTIFF		( KDFile* file, XMImage* image );
KDint		xmGetInfoWEBP		( KDFile* file, XMImage* image );

KDint		xmGetDecodePVR		( KDFile* file, XMImage* image );
KDint		xmGetDecodeBMP		( KDFile* file, XMImage* image );
KDint		xmGetDecodeTGA		( KDFile* file, XMImage* image );
KDint		xmGetDecodePNG		( KDFile* file, XMImage* image );
KDint		xmGetDecodeJPEG		( KDFile* file, XMImage* image );
KDint		xmGetDecodeS3TC		( KDFile* file, XMImage* image );
KDint		xmGetDecodeETC		( KDFile* file, XMImage* image );
KDint		xmGetDecodeATITC	( KDFile* file, XMImage* image );
KDint		xmGetDecodeTIFF		( KDFile* file, XMImage* image );
KDint		xmGetDecodeWEBP		( KDFile* file, XMImage* image );

KDuint8*	xmReadTilePVR		( KDFile* file, XMImage* image );
KDuint8*	xmReadTileS3TC		( KDFile* file, XMImage* image );
KDuint8*	xmReadTileETC		( KDFile* file, XMImage* image );
KDuint8*	xmReadTileATITC		( KDFile* file, XMImage* image );

KDint		xmReadRowsPVR		( KDFile* file, XMImage* image );
KDint		xmReadRowsBMP		( KDFile* file, XMImage* image );
KDint		xmReadRowsTGA		( KDFile* file, XMImage* image );
KDint		xmReadRowsPNG		( KDFile* file, XMImage* image );
KDint		xmReadRowsJPEG		( KDFile* file, XMImage* image );
KDint		xmReadRowsS3TC		( KDFile* file, XMImage* image );
KDint		xmReadRowsETC		( KDFile* file, XMImage* image );
KDint		xmReadRowsATITC		( KDFile* file, XMImage* image );
KDint		xmReadRowsTIFF		( KDFile* file, XMImage* image );
KDint		xmReadRowsWEBP		( KDFile* file, XMImage* image );

KDint		xmFreeImagePVR		( XMImage* image );
KDint		xmFreeImageBMP		( XMImage* image );
KDint		xmFreeImageTGA		( XMImage* image );
KDint		xmFreeImagePNG		( XMImage* image );
KDint		xmFreeImageJPEG		( XMImage* image );
KDint		xmFreeImageS3TC		( XMImage* image );
KDint		xmFreeImageETC		( XMImage* image );
KDint		xmFreeImageATITC	( XMImage* image );
KDint		xmFreeImageTIFF		( XMImage* image );
KDint		xmFreeImageWEBP		( XMImage* image );

KDint		xmSaveImagePVR		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint       xmSaveImageBMP		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint       xmSaveImageTGA		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint       xmSaveImagePNG		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint       xmSaveImageJPEG		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint		xmSaveImageS3TC		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint		xmSaveImageETC		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint		xmSaveImageATITC	( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint       xmSaveImageTIFF		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
KDint       xmSaveImageWEBP		( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );

#endif