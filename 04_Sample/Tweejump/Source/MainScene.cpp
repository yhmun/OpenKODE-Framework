/* --------------------------------------------------------------------------
 *
 *      File            MainScene.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org  
 *         https://code.google.com/p/tweejump-cocos2dx
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
#include "MainScene.h"

KDbool MainScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCRect&   tVisibleRect = CCEGLView::sharedOpenGLView ( )->getVisibleRect ( );
	const CCSize&   tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Get sprite sheet
	pFrameCache->addSpriteFramesWithFile ( "Images/spritesheet.plist" );

	// Get background from sprite sheet
	m_pBackground = CCSprite::createWithSpriteFrame ( pFrameCache->spriteFrameByName ( "background.png" ) );
	this->addChild ( m_pBackground );

	// Center the background on the screen
	m_pBackground->setPosition ( ccpMid ( tWinSize ) );

	// Initialze the clouds
	initClouds ( );

	// Start stepping
	schedule ( schedule_selector ( MainScene::step ) );

	// Add a menu item with "X" image, which is clicked to quit the program.
    // Create a "close" menu item with close icon, it's an auto release object.
    CCMenuItemImage*  pClose = CCMenuItemImage::create 
	(
        "Images/CloseNormal.png", "Images/CloseSelected.png", this, menu_selector ( MainScene::onClose ) 
	);
    pClose->setPosition ( ccp ( tVisibleRect.getMaxX ( ) - 20, tVisibleRect.getMaxY ( ) - 20 ) );
	this->addChild ( CCMenu::create ( pClose, KD_NULL ), 1 );

    return KD_TRUE;
}

KDvoid MainScene::onClose ( CCObject* pSender )
{
    CCDirector::sharedDirector ( )->end ( );
}


/////////////////////////////////////////////////////////
// Create kNumClouds
KDvoid MainScene::initClouds ( KDvoid )  
{
	CCLOG ( "initClouds" );

	m_nCurrentCloudTag = kCloudsStartTag;

	// Create the clouds and add them to the layer
	while ( m_nCurrentCloudTag < kCloudsStartTag + kNumClouds ) 
	{
		initCloud ( );

		m_nCurrentCloudTag++;
	}

	// randomly size and place each cloud
	resetClouds ( );
}

/////////////////////////////////////////////////////////
// There are 3 types of clouds, randomly choose them
KDvoid MainScene::initCloud ( KDvoid )
{
	CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	CCSprite*			 pCloud;

	switch ( kdRand ( ) % 3 ) 
	{
		case 0 :	pCloud = CCSprite::createWithSpriteFrame ( pFrameCache->spriteFrameByName ( "cloud1.png" ) );	break;
		case 1 : 	pCloud = CCSprite::createWithSpriteFrame ( pFrameCache->spriteFrameByName ( "cloud2.png" ) );	break;
		case 2 :	pCloud = CCSprite::createWithSpriteFrame ( pFrameCache->spriteFrameByName ( "cloud3.png" ) );	break;
	}	

	this->addChild ( pCloud, 3, m_nCurrentCloudTag );
	pCloud->setOpacity ( 128 );
}

/////////////////////////////////////////////////////////
// Loop through and reset the clouds on initialization
KDvoid MainScene::resetClouds ( KDvoid )
{
	CCLOG ( "resetClouds" );

	m_nCurrentCloudTag = kCloudsStartTag;
	
	while ( m_nCurrentCloudTag < kCloudsStartTag + kNumClouds ) 
	{
		resetCloud ( );

		CCSprite*  pCloud = (CCSprite*) getChildByTag ( m_nCurrentCloudTag );
		CCPoint    tPos   = pCloud->getPosition ( );

		tPos.y -= CCDirector::sharedDirector ( )->getWinSize ( ).cy;
		pCloud->setPosition ( tPos );
		m_nCurrentCloudTag++;
	}
}

/////////////////////////////////////////////////////////
// For each cloud, randomly size and place them
KDvoid MainScene::resetCloud ( KDvoid )
{
	CCSprite*  pCloud = (CCSprite*) getChildByTag ( m_nCurrentCloudTag );
	
	// Calculate a random distance for this cloud
	KDfloat  fDistance = kdRand ( ) % 20 + 5.f;
	
	// Scale the cloud based on the distance
	KDfloat  fScale = 5.0f / fDistance;

	// Scale the X and Y equally for the cloud
	pCloud->setScale ( fScale );

	// randomly invert the X scale for some of the clouds
	if ( kdRand ( ) % 2 == 1 )
	{
		pCloud->setScaleX ( -pCloud->getScaleX ( ) );
	}
	
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	const CCSize&  tSize    = pCloud->getContentSize ( );
	KDfloat        fScaledW = tSize.cx * fScale;

	// Randomly place each cloud within our view 
	KDfloat  x = kdRand ( ) % (KDint) ( tWinSize.cx + (KDint) fScaledW ) - fScaledW / 2;
	KDfloat  y = kdRand ( ) % (KDint) ( tWinSize.cy - (KDint) fScaledW ) + fScaledW / 2 + tWinSize.cy;

	pCloud->setPosition ( ccp ( x, y ) );
}

/////////////////////////////////////////////////////////
// For each cloud, move them in the x direction
// with a speed based on their size
KDvoid MainScene::step ( KDfloat fDelta )
{
	// CCLOG ( "Main::step" );

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	for ( KDint  nTag = kCloudsStartTag; nTag < kCloudsStartTag + kNumClouds; nTag++ ) 
	{
		CCSprite*  pCloud = (CCSprite*) getChildByTag ( nTag );
		CCPoint    tPos   = pCloud->getPosition ( );
		CCSize     tSize  = pCloud->getContentSize ( );

		// Control speed of clouds moving based on their size
		tPos.x += 0.1f * pCloud->getScaleY ( );

		if ( tPos.x > tWinSize.cx + tSize.cx / 2 ) 
		{
			tPos.x = -tSize.cx / 2;
		}

		pCloud->setPosition ( tPos );
	}
}
