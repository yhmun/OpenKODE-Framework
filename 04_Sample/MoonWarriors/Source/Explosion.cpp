/* --------------------------------------------------------------------------
 *
 *      File            Explosion.cpp
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
#include "Explosion.h"

KDbool Explosion::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	CCSpriteFrame*  pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "explosion_01.png" );

	this->initWithSpriteFrame ( pFrame );

	CCAnimation*  pAnimation = CCAnimationCache::sharedAnimationCache ( )->animationByName ( "Explosion" );

	this->runAction ( CCSequence::createWithTwoActions 
	(
		CCAnimate ::create ( pAnimation ),
		CCCallFunc::create ( this, callfunc_selector ( CCNode::removeFromParentAndCleanup ) )
	) );

	return KD_TRUE;
}

KDvoid Explosion::sharedExplosion ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( PATH_PLIST_EXPLOSION );
    CCArray*  pAnimFrames = CCArray::createWithCapacity ( 35 );
    KDchar    szStr [256] = "";
    
	for ( KDuint i = 1; i < 35; i++ )
	{
		kdSprintf ( szStr, "explosion_%02d.png", i );
        
		CCSpriteFrame*  pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szStr );
        pAnimFrames->addObject ( pFrame );
    }

	CCAnimation*  pAnimation = CCAnimation::createWithSpriteFrames ( pAnimFrames, 0.04f );
	CCAnimationCache::sharedAnimationCache ( )->addAnimation ( pAnimation, "Explosion" );
}