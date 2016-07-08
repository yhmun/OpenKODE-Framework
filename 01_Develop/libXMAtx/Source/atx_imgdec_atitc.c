/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_ATITC.c
 *      Description     The ability to decode ATITC images to the ATX_imgdec extension. 
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

#include "atx_library.h"
#include "ATITC/atitc_decode.h"

typedef struct  
{
	// HEADER
	KDchar			identifier[12];
	KDuint32		endianness;
	KDuint32		glType;
	KDuint32		glTypeSize;
	KDuint32		glFormat;
	KDuint32		glInternalFormat;
	KDuint32		glBaseInternalFormat;
	KDuint32		pixelWidth;
	KDuint32		pixelHeight;
	KDuint32		pixelDepth;
	KDuint32		numberOfArrayElements;
	KDuint32		numberOfFaces;
	KDuint32		numberOfMipmapLevels;
	KDuint32		bytesOfKeyValueData;
} ATITCInfo;

typedef struct 
{
	KDbool			uncomp;
	ATITCDecodeFlag	flag;	
	KDint			block_size;
	KDint32			offset;
	KDint32			row_count;
	KDuint8*		pixels;
} ATITCDecode;

KDint xmGetInfoATITC ( KDFile* file, XMImage* image )
{
	ATITCInfo*		info = 0;

	image->info = kdCalloc ( 1, sizeof ( ATITCInfo ) );
	info = (ATITCInfo*) image->info;

	if ( !info )
	{
		return -1;
	}

	if ( kdFread ( info, sizeof ( ATITCInfo ), 1, file ) == 0 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	if ( kdStrncmp ( &info->identifier [ 1 ], "KTX", 3 ) != 0 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	image->type		= KD_IMAGE_TYPE_ATITC;
	image->width	= info->pixelWidth;
	image->height	= info->pixelHeight;
	image->level	= info->numberOfMipmapLevels;
	image->alpha	= 0;
	image->bpp		= info->pixelDepth;
	image->format	= KD_IMAGE_FORMAT_COMPRESSED_ATX;

	return 0;
}

KDint xmGetDecodeATITC ( KDFile* file, XMImage* image )
{	
	ATITCInfo*		info   = (ATITCInfo*) image->info;
	ATITCDecode*	decode = 0;
	KDint			format = image->format;

	image->decode = kdCalloc ( 1, sizeof ( ATITCDecode ) );
	decode = (ATITCDecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}

	switch ( info->glInternalFormat )
	{
		case GL_ATC_RGB_AMD :
			decode->block_size = 8;	
			decode->flag = ATC_RGB;
			image->format = KD_IMAGE_FORMAT_ATC_RGB;
			break;

		case GL_ATC_RGBA_EXPLICIT_ALPHA_AMD :
			decode->block_size = 16;
			decode->flag = ATC_EXPLICIT_ALPHA;
			image->format = KD_IMAGE_FORMAT_ATC_RGBA_EXPLICIT_ALPHA_AMD;
			break;

		case GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD :	
			decode->block_size = 16;	
			decode->flag = ATC_INTERPOLATED_ALPHA;
			image->format = KD_IMAGE_FORMAT_ATC_RGBA_INTERPOLATED_ALPHA_AMD;
			break;
	}

	kdFseek ( file, info->bytesOfKeyValueData + 4, KD_SEEK_CUR );

	if ( format != KD_IMAGE_FORMAT_COMPRESSED_ATX )
	{		
		image->format  = format;		
		decode->uncomp = 1;
	}

	return 0;
}

KDint xmReadRowsATITC ( KDFile* file, XMImage* image )
{
	ATITCDecode*	decode = (ATITCDecode*) image->decode;

	kdMemcpy ( image->rows, &decode->pixels[ decode->row_count * image->ptr_tile->stride ], image->ptr_tile->stride );	
	decode->row_count++;

	return 0;
}

KDint xmFreeImageATITC ( XMImage* image )
{
	ATITCDecode*	decode = (ATITCDecode*) image->decode;

	if ( decode )
	{
		if ( decode->pixels )
		{
			kdFree ( decode->pixels );
			decode->pixels = 0;
		}
	}

	return 0;
}

KDint xmSaveImageATITC ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return 0;
}

KDuint8* xmReadTileATITC ( KDFile* file, XMImage* image )
{
	ATITCDecode*	decode = (ATITCDecode*) image->decode;

	KDuint8*		pixels		= 0;
	KDint			block_size	= 0; 
	KDint32			tile_size   = 0;

	if ( decode->pixels )
	{
		kdFree ( decode->pixels );
		decode->pixels = 0;
	}

	block_size = decode->block_size;
	tile_size  = ( ( image->ptr_tile->width + 3 ) / 4 ) * ( ( image->ptr_tile->height + 3 ) / 4 ) * block_size;

	pixels = (KDuint8*) kdMalloc ( tile_size );
	if ( !pixels )
	{
		goto cleanup;
	}

	if ( kdFread ( pixels, tile_size, 1, file ) == 0 )
	{			
		//goto cleanup;
	}

	if ( decode->uncomp == 1 )
	{
		tile_size = image->ptr_tile->width * image->ptr_tile->height * 4;
		decode->pixels = (KDuint8*) kdMalloc ( tile_size );

		if ( decode->pixels )
		{
			
			atitc_decode ( pixels, decode->pixels, image->ptr_tile->width, image->ptr_tile->height, decode->flag );
		}

		kdFree ( pixels );
	}
	else
	{
		decode->pixels = pixels;
		image->ptr_tile->size = tile_size;
	}

	decode->row_count = 0;
	image->ptr_tile->stride  = tile_size / image->ptr_tile->height;

	return decode->pixels;

cleanup :

	if ( pixels )
	{
		kdFree ( pixels );
	}

	return 0;
}