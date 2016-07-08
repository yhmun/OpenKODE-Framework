
/*
 * Author: 
 *      Guido Draheim <guidod@gmx.de>
 *      Tomi Ollila <Tomi.Ollila@iki.fi>
 *
 *      Copyright (c) 1999,2000,2001,2002,2003 Guido Draheim
 *          All rights reserved,
 *          use under the restrictions of the
 *          Lesser GNU General Public License
 *          or alternatively the restrictions 
 *          of the Mozilla Public License 1.1
 */

#include <XMZZip/lib.h>           /* exported... */
#include <XMZlib/zlib.h>

#include <string.h>

#include <XMZZip/file.h>

/* *INDENT-OFF* */
static struct errlistentry { int code; const char* mesg; } 
errlist[] = 
{
    { ZZIP_NO_ERROR,        "No error" },
    { ZZIP_OUTOFMEM,        
      "could not get temporary memory for internal structures" },
    { ZZIP_DIR_OPEN,        "Failed to open zip-file %s" },
    { ZZIP_DIR_STAT,        "Failed to fstat zip-file %s" },
    { ZZIP_DIR_SEEK,        "Failed to lseek zip-file %s" },
    { ZZIP_DIR_READ,        "Failed to read zip-file %s"},  
    { ZZIP_DIR_TOO_SHORT,   "zip-file %s too short" },
    { ZZIP_DIR_EDH_MISSING, "zip-file central directory not found" },
    { ZZIP_DIRSIZE,         "Directory size too big..." },
    { ZZIP_ENOENT,          "No such file found in zip-file %s" },
    { ZZIP_UNSUPP_COMPR,    "Unsupported compression format" },
    { ZZIP_CORRUPTED,       "Zipfile corrupted" }, 
    { ZZIP_UNDEF,           "Some undefined error occurred" },
    { ZZIP_DIR_LARGEFILE,   "Directory is largefile variant" },
    { 0, 0 },
};
/* *INDENT-ON* */


#define errlistSIZE (sizeof(errlist)/sizeof(*errlist))

/**
 * returns the static string for the given error code. The
 * error code can be either a normal system error (a
 * positive error code will flag this), it can be => libz
 * error code (a small negative error code will flag this)
 * or it can be an error code from => libzzip, which is an
 * negative value lower than => ZZIP_ERROR
 */
zzip_char_t *
zzip_strerror(int errcode)
{
	static KDchar szError[256];

    if (errcode < ZZIP_ERROR && errcode > ZZIP_ERROR - 32)
    {
        struct errlistentry *err = errlist;

        for (; err->mesg; err++)
        {
            if (err->code == errcode)
                return err->mesg;
        }
        errcode = KD_EINVAL;
    }

    if (errcode < 0)
    {
        if (errcode == -1)
		{			
            kdSprintf ( szError, "Error = %d", kdGetError ( ) );
			return szError;//strerror(errcode);
		}
        else
            return zError(errcode);
    }

	kdSprintf ( szError, "Error = %d", kdGetError ( ) );
    return szError; //strerror(errcode);
}

/** => zzip_strerror
 * This function fetches the errorcode from the => DIR-handle and 
 * runs it through => zzip_strerror to obtain the static string
 * describing the error.
 */
zzip_char_t *
zzip_strerror_of(ZZIP_DIR * dir)
{
	static KDchar szError[256];

    if (! dir)
	{
		kdSprintf ( szError, "Error = %d", kdGetError ( ) );
        return szError;//strerror(errno);
	}
    return zzip_strerror(dir->errcode);
}

/* *INDENT-OFF* */
static struct errnolistentry { int code; int e_no; } 
errnolist[] =
{
	{ Z_STREAM_ERROR, KD_EIO }, //KD_EPIPE },
	{ Z_DATA_ERROR	, KD_EIO }, //KD_ESPIPE },
    { Z_MEM_ERROR	, KD_ENOMEM },
    { Z_BUF_ERROR	, KD_EMFILE },
	{ Z_VERSION_ERROR, KD_EINVAL }, //KD_ENOEXEC },
      
	{ ZZIP_DIR_OPEN, KD_ENOENT }, //KD_ENOTDIR },
	{ ZZIP_DIR_STAT, KD_EIO }, //KD_EREMOTE },
    { ZZIP_DIR_SEEK, KD_EIO }, //KD_ESPIPE },
#  ifdef ESTRPIPE
    { ZZIP_DIR_READ, ESTRPIPE},  
#  else
    { ZZIP_DIR_READ, KD_EIO }, //KD_EPIPE},  
#  endif
	{ ZZIP_DIR_TOO_SHORT, KD_EINVAL }, //KD_ENOEXEC },
#  ifdef ENOMEDIUM
    { ZZIP_DIR_EDH_MISSING, ENOMEDIUM },
#  else
    { ZZIP_DIR_EDH_MISSING, KD_EIO },
#  endif
    { ZZIP_DIRSIZE	, KD_EFBIG },
    { ZZIP_OUTOFMEM	, KD_ENOMEM },
    { ZZIP_ENOENT	, KD_ENOENT },
#  ifdef EPFNOSUPPORT
    { ZZIP_UNSUPP_COMPR, EPFNOSUPPORT },
#  else
    { ZZIP_UNSUPP_COMPR, KD_EACCES },
#  endif 
# ifdef EILSEQ
    { ZZIP_CORRUPTED, EILSEQ }, 
# else
    { ZZIP_CORRUPTED, KD_ELOOP }, 
# endif
    { ZZIP_UNDEF	, KD_EINVAL },
    { 0, 0 },
};    
/* *INDENT-ON* */

/**
 * map the error code to a system error code. This is used
 * for the drop-in replacement functions to return a value
 * that can be interpreted correctly by code sections that
 * are unaware of the fact they their => open(2) call had been
 * diverted to a file inside a zip-archive.
 */
int
zzip_errno(int errcode)
{
    if (errcode >= -1)
    {
        return kdGetError ( );
    } else
    {
        struct errnolistentry *err = errnolist;

        for (; err->code; err++)
        {
            if (err->code == errcode)
                return err->e_no;
        }
        return KD_EINVAL;
    }
}

/* 
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */
