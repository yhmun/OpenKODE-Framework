/* --------------------------------------------------------------------------
 *
 *      File            android_file.c
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

#undef fopen

#define XM_MEMORY_OPEN   ( 512 * 1024 )
#define XM_BUFFER_SIZE   ( 1024 * 1024 )

static KDbyte         l_buffer[ XM_BUFFER_SIZE ];

static KDchar*        l_resdir  = 0;
static struct zip*    l_apkfile = 0;

// xmPlatformMapDirectoryPath : Map absoulte path to virtual path.
KDvoid xmPlatformMapDirectoryPath ( KDchar** paths )
{
	KDint    len;
	KDint    i;
	
	len = strlen ( g_argv[ 1 ] );
	
	for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
	{
		if ( ( paths[ i ] = kdCalloc ( 1, 256 ) ) )
		{
			switch ( i ) 
			{									
				case 0 : sprintf ( paths[ i ], "%s/assets", g_argv[ 1 ] );  break;  // "/res"
				case 1 : sprintf ( paths[ i ], "%s/files" , g_argv[ 1 ] );  break;  // "/data"
				case 2 : sprintf ( paths[ i ], "%s/tmp"   , g_argv[ 1 ] );  break;  // "/tmp"
				case 3 : sprintf ( paths[ i ], "%s"       , g_argv[ 2 ] );  break;  // "/removable"
				case 4 : sprintf ( paths[ i ], "%s"       , g_argv[ 2 ] );  break;  // "/storage"
				case 5 :                                                    break;  // "/native"
				case 6 : sprintf ( paths[ i ], "%s"       , g_argv[ 1 ] );  break;  // "/"
			}		
		}
		else
		{													
			kdLogMessage ( "Alloc memory failed." );
			kdExit ( -3 );
		}
	}	
	
	l_resdir = paths[ 0 ];
}

KDvoid xmAndroidResInit ( KDvoid )
{
	l_apkfile = zip_open ( g_argv[ 0 ], 0, 0 );
	if ( !l_apkfile )
	{
		kdLogMessage ( "Apk file open failed." );
		kdExit ( -3 );
	}
}

KDvoid xmAndroidResFree ( KDvoid )
{
	if ( l_apkfile )
	{
		zip_close ( l_apkfile );
		l_apkfile = 0;
	}
}

static KDint xmAndroidGetZipError ( KDvoid )
{
	KDint    zerr = 0;
	KDint    err  = 0;
	
	zip_error_get ( l_apkfile, &zerr, 0 );

	switch ( zerr )
	{
		case ZIP_ER_OK          : err = 0;      break;        // N No error 
		case ZIP_ER_MULTIDISK   : err = EIO;    break;        // N Multi-disk zip archives not supported 
		case ZIP_ER_RENAME      : err = EIO;    break;        // S Renaming temporary file failed 
		case ZIP_ER_CLOSE       : err = EIO;    break;        // S Closing zip archive failed 
		case ZIP_ER_SEEK        : err = EIO;    break;        // S Seek error 
		case ZIP_ER_READ        : err = EIO;    break;        // S Read error 
		case ZIP_ER_WRITE       : err = EIO;    break;        // S Write error 
		case ZIP_ER_CRC         : err = EINVAL; break;        // N CRC error 
		case ZIP_ER_ZIPCLOSED   : err = EINVAL; break;        // N Containing zip archive was closed 
		case ZIP_ER_NOENT       : err = ENOENT; break;        // N No such file 
		case ZIP_ER_EXISTS      : err = EEXIST; break;        // N File already exists 
		case ZIP_ER_OPEN        : err = EBADF;  break;        // S Can't open file 
		case ZIP_ER_TMPOPEN     : err = EBADF;  break;        // S Failure to create temporary file 
		case ZIP_ER_ZLIB        : err = EINVAL; break;        // Z Zlib error 
		case ZIP_ER_MEMORY      : err = ENOMEM; break;        // N Malloc failure 
		case ZIP_ER_CHANGED     : err = EINVAL; break;        // N Entry has been changed 
		case ZIP_ER_COMPNOTSUPP	: err = EINVAL; break;        // N Compression method not supported 
		case ZIP_ER_EOF         : err = EOF;    break;        // N Premature EOF 
		case ZIP_ER_INVAL       : err = EINVAL; break;        // N Invalid argument 
		case ZIP_ER_NOZIP       : err = EINVAL; break;        // N Not a zip archive 
		case ZIP_ER_INTERNAL    : err = EINVAL; break;        // N Internal error 
		case ZIP_ER_INCONS      : err = EINVAL; break;        // N Zip archive inconsistent 
		case ZIP_ER_REMOVE      : err = EINVAL; break;        // S Can't remove file 
		case ZIP_ER_DELETED     : err = EINVAL; break;        // N Entry has been deleted 		
	}
	
	return err;
}

KDFile* xmAndroidFopen ( const KDchar* filename, const KDchar* mode )
{
    KDFile*  ret = 0;
	KDvoid*  buf = 0;
    
	if ( !strncmp ( filename, l_resdir, strlen ( l_resdir ) ) )
	{
		const  KDchar*    zname = &filename[ strlen ( g_argv[ 1 ] ) + 1 ];
		struct zip_file*  zfile = 0;
		struct zip_stat	  zstat;
		struct stat       fstat;
		
		if ( !zip_stat ( l_apkfile, zname, 0, &zstat )  )
		{
			if ( zstat.size > XM_MEMORY_OPEN )
			{
				FILE*  file   = 0;
				KDint  update = 1;
										
				if ( !stat ( filename, &fstat ) && zstat.size == fstat.st_size )
				{					
					update = 0; 
				}

				if ( update )
				{									
					if ( zfile = zip_fopen ( l_apkfile, zname, 0 ) )
					{
						KDint      i   = strlen ( l_resdir ) + 1;
						KDint      len = strlen ( filename );
						KDchar*	   dir = malloc ( len );						
					
						memset ( dir, 0, len );
						strncpy ( dir, filename, i );
										
						while ( i < len )
						{							
							if ( filename[ i ] == '/' )
							{								
								KDint ret2 = mkdir ( dir, 0755 );
								if ( ret2 != 0 && errno == EEXIST )
								{
									errno = 0;
								}
							}
						
							dir[ i ] = filename[ i ];	
							i++;						
						}

						if ( ( file = fopen ( filename, "wb" ) ) )
						{
							struct    utimbuf ubuf; 

							KDuint    bytes;
													
							ubuf.actime  = zstat.mtime;
							ubuf.modtime = zstat.mtime;
							
							while ( ( bytes = zip_fread ( zfile, l_buffer, XM_BUFFER_SIZE ) ) > 0 )
							{
								fwrite ( l_buffer, bytes, 1, file );
							}
							
							fclose ( file );						
							utime ( filename, &ubuf );
						}
						else
						{
							return 0;
						}		
					
						zip_fclose ( zfile );
					}
					else
					{
						return 0;
					}
				}

				file = fopen ( filename, mode );
				if ( ( ret = (KDFile *) kdMalloc ( sizeof ( KDFile ) ) ) )
				{
					ret->ptr  = file;
					ret->path = (KDchar*) filename;
					ret->mem  = 0;
					ret->off  = 0;
					ret->size = 0;
				}
				else
				{
					kdSetError ( KD_ENOMEM );
				}
			}			
            else if ( zfile = zip_fopen ( l_apkfile, zname, 0 ) )
            {
                buf = kdMalloc ( zstat.size );

                if ( zip_fread ( zfile, buf, zstat.size ) == zstat.size )
                {
                    ret = kdFmemopen ( buf, zstat.size, mode );
                    if ( ret )
                    {
                        ret->path = (KDchar*) filename;
                    }
                }
				
                zip_fclose ( zfile );
            }
            else
            {
                errno = xmAndroidGetZipError ( );
            }			
		}
	}
    
    if ( !ret && buf )
    {
        kdFree ( buf );
    }
    
	return ret;
}

KDint xmAndroidStat ( const KDchar* pathname, struct KDStat* kstat )
{
    KDint  ret = -1;
    
	if ( !strncmp ( pathname, l_resdir, strlen ( l_resdir ) ) )
	{
		const  KDchar*    zname = &pathname[ strlen ( g_argv[ 1 ] ) + 1 ];
		struct zip_stat	  zstat;
		
		if ( !zip_stat ( l_apkfile, zname, 0, &zstat )  )
		{
			kstat->st_mode  = 0;
			kstat->st_size  = (KDoff)  zstat.size;
			kstat->st_mtime = (KDtime) zstat.mtime;		
			ret = 0;
		}
	}
    
	return ret;
}

KDDir* xmAndroidOpenDir ( const KDchar* pathname )
{
	KDDir*  ret = KD_NULL;

	if ( ( ret = (KDDir *) kdMalloc ( sizeof ( KDDir ) ) ) )
	{
		const  KDchar*  zname = &pathname[ strlen ( g_argv[ 1 ] ) + 1 ];
		const  KDchar*  fname;
		KDint  i, len;

		ret->ptr = KD_NULL;
		ret->idx = 0;
		ret->cnt = zip_get_num_files ( l_apkfile );

		len = kdStrlen ( zname );
		for ( i = 0; i < ret->cnt; i++ )
		{
			fname = zip_get_name ( l_apkfile, i, 0 );
			if ( !kdStrncmp ( zname, fname, len ) )
			{
				ret->idx = i;
				kdStrncpy ( ret->path, zname, 256 );	
				return ret;
			}
		}
			
		kdFree ( ret );
		ret = KD_NULL;
	}
	else
	{
		kdSetError ( KD_ENOMEM );
	}

	return ret;
}

KDDirent* xmAndroidReadDir ( KDDir* dir )
{
	if ( dir->idx < dir->cnt )
	{
		const  KDchar*  zname = dir->path;
		const  KDchar*  fname = zip_get_name ( l_apkfile, dir->idx, 0 );
		
		KDsize  len = kdStrlen ( zname );

		if ( !kdStrncmp ( zname, fname, len ) )
		{
			KDint  i = len + 1;		
			KDint  len2 = kdStrlen ( fname );							

			dir->idx++;

			for ( ; i < len2; i++ )
			{
				if ( fname[i] == '/' )
				{
					return xmAndroidReadDir ( dir );
				}
			}
			
			dir->dirent.d_name = &fname[len + 1];
			return &dir->dirent;
		}
	}

	return KD_NULL;
}

KDint xmAndroidOpen ( const KDchar* filename, KDint flags, KDint mode )
{
    KDint    fd  = 0;
    
	if ( !strncmp ( filename, l_resdir, strlen ( l_resdir ) ) )
	{
		const  KDchar*    zname = &filename[ strlen ( g_argv[ 1 ] ) + 1 ];
		struct zip_file*  zfile = 0;
		struct zip_stat	  zstat;
		struct stat       fstat;
		
		if ( !zip_stat ( l_apkfile, zname, 0, &zstat )  )
		{
			FILE*  file   = 0;
			KDint  update = 1;
										
			if ( !stat ( filename, &fstat ) && zstat.size == fstat.st_size )
			{					
				update = 0; 
			}

			if ( update )
			{									
				if ( zfile = zip_fopen ( l_apkfile, zname, 0 ) )
				{
					KDint      i   = strlen ( l_resdir ) + 1;
					KDint      len = strlen ( filename );
					KDchar*	   dir = malloc ( len );						
					
					memset ( dir, 0, len );
					strncpy ( dir, filename, i );
									
					while ( i < len )
					{							
						if ( filename[ i ] == '/' )
						{								
							KDint ret2 = mkdir ( dir, 0755 );
							if ( ret2 != 0 && errno == EEXIST )
							{
								errno = 0;
							}
						}
					
						dir[ i ] = filename[ i ];	
						i++;						
					}

					if ( ( file = fopen ( filename, "wb" ) ) )
					{
						struct    utimbuf ubuf; 

						KDuint    bytes;
												
						ubuf.actime  = zstat.mtime;
						ubuf.modtime = zstat.mtime;
						
						while ( ( bytes = zip_fread ( zfile, l_buffer, XM_BUFFER_SIZE ) ) > 0 )
						{
							fwrite ( l_buffer, bytes, 1, file );
						}
						
						fclose ( file );						
						utime ( filename, &ubuf );
					}
					else
					{
						return 0;
					}		
				
					zip_fclose ( zfile );
				}
				else
				{
					return 0;
				}				
			}		

			fd = open ( filename, flags, mode );
		}
	}

	return fd;
}

/*
FILE* xmAndroidFopen ( const KDchar* filename, const KDchar* mode )
{
	FILE*    file = 0;
	KDint    ret  = 0;
	
	if ( !strncmp ( filename, l_resdir, strlen ( l_resdir ) ) )
	{
		const  KDchar*       zname = &filename[ strlen ( g_argv[ 1 ] ) + 1 ];
		struct zip_file*     zfile = 0;
		struct zip_stat	     zstat;
		struct stat          fstat;		
		
		if ( !zip_stat ( l_apkfile, zname, 0, &zstat )  )
		{
			KDint    update = 1;
									
			if ( !stat ( filename, &fstat ) && zstat.size == fstat.st_size )
			{					
				update = 0;
			}

			if ( update )
			{									
				if ( zfile = zip_fopen ( l_apkfile, zname, 0 ) )
				{
					KDint      i   = strlen ( l_resdir ) + 1;
					KDint      len = strlen ( filename );
					KDchar*	   dir = malloc ( len );
					
					memset ( dir, 0, len );
					strncpy ( dir, filename, i );
										
					while ( i < len )
					{							
						if ( filename[ i ] == '/' )
						{								
							ret = mkdir ( dir, 0755 );
							if ( ret != 0 && errno == EEXIST )
							{
								errno = 0;
								ret   = 0;
							}
						}
						
						dir[ i ] = filename[ i ];	
						i++;						
					}

					if ( ( file = fopen ( filename, "wb" ) ) )
					{
						struct    utimbuf ubuf; 

						KDuint    bytes;
												
						ubuf.actime  = zstat.mtime;
						ubuf.modtime = zstat.mtime;
						
						while ( ( bytes = zip_fread ( zfile, l_buffer, XM_BUFFER_SIZE ) ) > 0 )
						{
							fwrite ( l_buffer, bytes, 1, file );
						}
						
						fclose ( file );						
						utime ( filename, &ubuf );
					}
					else
					{
						ret = -1;
					}		
					
					zip_fclose ( zfile );
				}
				else
				{
					errno = xmAndroidGetZipError ( );
					ret   = -1;
				}
			}
		}
		else
		{
			errno = EEXIST;
			ret   = -1;
		}
	}
	
	if ( ret != 0 )
	{
		return 0;
	}

	return fopen ( filename, mode );
}
*/

// xmPlatformGetFree : Get free space on a drive in in platform specific.
KDoff xmPlatformGetFree ( const KDchar* path )
{
	return 0;
}