/* --------------------------------------------------------------------------
 *
 *      File            CCBSequenceProperty.cpp
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
 *      version 2 of the License, or (at your otPositionion) any later version.
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
#include "extensions/CCBReader/CCBSequenceProperty.h"

NS_CC_BEGIN

CCBSequenceProperty::CCBSequenceProperty ( KDvoid )
{
	m_sName = "";
	m_nType = 0;

    init ( );
}

KDbool CCBSequenceProperty::init ( KDvoid )
{
    m_pKeyframes = new CCArray ( );
    
    return KD_TRUE;
}

CCBSequenceProperty::~CCBSequenceProperty ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pKeyframes );
}

const KDchar* CCBSequenceProperty::getName ( KDvoid )
{
    return m_sName.c_str ( );
}

KDvoid CCBSequenceProperty::setName ( const KDchar* szName )
{
    m_sName = szName;
}

KDint CCBSequenceProperty::getType ( KDvoid )
{
    return m_nType;
}

KDvoid CCBSequenceProperty::setType ( KDint nType )
{
    m_nType = nType;
}

CCArray* CCBSequenceProperty::getKeyframes ( KDvoid )
{
    return m_pKeyframes;
}

NS_CC_END
