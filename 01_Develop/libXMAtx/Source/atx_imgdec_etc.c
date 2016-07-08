/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_etc.c
 *      Description     The ability to decode ETC1 images to the ATX_imgdec extension. 
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
#include "ETC/etc1_decode.h"

typedef struct  
{	
	etc1_byte		header [ ETC_PKM_HEADER_SIZE ];
} ETCInfo;

typedef struct 
{
	KDbool			uncomp;
	KDint32			offset;
	KDint32			row_count;
	KDuint8*		pixels;
} ETCDecode;

KDint xmGetInfoETC ( KDFile* file, XMImage* image )
{
	ETCInfo*		info = 0;

	image->info = kdCalloc ( 1, sizeof ( ETCInfo ) );
	info = (ETCInfo*) image->info;

	if ( !info )
	{
		return -1;
	}

	if ( kdFread ( info, sizeof ( ETCInfo ), 1, file ) == 0 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	if ( !etc1_pkm_is_valid ( info->header ) )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	image->type		= KD_IMAGE_TYPE_ETC;
	image->width	= etc1_pkm_get_width  ( info->header );
	image->height	= etc1_pkm_get_height ( info->header );
	image->level	= 1;
	image->alpha	= 0;
	image->bpp		= 0;
	image->format	= KD_IMAGE_FORMAT_COMPRESSED_ATX;

	return 0;
}

KDint xmGetDecodeETC ( KDFile* file, XMImage* image )
{
	ETCDecode*		decode = 0;
	KDint			format = image->format;

	image->decode = kdCalloc ( 1, sizeof ( ETCDecode ) );
	decode = (ETCDecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}

	if ( format != KD_IMAGE_FORMAT_COMPRESSED_ATX )
	{		
		image->format  = format;		
		decode->uncomp = 1;
	}
	else
	{
		image->format = KD_IMAGE_FORMAT_ETC1_ATX;
	}

	return 0;
}

KDint xmReadRowsETC ( KDFile* file, XMImage* image )
{
	ETCDecode*		decode = (ETCDecode*) image->decode;

	kdMemcpy ( image->rows, &decode->pixels[ decode->row_count * image->ptr_tile->stride ], image->ptr_tile->stride );	
	decode->row_count++;

	return 0;
}

KDint xmFreeImageETC ( XMImage* image )
{
	ETCDecode*		decode = (ETCDecode*) image->decode;

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

KDint xmSaveImageETC ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return 0;
}

KDuint8* xmReadTileETC ( KDFile* file, XMImage* image )
{
	ETCDecode*		decode = (ETCDecode*) image->decode;

	KDuint8*		pixels		= 0;
	KDint			block_size	= 0; 
	KDint32			tile_size   = 0;

	if ( decode->pixels )
	{
		kdFree ( decode->pixels );
		decode->pixels = 0;
	}

	block_size = 8;

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
		tile_size = image->ptr_tile->width * image->ptr_tile->height * 3;
		decode->pixels = (KDuint8*) kdMalloc ( tile_size );

		if ( decode->pixels )
		{
			etc1_decode_image ( pixels, decode->pixels, image->ptr_tile->width, image->ptr_tile->height, 3, image->ptr_tile->width * 3 );
		}

		kdFree ( pixels );
	}
	else
	{
		decode->pixels = pixels;
		image->ptr_tile->size = tile_size;
	}

	decode->row_count = 0;
	image->ptr_tile->stride = tile_size / image->ptr_tile->height;

	return decode->pixels;

cleanup :

	if ( pixels )
	{
		kdFree ( pixels );
	}

	return 0;
}