/* --------------------------------------------------------------------------
 *
 *      File            khr_formatted.h
 *      Description     Formatted input and output functions.
 *      Version         10, 2009-04-02
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __kd_KHR_formatted_h_
#define __kd_KHR_formatted_h_

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KDVaListKHR
typedef KDvoid*                 KDVaListKHR; 
#endif

// kdSnprintfKHR, kdVsnprintfKHR, kdSprintfKHR, kdVsprintfKHR : Formatted output to a buffer.
KD_API KDint KD_APIENTRY kdSnprintfKHR ( KDchar* buf, KDsize bufsize, const KDchar* format, ... );
KD_API KDint KD_APIENTRY kdVsnprintfKHR ( KDchar* buf, KDsize bufsize, const KDchar* format, KDVaListKHR ap );
KD_API KDint KD_APIENTRY kdSprintfKHR ( KDchar* buf, const KDchar* format, ... );
KD_API KDint KD_APIENTRY kdVsprintfKHR ( KDchar* buf, const KDchar* format, KDVaListKHR ap );

// kdFprintfKHR, kdVfprintfKHR : Formatted output to an open file.
KD_API KDint KD_APIENTRY kdFprintfKHR ( KDFile* file, const KDchar *format, ... );
KD_API KDint KD_APIENTRY kdVfprintfKHR ( KDFile* file, const KDchar *format, KDVaListKHR ap );

// kdLogMessagefKHR : Formatted output to the platform's debug logging facility.
#ifdef KD_NDEBUG
#define kdLogMessagefKHR(s, ...)
#define kdVlogMessagefKHR(s, ap)
#else
KD_API KDint KD_APIENTRY kdLogMessagefKHR ( const KDchar* format, ... );
KD_API KDint KD_APIENTRY kdVlogMessagefKHR ( const KDchar* format, KDVaListKHR ap );

#endif

// kdSscanfKHR, kdVsscanfKHR : Read formatted input from a buffer.
KD_API KDint KD_APIENTRY kdSscanfKHR ( const KDchar* str, const KDchar* format, ... );
KD_API KDint KD_APIENTRY kdVsscanfKHR ( const KDchar* str, const KDchar* format, KDVaListKHR ap );

// kdFscanfKHR, kdVfscanfKHR : Read formatted input from a file. 
KD_API KDint KD_APIENTRY kdFscanfKHR ( KDFile* file, const KDchar* format, ... );
KD_API KDint KD_APIENTRY kdVfscanfKHR ( KDFile* file, const KDchar* format, KDVaListKHR ap );

#define kdSnprintf			kdSnprintfKHR
#define kdVsnprintf			kdVsnprintfKHR
#define kdVsnprintf			kdVsnprintfKHR
#define kdSprintf			kdSprintfKHR
#define kdVsprintf			kdVsprintfKHR
#define kdFprintf			kdFprintfKHR
#define kdVfprintf			kdVfprintfKHR
#define kdLogMessagef		kdLogMessagefKHR
#define kdVlogMessagef		kdVlogMessagefKHR
#define kdSscanf			kdSscanfKHR
#define kdVsscanf			kdVsscanfKHR
#define kdFscanf			kdFscanfKHR 
#define kdVfscanf			kdVfscanfKHR 
#define kdPrintf			kdLogMessagefKHR

#ifdef __cplusplus
}
#endif

#endif

 

