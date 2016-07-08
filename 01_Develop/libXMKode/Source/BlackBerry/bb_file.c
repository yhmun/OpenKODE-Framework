/* --------------------------------------------------------------------------
 *
 *      File            bb_file.c
 *      Description     File system
 *      Author          H.S Lim
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
	KDint    i;

	for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
	{
		if ( ( paths[ i ] = (KDchar *) kdCalloc ( 1, 256 ) ) )
		{
			switch ( i )
			{
				case 0 : strcpy ( paths[ i ], "/Home/Resource" );     break; // "/res"
				case 1 : strcpy ( paths[ i ], "/Home/Data" );         break; // "/data"
				case 2 : strcpy ( paths[ i ], "/Home/Temp" );         break; // "/tmp"
				case 3 : strcpy ( paths[ i ], "/Storagecard");        break; // "/removable"
				case 4 : strcpy ( paths[ i ], "/Storagecard/Media");  break; // "/storage"
				case 5 : break; // "/native"
				case 6 : strcpy ( paths[ i ], "/Home" );              break; // "/"
			}
		}
		else
		{
			kdLogMessage ( "Alloc memory failed." );
			kdExit ( -3 );
		}
	}
}
/*
KDvoid static xmBbSetError ( result r )
{
	switch ( r )
	{
		case E_INVALID_ARG        :	errno = EINVAL;	break;
		case E_ILLEGAL_ACCESS     :	errno = EEXIST;	break;
		case E_FILE_ALREADY_EXIST :	errno = EEXIST;	break;
		case E_FILE_NOT_FOUND     :	errno = ENOENT;	break;
		case E_MAX_EXCEEDED       :	errno = EMFILE; break;
		case E_IO                 :	errno = EIO;    break;
	}
}

KDint xmBbMkdir ( const KDchar* path, mode_t mode )
{
	result    r;

	r = Directory::Create ( path );

	if ( IsFailed ( r ) )
	{
		xmBbSetError ( r );
		return -1;
	}

	return 0;
}

KDint xmBbRmdir ( const KDchar* path )
{
	result    r;

	r = Directory::Remove ( path );

	if ( IsFailed ( r ) )
	{
		xmBbSetError ( r );
		return -1;
	}

	return 0;
}

KDint xmBbRename ( const KDchar* _old, const KDchar* _new )
{
	FileAttributes attr;
	result         r;

	r = File::GetAttributes ( _old, attr );

	if ( IsFailed ( r ) )
	{
		goto failed;
	}

	if ( attr.IsDirectory ( ) )
	{
		r = Directory::Rename ( _old, _new );
	}
	else
	{
		r = File::Move ( _old, _new );
	}

	if ( IsFailed ( r ) )
	{
		goto failed;
	}

	return 0;

failed :

	xmBbSetError ( r );

	return -1;
}

KDint xmBbTruncate ( const KDchar* path, off_t length )
{
	File     file;
	result   r;

	r = file.Construct ( path, "wb" );

	if ( IsFailed ( r ) )
	{
		goto failed;
	}

	r = file.Truncate ( (KDint) length );

	if ( IsFailed ( r ) )
	{
		goto failed;
	}

	return 0;

failed :

	xmBbSetError ( r );

	return -1;
}

KDint xmBbStat ( const KDchar* path, struct stat* buf )
{
	struct tm       t;
	FileAttributes  attr;
	DateTime        dt;
	result          r;

	r = File::GetAttributes ( path, attr );

	if ( IsFailed ( r ) )
	{
		goto failed;
	}

	dt = attr.GetLastModifiedTime ( );

	t.tm_year = dt.GetYear   ( );
	t.tm_mon  = dt.GetMonth  ( ) - 1;
	t.tm_mday = dt.GetDay    ( );
	t.tm_hour = dt.GetHour   ( );
	t.tm_min  = dt.GetMinute ( );
	t.tm_sec  = dt.GetSecond ( );

	buf->st_mtime = mktime ( &t );
	buf->st_mode  = attr.IsDirectory ( ) ? 0x4000 : 0x8000;
	buf->st_size  = attr.GetFileSize ( );

	return 0;

failed :

	xmBbSetError ( r );

	return -1;
}

KDint xmBbAccess ( const KDchar* path, KDint amode )
{
	errno = ENOSPC;

	return -1;
}

struct DIR
{
	DirEnumerator*    dir_enum;
	struct dirent     dir_info;
};

XMDir* xmBbOpendir ( const KDchar* dirname )
{
	Directory   dir;
	XMDir*      ret;
	result      r;

	r = dir.Construct ( dirname );

	if ( IsFailed ( r ) )
	{
		ret = 0;
		goto failed;
	}

	ret = (XMDir *) kdMalloc ( sizeof ( XMDir ) );
	if ( ret )
	{
		ret->dir_enum = dir.ReadN ( );
		ret->dir_info.d_name = (const KDchar *) kdMalloc ( 256 );
	}
	else
	{
		goto failed;
	}

	return ret;

failed :

	if ( ret )
	{
		kdFree( ret );
		ret = 0;
	}

	xmBbSetError ( r );

	return ret;
}

XMDirent* xmBbReaddir ( XMDir* dirp )
{
	XMDirent*    ret = 0;

	if ( dirp->dir_enum->MoveNext ( ) == E_SUCCESS )
	{
		DirEntry    entry = dirp->dir_enum->GetCurrentDirEntry ( );
		String      name  = entry.GetName ( );
		ByteBuffer* utf8  = StringUtil::StringToUtf8N ( name );

		strcpy ( (KDchar *) dirp->dir_info.d_name, (const KDchar *) utf8->GetPointer ( ) );
		ret = &dirp->dir_info;
	}

	return ret;
}

KDint xmBbClosedir ( XMDir* dirp )
{
	kdFree ( (KDvoid *) dirp->dir_info.d_name );
	kdFree ( dirp );

	return 0;
}

// xmPlatformGetFree : Get free space on a drive in in platform specific.
KDoff xmPlatformGetFree ( const KDchar* path )
{
	result 	   r;
	long long  ret = 0;

	if ( !strncmp ( path, "/Storagecard", 12 ) )
	{
		r = RuntimeInfo::GetValue ( "AvailableExternalStorage", ret );
	}
	else
	{
		r = RuntimeInfo::GetValue ( "AvailableInternalStorage", ret );
	}

	if ( IsFailed ( r ) )
	{
		errno = EIO;
	}

	return (KDoff) ret;
}
*/
