/* --------------------------------------------------------------------------
 *
 *      File            KawazTan.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/14 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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
#include "KawazTan.h"

KawazTan* KawazTan::create ( const CCPoint& tPosition )
{
	KawazTan*	pRet = new KawazTan ( );

	if ( pRet && pRet->initWithPosition ( tPosition ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool KawazTan::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
	if ( !KWSprite::initWithTexture ( pTexture, tRect ) )
	{
		return KD_FALSE;
	}

    m_eType  = (KawazTanType) ( kdRand ( ) % 3 ); 
    m_nScore = KAWAZTAN_SCORE;
    m_eState = KawazTanStateWaiting;

	return KD_TRUE;
}

KDbool KawazTan::initWithPosition ( const CCPoint& tPosition )
{
	KawazTanType	eType = (KawazTanType) ( kdRand ( ) % 3 ); 
	
	if ( !KWSprite::initWithFile ( ccszf ( "image/main/kawaz%d-hd.png", eType ) ) )
	{
		return KD_FALSE;
	}

	m_eType = eType;

	this->setPosition ( tPosition );

	return KD_TRUE;
}

KDbool KawazTan::start ( KDfloat fWaitTime )
{
	if ( !this->isMoving ( ) )
	{
		if ( m_eType == KawazTanTypeBomb )
		{
			fWaitTime *= 1.5f;
		}

		m_eState = KawazTanStateMoving;

		const CCPoint&		tPoint = this->getPosition ( );
		const CCSize&		tSize  = this->getContentSize ( );

		this->runAction 
		(
			CCSequence::create 
			(
				CCEaseOut	::create ( CCMoveTo::create ( 0.25f, ccp ( tPoint.x, tPoint.y + tSize.cy * 0.8f ) ), 0.5f ),
				CCCallFunc	::create ( this, callfunc_selector ( KawazTan::onChangeStateNormal ) ),
				CCDelayTime	::create ( fWaitTime ),
				CCCallFunc	::create ( this, callfunc_selector ( KawazTan::onChangeStateMoving ) ),
				CCEaseIn	::create ( CCMoveTo::create ( 0.25f, tPoint ), 0.5f ),
				CCCallFunc	::create ( this, callfunc_selector ( KawazTan::onBacked ) ),
				KD_NULL
			)			
		);

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDbool KawazTan::isMoving ( KDvoid )
{
	return this->numberOfRunningActions ( ) != 0;
}

KDbool KawazTan::tab ( KDvoid )
{
	if ( m_eState == KawazTanStateNormal )
	{
		m_eState = KawazTanStateDamaged;

		CCTexture2D*	pDamageTexture;

		if ( m_eType == KawazTanTypeBomb )
		{		
			pDamageTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "image/main/bomb_effect-hd.png" );
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/bomb.wav" );
		}
		else
		{		
			pDamageTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "image/main/damage-hd.png" );
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );	
		}

		CCSpriteFrame*	pDamaged = CCSpriteFrame::createWithTexture ( pDamageTexture, CCRect ( ccpz, pDamageTexture->getContentSize ( ) ) );
		this->setDisplayFrame ( pDamaged ); 

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDbool KawazTan::canTouch ( KDvoid )
{
	return m_eState == KawazTanStateNormal || m_eState == KawazTanStateInvinsible || m_eState == KawazTanStateDamaged;
}

KawazTanType KawazTan::getType ( KDvoid )
{
	return m_eType;
}

KDint KawazTan::getScore ( KDvoid )
{
	return m_nScore;
}

KDvoid KawazTan::onBacked ( KDvoid )
{
	m_eState = KawazTanStateWaiting; 

	const KDchar*	szFilename;	
	KDint			r = kdRand ( ) % 100;
	if ( r < 100.0f * BOMB_RATE )
	{	
		m_eType = KawazTanTypeBomb;
		szFilename = "image/main/bomb-hd.png";
		m_nScore = BOMB_SCORE;
	}
	else
	{
		m_eType = (KawazTanType) ( kdRand ( ) % 3 ); 
		szFilename = ccszf ( "image/main/kawaz%d-hd.png", m_eType );
		m_nScore = KAWAZTAN_SCORE;
	}

	CCTexture2D*	pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilename ); 
	CCSpriteFrame*	pSprite  = CCSpriteFrame::createWithTexture ( pTexture, CCRect ( ccpz, pTexture->getContentSize ( ) ) );
	this->setDisplayFrame ( pSprite ); 
}

KDvoid KawazTan::onChangeStateNormal ( KDvoid )
{
	m_eState = KawazTanStateNormal;
	if ( m_eType == KawazTanTypeBomb )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/fire.wav" );
	}
}

KDvoid KawazTan::onChangeStateMoving ( KDvoid )
{
	m_eState = KawazTanStateMoving;
}

