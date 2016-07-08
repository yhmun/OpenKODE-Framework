/* --------------------------------------------------------------------------
 *
 *      File            CCSorting.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Sangwoo Im
 *
 *         http://www.cocos2d-x.org      
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

#ifndef __CCSorting_h__
#define __CCSorting_h__

#include "../../cocoa/CCArray.h"

NS_CC_BEGIN

class CCSortableObject
{
	public :

		virtual ~CCSortableObject ( KDvoid ) { }

	public :

		virtual KDvoid		setObjectID ( KDuint uObjectID ) = 0;
		virtual KDuint		getObjectID ( KDvoid ) = 0;
};

class CCArrayForObjectSorting : public CCArray
{
	public :

		CCArrayForObjectSorting ( KDvoid );

	public :

		/**
		 *	Inserts a given object into array.
		 * 
		 *	Inserts a given object into array with key and value that are used in
		 *	sorting. "value" must respond to message, compare:, which returns 
		 *	(NSComparisonResult). If it does not respond to the message, it is appended.
		 *	If the compare message does not result NSComparisonResult, sorting behavior
		 *	is not defined. It ignores duplicate entries and inserts next to it.
		 *
		 *	@param object to insert
		 */
		KDvoid				insertSortedObject ( CCSortableObject* pObject );

		/**
		 *	Removes an object in array.
		 *
		 *	Removes an object with given key and value. If no object is found in array
		 *	with the key and value, no action is taken.
		 *
		 *	@param value to remove
		 */
		KDvoid				removeSortedObject ( CCSortableObject* pObject );

		/**
		 *	Sets a new value of the key for the given object.
		 * 
		 *	In case where sorting value must be changed, this message must be sent to
		 *	keep consistency of being sorted. If it is changed externally, it must be
		 *	sorted completely again.
		 *
		 *	@param value to set
		 *	@param object the object which has the value
		 */
		KDvoid				setObjectID_ofSortedObject ( KDuint uTag, CCSortableObject* pObject );

		CCSortableObject*	objectWithObjectID ( KDuint uTag );

		/**
		 *	Returns an object with given key and value.
		 * 
		 *	Returns an object with given key and value. If no object is found,
		 *	it returns nil.
		 *
		 *	@param value to locate object
		 *	@return object found or nil.
		 */
		CCSortableObject*	getObjectWithObjectID ( KDuint uTag );

		/**
		 *	Returns an index of the object with given key and value.
		 *
		 *	Returns the index of an object with given key and value. 
		 *	If no object is found, it returns an index at which the given object value
		 *	would have been located. If object must be located at the end of array,
		 *	it returns the length of the array, which is out of bound.
		 * 
		 *	@param value to locate object
		 *	@return index of an object found
		 */
		KDuint				indexOfSortedObject ( CCSortableObject* pObject );

};

NS_CC_END

#endif	// __CCSorting_h__

