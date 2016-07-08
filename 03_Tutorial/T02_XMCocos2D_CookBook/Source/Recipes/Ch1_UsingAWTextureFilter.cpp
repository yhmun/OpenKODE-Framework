/* --------------------------------------------------------------------------
 *
 *      File            Ch1_UsingAWTextureFilter.cpp
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
#include "Ch1_UsingAWTextureFilter.h"
#include "Libraries/CCTexture2DMutable.h"
#include "Libraries/AWTextureFilter.h"

static const KDchar*	l_aAwEffects [ ] = 
{
	"GaussianBlur"	,
	"SelectiveBlur"	,
	"Shadow"		,
};

KDbool Ch1_UsingAWTextureFilter::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_tEffectPosition = ccp ( 240, 160 );

	this->initSubMenus ( sizeof ( l_aAwEffects ) / sizeof ( l_aAwEffects [ 0 ] ) );

	return KD_TRUE;
}

const KDchar* Ch1_UsingAWTextureFilter::runSubAction ( KDint nIndex )
{
	this->removeChildByTag ( TAG_EFFECT_NODE );

	switch ( nIndex )
	{
		case 0 :	this->showGaussianBlurEffect	( );	break;
		case 1 :	this->showSelectiveBlurEffect	( );	break;
		case 2 :	this->showShadowEffect			( );	break;
	}

	return l_aAwEffects [ nIndex ];
}

KDvoid Ch1_UsingAWTextureFilter::showGaussianBlurEffect ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	// Create the mutable texture
	CCTexture2DMutable*		pMutableTexture = CCTexture2DMutable::createWithImage ( CCImage::create ( "cocos2d_beginner.png", kCCTexture2DPixelFormat_RGBA8888 ) ); 

	// Copy the mutable texture as a non mutable texture
	CCTexture2D*			pNoMutableTexture = CCTexture2D::createWithImage ( CCImage::create ( "cocos2d_beginner.png", kCCTexture2DPixelFormat_RGBA8888 ) ); 

	// Apply blur to the mutable texture
	AWTextureFilter::blur ( pMutableTexture, 3 );

	// Create sprites to show the textures
	CCSprite*	pOriginal = CCSprite::createWithTexture ( pNoMutableTexture );
	pOriginal->setPosition ( ccp ( tLyrSize.cx / 2 - pOriginal->getContentSize ( ).cx / 2 - 1, tLyrSize.cy / 2 ) ); 

	CCSprite*	pBlur = CCSprite::createWithTexture ( pMutableTexture );
	pBlur->setPosition ( ccp ( tLyrSize.cx / 2 + pBlur->getContentSize ( ).cx / 2 + 1, tLyrSize.cy / 2 ) ); 

	CCNode*		pNode = CCNode::create ( );
	pNode->addChild ( pOriginal, 0, 0 );
	pNode->addChild ( pBlur, 0, 1 );
	this->addChild ( pNode, -1, TAG_EFFECT_NODE );	
}
		
KDvoid Ch1_UsingAWTextureFilter::showSelectiveBlurEffect ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	// Create mutable texture
	CCTexture2DMutable*		pMutableTexture = CCTexture2DMutable::createWithImage ( CCImage::create ( "cocos2d_beginner.png", kCCTexture2DPixelFormat_RGBA8888 ) ); 
	
	// Apply blur to the mutable texture
	AWTextureFilter::blur ( pMutableTexture, 8, CCRect ( m_tEffectPosition.x - 200, ( tLyrSize.cy - m_tEffectPosition.y ) - 75, 150, 150 )  );

	// Create sprites to show the textures
	CCSprite*	pBlur = CCSprite::createWithTexture ( pMutableTexture );
	pBlur->setPosition ( ccpMid ( tLyrSize ) ); 
	this->addChild ( pBlur, -1, TAG_EFFECT_NODE );	
}

KDvoid Ch1_UsingAWTextureFilter::showShadowEffect ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	// Create a background so we can see the shadow
	CCLayerColor*	pBackground = CCLayerColor::create ( ccc4 ( 200, 100, 100, 255 ), 300, 50 );
	pBackground->ignoreAnchorPointForPosition ( KD_FALSE );	
	pBackground->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pBackground->setPosition ( ccpMid ( tLyrSize ) );
		
	// Create a sprite for the font label
	CCSprite*		pLabelSprite = CCSprite::create ( );
	pLabelSprite->setPosition ( ccpMid ( tLyrSize ) );
	
	// Create a sprite for the shadow
	CCSprite*		pShadowSprite = CCSprite::create ( );
	pShadowSprite->setPosition ( ccp ( tLyrSize.cx / 2 + 1, tLyrSize.cy / 2 + 1 ) );
	
	// Color it black
	pShadowSprite->setColor ( ccBLACK );
	
	// Add does to scene
	CCNode*			pNode = CCNode::create ( );
	pNode->addChild ( pBackground  , -1 );
	pNode->addChild ( pShadowSprite,  0 );
	pNode->addChild ( pLabelSprite ,  1 );
	this->addChild ( pNode, -1, TAG_EFFECT_NODE );	
		
	// Create a mutable texture with a string (a feature of CCTexture2DMutable)
	CCTexture2DMutable*	pShadowTexture = CCTexture2DMutable::createWithString ( "Shadowed Text", "Arial.ttf", 28 );
	
	// Copy the mutable texture as non mutable texture
	CCTexture2D*		pLabelTexture = pShadowTexture->copyMutable ( KD_FALSE ); 
	
	const CCSize&		tShadowSize = pShadowTexture->getContentSize ( );

	// Set the label texture
	pLabelSprite->setTexture ( pLabelTexture );
	pLabelSprite->setTextureRect ( CCRect ( 0, 0, tShadowSize.cx, tShadowSize.cy ) );
	
	// Apply blur to the shadow texture
	AWTextureFilter::blur ( pShadowTexture, 4 ); 
	
	// Set the shadow texture
	pShadowSprite->setTexture ( pShadowTexture );
	pShadowSprite->setTextureRect ( CCRect ( 0, 0, tShadowSize.cx, tShadowSize.cy ) );
}

KDvoid Ch1_UsingAWTextureFilter::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	this->ccTouchesMoved ( pTouches, pEvent );
}

KDvoid Ch1_UsingAWTextureFilter::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	
	m_tEffectPosition = this->convertTouchToNodeSpace ( pTouch );

	this->reloadSubCallback ( );
}