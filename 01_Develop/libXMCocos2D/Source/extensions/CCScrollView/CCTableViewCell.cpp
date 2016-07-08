/* --------------------------------------------------------------------------
 *
 *      File            CCTableViewCell.cpp
 *      Author          Y.H Mun
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

#include "Precompiled.h"
#include "extensions/CCScrollView/CCTableViewCell.h"

NS_CC_BEGIN

CCTableViewCell* CCTableViewCell::create ( KDvoid )
{
	CCTableViewCell*  pRet = new CCTableViewCell ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}

	return pRet;
}

CCTableViewCell::CCTableViewCell ( KDvoid )
{

}

KDvoid CCTableViewCell::reset ( KDvoid )
{
    m_uIdx = CC_INVALID_INDEX;
}

KDvoid CCTableViewCell::setObjectID ( KDuint uIdx )
{
    m_uIdx = uIdx;
}

KDuint CCTableViewCell::getObjectID ( KDvoid )
{
    return m_uIdx;
}

KDuint CCTableViewCell::getIdx ( KDvoid )
{
    return m_uIdx;
}

KDvoid CCTableViewCell::setIdx ( KDuint uIdx )
{
    m_uIdx = uIdx;
}

NS_CC_END
