/* --------------------------------------------------------------------------
 *
 *      File            CCBSequence.cpp
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
#include "extensions/CCBReader/CCBSequence.h"

NS_CC_BEGIN

CCBSequence::CCBSequence ( KDvoid )
{
	m_fDuration			 = 0.0f;
	m_sName				 = "";
	m_nSequenceId		 = 0;
	m_nChainedSequenceId = 0;
	m_pCallbackChannel	 = KD_NULL;
	m_pSoundChannel		 = KD_NULL;
}

CCBSequence::~CCBSequence ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pCallbackChannel );
	CC_SAFE_RELEASE ( m_pSoundChannel );
}

KDfloat CCBSequence::getDuration ( KDvoid )
{
    return m_fDuration;
}

KDvoid CCBSequence::setDuration ( KDfloat fDuration )
{
    m_fDuration = fDuration;
}

const KDchar* CCBSequence::getName ( KDvoid )
{
    return m_sName.c_str ( );
}

KDvoid CCBSequence::setName ( const KDchar* szName )
{
    m_sName = szName;
}

KDint CCBSequence::getSequenceId ( KDvoid )
{
    return m_nSequenceId;
}

KDvoid CCBSequence::setSequenceId ( KDint nSequenceId )
{
    m_nSequenceId = nSequenceId;
}

CCBSequenceProperty* CCBSequence::getCallbackChannel ( KDvoid )
{
    return m_pCallbackChannel;
}

KDvoid CCBSequence::setCallbackChannel ( CCBSequenceProperty* pCallbackChannel )
{
    CC_SAFE_RELEASE ( m_pCallbackChannel );
    m_pCallbackChannel = pCallbackChannel;
    CC_SAFE_RETAIN  ( m_pCallbackChannel );
}


CCBSequenceProperty* CCBSequence::getSoundChannel ( KDvoid )
{
    return m_pSoundChannel;
}

KDvoid CCBSequence::setSoundChannel ( CCBSequenceProperty* pSoundChannel )
{
    CC_SAFE_RELEASE ( m_pSoundChannel );
    m_pSoundChannel = pSoundChannel;
    CC_SAFE_RETAIN  ( m_pSoundChannel );
}

KDint CCBSequence::getChainedSequenceId ( KDvoid )
{
    return m_nChainedSequenceId;
}

KDvoid CCBSequence::setChainedSequenceId ( KDint nChainedSequenceId )
{
    m_nChainedSequenceId = nChainedSequenceId;
}

NS_CC_END