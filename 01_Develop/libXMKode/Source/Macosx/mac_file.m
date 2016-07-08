/* --------------------------------------------------------------------------
 *
 *      File            ios_file.m
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

#import  <Foundation/Foundation.h>

// xmPlatformMapDirectoryPath : Map absoulte path to virtual path.
KDvoid xmPlatformMapDirectoryPath ( KDchar** paths )
{
    NSString*          bid = [[NSBundle mainBundle] bundleIdentifier];
    NSFileManager*    fmgr = [NSFileManager defaultManager];    
    NSArray*          adir = [fmgr URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    NSURL*            ddir = nil;
    
    if ( [adir count] > 0 )
    {
        ddir = [[adir objectAtIndex:0] URLByAppendingPathComponent:bid];
        
        NSError*       err = nil;
        [fmgr createDirectoryAtURL:ddir withIntermediateDirectories:YES attributes:nil error:&err];
    }
    
	NSArray*    adocu    = NSSearchPathForDirectoriesInDomains ( NSDocumentDirectory, NSUserDomainMask, YES );
    NSString*   sharedir = [adocu objectAtIndex:0];
    NSString*   datdir   = [ddir path];
    NSString*   rootdir  = [[NSBundle mainBundle] executablePath];
	NSString*   resdir   = [[NSBundle mainBundle] resourcePath];
	NSString*   tempdir  = NSTemporaryDirectory ( );
	
	KDsize      len;
	int         i;
	
	if ( rootdir && tempdir )
	{
		for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
		{		
			switch ( i )
			{
				case 0  : len = [resdir   length] +  1; break;
				case 1  : len = [datdir   length] +  1; break;
				case 2  : len = [tempdir  length] +  1; break;
                case 4  : len = [sharedir length] + 10; break;
				case 6  : len = [rootdir  length] +  1; break;                
				default : len = [rootdir  length] + 10; break;
			}
			
			if ( ( paths[i] = kdCalloc ( 1, len ) ) )
			{				
				switch ( i ) 
				{											
					case 0 : strcpy  ( paths[ i ], [resdir  UTF8String] );                  break; // "/res"
					case 1 : strcpy  ( paths[ i ], [datdir  UTF8String] );                  break; // "/data"
					case 2 : strcpy  ( paths[ i ], [tempdir UTF8String] );                  break; // "/tmp"
					case 3 :                                                                break; // "/removable"
					case 4 : sprintf ( paths[ i ], "%s/XMStorage", [sharedir UTF8String] ); break; // "/storage"
					case 5 :                                                                break; // "/native"
					case 6 : strcpy  ( paths[ i ], [rootdir UTF8String] );                  break; // "/"
				}				
			}
			else
			{
				kdLogMessage ( "Alloc memory failed." );
				kdExit ( 3 );
			}
		}
	}
	else
	{				
		kdLogMessage ( "Get root directory failed." );
		kdExit ( 3 );
	}	
}




