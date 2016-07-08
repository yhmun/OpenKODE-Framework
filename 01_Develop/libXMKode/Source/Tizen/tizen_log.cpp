/* --------------------------------------------------------------------------
 *
 *      File            tizen_log.cpp
 *      Description     Log message
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


class A
{
public :
	A ( )
	{

	}

		void print ( const KDchar* msg )
		{
			AppLogTag ( "XMKode", msg );
		}
};

extern "C"
{


// xmPlatformLogMessage : Output a log message in in platform specific.
KDvoid xmPlatformLogMessage ( const KDchar* msg )
{
	A  a;
	a.print ( msg );
	//AppLogTag ( "XMKode", msg );
}


}

