/* --------------------------------------------------------------------------
 *
 *      File            khr_formatted.c
 *      Description     Formatted input and output functions.
 *      Version         10, 2009-04-02
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

#define	XM_LOG_MAX			1024
static KDchar				l_log [ XM_LOG_MAX ] = "";

// kdSnprintfKHR, kdVsnprintfKHR, kdSprintfKHR, kdVsprintfKHR : Formatted output to a buffer.
KD_API KDint KD_APIENTRY kdSnprintfKHR ( KDchar* buf, KDsize bufsize, const KDchar* format, ... )
{
	KDVaListKHR     ap;
	KDint           ret;
	
	KD_VA_START_KHR ( ap, format );

	ret = kdVsnprintfKHR ( buf, bufsize, format, ap );
	
	KD_VA_END_KHR( ap );
	
	return ret;
}

KD_API KDint KD_APIENTRY kdVsnprintfKHR ( KDchar* buf, KDsize bufsize, const KDchar* format, KDVaListKHR ap )
{	
	return vsnprintf ( buf, bufsize, format, ap );
}

KD_API KDint KD_APIENTRY kdSprintfKHR ( KDchar* buf, const KDchar* format, ... )
{	
	KDVaListKHR     ap;
	KDint           ret;
	
	KD_VA_START_KHR ( ap, format );

	ret = kdVsprintfKHR ( buf, format, ap );
	
	KD_VA_END_KHR ( ap );
	
	return ret;
}

KD_API KDint KD_APIENTRY kdVsprintfKHR ( KDchar* buf, const KDchar* format, KDVaListKHR ap )
{	
	return vsprintf ( buf, format, ap );
}

// kdFprintfKHR, kdVfprintfKHR : Formatted output to an open file.
KD_API KDint KD_APIENTRY kdFprintfKHR ( KDFile* file, const KDchar *format, ... )
{
	KDVaListKHR     ap;
	KDint           ret;

	KD_VA_START_KHR ( ap, format );

	ret = kdVfprintfKHR ( file, format, ap );
	
	KD_VA_END_KHR ( ap );

	return ret;
}

KD_API KDint KD_APIENTRY kdVfprintfKHR ( KDFile* file, const KDchar *format, KDVaListKHR ap )
{
	return vfprintf ( file->ptr, format, ap );
}

// kdLogMessagefKHR : Formatted output to the platform's debug logging facility.
KD_API KDint KD_APIENTRY kdLogMessagefKHR ( const KDchar* format, ... )
{
	KDVaListKHR     ap;
	KDint           ret;
	
 	KD_VA_START_KHR ( ap, format );

 	ret = kdVsnprintfKHR ( l_log, XM_LOG_MAX, format, ap );
 	
	KD_VA_END_KHR ( ap );

	kdLogMessage ( l_log );

	return ret;	
}

KD_API KDint KD_APIENTRY kdVlogMessagefKHR ( const KDchar* format, KDVaListKHR ap )
{
	KDint  ret;

	ret = kdVsnprintfKHR ( l_log, XM_LOG_MAX, format, ap );

	kdLogMessage ( l_log );

	return ret;	
}

// kdSscanfKHR, kdVsscanfKHR : Read formatted input from a buffer.
KD_API KDint KD_APIENTRY kdSscanfKHR ( const KDchar* str, const KDchar* format, ... )
{	
	KDVaListKHR     ap;
	KDint           ret;	
	
	KD_VA_START_KHR ( ap, format );

	ret = kdVsscanfKHR ( str, format, ap );

	KD_VA_END_KHR ( ap );

	return ret;	
}

KD_API KDint KD_APIENTRY kdVsscanfKHR ( const KDchar* str, const KDchar* format, KDVaListKHR ap )
{	
	return vsscanf ( str, format, ap );
}

// kdFscanfKHR, kdVfscanfKHR : Read formatted input from a file. 
KD_API KDint KD_APIENTRY kdFscanfKHR ( KDFile* file, const KDchar* format, ... )
{
	KDVaListKHR     ap;
	KDint           ret;		

	KD_VA_START_KHR ( ap, format );

	ret = kdVfscanfKHR ( file, format, ap );
	
	KD_VA_END_KHR ( ap );

	return ret;	
}

KD_API KDint KD_APIENTRY kdVfscanfKHR ( KDFile* file, const KDchar* format, KDVaListKHR ap )
{		
	return vfscanf ( file->ptr, format, ap );
}
