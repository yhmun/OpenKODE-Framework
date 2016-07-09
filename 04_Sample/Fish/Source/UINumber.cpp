/* --------------------------------------------------------------------------
 *
 *      File            UINumber.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      周海锋 on 12-04-08
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      CJLU. All rights reserved.
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
#include "UINumber.h"

UINumber* UINumber::create ( NumStyle eStyle )
{
	UINumber*	pRet = new UINumber ( );

	if ( pRet && pRet->initWithStyle ( eStyle ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool UINumber::initWithStyle ( NumStyle eStyle )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_pTexture	= KD_NULL;
	m_eStyle	= NumStyleNormal;
	m_nNum		= 0;
	m_nPosCur	= 0;
	m_nPosEnd	= 0;   

	this->setup ( );

	return KD_TRUE;
}

KDvoid UINumber::setNumber ( KDint nNumer )
{
	m_nPosCur = NUM_HEIGHT * m_nNum;
	m_nPosEnd = NUM_HEIGHT * nNumer;

	if ( NumStyleNormal == m_eStyle )
	{
		m_nMoveLen = 4;
	}
	else if ( NumStyleSameTime == m_eStyle ) 
	{
		m_nMoveLen = ( m_nPosEnd - m_nPosCur ) / 20;
	}

	if ( m_nNum > nNumer )
	{
		this->schedule ( schedule_selector ( UINumber::onRollUP ), 0.03f );
	}
	else
	{
		this->schedule ( schedule_selector ( UINumber::onRollDown ), 0.03f );
	}

	m_nNum = nNumer;
}

KDvoid UINumber::onRollDown ( KDfloat fDelta )
{
    m_nPosCur += m_nMoveLen;
    if ( m_nPosCur >= m_nPosEnd ) 
	{
        m_nPosCur = m_nPosEnd;
		this->unschedule ( schedule_selector ( UINumber::onRollDown ) );
    }

	CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 0, m_nPosCur, NUM_WIDTH, NUM_HEIGHT ) );
	this->setDisplayFrame ( pFrame );
}

KDvoid UINumber::onRollUP ( KDfloat fDelta )
{
    m_nPosCur -= 4;
    if ( m_nPosCur <= m_nPosEnd )
	{
        m_nPosCur = m_nPosEnd;
		this->unschedule ( schedule_selector ( UINumber::onRollUP ) );
    }

	CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 0, m_nPosCur, NUM_WIDTH, NUM_HEIGHT ) );
	this->setDisplayFrame ( pFrame );
}

KDvoid UINumber::setup ( KDvoid )
{
	m_pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "number.png" );
       
	CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( m_pTexture, CCRect ( 0, 0, NUM_WIDTH, NUM_HEIGHT ) );
    this->setDisplayFrame ( pFrame );
}
