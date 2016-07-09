/* --------------------------------------------------------------------------
 *
 *      File            SThumbLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
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
#include "SThumbLayer.h"
#include "SLevel.h"

SThumbLayer* SThumbLayer::create ( SLevel* pLevel )
{
	SThumbLayer*	pRet = new SThumbLayer ( );

	if ( pRet && pRet->initWithLevel ( pLevel ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool SThumbLayer::initWithLevel ( SLevel* pLevel )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// 466  *  316
	this->setAnchorPoint ( ccp ( 0, 0 ) );
	this->setContentSize ( ccs ( 233, 158 ) );

	CCSprite*	pThumb = CCSprite::create ( pLevel->getBackgroundThumb ( ) );
	pThumb->setAnchorPoint ( ccp ( 0, 0 ) );
	pThumb->setPosition ( ccp ( 3, 7 ) );
	this->addChild ( pThumb );

	CCSprite*	pBorder = CCSprite::create ( "level_border.png" );
	pBorder->setAnchorPoint ( ccp ( 0, 0 ) );
	pBorder->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( pBorder, 1 );

	CCSprite*	pLevelBkg = CCSprite::create ( "level_bkg.png" );
	pLevelBkg->setAnchorPoint ( ccp ( 0, 0 ) );
	pLevelBkg->setPosition ( ccp ( 15, 127 ) );
	this->addChild ( pLevelBkg, 2 );

	CCLabelTTF*	pLevelText = CCLabelTTF::create ( ccszf ( "%02d", pLevel->getLevel ( ) ), "Marker Felt.ttf", 14 );
	pLevelText->setAnchorPoint ( ccp ( 0, 0 ) );
	pLevelText->setPosition ( ccp ( 22, 136 ) );
	this->addChild ( pLevelText, 3 );

	return KD_TRUE;
}

