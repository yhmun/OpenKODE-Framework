/* --------------------------------------------------------------------------
 *
 *      File            APSAutoreleasePool.h
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

#ifndef __APSAutoreleasePool_h__
#define __APSAutoreleasePool_h__

#include "APSMacroCPP.h"
#include <vector>

NS_APS_BEGIN

class APSObject;

/**
 *	@brief	 APSAutoreleasePool manages autoreleased APSObject objects.
 *	@warning APSAutoreleasePool is not integrated to Operating System or cocos2d framework, so popPool ( ) and pushPool ( ) are should be called manually.
 */

class APSAutoreleasePool : public std::vector<APSObject*>
{
	public :

		/** 
		 *	Adds an APSObject object at the top pool.
		 *	@param object   APSObject object to be added.
		 */
		static KDvoid		addObject ( APSObject* pObject );
	    
		/** Drains the top pool and removes from the pool stack. */
		static KDvoid		popPool ( KDvoid );
	    
		/** Adds a release pool at the top of the pool stack. */
		static KDvoid		pushPool ( KDvoid );
	    
		/** Drains all objects in this pool. */
		KDvoid				drain ( KDvoid );
};

NS_APS_END

#endif	// __APSAutoreleasePool_h__
