/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_pvr.c
 *      Description     The ability to decode PVR images to the ATX_imgdec extension. 
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
#include "PVR/PVRTDecompress.h"

//***************************************************************************
// Describes the header of a PVR header-texture
//***************************************************************************

typedef struct 
{
	KDuint						version;
	KDuint						compress;

	union
	{
		PVR_Texture_Header		pvr2;
		PVRTextureHeaderV3		pvr3;
	};	
} PVRInfo;

typedef struct 
{
	KDbool						uncomp;
	KDuint8*					pixels;
	KDint32						offset;
	KDint32						row_count;
} PVRDecode;


KDint xmGetInfoPVR ( KDFile* file, XMImage* image )
{
	PVRInfo*	info;
	KDuint		block;
	KDuint		size;

	image->info = kdCalloc ( 1, sizeof ( PVRInfo ) );
	info = (PVRInfo*) image->info;
	if ( !info )
	{
		return -1;
	}

	if ( kdFread ( &block, sizeof ( KDuint32 ), 1, file ) == 0 )
	{			
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	size = KD_SWAP_INT32_LITTLE_TO_HOST ( block );

	if ( size == PVRTEX_V1_HEADER_SIZE || size == PVRTEX_V2_HEADER_SIZE )
	{
		info->version = 2;
		info->pvr2.uHeaderSize = size; 
		if ( kdFread ( &info->pvr2.uHeight, PVRTEX_V2_HEADER_SIZE - 4, 1, file ) == 0 )
		{
			kdSetError ( KD_EILSEQ );
			return -1;			
		}		

		info->pvr2.uPVR = KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uPVR );
		if ( PVRTexIdentifier [ 0 ] != (KDchar) ( ( ( info->pvr2.uPVR >>  0 ) & 0xff ) ) ||
			 PVRTexIdentifier [ 1 ] != (KDchar) ( ( ( info->pvr2.uPVR >>  8 ) & 0xff ) ) ||
			 PVRTexIdentifier [ 2 ] != (KDchar) ( ( ( info->pvr2.uPVR >> 16 ) & 0xff ) ) ||
			 PVRTexIdentifier [ 3 ] != (KDchar) ( ( ( info->pvr2.uPVR >> 24 ) & 0xff ) ) )
		{
			kdSetError ( KD_EILSEQ );
			return -1;		
		}

		info->pvr2.uHeight			= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uHeight			);		
		info->pvr2.uWidth			= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uWidth			);	
		info->pvr2.uMipMapCount		= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uMipMapCount	);	
		info->pvr2.upfFlags			= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.upfFlags		);	
		info->pvr2.uTextureDataSize = KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uTextureDataSize);	
		info->pvr2.uBitCount 		= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uBitCount		);	
		info->pvr2.uRBitMask		= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uRBitMask		);	
		info->pvr2.uGBitMask 		= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uGBitMask		);	
		info->pvr2.uBBitMask		= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uBBitMask		);	
		info->pvr2.uAlphaBitMask 	= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uAlphaBitMask	);				
		info->pvr2.uNumSurfs 		= KD_SWAP_INT32_LITTLE_TO_HOST ( info->pvr2.uNumSurfs		);

		image->type			= KD_IMAGE_TYPE_PVR;
		image->bpp			= info->pvr2.uBitCount;
		image->width		= info->pvr2.uWidth;
		image->height		= info->pvr2.uHeight;
		image->num_surf		= info->pvr2.uNumSurfs;
		image->bitmask[0]	= info->pvr2.uRBitMask;
		image->bitmask[1]	= info->pvr2.uGBitMask;
		image->bitmask[2]	= info->pvr2.uBBitMask;
		image->bitmask[3]	= info->pvr2.uAlphaBitMask;

		image->level		= info->pvr2.upfFlags & PVRTEX_MIPMAP ? info->pvr2.uMipMapCount : 0;
		image->flip_y		= info->pvr2.upfFlags & PVRTEX_VERTICAL_FLIP ? 1 : 0;

		switch ( info->pvr2.upfFlags & PVRTEX_PIXELTYPE )
		{
			case OGL_BGRA_8888 :
			case OGL_RGBA_8888 :
				image->format = KD_IMAGE_FORMAT_RGBA_ATX;
				image->alpha  = 8;
				break;

			case OGL_RGBA_4444 :
				image->format = KD_IMAGE_FORMAT_RGBA_ATX;
				image->alpha  = 4;
				break;

			case OGL_RGBA_5551 : 
				image->format = KD_IMAGE_FORMAT_RGBA_ATX;
				image->alpha  = 1;
				break;

			case OGL_RGB_888 :
				image->bitmask[0] = info->pvr2.uBBitMask;
				image->bitmask[2] = info->pvr2.uRBitMask;
			case OGL_RGB_565 :		
				image->format = KD_IMAGE_FORMAT_RGB_ATX;
				image->alpha  = 0;
				break;

			case OGL_I_8 :
				image->format = KD_IMAGE_FORMAT_LUMINANCE_ATX;
				image->alpha  = 0;
				break;

			case OGL_AI_88 :
				image->format = KD_IMAGE_FORMAT_LUMALPHA_ATX;
				image->alpha  = 8;
				break;

			case OGL_A_8 :
				image->format = KD_IMAGE_FORMAT_ALPHA8_ATX;
				image->alpha  = 8;
				break;

			case OGL_PVRTC2 :
			case MGLPT_PVRTC2 :		
				image->format = KD_IMAGE_FORMAT_COMPRESSED_ATX;
				image->alpha  = info->pvr2.uAlphaBitMask;
				info->compress = 2;
				break;

			case OGL_PVRTC4 :
			case MGLPT_PVRTC4 :	
				image->format = KD_IMAGE_FORMAT_COMPRESSED_ATX;
				image->alpha  = info->pvr2.uAlphaBitMask;	
				info->compress = 4;
				break;

			default :											
				kdSetError ( KD_ENOSPC );
				return -1;
		}
	}
	else if ( KD_SWAP_INT32_BIG_TO_HOST ( block ) == PVRTEX3_IDENT_REV )
	{		
		info->version = 3;
		info->pvr3.u32Version = PVRTEX3_IDENT_REV;
		if ( kdFread ( &info->pvr3.u32Flags, PVRTEX_V3_HEADER_SIZE - 4, 1, file ) == 0 )
		{
			kdSetError ( KD_EILSEQ );
			return -1;				
		}	

		kdFseek ( file, info->pvr3.u32MetaDataSize, KD_SEEK_CUR );

		image->type			= KD_IMAGE_TYPE_PVR;
		image->width		= info->pvr3.u32Width;
		image->height		= info->pvr3.u32Height;
		image->num_surf		= info->pvr3.u32NumSurfaces;		
		image->level		= info->pvr3.u32MIPMapCount;	
		
		image->bitmask[0]	= 0x000000FF;
		image->bitmask[1]	= 0x0000FF00;
		image->bitmask[2]	= 0x00FF0000;
		image->bitmask[3]	= 0xFF000000;
		
		image->premult_alpha = info->pvr3.u32Flags & PVRTEX3_PREMULTIPLIED ? 1 : 0;

		switch ( info->pvr3.u64PixelFormat )
		{
			case ePVRTPF_PVRTCI_2bpp_RGB :
				image->alpha			= 2;
			case ePVRTPF_PVRTCI_2bpp_RGBA :		
				image->format			= KD_IMAGE_FORMAT_COMPRESSED_ATX;
				info->compress	= 2;
				image->bpp				= 2;
				break;

			case ePVRTPF_PVRTCI_4bpp_RGB :
				image->alpha			= 4;
			case ePVRTPF_PVRTCI_4bpp_RGBA :
				image->format			= KD_IMAGE_FORMAT_COMPRESSED_ATX;
				info->compress	= 4;
				image->bpp				= 4;
				break;

			case kPVR3TexturePixelFormat_BGRA_8888 :
				KD_SWAP ( image->bitmask[0], image->bitmask[2], KDint32 );
			case kPVR3TexturePixelFormat_RGBA_8888 :	
				image->format		= KD_IMAGE_FORMAT_RGBA_ATX;
				image->alpha		= 8;
				image->bpp			= 32;
				break;

			case kPVR3TexturePixelFormat_RGBA_4444 :
				image->format		= KD_IMAGE_FORMAT_RGBA_ATX;
				image->alpha		= 4;
				image->bpp			= 16;
				image->bitmask[0]	= 0x0000F000;
				image->bitmask[1]	= 0x00000F00;
				image->bitmask[2]	= 0x000000F0;
				image->bitmask[3]	= 0x0000000F;
				break;

			case kPVR3TexturePixelFormat_RGBA_5551 :
				image->format		= KD_IMAGE_FORMAT_RGBA_ATX;
				image->alpha		= 1;
				image->bpp			= 16;
				image->bitmask[0]	= 0x0000F800;
				image->bitmask[1]	= 0x000007C0;
				image->bitmask[2]	= 0x0000003E;
				image->bitmask[3]	= 0x00000001;
				break;

			case kPVR3TexturePixelFormat_RGB_565 :
				image->format		= KD_IMAGE_FORMAT_RGB_ATX;
				image->alpha		= 0;
				image->bpp			= 16;
				image->bitmask[0]	= 0x0000F800;
				image->bitmask[1]	= 0x000007E0;
				image->bitmask[2]	= 0x0000001F;
				image->bitmask[3]	= 0x00000000;
				break;

			case kPVR3TexturePixelFormat_RGB_888 :				
				image->format		= KD_IMAGE_FORMAT_RGB_ATX;
				image->alpha		= 0;
				image->bpp			= 24;
				break;

			case kPVR3TexturePixelFormat_A_8 :
				image->format		= KD_IMAGE_FORMAT_ALPHA8_ATX;
				image->alpha		= 8;
				image->bpp			= 8;
				break;

			case kPVR3TexturePixelFormat_L_8 :
				image->format		= KD_IMAGE_FORMAT_LUMINANCE_ATX;
				image->alpha		= 0;
				image->bpp			= 8;
				break;

			case kPVR3TexturePixelFormat_LA_88 :	
				image->format		= KD_IMAGE_FORMAT_LUMALPHA_ATX;
				image->alpha		= 8;
				image->bpp			= 16;
				break;

			default :											
				kdSetError ( KD_ENOSPC );
				return -1;
		}	
	}
	else
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}
	
	if ( image->level > 1 )
	{
		image->level++;
	}

	return 0;
}

KDint xmGetDecodePVR ( KDFile* file, XMImage* image )
{
	PVRInfo*	info = (PVRInfo*) image->info;
	PVRDecode*	decode = 0;

	image->decode = kdCalloc ( 1, sizeof ( PVRDecode ) );
	decode = (PVRDecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}

	switch ( info->compress )
	{
		case 2 :
			if ( image->format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
			{
				image->format = KD_IMAGE_FORMAT_PVRTC2_ATX;				
			}
			else
			{
				decode->uncomp = 1;
			}
			break;

		case 4 :
			if ( image->format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
			{
				image->format = KD_IMAGE_FORMAT_PVRTC4_ATX;				
			}
			else
			{
				decode->uncomp = 1;
			}
			break;

		default :
			image->flip_y = 0;
	}

	return 0;
}

KDint xmReadRowsPVR ( KDFile* file, XMImage* image )
{
	PVRDecode*		decode = (PVRDecode*) image->decode;

	kdMemcpy ( image->rows, &decode->pixels[ decode->row_count * image->ptr_tile->stride ], image->ptr_tile->stride );	
	decode->row_count++;

	return 0;
}

KDuint8* xmReadTilePVR ( KDFile* file, XMImage* image )
{
	PVRInfo*		info		 = (PVRInfo*) image->info;
	PVRDecode*		decode		 = (PVRDecode*) image->decode;
	KDuint8*		pixels		 = 0;
	KDint32			Do2bitMode	 = 0;
	KDint32			block_size	 = 0;
	KDint32			block_width  = 0;
	KDint32			block_height = 0;
	KDint32			block_bpp	 = 0;
	KDint32			tile_size    = 0;

	if ( decode->pixels )
	{
		kdFree ( decode->pixels );
		decode->pixels = 0;
	}

	switch ( info->compress )
	{
		case 2 :
			Do2bitMode   = 1;
			block_size   = 8 * 4;
			block_width  = image->ptr_tile->width  / 8;
			block_height = image->ptr_tile->height / 4;
			block_bpp	 = 2;
			break;

		case 4 :
			Do2bitMode   = 0;
			block_size   = 4 * 4;
			block_width  = image->ptr_tile->width  / 4;
			block_height = image->ptr_tile->height / 4;
			block_bpp	 = 4;
			break;

		default :
			block_size	 = 1;
			block_width	 = image->ptr_tile->width;
			block_height = image->ptr_tile->height;
			block_bpp	 = image->bpp;
			break;
	}

	if ( block_width  < 2 )	block_width  = 2;
	if ( block_height < 2 )	block_height = 2;

	tile_size = block_width * block_height * ( ( block_size * block_bpp ) / 8 );
	
	pixels = (KDuint8*) kdMalloc ( tile_size );
	if ( !pixels )
	{
		goto cleanup;
	}

	if ( kdFread ( pixels, tile_size, 1, file ) == 0 )
	{			
		//goto cleanup;
	}

	decode->offset = 0;

	if ( decode->uncomp )
	{
		tile_size = image->ptr_tile->width * image->ptr_tile->height * 4;
		decode->pixels = (KDuint8 *) kdMalloc ( tile_size );

		if ( decode->pixels )
		{
			PVRTDecompressPVRTC ( (KDubyte*) pixels, Do2bitMode, image->ptr_tile->width, image->ptr_tile->height, decode->pixels );
		}

		kdFree ( pixels );
	}
	else
	{
		decode->pixels = pixels;

		if ( block_bpp == 2 || block_bpp == 4 )
		{
			image->ptr_tile->size = tile_size;
		}
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

KDint xmFreeImagePVR ( XMImage* image )
{
	PVRDecode*		decode = (PVRDecode*) image->decode;

	if ( decode )
	{
		if ( decode->pixels )
		{
			kdFree ( decode->pixels );
		}
	}

	return 0;
}

KDint xmSaveImagePVR ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return -1;
}