/* --------------------------------------------------------------------------
 *
 *      File            Truck.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-10-26
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "Truck.h"

KDbool Truck::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*		pCaster1 = CCSprite::create ( "Image 906 at frame 10.png" );
	pCaster1->setPosition ( ccp ( 50, -38 ) );

	CCSprite*		pCaster2 = CCSprite::create ( "Image 906 at frame 10.png" );
	pCaster2->setPosition ( ccp ( -63, -38 ) );

	CCSpriteFrame*	pFrame1 = CCSpriteFrame::create ( "Image 911 at frame 10.png", CCRect ( 0, 0, 27, 35 ) );
	CCSpriteFrame*	pFrame2 = CCSpriteFrame::create ( "Image 912 at frame 10.png", CCRect ( 0, 0, 27, 35 ) );
	CCSpriteFrame*	pFrame3 = CCSpriteFrame::create ( "Image 913 at frame 10.png", CCRect ( 0, 0, 27, 35 ) );

	CCArray*		pFrames = CCArray::create ( pFrame1, pFrame2, pFrame3, KD_NULL );
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pFrames, 0.2f );
	pAnimation->setRestoreOriginalFrame ( KD_FALSE );

	CCSprite*	pCurtain1 = CCSprite::createWithSpriteFrame ( pFrame1 );
	CCSprite*	pCurtain2 = CCSprite::createWithSpriteFrame ( pFrame1 );
	CCSprite*	pCurtain3 = CCSprite::createWithSpriteFrame ( pFrame1 );

	pCurtain1->setPosition ( ccp ( -10, -5 ) );
	pCurtain2->setPosition ( ccp ( -40, -5 ) );
	pCurtain3->setPosition ( ccp ( -70, -5 ) );

	pCurtain1->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );
	pCurtain2->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );
	pCurtain3->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );

	CCSprite*	pHead = CCSprite::create ( "Image 907 at frame 10.png" );
	CCSprite*	pBody = CCSprite::create ( "Image 915 at frame 10.png" );
	pBody->setPosition ( ccp ( -8, 5 ) );
	CCSprite*	pDoor = CCSprite::create ( "Image 908 at frame 10.png" );
	pDoor->setPosition ( ccp ( 25, -5 ) );
	CCSprite*	pLight = CCSprite::create ( "Image 909 at frame 10.png" );
	pLight->setPosition ( ccp ( 75, -30 ) );

	this->addChild ( pHead );
	this->addChild ( pCaster1 );
	this->addChild ( pCaster2 );
	this->addChild ( pCurtain1 );
	this->addChild ( pCurtain2 );
	this->addChild ( pCurtain3 );
	this->addChild ( pBody );
	this->addChild ( pDoor );
	this->addChild ( pLight );

	return KD_TRUE;
}
