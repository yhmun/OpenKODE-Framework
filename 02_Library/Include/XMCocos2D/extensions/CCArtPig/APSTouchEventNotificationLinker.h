/* --------------------------------------------------------------------------
 *
 *      File            APSTouchEventNotificationLinker.h
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

#ifndef __APSTouchEventNotificationLinker_h__
#define __APSTouchEventNotificationLinker_h__

#include "APSLinkerAction.h"
#include "APSTouchEventObserver.h"

NS_APS_BEGIN

class APSTouchEventNotificationLinker : public APSLinkerAction
{
	public :
    
		APS_CLASS_NAME ( APSTouchEventNotificationLinker );
    
		APS_SYNTHESIZE ( APSTouchEventObserver*, m_pObserver, Observer );
};

NS_APS_END

#endif	// __APSTouchEventNotificationLinker_h__
