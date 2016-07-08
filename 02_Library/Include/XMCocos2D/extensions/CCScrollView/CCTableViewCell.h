/* --------------------------------------------------------------------------
 *
 *      File            CCTableViewCell.h
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

#ifndef __CCTableViewCell_h__
#define __CCTableViewCell_h__

#include "../../base_nodes/CCNode.h"
#include "CCSorting.h"

NS_CC_BEGIN

/**
 *	Abstract class for SWTableView cell node
 */
class CCTableViewCell: public CCNode, public CCSortableObject
{
	public : 

		CCTableViewCell ( KDvoid );

		static  CCTableViewCell*	create ( KDvoid );

	public : 

		/** The index used internally by SWTableView and its subclasses */
		KDuint			getIdx ( KDvoid );
		KDvoid			setIdx ( KDuint uIdx );

		/** Cleans up any resources linked to this cell and resets <code>idx</code> property. */
		KDvoid			reset ( KDvoid );

		KDvoid			setObjectID ( KDuint uIdx );
		KDuint			getObjectID ( KDvoid );

	private :

		KDuint			m_uIdx;
};

NS_CC_END

#endif	// __CCTableViewCell_h__
