/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "precomp.hpp"
#include "grfmt_png.hpp"

#ifdef HAVE_PNG

/****************************************************************************************\
    This part of the file implements PNG codec on base of libpng library,
    in particular, this code is based on example.c from libpng
    (see otherlibs/_graphics/readme.txt for copyright notice)
    and png2bmp sample from libpng distribution (Copyright (C) 1999-2001 MIYASAKA Masaru)
\****************************************************************************************/

#include "png/png.h"
#include <XMZlib/zlib.h>

namespace cv
{

/////////////////////// PngDecoder ///////////////////

PngDecoder::PngDecoder()
{
    m_signature = "\x89\x50\x4e\x47\xd\xa\x1a\xa";
    m_color_type = 0;
    m_png_ptr = 0;
    m_info_ptr = m_end_info = 0;
    m_f = 0;
    m_buf_supported = true;
    m_buf_pos = 0;
}


PngDecoder::~PngDecoder()
{
    close();
}

ImageDecoder PngDecoder::newDecoder() const
{
    return new PngDecoder;
}

void  PngDecoder::close()
{
    if( m_f )
    {
        kdFclose( m_f );
        m_f = 0;
    }

    if( m_png_ptr )
    {
        _png_structp _png_ptr = (_png_structp)m_png_ptr;
        _png_infop info_ptr = (_png_infop)m_info_ptr;
        _png_infop end_info = (_png_infop)m_end_info;
        _png_destroy_read_struct( &_png_ptr, &info_ptr, &end_info );
        m_png_ptr = m_info_ptr = m_end_info = 0;
    }
}


void  PngDecoder::readDataFromBuf( void* __png_ptr, uchar* dst, size_t size )
{
    _png_structp _png_ptr = (_png_structp)__png_ptr;
    PngDecoder* decoder = (PngDecoder*)(_png_get_io_ptr(_png_ptr));
    CV_Assert( decoder );
    const Mat& buf = decoder->m_buf;
    if( decoder->m_buf_pos + size > buf.cols*buf.rows*buf.elemSize() )
    {
        //_png_error(_png_ptr, "PNG input buffer is incomplete");
        return;
    }
    memcpy( dst, &decoder->m_buf.data[decoder->m_buf_pos], size );
    decoder->m_buf_pos += size;
}

static KDvoid PNGAPI xmReadData ( _png_structp _png_ptr, _png_bytep data, _png_size_t length )
{
	KDFile*		file = 0;
	
	file = (KDFile *) _png_get_io_ptr ( _png_ptr );
	if ( !file || kdFread ( data, (KDsize) length, 1, file ) == 0 )
	{
		//_png_error ( _png_ptr, "Read Error" );		
	}	
}

bool  PngDecoder::readHeader()
{
    bool result = false;
    close();

    _png_structp _png_ptr = _png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );

    if( _png_ptr )
    {
        _png_infop info_ptr = _png_create_info_struct( _png_ptr );
        _png_infop end_info = _png_create_info_struct( _png_ptr );

        m_png_ptr = _png_ptr;
        m_info_ptr = info_ptr;
        m_end_info = end_info;
        m_buf_pos = 0;

        if( info_ptr && end_info )
        {
#if !defined SHP
            if( setjmp( _png_jmpbuf( _png_ptr ) ) == 0 )
#endif
            {
                if( !m_buf.empty() )
                    _png_set_read_fn(_png_ptr, this, (_png_rw_ptr)readDataFromBuf );
                else
                {					
                    m_f = kdFopen( m_filename.c_str(), "rb" );
                    if ( m_f )
					{
						_png_set_read_fn  ( _png_ptr, m_f, xmReadData );
                        //_png_init_io( _png_ptr, m_f );
					}
                }

                if( !m_buf.empty() || m_f )
                {
                    _png_uint_32 width, height;
                    int bit_depth, color_type;

                    _png_read_info( _png_ptr, info_ptr );

                    _png_get_IHDR( _png_ptr, info_ptr, &width, &height,
                                  &bit_depth, &color_type, 0, 0, 0 );

                    m_width = (int)width;
                    m_height = (int)height;
                    m_color_type = color_type;
                    m_bit_depth = bit_depth;

                    if( bit_depth <= 8 || bit_depth == 16 )
                    {
                        switch(color_type) 
                        {
                           case PNG_COLOR_TYPE_RGB:
                           case PNG_COLOR_TYPE_PALETTE:
                               m_type = CV_8UC3;
                               break;
                          case PNG_COLOR_TYPE_RGB_ALPHA:
                               m_type = CV_8UC4;
                               break;
                          default:
                               m_type = CV_8UC1;
                        }
                        if( bit_depth == 16 )
                            m_type = CV_MAKETYPE(CV_16U, CV_MAT_CN(m_type));
                        result = true;
                    }
                }
            }
        }
    }

    if( !result )
        close();

    return result;
}


bool  PngDecoder::readData( Mat& img )
{
    bool result = false;
    AutoBuffer<uchar*> _buffer(m_height);
    uchar** buffer = _buffer;
    int color = img.channels() > 1;
    uchar* data = img.data;
    int step = (int)img.step;

    if( m_png_ptr && m_info_ptr && m_end_info && m_width && m_height )
    {
        _png_structp _png_ptr = (_png_structp)m_png_ptr;
        _png_infop info_ptr = (_png_infop)m_info_ptr;
        _png_infop end_info = (_png_infop)m_end_info;

#if !defined SHP
        if( setjmp( _png_jmpbuf ( _png_ptr ) ) == 0 )
#endif
        {
            int y;

            if( img.depth() == CV_8U && m_bit_depth == 16 )
                _png_set_strip_16( _png_ptr );
            else if( !isBigEndian() )
                _png_set_swap( _png_ptr );

            if(img.channels() < 4) 
            {
                /* observation: _png_read_image() writes 400 bytes beyond
                 * end of data when reading a 400x118 color png
                 * "mpplus_sand.png".  OpenCV crashes even with demo
                 * programs.  Looking at the loaded image I'd say we get 4
                 * bytes per pixel instead of 3 bytes per pixel.  Test
                 * indicate that it is a good idea to always ask for
                 * stripping alpha..  18.11.2004 Axel Walthelm
                 */
                 _png_set_strip_alpha( _png_ptr );
            }

            if( m_color_type == PNG_COLOR_TYPE_PALETTE )
                _png_set_palette_to_rgb( _png_ptr );

            if( m_color_type == PNG_COLOR_TYPE_GRAY && m_bit_depth < 8 )
#if PNG_LIBPNG_VER_MAJOR*100 + PNG_LIBPNG_VER_MINOR >= 104
                _png_set_expand_gray_1_2_4_to_8( _png_ptr );
#else
                _png_set_gray_1_2_4_to_8( _png_ptr );
#endif
            
            if( CV_MAT_CN(m_type) > 1 && color )
                _png_set_bgr( _png_ptr ); // convert RGB to BGR
            else if( color )
                _png_set_gray_to_rgb( _png_ptr ); // Gray->RGB
            else
                _png_set_rgb_to_gray( _png_ptr, 1, 0.299, 0.587 ); // RGB->Gray

            _png_read_update_info( _png_ptr, info_ptr );

            for( y = 0; y < m_height; y++ )
                buffer[y] = data + y*step;

            _png_read_image( _png_ptr, buffer );
            _png_read_end( _png_ptr, end_info );

            result = true;
        }
    }

    close();
    return result;
}


/////////////////////// PngEncoder ///////////////////


PngEncoder::PngEncoder()
{
    m_description = "Portable Network Graphics files (*.png)";
    m_buf_supported = true;
}


PngEncoder::~PngEncoder()
{
}


bool  PngEncoder::isFormatSupported( int depth ) const
{
    return depth == CV_8U || depth == CV_16U;
}

ImageEncoder PngEncoder::newEncoder() const
{
    return new PngEncoder;
}


void PngEncoder::writeDataToBuf(void* __png_ptr, uchar* src, size_t size)
{
    if( size == 0 )
        return;
    _png_structp _png_ptr = (_png_structp)__png_ptr;
    PngEncoder* encoder = (PngEncoder*)(_png_get_io_ptr(_png_ptr));
    CV_Assert( encoder && encoder->m_buf );
    size_t cursz = encoder->m_buf->size();
    encoder->m_buf->resize(cursz + size);
    memcpy( &(*encoder->m_buf)[cursz], src, size );
}


void PngEncoder::flushBuf(void*)
{
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

bool  PngEncoder::write( const Mat& img, const vector<int>& params )
{
    int compression_level = 0;
    int compression_strategy = Z_RLE;

    for( size_t i = 0; i < params.size(); i += 2 )
    {
        if( params[i] == CV_IMWRITE_PNG_COMPRESSION )
        {
            compression_level = params[i+1];
            compression_level = MIN(MAX(compression_level, 0), MAX_MEM_LEVEL);
        }
        if( params[i] == CV_IMWRITE_PNG_STRATEGY )
        {
            compression_strategy = params[i+1];
            compression_strategy = MIN(MAX(compression_strategy, 0), Z_FIXED); 
        }
    }

    _png_structp _png_ptr = _png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
    _png_infop info_ptr = 0;
    KDFile* f = 0;
    int y, width = img.cols, height = img.rows;
    int depth = img.depth(), channels = img.channels();
    bool result = false;
    AutoBuffer<uchar*> buffer;

    if( depth != CV_8U && depth != CV_16U )
        return false;

    if( _png_ptr )
    {
        info_ptr = _png_create_info_struct( _png_ptr );

        if( info_ptr )
        {
#if !defined SHP
            if( setjmp( _png_jmpbuf ( _png_ptr ) ) == 0 )
#endif
            {
                if( m_buf )
                {
                    _png_set_write_fn(_png_ptr, this,
                        (_png_rw_ptr)writeDataToBuf, (_png_flush_ptr)flushBuf);
                }
                else
                {
                    f = kdFopen ( m_filename.c_str(), "wb" );
                    if ( f )
					{
						_png_set_write_fn  ( _png_ptr, f, xmWriteData, xmFlush );
                    //    _png_init_io( _png_ptr, f );
					}
                }

                if( m_buf || f )
                {
                    if( compression_level > 0 )
                    {
                        _png_set_compression_mem_level( _png_ptr, compression_level );
                    }
                    else
                    {
                        // tune parameters for speed
                        // (see http://wiki.linuxquestions.org/wiki/Libpng)
                        _png_set_filter(_png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
                        _png_set_compression_level(_png_ptr, Z_BEST_SPEED);
                    }
                    _png_set_compression_strategy(_png_ptr, compression_strategy);

                    _png_set_IHDR( _png_ptr, info_ptr, width, height, depth == CV_8U ? 8 : 16,
                        channels == 1 ? PNG_COLOR_TYPE_GRAY :
                        channels == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
                        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                        PNG_FILTER_TYPE_DEFAULT );

                    _png_write_info( _png_ptr, info_ptr );

                    _png_set_bgr( _png_ptr );
                    if( !isBigEndian() )
                        _png_set_swap( _png_ptr );

                    buffer.allocate(height);
                    for( y = 0; y < height; y++ )
                        buffer[y] = img.data + y*img.step;

                    _png_write_image( _png_ptr, buffer );
                    _png_write_end( _png_ptr, info_ptr );

                    result = true;
                }
            }
        }
    }

    _png_destroy_write_struct( &_png_ptr, &info_ptr );
    if(f) kdFclose ( f );

    return result;
}

}

#endif

/* End of file. */
