/* --------------------------------------------------------------------------
 *
 *      File            atx_imgdec_png.c
 *      Description     The ability to decode PNG images to the ATX_imgdec extension. 
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

#include "PNG/png.h"
#include "PNG/pngstruct.h"
#include "PNG/pnginfo.h"

#define PNG_BYTES_TO_CHECK	 8

typedef struct 
{
	_png_struct*     ptr_struct;
	_png_info*       ptr_info;
} PNGInfo;

typedef struct 
{
	KDuint8*         pixels;
	_png_uint_32     row_count;
} PNGDecode;

static KDchar       l_last_error[256] = "";

static KDvoid PNGAPI xmReadData ( _png_structp _png_ptr, _png_bytep data, _png_size_t length )
{
	KDFile*		file = 0;
	
	file = (KDFile *) _png_get_io_ptr ( _png_ptr );
	if ( !file || kdFread ( data, (KDsize) length, 1, file ) == 0 )
	{
		//_png_error ( _png_ptr, "Read Error" );
	}	
}

static KDvoid PNGAPI xmWriteData ( _png_structp _png_ptr, _png_bytep data, _png_size_t length )
{
	KDFile*		file = 0;
	
	file = (KDFile *) _png_get_io_ptr ( _png_ptr );
	if ( !file || kdFwrite ( data, (KDsize) length, 1, file ) == 0 )
	{
		//_png_error ( _png_ptr, "Write Error" );
	}	
}

static KDvoid PNGAPI xmFlush ( _png_structp _png_ptr ) 
{

} 

static KDvoid PNGAPI xmError ( _png_structp _png_ptr, _png_const_charp error_msg )
{
	kdStrncpy_s ( (KDchar *) _png_ptr->error_ptr, 255, error_msg, 255 );

	#if defined ( PNG_SETJMP_SUPPORTED )

	longjmp ( _png_ptr->longjmp_buffer, 1 );

	#endif
}

KDint xmGetInfoPNG ( KDFile* file, XMImage* image )
{
	PNGInfo*		info;
	KDubyte			header [8];
	_png_struct*	ptr_struct;
	_png_info*		ptr_info;

	image->info = kdCalloc ( 1, sizeof ( PNGInfo ) );
	info = (PNGInfo*) image->info;
	if ( !info )
	{
		return -1;
	}

	// Read in some of the signature bytes 
	if ( kdFread ( header, PNG_BYTES_TO_CHECK, 1, file ) != 1 )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	// 
	// Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
	// Return nonzero (true) if they match 
	//
	if ( _png_sig_cmp ( header, (_png_size_t) 0, PNG_BYTES_TO_CHECK ) )
	{
		kdSetError ( KD_EILSEQ );
		return -1;
	}

	image->type = KD_IMAGE_TYPE_PNG;

	//
	// Create and initialize the _png_struct with the desired error handler
	// functions.  If you want to use the default stderr and longjump method,
	// you can supply NULL for the last three parameters.  We also supply the
	// the compiler header file version, so that we know if the application
	// was compiled with a compatible version of the library.  REQUIRED    
	//		
	if( ( ptr_struct = info->ptr_struct = _png_create_read_struct ( PNG_LIBPNG_VER_STRING, (KDvoid *) NULL, NULL, NULL ) ) == NULL )
	{
		kdSetError ( KD_ENOMEM );
		return -1;
	}

	// Allocate/initialize the memory for image information.  REQUIRED. 
	if ( ( ptr_info = info->ptr_info = _png_create_info_struct ( info->ptr_struct ) ) == NULL )
	{		
		kdSetError ( KD_ENOMEM );

		return -1;
	}

	//
	// Set error handling if you are using the setjmp/longjmp method (this is
	// the normal method of doing things with libpng).  REQUIRED unless you
	// set up your own error handlers in the _png_create_read_struct() earlier.
	//
	#if defined ( PNG_SETJMP_SUPPORTED )

	if ( setjmp ( _png_jmpbuf ( info->ptr_struct ) ) )
	{
		kdSetError ( KD_ENOMEM );

		return -1;
	}

	#endif

	// One of the following I/O initialization methods is REQUIRED 
	// Set up the input control if you are using standard C streams 
	// _png_init_io(_png_ptr, (FILE*) file);

	// use custom I/O functions
	_png_set_read_fn  ( ptr_struct, file, xmReadData );
	_png_set_error_fn ( ptr_struct, l_last_error, xmError, NULL );

	// If we have already read some of the signature
	_png_set_sig_bytes ( ptr_struct, PNG_BYTES_TO_CHECK );

	// 
	// The call to _png_read_info() gives us all of the information from the
	// PNG file before the first IDAT (image data chunk).  REQUIRED
	//
	_png_read_info ( ptr_struct, ptr_info );
	
	image->bpp    = ptr_info->pixel_depth;
	image->width  = ptr_info->width;
	image->height = ptr_info->height;
	image->flip_y = 0;

	switch ( ptr_info->pixel_depth )
	{
		case 64 :

			image->format = KD_IMAGE_FORMAT_RGBA_ATX;
			image->alpha  = 16;
			break;

		case 48 :

			image->format = KD_IMAGE_FORMAT_RGB_ATX;
			image->alpha  = 0;
			break;

		case 32 :

			if ( ptr_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
			{
				image->format = KD_IMAGE_FORMAT_LUMALPHA_ATX;
				image->alpha  = 16;
			}
			else
			{
				image->format = KD_IMAGE_FORMAT_RGBA_ATX;
				image->alpha  = 8;
			}
			break;

		case 16 :

			image->format = KD_IMAGE_FORMAT_LUMALPHA_ATX;
			image->alpha  = 8;
			break;

		case 24 :

			image->format = KD_IMAGE_FORMAT_RGB_ATX;
			image->alpha  = 0;
			break;

		case 8 :
		case 4 :
		case 2 :
		case 1 :

			image->format = ptr_info->num_palette == 0 ? KD_IMAGE_FORMAT_LUMINANCE_ATX : KD_IMAGE_FORMAT_PALETTED_ATX;
			image->alpha  = 0;
			break;
	}	
	
	return 0;
}

KDint xmGetDecodePNG ( KDFile* file, XMImage* image )
{
	PNGInfo*		info = (PNGInfo*) image->info;
	PNGDecode*		decode;
	_png_struct*	ptr_struct;
	_png_info*		ptr_info;
	_png_uint_32	pass_num = 0;
	_png_uint_32	pass     = 0;
	_png_uint_32	row      = 0;
	KDint			i;

	ptr_struct	= info->ptr_struct;
	ptr_info	= info->ptr_info;

	if ( ptr_info->color_type & PNG_COLOR_MASK_COLOR )
	{
		_png_set_bgr ( ptr_struct );
	}

	if ( !ptr_struct->interlaced && !ptr_struct->num_palette )
	{		
		return 0;
	}

	image->decode = kdCalloc ( 1, sizeof ( PNGDecode ) );
	decode = (PNGDecode*) image->decode;
	if ( !decode )
	{
		return -1;
	}

	for ( i = 0; i < ptr_struct->num_palette; i++ )
	{
		image->palette[ i ].r = ptr_info->palette[ i ].red;
		image->palette[ i ].g = ptr_info->palette[ i ].green;
		image->palette[ i ].b = ptr_info->palette[ i ].blue;
		image->palette[ i ].a = i < ptr_info->num_trans ? ptr_info->trans_alpha [ i ] : 255;
	}

	if ( info->ptr_struct->interlaced )
	{		
		decode->pixels = (KDuint8 *) kdMalloc ( ptr_info->rowbytes * ptr_info->height );
		if( !decode->pixels )
		{
			return -1;
		}

		pass_num = _png_set_interlace_handling ( ptr_struct );
		decode->row_count = 0;

		for ( pass = 0; pass < pass_num; pass++ )
		{
			for ( row = 0; row < ptr_info->height; row++ )
			{	
				_png_read_row ( ptr_struct, &decode->pixels[ row * ptr_info->rowbytes ], NULL );
			}
		}
	}

	return 0;
}

KDint xmReadRowsPNG ( KDFile* file, XMImage* image )
{
	PNGInfo*		info = (PNGInfo*) image->info;
	PNGDecode*		decode = (PNGDecode*) image->decode;

	if ( info->ptr_struct->interlaced )
	{
		kdMemcpy ( image->rows, &decode->pixels[ decode->row_count * info->ptr_info->rowbytes ], info->ptr_info->rowbytes );	
		decode->row_count++;
	}
	else
	{
		_png_read_row ( info->ptr_struct, image->rows, NULL);
	}	

	return 0;
}

KDint xmFreeImagePNG ( XMImage* image )
{
	PNGInfo*		info = (PNGInfo*) image->info;
	PNGDecode*		decode = (PNGDecode*) image->decode;

	if ( decode && decode->pixels )
	{
		kdFree ( decode->pixels );
	}

	if ( info )
	{
		if ( info->ptr_info )
		{
			_png_destroy_read_struct( &info->ptr_struct, &info->ptr_info, KD_NULL );
		}
		else if ( info->ptr_struct )
		{
			_png_destroy_read_struct( &info->ptr_struct, KD_NULL, KD_NULL );
		}
	}

	return 0;
}

KDint xmSaveImagePNG ( const KDchar* pathname, KDint width, KDint height, KDint format, KDubyte* pixels )
{
	_png_structp	 _png_ptr;
	_png_infop		info_ptr;
	_png_colorp		palette;

	KDubyte*		row_buffer;
	KDFile*			outfile;   

	KDint			col;
	KDint			row;

	KDuint8			color8;
	KDuint16		color16;	
	XMRGB			color24;
	XMRGBA			color32;

	KDbool			has_alpha;
	KDubyte			img_bytes;

	switch ( format )
	{
		case KD_IMAGE_FORMAT_BGRA8888_ATX :
		case KD_IMAGE_FORMAT_RGBA8888_ATX :
		case KD_IMAGE_FORMAT_RGBA5551_ATX :
		case KD_IMAGE_FORMAT_RGBA4444_ATX :
		case KD_IMAGE_FORMAT_LUMALPHA88_ATX :
			has_alpha = KD_TRUE;
			img_bytes = 4;
			break;

		default :
			has_alpha = KD_FALSE;
			img_bytes = 3;
			break;

	}

	outfile = kdFopen ( pathname, "wb" );
	if ( !outfile )
	{
		return -1;
	}

	_png_ptr = _png_create_write_struct ( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if ( !_png_ptr )
	{
		return -1;
	}

	info_ptr = _png_create_info_struct ( _png_ptr );
	if ( !info_ptr )
	{
		kdFclose ( outfile );
		_png_destroy_write_struct ( &_png_ptr, 0 );
		return -1;
	}

#if defined ( PNG_SETJMP_SUPPORTED )
    if ( setjmp ( _png_jmpbuf ( _png_ptr ) ) )
    {
        kdFclose ( outfile );
        _png_destroy_write_struct ( &_png_ptr, &info_ptr );
        return -1;
    }
#endif

	_png_set_write_fn ( _png_ptr, outfile, xmWriteData, xmFlush );
	_png_set_error_fn ( _png_ptr, l_last_error, xmError, 0 );

	if ( has_alpha )
	{
		_png_set_IHDR ( _png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );
	}
	else
	{
		_png_set_IHDR ( _png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );
	}

//	palette = (_png_colorp) _png_malloc ( _png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof ( _png_color ) );
//	_png_set_PLTE ( _png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH );

	_png_write_info ( _png_ptr, info_ptr );

	_png_set_packing ( _png_ptr );

	row_buffer = (KDubyte*) kdMalloc ( width * img_bytes );
    if ( !row_buffer )
    {
        kdFclose ( outfile );
        _png_destroy_write_struct ( &_png_ptr, &info_ptr );
        return -1;
    }

	for ( row = height - 1; row >= 0; row-- )
	{
		for ( col = 0; col < width; col++ )
		{
			switch ( format )
			{
				case KD_IMAGE_FORMAT_BGRA8888_ATX :

					color32 = *( (XMRGBA*) ( &pixels [ ( width * row + col ) * 4 ] ) );

					row_buffer [ col * img_bytes + 0 ] = color32.b;
					row_buffer [ col * img_bytes + 1 ] = color32.g;
					row_buffer [ col * img_bytes + 2 ] = color32.r;
					row_buffer [ col * img_bytes + 3 ] = color32.a;

					break;

				case KD_IMAGE_FORMAT_RGBA8888_ATX :

					color32 = *( (XMRGBA*) ( &pixels [ ( width * row + col ) * 4 ] ) );

					row_buffer [ col * img_bytes + 0 ] = color32.r;
					row_buffer [ col * img_bytes + 1 ] = color32.g;
					row_buffer [ col * img_bytes + 2 ] = color32.b;
					row_buffer [ col * img_bytes + 3 ] = color32.a;

					break;

				case KD_IMAGE_FORMAT_RGB888_ATX :

					color24 = *( (XMRGB*) ( &pixels [ ( width * row + col ) * 3 ] ) );

					row_buffer [ col * img_bytes + 0 ] = color24.r;
					row_buffer [ col * img_bytes + 1 ] = color24.g;
					row_buffer [ col * img_bytes + 2 ] = color24.b;

					break;

				case KD_IMAGE_FORMAT_RGB565_ATX :

					color16 = *( (KDuint16*) ( &pixels [ ( width * row + col ) * 2 ] ) );

					row_buffer [ col * img_bytes + 0 ] = ( color16 >> 8 ) & 0xF8;
					row_buffer [ col * img_bytes + 1 ] = ( color16 >> 3 ) & 0xFC;
					row_buffer [ col * img_bytes + 2 ] = ( color16 << 3 ) & 0xF8;				

					break;

				case KD_IMAGE_FORMAT_RGBA5551_ATX :

					color16 = *( (KDuint16*) ( &pixels [ ( width * row + col ) * 2 ] ) );

					row_buffer [ col * img_bytes + 0 ] = ( color16 >> 8 ) & 0xF8;
					row_buffer [ col * img_bytes + 1 ] = ( color16 >> 3 ) & 0xF8;
					row_buffer [ col * img_bytes + 2 ] = ( color16 << 2 ) & 0xF8;
					row_buffer [ col * img_bytes + 3 ] = ( color16 << 7 ) & 0x80;

					break;

				case KD_IMAGE_FORMAT_RGBA4444_ATX :

					color16 = *( (KDuint16*) ( &pixels [ ( width * row + col ) * 2 ] ) );

					row_buffer [ col * img_bytes + 0 ] = ( color16 >> 8 ) & 0xF0;
					row_buffer [ col * img_bytes + 1 ] = ( color16 >> 4 ) & 0xF0;
					row_buffer [ col * img_bytes + 2 ] = ( color16 << 0 ) & 0xF0;
					row_buffer [ col * img_bytes + 3 ] = ( color16 << 4 ) & 0xF0;  

					break;

				case KD_IMAGE_FORMAT_LUMALPHA88_ATX :

					color8 = pixels [ ( width * row + col ) * 2 ];

					row_buffer [ col * img_bytes + 0 ] = color8;
					row_buffer [ col * img_bytes + 1 ] = color8;
					row_buffer [ col * img_bytes + 2 ] = color8;
					row_buffer [ col * img_bytes + 3 ] = pixels [ ( width * row + col ) * 2 + 1 ];

					break;

				case KD_IMAGE_FORMAT_LUM8_ATX :

					color8 = pixels [ width * row + col ];

					row_buffer [ col * img_bytes + 0 ] = color8;
					row_buffer [ col * img_bytes + 1 ] = color8;
					row_buffer [ col * img_bytes + 2 ] = color8;

					break;
			}	
		}

		_png_write_row ( _png_ptr, row_buffer );
	}

    _png_write_end ( _png_ptr, info_ptr );

    //_png_free ( _png_ptr, palette );
    palette = KD_NULL;

    _png_destroy_write_struct ( &_png_ptr, &info_ptr );

	kdFree ( row_buffer );
	kdFclose ( outfile );

	return 0;
}
