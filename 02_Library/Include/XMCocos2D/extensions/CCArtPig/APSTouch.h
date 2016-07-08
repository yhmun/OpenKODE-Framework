/* --------------------------------------------------------------------------
 *
 *      File            APSTouch.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Created by Kim Kiyoung on 5/31/12.
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

#ifndef __APSTouch_h__
#define __APSTouch_h__

#include "APSSharedType.h"

NS_APS_BEGIN

class APSTouch 
{	    
	public :

		APSTouch ( APSEventType uType = kAPSEventTypeTouchBegan, CCPoint tPosition = CCPointZero, KDint nTapCount = 1 );	    	

	public :

		APS_SYNTHESIZE ( APSEventType, m_uType    , Type     );
		APS_SYNTHESIZE ( CCPoint     , m_tPosition, Position );
		APS_SYNTHESIZE ( KDint       , m_nTapCount, TapCount );
};

NS_APS_END

#endif	// __APSTouch_h__
