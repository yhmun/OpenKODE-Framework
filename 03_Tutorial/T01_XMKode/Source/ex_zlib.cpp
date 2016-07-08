/*
 *
 *      File            ex_zlib.cpp
 *      Description     usage example of the zlib compression library
 *      Version         version 1.2.5, April 19th, 2010
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright ( C ) 1995-2010 Jean-loup Gailly and Mark Adler
 * 
 *      Jean-loup Gailly        Mark Adler
 *      jloup@gzip.org          madler@alumni.caltech.edu	
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright ( C ) 2010 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
	
 * 
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
 */

#include <XMZlib/zlib.h>

#define CHECK_ERR( err, msg ) {                              \
    if ( err != Z_OK ) {                                     \
        kdLogMessagefKHR ( " * %s error: %d\n", msg, err );  \
        kdExit ( 1 );                                        \
    }                                                        \
}

const char  hello[] = "hello, hello!";
const char  dictionary[] = "hello";
      uLong dictId; 

void test_compress      ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen );
void test_gzio          ( const char* fname, Byte* uncompr, uLong uncomprLen );
void test_deflate       ( Byte* compr, uLong comprLen );
void test_inflate       ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen );
void test_large_deflate ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen );
void test_large_inflate ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen );
void test_flush         ( Byte* compr, uLong* comprLen );
void test_sync          ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen );
void test_dict_deflate  ( Byte* compr, uLong comprLen );
void test_dict_inflate  ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen );

/* ===========================================================================
 * Test compress ( ) and uncompress ( )
 */
void test_compress ( Byte *compr, uLong comprLen, Byte* uncompr, uLong uncomprLen )
{
    int err;
    uLong len = ( uLong ) kdStrlen ( hello )+1;

    err = compress ( compr, &comprLen, ( const Bytef* ) hello, len );
    CHECK_ERR ( err, "compress" );

    kdStrcpy ( ( char* ) uncompr, "garbage" );

    err = uncompress ( uncompr, &uncomprLen, compr, comprLen );
    CHECK_ERR ( err, "uncompress" );

    if ( kdStrcmp ( ( char* ) uncompr, hello ) )
	{
        kdLogMessagefKHR ( " * bad uncompress\n" );
        kdExit ( 1 );
    } else
	{
        kdLogMessagefKHR ( " * uncompress      : %s\n", ( char * ) uncompr );
    }
}

/* ===========================================================================
 * Test read/write of .gz files
 */
void test_gzio ( const char* fname, Byte* uncompr, uLong uncomprLen )
{
#ifdef NO_GZCOMPRESS
    kdLogMessagefKHR ( "NO_GZCOMPRESS -- gz* functions cannot compress\n" );
#else

    int len = ( int ) kdStrlen ( hello ) + 1;
    gzFile file;
    z_off_t pos;

    file = gzopen ( fname, "wb" );
    if ( file == KD_NULL ) 
	{
        kdLogMessagefKHR ( " * gzopen error\n" );
        kdExit ( 1 );
    }
    gzputc ( file, 'h' );
    if ( gzputs ( file, "ello" ) != 4 )
	{
        kdLogMessagefKHR ( " * gzputs err\n" );
        kdExit ( 1 );
    }
    if ( gzprintf ( file, ", %s!", "hello" ) != 8 ) 
	{
        kdLogMessagefKHR ( " * gzprintf err\n" );
        kdExit ( 1 );
    }
    gzseek ( file, 1L, KD_SEEK_CUR );
    gzclose ( file );

    file = gzopen ( fname, "rb" );
    if ( file == KD_NULL )
	{
        kdLogMessagefKHR ( " * gzopen error\n" );
        kdExit ( 1 );
    }
    kdStrcpy ( ( char* ) uncompr, "garbage" );

    if ( gzread ( file, uncompr, ( unsigned ) uncomprLen ) != len ) 
	{
        kdLogMessagefKHR ( " * gzread err\n" );
        kdExit ( 1 );
    }
    if ( kdStrcmp ( ( char* ) uncompr, hello ) ) 
	{
        kdLogMessagefKHR ( " * bad gzread: %s\n", ( char* ) uncompr );
        kdExit ( 1 );
    } else 
	{
        kdLogMessagefKHR ( " * gzread          : %s\n", ( char* ) uncompr );
    }

    pos = gzseek ( file, -8L, KD_SEEK_CUR );
    if ( pos != 6 || gztell ( file ) != pos )
	{
        kdLogMessagefKHR ( " * gzseek error, pos = %ld, gztell = %ld\n", ( long ) pos, ( long ) gztell ( file ) );
        kdExit ( 1 );
    }

    if ( gzgetc ( file ) != ' ' )
	{
        kdLogMessagefKHR ( " * gzgetc error\n" );
        kdExit ( 1 );
    }

    if ( gzungetc ( ' ', file ) != ' ' )
	{
        kdLogMessagefKHR ( " * gzungetc error\n" );
        kdExit ( 1 );
    }

    gzgets ( file, ( char* ) uncompr, ( int ) uncomprLen );
    if ( kdStrlen ( ( char* ) uncompr ) != 7 ) 
	{ 
        kdLogMessagefKHR ( " * gzgets err after gzseek\n" );
        kdExit ( 1 );
    }
    if ( kdStrcmp ( ( char* )uncompr, hello + 6 ) )
	{
        kdLogMessagefKHR ( " * bad gzgets after gzseek\n" );
        kdExit ( 1 );
    } 
	else
	{
        kdLogMessagefKHR ( " * gzgets after gzseek : %s\n", ( char* ) uncompr );
    }

    gzclose ( file );
#endif
}

/* ===========================================================================
 * Test deflate ( ) with small buffers
 */
void test_deflate ( Byte *compr, uLong comprLen )
{
    z_stream c_stream; /* compression stream */
    int err;
    uLong len = ( uLong ) kdStrlen ( hello ) + 1;

    c_stream.zalloc = ( alloc_func ) 0;
    c_stream.zfree = ( free_func ) 0;
    c_stream.opaque = ( voidpf ) 0;

    err = deflateInit ( &c_stream, Z_DEFAULT_COMPRESSION );
    CHECK_ERR ( err, "deflateInit" );

    c_stream.next_in  = ( Bytef* ) hello;
    c_stream.next_out = compr;

    while ( c_stream.total_in != len && c_stream.total_out < comprLen ) 
	{
        c_stream.avail_in = c_stream.avail_out = 1; /* force small buffers */
        err = deflate ( &c_stream, Z_NO_FLUSH );
        CHECK_ERR ( err, "deflate" );
    }
    /* Finish the stream, still forcing small buffers: */
    for ( ;; ) 
	{
        c_stream.avail_out = 1;
        err = deflate ( &c_stream, Z_FINISH );
        if ( err == Z_STREAM_END ) 
		{
			break;
		}
        CHECK_ERR ( err, "deflate" );
    }

    err = deflateEnd ( &c_stream );
    CHECK_ERR ( err, "deflateEnd" );
}

/* ===========================================================================
 * Test inflate ( ) with small buffers
 */
void test_inflate ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen )
{
    int err;
    z_stream d_stream; /* decompression stream */

    kdStrcpy ( ( char* ) uncompr, "garbage" );

    d_stream.zalloc = ( alloc_func ) 0;
    d_stream.zfree  = ( free_func ) 0;
    d_stream.opaque = ( voidpf ) 0;

    d_stream.next_in  = compr;
    d_stream.avail_in = 0;
    d_stream.next_out = uncompr;

    err = inflateInit ( &d_stream );
    CHECK_ERR ( err, "inflateInit" );

    while ( d_stream.total_out < uncomprLen && d_stream.total_in < comprLen ) 
	{
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        err = inflate ( &d_stream, Z_NO_FLUSH );
        if ( err == Z_STREAM_END ) 
		{
			break;
		}
        CHECK_ERR ( err, "inflate" );
    }

    err = inflateEnd ( &d_stream );
    CHECK_ERR ( err, "inflateEnd" );

    if ( kdStrcmp ( ( char* )uncompr, hello ) )
	{
        kdLogMessagefKHR ( " * bad inflate\n" );
        kdExit ( 1 );
    }
	else 
	{
		kdLogMessagefKHR ( " * inflate         : %s\n", ( char * ) uncompr );
    }
}

/* ===========================================================================
 * Test deflate ( ) with large buffers and dynamic change of compression level
 */
void test_large_deflate ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen )	
{
    z_stream c_stream; /* compression stream */
    int err;

    c_stream.zalloc = ( alloc_func ) 0;
    c_stream.zfree = ( free_func ) 0;
    c_stream.opaque = ( voidpf ) 0;

    err = deflateInit ( &c_stream, Z_BEST_SPEED );
    CHECK_ERR ( err, "deflateInit" );

    c_stream.next_out = compr;
    c_stream.avail_out = ( uInt )comprLen;

    /* At this point, uncompr is still mostly zeroes, so it should compress
     * very well:
     */
    c_stream.next_in = uncompr;
    c_stream.avail_in = ( uInt ) uncomprLen;
    err = deflate ( &c_stream, Z_NO_FLUSH );
    CHECK_ERR ( err, "deflate" );
    if ( c_stream.avail_in != 0 )
	{
        kdLogMessagefKHR ( " * deflate not greedy\n" );
        kdExit ( 1 );
    }

    /* Feed in already compressed data and switch to no compression: */
    deflateParams ( &c_stream, Z_NO_COMPRESSION, Z_DEFAULT_STRATEGY );
    c_stream.next_in = compr;
    c_stream.avail_in = ( uInt ) comprLen / 2;
    err = deflate ( &c_stream, Z_NO_FLUSH );
    CHECK_ERR ( err, "deflate" );

    /* Switch back to compressing mode: */
    deflateParams ( &c_stream, Z_BEST_COMPRESSION, Z_FILTERED );
    c_stream.next_in = uncompr;
    c_stream.avail_in = ( uInt ) uncomprLen;
    err = deflate ( &c_stream, Z_NO_FLUSH );
    CHECK_ERR ( err, "deflate" );

    err = deflate ( &c_stream, Z_FINISH );
    if ( err != Z_STREAM_END ) 
	{
        kdLogMessagefKHR ( " * deflate should report Z_STREAM_END\n" );
        kdExit ( 1 );
    }
    err = deflateEnd ( &c_stream );
    CHECK_ERR ( err, "deflateEnd" );
}

/* ===========================================================================
 * Test inflate ( ) with large buffers
 */
void test_large_inflate ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen )
{
    int err;
    z_stream d_stream; /* decompression stream */

    kdStrcpy ( ( char* ) uncompr, "garbage" );

    d_stream.zalloc = ( alloc_func ) 0;
    d_stream.zfree = ( free_func ) 0;
    d_stream.opaque = ( voidpf ) 0;

    d_stream.next_in  = compr;
    d_stream.avail_in = ( uInt )comprLen;

    err = inflateInit ( &d_stream );
    CHECK_ERR ( err, "inflateInit" );

    for ( ;; ) 
	{
        d_stream.next_out = uncompr;            /* discard the output */
        d_stream.avail_out = ( uInt ) uncomprLen;
        err = inflate ( &d_stream, Z_NO_FLUSH );
        if ( err == Z_STREAM_END ) 
		{
			break;
		}
        CHECK_ERR ( err, "large inflate" );
    }

    err = inflateEnd ( &d_stream );
    CHECK_ERR ( err, "inflateEnd" );

    if ( d_stream.total_out != 2 * uncomprLen + comprLen / 2 ) 
	{
        kdLogMessagefKHR ( " * bad large inflate : %ld\n", d_stream.total_out );
        kdExit ( 1 );
    } else 
	{
        kdLogMessagefKHR ( " * large_inflate   : OK\n" );
    }
}

/* ===========================================================================
 * Test deflate ( ) with full flush
 */
void test_flush ( Byte* compr, uLong* comprLen )
{
    z_stream c_stream; /* compression stream */
    int err;
    uInt len = ( uInt ) kdStrlen ( hello ) + 1;

    c_stream.zalloc = ( alloc_func ) 0;
    c_stream.zfree = ( free_func ) 0;
    c_stream.opaque = ( voidpf ) 0;

    err = deflateInit ( &c_stream, Z_DEFAULT_COMPRESSION );
    CHECK_ERR ( err, "deflateInit" );

    c_stream.next_in  = ( Bytef* ) hello;
    c_stream.next_out = compr;
    c_stream.avail_in = 3;
    c_stream.avail_out = ( uInt ) *comprLen;
    err = deflate ( &c_stream, Z_FULL_FLUSH );
    CHECK_ERR ( err, "deflate" );

    compr[3]++; /* force an error in first compressed block */
    c_stream.avail_in = len - 3;

    err = deflate ( &c_stream, Z_FINISH );
    if ( err != Z_STREAM_END ) 
	{
        CHECK_ERR ( err, "deflate" );
    }
    err = deflateEnd ( &c_stream );
    CHECK_ERR ( err, "deflateEnd" );

    *comprLen = c_stream.total_out;
}

/* ===========================================================================
 * Test inflateSync ( )
 */
void test_sync ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen )
{
    int err;
    z_stream d_stream; /* decompression stream */

    kdStrcpy ( ( char* ) uncompr, "garbage" );

    d_stream.zalloc = ( alloc_func ) 0;
    d_stream.zfree = ( free_func ) 0;
    d_stream.opaque = ( voidpf ) 0;

    d_stream.next_in  = compr;
    d_stream.avail_in = 2; /* just read the zlib header */

    err = inflateInit ( &d_stream );
    CHECK_ERR ( err, "inflateInit" );

    d_stream.next_out = uncompr;
    d_stream.avail_out = ( uInt ) uncomprLen;

    inflate ( &d_stream, Z_NO_FLUSH );
    CHECK_ERR ( err, "inflate" );

    d_stream.avail_in = ( uInt ) comprLen-2;   /* read all compressed data */
    err = inflateSync ( &d_stream );           /* but skip the damaged part */
    CHECK_ERR ( err, "inflateSync" );

    err = inflate ( &d_stream, Z_FINISH );
    if ( err != Z_DATA_ERROR ) 
	{
        kdLogMessagefKHR ( " * inflate should report DATA_ERROR\n" );
        /* Because of incorrect adler32 */
        kdExit ( 1 );
    }
    err = inflateEnd ( &d_stream );
    CHECK_ERR ( err, "inflateEnd" );

    kdLogMessagefKHR ( " * after inflateSync : hel%s\n", ( char * ) uncompr );
}

/* ===========================================================================
 * Test deflate ( ) with preset dictionary
 */
void test_dict_deflate ( Byte* compr, uLong comprLen )
{
    z_stream c_stream; /* compression stream */
    int err;

    c_stream.zalloc = ( alloc_func ) 0;
    c_stream.zfree = ( free_func ) 0;
    c_stream.opaque = ( voidpf ) 0;

    err = deflateInit ( &c_stream, Z_BEST_COMPRESSION );
    CHECK_ERR ( err, "deflateInit" );

    err = deflateSetDictionary ( &c_stream, ( const Bytef* ) dictionary, sizeof ( dictionary ) );
    CHECK_ERR ( err, "deflateSetDictionary" );

    dictId = c_stream.adler;
    c_stream.next_out = compr;
    c_stream.avail_out = ( uInt )comprLen;

    c_stream.next_in = ( Bytef* ) hello;
    c_stream.avail_in = ( uInt ) kdStrlen ( hello ) + 1;

    err = deflate ( &c_stream, Z_FINISH );
    if ( err != Z_STREAM_END )
	{
        kdLogMessagefKHR ( " * deflate should report Z_STREAM_END\n" );
        kdExit ( 1 );
    }
    err = deflateEnd ( &c_stream );
    CHECK_ERR ( err, "deflateEnd" );
}

/* ===========================================================================
 * Test inflate ( ) with a preset dictionary
 */
void test_dict_inflate ( Byte* compr, uLong comprLen, Byte* uncompr, uLong uncomprLen )
{
    int err;
    z_stream d_stream; /* decompression stream */

    kdStrcpy ( ( char* ) uncompr, "garbage" );

    d_stream.zalloc = ( alloc_func ) 0;
    d_stream.zfree = ( free_func ) 0;
    d_stream.opaque = ( voidpf ) 0;

    d_stream.next_in  = compr;
    d_stream.avail_in = ( uInt ) comprLen;

    err = inflateInit ( &d_stream );
    CHECK_ERR ( err, "inflateInit" );

    d_stream.next_out = uncompr;
    d_stream.avail_out = ( uInt ) uncomprLen;

    for ( ;; ) 
	{
        err = inflate ( &d_stream, Z_NO_FLUSH );
        if ( err == Z_STREAM_END ) 
		{
			break;
		}
        if ( err == Z_NEED_DICT ) 
		{
            if ( d_stream.adler != dictId ) 
			{
                kdLogMessagefKHR ( "unexpected dictionary" );
                kdExit ( 1 );
            }
            err = inflateSetDictionary ( &d_stream, ( const Bytef* ) dictionary, sizeof ( dictionary ) );
        }
        CHECK_ERR ( err, "inflate with dict" );
    }

    err = inflateEnd ( &d_stream );
    CHECK_ERR ( err, "inflateEnd" );

    if ( kdStrcmp ( ( char* ) uncompr, hello ) ) 
	{
        kdLogMessagefKHR ( " * bad inflate with dict\n" );
        kdExit ( 1 );
    } 
	else 
	{
        kdLogMessagefKHR ( " * inflate with dictionary : %s\n", ( char * ) uncompr );
    }
}

KDvoid xmExample_ZLib ( KDvoid )
{
    Byte *compr, *uncompr;
    uLong comprLen = 10000 * sizeof ( int ); 
    uLong uncomprLen = comprLen;
    static const char* myVersion = ZLIB_VERSION;
	
    if ( zlibVersion ( )[ 0 ] != myVersion[ 0 ] ) 
	{
        kdLogMessagefKHR ( "zlibVersion        : incompatible zlib version" );
        return;
    } 
	else if ( kdStrcmp ( zlibVersion ( ), ZLIB_VERSION ) != 0 )
	{
		kdLogMessagefKHR ( "zlibVersion        : warning - different zlib version" );
    }

	kdLogMessagefKHR ( "zlibVersion        : %s = 0x%04x, compile flags = 0x%lx", ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags ( ) );	
 
    compr    = ( Byte* ) kdCalloc ( ( uInt ) comprLen, 1 );
    uncompr  = ( Byte* ) kdCalloc ( ( uInt ) uncomprLen, 1 );
  
    if ( compr == Z_NULL || uncompr == Z_NULL ) 
	{
		kdLogMessagefKHR ( "xmExample_ZLib     : out of memory" );
        kdExit ( 1 );
    }

	kdLogMessagefKHR ( "xmExample_ZLib     : test_compress" );
    test_compress ( compr, comprLen, uncompr, uncomprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_gzio" );
    test_gzio ( "/data/foo.gz", uncompr, uncomprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_deflate" );
    test_deflate ( compr, comprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_inflate" );
    test_inflate ( compr, comprLen, uncompr, uncomprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_large_deflate" );
    test_large_deflate ( compr, comprLen, uncompr, uncomprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_large_inflate" );
    test_large_inflate ( compr, comprLen, uncompr, uncomprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_flush" );
    test_flush ( compr, &comprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_sync" );
    test_sync ( compr, comprLen, uncompr, uncomprLen );
    comprLen = uncomprLen;

	kdLogMessagefKHR ( "xmExample_ZLib     : test_dict_deflate" );
    test_dict_deflate ( compr, comprLen );

	kdLogMessagefKHR ( "xmExample_ZLib     : test_dict_inflate" );
    test_dict_inflate ( compr, comprLen, uncompr, uncomprLen );

    kdFree ( compr );
    kdFree ( uncompr );
}
