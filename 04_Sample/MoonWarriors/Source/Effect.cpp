/* --------------------------------------------------------------------------
 *
 *      File            Effect.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors    
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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
#include "Effect.h"

KDvoid  flareEffect ( CCNode* pParent, CCObject* pTarget, SEL_CallFuncN pSelector )
{
	CCSprite*  pFlare = CCSprite::create ( PATH_IMAGE_FLARE );

	pParent->addChild ( pFlare, 10 );
    pFlare->setOpacity ( 0 );
    pFlare->setPosition ( ccp ( -30, 297 ) );
    pFlare->setRotation ( -120 );
    pFlare->setScale ( 0.2f );

	pFlare->runAction ( CCSequence::create
	(
		CCFadeTo::create ( 0.5f, 255 ),
		CCScaleBy::create ( 0.7f, 1.2f, 1.2f ),
		CCFadeTo::create ( 1, 0 ),
		CCCallFunc::create ( pFlare, callfunc_selector ( CCNode::removeFromParentAndCleanup ) ), 
		CCCallFuncN::create ( pTarget, pSelector ),
		KD_NULL
	) );

	pFlare->runAction ( CCEaseSineOut::create ( CCMoveBy::create ( 0.5f, ccp ( 328, 0 ) ) ) );
	pFlare->runAction ( CCEaseExponentialOut::create ( CCRotateBy::create ( 2.5f, 90 ) ) );
	pFlare->runAction ( CCScaleTo::create ( 0.5f, 1 ) );
}

KDvoid  spark ( const CCPoint& tPosition, CCNode* pParent, KDfloat fScale, KDfloat fDuration )
{
	fScale    = KD_MAX ( fScale, 0.3f );
	fDuration = KD_MAX ( fDuration, 0.5f );

	CCSprite*  pSprite1 = CCSprite::create ( PATH_IMAGE_EXPLODE1 );
	CCSprite*  pSprite2 = CCSprite::create ( PATH_IMAGE_EXPLODE2 );
	CCSprite*  pSprite3 = CCSprite::create ( PATH_IMAGE_EXPLODE3 );

	pParent->addChild ( pSprite1 );
	pParent->addChild ( pSprite2 );
	pParent->addChild ( pSprite3 );

	pSprite1->setPosition ( tPosition );
	pSprite2->setPosition ( tPosition );
	pSprite3->setPosition ( tPosition );

	pSprite1->setScale ( fScale );
	pSprite2->setScale ( fScale );
	pSprite3->setScale ( fScale );

	pSprite3->setRotation ( CCRANDOM_0_1 ( ) * 360.f );

	CCActionInterval*  pLeft   = CCRotateBy::create ( fDuration, -45 );
	CCActionInterval*  pRight  = CCRotateBy::create ( fDuration,  45 );
	CCActionInterval*  pScaleBy = CCScaleBy::create ( fDuration, 3 );
	CCActionInterval*  pFadeOut = CCFadeOut::create ( fDuration );

	pSprite1->runAction ( CCSequence::createWithTwoActions
	(
		CCSpawn::create ( pLeft, pScaleBy, pFadeOut, KD_NULL ),
		CCCallFunc::create ( pSprite1, callfunc_selector ( CCNode::removeFromParentAndCleanup ) )
	) );

	pSprite2->runAction ( CCSequence::createWithTwoActions
	(
		CCSpawn::create ( pRight, pScaleBy->copy ( ), pFadeOut->copy ( ), KD_NULL ),
		CCCallFunc::create ( pSprite2, callfunc_selector ( CCNode::removeFromParentAndCleanup ) )
	) );

	pSprite3->runAction ( CCSequence::createWithTwoActions
	(
		CCSpawn::create ( (CCFiniteTimeAction*) pScaleBy->copy ( ), pFadeOut->copy ( ), KD_NULL ),
		CCCallFunc::create ( pSprite3, callfunc_selector ( CCNode::removeFromParentAndCleanup ) )
	) );
}