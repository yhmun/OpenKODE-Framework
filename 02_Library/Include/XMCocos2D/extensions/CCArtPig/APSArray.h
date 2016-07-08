/* --------------------------------------------------------------------------
 *
 *      File            APSArray.h
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

#ifndef __APSArray_h__
#define __APSArray_h__

#include "APSObject.h"

NS_APS_BEGIN

typedef std::vector<APSObject*> APSArrayStorage;


/**
 *	@brief APSArray manages an array of APSObject objects. 
 */
class APSArray : public APSObject
{
	public :

		APSArray ( KDvoid );
		APSArray ( APSArray* pArray );

		virtual ~APSArray ( KDvoid );
	    
		APS_CLASS_NAME ( APSArray );

		/** Allocates, autorelease, and returns an APSArray object. */
		static  APSArray*  array ( KDvoid );

	public :
	    
		APSObject*  getObjectAtIndex ( KDuint uIndex ) const;
	    
		/**
		 *	Adds a given APSObject object at the end of array.
		 *	@param object   APSObject object to be added.
		 */
		KDvoid  addObject ( APSObject* pObject );
	    
		/** 
		 *	Adds all objects in a given APSArray object.
		 *	@param array   APSArray object to be added.
		 */
		KDvoid  addObjectsFromArray ( APSArray* pArray );
	    
		KDvoid  insertObjectAtIndex ( APSObject* pObject, KDuint uIndex );

		KDvoid  removeObjectAtIndex ( KDuint uIndex );

		KDvoid  clear ( KDvoid );
	    
		inline  KDint   count ( KDvoid ) const { return this->m_aArray.size ( ); }
	    
		inline  APSArrayStorage::iterator begin ( KDvoid ) { return m_aArray.begin ( ); }
		inline  APSArrayStorage::iterator end   ( KDvoid ) { return m_aArray.end   ( ); }

	protected :

		APSArrayStorage		m_aArray;
};

NS_APS_END

#endif	// __APSArray_h__
