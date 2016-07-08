/* --------------------------------------------------------------------------
 *
 *      File            kd_file.c
 *      Description     File system
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

#include "kd_library.h"

#define READ_ONLY			0x0001
#define CHK_DIR_ROOT		0x0010
#define CHK_DIR_RES			0x0020
#define CHK_DIR_REMOVE		0x0040
#define CHK_DIR_ALL			0x0070

typedef struct XMStat       XMStat;

struct XMStat 
{
    KDmode                  mode;
    KDoff                   size;
    KDtime                  time;
};

static KDchar*		        l_dirpaths [ XM_DIRECTORY_COUNT ];	
static KDchar               l_vpaths   [][16] = { "/res", "/data", "/tmp", "/removable", "/storage", "/native", "/", "" };

#ifdef XM_HAS_PLATFORM_FILE_LOG
static KDFile*              l_file_log = KD_NULL;

// xmPlatformFileLog : Output a log message to file.
KDvoid xmPlatformFileLog ( const KDchar* str )
{
	if ( l_file_log )
	{
		kdFwrite ( str, kdStrlen ( str ), 1, l_file_log );
	}
}
#endif

static KDint xmCheckAccessDirectory ( const KDchar* vpath, KDint flag )
{
	KDint	ret = 0;

	if ( ( ( ( flag & CHK_DIR_ROOT	 ) && !kdStrcmp ( vpath, "/" ) ) ||
		   ( ( flag & CHK_DIR_RES	 ) && !kdStrcmp ( vpath, "/res" ) ) ||
		   ( ( flag & CHK_DIR_REMOVE ) && !kdStrcmp ( vpath, "/removable" ) )
		 ) && !( flag & READ_ONLY ) )
	{
		kdSetError ( KD_EACCES );
		ret = -1;
	}

	return ret;
}

KDvoid xmInitDirectory ( KDvoid )
{
	KDint   i;

	xmPlatformMapDirectoryPath ( l_dirpaths );

    kdPrintf ( "\n* Path Informations\n\n" );
    kdPrintf ( ". /          : %s", l_dirpaths [ 6 ] );
    kdPrintf ( ". /res       : %s", l_dirpaths [ 0 ] );
    kdPrintf ( ". /data      : %s", l_dirpaths [ 1 ] );
    kdPrintf ( ". /tmp       : %s", l_dirpaths [ 2 ] );
    kdPrintf ( ". /storage   : %s", l_dirpaths [ 4 ] );
    kdPrintf ( ". /removable : %s", l_dirpaths [ 3 ] );
    kdPrintf ( ". /native    : " );
    kdPrintf ( "\n" );
    
	for ( i = 0; i < 5; i++ )
	{		        
        if ( i == 3 )
        {
            continue;
        }
        
		if ( kdMkdir ( l_vpaths[ i ] ) == -1 )
		{
			if ( kdGetError ( ) != KD_EEXIST )
			{
				kdLogMessage ( "Make init directory failed." );
				//kdExit ( 3 );
			}

			kdSetError ( 0 );		
		}
	}

#ifdef XM_HAS_PLATFORM_FILE_LOG
	l_file_log = kdFopen ( "/data/XMSoft_Log.txt", "wb" );
#endif
}

KDvoid xmFreeDirectory ( KDvoid )
{
	KDint    i;

#ifdef XM_HAS_PLATFORM_FILE_LOG
	if ( l_file_log )
	{
		kdFclose ( l_file_log );
	}
#endif

	for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
	{
		if ( l_dirpaths[ i ] )
		{
			kdFree ( l_dirpaths[ i ] );
			l_dirpaths[ i ] = 0;
		}
	}
}

KD_API KDchar* KD_APIENTRY xmGetNativePath ( const KDchar* path, KDchar** vpath )
{		
	const KDchar*	dpath = KD_NULL;
	KDchar*         rpath = KD_NULL;
	KDsize          plen  = 0;
	KDsize          vlen  = 0;
	KDsize          dlen  = 0;
	KDsize          rlen  = 0;
	KDint           i;

	plen = kdStrlen ( path );

	for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
	{
		vlen = kdStrlen ( l_vpaths [ i ] );

		if ( !kdStrncmp ( path, l_vpaths [ i ], vlen ) )
		{
			if ( plen > vlen && path[ vlen ] != '/' )
			{
				continue;
			}

			if ( kdStrlen ( l_dirpaths [ i ] ) == 0 )
			{
				if ( path [ vlen + 1 ] == ':' )
				{
					dpath = l_vpaths [ XM_DIRECTORY_COUNT ];
				}
				else
				{
					dpath = l_vpaths [ XM_DIRECTORY_COUNT ];
				}
			}
			else
			{
				dpath = l_dirpaths [ i ];
			}

			dlen = kdStrlen ( dpath );
			rlen = dlen + plen - vlen + 1;

			if ( ( rpath = (KDchar *) kdMalloc ( rlen ) ) )
			{
				kdStrcpy ( rpath, dpath );

				if ( plen - vlen > 0 )
				{
					kdStrncat ( rpath, path + vlen, plen - vlen );				
				}

				if ( vpath )
				{
					*vpath = l_vpaths[ i ];				
				}
			}
			else
			{
				kdSetError ( KD_ENOMEM );				
			}			

#if !defined ( SHP ) && defined ( _WIN32 ) || defined ( _WIN32_WCE )
			{
				rlen = kdStrlen ( rpath );

				for ( i = 0; i < (KDint) rlen; i++ )
				{
					if ( rpath[ i ] == '/' )
					{
						rpath[ i ] = '\\';
					}
				}
			}
#endif
			return rpath;
		}		
	}

	kdSetError ( KD_ENOENT );	
	
	return KD_NULL;
}

// kdFopen : Open a file from the file system.
KD_API KDFile *KD_APIENTRY kdFopen ( const KDchar* pathname, const KDchar* mode )
{
	FILE*		file  = NULL;	
	KDchar*		path  = KD_NULL;
	KDchar*		vpath = KD_NULL;
	KDFile*		ret   = KD_NULL;

	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL | !( kdStrchr ( mode, 'w' ) ||  kdStrchr ( mode, 'a' ) ) ) )
		{	
#if defined ( ANDROID )
            if ( !kdStrcmp ( vpath, "/res" ) )
            {
                ret = xmAndroidFopen ( path, mode );

                if ( !ret )
                {
                    kdFree ( path );
                }
                
                return ret;
            }
#endif
			if ( !( file = (FILE *) fopen ( path, mode ) ) )
			{
				goto err;
			}	
			else
			{
				if ( ( ret = (KDFile *) kdMalloc ( sizeof ( KDFile ) ) ) )
				{
					fseek ( file, 0, SEEK_END );
					ret->ptr  = file;
					ret->path = path;
					ret->mem  = 0;
					ret->off  = 0;
					ret->size = ftell ( file );
					fseek ( file, 0, SEEK_SET );
				}
				else
				{
					kdSetError ( KD_ENOMEM );
				}
			}
		}

		goto end;
	}	

err :

	switch ( xmGetErrno ( ) )
	{			
		case	EROFS		 :
		case	EACCES		 :	kdSetError ( KD_EACCES );		break;
		case	EINVAL		 :	kdSetError ( KD_EINVAL );		break;
		case	EIO			 :	kdSetError ( KD_EIO );			break;
		case	EISDIR		 :	kdSetError ( KD_EISDIR );		break;
		case	ENFILE		 :
		case	EMFILE		 :	kdSetError ( KD_EMFILE );		break;
		case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );	break;
		case	ENOTDIR		 :
		case	ENOENT		 :	kdSetError ( KD_ENOENT );		break;	
		case	ENOMEM		 :	kdSetError ( KD_ENOMEM );		break;
		case	ENOSPC		 :	kdSetError ( KD_ENOSPC );		break;
	}

end :

	if ( !ret && path )
	{
		kdFree ( path );
	}

	return ret;

}

// kdFclose : Close an open file.
KD_API KDint KD_APIENTRY kdFclose ( KDFile* file )
{
	KDint       ret = 0;

	if ( file->ptr )
	{
		if ( fclose ( file->ptr ) == EOF )
		{				
			switch ( xmGetErrno ( ) )
			{
				case	EFBIG	:	kdSetError ( KD_EFBIG );    break;
				case	EIO		:	kdSetError ( KD_EIO );      break;
				case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
				case	ENOSPC	:	kdSetError ( KD_ENOSPC );   break;		
			}
				
			ret = KD_EOF;
		}		
	}
#if defined ( ANDROID )     
    else if ( file->path && file->mem )
    {
        kdFree ( file->mem );
    }
#endif
	
	if ( file->path )
	{        
		kdFree ( file->path );
	}

	kdFree ( file );
	return ret;
}

// kdFsize : Get file size.
// XMSoft's revision : Added API.
KD_API KDsize KD_APIENTRY kdFsize ( KDFile* file )
{
	return file->size;
}

// kdFflush : Flush an open file.
KD_API KDint KD_APIENTRY kdFflush ( KDFile* file )
{
	KDint       ret = 0;

	if ( file->mem )
	{
		return ret;
	}

	if ( fflush ( file->ptr ) == EOF )
	{			
		switch ( xmGetErrno ( ) )
		{
			case	EFBIG	:	kdSetError ( KD_EFBIG );    break;
			case	EIO		:	kdSetError ( KD_EIO );      break;
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
			case	ENOSPC	:	kdSetError ( KD_ENOSPC );   break;
		}
		
		ret = KD_EOF;
	}

	return ret;
}

// kdFread : Read from a file.
KD_API KDsize KD_APIENTRY kdFread ( KDvoid* buffer, KDsize size, KDsize count, KDFile* file )
{
	KDsize       ret = 0;
	KDsize       i   = 0;
	KDbyte*      buf = 0;

	if ( file->mem )
	{
		buf = (KDbyte*) buffer;
		for ( i = 0; i < count; i++ )
		{
			if ( file->size == (KDsize) file->off )
			{
				break;
			}
			else //if ( ( file->size - ( file->off + size ) ) >= 0 )
			{
				kdMemcpy ( &buf[ i * size ], &file->mem[ file->off ], size );
				file->off += size;				
			}
			//else
			//{
			//	break;
			//}
		}		
		
		return i;
	}

	if ( ( ret  = (KDsize) fread ( buffer, size, count, file->ptr ) ) < count )
	{		
		switch ( xmGetErrno ( ) )
		{
			case	EBADF	:	kdSetError ( KD_EBADF );    break;
			case	EIO		:	kdSetError ( KD_EIO );      break;
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;	
		}			
	}	

	return ret;
}

// kdFwrite : Write to a file.
KD_API KDsize KD_APIENTRY kdFwrite ( const KDvoid* buffer, KDsize size, KDsize count, KDFile* file )
{
	KDsize       ret = 0;

	if ( file->mem )
	{
		kdSetError ( KD_EINVAL );
		return 0;
	}

	if ( ( ret = (KDsize) fwrite ( buffer, size, count, file->ptr ) ) < count )
	{		
		switch ( xmGetErrno ( ) )
		{
			case	EBADF	:	kdSetError ( KD_EBADF );    break;
			case	EFBIG	:	kdSetError ( KD_EFBIG );    break;
			case	EIO		:	kdSetError ( KD_EIO );      break;
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
			case	ENOSPC	:	kdSetError ( KD_ENOSPC );   break;
		}		
	}

	return ret;
}

// kdGetc : Read next byte from an open file.
KD_API KDint KD_APIENTRY kdGetc ( KDFile* file )
{
	KDint       ret = 0;	

	if ( file->mem )
	{
		if ( file->size == (KDsize) file->off )
		{
			return KD_EOF;
		}
		else //if ( ( file->size - ( file->off + 1 ) ) >= 0 )
		{
			ret = (KDint) file->mem[ file->off ];
			file->off++;
		}
		//else
		//{
		//	ret = KD_EOF;
		//}

		return ret;
	}

	if ( ( ret = getc ( file->ptr ) ) < 0 )
	{		
		switch ( xmGetErrno ( ) )
		{
			case	EBADF	:	kdSetError ( KD_EBADF );    break;
			case	EIO		:	kdSetError ( KD_EIO );      break;
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
		}
		
		ret = KD_EOF;
	}

	return ret;
}

// kdUngetc : Unget character from stream.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdUngetc ( KDint ch, KDFile* file )
{
	KDint  ret = kdGetc ( file );

	if ( ret == ch )
	{
		if ( file->mem )
		{
			file->off--;
			ret = (KDint) file->mem[ file->off ];

			return ret;
		}

		if ( ( ret = ungetc ( ch, file->ptr ) ) < 0 )
		{		
			switch ( xmGetErrno ( ) )
			{
				case	EBADF	:	kdSetError ( KD_EBADF );    break;
				case	EIO		:	kdSetError ( KD_EIO );      break;
				case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
			}
			
			ret = KD_EOF;
		}
	}

	return ret;
}

// kdPutc : Write a byte to an open file.
KD_API KDint KD_APIENTRY kdPutc ( KDint c, KDFile* file )
{
	KDint       ret = 0;

	if ( file->mem )
	{
		kdSetError ( KD_EINVAL );
		return 0;
	}

	if ( ( ret = putc ( c, file->ptr ) ) < 0 )
	{		
		switch ( xmGetErrno ( ) )
		{
			case	EBADF	:	kdSetError ( KD_EBADF );    break;
			case	EFBIG	:	kdSetError ( KD_EFBIG );    break;
			case	EIO		:	kdSetError ( KD_EIO );      break;
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
			case	ENOSPC	:	kdSetError ( KD_ENOSPC );   break;
		}		

		ret = KD_EOF;
	}

	return ret;
}

// kdFgets : Read a line of text from an open file.
KD_API KDchar* KD_APIENTRY kdFgets ( KDchar* buffer, KDsize buflen, KDFile* file )
{
	KDchar*      ret = KD_NULL;
	KDsize       i   = 0;

	if ( file->mem )
	{	
		//kdMemset ( buffer, 0, buflen );
		for ( i = 0; i < buflen; i++ )
		{
			if ( file->size == (KDsize) file->off )
			{
				buffer [ 0 ] = '\0';
				break;
			}
			else //if ( ( file->size - ( file->off + 1 ) ) >= 0 )
			{
				buffer[ i ] = file->mem[ file->off ];
				file->off++;

				if ( file->mem[ file->off - 1 ] == '\n' )
				{
					i++;
					break;
				}
			}
			//else
			//{
			//	file->off = file->size;
			//	break;
			//}
		}

		if ( i < buflen )
		{
			buffer [ i ] = '\0';
		}

		if ( strlen ( buffer ) )
		{
			ret = buffer;
		}

		return ret;
	}

	if ( !( ret = (KDchar *) fgets ( buffer, (KDint) buflen, file->ptr ) ) )
	{		
		switch ( xmGetErrno ( ) )
		{
			case	EBADF	:	kdSetError ( KD_EBADF );    break;
			case	EIO		:	kdSetError ( KD_EIO );      break;
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
		}		
	}

	return ret;
}

// kdGetline ( ) reads an entire line from stream, storing the address of the buffer containing the text into *lineptr.
// The buffer is null-terminated and includes the newline character, if one was found.
// XMSoft's revision : Added API.
KD_API KDssize kdGetline ( KDchar** lineptr, KDsize* buflen, KDFile* file )
{
#if defined ( __GNUC__ ) && !defined ( ANDROID )
	return getline ( lineptr, (size_t*) buflen, file->ptr );
#else

	*lineptr = kdMalloc ( *buflen );
	kdFgets ( *lineptr, *buflen, file );

	return kdStrlen ( *lineptr );

#endif
}

// kdPuts : Write a line of text to an open file.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdPuts ( const KDchar* str, KDFile* file )
{
	KDint       ret = 0;

	if ( file->mem )
	{
		kdSetError ( KD_EINVAL );
		return 0;
	}

	if ( ( ret = fputs ( str, file->ptr ) ) < 0 )
	{		
		switch ( xmGetErrno ( ) )
		{
			case	EBADF	:	kdSetError ( KD_EBADF );    break;
			case	EFBIG	:	kdSetError ( KD_EFBIG );    break;
			case	EIO		:	kdSetError ( KD_EIO );      break;
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;
			case	ENOSPC	:	kdSetError ( KD_ENOSPC );   break;
		}		

		ret = KD_EOF;
	}

	return ret;
}

// kdFEOF : Check for end of file.
KD_API KDint KD_APIENTRY kdFEOF ( KDFile* file )
{
	if ( file->mem )
	{
		return file->size == (KDsize) file->off ? KD_EOF : 0;
	}

	return ( !feof ( file->ptr ) ) ? 0 : KD_EOF;
}

// kdFerror : Check for an error condition on an open file.
// XMSoft's revision : Unexpectedly processed in "iOS" Platform.
KD_API KDint KD_APIENTRY kdFerror ( KDFile* file )
{
	if ( file->mem )
	{
		return 0;
	}

	return ferror ( file->ptr );	
}

// kdClearerr : Clear a file's error and end-of-file indicators.
KD_API void KD_APIENTRY kdClearerr ( KDFile* file )
{	
	if ( file->mem )
	{
		return;
	}

	clearerr ( file->ptr );
}

// kdFseek : Reposition the file position indicator in a file.
KD_API KDint KD_APIENTRY kdFseek ( KDFile* file, KDoff offset, KDfileSeekOrigin origin )
{	
	KDint       ret     = -1;	
	KDint       _origin = 0;

	switch ( origin )
	{
		case KD_SEEK_SET : _origin = SEEK_SET; break;
		case KD_SEEK_CUR : _origin = SEEK_CUR; break;
		case KD_SEEK_END : _origin = SEEK_END; break;
	}		

	if ( file->mem )
	{
		switch ( origin )
		{
			case KD_SEEK_SET : file->off = 0;          break;
			case KD_SEEK_END : file->off = file->size; return 0;
			case KD_SEEK_CUR : ;
		}

		if ( ( file->size - ( file->off + offset ) ) > 0 )
		{
			file->off += offset;
		}
		else
		{
			file->off = file->size;
		}

		return 0;
	}

	if ( ( ret = fseek ( file->ptr, (long) offset, _origin ) ) )
	{		
		switch ( xmGetErrno ( ) )
		{
			case	EFBIG		:	kdSetError ( KD_EFBIG );        break;
			case	EINVAL		:	kdSetError ( KD_EINVAL );       break;
			case	EIO			:	kdSetError ( KD_EIO );          break;
			case	ENOMEM		:	kdSetError ( KD_ENOMEM );       break;
			case	ENOSPC		:	kdSetError ( KD_ENOSPC );       break;
			case	EOVERFLOW	:	kdSetError ( KD_EOVERFLOW );    break;
		}		
	}	

	return ret;
}

// kdFtell : Get the file position of an open file.
KD_API KDoff KD_APIENTRY kdFtell ( KDFile* file )
{
	KDoff       ret = 0;

	if ( file->mem )
	{
		return file->off;
	}

	if ( ( ret = (KDoff) ftell ( file->ptr ) ) == -1 )
	{
		kdSetError ( KD_EOVERFLOW );
	}

	return ret;
}

// kdRewind : Set position indicator to the beginning
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdRewind ( KDFile* file )
{
	kdFseek ( file , 0L , KD_SEEK_SET ); 
}

// kdMkdir : Create new directory.
KD_API KDint KD_APIENTRY kdMkdir ( const KDchar* pathname )
{	
	KDint       ret	  = -1;
	KDchar*     path  = KD_NULL;
	KDchar*     vpath = KD_NULL;
	
	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !kdStrcmp ( pathname, "/res" ) || !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
		{
			if ( ( ret = mkdir ( path, 0755 ) ) )
			{
				switch ( xmGetErrno ( ) )
				{
					case	EROFS		 :
					case	EACCES		 :	kdSetError ( KD_EACCES );       break;
					case	EEXIST		 :	kdSetError ( KD_EEXIST );       break;						
					case	EIO			 :	kdSetError ( KD_EIO );          break;
					case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG ); break;
					case	ENOTDIR		 :
					case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;
					case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;
					case	ENOSPC		 :	kdSetError ( KD_ENOSPC );       break;
				}
			}
		}	

		kdFree ( path );
	}	

	return ret;
}

// kdRmdir : Delete a directory.
KD_API KDint KD_APIENTRY kdRmdir ( const KDchar* pathname )
{
	KDint       ret	  = -1;
	KDchar*     path  = KD_NULL;
	KDchar*     vpath = KD_NULL;
	
	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
		{
			if ( ( ret = rmdir ( path ) ) )
			{
				switch ( xmGetErrno ( ) )
				{
					case	EROFS		 :
					case	EACCES		 :	kdSetError ( KD_EACCES );       break;
					case	EBUSY		 :	kdSetError ( KD_EBUSY );        break;
					case	ENOTEMPTY	 :
					case	EEXIST		 :	kdSetError ( KD_EEXIST );       break;						
					case	EIO			 :	kdSetError ( KD_EIO );          break;
					case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG ); break;
					case	ENOTDIR		 :
					case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;
					case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;
				}			
			}
		}

		kdFree ( path );
	}	

	return ret;
}

// kdRename : Rename a file.
KD_API KDint KD_APIENTRY kdRename ( const KDchar* src, const KDchar* dest )
{	
	KDint       ret     = -1;
	KDchar*     vpath   = KD_NULL;
	KDchar*     srcpath = KD_NULL;
	KDchar*     dstpath = KD_NULL;	
	
	if ( ( srcpath = xmGetNativePath ( src, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
		{
			if ( ( dstpath = xmGetNativePath ( dest, &vpath ) ) )
			{
				if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
				{
					if ( ( ret = rename ( srcpath, dstpath ) ) )
					{
						switch ( xmGetErrno ( ) )
						{
							case	EROFS		 :
							case	EACCES		 :	kdSetError ( KD_EACCES );       break;
							case	EBUSY		 :	kdSetError ( KD_EBUSY );        break;
							case	EINVAL		 :	kdSetError ( KD_EINVAL );       break;
							case	EIO			 :	kdSetError ( KD_EIO );          break;	
							case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG ); break;
							case	ENOTDIR		 :
							case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;
							case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;
							case	ENOSPC		 :	kdSetError ( KD_ENOSPC );       break;
						}	
					}
				}		

				kdFree ( dstpath );
			}
		}	

		kdFree ( srcpath );
	}	

	return ret;
}

// kdRemove : Delete a file.
KD_API KDint KD_APIENTRY kdRemove ( const KDchar* pathname )
{	
	KDint       ret   = -1;
	KDchar*     path  = KD_NULL;
	KDchar*     vpath = KD_NULL;
	
	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
		{
			if ( ( ret = remove ( path ) ) )
			{
				switch ( xmGetErrno ( ) )
				{
					case	EROFS		 :
					case	EACCES		 :	kdSetError ( KD_EACCES );       break;
					case	EBUSY		 :	kdSetError ( KD_EBUSY );        break;
					case	EIO			 :	kdSetError ( KD_EIO );          break;	
					case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );	break;
					case	ENOTDIR		 :
					case	ENOENT		 :	kdSetError ( KD_ENOENT);        break;
					case	ENOMEM		 :	kdSetError ( KD_ENOMEM);        break;
				}			
			}
		}

		kdFree ( path );
	}	

	return ret;
}

// kdTruncate : Truncate or extend a file.
// XMSoft's revision : Unexpectedly processed in "Android" Platform. ( Case : Smaller )
KD_API KDint KD_APIENTRY kdTruncate ( const KDchar *pathname, KDoff length )
{
	KDint       ret	  = -1;
	KDchar*	    path  = KD_NULL;
	KDchar*	    vpath = KD_NULL;
	
	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
		{
			if( ( ret = truncate ( path, length ) ) )
			{
				switch ( xmGetErrno ( ) )
				{
					case	EROFS		 :
					case	EISDIR		 :
					case	EACCES		 :	kdSetError ( KD_EACCES );        break;	
					case	EFBIG		 :	
					case	EINVAL		 :	kdSetError ( KD_EINVAL );        break;
					case	EIO			 :	kdSetError ( KD_EIO );           break;	
					case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );  break;
					case	ENOTDIR		 :
					case	ENOENT		 :	kdSetError ( KD_ENOENT );        break;
					case	ENOMEM		 :	kdSetError ( KD_ENOMEM );        break;
				}
			}
		}

		kdFree ( path );
	}

	return ret;
}


// kdStat, kdFstat : Return information about a file.
KD_API KDint KD_APIENTRY kdStat ( const KDchar* pathname, struct KDStat* buf )
{		
	KDint            ret   = -1;
	KDchar*          path  = KD_NULL;
	KDchar*	         vpath = KD_NULL;
	struct XMStat*   dbuf  = (struct XMStat*) buf; 
	struct stat      sbuf;

	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ROOT | CHK_DIR_REMOVE ) )
		{
#if defined ( ANDROID )
            if ( !kdStrcmp ( vpath, "/res" ) )
            {
                if ( ( ret = xmAndroidStat ( path, buf ) ) != 0 )
				{
					goto err;
				}

                goto end;
            }
#endif
			if ( ( ret = stat ( path, &sbuf ) ) )
			{
				goto err;
			}
			else
			{			
				dbuf->mode = (KDmode) sbuf.st_mode;
				dbuf->size = (KDoff)  sbuf.st_size;
				dbuf->time = (KDtime) sbuf.st_mtime;
			}		
		}

		goto end;
	}
	
err :

	switch( xmGetErrno ( ) )
	{
		case	EACCES			:	kdSetError ( KD_EACCES );       break;	
		case	EIO				:	kdSetError ( KD_EIO );          break;	
		case	ENAMETOOLONG	:	kdSetError ( KD_ENAMETOOLONG );	break;
		case	ENOTDIR			:
		case	ENOENT			:	kdSetError ( KD_ENOENT );       break;
		case	ENOMEM			:	kdSetError ( KD_ENOMEM );       break;			
		case	EOVERFLOW		:	kdSetError ( KD_EOVERFLOW );    break;
	}	

end :

	if ( path )
	{
		kdFree( path );
	}

	return ret;
}

KD_API KDint KD_APIENTRY kdFstat ( KDFile* file, struct KDStat* buf )
{
	KDint            ret  = -1;	
	struct XMStat*   dbuf = (struct XMStat*) buf; 
	struct stat      sbuf;
		
	if ( file->mem )
	{
		dbuf->mode = 0;
		dbuf->size = file->size;
		dbuf->time = 0;

		return 0;
	}

	if( ( ret = fstat ( fileno ( file->ptr ), &sbuf ) ) )
	{
		switch( xmGetErrno ( ) )
		{
			case	EACCES		 :	kdSetError ( KD_EACCES );       break;	
			case	EIO			 :	kdSetError ( KD_EIO );          break;	
			case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );	break;
			case	ENOTDIR		 :
			case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;
			case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;			
			case	EOVERFLOW	 :	kdSetError ( KD_EOVERFLOW );    break;		
			case	ENOSYS		 :	kdSetError ( KD_ENOSYS );       break;
		}
	}
	else
	{
		dbuf->mode = (KDmode) sbuf.st_mode;
		dbuf->size = (KDoff)  sbuf.st_size;
		dbuf->time = (KDtime) sbuf.st_mtime;
	}

	return ret;
}

KD_API KDint KD_APIENTRY kdFstat_n ( KDint fileno, struct KDStat* buf )
{
	KDint            ret  = -1;	
#if !defined ( _WIN32_WCE )
	struct XMStat*   dbuf = (struct XMStat*) buf; 
	struct stat      sbuf;

	if( ( ret = fstat ( fileno, &sbuf ) ) )
	{
		switch( xmGetErrno ( ) )
		{
			case	EACCES		 :	kdSetError ( KD_EACCES );       break;	
			case	EIO			 :	kdSetError ( KD_EIO );          break;	
			case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );	break;
			case	ENOTDIR		 :
			case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;
			case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;			
			case	EOVERFLOW	 :	kdSetError ( KD_EOVERFLOW );    break;		
			case	ENOSYS		 :	kdSetError ( KD_ENOSYS );       break;
		}
	}
	else
	{
		dbuf->mode = (KDmode) sbuf.st_mode;
		dbuf->size = (KDoff)  sbuf.st_size;
		dbuf->time = (KDtime) sbuf.st_mtime;
	}
#endif	

	return ret;
}

// kdLstat The kdLstat() function shall be equivalent to kdStat(), except when path refers to a symbolic link. In that case lstat() shall return information about the link, while stat() shall return information about the file the link references.
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdLstat ( const KDchar* pathname, struct KDStat* buf )
{
	KDint            ret   = -1;
	KDchar*          path  = KD_NULL;
	KDchar*	         vpath = KD_NULL;
	struct XMStat*   dbuf  = (struct XMStat*) buf; 
	struct stat      sbuf;

	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ROOT | CHK_DIR_REMOVE ) )
		{
#if defined ( ANDROID )
            if ( !kdStrcmp ( vpath, "/res" ) )
            {
                if ( ( ret = xmAndroidStat ( path, buf ) ) != 0 )
				{
					goto err;
				}

                goto end;
            }
#endif
			if ( ( ret = lstat ( path, &sbuf ) ) )
			{
				goto err;
			}
			else
			{			
				dbuf->mode = (KDmode) sbuf.st_mode;
				dbuf->size = (KDoff)  sbuf.st_size;
				dbuf->time = (KDtime) sbuf.st_mtime;
			}		
		}

		goto end;
	}
	else
	{
		if ( ( ret = lstat ( path, &sbuf ) ) )
		{
			goto err;
		}
		else
		{			
			dbuf->mode = (KDmode) sbuf.st_mode;
			dbuf->size = (KDoff)  sbuf.st_size;
			dbuf->time = (KDtime) sbuf.st_mtime;
		}	
	}
	
err :

	switch( xmGetErrno ( ) )
	{
		case	EACCES			:	kdSetError ( KD_EACCES );       break;	
		case	EIO				:	kdSetError ( KD_EIO );          break;	
		case	ENAMETOOLONG	:	kdSetError ( KD_ENAMETOOLONG );	break;
		case	ENOTDIR			:
		case	ENOENT			:	kdSetError ( KD_ENOENT );       break;
		case	ENOMEM			:	kdSetError ( KD_ENOMEM );       break;			
		case	EOVERFLOW		:	kdSetError ( KD_EOVERFLOW );    break;
	}	

end :

	if ( path )
	{
		kdFree( path );
	}

	return ret;
}

// kdAccess : Determine whether the application can access a file or directory.
// XMSoft's revision : Unexpectedly processed in each platform
KD_API KDint KD_APIENTRY kdAccess ( const KDchar* pathname, KDint amode )
{		
	KDint       ret	  = -1;
	KDint       mode  = 0;
	KDchar*     path  = KD_NULL;
	KDchar*	    vpath = KD_NULL;
		
	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) ) 
	{
		if ( ( !xmCheckAccessDirectory ( vpath, CHK_DIR_RES | !( amode & KD_W_OK ) ) ) &&
			   !xmCheckAccessDirectory ( vpath, CHK_DIR_ROOT | CHK_DIR_REMOVE ) )
		{
			mode |= amode & KD_R_OK ? R_OK : 0;
			mode |= amode & KD_W_OK ? W_OK : 0;
			mode |= amode & KD_X_OK ? X_OK : 0;
			mode |= amode & KD_F_OK ? F_OK : 0;

			if ( ( ret = access ( path, mode ) ) )
			{
				switch( xmGetErrno ( ) )
				{
					case	EACCES			:	kdSetError ( KD_EACCES );       break;	
					case	EIO				:	kdSetError ( KD_EIO );          break;	
					case	ENAMETOOLONG	:	kdSetError ( KD_ENAMETOOLONG );	break;
					case	ENOTDIR			:
					case	ENOENT			:	kdSetError ( KD_ENOENT );       break;
					case	ENOMEM			:	kdSetError ( KD_ENOMEM );       break;
					case 	ENOSPC			:	kdSetError ( KD_ENOSPC );       break;
				}					
			}
		}

		kdFree ( path );
	}	

	return ret;
}

// kdOpenDir : Open a directory ready for listing.
KD_API KDDir* KD_APIENTRY kdOpenDir ( const KDchar* pathname )
{
	KDDir*		ret   = KD_NULL;
	DIR*		dir	  = KD_NULL;
	KDchar*     path  = KD_NULL;
	KDchar*     vpath = KD_NULL;
		
	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) ) 
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ROOT ) )
		{			
			#if defined ( ANDROID )
            if ( !kdStrcmp ( vpath, "/res" ) )
            {
				ret = xmAndroidOpenDir ( path );
				kdFree ( path );
				return ret;
            }
			#endif

			if ( !( dir = opendir ( path ) ) )
			{
				switch( xmGetErrno ( ) )
				{
					case	EACCES		 :	kdSetError ( KD_EACCES );       break;	
					case	EIO			 :	kdSetError ( KD_EIO );          break;	
					case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );	break;
					case	ENOTDIR		 :
					case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;
					case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;
				}
			}
			else
			{
				if ( ( ret = (KDDir *) kdMalloc ( sizeof ( KDDir ) ) ) )
				{
					ret->ptr = dir;
				}
				else
				{
					closedir ( dir );
					kdSetError ( KD_ENOMEM );
				}
			}
		}

		kdFree ( path );
	}

	return ret;
}

// kdReadDir : Return the next file in a directory.
KD_API KDDirent* KD_APIENTRY kdReadDir ( KDDir* dir )
{
	struct dirent*  dp  = 0;
	KDDirent*       ret	= KD_NULL;

	#if defined ( ANDROID )
	if ( dir->ptr == KD_NULL )
	{
		ret = xmAndroidReadDir ( dir );
		return ret;
	}
	#endif

	if ( !( dp = readdir ( dir->ptr )  ) )
	{
		switch ( xmGetErrno ( ) )
		{
			case	EIO		:	kdSetError ( KD_EIO );      break;			
			case	ENOMEM	:	kdSetError ( KD_ENOMEM );   break;	
		}		
	}
	else
	{
		ret = (KDDirent*) dp;
		ret->d_name = dp->d_name;
	}

	return ret;	
}

// kdCloseDir : Close a directory.
KD_API KDint KD_APIENTRY kdCloseDir ( KDDir* dir )
{
	if ( dir->ptr )
	{
		closedir ( dir->ptr );
	}
	kdFree ( dir );
	return 0;
}

// kdGetFree : Get free space on a drive.
// XMSoft's revision : Not yet to be supported in "Android" platform.
KD_API KDoff KD_APIENTRY kdGetFree ( const KDchar* pathname )
{
	KDoff		off	  = -1;	
	KDchar*		path  = KD_NULL;
	KDchar*		vpath = KD_NULL;
	
	if ( ( path = xmGetNativePath( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ROOT | CHK_DIR_REMOVE ) )
		{
			#ifdef	XM_HAS_PLATFORM_FREE_SPACE			
			
			off = xmPlatformGetFree ( path );
			
			#else
								
			struct statvfs info;
			
			if ( !statvfs ( path, &info ) )
			{
				off = info.f_bavail * info.f_frsize;
//				off = info.f_bsize * info.f_bavail;
			}			
			
			#endif
			
			if ( off == -1 )			
			{
				switch ( xmGetErrno ( ) )
				{
					case	EACCES		 :	kdSetError ( KD_EACCES );       break;			
					case	EIO			 :	kdSetError ( KD_EIO );          break;	
					case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );	break;			
					case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;	
					case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;			
					case	ENOSYS		 :	kdSetError ( KD_ENOSYS );       break;	
					case	EOVERFLOW	 :	kdSetError ( KD_EOVERFLOW );    break;
				}
			}
		}

		kdFree ( path );
	}
	
	return off;
}

// kdOpen : Open a file.
// XMSoft's revision : Additional APIs.
KD_API KDFile* KD_APIENTRY kdFmemopen ( KDvoid* buf, KDsize size, const KDchar* mode )
{
	KDFile*    ret = 0;

	if ( size == 0 )
	{
		kdSetError ( KD_EINVAL );
		return 0;
	}

	if ( mode[0] != 'r' )
	{
		kdSetError ( KD_EINVAL );
		return 0;
	}

	if ( ( ret = (KDFile *) kdMalloc ( sizeof ( KDFile ) ) ) )
	{
		ret->ptr  = 0;
		ret->path = 0;
		ret->mem  = (KDubyte*) buf;
		ret->off  = 0;
		ret->size = size;
	}
	
	return ret;
}

// kdOpen : Get current position in stream.
// XMSoft's revision : Added API.
KD_API KDint kdFgetpos ( KDFile* file, KDoff* pos )
{
	KDint  ret = 0;

	*pos = kdFtell ( file );
	if ( *pos == -1L )
	{
		ret = kdFerror ( file );
		return ret == 0 ? 1 : ret;
	}

	return 0;
}

// kdOpen : Open a file.
// XMSoft's revision : Additional APIs.
KD_API KDint KD_APIENTRY kdOpen ( const KDchar* pathname, KDint flags, KDint mode )
{
	KDchar*		vpath  = KD_NULL;
	KDchar*		path   = KD_NULL;
	KDint       fd     = 0;
	KDint       _flags = 0;

	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( flags & KD_O_RDONLY ) _flags |= O_RDONLY;	
		if ( flags & KD_O_WRONLY ) _flags |= O_WRONLY; 
		if ( flags & KD_O_RDWR   ) _flags |= O_RDWR;   
		if ( flags & KD_O_APPEND ) _flags |= O_APPEND; 
		if ( flags & KD_O_CREAT  ) _flags |= O_CREAT;  
		if ( flags & KD_O_TRUNC  ) _flags |= O_TRUNC;  
		if ( flags & KD_O_EXCL   ) _flags |= O_EXCL; 
	#if O_NOCTTY
		if ( flags & KD_O_NOCTTY ) _flags |= O_NOCTTY;
	#endif
	#if O_NONBLOCK
		if ( flags & KD_O_NONBLOCK ) _flags |= O_NONBLOCK;
	#endif		

		#if defined ( ANDROID )
        if ( !kdStrcmp ( vpath, "/res" ) )
        {
			fd = xmAndroidOpen ( path, _flags, mode );
			kdFree ( path );
			return fd;
        }
		#endif  
		
		fd = open ( path, _flags, mode );
		kdFree ( path );
	}

	return fd;
}

// kdClose : Close a file.
// XMSoft's revision : Additional APIs.
KD_API KDint KD_APIENTRY kdClose ( KDint fd )
{
	return close ( fd );
}

// kdLseek : Set position in a file.
// XMSoft's revision : Additional APIs.
KD_API KDint KD_APIENTRY kdLseek ( KDint fd, KDint position, KDint startpoint )
{
	return (KDint) lseek ( fd, position, startpoint );
}

// kdRead : Read from a file.
// XMSoft's revision : Additional APIs.
KD_API KDint KD_APIENTRY kdRead ( KDint fd, KDvoid* ptr, KDint len )
{
	return (KDint) read ( fd, ptr, len );
}

// kdWrite : Write to a file. 
// XMSoft's revision : Additional APIs.
KD_API KDint KD_APIENTRY kdWrite ( KDint fd, KDvoid* ptr, KDint len )
{
	return (KDint) write ( fd, ptr, len );
}

// kdChdir : change working directory
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdChdir ( const KDchar* path )
{
	return chdir ( path );
}

// Provides an alternative pathname for the existing file, so that the file can be accessed by either the old or the new name.
// kdLink ( ) creates a link from the pathname newname to an existing file, with the pathname oldfile.
// XMSoft's revision : Added API.
KD_API KDint kdLink ( const KDchar* oldfile, const KDchar* newname )
{
	KDint       ret     = -1;
	KDchar*     vpath   = KD_NULL;
	KDchar*     oldpath = KD_NULL;
	KDchar*     newpath = KD_NULL;	
	
	if ( ( oldpath = xmGetNativePath ( oldfile, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
		{
			if ( ( newpath = xmGetNativePath ( newname, &vpath ) ) )
			{
				if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
				{
					if ( ( ret = link ( oldpath, newpath ) ) )
					{
						switch ( xmGetErrno ( ) )
						{
							case	EROFS		 :
							case	EACCES		 :	kdSetError ( KD_EACCES );       break;
							case	EBUSY		 :	kdSetError ( KD_EBUSY );        break;
							case	EINVAL		 :	kdSetError ( KD_EINVAL );       break;
							case	EIO			 :	kdSetError ( KD_EIO );          break;	
							case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG ); break;
							case	ENOTDIR		 :
							case	ENOENT		 :	kdSetError ( KD_ENOENT );       break;
							case	ENOMEM		 :	kdSetError ( KD_ENOMEM );       break;
							case	ENOSPC		 :	kdSetError ( KD_ENOSPC );       break;
						}	
					}
				}		

				kdFree ( newpath );
			}
		}	

		kdFree ( oldpath );
	}	

	return ret;
}

// kdUnlink : Delete a name and possibly the file it refers to
// XMSoft's revision : Added API.
KD_API KDint kdUnlink ( const KDchar* pathname )
{
	KDint       ret   = -1;
	KDchar*     path  = KD_NULL;
	KDchar*     vpath = KD_NULL;
	
	if ( ( path = xmGetNativePath ( pathname, &vpath ) ) )
	{
		if ( !xmCheckAccessDirectory ( vpath, CHK_DIR_ALL ) )
		{
			if ( ( ret = unlink ( path ) ) )
			{
				switch ( xmGetErrno ( ) )
				{
					case	EROFS		 :
					case	EACCES		 :	kdSetError ( KD_EACCES );       break;
					case	EBUSY		 :	kdSetError ( KD_EBUSY );        break;
					case	EIO			 :	kdSetError ( KD_EIO );          break;	
					case	ENAMETOOLONG :	kdSetError ( KD_ENAMETOOLONG );	break;
					case	ENOTDIR		 :
					case	ENOENT		 :	kdSetError ( KD_ENOENT);        break;
					case	ENOMEM		 :	kdSetError ( KD_ENOMEM);        break;
				}			
			}
		}

		kdFree ( path );
	}	

	return ret;
}

// kdSync :  synchronize data on disk with memory.
// XMSoft's revision : Added API.
KD_API KDvoid KD_APIENTRY kdSync ( KDvoid )
{
	sync ( );
}

// kdIsatty : test whether a file descriptor refers to a terminal
// XMSoft's revision : Added API.
KD_API KDint KD_APIENTRY kdIsatty ( KDint fd )
{
    return isatty ( fd );
}
