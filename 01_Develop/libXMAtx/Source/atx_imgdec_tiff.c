/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_tiff.c
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

#include "atx_library.h"
#include "TIFF/tiffio.h"

#pragma pack(1)

typedef struct 
{
	TIFF*		tiff;
} TIFFInfo;

typedef struct 
{
	KDuint32*   pixels;
	KDuint      row_count;
} TIFFDecode;

#pragma pack()

KDint xmGetInfoTIFF ( KDFile* file, XMImage* image )
{
	TIFFInfo*		info = 0;
	
	image->info = kdCalloc ( 1, sizeof ( TIFFInfo ) );
	info = (TIFFInfo*) image->info;
	if ( !info )
	{
		return -1;
	}

	info->tiff = TIFFStreamOpen ( file );
	if ( !info->tiff )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	image->type   = KD_IMAGE_TYPE_TIFF;
	image->bpp    = 32;
	image->alpha  = 8;
	image->format = KD_IMAGE_FORMAT_RGBA_ATX;
	image->flip_y = 0;
	image->bitmask[0] = 0x000000FF;
	image->bitmask[1] = 0x0000FF00;
	image->bitmask[2] = 0x00FF0000;	

    TIFFGetField ( info->tiff, TIFFTAG_IMAGEWIDTH	, &image->width );
    TIFFGetField ( info->tiff, TIFFTAG_IMAGELENGTH	, &image->height );

	return 0;
}

KDint xmGetDecodeTIFF ( KDFile* file, XMImage* image )
{
	TIFFInfo*		info   = (TIFFInfo*) image->info;
	TIFFDecode*		decode = 0;
	
	image->decode = kdCalloc ( 1, sizeof ( TIFFDecode ) );
	decode = (TIFFDecode*) image->decode;

	if ( !decode )
	{
		return -1;
	}

	decode->pixels = (KDuint32*) _TIFFmalloc ( image->width * image->height * sizeof (KDuint32) );
	if ( !decode->pixels )
	{
		return -1;
	}

	if ( !TIFFReadRGBAImageOriented ( info->tiff, image->width, image->height, decode->pixels , ORIENTATION_TOPLEFT, 0 ) )
	{
		return -1;
	}

	return 0;
}

KDint xmReadRowsTIFF ( KDFile* file, XMImage* image )
{	
	TIFFDecode*		decode = (TIFFDecode*) image->decode;
	kdMemcpy ( image->rows, &decode->pixels[ decode->row_count * image->width ], image->width * sizeof (KDuint32) );	
	decode->row_count++;
	return 0;
}

KDint xmFreeImageTIFF ( XMImage* image )
{
	TIFFInfo*		info   = (TIFFInfo*) image->info;
	TIFFDecode*		decode = (TIFFDecode*) image->decode;

	if ( decode && decode->pixels )
	{
		kdFree ( decode->pixels );
	}

	if ( info && info->tiff )
	{
		TIFFClose ( info->tiff );
	}

	return 0;
}

KDint xmSaveImageTIFF ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return -1;
}