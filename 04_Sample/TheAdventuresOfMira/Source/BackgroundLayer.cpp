/* --------------------------------------------------------------------------
 *
 *      File            BackgroundLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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
#include "BackgroundLayer.h"

KDbool BackgroundLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }
    
    m_pParallax = CCParallaxScrollNode::create ( );
    
    CCSprite*	pBackgroundImage1 = CCSprite::createWithSpriteFrameName ( "background.png" );
    CCSprite*	pBackgroundImage2 = CCSprite::createWithSpriteFrameName ( "background.png" );
    m_pParallax->addInfiniteScrollXWithZ ( 0, ccp ( 0.05f, 0 ), ccp ( 0, SCREEN_HEIGHT - pBackgroundImage1->getContentSize().height), pBackgroundImage1, pBackgroundImage2, KD_NULL );
    
    CCSprite*	pHillsImage1 = CCSprite::createWithSpriteFrameName ( "hills_bg.png" );
    CCSprite*	pHillsImage2 = CCSprite::createWithSpriteFrameName ( "hills_bg.png" );
    m_pParallax->addInfiniteScrollXWithZ ( 1, ccp ( 0.15f, 0 ), ccp ( 0, SCREEN_HEIGHT * 0.08f ), pHillsImage1, pHillsImage2, KD_NULL );
    
    CCSprite*	pTreesImage1 = CCSprite::createWithSpriteFrameName ( "trees_bg.png" );
    CCSprite*	pTreesImage2 = CCSprite::createWithSpriteFrameName ( "trees_bg.png" );
    m_pParallax->addInfiniteScrollXWithZ ( 2, ccp ( 0.25f, 0 ), ccp ( 0, SCREEN_HEIGHT * 0.045f ), pTreesImage1, pTreesImage2, KD_NULL );
    
    CCSprite*	pGrassImage1 = CCSprite::createWithSpriteFrameName ( "grass_bg.png" );
    CCSprite*	pGrassImage2 = CCSprite::createWithSpriteFrameName ( "grass_bg.png" );
    m_pParallax->addInfiniteScrollXWithZ ( 4, ccp ( 1, 0 ), ccp ( 0, 0 ), pGrassImage1, pGrassImage2, KD_NULL );
    
    CCSprite*	pWindImage1 = CCSprite::createWithSpriteFrameName ( "wind.png" );
    pWindImage1->setTag ( kWindTag1 );
    pWindImage1->setOpacity ( 90 );

	CCSprite*	pWindImage2 = CCSprite::createWithSpriteFrameName ( "wind.png" );
    pWindImage2->setTag ( kWindTag2 );
    pWindImage2->setOpacity ( 90 );
    m_pParallax->addInfiniteScrollXWithZ ( 3, ccp ( 4, 0 ), ccp ( 0, 0 ), pWindImage1, pWindImage2, KD_NULL );
    
    this->addChild ( m_pParallax );
    this->scheduleUpdate ( );
    
    this->hideWind ( );
    
    return KD_TRUE;
}

KDvoid BackgroundLayer::hideWind ( KDvoid )
{
    m_pParallax->getChildByTag ( kWindTag1 )->setVisible ( KD_FALSE );
    m_pParallax->getChildByTag ( kWindTag2 )->setVisible ( KD_FALSE );
}

KDvoid BackgroundLayer::showWind ( KDvoid )
{
    m_pParallax->getChildByTag ( kWindTag1 )->setVisible ( KD_TRUE );
    m_pParallax->getChildByTag ( kWindTag2 )->setVisible ( KD_TRUE );
}

KDvoid BackgroundLayer::update ( KDfloat fDelta )
{
    m_pParallax->updateWithVelocity ( ccp ( GameManager::sharedGameManager ( )->getVelocity ( ), 0 ), fDelta );
}
