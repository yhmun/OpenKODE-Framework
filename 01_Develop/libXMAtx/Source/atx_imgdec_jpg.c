/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_jpg.c
 *      Description     The ability to decode JPEG images to the ATX_imgdec extension. 
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

#include "JPEG/jpeglib.h"
#include "JPEG/jerror.h"

struct KDFile
{
	FILE*          ptr;
	KDchar*        path;
};

struct my_error_mgr
{
	struct jpeg_error_mgr           pub;                    // "public" fields 
#ifndef SHP
	jmp_buf                         setjmp_buffer;          // for return to caller
#endif
};

typedef struct 
{
	struct jpeg_decompress_struct   jpeg_decompress;
	struct my_error_mgr             error;		
} JPEGInfo;

typedef struct 
{
	JSAMPARRAY                      buffer;	
} JPEGDecode;

static KDvoid xmErrorExit ( j_common_ptr cinfo )
{

}

KDint xmGetInfoJPEG ( KDFile* file, XMImage* image )
{
	JPEGInfo*		info = 0;

	image->info = kdCalloc ( 1, sizeof ( JPEGInfo ) );
	info = (JPEGInfo*) image->info; 
	if ( !info )
	{
		return -1;
	}
    
	// Step 1: allocate and initialize JPEG decompression object 
	info->jpeg_decompress.err  = jpeg_std_error ( & info->error.pub );
	info->error.pub.error_exit = xmErrorExit;

	// Establish the setjmp return context for my_error_exit to use. 

	// Now we can initialize the JPEG decompression object. 
	jpeg_create_decompress ( &info->jpeg_decompress );

	// Step 2: specify data source (eg, a file) 
	jpeg_stdio_src ( &info->jpeg_decompress, file );

	// Step 3: read file parameters with jpeg_read_header() 
	if ( jpeg_read_header ( &info->jpeg_decompress, 1 ) != JPEG_HEADER_OK )
	{
		kdSetError ( KD_EILSEQ );
		jpeg_destroy_decompress ( &info->jpeg_decompress );

		return -1;
	}

	image->type   = KD_IMAGE_TYPE_JPEG;
	image->format = info->jpeg_decompress.out_color_space == JCS_GRAYSCALE ? KD_IMAGE_FORMAT_LUMINANCE_ATX : KD_IMAGE_FORMAT_RGB_ATX;			
	image->width  = info->jpeg_decompress.image_width;
	image->height = info->jpeg_decompress.image_height;
	image->flip_y = 0;	

	switch ( info->jpeg_decompress.out_color_space )
	{
		case JCS_GRAYSCALE :
			image->bpp = 8;
			break;

		case JCS_CMYK :				 		
			image->bpp = 24;			
			break;

		case JCS_RGB :
			image->bpp = 24;
			break;
		
		case JCS_YCbCr   :              // Y/Cb/Cr (also known as YUV) 
		case JCS_YCCK    :              // Y/Cb/Cr/K 
		case JCS_UNKNOWN :
			kdSetError ( KD_ENOSPC );
			jpeg_destroy_decompress ( &info->jpeg_decompress );
			return -1;	
	}
	
	return 0;
}

KDint xmGetDecodeJPEG ( KDFile* file, XMImage* image )
{	
	JPEGInfo*		info = (JPEGInfo*) image->info;
	JPEGDecode*		decode = 0;
	KDint32			stride  = 0;

	image->decode = kdCalloc ( 1, sizeof ( JPEGDecode ) );
	decode = (JPEGDecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}
 
	// Step 4: set parameters for decompression 

	//
	// In this example, we don't need to change any of the defaults set by
	// jpeg_read_header(), so we do nothing here.
	//

	// Step 5: Start decompressor 
	jpeg_start_decompress ( &info->jpeg_decompress );

	// 
	// We can ignore the return value since suspension is not possible
	// with the stdio data source.
	//

	// 
	// We may need to do some setup of our own at this point before reading
	// the data.  After jpeg_start_decompress() we have the correct scaled
	// output image dimensions available, as well as the output colormap
	// if we asked for color quantization.
	// In this example, we need to make an output work buffer of the right size.
	// 

	// JSAMPLEs per row in output buffer 
	stride = info->jpeg_decompress.output_width * info->jpeg_decompress.output_components;	
	if ( info->jpeg_decompress.out_color_space == JCS_CMYK )
	{
		image->bpp = 24;
	}

	// Make a one-row-high sample array that will go away when done with image 
	decode->buffer = ( *info->jpeg_decompress.mem->alloc_sarray ) ( ( j_common_ptr ) &info->jpeg_decompress, JPOOL_IMAGE, stride, 1 );
    
	return 0;
}

KDint xmReadRowsJPEG ( KDFile* file, XMImage* image )
{	
	JPEGInfo*		info = (JPEGInfo*) image->info;
	JPEGDecode*		decode = (JPEGDecode*) image->decode;
	KDuint8			k;
	KDint32			i;
	KDint32			src_x;
	KDint32			dst_x;

	if ( info->jpeg_decompress.output_scanline >= info->jpeg_decompress.output_height )
	{
		kdSetError ( KD_EIO );

		return -1;
	}

	//	Step 6: while (scan lines remain to be read) 
	//           jpeg_read_scanlines(...); 

	//
	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	//

	//
	// jpeg_read_scanlines expects an array of pointers to scanlines.
	// Here the array is only one element long, but you could ask for
	// more than one scanline at a time if that's more convenient.
	//
	jpeg_read_scanlines ( &info->jpeg_decompress, decode->buffer, 1 );
	
	switch ( info->jpeg_decompress.out_color_space )
	{
		case JCS_GRAYSCALE :

			kdMemcpy ( image->rows, decode->buffer[ 0 ], image->width );

			break;

		case JCS_CMYK :

			for ( i = 0; i < image->width; i++ )
			{
				dst_x = i * 3;
				src_x = i * 4;

				k = decode->buffer[ 0 ][ src_x + 3 ];
				
				image->rows[ dst_x + 0 ] = k * decode->buffer[ 0 ][ src_x + 2 ] / 255;
				image->rows[ dst_x + 1 ] = k * decode->buffer[ 0 ][ src_x + 1 ] / 255;
				image->rows[ dst_x + 2 ] = k * decode->buffer[ 0 ][ src_x + 0 ] / 255;
			}			

			break;

		case JCS_RGB :

			for ( i = 0; i < image->width; i++ )
			{
				dst_x = i * 3;
				src_x = i * 3;

				image->rows[ dst_x + 0 ] = decode->buffer[ 0 ][ src_x + 2 ];
				image->rows[ dst_x + 1 ] = decode->buffer[ 0 ][ src_x + 1 ];
				image->rows[ dst_x + 2 ] = decode->buffer[ 0 ][ src_x + 0 ];
			}

			break;

		case JCS_YCbCr   :              // Y/Cb/Cr (also known as YUV) 
		case JCS_YCCK    :              // Y/Cb/Cr/K 
		case JCS_UNKNOWN :

			kdSetError ( KD_ENOSPC );	

			return -1;
	}

	return 0;
}

KDint xmFreeImageJPEG ( XMImage* image )
{
	JPEGInfo*		info = (JPEGInfo*) image->info;

	if ( info )
	{
		// Step 7: Finish decompression
        if ( image->decode )
        {
            jpeg_finish_decompress ( &info->jpeg_decompress );
        }
        
		jpeg_destroy_decompress ( &info->jpeg_decompress );

		// Step 8: Release JPEG decompression object 
		// This is an important step since it will release a good deal of memory. 
	}

	return 0;
}

KDint xmSaveImageJPEG ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	struct jpeg_compress_struct   cinfo;
	struct my_error_mgr           jerr;	
	  	
	JSAMPROW  row_pointer[1];
	XMRGB*    row_buffer;
	KDint     row_stride;
	KDFile*	  outfile;   

	KDint     col;
	KDint     row;

	KDuint8	  color8;
	KDuint16  color16;	
	XMRGBA	  color32;

	row_buffer = (XMRGB*) kdMalloc ( sizeof ( XMRGB ) * width );
	if ( !row_buffer )
	{
		return -1;
	}

	outfile = kdFopen ( pathname, "wb" );
	if ( !outfile )
	{
		kdFree ( row_buffer );
		return -1;
	}

	cinfo.err = jpeg_std_error ( &jerr.pub );

	jpeg_create_compress ( &cinfo );

	jpeg_stdio_dest ( &cinfo, outfile );

    cinfo.image_width		= width;
    cinfo.image_height		= height;
    cinfo.input_components	= 3;       
    cinfo.in_color_space	= JCS_RGB;       

	jpeg_set_defaults ( &cinfo );

	jpeg_start_compress ( &cinfo, 1 );

	row_stride = width * 3; 

	for ( row = height - 1; row >= 0; row-- )
	{
		for ( col = 0; col < width; col++ )
		{
			switch ( format )
			{
				case KD_IMAGE_FORMAT_BGRA8888_ATX :

					color32 = *( (XMRGBA*) ( &pixels [ ( width * row + col ) * 4 ] ) );

					row_buffer [ col ].r = color32.b;
					row_buffer [ col ].g = color32.g;
					row_buffer [ col ].b = color32.r;

					break;

				case KD_IMAGE_FORMAT_RGBA8888_ATX :

					color32 = *( (XMRGBA*) ( &pixels [ ( width * row + col ) * 4 ] ) );

					row_buffer [ col ].r = color32.r;
					row_buffer [ col ].g = color32.g;
					row_buffer [ col ].b = color32.b;

					break;

				case KD_IMAGE_FORMAT_RGB888_ATX :

					row_buffer [ col ] = *( (XMRGB*) ( &pixels [ ( width * row + col ) * 3 ] ) );

					break;

				case KD_IMAGE_FORMAT_RGB565_ATX :

					color16 = *( (KDuint16*) ( &pixels [ ( width * row + col ) * 2 ] ) );

					row_buffer [ col ].r = ( color16 >> 8 ) & 0xF8;
					row_buffer [ col ].g = ( color16 >> 3 ) & 0xFC;
					row_buffer [ col ].b = ( color16 << 3 ) & 0xF8;				

					break;

				case KD_IMAGE_FORMAT_RGBA5551_ATX :

					color16 = *( (KDuint16*) ( &pixels [ ( width * row + col ) * 2 ] ) );

					row_buffer [ col ].r = ( color16 >> 8 ) & 0xF8;
					row_buffer [ col ].g = ( color16 >> 3 ) & 0xF8;
					row_buffer [ col ].b = ( color16 << 2 ) & 0xF8;

					break;

				case KD_IMAGE_FORMAT_RGBA4444_ATX :

					color16 = *( (KDuint16*) ( &pixels [ ( width * row + col ) * 2 ] ) );

					row_buffer [ col ].r = ( color16 >> 8 ) & 0xF0;
					row_buffer [ col ].g = ( color16 >> 4 ) & 0xF0;
					row_buffer [ col ].b = ( color16 << 0 ) & 0xF0;

					break;

				case KD_IMAGE_FORMAT_LUMALPHA88_ATX :

					color8 = pixels [ ( width * row + col ) * 2 ];

					row_buffer [ col ].r = color8;
					row_buffer [ col ].g = color8;
					row_buffer [ col ].b = color8;

					break;

				case KD_IMAGE_FORMAT_LUM8_ATX :

					color8 = pixels [ width * row + col ];

					row_buffer [ col ].r = color8;
					row_buffer [ col ].g = color8;
					row_buffer [ col ].b = color8;

					break;
			}	
		}

		row_pointer[0] = (JSAMPROW) row_buffer;
		jpeg_write_scanlines ( &cinfo, row_pointer, 1 );
	}

    jpeg_finish_compress ( &cinfo );
    jpeg_destroy_compress ( &cinfo );

	kdFree ( row_buffer );
	kdFclose ( outfile );

	return 0;
}