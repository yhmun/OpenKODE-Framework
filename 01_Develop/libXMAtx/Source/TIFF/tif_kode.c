/* $Id: tif_unix.c,v 1.23 2012-06-01 21:40:59 fwarmerdam Exp $ */

/*
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

/*
 * TIFF Library UNIX-specific Routines. These are should also work with the
 * Windows Common RunTime Library.
 */

#include "tif_config.h"
#include "tiffiop.h"

typedef struct 
{
	KDFile*			file;
	KDsize			size;
} tiff_file;

static tmsize_t
_tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
{
	tiff_file*  ptr = (tiff_file*) fd;
	return kdFread ( buf, 1, size, ptr->file );
}

static tmsize_t
_tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
{
	tiff_file*  ptr = (tiff_file*) fd;
	return kdFwrite ( buf, 1, size, ptr->file );
}

static uint64
_tiffSeekProc(thandle_t fd, uint64 off, int whence)
{
	tiff_file*  ptr = (tiff_file*) fd;
	kdFseek ( ptr->file, (KDoff) off, (KDfileSeekOrigin)whence );
	return kdFtell ( ptr->file );
}

static int
_tiffCloseProc(thandle_t fd)
{
	kdFree ( fd );
	return 0;
}

static uint64
_tiffSizeProc(thandle_t fd)
{
	tiff_file*  ptr = (tiff_file*) fd;
	return ptr->size;
}

static int
_tiffMapProc(thandle_t fd, void** pbase, toff_t* psize)
{
	(void) fd; (void) pbase; (void) psize;
	return (0);
}

static void
_tiffUnmapProc(thandle_t fd, void* base, toff_t size)
{
	(void) fd; (void) base; (void) size;
}

TIFF* TIFFStreamOpen ( void* stream )
{
	TIFF*		tif;
	KDStat		st;
	tiff_file*	fp;

	fp = (tiff_file*) kdMalloc ( sizeof ( tiff_file ) );
	if ( !fp )
	{
		return 0;
	}

	fp->file = (KDFile*) stream;

	kdFstat ( fp->file, &st );
	fp->size = st.st_size;
	
	tif = TIFFClientOpen("File.Tiff", "r",
	    (thandle_t) fp,
	    _tiffReadProc, _tiffWriteProc,
	    _tiffSeekProc, _tiffCloseProc, _tiffSizeProc,
	    _tiffMapProc, _tiffUnmapProc);

    if ( !tif )
    {
        kdFree ( fp );
    }
	return (tif);
}

TIFF* TIFFOpen(const char* name, const char* mode)
{
	TIFF*		tif;
	KDStat		st;
	tiff_file*	fp;

	fp = (tiff_file*) kdMalloc ( sizeof ( tiff_file ) );
	if ( !fp )
	{
		return 0;
	}

	fp->file = kdFopen ( name, mode );
	if ( !fp->file )
	{
		return 0;
	}

	kdStat ( name, &st );
	fp->size = st.st_size;
	
	tif = TIFFClientOpen(name, mode,
	    (thandle_t) fp,
	    _tiffReadProc, _tiffWriteProc,
	    _tiffSeekProc, _tiffCloseProc, _tiffSizeProc,
	    _tiffMapProc, _tiffUnmapProc);

	return (tif);
}

void*
_TIFFmalloc(tmsize_t s)
{
	return (kdMalloc((size_t) s));
}

void
_TIFFfree(void* p)
{
	kdFree(p);
}

void*
_TIFFrealloc(void* p, tmsize_t s)
{
	return (kdRealloc(p, (size_t) s));
}

void
_TIFFmemset(void* p, int v, tmsize_t c)
{
	kdMemset(p, v, (size_t) c);
}

void
_TIFFmemcpy(void* d, const void* s, tmsize_t c)
{
	kdMemcpy(d, s, (size_t) c);
}

int
_TIFFmemcmp(const void* p1, const void* p2, tmsize_t c)
{
	return (kdMemcmp(p1, p2, (size_t) c));
}

static void
unixWarningHandler(const char* module, const char* fmt, va_list ap)
{
	/*
	if (module != NULL)
		fprintf(stderr, "%s: ", module);
	fprintf(stderr, "Warning, ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
	*/
}
TIFFErrorHandler _TIFFwarningHandler = unixWarningHandler;

static void
unixErrorHandler(const char* module, const char* fmt, va_list ap)
{
	/*
	if (module != NULL)
		fprintf(stderr, "%s: ", module);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
	*/
}
TIFFErrorHandler _TIFFerrorHandler = unixErrorHandler;

/* vim: set ts=8 sts=8 sw=8 noet: */

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * fill-column: 78
 * End:
 */
