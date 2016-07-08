/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec.c
 *      Description     Functions for decoding an image that is stored in a file.  
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

typedef KDint    ( *PFNGetInfo   ) ( KDFile* file, XMImage* image );
typedef KDint    ( *PFNGetDecode ) ( KDFile* file, XMImage* image );
typedef KDint    ( *PFNReadRows  ) ( KDFile* file, XMImage* image );
typedef KDint    ( *PFNFreeImage ) ( XMImage* image);
typedef KDint    ( *PFNSaveImage ) ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels );
typedef KDuint8* ( *PFNReadTile  ) ( KDFile* file, XMImage* image );

static PFNGetInfo    l_pfn_get_info[ ] =
{
	xmGetInfoPNG	,
	xmGetInfoPVR	,
	xmGetInfoS3TC	,
	xmGetInfoETC	,
	xmGetInfoATITC	,
	xmGetInfoTIFF	,
	xmGetInfoWEBP	,
	xmGetInfoBMP	,
	xmGetInfoTGA	,
	xmGetInfoJPEG	,
	0,
};

static PFNGetDecode	 l_pfn_get_decode[ ] =
{
	xmGetDecodePNG	,
	xmGetDecodePVR	,
	xmGetDecodeS3TC	,
	xmGetDecodeETC	,
	xmGetDecodeATITC,
	xmGetDecodeTIFF	,
	xmGetDecodeWEBP	,
	xmGetDecodeBMP	,
	xmGetDecodeTGA	,
	xmGetDecodeJPEG	,
	0,
};

static PFNReadRows   l_pfn_read_rows[ ] =
{
	xmReadRowsPNG	,
	xmReadRowsPVR	,
	xmReadRowsS3TC	,
	xmReadRowsETC	,
	xmReadRowsATITC	,
	xmReadRowsTIFF	,
	xmReadRowsWEBP	,
	xmReadRowsBMP	,
	xmReadRowsTGA	,
	xmReadRowsJPEG	,
	0,
};

static PFNReadTile	l_pfn_read_tile[ ] =
{
	KD_NULL,
	xmReadTilePVR	,
	xmReadTileS3TC	,
	xmReadTileETC	,
	xmReadTileATITC	,
	KD_NULL,
	KD_NULL,
	KD_NULL,
	KD_NULL,
	KD_NULL,
	0,
};

static PFNFreeImage  l_pfn_free_image[ ] =
{
	xmFreeImagePNG	,
	xmFreeImagePVR	,
	xmFreeImageS3TC	,
	xmFreeImageETC	,
	xmFreeImageATITC,
	xmFreeImageTIFF	,
	xmFreeImageWEBP	,
	xmFreeImageBMP	,
	xmFreeImageTGA	,
	xmFreeImageJPEG	,
	0,
};

static PFNSaveImage  l_pfn_save_image[ ] = 
{
	xmSaveImagePNG	,
	xmSaveImagePVR	,
	xmSaveImageS3TC	,
	xmSaveImageETC	,
	xmSaveImageATITC,
	xmSaveImageTIFF	,
//	xmSaveImageWEBP	,
	xmSaveImageBMP	,
	xmSaveImageTGA	,
	xmSaveImageJPEG	,
	0,
};

// kdGetImageInfoATX, kdGetImageInfoFromStreamATX : Construct an informational image object based on an image in a file or stream.
KD_API KDImageATX KD_APIENTRY kdGetImageInfoATX ( const KDchar* pathname )
{
	KDImageATX      image = 0;
	KDFile*         file  = 0;

	file = kdFopen ( pathname, "rb" );
	if ( file )
	{
		image = kdGetImageInfoFromStreamATX ( file );
		kdFclose ( file );
	}

	return image;
}

KD_API KDImageATX KD_APIENTRY kdGetImageInfoFromStreamATX ( KDFile* file )
{	
	XMImage*        image  = 0;
	KDint           index  = 0;
	KDoff           offset = 0;
	
	offset = kdFtell ( file );
	if ( offset != -1 )
	{
		image = (XMImage *) kdMalloc ( sizeof ( XMImage ) );		
		if ( image )
		{			
			while ( l_pfn_get_info[ index ] != 0 )
			{
				kdFseek ( file, offset, KD_SEEK_SET );
				kdMemset ( image, 0, sizeof ( XMImage ) );
								
				if ( l_pfn_get_info[ index ] ( file, image ) == 0 )
				{
					if ( image->num_surf < 1 )	image->num_surf = 1;
					if ( image->level    < 1 )	image->level    = 1;
					break;
				}
				else
				{
					if ( kdGetError ( ) != KD_EILSEQ )
					{
						goto cleanup;
					}

					if ( image->tiles )
					{
						kdFree ( image->tiles );
						image->tiles = 0;
					}

					if ( image->info )
					{
						kdFree ( image->info );
						image->info = 0;
					}
				}

				index++;
			}
		}
	}
	
	return (KDImageATX) image;

cleanup :

	if ( image )
	{
		kdFreeImageATX ( (KDImageATX) image );
	}

	return 0;
}

// kdGetImageATX, kdGetImageFromStreamATX : Read and decode an image from a file or stream, returning a decoded image object.
KD_API KDImageATX KD_APIENTRY kdGetImageATX ( const KDchar* pathname, KDint format, KDint flags )
{
	KDImageATX      image = 0;
	KDFile*         file  = 0;

	file = kdFopen ( pathname, "rb" );
	if ( file )
	{
		image = kdGetImageFromStreamATX ( file, format, flags );
		kdFclose ( file );
	}

	return image;
}

KD_API KDImageATX KD_APIENTRY kdGetImageFromStreamATX ( KDFile* file, KDint format, KDint flags )
{
	XMImage*    image       = 0;

	KDint32	    img_x       = 0;
	KDint32	    img_y       = 0;
	KDint32	    img_width   = 0;
	KDint32	    img_height  = 0;
	KDint32	    img_bytes   = 0;
	KDint32	    img_alpha   = 0;
	KDint32	    img_format  = 0;
	KDuint8*    img_pixel   = 0;
	KDint       img_flip_x  = 0;
	KDint       img_flip_y  = 0;
	KDint		img_premult = 0;

	KDint32	    idx_surf    = 0;
	KDint32	    idx_level   = 0;
	KDint32	    idx_palette	= 0;

	KDint32	    off_row	    = 0;
	KDint32	    off_pixel   = 0;

	KDuint8*	pixels		= 0;

	KDuint32    num_shift[4];
	KDfloat32   interpolate[4];

	KDint32	    i, j, x, y;		
	
	XMBGRA      color = { 0, 0, 0, 0 };
	KDuint8	    color8;
	KDuint16    color16;	


	image = (XMImage *) kdGetImageInfoFromStreamATX ( file );

	if ( !image )
	{
		goto cleanup;
	}

	if ( !image->width || !image->height )
	{
		goto cleanup;
	}

	image->tiles = (XMTile *) kdMalloc ( sizeof( XMTile ) * image->level * image->num_surf ); 
	if ( !image->tiles )
	{
		goto cleanup;
	}
	kdMemset ( image->tiles, 0, sizeof ( XMTile ) * image->level * image->num_surf );

	img_format = image->format;
	image->format = format;

	switch ( format )
	{
		case KD_IMAGE_FORMAT_BGRA8888_ATX :
		case KD_IMAGE_FORMAT_RGBA8888_ATX :
			img_bytes = 4;
			img_alpha = 8;
			break;

		case KD_IMAGE_FORMAT_RGB888_ATX	:
			img_bytes = 3;
			img_alpha = 0;
			break;

		case KD_IMAGE_FORMAT_RGB565_ATX	:	
			img_bytes = 2;
			img_alpha = 0;
			break;

		case KD_IMAGE_FORMAT_RGBA4444_ATX :
			img_bytes = 2;
			img_alpha = 4;
			break;

		case KD_IMAGE_FORMAT_RGBA5551_ATX :
			img_bytes = 2;
			img_alpha = 1;
			break;

		case KD_IMAGE_FORMAT_LUMALPHA88_ATX :
			img_bytes = 2;
			img_alpha = 8;
			break;
		
		case KD_IMAGE_FORMAT_LUM8_ATX :
			img_bytes = 1;
			img_alpha = 0;
			break;

		case KD_IMAGE_FORMAT_ALPHA8_ATX	:
			img_bytes = 1;	
			img_alpha = 8;
			break;

		case KD_IMAGE_FORMAT_COMPRESSED_ATX :

			if ( image->type == KD_IMAGE_TYPE_PVR  ||
				 image->type == KD_IMAGE_TYPE_S3TC || 
				 image->type == KD_IMAGE_TYPE_ETC  ||
				 image->type == KD_IMAGE_TYPE_ATITC )
			{
				break;
			}

			kdSetError ( KD_EINVAL );
			goto cleanup;

		default :

			kdSetError ( KD_EINVAL );
			goto cleanup;
	}

	if ( format != KD_IMAGE_FORMAT_COMPRESSED_ATX )
	{	
		if ( img_format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
		{
			if ( image->type == KD_IMAGE_TYPE_ETC )
			{
				image->bpp = 24;	
			}
			else
			{
				image->bpp = 32;	
			}
				
			image->flip_y = 0;
			image->bitmask[0] = 0x000000FF;
			image->bitmask[1] = 0x0000FF00;
			image->bitmask[2] = 0x00FF0000;
			image->bitmask[3] = 0xFF000000;		
		}

		if ( ( flags & KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA ) && !image->premult_alpha && img_format != KD_IMAGE_FORMAT_ALPHA8_ATX )
		{
			img_premult = 1;
		}

		image->rows = (KDuint8 *) kdMalloc ( image->width * KD_MAX ( 1, ( image->bpp >> 3 ) ) + 4 );
		if ( !image->rows )
		{
			goto cleanup;
		}

		kdMemset ( num_shift, 0, sizeof ( num_shift ) );

		switch ( image->bpp )
		{
			case 64 :
			case 48 :
			case 32 :
			case 24 :

				if ( img_format == KD_IMAGE_FORMAT_LUMALPHA_ATX )
				{
					break;
				}

				if ( image->bitmask[0] == 0 && image->bitmask[1] == 0 && image->bitmask[2] == 0 )
				{
					num_shift[ 0 ] = 2;
					num_shift[ 1 ] = 1;
					num_shift[ 2 ] = 0;
					num_shift[ 3 ] = 3;							
				}
				else
				{
					for ( i = 8; i < 32; i += 8 ) 
					{
						for ( j = 0; j < 3; j++ )
						{
							if ( image->bitmask[ j ] >> i )
							{
								num_shift[ j ]++;
							}							
						}
					}

					num_shift[ 3 ] = 6 - num_shift[ 0 ] - num_shift[ 1 ] - num_shift[ 2 ];
				}			

				break;

			case 16 :

				if ( img_format == KD_IMAGE_FORMAT_LUMALPHA_ATX )
				{
					break;
				}

				for ( i = 0; i < 16; i++ )
				{
					for ( j = 0; j < 3; j++ )
					{
						if ( ( image->bitmask[ j ] >> i ) & 0x01 )
						{
							num_shift[j]++;
						}
					}
				}

				num_shift[ 3 ] = 16 - num_shift[ 0 ] - num_shift[ 1 ] - num_shift[ 2 ];
						
				for ( i = 0; i < 4; i++ )
				{
					interpolate[ i ] = 255 / ( kdPowf ( 2.0f, (KDfloat32) num_shift[ i ] ) - 1 );
				}		

				break;

			default :

				if ( img_format == KD_IMAGE_FORMAT_PALETTED_ATX )
				{
					image->palette = (XMBGRA *) kdMalloc ( sizeof ( XMBGRA ) * ( 1 << image->bpp ) );
					if ( !image->palette ) 
					{
						goto cleanup;
					}

					kdMemset ( image->palette, 255, sizeof ( XMBGRA ) * ( 1<< image->bpp ) );
				}							
		}
	}

	if ( l_pfn_get_decode[ image->type ]( file, image ) != 0 )
	{
		goto cleanup;
	}

	img_flip_x = image->flip_x;
	img_flip_y = image->flip_y;

	if ( flags & KD_IMAGE_FLAG_FLIP_X_ATX )	img_flip_x = !img_flip_x;
	if ( flags & KD_IMAGE_FLAG_FLIP_Y_ATX )	img_flip_y = !img_flip_y;

	for ( idx_surf = 0; idx_surf < image->num_surf; idx_surf++ )
	{
		img_width  = image->width;
		img_height = image->height;

		for ( idx_level = 0; idx_level < image->level; idx_level++ )
		{			
			image->ptr_tile = &image->tiles [ image->level * idx_surf + idx_level ];	

			image->ptr_tile->width  = img_width;
			image->ptr_tile->height = img_height;
			image->ptr_tile->offset = off_pixel;
			image->ptr_tile->size   = img_width * img_height * img_bytes;

			if ( l_pfn_read_tile [ image->type ] )
			{
				if ( !( pixels = l_pfn_read_tile [ image->type ] ( file, image ) ) )
				{
					goto cleanup;
				}
			}

			image->pixels = (KDuint8*) kdRealloc ( image->pixels, off_pixel + image->ptr_tile->size );
			
			if ( format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
			{								
				kdMemcpy ( image->pixels + off_pixel, pixels, image->ptr_tile->size );
			}
			else
			{									
				for ( y = 0; y < img_height; y++ )
				{
					img_y = img_flip_y ? img_height - y - 1 : y;
									
					if ( l_pfn_read_rows[ image->type ]( file, image ) != 0 )
					{
						goto cleanup;
					}		

					for ( x = 0; x < img_width; x++ )
					{
						img_x = img_flip_x ? img_width - x - 1 : x;
						
						off_row = image->bpp > 8 ? x * ( image->bpp >> 3 ) : x / ( 8 / image->bpp );
						img_pixel = &image->pixels[ off_pixel + img_y * img_width * img_bytes + img_x * img_bytes ];

						switch ( image->bpp )
						{
							case 64 :

								color.r = image->rows[ off_row + num_shift[ 0 ] * 2 ];
								color.g = image->rows[ off_row + num_shift[ 1 ] * 2 ];
								color.b = image->rows[ off_row + num_shift[ 2 ] * 2 ];
								color.a = image->rows[ off_row + num_shift[ 3 ] * 2 ];
								break;

							case 48 :

								color.r = image->rows[ off_row + num_shift[ 0 ] * 2 ];
								color.g = image->rows[ off_row + num_shift[ 1 ] * 2 ];
								color.b = image->rows[ off_row + num_shift[ 2 ] * 2 ];
								color.a = 255;
								break;

							case 32 :

								if ( img_format == KD_IMAGE_FORMAT_LUMALPHA_ATX )
								{
									color.r = image->rows[ off_row + 0 ];
									color.g = image->rows[ off_row + 0 ];
									color.b = image->rows[ off_row + 0 ];
									color.a = image->rows[ off_row + 2 ];
								}
								else
								{
									color.r = image->rows[ off_row + num_shift[ 0 ] ];
									color.g = image->rows[ off_row + num_shift[ 1 ] ];
									color.b = image->rows[ off_row + num_shift[ 2 ] ];
									color.a = image->rows[ off_row + num_shift[ 3 ] ];
								}
								break;

							case 24 :				

								color.r = image->rows[ off_row + num_shift[ 0 ] ];
								color.g = image->rows[ off_row + num_shift[ 1 ] ];
								color.b = image->rows[ off_row + num_shift[ 2 ] ];
								color.a = 255;
								break;

							case 16 :
								
								switch( img_format )
								{
									case KD_IMAGE_FORMAT_LUMALPHA_ATX :

										color.r = image->rows[ off_row + 0 ];
										color.g = image->rows[ off_row + 0 ];
										color.b = image->rows[ off_row + 0 ];	
										color.a = image->rows[ off_row + 1 ];	
										break;

									case KD_IMAGE_FORMAT_RGBA_ATX :

										color16 = image->rows[ off_row + 0 ] + 256 * image->rows[ off_row + 1 ];		

										if ( image->type != KD_IMAGE_TYPE_PVR )
										{
											color.r = (KDuint8) ( ( ( color16 & image->bitmask[ 0 ] ) >> ( num_shift[ 1 ] + num_shift[ 0 ] ) ) * interpolate[ 0 ] );			
											color.g = (KDuint8) ( ( ( color16 & image->bitmask[ 1 ] ) >>   num_shift[ 0 ] ) * interpolate[ 1 ] );
											color.b = (KDuint8) (   ( color16 & image->bitmask[ 2 ] ) *    interpolate[ 2 ]);
											color.a = (KDuint8) ( ( ( color16 & image->bitmask[ 3 ] ) >> ( num_shift[ 0 ] + num_shift[ 1 ] + num_shift[ 2 ] ) ) * interpolate[ 3 ] );
										}
										else
										{
											color.r = (KDuint8) ( ( ( color16 & image->bitmask[ 0 ] ) >> ( num_shift[ 3 ] + num_shift[ 2 ] + num_shift[ 1 ] ) ) * interpolate[ 0 ] );			
											color.g = (KDuint8) ( ( ( color16 & image->bitmask[ 1 ] ) >> ( num_shift[ 3 ] + num_shift[ 2 ] ) ) * interpolate[ 1 ] );
											color.b = (KDuint8) ( ( ( color16 & image->bitmask[ 2 ] ) >> ( num_shift[ 3 ] ) ) * interpolate[ 2 ] );
											color.a = (KDuint8) ( ( ( color16 & image->bitmask[ 3 ] ) ) * interpolate[ 3 ] );
										}
				
										break;

									case KD_IMAGE_FORMAT_RGB_ATX :

										color16	 = image->rows[ off_row + 0 ] + 256 * image->rows[ off_row + 1 ];		

										color.r	 = (KDuint8) ( ( ( color16 & image->bitmask[ 0 ] ) >> ( num_shift[ 1 ] + num_shift[ 0 ] ) ) * interpolate[ 0 ] );			
										color.g	 = (KDuint8) ( ( ( color16 & image->bitmask[ 1 ] ) >>   num_shift[ 0 ] ) * interpolate[ 1 ]);
										color.b	 = (KDuint8) (   ( color16 & image->bitmask[ 2 ] ) *    interpolate[ 2 ] );
										color.a = 255;
										break;
								}
								break;

							default :

								switch( image->bpp )
								{
									case 8 : interpolate[ 0 ] = 1;		break;
									case 4 : interpolate[ 0 ] = 15;		break;
									case 2 : interpolate[ 0 ] = 127;	break;
									case 1 : interpolate[ 0 ] = 255;	break;
								}
								
								num_shift[ 0 ] = ( ( 8 / image->bpp - 1 ) - ( x % ( 8 / image->bpp ) ) ) * image->bpp;						 

								switch ( img_format )
								{
									case KD_IMAGE_FORMAT_PALETTED_ATX :

										idx_palette = ( image->rows[ off_row ] >> num_shift[ 0 ] ) & ( ( 0x1 << image->bpp ) - 1 );								

										color.r	= image->palette[ idx_palette ].r;
										color.g	= image->palette[ idx_palette ].g;
										color.b	= image->palette[ idx_palette ].b;
										color.a = image->palette[ idx_palette ].a;
										break;
									
									case KD_IMAGE_FORMAT_LUMINANCE_ATX :

										color8	= ( ( image->rows[ off_row ] >> num_shift[ 0 ] ) & ( ( 0x1 << image->bpp ) - 1 ) ) * (KDint) interpolate[ 0 ];

										color.r	= color8;
										color.g	= color8;
										color.b	= color8;
										color.a = 255;
										break;

									case KD_IMAGE_FORMAT_ALPHA8_ATX :
										color8	= ( ( image->rows[ off_row ] >> num_shift[ 0 ] ) & ( ( 0x1 << image->bpp ) - 1 ) ) * (KDint) interpolate[ 0 ];
										color.r	= color8;
										color.g	= color8;
										color.b	= color8;
										color.a = color8;
										break;
								}
						}

						if ( img_premult )
						{
							color.r = (KDuint8) ( (KDuint32) ( color.r * ( color.a + 1 ) ) >> 8 );
							color.g = (KDuint8) ( (KDuint32) ( color.g * ( color.a + 1 ) ) >> 8 );
							color.b = (KDuint8) ( (KDuint32) ( color.b * ( color.a + 1 ) ) >> 8 );
						}						

						switch ( format )
						{
							case KD_IMAGE_FORMAT_BGRA8888_ATX :
								img_pixel[ 0 ] = color.b;
								img_pixel[ 1 ] = color.g;
								img_pixel[ 2 ] = color.r;
								img_pixel[ 3 ] = color.a;					
								break;

							case KD_IMAGE_FORMAT_RGBA8888_ATX :	
								img_pixel[ 0 ] = color.r;
								img_pixel[ 1 ] = color.g;
								img_pixel[ 2 ] = color.b;
								img_pixel[ 3 ] = color.a;					
								break;

							case KD_IMAGE_FORMAT_RGB888_ATX	:
								img_pixel[ 0 ] = color.r;
								img_pixel[ 1 ] = color.g;
								img_pixel[ 2 ] = color.b;				
								break;

							case KD_IMAGE_FORMAT_RGB565_ATX	:
								color16 = KD_MAKE_RGB565 ( color.r, color.g, color.b );
								kdMemcpy ( img_pixel, &color16, 2 );						
								break;

							case KD_IMAGE_FORMAT_RGBA5551_ATX :
								color16 = KD_MAKE_RGBA5551 ( color.r, color.g, color.b, color.a );
								kdMemcpy ( img_pixel, &color16, 2 );						
								break;
			
							case KD_IMAGE_FORMAT_RGBA4444_ATX :
								color16 = KD_MAKE_RGBA4444 ( color.r, color.g, color.b, color.a );
								kdMemcpy ( img_pixel, &color16, 2 );						
								break;

							case KD_IMAGE_FORMAT_ALPHA8_ATX	:
								img_pixel[ 0 ] = color.a;					
								break;

							case KD_IMAGE_FORMAT_LUM8_ATX :
								img_pixel[ 0 ] = (KDuint8) ( 0.299f * color.r + 0.587f * color.g + 0.114f * color.b );					
								break;

							case KD_IMAGE_FORMAT_LUMALPHA88_ATX :
								img_pixel[ 0 ] = (KDuint8) ( 0.299f * color.r + 0.587f * color.g + 0.114f * color.b );				
								img_pixel[ 1 ] = color.a;						
								break;
						}	
					}
				}
			}

			off_pixel += image->ptr_tile->size;

			img_width  = KD_MAX ( img_width  / 2, 1 );
			img_height = KD_MAX ( img_height / 2, 1 );
		}
	}

	if ( format != KD_IMAGE_FORMAT_COMPRESSED_ATX )
	{
		image->bpp   = img_bytes << 3;
		image->alpha = img_alpha;
	}

	image->premult_alpha |= img_premult;

	l_pfn_free_image[ image->type ]( image );

	kdFree ( image->palette );
	image->palette = 0;

	kdFree ( image->rows );
	image->rows = 0;

	if ( image->info )
	{
		kdFree ( image->info );
		image->info = 0;
	}

	if ( image->decode )
	{
		kdFree ( image->decode );
		image->decode = 0;
	}

	return (KDImageATX) image;

cleanup :

	if ( image )
	{
		kdFreeImageATX ( (KDImageATX) image );
	}

	return 0;
}

// kdFreeImageATX : Free image object. 
KD_API KDvoid KD_APIENTRY kdFreeImageATX ( KDImageATX image )
{
	XMImage*     ximage = (XMImage *) image;

	l_pfn_free_image[ ximage->type ] ( ximage );

	if ( ximage->tiles )
	{
		kdFree ( ximage->tiles );
	}

	if ( ximage->info )
	{
		kdFree ( ximage->info );
	}

	if ( ximage->decode )
	{
		kdFree ( ximage->decode );
	}

	if ( ximage->palette )
	{
		kdFree ( ximage->palette );
	}

	if ( ximage->pixels )
	{
		kdFree ( ximage->pixels );
	}

	if ( ximage->rows )
	{
		kdFree ( ximage->rows );
	}

	kdFree( image );
}

// kdGetImagePointerATX : Get the value of an image pointer attribute.
KD_API KDvoid* KD_APIENTRY kdGetImagePointerATX ( KDImageATX image, KDint attr )
{
	if ( attr == KD_IMAGE_POINTER_BUFFER_ATX )
	{
		return ( (XMImage *) image )->pixels;
	}
	else
	{
		kdSetError ( KD_EINVAL );
	}

	return 0;
}

// kdGetImageIntATX, kdGetImageLevelIntATX : Get the value of an image integer attribute.
KD_API KDint KD_APIENTRY kdGetImageIntATX ( KDImageATX image, KDint attr )
{	
	if ( attr == KD_IMAGE_DATASIZE_ATX )
	{
		XMImage*    ximage = (XMImage *) image;
		KDint		size = 0;
		KDint       i;

		if ( ximage->tiles )
		{
			for ( i = 0; i < ximage->level; i++ )
			{
				size += ximage->tiles [ i ].size;				
			}
		}		

		return size;
	}
	else
	{
		return kdGetImageLevelIntATX ( image, attr, 0 );
	}
}

KD_API KDint KD_APIENTRY kdGetImageLevelIntATX ( KDImageATX image, KDint attr, KDint level )
{
	XMImage*    ximage = (XMImage *) image;

	switch ( attr )
	{
		case KD_IMAGE_FORMAT_ATX				:   return ximage->format;		
		case KD_IMAGE_BITSPERPIXEL_ATX			:   return ximage->bpp;
		case KD_IMAGE_LEVELS_ATX				:   return ximage->level;
		case KD_IMAGE_ALPHA_ATX					:   return ximage->alpha;
		case KD_IMAGE_WIDTH_ATX					:   return ximage->tiles ? ximage->tiles[ level ].width  : ximage->width;
		case KD_IMAGE_HEIGHT_ATX				:   return ximage->tiles ? ximage->tiles[ level ].height : ximage->height;
		case KD_IMAGE_STRIDE_ATX				:   return ximage->tiles ? ximage->tiles[ level ].stride : 0;
		case KD_IMAGE_BUFFEROFFSET_ATX			:   return ximage->tiles ? ximage->tiles[ level ].offset : 0;
		case KD_IMAGE_DATASIZE_ATX				:   return ximage->tiles ? ximage->tiles[ level ].size   : 0;
		case KD_IMAGE_TYPE						:	return ximage->type;
		case KD_IMAGE_FLAG_FLIP_X_ATX			:	return ximage->flip_x;
		case KD_IMAGE_FLAG_FLIP_Y_ATX			:	return ximage->flip_y;
		case KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA	:	return ximage->premult_alpha;
		
		default	:   kdSetError ( KD_EINVAL );
	}

	return 0;
}

KD_API KDint KD_APIENTRY kdSaveImageATX ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	KDint    len  = kdStrlen ( pathname );
	KDint    type = KD_IMAGE_TYPE_JPEG;

	if ( len > 4 )
	{
		if ( !kdStrncasecmp ( &pathname [ len - 3 ], "PNG", 3 ) )
		{
			type = KD_IMAGE_TYPE_PNG;
		}
		/*
		else if ( !kdStrncasecmp ( &pathname [ len - 3 ], "BMP", 3 ) )
		{
			type = KD_IMAGE_TYPE_BMP;
		}
		else if ( !kdStrncasecmp ( &pathname [ len - 3 ], "TGA", 3 ) )
		{
			type = KD_IMAGE_TYPE_BMP;
		}
		else if ( !kdStrncasecmp ( &pathname [ len - 3 ], "PVR", 3 ) )
		{
			type = KD_IMAGE_TYPE_PVR;
		}
		else if ( !kdStrncasecmp ( &pathname [ len - 3 ], "TIF", 3 ) || !kdStrncasecmp ( &pathname [ len - 4 ], "TIFF", 4 ) )
		{
			type = KD_IMAGE_TYPE_TIFF;
		}
		*/
	}

	return l_pfn_save_image [ type ] ( pathname, width, height, format, pixels );
}