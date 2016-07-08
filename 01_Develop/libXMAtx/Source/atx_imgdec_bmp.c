/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_bmp.c
 *      Description     The ability to decode Bitmap images to the ATX_imgdec extension. 
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

#define XM_BI_RGB                           0
#define XM_BI_RLE8                          1L
#define XM_BI_RLE4                          2L
#define XM_BI_BITFIELDS                     3L

#define XM_BFT_ICON                         0x4349   // 'IC' 
#define XM_BFT_BITMAP                       0x4d42   // 'BM' 
#define XM_BFT_CURSOR                       0x5450   // 'PT' 

#define XM_WIDTH_BYTES( i )                 ( ( KDuint32 ) ( ( i + 31 ) & ( ~31 ) ) / 8 )	
#define XM_DIB_WIDTH_BYTES_N( lpbi, n )	    ( KDuint32 ) XM_WIDTH_BYTES ( ( KDuint32 ) ( lpbi )->biWidth * ( KDuint32 ) ( n ) )
#define XM_DIB_WIDTH_BYTES( lpbi )		    XM_DIB_WIDTH_BYTES_N ( lpbi, (lpbi)->biBitCount )

#define XM_DIB_SIZE_IMAGE( lpbi )           ( ( lpbi )->biSizeImage == 0											         \
                                            ? ( ( KDuint32 ) XM_DIB_WIDTH_BYTES ( lpbi ) * ( KDuint32 ) ( lpbi )->biHeight ) \
                                            : ( lpbi)->biSizeImage )

#define XM_DIB_NUM_COLORS( lpbi )           ( ( lpbi )->biClrUsed == 0 && (lpbi )->biBitCount <= 8 \
                                            ? ( KDint32 )( 1 << ( KDint32 )( lpbi )->biBitCount )  \
                                            : ( KDint32 )( lpbi )->biClrUsed)

#define XM_FIX_BITMAP_INFO( lpbi )          if ( ( lpbi )->biSizeImage == 0 )                   \
                                            ( lpbi )->biSizeImage = XM_DIB_SIZE_IMAGE ( lpbi ); \
                                            if ( ( lpbi )->biClrUsed == 0 )	                    \
                                            ( lpbi )->biClrUsed = XM_DIB_NUM_COLORS ( lpbi );							

#define XM_RLE_COMMAND                      0
#define XM_RLE_ENDOFLINE                    0
#define XM_RLE_ENDOFBITMAP                  1
#define XM_RLE_DELTA                        2

typedef struct XMBMPFile                    XMBMPFile;
typedef struct XMBMPInfo                    XMBMPInfo;
typedef struct XMBMPCore                    XMBMPCore;

#pragma pack(2)

struct XMBMPFile
{
	KDuint16            bfType;
	KDuint32            bfSize;
	KDuint16            bfReserved1;
	KDuint16            bfReserved2;
	KDuint32            bfOffBits;
};

struct XMBMPInfo
{
	KDuint32            biSize;
	KDint32             biWidth;
	KDint32             biHeight;
	KDuint16            biPlanes;
	KDuint16            biBitCount;
	KDuint32            biCompression;
	KDuint32            biSizeImage;
	KDint32             biXPelsPerMeter;
	KDint32             biYPelsPerMeter;
	KDuint32            biClrUsed;
	KDuint32            biClrImportant;
};

struct XMBMPCore
{
	KDuint32            bcSize;            // used to get to color table 
	KDuint16            bcWidth;
	KDuint16            bcHeight;
	KDuint16            bcPlanes;
	KDuint16            bcBitCount;
};

typedef struct 
{
	XMBMPFile           file;
	XMBMPInfo           info;
} BMPInfo;

typedef struct 
{	
	KDuint32            offset_x;
	KDuint32            offset_y;
	KDuint32            offset_row;	
} BMPDecode;

#pragma pack()

static KDvoid xmBMPInfoToLittleEndian ( XMBMPInfo* info )
{
	if ( kdIsLittleEndian ( ) == KD_FALSE )
	{
		info->biSize          = kdEndianSwap32 ( info->biSize          );
		info->biWidth         = kdEndianSwap32 ( info->biWidth         );	
		info->biHeight        = kdEndianSwap32 ( info->biHeight        );
		info->biPlanes        = kdEndianSwap16 ( info->biPlanes        );
		info->biBitCount      = kdEndianSwap16 ( info->biBitCount      );
		info->biCompression	  = kdEndianSwap32 ( info->biCompression   );
		info->biSizeImage     = kdEndianSwap32 ( info->biSizeImage	   );
		info->biXPelsPerMeter = kdEndianSwap32 ( info->biXPelsPerMeter );
		info->biYPelsPerMeter = kdEndianSwap32 ( info->biYPelsPerMeter );
		info->biClrUsed	      = kdEndianSwap32 ( info->biClrUsed       );
		info->biClrImportant  = kdEndianSwap32 ( info->biClrImportant  );
	}
}

KDint xmGetInfoBMP ( KDFile* file, XMImage* image )
{
	BMPInfo*		info;
	XMBMPCore       core;
	KDint           i, j;
	KDuint32        bitnumber[3] = { 0, 0, 0 };

	image->info = kdCalloc ( 1, sizeof ( BMPInfo ) );
	info = (BMPInfo*) image->info;
	if ( !info )
	{
		return -1;
	}

	if ( kdFread ( &info->file, sizeof ( XMBMPFile ), 1, file ) == 0 )
	{			
		kdSetError ( KD_EILSEQ );

		return -1;
	}

	if ( kdIsLittleEndian ( ) == KD_FALSE )
	{
		info->file.bfSize = kdEndianSwap32 ( info->file.bfSize );
		info->file.bfOffBits = kdEndianSwap32 ( info->file.bfOffBits );
	}

	if ( info->file.bfType != XM_BFT_BITMAP )					
	{
		kdSetError ( KD_EILSEQ );

		return -1;
	}

	if(	kdFread ( &info->info, sizeof ( XMBMPInfo ), 1, file ) == 0 )
	{
		kdSetError ( KD_EILSEQ );

		return -1;
	}

	xmBMPInfoToLittleEndian ( &info->info );

	switch ( info->info.biSize )					
	{
		case sizeof ( XMBMPInfo ) :
			break;

		case 64 :	

			kdFseek ( file, (KDoff) ( 64 - sizeof ( XMBMPInfo ) ), KD_SEEK_CUR );			
			break;

		case sizeof ( XMBMPCore ) :

			kdMemcpy ( &core, &info->info, sizeof ( XMBMPCore ) );
							
			info->info.biSize          = core.bcSize;
			info->info.biWidth         = (KDuint32) core.bcWidth;
			info->info.biHeight        = (KDuint32) core.bcHeight;
			info->info.biPlanes        = core.bcPlanes;
			info->info.biBitCount      = core.bcBitCount;
			info->info.biCompression	  = XM_BI_RGB;
			info->info.biSizeImage     = 0;
			info->info.biXPelsPerMeter = 0;
			info->info.biYPelsPerMeter = 0;
			info->info.biClrUsed       = 0;
			info->info.biClrImportant  = 0;

			kdFseek ( file, (KDoff) ( sizeof ( XMBMPCore )  - sizeof ( XMBMPInfo ) ), KD_SEEK_CUR );	

			break;

		default :			

			if ( info->info.biSize > ( sizeof ( XMBMPInfo ) ) &&
			   ( info->info.biSizeImage >= (KDuint32) ( info->info.biHeight * ( ( ( ( info->info.biBitCount * info->info.biWidth ) + 31 ) / 32 ) * 4 ) ) ) &&
			   ( info->info.biPlanes == 1 ) && ( info->info.biClrUsed == 0 ) )
			{
				if ( info->info.biCompression == XM_BI_RGB )
				{
					kdFseek ( file, (KDoff) ( info->info.biSize - sizeof ( XMBMPInfo ) ), KD_SEEK_CUR );						
				}

				break;
			}

			kdSetError ( KD_EILSEQ );

			return -1;
	}

	XM_FIX_BITMAP_INFO ( &info->info );

	image->type	 = KD_IMAGE_TYPE_BMP;
	image->bpp   = info->info.biBitCount;
	image->width = info->info.biWidth;

	if ( info->info.biHeight < 0 )
	{
		image->height = -info->info.biHeight;
		image->flip_y = -1;
	}
	else
	{
		image->height = info->info.biHeight;
		image->flip_y = 0;
	}	

	switch ( info->info.biBitCount )
	{
		case 32 : 	

			if ( info->info.biCompression == XM_BI_BITFIELDS )
			{
				if ( kdFread ( image->bitmask, 12, 1, file ) == 0 )
				{			
					kdSetError ( KD_EILSEQ );
					
					return -1;
				}
			}
			else
			{
				image->bitmask[0] = 0x00FF0000;
				image->bitmask[1] = 0x0000FF00;
				image->bitmask[2] = 0x000000FF;								
			}							

			image->alpha  = 8;
			image->format = KD_IMAGE_FORMAT_RGBA_ATX;

			break;

		case 24 : 			

			if ( info->info.biCompression != XM_BI_RGB )
			{
				kdSetError ( KD_ENOSPC );

				return -1;
			}

			image->alpha  = 0;
			image->format = KD_IMAGE_FORMAT_RGB_ATX;	

			break;

		case 16 : 		

			if ( info->info.biCompression == XM_BI_BITFIELDS )	
			{
				if ( kdFread( image->bitmask, 12, 1, file ) == 0 )
				{			
					return -1;
				}

				for ( i = 0; i < 16; i++ )
				{
					for( j = 0; j < 3; j++ )
					{
						if ( ( image->bitmask[ j ] >> i ) & 0x01 )
						{
							bitnumber[ j ]++;
						}
					}
				}

				image->alpha  = 16 - bitnumber[0] - bitnumber[1] - bitnumber[2];
				image->format = image->alpha > 0 ? KD_IMAGE_FORMAT_RGBA_ATX : KD_IMAGE_FORMAT_RGB_ATX;
			}
			else
			{
				image->bitmask[0] = 0x7C00;
				image->bitmask[1] = 0x03E0;
				image->bitmask[2] = 0x001F; 

				image->alpha  = 0;
				image->format = KD_IMAGE_FORMAT_RGB_ATX;	
			}

			image->bitmask[3] = 0xffff & ~( image->bitmask[0] | image->bitmask[1] | image->bitmask[2] );

			break;

		case  8 : 
		case  4 :
		case  1 : 

			image->alpha  = 0;
			image->format = KD_IMAGE_FORMAT_PALETTED_ATX; 

			break;	

		default :

			kdSetError ( KD_ENOSPC );
			return -1;
	}

	return 0;
}

KDint xmGetDecodeBMP ( KDFile* file, XMImage* image )
{
	BMPInfo*		info = (BMPInfo*) image->info;
	BMPDecode*		decode = 0;
	XMBGR*			palette = 0;
	KDint			i;

	image->flip_y = 1;

	if ( image->bpp > 8 )
	{
		return 0;
	}

	image->decode = kdCalloc ( 1, sizeof ( BMPDecode ) );
	decode = (BMPDecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}	

	if ( image->bpp <= 8 )
	{			 
		if ( info->info.biSize == sizeof ( XMBMPCore ) )
		{
			if ( ( palette = (XMBGR *) kdMalloc ( sizeof ( XMBGR ) * XM_DIB_NUM_COLORS ( &info->info ) ) ) )
			{
				if ( kdFread ( palette, XM_DIB_NUM_COLORS ( &info->info ) * sizeof ( XMBGR ), 1, file ) != 0 )
				{				
					for ( i = 0; i < XM_DIB_NUM_COLORS ( &info->info ); i++ )
					{
						image->palette[i].r = palette[i].r;
						image->palette[i].g = palette[i].g;
						image->palette[i].b = palette[i].b;
						image->palette[i].a = 255;				
					}

					kdFree ( palette );
				}				
			}
			else
			{
				return -1;
			}
		}
		else
		{	 			
			if ( kdFread ( image->palette, XM_DIB_NUM_COLORS ( &info->info ) * sizeof ( XMBGRA ), 1, file ) != 0 )
			{			
				for ( i = 0; i < XM_DIB_NUM_COLORS ( &info->info ); i++ )
				{
					image->palette[i].a = 255;				
				}
			}		
			else
			{
				return -1;
			}
		} 
	}

	kdFseek ( file, (KDoff) ( info->file.bfOffBits ), KD_SEEK_SET );

	return 0;
}

KDint xmReadRowsBMP ( KDFile* file, XMImage* image )
{	
	BMPInfo*		info		= (BMPInfo*) image->info;
	BMPDecode*		decode		= (BMPDecode*) image->decode;
	KDint32			stride		= 0;
	KDint			low_nibble	= 0;
	KDint			reading		= 1;

	KDuint8			rle[2];
	KDint			i;

	switch ( image->bpp )
	{
		case 1  :	stride = image->width / 8;                     break;
		case 4  :	stride = image->width / 2;                     break;
		default :	stride = image->width * ( image->bpp >> 3 );   break;
	}

	while ( stride % 4 )	
	{
		stride++;	
	}	

	switch ( info->info.biCompression )
	{
		case XM_BI_RLE8 :
		case XM_BI_RLE4 :	

			kdMemset ( image->rows, 0, stride );	

			if ( decode->offset_y != 0 )
			{
				if ( decode->offset_row < decode->offset_y )
				{
					break;
				}

				decode->offset_y = 0;
			}

			while ( reading && kdFread ( rle, 2, 1, file ) )
			{
				if ( rle[0] == XM_RLE_COMMAND )
				{
					switch ( rle[1] )
					{
						case XM_RLE_ENDOFBITMAP :
						case XM_RLE_ENDOFLINE :

							decode->offset_x = 0;
							decode->offset_y = 0;

							reading = 0;
							low_nibble = 0;

							break;

						case XM_RLE_DELTA :

							kdFread ( rle, 2, 1, file );
							
							decode->offset_x = rle[ 0 ];
							decode->offset_y = rle[ 1 ];

							if ( info->info.biCompression == XM_BI_RLE4 )
							{
								decode->offset_x /= 2;										
							}
							
							if ( decode->offset_y != 0 )
							{
								reading = 0;
							}

							break;

						default :

							if ( info->info.biCompression == XM_BI_RLE8 )
							{
								kdFread ( &image->rows[ decode->offset_x ], rle[ 1 ], 1, file );

								decode->offset_x += rle[ 1 ];
							
								if ( ( rle[ 1 ] & 1 ) == 1 )
								{
									kdFread( &rle[ 1 ], 1, 1, file );
								}	
							}
							else
							{										
								kdFread ( &rle[ 0 ], 1, 1, file );
								
								for ( i = 0; i < rle[ 1 ]; i++ )
								{
									if ( low_nibble == 1 )
									{
										image->rows[ decode->offset_x ] |= ( i & 1 ) ? rle[ 0 ] & 0x0F : ( rle[ 0 ] & 0xF0 ) >> 4;
										decode->offset_x++;
									}
									else
									{
										image->rows[ decode->offset_x ] |= (i & 1) ? ( rle[ 0 ] & 0x0F ) << 4 : rle[ 0 ] & 0xF0;
									}
							
									if ( ( i & 1 ) && ( i != ( rle[ 1 ] - 1 ) ) )
									{
										kdFread ( &rle[ 0 ], 1, 1, file );
									}
							
									low_nibble = !low_nibble;
								}
							
								if ( ( ( ( rle[ 1 ] + 1 ) >> 1 ) & 1 ) == 1 )
								{
									kdFread ( &rle[ 0 ], 1, 1, file );
								}
							}		
					}
				}
				else
				{
					if ( info->info.biCompression == XM_BI_RLE8 )
					{
						kdMemset ( &image->rows[ decode->offset_x ], rle[ 1 ], rle[ 0 ] );
						decode->offset_x += rle[ 0 ];
					}
					else
					{								
						for ( i = 0; i < rle[ 0 ]; i++ )
						{
							if ( low_nibble == 1 )
							{
								image->rows[ decode->offset_x ] |= ( i & 1 ) ? rle[ 1 ] & 0x0F : ( rle[ 1 ] & 0xF0 ) >> 4;
								decode->offset_x++;
							}
							else
							{
								image->rows[ decode->offset_x ] |= ( i & 1 ) ? ( rle[ 1 ] & 0x0F ) << 4 : rle[ 1 ] & 0xF0;
							}

							low_nibble = !low_nibble;
						}
					}
				}
			}			

			break;

		default :

			if ( kdFread ( image->rows, stride, 1, file ) == 0 )
			{			
				return -1;
			}							
	}	

	if ( decode )
	{
		decode->offset_row++;
	}

	return 0;
}

KDint xmFreeImageBMP ( XMImage* image )
{
	return 0;
}

KDint xmSaveImageBMP ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	return -1;
}