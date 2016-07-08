/* --------------------------------------------------------------------------
 *
 *      File            linux_file.c
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
	KDchar    rootdir[256] = "";
	KDint     rootlen = 0;
	KDint     i;
	
	if ( ( getcwd ( rootdir, 256 ) ) )
	{
		rootlen = kdStrlen ( rootdir );

		for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
		{
			if ( ( paths[ i ] = (KDchar *) kdCalloc ( 1, rootlen + 25 ) ) )
			{
				switch ( i ) 
				{					
					case 0 : kdSprintfKHR ( paths[ i ], "%s/../../Resource" , rootdir ); break; // "/res"	
					case 1 : kdSprintfKHR ( paths[ i ], "%s/../../Data"		, rootdir ); break; // "/data"					
					case 2 : kdSprintfKHR ( paths[ i ], "%s/Temp"			, rootdir ); break; // "/tmp"	
					case 3 : break;	// "/removable"				
					case 4 : kdSprintfKHR ( paths[ i ], "/media/psf/Home/XMStorage"); break; // "/storage"
					case 5 : break; // "/native"
					case 6 : kdSprintfKHR ( paths[ i ], "%s"           , rootdir ); break; // "/"	
				}		
			}
			else
			{
				kdLogMessage ( "Alloc memory failed." );
				kdExit ( -3 );
			}
		}
	}
	else
	{
		kdLogMessage ( "Get root directory failed." );
		kdExit ( -3 );
	}
}

