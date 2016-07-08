/* --------------------------------------------------------------------------
 *
 *      File            SimpleAnimObject.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "SimpleAnimObject.h"

SimpleAnimObject* SimpleAnimObject::create ( KDvoid )
{
	SimpleAnimObject*	pRet = new SimpleAnimObject ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SimpleAnimObject* SimpleAnimObject::create ( const KDchar* szFilePath )
{
	SimpleAnimObject*	pRet = new SimpleAnimObject ( );

	if ( pRet && pRet->initWithFile ( szFilePath ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SimpleAnimObject* SimpleAnimObject::createWithBatchNode ( CCSpriteBatchNode* pBatchNode, const CCRect& tRect )
{
	SimpleAnimObject*	pRet = new SimpleAnimObject ( );

	if ( pRet && pRet->initWithTexture ( pBatchNode->getTexture ( ), tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SimpleAnimObject* SimpleAnimObject::createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
	SimpleAnimObject*	pRet = new SimpleAnimObject ( );

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

SimpleAnimObject* SimpleAnimObject::createWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
	SimpleAnimObject*	pRet = new SimpleAnimObject ( );

	if ( pRet && pRet->initWithSpriteFrameName ( szSpriteFrameName ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid SimpleAnimObject::update ( KDfloat fDelta )
{
	this->setPosition ( ccpAdd ( this->getPosition ( ), m_tVelocity ) ); 
}

CCRect SimpleAnimObject::rect ( KDvoid )
{
	KDfloat		fScaleMod = 0.5f;

	KDfloat		fScale = this->getScale ( );
	CCSize		tSize  = this->getContentSize ( );
	CCPoint		tPoint = this->getPosition ( ); 

	tSize.cx = tSize.cx * fScale * fScaleMod;
	tSize.cy = tSize.cy * fScale * fScaleMod;

	tPoint = ccp ( tPoint.x - ( tSize.cx / 2 ), tPoint.y - ( tSize.cy / 2 ) );

	return CCRect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 
}

KDint SimpleAnimObject::getAnimationType ( KDvoid )
{
	return m_nAnimationType;
}

KDvoid SimpleAnimObject::setAnimationType ( KDint nAnimationType )
{
	m_nAnimationType = nAnimationType;
}

const CCPoint& SimpleAnimObject::getVelocity ( KDvoid )
{
	return m_tVelocity;
}

KDvoid SimpleAnimObject::setVelocity ( const CCPoint& tVelocity )
{
	m_tVelocity = tVelocity;
}