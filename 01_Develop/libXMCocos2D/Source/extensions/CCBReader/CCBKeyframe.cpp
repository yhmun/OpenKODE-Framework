/* --------------------------------------------------------------------------
 *
 *      File            CCBKeyframe.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "extensions/CCBReader/CCBKeyframe.h"

NS_CC_BEGIN

CCBKeyframe::CCBKeyframe ( KDvoid )
{
	m_pValue		= KD_NULL;
	m_fTime			= 0.0f;
	m_nEasingType	= 0;
	m_fEasingOpt	= 0.0f;
}

CCBKeyframe::~CCBKeyframe ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pValue );
}

CCObject* CCBKeyframe::getValue ( KDvoid )
{
    return m_pValue;
}

KDvoid CCBKeyframe::setValue ( CCObject* pValue )
{
    CC_SAFE_RELEASE ( m_pValue );
    m_pValue = pValue;
    CC_SAFE_RETAIN  ( m_pValue );
}

KDfloat CCBKeyframe::getTime ( KDvoid )
{
    return m_fTime;
}

KDvoid CCBKeyframe::setTime ( KDfloat fTime )
{
    m_fTime = fTime;
}

KDint CCBKeyframe::getEasingType ( KDvoid )
{
    return m_nEasingType;
}

KDvoid CCBKeyframe::setEasingType ( KDint nEasingType )
{
    m_nEasingType = nEasingType;
}

KDfloat CCBKeyframe::getEasingOpt ( KDvoid )
{
    return m_fEasingOpt;
}

KDvoid CCBKeyframe::setEasingOpt ( KDfloat fEasingOpt )
{
    m_fEasingOpt = fEasingOpt;
}

NS_CC_END
