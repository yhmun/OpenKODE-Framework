/* --------------------------------------------------------------------------
 *
 *      File            win32_file.c
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

#include "../kd_library.h"

// xmPlatformMapDirectoryPath : Map absoulte path to virtual path.
KDvoid xmPlatformMapDirectoryPath ( KDchar** paths )
{
	TCHAR       tpath[MAX_PATH] = _T( "" );
	char*       rootdir = "";
	int         rootlen = 0;
	int         ret		= 0;
	int         i;
	
	#ifdef _WIN32_WCE

	if ( GetModuleFileName ( NULL, tpath, MAX_PATH ) )
	{		
		if ( XM_SET_STR ( rootdir, tpath ) )
		{
			for ( i = strlen ( rootdir ) - 1; i > 0 && rootdir[ i ] != '\\'; --i )
			{
				rootdir[ i ] = '\0';
			}

			rootdir[ i ] = '\0';
		}
	}

	#else

	if ( GetCurrentDirectory ( MAX_PATH, tpath ) )
	{
		XM_SET_STR ( rootdir, tpath );		
	}

	#endif

	if ( !rootdir )
	{
		kdLogMessage ( "Get root directory failed." );
		kdExit ( 3 );
	}

	rootlen = strlen ( rootdir );
	
	for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
	{
		paths[ i ] = (char*) kdCalloc ( 1, rootlen + 20 );

		if ( !paths[ i ] )
		{
			kdLogMessage ( "Alloc memory failed." );
			kdExit ( 3 );
		}

		switch ( i ) 
		{
			#ifdef _WIN32_WCE

			case 0 :	sprintf ( paths[ i ], "%s/Resource"       , rootdir );	break; // "/res"			
			case 1 :	sprintf ( paths[ i ], "%s/Data"			  , rootdir );	break; // "/data"
			case 2 :	strcpy  ( paths[ i ], "/Temp"						);  break; // "/tmp"
			case 3 :    break; // "/removable"
			
			#if   defined ( Platform_SiRFPrima_EVB_CE6 )
			case 4 :    strcpy  ( paths[ i ], "/SDMMC DISK"					);  break; // "/storage"
			#elif defined ( Platform_TCC8801 )
			case 4 :    strcpy  ( paths[ i ], "/SDCARD"						);  break; // "/storage"
			#endif

			case 5 :    break; // "/native"
					 
			#else

			case 0 :	sprintf ( paths[ i ], "%s/../../Resource"	, rootdir );	break; // "/res"	
			case 1 :	sprintf ( paths[ i ], "%s/../../Data"		, rootdir );	break; // "/data"					
			case 2 :	sprintf ( paths[ i ], "%s/Temp"				, rootdir );	break; // "/tmp"	
			case 3 :    break; // "/removable"
			case 5 :    break; // "/native"

			case 4 :

				if ( !kdStrncmp ( rootdir, "\\\\psf\\Home", 10 ) )
				{
					kdStrcpy ( paths[ i ], "\\\\psf\\Home\\XMStorage" );
				}
				else if ( !kdStrncmp ( rootdir, "\\\\vmware-host\\shared folders", 10 ) )
				{
					kdStrcpy ( paths[ i ], "\\\\vmware-host\\shared folders\\XMStorage" );
				}
				else
				{
					//kdStrncpy ( paths[ i ], rootdir, 2 );
					kdStrcat  ( paths[ i ], "\\XMStorage" );
				}				

				break;

			#endif

			case 6  :	strcpy  ( paths[ i ],                  rootdir );	break; // "/"			
		}		
	}
	
	KD_FREE_STR ( rootdir );
}

// xmWinTruncate : Truncate or extend a file
int	xmWinTruncate ( const char* path, off_t_ length )
{
	HANDLE      file  = INVALID_HANDLE_VALUE;
	TCHAR*      tpath = 0;	
	DWORD       low	  = 0;
	DWORD       high  = 0;
	off_t_      size  = 0;
	int         ret	  = -1;
	
	if ( XM_SET_TSTR ( tpath, (char*) path ) )
	{
		if ( ( file = CreateFile ( tpath, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ) != INVALID_HANDLE_VALUE )
		{
			low   = GetFileSize ( file, &high );
 			size  = high;
 			size  = size << 32;
 			size += low;

 			if ( size >= length )
 			{
				if ( SetFilePointer ( file, (long) length, &high, FILE_BEGIN ) == 0xFFFFFFFF )
				{
					goto jump_err;
				}
			}
			else
			{
				off_t_          csize     = size; 
				off_t_          wsize     = 0;
				DWORD           wbytes    = 0;
				unsigned char   tmp[4096] = "";
							
				if ( SetFilePointer ( file, 0, 0, FILE_END ) == 0xFFFFFFFF )
				{
					goto jump_err;
				}
			
				while ( csize != length )
				{					
					wsize = ( ( csize + 4096 ) > length ) ? length - csize : 4096;
					
					if( !WriteFile ( file, tmp, (long) wsize, &wbytes, NULL ) || wsize != wbytes )			
					{
						goto jump_err;
					}

					csize += wsize;			
				}
			}

			if ( SetEndOfFile ( file ) )
			{
				ret = 0;
				goto jump_exit;
			}	

		jump_err :

			xmWinDosMapErr ( GetLastError ( ) );

		jump_exit :

			CloseHandle ( file );
		}

		KD_FREE_TSTR ( tpath );
	}
	else
	{
		xmPlatformSetErrno ( ENOMEM );
	}

	return ret;
}

struct DIR 
{	
	HANDLE              handle;		
	BOOL                findfirst;
	WIN32_FIND_DATA     find_data;
	struct dirent       dir_info;	
};

// xmWinOpendir : Open a directory ready for listing.
DIR* xmWinOpendir ( const char* dirname )
{
	DIR*        ret	  = 0;
	TCHAR*      tpath = NULL;
	TCHAR*      tfind = NULL;

	if ( XM_SET_TSTR ( tpath, (char*) dirname ) )
	{
		if ( tfind = malloc ( sizeof ( TCHAR ) * ( lstrlen ( tpath ) + 5 ) ) )
		{
			_tcscpy ( tfind, tpath );
			_tcscat ( tfind, _T("\\*") );

			if ( ret = malloc ( sizeof( DIR ) ) )
			{
				if ( ( ret->handle = FindFirstFile ( tfind, &ret->find_data ) ) == INVALID_HANDLE_VALUE )
				{
					xmWinDosMapErr ( GetLastError ( ) );
					free ( ret );
					ret = 0;
				}
				else
				{
					XM_SET_STR ( ret->dir_info.d_name, ret->find_data.cFileName );
					ret->findfirst = TRUE;										
				}				
			}
			else
			{
				xmPlatformSetErrno ( ENOMEM );
			}

			free ( tfind );
		}
		else
		{
			xmPlatformSetErrno ( ENOMEM );
		}

		KD_FREE_TSTR ( tpath );
	}

	return ret;	
}

// xmWinReaddir : Return the next file in a directory.
dirent*	xmWinReaddir ( DIR* dirp )
{	
	dirent*     ret = 0;

	if ( dirp->findfirst )
	{
		dirp->findfirst = FALSE;
		ret = &dirp->dir_info;
	}
	else
	{
		if ( FindNextFile ( dirp->handle, &dirp->find_data ) )
		{
			KD_FREE_STR ( (char*) dirp->dir_info.d_name );
			XM_SET_STR ( dirp->dir_info.d_name, dirp->find_data.cFileName );

			ret = &dirp->dir_info;
		}
		else
		{
			DWORD    err_code = GetLastError ( );

			if ( err_code != ERROR_NO_MORE_FILES )
			{
				xmWinDosMapErr ( err_code );
			}	
		}
	}

	return ret;
}

// xmWinClosedir : Close a directory.
int	xmWinClosedir ( DIR* dirp )
{
	int     ret = 0;

	KD_FREE_STR ( (char*) dirp->dir_info.d_name );

	if ( dirp->handle )
	{
		if ( !FindClose ( dirp->handle ) )
		{
			xmWinDosMapErr ( GetLastError ( ) );
			ret = -1;
		}
	}

	return ret;
}

// xmWinLink : Creates a link from the pathname newname to an existing file, with the pathname oldfile.
int xmWinLink ( const KDchar* oldfile, const KDchar* newname )
{
#if !defined ( _WIN32_WCE )
	TCHAR*      toldpath = KD_NULL;
	TCHAR*      tnewpath = KD_NULL;
	KDint		ret      = 0;

	XM_SET_TSTR ( toldpath, oldfile );
	XM_SET_TSTR ( tnewpath, newname );

	if ( CreateSymbolicLink ( toldpath, tnewpath, 0 ) == FALSE )
	{
		xmWinDosMapErr ( GetLastError ( ) );
		ret = -1;
	}

	KD_FREE_TSTR ( toldpath );
	KD_FREE_TSTR ( tnewpath );

	return ret;
#else
	return -1;
#endif
}

// xmPlatformGetFree : Get free space on a drive in in platform specific.
KDoff xmPlatformGetFree ( const KDchar* path )
{
	KDoff       space = -1;
	TCHAR*      tpath = 0;

	XM_SET_TSTR ( tpath, (KDchar*) path );

	if ( !GetDiskFreeSpaceEx ( tpath, (PULARGE_INTEGER) &space, NULL, NULL ) )
	{
		xmWinDosMapErr ( GetLastError ( ) );
	}

	KD_FREE_TSTR ( tpath );

	return space;
}

#ifdef _WIN32_WCE

// xmWinMkdir : Create new directory.
int	xmWinMkdir ( const char* path, mode_t mode )
{
	int         ret	  = -1;
	TCHAR*      tpath = 0;
	
	if ( XM_SET_TSTR ( tpath, path ) )
	{
		if ( CreateDirectory ( tpath, NULL ) )
		{
			ret = 0;
		}
		else
		{
			xmWinDosMapErr ( GetLastError ( ) );	
		}

		KD_FREE_TSTR ( tpath );
	}
	else
	{
		xmPlatformSetErrno ( ENOMEM );
	}	

	return ret;
}

// xmWinRmdir : Delete a directory.
int	xmWinRmdir ( const char* path )
{
	int         ret	  = -1;
	TCHAR*      tpath = 0;
	
	if ( XM_SET_TSTR ( tpath, path ) )
	{
		access ( path, R_OK | W_OK );
		if ( RemoveDirectory ( tpath ) )
		{
			ret = 0;
		}
		else
		{
			xmWinDosMapErr ( GetLastError ( ) );	
		}

		KD_FREE_TSTR ( tpath );	
	}
	else
	{
		xmPlatformSetErrno ( ENOMEM );
	}

	return ret;
}

// xmWinRename : Rename a file.
int	xmWinRename ( const char* _old, const char* _new )
{
	int         ret	 = -1;
	TCHAR*      told = 0;
	TCHAR*      tnew = 0;

	if ( XM_SET_TSTR ( told, _old ) )
	{
		if (  XM_SET_TSTR ( tnew, _new ) )
		{
			if ( MoveFile ( told, tnew ) )
			{
				ret = 0;
			}
			else
			{
				xmWinDosMapErr ( GetLastError ( ) );	
			}
		}
		else
		{
			KD_FREE_TSTR ( tnew );		
		}

		KD_FREE_TSTR ( told );	
	}
	else
	{
		xmPlatformSetErrno ( ENOMEM );
	}		

	return ret;
}

// xmWinRemove : Delete a file.
int	xmWinRemove ( const char* path )
{
	int         ret	  = -1;
	TCHAR*      tpath = 0;

	if ( XM_SET_TSTR ( tpath, path ) )
	{
		access ( path, R_OK | W_OK );
		if ( DeleteFile ( tpath ) )
		{
			ret = 0;
		}
		else
		{
			xmWinDosMapErr ( GetLastError ( ) );	
		}

		KD_FREE_TSTR ( tpath );	
	}
	else
	{
		xmPlatformSetErrno ( ENOMEM );
	}	

	return ret;
}



static BOOL FileTimeToUnixTime ( const FILETIME* ft, time_t* ut )
{       
	BOOL success = FALSE;
    const ULONGLONG second = 10000000L;

    SYSTEMTIME base_st = 
    {
        1970,   // wYear            
        1,      // wMonth           
        0,      // wDayOfWeek       
        1,      // wDay             
        0,      // wHour            
        0,      // wMinute          
        0,      // wSecond          
        0       // wMilliseconds    
    };
    
    ULARGE_INTEGER itime;
    FILETIME base_ft;
    
    if ( ( success = SystemTimeToFileTime( &base_st, &base_ft ) ) ) 
    {
        itime.QuadPart  = ((ULARGE_INTEGER *) ft)->QuadPart;
        itime.QuadPart -= ((ULARGE_INTEGER *) &base_ft)->QuadPart;
        itime.QuadPart /= second;

        *ut = itime.LowPart;
    }
	else
	{
		*ut = -1;   // error value used by mktime() 
	}
   
    return success;
}

// xmWinStat : Return information about a file.
int	xmWinStat ( const char* path, struct stat* buf )
{
	int         ret	  = -1;
	TCHAR*      tpath = 0;
	
	if ( XM_SET_TSTR ( tpath, path ) )
	{
		WIN32_FIND_DATA     find;
		HANDLE              file;

		if ( ( file = FindFirstFile ( tpath, &find ) ) != INVALID_HANDLE_VALUE )
		{
			ret = 0;

			buf->st_size  = find.nFileSizeHigh;
			buf->st_size  = buf->st_size << 32;
			buf->st_size += find.nFileSizeLow;

			FileTimeToUnixTime ( &find.ftLastWriteTime, &buf->st_mtime );

			buf->st_mode  = ( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )? 0x4000 : 0x8000;
			
			FindClose ( file );			
		}
		else
		{
			xmWinDosMapErr ( GetLastError ( ) );		
		}

		KD_FREE_TSTR ( tpath );	
	}
	else
	{
		xmPlatformSetErrno ( ENOMEM );
	}

	return ret;
}

// xmWinAccess : Determine whether the application can access a file or directory.
// XMSoft's revision : Unexpectedly processed in each platform
int	xmWinAccess ( const char *path, int amode )
{
	TCHAR*      tpath	= 0;
	DWORD       attr	= 0;
	int         ret		= -1;	
	
	if ( XM_SET_TSTR ( tpath, path ) )
	{
		if ( ( attr = GetFileAttributes ( tpath ) ) != 0xffffffff )
		{
			if ( amode & W_OK )
			{                
                attr &= ~FILE_ATTRIBUTE_READONLY;
			}
			else 
			{					
				attr |=  FILE_ATTRIBUTE_READONLY;
			}
			
			if ( SetFileAttributes ( tpath, attr) ) 
			{    
				ret = 0;
			}
			else
			{
				xmWinDosMapErr ( GetLastError ( ) );
			}
		}
		else
		{
			xmWinDosMapErr ( GetLastError ( ) );	
		}

		KD_FREE_TSTR ( tpath );	
	}
	else
	{
		xmPlatformSetErrno ( ENOMEM );
	}		

	return ret;
}

// xmWinOpen : Open a file.
int xmWinOpen ( const char* pathname, int flags, int mode )
{
	return -1;
}

// xmWinClose : Close a file.
int xmWinClose ( int fd )
{
	return -1;
}

// xmWinTell : Determine the current file position
KD_API KDint KD_APIENTRY xmWinTell ( KDint fd )
{
	return -1;
}

// xmWinLseek : Set position in a file.
int xmWinLseek ( int fd, int position, int startpoint )
{
	return -1;
}

// xmWinRead : Read from a file.
int xmWinRead ( int fd, void* ptr, int len )
{
	return -1;
}

// xmWinWrite : Write to a file. 
int xmWinWrite ( int fd, void* ptr, int len )
{
	return -1;
}

// xmWinChdir : Change working directory
KDint xmWinChdir ( const KDchar* path )
{
	return -1;
}
#endif

