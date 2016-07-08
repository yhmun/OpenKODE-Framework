/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_s3tc.c
 *      Description     The ability to decode S3TC images to the ATX_imgdec extension. 
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

#include "atx_library.h"
#include "S3TC/s3tc_decode.h"

typedef struct  
{
	KDchar			fileCode [ 4 ];
	DDSURFACEDESC2	ddsd;
} S3TCInfo;

typedef struct 
{
	KDbool			uncomp;
	S3TCDecodeFlag	flag;	
	KDint32			offset;
	KDint32			row_count;
	KDuint8*		pixels;
} S3TCDecode;

KDint xmGetInfoS3TC ( KDFile* file, XMImage* image )
{
	S3TCInfo*		info = 0;

	image->info = kdCalloc ( 1, sizeof ( S3TCInfo ) );
	info = (S3TCInfo*) image->info;

	if ( !info )
	{
		return -1;
	}

	if ( kdFread ( info, sizeof ( S3TCInfo ), 1, file ) == 0 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	if ( kdStrncmp ( info->fileCode, "DDS", 3 ) != 0 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	image->type		= KD_IMAGE_TYPE_S3TC;
	image->width	= info->ddsd.width;
	image->height	= info->ddsd.height;
	image->level	= info->ddsd.DUMMYUNIONNAMEN2.mipMapCount;
	image->alpha	= 0;
	image->bpp		= 32;
	image->format	= KD_IMAGE_FORMAT_COMPRESSED_ATX;

	return 0;
}

KDint xmGetDecodeS3TC ( KDFile* file, XMImage* image )
{	
	S3TCInfo*		info   = (S3TCInfo*) image->info;
	S3TCDecode*		decode = 0;
	KDuint			fourcc = info->ddsd.DUMMYUNIONNAMEN4.ddpfPixelFormat.fourCC;
	KDint			format = image->format;

	image->decode = kdCalloc ( 1, sizeof ( S3TCDecode ) );
	decode = (S3TCDecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}

	if (	  fourcc == makeFourCC ( 'D', 'X', 'T', '1' ) )
	{
		image->format = KD_IMAGE_FORMAT_DXT1_ATX;
		decode->flag = DXT1;
	}
	else if ( fourcc == makeFourCC ( 'D', 'X', 'T', '3' ) )
	{
		image->format = KD_IMAGE_FORMAT_DXT3_ATX;
		decode->flag = DXT3;
	}
	else if ( fourcc == makeFourCC ( 'D', 'X', 'T', '5' ) )
	{
		image->format = KD_IMAGE_FORMAT_DXT5_ATX;
		decode->flag = DXT5;
	}

	if ( format != KD_IMAGE_FORMAT_COMPRESSED_ATX )
	{		
		image->format = format;		
		decode->uncomp = 1;
	}

	return 0;
}

KDint xmReadRowsS3TC ( KDFile* file, XMImage* image )
{
	S3TCDecode*		decode = (S3TCDecode*) image->decode;

	kdMemcpy ( image->rows, &decode->pixels[ decode->row_count * image->ptr_tile->stride ], image->ptr_tile->stride );	
	decode->row_count++;

	return 0;
}

KDint xmFreeImageS3TC ( XMImage* image )
{
	S3TCDecode*		decode = (S3TCDecode*) image->decode;

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

KDint xmSaveImageS3TC ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return 0;
}

KDuint8* xmReadTileS3TC ( KDFile* file, XMImage* image )
{
	S3TCDecode*		decode = (S3TCDecode*) image->decode;

	KDuint8*		pixels		= 0;
	KDint			block_size	= 0; 
	KDint32			tile_size   = 0;
	
	if ( decode->pixels )
	{
		kdFree ( decode->pixels );
		decode->pixels = 0;
	}

	block_size = decode->flag == DXT1 ? 8 : 16;

	tile_size = ( ( image->ptr_tile->width + 3 ) / 4 ) * ( ( image->ptr_tile->height + 3 ) / 4 ) * block_size;

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
			s3tc_decode ( pixels, decode->pixels, image->ptr_tile->width, image->ptr_tile->height, decode->flag );
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