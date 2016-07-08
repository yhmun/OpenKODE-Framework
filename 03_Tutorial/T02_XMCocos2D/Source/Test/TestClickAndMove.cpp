/* --------------------------------------------------------------------------
 *
 *      File            TestClickAndMove.cpp
 *      Author          Young-Hwan Mun
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
#include "TestClickAndMove.h"

enum
{
	kTagSprite = 1,
};

KDbool TestClickAndMove::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	CCLayer*  layer = CCLayerColor::create ( ccc4 ( ccYELLOW ) );

	layer->runAction 
	(
		CCRepeatForever::create 
		( 
			CCSequence::create ( CCFadeIn ::create ( 1 ), CCFadeOut::create ( 1 ), KD_NULL ) 
		) 
	); 

	this->addChild ( layer );

	CCSprite*  sprite = CCSprite::create ( "Images/grossini.png" );

	sprite->setPosition ( ccp ( 20, 150 ) );
	sprite->runAction ( CCJumpTo::create ( 4, ccp ( 300, 48 ), 100, 4 ) );

	this->addChild ( sprite, 0, kTagSprite );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

const KDchar* TestClickAndMove::subtitle ( KDvoid )
{
	return "( tap screen )";
}

KDvoid TestClickAndMove::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint   tLocation  = this->convertTouchToNodeSpace ( pTouch );

	CCNode*   pNode = getChildByTag ( kTagSprite );

	pNode->stopAllActions ( );
	pNode->runAction ( CCMoveTo::create ( 0.5f, tLocation ) );

	KDfloat  o = tLocation.x - pNode->getPosition ( ).x;
	KDfloat  a = tLocation.y - pNode->getPosition ( ).y;
	KDfloat at = CC_RADIANS_TO_DEGREES ( kdAtanf ( o / a == 0 ? 1 : a ) );
	
	if ( a < 0 ) 
	{
		at = 180 + kdFabsf ( at ) * ( o < 0 ? 1 : -1 );
	}
	
	pNode->runAction ( CCRotateTo::create ( 1, at ) );
}