/* --------------------------------------------------------------------------
 *
 *      File            CCInvocation.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
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
#include "extensions/CCControl/CCInvocation.h"

NS_CC_BEGIN

CCInvocation* CCInvocation::create ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent )
{
    CCInvocation*  pRet = new CCInvocation ( pTarget, pAction, uControlEvent );
    
	if ( pRet != KD_NULL )
    {
        pRet->autorelease ( );
    }

    return pRet;
}

CCInvocation::CCInvocation ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent )
{
	m_pTarget = pTarget;
	m_pAction = pAction;
	m_uControlEvent = uControlEvent;
}

KDvoid CCInvocation::invoke ( CCObject* pSender )
{
	if ( m_pTarget && m_pAction )
	{
		( m_pTarget->*m_pAction ) ( pSender, m_uControlEvent );
	}				
}

NS_CC_END