/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_tga.c
 *      Description     The ability to decode Targa images to the ATX_imgdec extension. 
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

#define XM_TGA_NULL                     0

#define XM_TGA_INDEX_COLOR              1
#define XM_TGA_TRUE_COLOR               2
#define XM_TGA_MONO_COLOR               3

#define XM_TGA_RLE_INDEX_COLOR          9
#define XM_TGA_RLE_TRUE_COLOR           10
#define XM_TGA_RLE_MONO_COLOR           11

#pragma pack(1)

typedef struct 
{
	KDuint8        id_length;                   // Length of the image ID field
	KDuint8        color_map_type;              // Whether a color map is included
	KDuint8        image_type;                  // Compression and color types

	// Color map specification                  // Describes the color map			
	KDuint16        first_entry_index;          // offset into the color map table 
	KDuint16        color_map_length;           // number of entries 
	KDuint8         color_map_entry_size;       // number of bits per pixel 
	
	//	Image specification                     // Image dimensions and format
	KDuint16        origin_x;                   // absolute coordinate of lower-left corner for displays where origin is at the lower left 
	KDuint16        origin_y;                   // as for X-origin 
	KDuint16        image_width;                // width in pixels 
	KDuint16        image_height;               // height in pixels 
	KDuint8         pixel_depth;                // bits per pixel 
	KDuint8         image_descriptor;           // bits 3-0 give the alpha channel depth, bits 5-4 give direction 
} TGAInfo;

typedef struct 
{
	KDuint8         rle;
	KDuint8         flag;
	KDuint8         skip;
	KDuint8         aux[4];
} TGADecode;

#pragma pack()

static KDvoid xmTGAInfoToLittleEndian ( TGAInfo* info )
{
	if ( kdIsLittleEndian ( ) == KD_FALSE )
	{
 		info->first_entry_index	= kdEndianSwap16 ( info->first_entry_index );
 		info->color_map_length  = kdEndianSwap16 ( info->color_map_length  );
 		info->origin_x          = kdEndianSwap16 ( info->origin_x          );
 		info->origin_y          = kdEndianSwap16 ( info->origin_y          );
 		info->image_width       = kdEndianSwap16 ( info->image_width       );
 		info->image_height      = kdEndianSwap16 ( info->image_height      );
	}
}

KDint xmGetInfoTGA ( KDFile* file, XMImage* image )
{
	TGAInfo*		info = 0;

	image->info = kdCalloc ( 1, sizeof ( TGAInfo ) );
	info = (TGAInfo*) image->info;
	if ( !info )
	{
		return -1;
	}

	if ( kdFread ( info, sizeof ( TGAInfo ), 1, file ) == 0 )
	{			
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	xmTGAInfoToLittleEndian ( info );

	if ( info->color_map_length > 256 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	switch ( info->image_type )
	{
		case XM_TGA_INDEX_COLOR	:
		case XM_TGA_TRUE_COLOR  :			
		case XM_TGA_MONO_COLOR  :

		case XM_TGA_RLE_INDEX_COLOR	:
		case XM_TGA_RLE_TRUE_COLOR  :
		case XM_TGA_RLE_MONO_COLOR  :		
			break;

		default :
			kdSetError ( KD_EILSEQ );
			return -1;
	}

	switch ( info->pixel_depth )
	{
		case 32 :
			image->format = KD_IMAGE_FORMAT_RGBA_ATX;
			image->alpha  = 8;
			break;

		case 24 :
			image->format = KD_IMAGE_FORMAT_RGB_ATX;
			image->alpha  = 0;
			break;	

		case 16 :
			image->format = KD_IMAGE_FORMAT_RGB_ATX;
			image->alpha  = 0;			

			image->bitmask[0] = 0x7C00;
			image->bitmask[1] = 0x03E0;
			image->bitmask[2] = 0x001F; 
			image->bitmask[3] = 0;
			break;

		case 8 :			
			image->format = info->image_type == XM_TGA_MONO_COLOR || 
							info->image_type == XM_TGA_RLE_MONO_COLOR ? 
								KD_IMAGE_FORMAT_LUMINANCE_ATX : KD_IMAGE_FORMAT_PALETTED_ATX; 
			image->alpha  = 0;
			break;

		default :
			kdSetError ( KD_EILSEQ );
			return -1;
	}

	image->type	  = KD_IMAGE_TYPE_TGA;
	image->flip_x = info->image_descriptor & 16 ? 1 : 0;
	image->flip_y = info->image_descriptor & 32 ? 1 : 0;
	image->width  = info->image_width;
	image->height = info->image_height;
	image->bpp    = info->pixel_depth;
	
	return 0;
}

KDint xmGetDecodeTGA ( KDFile* file, XMImage* image )
{
	TGAInfo*	info = (TGAInfo*) image->info;
	TGADecode*	decode = 0;
	XMBGR*      palette = 0;
	KDint       i;

	image->flip_y = !image->flip_y;

	if ( info->image_type == XM_TGA_TRUE_COLOR || info->image_type == XM_TGA_MONO_COLOR )
	{		
		return 0;
	}

	image->decode = kdCalloc ( 1, sizeof ( TGADecode ) );
	decode = (TGADecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}

	if ( info->id_length > 0 )
	{
	 	kdFseek ( file, (KDoff) info->id_length, KD_SEEK_CUR );	
	}

	if ( info->color_map_type != 0 )
	{		
		palette = (XMBGR *) kdMalloc ( info->color_map_length * sizeof ( XMBGR ) );
		if ( !palette )
		{
			return -1;
		}

		if ( kdFread ( palette, info->color_map_length * sizeof ( XMBGR ), 1, file ) == 0 )
		{				
			return -1;
		}	

		for ( i = 0; i < info->color_map_length; i++ )
		{
			image->palette[ i ].r = palette[ i ].r;
			image->palette[ i ].g = palette[ i ].g;
			image->palette[ i ].b = palette[ i ].b;
			image->palette[ i ].a = 255;				
		}

		kdFree ( palette );
	}

	return 0;
}

KDint xmReadRowsTGA ( KDFile* file, XMImage* image )
{	
	TGAInfo*	info = (TGAInfo*) image->info;
	TGADecode*	decode = (TGADecode*) image->decode;
	KDint32	    image_bytes = image->bpp >> 3;
	KDint32	    offset      = 0;

	switch ( info->image_type )
	{
		case XM_TGA_RLE_INDEX_COLOR :
		case XM_TGA_RLE_TRUE_COLOR  :
		case XM_TGA_RLE_MONO_COLOR  :

			while ( offset < image->width )
			{
				if ( decode->rle != 0 )
				{
					decode->rle--;
					decode->skip = ( decode->flag != 0 );
				}
				else
				{
					if ( kdFread ( &decode->rle, 1, 1, file ) == 0 )
					{
						return -1;
					}

					decode->flag = decode->rle & 0x80;
					if ( decode->flag )
					{
						decode->rle -= 128;
					}

					decode->skip = 0;
				}

				if ( !decode->skip )
				{
					if ( kdFread ( decode->aux, image_bytes, 1, file ) == 0 )
					{						
						return -1;
					}
				}

				kdMemcpy ( &image->rows[ offset * image_bytes ], decode->aux, image_bytes );
				offset++;
			}

			break;

		case XM_TGA_INDEX_COLOR :
		case XM_TGA_TRUE_COLOR  :
		case XM_TGA_MONO_COLOR  :

			if ( kdFread ( image->rows, image->width * image_bytes, 1, file ) == 0 )
			{			
				return -1;
			}	
	}

	return 0;
}

KDint xmFreeImageTGA ( XMImage* image )
{
	return 0;
}

KDint xmSaveImageTGA ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return -1;
}
