/* --------------------------------------------------------------------------
 *
 *      File            BackgroundGirls.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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
#include "BackgroundGirls.h"

BackgroundGirls::BackgroundGirls ( KDvoid )
{
	m_pDifferentGirlsActions = KD_NULL;
	m_pSpriteSheetTextures	 = KD_NULL;
}

BackgroundGirls::~BackgroundGirls ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pDifferentGirlsActions );
	CC_SAFE_RELEASE ( m_pSpriteSheetTextures   );
}

KDbool BackgroundGirls::init ( KDvoid )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	// init vars
	m_uCurrentGirlNumber = 0;

	// frame indices and its size (copied from a text file)
	KDuint		aFramesIndices [ ] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 9, 8, 1, 8, 9, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 9, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 11, 10, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 8, 9, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 9, 8, 1, 8, 9, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 11, 10, 11, 10, 11, 10, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 11, 10, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 11, 10, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1 };
	KDuint		uFramesIndicesNumberOfElements = sizeof ( aFramesIndices ) / sizeof ( aFramesIndices [ 0 ] );

	// test frame indices, all must be between 1 and 11
	for ( KDuint i = 0; i < uFramesIndicesNumberOfElements; i++ )
	{
		if ( aFramesIndices [ i ] < 1 || aFramesIndices [ i ] > NUMBER_OF_FRAMES_PER_GIRL )
		{
			CCAssert ( KD_FALSE, "Frame indices must be between 1 and 11." );			
		}
	}

	// load all sprite sheets and create animations
	m_pDifferentGirlsActions = new CCArray ( );
	m_pSpriteSheetTextures   = new CCArray ( );
	
	CCSpriteFrameCache*		pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	for ( KDuint uGirl = 0; uGirl < NUMBER_OF_GIRLS; uGirl++ )
	{
		// if odd (1 texture/coordinate for 2 girls)
		if ( uGirl % 2 == 0 )
		{			
			// texture filename and path			
			const KDchar*	szTexturePathAndFilename = ccszf ( "girls_%02d_%02d.pvr", uGirl + 1, uGirl + 2 );
			
			
			// load texture	
			CCTexture2D*	pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szTexturePathAndFilename );
			m_pSpriteSheetTextures->addObject ( pTexture );
		
			// load sprite sheet coordinates file
			const KDchar*	szPlistFilename = ccszf ( "girls_%02d_%02d.plist", uGirl + 1, uGirl + 2 );
			pSpriteFrameCache->addSpriteFramesWithFile ( szPlistFilename, pTexture );
		}
		
		// add as many textures as frame indices we have
		CCAnimation*	pGirlAnimation = CCAnimation::create ( ); 
		pGirlAnimation->setDelayPerUnit ( 1 / 12.f );
		
		for ( KDuint uFrameIndex = 0; uFrameIndex < uFramesIndicesNumberOfElements; uFrameIndex++ )
		{
			const KDchar*	szOriginalTextureFilename = ccszf ( "girl_%02d_%04d.png", uGirl + 1, aFramesIndices [ uFrameIndex ] );
			CCSpriteFrame*	pSpriteFrame = pSpriteFrameCache->spriteFrameByName ( szOriginalTextureFilename );
			pGirlAnimation->addSpriteFrame ( pSpriteFrame );
		}

		// create an animation/action and add it to action container
		CCAnimate*			pGirlAction = CCAnimate::create ( pGirlAnimation );
		CCRepeatForever*	pRepeatForever = CCRepeatForever::create ( pGirlAction );

		m_pDifferentGirlsActions->addObject ( pRepeatForever );
	}

	// girl sprite		
	m_pGirlSprite = CCSprite::createWithSpriteFrameName ( "girl_01_0001.png" ); 
	this->addChild ( m_pGirlSprite );

	return KD_TRUE;
}

KDvoid BackgroundGirls::onEnter ( KDvoid )
{
	CCNode::onEnter ( );

	m_pGirlSprite->stopAllActions ( );
	m_pGirlSprite->runAction ( (CCAction*) m_pDifferentGirlsActions->objectAtIndex ( m_uCurrentGirlNumber ) );
}

KDvoid BackgroundGirls::onExit ( KDvoid )
{
	m_pGirlSprite->stopAllActions ( );

	CCNode::onExit ( );
}

KDuint BackgroundGirls::getCurrentGirlNumber ( KDvoid )
{
	return m_uCurrentGirlNumber;
}

KDvoid BackgroundGirls::setGirlNumber ( KDuint uGirlNumber )
{
	CCAssert ( uGirlNumber < NUMBER_OF_GIRLS, "girlNumber too high." );

	m_uCurrentGirlNumber = uGirlNumber;

	m_pGirlSprite->stopAllActions ( );
	m_pGirlSprite->runAction ( (CCAction*) m_pDifferentGirlsActions->objectAtIndex ( m_uCurrentGirlNumber ) );
	CCLOG ( "setting girlNumber to: %d", m_uCurrentGirlNumber );
}

// Activate next girl modifiying m_currentGirlNumber. 0...NUMBER_OF_GIRLS-1,0,1,2...
KDvoid BackgroundGirls::incrementGirl ( KDvoid )
{
	KDuint		uNewGirlNumber = ( m_uCurrentGirlNumber + 1 ) % NUMBER_OF_GIRLS;
	this->setGirlNumber ( uNewGirlNumber );
}
