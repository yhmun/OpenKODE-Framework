/* --------------------------------------------------------------------------
 *
 *      File            KWSprite.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/30 
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
#include "KWSprite.h"
#include "KWVector.h"

KWSprite* KWSprite::create ( const KDchar* szPath )
{
	KWSprite*	pRet = new KWSprite ( );

	if ( pRet && pRet->initWithFile ( szPath ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KWSprite* KWSprite::createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
	KWSprite*	pRet = new KWSprite ( );

	if ( pRet && pRet->initWithTexture ( pTexture, tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KWSprite* KWSprite::createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
	KWSprite*	pRet = new KWSprite ( );

	if ( pRet && pRet->initWithSpriteFrame ( pSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool KWSprite::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
	if ( !CCSprite::initWithTexture ( pTexture, tRect ) )
	{
		return KD_FALSE;
	}

	const CCSize&	tSize = this->getContentSize ( );

	m_tHitArea = CCRect ( 0, 0, tSize.cx, tSize.cy );

	return KD_TRUE;
}

KDbool KWSprite::collideWithPoint ( const CCPoint& tPoint )
{
	KDfloat		fRotate = this->getRotation ( );
	if ( fRotate == 0 )
	{
		return this->absoluteHitArea ( ).containsPoint ( tPoint ); 
	}
	else
	{
		KWVector*	vx = KWVector::create ( ccp ( this->absoluteHitArea ( ).size.cx, 0 ) );
		KWVector*	vy = KWVector::create ( ccp ( 0, this->absoluteHitArea ( ).size.cy ) );
		KWVector*	p  = KWVector::create ( tPoint )->sub ( KWVector::create ( this->absoluteHitArea ( ).origin ) );

		vx->rotate ( fRotate );
		vy->rotate ( fRotate );

		return kdFabsf ( vx->scalar ( p ) / vx->scalar ( vx ) ) <= 1 && ( vy->scalar ( p ) / vy->scalar ( vy ) ) <= 1;
	}
}

KDbool KWSprite::collideWithSprite ( KWSprite* pSprite )
{
	if ( this->getRotation ( ) == 0 )
	{
		return this->absoluteHitArea ( ).intersectsRect ( pSprite->absoluteHitArea ( ) );
	}
	else
	{
		// have not implemented.
		return KD_TRUE;
	}
}

KDbool KWSprite::collideWithCircle ( const CCPoint& tCenter, KDfloat fRadius )
{
	return KD_FALSE;
}

KDfloat KWSprite::distance ( KWSprite* pSprite )
{
	KWVector*	pThis  = KWVector::create ( this->getAnchorPoint ( ) );
	KWVector*	pOther = KWVector::create ( pSprite->getAnchorPoint ( ) );

	return pThis->sub ( pOther )->length ( );
}

CCRect KWSprite::absoluteHitArea ( KDvoid )
{
	CCPoint		tOrigin = this->convertToWorldSpaceAR ( m_tHitArea.origin );

	return CCRect ( tOrigin.x - m_tHitArea.size.cx / 2, 
					tOrigin.y - m_tHitArea.size.cy / 2, 
					m_tHitArea.size.cx, 
					m_tHitArea.size.cy );
}

CCPoint KWSprite::center ( KDvoid )
{
	const CCPoint&	tPos = this->getPosition ( );
	const CCSize&	tSize = this->getContentSize ( );

	return ccp ( tPos.x + tSize.cx / 2, tPos.y + tSize.cy / 2 );
}

KDdouble KWSprite::x ( KDvoid )
{
	return this->getPositionX ( );
}

KDdouble KWSprite::y ( KDvoid )
{
	return this->getPositionY ( );
}

KDvoid KWSprite::setX ( KDdouble x )
{
	this->setPositionX ( (KDfloat) x );
}

KDvoid KWSprite::setY ( KDdouble y )
{
	this->setPositionY ( (KDfloat) y );
}

