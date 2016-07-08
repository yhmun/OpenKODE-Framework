/* --------------------------------------------------------------------------
 *
 *      File            APSObject.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSObject_h__
#define __APSObject_h__

#include "APSSharedType.h"
#include <string>
#include <vector>
#include <map>

NS_APS_BEGIN

class APSObject;

class APSObject 
{
	public :

			APSObject ( KDvoid );

			APS_CLASS_NAME ( APSObject );
		    
	public :

			APSObject*  retain ( KDvoid );
			
			KDvoid      release ( KDvoid );

			APSObject*  autorelease ( KDvoid );

			APS_PROPERTY_KEY ( ClassName );
		    
			APS_SYNTHESIZE ( KDint, m_nRetainCount, RetainCount );
};

NS_APS_END

#endif	// __APSObject_h__
