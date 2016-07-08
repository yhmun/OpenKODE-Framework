/* --------------------------------------------------------------------------
 *
 *      File            tizen_file.cpp
 *      Description     File system
 *
 *      Author          Young-Hwan Mun
 *                      Hyeon-Ho Jeong
 *
 *      Contact         xmsoft77@gmail.com
 *                      skdldnjs@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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
#include <FApp.h>
#include <FBase.h>
#include <FIo.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Io;

static KDbool convString2Utf8 ( KDchar* pszDst, KDsize uMaxLen, String strSrc )
{
	ByteBuffer*  pBuff = KD_NULL;
	KDsize       uCnt  = 0;

	pBuff = StringUtil::StringToUtf8N ( strSrc );
	if ( !pBuff )
	{
		goto end;
	}

	while ( pBuff->HasRemaining ( ) )
	{
		byte   b;
		pBuff->GetByte ( b );

		if ( uCnt < ( uMaxLen - 1 ) )
		{
			pszDst [ uCnt++ ] = b;
		}
		else
		{
			break;
		}
	}

	pszDst [ uCnt ] = '\0';

end :

	if ( pBuff )
	{
		delete pBuff;
	}


	return KD_TRUE;
}

extern "C"
{

// xmPlatformMapDirectoryPath : Map absoulte path to virtual path.
KDvoid xmPlatformMapDirectoryPath ( KDchar** paths )
{
	KDchar   szRoot [256];
	KDchar   szRes  [256];
	KDchar   szData [256];
	KDchar   szShare[256];
	KDint    i;

	convString2Utf8 ( szRoot , 256, App::GetInstance ( )->GetAppRootPath     ( ) );
	convString2Utf8 ( szRes  , 256, App::GetInstance ( )->GetAppResourcePath ( ) );
	convString2Utf8 ( szData , 256, App::GetInstance ( )->GetAppDataPath     ( ) );
	convString2Utf8 ( szShare, 256, App::GetInstance ( )->GetAppSharedPath   ( ) );

	for ( i = 0; i < XM_DIRECTORY_COUNT; i++ )
	{
		if ( ( paths[ i ] = (KDchar *) kdCalloc ( 1, 256 ) ) )
		{
			switch ( i )
			{
				case 0 : strcpy  ( paths[ i ], szRes ); 			break; // "/res"
				case 1 : strcpy  ( paths[ i ], szData ); 			break; // "/data"
				case 2 : sprintf ( paths[ i ], "%sTemp", szData ); 	break; // "/tmp"
				case 3 : break; // "/removable"
				case 4 : strcpy  ( paths[ i ], szShare ); 			break; // "/storage"
				case 5 : break; // "/native"
				case 6 : strcpy  ( paths[ i ], szRoot ); 			break; // "/"
			}
		}
		else
		{
			kdLogMessage ( "Alloc memory failed." );
			kdExit ( -3 );
		}
	}
}

// xmPlatformGetFree : Get free space on a drive in in platform specific.
KDoff xmPlatformGetFree ( const KDchar* path )
{
	return 0;
}

}	// extern "C"


