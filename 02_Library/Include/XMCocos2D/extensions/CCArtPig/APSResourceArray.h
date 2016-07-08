/* --------------------------------------------------------------------------
 *
 *      File            APSResourceArray.h
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

#ifndef __APSResourceArray_h__
#define __APSResourceArray_h__

#include "APSResource.h"

NS_APS_BEGIN

typedef std::vector<std::pair<APSIndex, APSResource*> >  APSResourceArrayStorage;

/**
 *	@brief	APSResourceArray contains an array of pair<APSIndex, APSResource *> 
 *	types.
 *
 *	When an APSGraphicGroup object is allocated and initialized from an xml file, 
 *	it has only children's APSIndex values instead of direct pointer references.
 *	APSResourceArray lazily aquires and stores APSResource object's pointer
 *	when it is actually needed.
 *
 */
class APSResourceArray : public APSResourceArrayStorage 
{	    
	public :

		/** Constructor of APSResourceArray with a given container resource. */
		APSResourceArray ( APSResource* pContainer = KD_NULL, KDuint uCapacity = 4 );
	    
		/** Returns an APSResource object at index in pair<APSIndex, APSResource *> list. */
		APSResource *getResourceAtIndex ( KDuint uIndex );
	    
		/** Assigns a pair of APSIndex and APSResource at a given index in pair<APSIndex, APSResource *> list. */
		inline KDvoid  setResourceAtIndex ( KDuint uIndex, APSIndex uApsIndex, APSResource* pResource = KD_NULL ) 
		{
			(*this) [ uIndex ] = std::pair<APSIndex, APSResource*> ( uApsIndex, pResource );
		}
	    
		/** Returns APSIndex value at a given index in pair<APSIndex, APSResource *> list. */
		inline APSIndex  getApsIndexAtIndex ( KDuint uIndex ) const
		{
			return (*this) [ uIndex ].first;
		}
	    
		KDuint  getIndexOfApsIndex ( APSIndex uIndex ) const;
	    
		/** Assigns a pair of APSIndex at the end of pair<APSIndex, APSResource *> list. */
		KDvoid  pushBack ( APSIndex uApsIndex );
	    
		KDvoid  pushBack ( APSResource* pResource );
	    
		KDvoid  insertResource ( KDint nIndex, APSIndex uApsIndex );
	    
		KDvoid  insertResource ( KDint nIndex, APSResource* pResource );
	    
		KDvoid  preloadAll ( KDvoid );
	    
	protected :

		/** Returns a resource from symbol. If there is no resource with the index, NULL is returned. */
		APSResource*  getResource ( APSIndex uIndex ) const;

		APS_SYNTHESIZE ( APSResource*, m_pContainer, Container );     // APSResource object that contains this object.

		APS_SYNTHESIZE_READONLY ( KDbool, m_bIsPreloaded, IsPreloaded );
};

NS_APS_END

#endif	// __APSResourceArray_h__
