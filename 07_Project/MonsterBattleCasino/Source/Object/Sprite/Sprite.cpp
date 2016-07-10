/* -----------------------------------------------------------------------------------
 *
 *      File            Sprite.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "Sprite.h"

cSprite::cSprite ( Node* pNode, const KDchar* pPath, int z, Point tAnchorPoint, Point tPoint )
: m_pNode			( pNode			)
, m_pSprite			( nullptr		)
, m_bIsMove			( false			)
, m_tMoveBefore		( Point::ZERO	)
, m_tMove			( Point::ZERO	)
, m_dTimeMoveFull	( 0				)
, m_dTimeMoveLeft	( 0				)
{
	m_pSprite = Sprite::create ( pPath );
	m_pSprite->retain ( );
	m_pSprite->setAnchorPoint ( tAnchorPoint );
	m_pSprite->setPosition ( tPoint );
	m_pNode->addChild ( m_pSprite, z );	
}

cSprite::cSprite ( Node* pNode )
: m_pNode			( pNode			)
, m_pSprite			( nullptr		)
, m_bIsMove			( false			)
, m_tMoveBefore		( Point::ZERO	)
, m_tMove			( Point::ZERO	)
, m_dTimeMoveFull	( 0				)
, m_dTimeMoveLeft	( 0				)
{

}

cSprite::~cSprite ( KDvoid )
{
	for ( auto pFrame : m_aFrames )
	{
		CC_SAFE_RELEASE ( pFrame );		
	}
	m_aFrames.clear ( );

	if ( m_pSprite )
	{
		m_pNode->removeChild ( m_pSprite, true );
		m_pSprite->release ( );
		m_pSprite = nullptr;
	}
}

KDvoid cSprite::changeNode ( Node* pNode, KDint z )
{
	if ( m_pSprite )
	{
		m_pNode->removeChild ( m_pSprite, true );
		pNode->addChild ( m_pSprite, z );
	}

	m_pNode = pNode;
}

KDvoid cSprite::initTexture ( const KDchar* pPath, KDint z, Point tPoint )
{
	if ( m_pSprite != nullptr )
	{
		return;
	}

	m_pSprite = Sprite::create ( pPath );
	m_pSprite->retain ( );
	m_pSprite->setAnchorPoint ( Point::ZERO );
	m_pSprite->setPosition ( tPoint );
	m_pNode->addChild ( m_pSprite, z );	
}

KDvoid cSprite::releaseTexture ( KDvoid )
{
	if ( m_pSprite == nullptr )
	{
		return;
	}

	for ( auto pFrame : m_aFrames )
	{
		pFrame->release ( );
	}
	m_aFrames.clear ( );

	if ( m_pSprite )
	{
		m_pNode->removeChild ( m_pSprite, true );
		m_pSprite->release ( );
		m_pSprite = nullptr;
	}
}

KDvoid cSprite::addFrame ( Rect tRect )
{
	if ( m_pSprite == nullptr )
	{
		return;
	}

	SpriteFrame*	pFrame = SpriteFrame::createWithTexture ( m_pSprite->getTexture ( ), tRect ); 
	pFrame->retain ( );
	m_aFrames.push_back ( pFrame );
}

KDvoid cSprite::setFrame ( KDint nFrame )
{
	if ( m_pSprite == nullptr )
	{
		return;
	}

	m_pSprite->setDisplayFrame ( m_aFrames [ nFrame ] );
}

KDvoid cSprite::setPointTime ( Point tPoint, KDdouble dLeftTime )
{
	if ( m_pSprite == nullptr )
	{
		return;
	}

	m_bIsMove		= true;
	m_tMove			= tPoint - m_pSprite->getPosition ( ); 
	m_tMoveBefore	= m_pSprite->getPosition ( ); 
	m_dTimeMoveFull	= dLeftTime;
	m_dTimeMoveLeft	= dLeftTime;
}

KDvoid cSprite::update ( KDdouble dLeftTime )
{
	if ( m_pSprite == nullptr )
	{
		return;
	}

	if ( m_bIsMove == true )
	{
		m_dTimeMoveLeft -= dLeftTime;

		if ( m_dTimeMoveLeft < 0 )
		{
			Point	tPoint = Point ( m_tMoveBefore.x + m_tMove.x, m_tMoveBefore.y + m_tMove.y );
			m_pSprite->setPosition ( tPoint );
			m_bIsMove = false;
		}
		else
		{
			Point	tPoint = Point ( m_tMoveBefore.x + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.x / m_dTimeMoveFull,
									 m_tMoveBefore.y + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.y / m_dTimeMoveFull );
			
			m_pSprite->setPosition ( tPoint );
		}
	}
}
