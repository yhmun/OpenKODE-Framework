/* --------------------------------------------------------------------------
 *
 *      File            ex_imgdec.cpp
 *      Description     ATX Image decoder Example
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#include "main.h"

#define XM_TEST_MEMFILE  0
#define XM_TEST_SAVEFILE 1

static const KDchar	l_img_path[][128] =
{			
	"Atitc/test_256x256_ATC_RGB_mipmaps.ktx",					// 0
	"Atitc/test_256x256_ATC_RGBA_Explicit_mipmaps.ktx",			// 1
	"Atitc/test_256x256_ATC_RGBA_Interpolated_mipmaps.ktx",		// 2

	"S3TC/test_256x256_s3tc_dxt1_mipmaps.dds",					// 0
	"S3TC/test_256x256_s3tc_dxt3_mipmaps.dds",					// 1
	"S3TC/test_256x256_s3tc_dxt5_mipmaps.dds",					// 2
	"S3TC/water_2_dxt1.dds",									// 3
	"S3TC/water_2_dxt3.dds",									// 4
	"S3TC/water_2_dxt5.dds",									// 5
	"S3TC/ufo.dds",												// 6
	"S3TC/floor.dds",											// 7
	"S3TC/stone34.dds",											// 8
	"S3TC/synbr05.dds",											// 9	

	"Pvr/test_image.pvr",										// 0
	"Pvr/test_image_a8.pvr",									// 1
	"Pvr/test_image_a8_v3.pvr",									// 2
	"Pvr/test_image_ai88.pvr",									// 3
	"Pvr/test_image_ai88_v3.pvr",								// 4
	"Pvr/test_image_bgra8888.pvr",								// 5
	"Pvr/test_image_bgra8888_v3.pvr",							// 6
	"Pvr/test_image_i8.pvr",									// 7
	"Pvr/test_image_i8_v3.pvr",									// 8
	"Pvr/test_image_pvrtc2bpp.pvr",								// 9
	"Pvr/test_image_pvrtc2bpp_v3.pvr",							// 10
	"Pvr/test_image_pvrtc4bpp.pvr",								// 11
	"Pvr/test_image_pvrtc4bpp_v3.pvr",							// 12
	"Pvr/test_image_rgb565.pvr",								// 13
	"Pvr/test_image_rgb565_v3.pvr",								// 14
	"Pvr/test_image_rgb888.pvr",								// 15
	"Pvr/test_image_rgb888_v3.pvr",								// 16
	"Pvr/test_image_rgba4444.pvr",								// 17
	"Pvr/test_image_rgba4444_mipmap.pvr",						// 18
	"Pvr/test_image_rgba4444_v3.pvr",							// 19
	"Pvr/test_image_rgba5551.pvr",								// 20
	"Pvr/test_image_rgba5551_v3.pvr",							// 21
	"Pvr/test_image_rgba8888.pvr",								// 22
	"Pvr/test_image_rgba8888_v3.pvr",							// 23

	"Pvr/fire_rgba8888.pvr",									// 24
	"Pvr/grossini_pvr_rgba8888.pvr",							// 25
	"Pvr/grossini_pvr_rgba4444.pvr",							// 26
	"Pvr/grossini_pvr_rgba8888.pvr",							// 27
	"Pvr/grossinis_sister1-testalpha_nopremult.pvr",			// 28
	"Pvr/grossinis_sister1-testalpha_premult.pvr",				// 29
	"Pvr/grossini_128x256_mipmap.pvr",							// 30
	
	"Pvr/logo-mipmap.pvr",										// 31
	"Pvr/logo-nomipmap.pvr",									// 32
   	"Pvr/OGL_BGRA_8888.pvr",									// 33
	"Pvr/OGL_RGBA_8888.pvr",									// 34
	"Pvr/OGL_RGB_888.pvr",										// 35	
	"Pvr/OGL_AI_88.pvr",										// 36
	"Pvr/OGL_RGB_565.pvr",										// 37	
	"Pvr/GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG.pvr",				// 38	
	"Pvr/GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG.pvr",				// 39

//	"Pvr/test_1021x1024_a8.pvr",								// 40
//	"Pvr/test_1021x1024_rgb888.pvr",							// 41
//	"Pvr/test_1021x1024_rgba4444.pvr",							// 42
//	"Pvr/test_1021x1024_rgba8888.pvr",							// 43

//	"Pvr/test_image-bad_encoding.pvr",							// 44 ¿¡·¯
//	"Pvr/test_image_pvrtcii2bpp_v3.pvr",						// 45 Not Support
//	"Pvr/test_image_pvrtcii4bpp_v3.pvr",						// 46 Not Support

	"Webp/test.webp",											// 0
	"Webp/test_image.webp",										// 1
	"Webp/webp_a.webp",											// 2

	"Tiff/minisblack-1c-8b.tiff",								// 0
	"Tiff/minisblack-1c-16b.tiff",								// 1
	"Tiff/minisblack-2c-8b-alpha.tiff",							// 2
	"Tiff/miniswhite-1c-1b.tiff",								// 3
	"Tiff/palette-1c-1b.tiff",									// 4
	"Tiff/palette-1c-4b.tiff",									// 5
	"Tiff/palette-1c-8b.tiff",									// 6
	"Tiff/rgb-3c-8b.tiff",										// 7
	"Tiff/rgb-3c-16b.tiff",										// 8

	"Jpeg/baseline_standard.jpg",								// 0
	"Jpeg/baseline_optimize.jpg",								// 1
	"Jpeg/progressive3.jpg",									// 2
	"Jpeg/progressive4.jpg",									// 3
	"Jpeg/progressive5.jpg",									// 4
	"Jpeg/progressive_gray.jpg",								// 5
	"Jpeg/testimg.jpg",											// 6
	"Jpeg/testimgp.jpg",										// 7
	"Jpeg/testorig.jpg",										// 8
	"Jpeg/testprog.jpg",										// 9	

	"Bitmap/argb8888.bmp",										// 0
	"Bitmap/rgb888.bmp",										// 1
	"Bitmap/rgb888_os2.bmp",									// 2
	"Bitmap/rgb888_flip.bmp",									// 3
	"Bitmap/rgb565.bmp",										// 4
	"Bitmap/argb1555.bmp",										// 5
	"Bitmap/argb4444.bmp",										// 6
	"Bitmap/8bit.bmp",											// 7
	"Bitmap/8bit_2.bmp",										// 8
	"Bitmap/8bit_os2.bmp",										// 9
	"Bitmap/8bit_rle.bmp",										// 10
	"Bitmap/8bit_rle2.bmp",										// 11
	"Bitmap/4bit.bmp",											// 12
	"Bitmap/4bit_2.bmp",										// 13
	"Bitmap/4bit_os2.bmp",										// 14
	"Bitmap/4bit_rle.bmp",										// 15
	"Bitmap/4bit_rle2.bmp",										// 16
	"Bitmap/1bit.bmp",											// 17	
	"Bitmap/Test.bmp",											// 18	

	"Targa/32bit.tga",											// 0
	"Targa/24bit.tga",											// 1
	"Targa/16bit.tga",											// 2
	"Targa/8bit.tga",											// 3	
	"Targa/8bit_mono.tga",										// 4	
	"Targa/32bit_rle.tga",										// 5	
	"Targa/24bit_rle.tga",										// 6	
	"Targa/16bit_rle.tga",										// 7

	"Png/64bit_none.png",										// 0
	"Png/64bit_interlaced.png",									// 1
	"Png/64bit_gray.png",										// 2
	"Png/32bit_none.png",										// 3
	"Png/32bit_interlaced.png",									// 4
	"Png/24bit_none.png",										// 5
	"Png/24bit_interlaced.png",									// 6
	"Png/16bit_none_gray.png",									// 7
	"Png/16bit_interlaced_gray.png",							// 8
	"Png/8bit_none.png",										// 9
	"Png/8bit_interlaced.png",									// 10
	"Png/8bit_none_gray.png",									// 11
	"Png/8bit_interlaced_gray.png",								// 12
	"Png/4bit.png",												// 13
	"Png/1bit.png",												// 14

	"Png/ramp3-50.png",											// 15
	"Png/ramp3-b8-50.png",										// 16
	"Png/ramp3-faux-50.png",									// 17		

	"Png/pnggrad8rgb.png",										// 18
	"Png/pngtest8rgba.png",										// 19
	"Png/pnggrad16rgba.png",									// 20
	"Png/pngtest16rgba.png",									// 21

	"Png/straight1.png",										// 22
	"Png/straight2.png",										// 23
	"Png/straight3.png",										// 24
	"Png/straight4.png",										// 25
	"Png/straight5.png",										// 26
	"Png/straight6.png",										// 27

	"Png/boat1.png",											// 28	
	"Png/baboon.png",											// 29
	"Png/bigpnglogo.png",										// 30
	"Png/Grayscale_8bits_palette_sample_image.png",				// 31
	"Png/plogo.png",											// 32
	"Png/pnglogo.png",											// 33
	"Png/pengbrew.png",											// 34
	"Png/th_barbara.png",										// 35
	"Png/th_goldhill.png",										// 36
	"Png/pngtest.png",											// 37
	"Png/pngnow.png",											// 38
	"Png/pngbar.png",											// 39
	"Png/Coverflow.png",										// 40	    
};

static KDbool xmCheckGLExtension ( const KDchar* str )
{	
	KDchar*     ext  = (KDchar *) glGetString ( GL_EXTENSIONS );	
	KDsize      lstr = kdStrlen ( str );
	KDsize      lext = kdStrlen ( ext );
	KDsize      i;

	for ( i = 0; i < lext; i++ )
	{
		if ( !kdStrncmp ( ext + i , str, lstr ) )
		{
			return KD_TRUE;
		}
		else
		{
			for ( i++; i < lext; i++ )
			{
				if ( ext[ i ] == ' ' )
				{					
					break;
				}
			}
		}
	}

	return KD_FALSE;
}

static KDbyte* l_mem_file = 0;

static KDFile* xmLoadMemFile ( const KDchar* pathname )
{
	KDFile*    file = 0;
	KDsize     size = 0;

	if ( l_mem_file )
	{
		kdFree ( l_mem_file );
		l_mem_file = 0;
	}

	file = kdFopen ( pathname, "rb" );
	kdFseek ( file, 0, KD_SEEK_END );

	size = kdFtell ( file );
	l_mem_file = (KDbyte *) kdMalloc ( size + 1 );

	kdFseek ( file, 0, KD_SEEK_SET );
	kdFread ( l_mem_file, size, 1, file );

	kdFclose ( file );
	
	file = kdFmemopen ( l_mem_file, size, "r" );

	return file;
}

static KDvoid xmUnloadMemFile ( KDFile* file )
{
	if ( l_mem_file )
	{
		kdFree ( l_mem_file );
		l_mem_file = 0;
	}

	kdFclose ( file );
}

KDvoid xmLoadTexture ( KDvoid )
{
	KD_GET_TLS ( KDTLS, tls );
	
	KDFile*         file        = 0;
	KDImageATX      image       = 0;
	KDubyte*        pixels      = 0;
	KDubyte*        clear       = 0;

	KDint           img_bpp     = 0;
	KDint           img_alpha   = 0;
	KDint           img_format  = 0;
	KDint           img_level   = 0;
	KDint           img_offset  = 0;
	KDint           img_size    = 0;
	KDint           img_size_cx = 0;
	KDint           img_size_cy = 0;
	KDint			img_flip_y  = 0;
	KDint			img_premult = 0;
	KDint			img_type    = 0;
	
	GLenum          tex_type    = 0;
	GLenum          tex_format  = 0;
	GLint           tex_size_cx = 1;
	GLint           tex_size_cy = 1;

	KDint           idx_level   = 0;

	KDchar          path[128];

	kdSnprintfKHR ( path, 128, "/res/%s", l_img_path[ tls->index - 1 ] );	
	
#if XM_TEST_MEMFILE == 1
	file  = xmLoadMemFile ( path );
	image = kdGetImageInfoFromStreamATX ( file );
#else
	image = kdGetImageInfoATX ( path );
#endif

	img_size_cx	= kdGetImageIntATX ( image, KD_IMAGE_WIDTH_ATX        );	
	img_size_cy	= kdGetImageIntATX ( image, KD_IMAGE_HEIGHT_ATX       );
	img_format  = kdGetImageIntATX ( image, KD_IMAGE_FORMAT_ATX       );	
	img_bpp     = kdGetImageIntATX ( image, KD_IMAGE_BITSPERPIXEL_ATX );	
	img_alpha   = kdGetImageIntATX ( image, KD_IMAGE_ALPHA_ATX        );	
	img_level   = kdGetImageIntATX ( image, KD_IMAGE_LEVELS_ATX       );
	img_flip_y  = kdGetImageIntATX ( image, KD_IMAGE_FLAG_FLIP_Y_ATX  );
	img_type    = kdGetImageIntATX ( image, KD_IMAGE_TYPE			  );
	img_premult = kdGetImageIntATX ( image, KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA );

	kdFreeImageATX ( image );	

	xmSetQuad ( img_size_cx, img_size_cy, img_flip_y );

	switch ( img_format )
	{
		case KD_IMAGE_FORMAT_COMPRESSED_ATX :
			if ( ( img_type == KD_IMAGE_TYPE_PVR   && !xmCheckGLExtension ( "GL_IMG_texture_compression_pvrtc"    ) ) ||
				 ( img_type == KD_IMAGE_TYPE_S3TC  && !xmCheckGLExtension ( "GL_EXT_texture_compression_s3tc"     ) ) ||
				 ( img_type == KD_IMAGE_TYPE_ATITC && !xmCheckGLExtension ( "GL_AMD_compressed_ATC_texture"       ) ) ||
				 ( img_type == KD_IMAGE_TYPE_ETC   && !xmCheckGLExtension ( "GL_OES_compressed_ETC1_RGB8_texture" ) ) )				 
			{
				img_format = KD_IMAGE_FORMAT_RGBA8888_ATX;
				tex_format = GL_RGBA;
				tex_type   = GL_UNSIGNED_BYTE;
			}
			break;

		case KD_IMAGE_FORMAT_LUMINANCE_ATX :
			img_format = KD_IMAGE_FORMAT_LUM8_ATX;
			tex_format = GL_LUMINANCE;
			tex_type   = GL_UNSIGNED_BYTE;
			break;

		case KD_IMAGE_FORMAT_LUMALPHA_ATX :
			img_format = KD_IMAGE_FORMAT_LUMALPHA88_ATX;
			tex_format = GL_LUMINANCE_ALPHA;
			tex_type   = GL_UNSIGNED_BYTE;
			break;

		case KD_IMAGE_FORMAT_ALPHA8_ATX :
			img_format = KD_IMAGE_FORMAT_ALPHA8_ATX;
			tex_format = GL_ALPHA;
			tex_type   = GL_UNSIGNED_BYTE;
			break;

		case KD_IMAGE_FORMAT_PALETTED_ATX :
			img_format = KD_IMAGE_FORMAT_RGBA5551_ATX;
			tex_format = GL_RGBA;
			tex_type   = GL_UNSIGNED_SHORT_5_5_5_1;
			break;

		case KD_IMAGE_FORMAT_RGB_ATX  : 

			switch ( img_bpp )
			{
				case 24 :
					img_format = KD_IMAGE_FORMAT_RGB888_ATX;
					tex_format = GL_RGB;
					tex_type   = GL_UNSIGNED_BYTE;
					break;

				case 16 :
					img_format = KD_IMAGE_FORMAT_RGB565_ATX;
					tex_format = GL_RGB;
					tex_type   = GL_UNSIGNED_SHORT_5_6_5;
					break;
			}

			break;

		case KD_IMAGE_FORMAT_RGBA_ATX :

			switch( img_bpp )
			{
				case 64 :
				case 32 :
					img_format = KD_IMAGE_FORMAT_RGBA8888_ATX;
					tex_format = GL_RGBA;
					tex_type   = GL_UNSIGNED_BYTE;
					break;

				case 16 :		

					switch( img_alpha )
					{
						case 1 :
							img_format = KD_IMAGE_FORMAT_RGBA5551_ATX;
							tex_format = GL_RGBA;
							tex_type   = GL_UNSIGNED_SHORT_5_5_5_1;
							break;

						case 4 :
							img_format = KD_IMAGE_FORMAT_RGBA4444_ATX;
							tex_format = GL_RGBA;
							tex_type   = GL_UNSIGNED_SHORT_4_4_4_4;
							break;
					}

					break;		
			}
			break;

		default :

			return;
	}

#if XM_TEST_MEMFILE == 1
	kdFseek ( file, 0, KD_SEEK_SET );
	image = kdGetImageFromStreamATX ( file, img_format, 0 );
#else
	image = kdGetImageATX ( path, img_format, 0 );
#endif
	
	pixels = (KDubyte *) kdGetImagePointerATX ( image, KD_IMAGE_POINTER_BUFFER_ATX );
	img_bpp = kdGetImageIntATX ( image, KD_IMAGE_BITSPERPIXEL_ATX );

	if ( img_format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
	{
		switch ( kdGetImageIntATX ( image, KD_IMAGE_FORMAT_ATX ) )
		{
			case KD_IMAGE_FORMAT_PVRTC2_ATX :
				tex_format = img_alpha ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
				break;

			case KD_IMAGE_FORMAT_PVRTC4_ATX :
				tex_format = img_alpha ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				break;

			case KD_IMAGE_FORMAT_DXT1_ATX :
				tex_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				break;

			case KD_IMAGE_FORMAT_DXT3_ATX :
				tex_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				break;

			case KD_IMAGE_FORMAT_DXT5_ATX :
				tex_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				break;

			case KD_IMAGE_FORMAT_ATC_RGB :
				tex_format = GL_ATC_RGB_AMD;
				break;

			case KD_IMAGE_FORMAT_ATC_RGBA_EXPLICIT_ALPHA_AMD :
				tex_format = GL_ATC_RGBA_EXPLICIT_ALPHA_AMD;
				break;

			case KD_IMAGE_FORMAT_ATC_RGBA_INTERPOLATED_ALPHA_AMD :
				tex_format = GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD;
				break;

			case KD_IMAGE_FORMAT_ETC1_ATX :
				tex_format = GL_ETC1_RGB8_OES;
				break;
		}
	}
	else
	{
#if XM_TEST_SAVEFILE == 1
		kdSnprintfKHR ( path, 128, "/data/test_%d.png", tls->index - 1 );	
		kdSaveImageATX ( path, img_size_cx, img_size_cy, img_format, pixels );		
#endif
	}

	glGenTextures ( 1, &tls->tex );
	glBindTexture ( GL_TEXTURE_2D, tls->tex );

	for ( idx_level = 0; idx_level < img_level; idx_level++ )
	{
		img_size_cx = kdGetImageLevelIntATX ( image, KD_IMAGE_WIDTH_ATX       , idx_level );
		img_size_cy = kdGetImageLevelIntATX ( image, KD_IMAGE_HEIGHT_ATX      , idx_level );
		img_offset  = kdGetImageLevelIntATX ( image, KD_IMAGE_BUFFEROFFSET_ATX, idx_level );
		img_size    = kdGetImageLevelIntATX ( image, KD_IMAGE_DATASIZE_ATX    , idx_level );

		if ( img_format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
		{
			glCompressedTexImage2D ( GL_TEXTURE_2D, idx_level, tex_format, img_size_cx, img_size_cy, 0, img_size, (GLvoid*) ( pixels + img_offset ) );
		}
		else
		{
			tex_size_cx = 1;
			tex_size_cy = 1;

			while ( tex_size_cx < img_size_cx ) tex_size_cx = tex_size_cx << 1;
			while ( tex_size_cy < img_size_cy ) tex_size_cy = tex_size_cy << 1;

			if ( tex_size_cx != img_size_cx || tex_size_cy != img_size_cy )
			{
				clear = (KDubyte *) kdCalloc ( tex_size_cx * tex_size_cy * ( img_bpp >> 3 ), 1 );
				kdMemset ( clear, 0, tex_size_cx * tex_size_cy * ( img_bpp >> 3 ) );

				glTexImage2D ( GL_TEXTURE_2D, idx_level, tex_format, tex_size_cx, tex_size_cy, 0, tex_format, tex_type, (GLvoid *) clear );
				glTexSubImage2D ( GL_TEXTURE_2D, idx_level, 0, 0, img_size_cx, img_size_cy, tex_format, tex_type, (GLvoid*) ( pixels + img_offset ) );

				kdFree ( clear );
			}
			else
			{
				glTexImage2D ( GL_TEXTURE_2D, idx_level, tex_format, tex_size_cx, tex_size_cy, 0, tex_format, tex_type, (GLvoid*) ( pixels + img_offset ) );
			}
		}
	}

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, img_level > 1 ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	if ( img_premult )
	{
		glBlendFunc ( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	}
	else
	{
		glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}										

	kdFreeImageATX ( image );

	if ( file )
	{
		xmUnloadMemFile ( file );
	}

	kdLogMessagefKHR ( "* Load Texture      : %s", l_img_path[ tls->index - 1 ] );
}
