/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_webp.c
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
#include "WEBP/webp/decode.h"

#pragma pack(1)

typedef struct 
{
	WebPDecoderConfig		config;
	KDoff					offset;
	KDvoid*					data;
	KDsize					size;
} WEBPInfo;

typedef struct 
{
	KDuint32*				pixels;
	KDuint					row_count;
} WEBPDecode;

#pragma pack()

KDint xmGetInfoWEBP ( KDFile* file, XMImage* image )
{
	WEBPInfo*		info = 0;
	
	image->info = kdCalloc ( 1, sizeof ( WEBPInfo ) );
	info = (WEBPInfo*) image->info;

	if ( !info )
	{
		return -1;
	}

	if ( WebPInitDecoderConfig ( &info->config ) == 0 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	image->type	= KD_IMAGE_TYPE_WEBP;
	info->size = sizeof ( WebPBitstreamFeatures );
	info->data = kdMalloc ( info->size );
	info->offset = kdFtell ( file );

	if ( !kdFread ( info->data, info->size, 1, file ) )
	{
        kdFree ( info->data );
        kdSetError ( KD_EILSEQ );
		return -1;
	}

	if ( WebPGetFeatures ( info->data, info->size, &info->config.input ) != VP8_STATUS_OK )
	{
        kdFree ( info->data );
        kdSetError ( KD_EILSEQ );
		return -1;
	}

	image->width  = info->config.input.width;
	image->height = info->config.input.height;

	image->bpp    = 32;
	image->alpha  = 8;
	image->format = KD_IMAGE_FORMAT_RGBA_ATX;
	image->flip_y = 0;

	image->bitmask[0] = 0x000000FF;
	image->bitmask[1] = 0x0000FF00;
	image->bitmask[2] = 0x00FF0000;
	image->bitmask[3] = 0xFF000000;

	return 0;
}

KDint xmGetDecodeWEBP ( KDFile* file, XMImage* image )
{
	WEBPInfo*		info = (WEBPInfo*) image->info;
	WEBPDecode*		decode;

	image->decode = kdCalloc ( 1, sizeof ( WEBPDecode ) );
	decode = (WEBPDecode*) image->decode;

	if ( !decode )
	{
		return -1;
	}

	kdFseek ( file, 0, KD_SEEK_END );
	info->size = kdFtell ( file );
	info->data = kdRealloc ( info->data, info->size );

	kdFseek ( file, info->offset, KD_SEEK_SET );
	if ( !kdFread ( info->data, info->size, 1, file ) )
	{
		return -1;
	}
	
	decode->pixels = (KDuint32*) kdMalloc ( image->width * image->height * 4 );

	info->config.output.colorspace	 = MODE_RGBA;
	info->config.output.u.RGBA.rgba   = (uint8_t*) decode->pixels;
	info->config.output.u.RGBA.stride = image->width * 4;
	info->config.output.u.RGBA.size   = image->width * image->height * 4;
	info->config.output.is_external_memory = 1;

    if ( WebPDecode ( (uint8_t*) info->data, info->size, &info->config ) != VP8_STATUS_OK )
    {
        return -1;
    }

	return 0;
}

KDint xmReadRowsWEBP ( KDFile* file, XMImage* image )
{
	WEBPDecode*		decode = (WEBPDecode*) image->decode;

	kdMemcpy ( image->rows, &decode->pixels [ decode->row_count * image->width ], image->width * sizeof (KDuint32) );	
	decode->row_count++;

	return 0;
}

KDint xmFreeImageWEBP ( XMImage* image )
{
	WEBPInfo*		info   = (WEBPInfo*) image->info;
	WEBPDecode*		decode = (WEBPDecode*) image->decode;

	if ( decode && decode->pixels )
	{
		kdFree ( decode->pixels );
	}

	if ( info && info->data )
	{
		kdFree ( info->data );
	}

	return 0;
}

KDint xmSaveImageWEBP ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return -1;
}
