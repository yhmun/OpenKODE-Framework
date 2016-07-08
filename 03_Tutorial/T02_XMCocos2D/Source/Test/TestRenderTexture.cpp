/* --------------------------------------------------------------------------
 *
 *      File            TestRenderTexture.cpp
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestRenderTexture.h"

TestRenderTexture* TestRenderTexture::create ( KDvoid )
{
	TestRenderTexture*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case 0 : pLayer = new RenderTextureSave				( ); break;
		case 1 : pLayer = new RenderTextureZbuffer			( ); break;
		case 2 : pLayer = new RenderTextureTestDepthStencil	( ); break;
		case 3 : pLayer = new RenderTextureTargetNode		( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestRenderTexture::count ( KDvoid )
{
	return 4;
}

//------------------------------------------------------------------
//
// RenderTextureSave
//
//------------------------------------------------------------------
KDvoid RenderTextureSave::onEnter ( KDvoid )
{
	TestRenderTexture::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	// create a render texture, this is what we're going to draw into
	m_pTarget = CCRenderTexture::create ( (KDint) tLyrSize.cx, (KDint) tLyrSize.cy );	
	m_pTarget->retain ( );
	m_pTarget->setPosition ( ccpMid ( tLyrSize ) );
		
	// note that the render texture is a cocosnode, and contains a sprite of it's texture for convience,
	// so we can just parent it to the scene like any other cocos node
	this->addChild ( m_pTarget, -1 );

	// create a brush image to draw into the texture with
	m_pBrush = CCSprite::create ( "Images/fire.png" );
	m_pBrush->retain ( );
	m_pBrush->setColor ( ccRED );
	m_pBrush->setOpacity ( 20 );
	
	this->setTouchEnabled ( KD_TRUE );

	// Save Image menu
	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 24 );

    CCMenuItem*  pItem1 = CCMenuItemFont::create ( "Save Image", this, menu_selector ( RenderTextureSave::saveImage  ) );
    CCMenuItem*  pItem2 = CCMenuItemFont::create ( "Clear"	   , this, menu_selector ( RenderTextureSave::clearImage ) );
    CCMenu*       pMenu = CCMenu::create ( pItem1, pItem2, KD_NULL );
    this->addChild ( pMenu );
    pMenu->alignItemsVertically ( );
    pMenu->setPosition ( ccp ( tLyrSize.cx - 80, tLyrSize.cy / 2 ) );
}

KDvoid RenderTextureSave::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pBrush  );
	CC_SAFE_RELEASE ( m_pTarget );

    CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );

	TestRenderTexture::onExit ( );
}

const KDchar* RenderTextureSave::title ( KDvoid )
{
    return "Render Texture Test";
}

const KDchar* RenderTextureSave::subtitle ( KDvoid )
{
    return "Touch the screen\nPress 'Save Image'\n to create an snapshot of the render texture";
}

KDvoid RenderTextureSave::clearImage ( CCObject* pSender )
{
    m_pTarget->clear ( CCRANDOM_0_1 ( ), CCRANDOM_0_1 ( ), CCRANDOM_0_1 ( ), 0.5f );
}

KDvoid RenderTextureSave::saveImage ( CCObject* pSender )
{
    static KDint   nCounter = 0;

	const KDchar*  szPathPNG = ccszf ( "image-%d.png", nCounter );
	const KDchar*  szPathJPG = ccszf ( "image-%d.jpg", nCounter );

    m_pTarget->saveToFile ( szPathPNG, kCCImageFormatPNG  );
    m_pTarget->saveToFile ( szPathJPG, kCCImageFormatJPEG );

	this->removeChildByTag ( 100 );

    CCSprite*  pSprite = CCSprite::create ( CCFileUtils::sharedFileUtils ( )->fullPathFromWritablePath ( szPathPNG ) );

    pSprite->setScale ( 0.3f );
	pSprite->setPosition ( ccp ( 80, 80 ) );
	pSprite->setAnchorPoint ( ccp ( 0, 0 ) );

    this->addChild ( pSprite, 0, 100 );

    CCLOG ( "Image saved %s and %s", szPathPNG, szPathJPG );

    nCounter++;
}

KDvoid RenderTextureSave::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
    CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tStart = this->convertToNodeSpace ( pTouch->getLocation ( ) );
	CCPoint		tEnd   = this->convertToNodeSpace ( pTouch->getPreviousLocation ( ) );

	// begin drawing to the render texture
	m_pTarget->begin ( );

	// for extra points, we'll draw this smoothly from the last position and vary the sprite's
	// scale/rotation/offset
	KDfloat  fDistance = ccpDistance ( tStart, tEnd );
	if ( fDistance > 1 )
	{
		KDint  d = (KDint) fDistance;
		for ( KDint i = 0; i < d; i++ )
		{
			CCPoint  tDiff  = ccpSub ( tEnd, tStart );
			KDfloat  fDelta = (KDfloat) i / fDistance;

			m_pBrush->setPosition ( ccp ( tStart.x + ( tDiff.x * fDelta ), tStart.y + ( tDiff.y * fDelta ) ) );
			m_pBrush->setRotation ( (KDfloat) ( kdRand ( ) % 360 ) );

			KDfloat  fScale = ( (KDfloat) ( kdRand ( ) % 50 ) / 50.f ) + 0.25f;

			m_pBrush->setScale ( fScale );
			m_pBrush->setColor ( ccc3 ( kdRand ( ) % 127 + 128, 255, 255 ) );

			// Call visit to draw the brush, don't call draw..
			m_pBrush->visit ( );
		}
	}

	// finish drawing and return context back to the screen
	m_pTarget->end ( );
}

//------------------------------------------------------------------
//
// RenderTextureZbuffer
//
//------------------------------------------------------------------
KDvoid RenderTextureZbuffer::onEnter ( KDvoid )
{
	TestRenderTexture::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCLabelTTF*  label1 = CCLabelTTF::create ( "vertexZ = 50", "fonts/Marker Felt.ttf", 32 );
	label1->setPosition ( ccp ( s.cx / 2, s.cy * 0.25f ) );
	label1->setVertexZ ( 50 );
	this->addChild ( label1 );

	CCLabelTTF*  label2 = CCLabelTTF::create ( "vertexZ = 0", "fonts/Marker Felt.ttf", 32 );
	label2->setPosition ( ccp ( s.cx / 2, s.cy * 0.5f ) );
	label2->setVertexZ ( 0 );
	this->addChild ( label2 );

	CCLabelTTF*  label3 = CCLabelTTF::create ( "vertexZ = -50", "fonts/Marker Felt.ttf", 32 );
	label3->setPosition ( ccp ( s.cx / 2, s.cy * 0.75f ) );
	label3->setVertexZ ( -50 );
	this->addChild ( label3 );

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Images/bugs/circle.plist" );
	m_pMgr = CCSpriteBatchNode::create ( "Images/bugs/circle.png", 9 );
	this->addChild ( m_pMgr );

	for ( KDint i = 0; i < 9; i++ )
	{
		m_aSp [ i ] = CCSprite::createWithSpriteFrameName ( "circle.png" );
		m_pMgr->addChild ( m_aSp [ i ], 9 - i );
		m_aSp [ i ]->setVertexZ ( 400.f - i * 100.f );
	}

	m_aSp [ 8 ]->setScale ( 2 );
	m_aSp [ 8 ]->setColor ( ccYELLOW );

	this->setTouchEnabled ( KD_TRUE );
}

const KDchar* RenderTextureZbuffer::title ( KDvoid )
{
	return "Testing Z Buffer in Render Texture";
}

const KDchar* RenderTextureZbuffer::subtitle ( KDvoid )
{
	return "Touch screen. It should be green";
}

KDvoid RenderTextureZbuffer::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	this->ccTouchesMoved ( pTouches, pEvent );
}

KDvoid RenderTextureZbuffer::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); ++iter )
	{
		CCTouch*  pTouch = (CCTouch*) ( *iter );
		CCPoint   tPoint = this->convertTouchToNodeSpace ( pTouch );

		for ( KDint i = 0; i < 9; i++ )
		{
			m_aSp [ i ]->setPosition ( tPoint );
		}
	}
}

KDvoid RenderTextureZbuffer::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	this->renderScreenShot ( );
}

KDvoid RenderTextureZbuffer::renderScreenShot ( KDvoid )
{
	CCRenderTexture*  pTexture = CCRenderTexture::create ( 512, 512 );
	pTexture->setAnchorPoint ( ccp ( 0, 0 ) );

	pTexture->begin ( );

	this->visit ( );

	pTexture->end ( );

	CCSprite*  pSprite = CCSprite::createWithTexture ( pTexture->getSprite ( )->getTexture ( ) );

	pSprite->setPosition ( ccp ( 256, 256 ) );
	pSprite->setOpacity ( 182 );
	pSprite->setFlipY ( 1 );
	pSprite->setColor ( ccGREEN );
	pSprite->runAction ( CCSequence::create ( CCFadeTo::create ( 2, 0 ), CCHide::create ( ), KD_NULL ) );

	this->addChild ( pSprite, 999999 );			
}

//------------------------------------------------------------------
//
// RenderTextureTestDepthStencil
//
//------------------------------------------------------------------

KDvoid RenderTextureTestDepthStencil::onEnter ( KDvoid )
{
	TestRenderTexture::onEnter ( );

    const CCSize&  s = this->getContentSize ( );

    CCSprite*  pSprite = CCSprite::create ( "Images/fire.png" );
    pSprite->setPosition( ccp ( s.cx * 0.25f, 0 ) );
    pSprite->setScale ( 10 );
    CCRenderTexture*  pRend = CCRenderTexture::create ( (KDuint) s.cx, (KDuint) s.cy, kCCTexture2DPixelFormat_RGBA4444, CC_GL_DEPTH24_STENCIL8 );

    glStencilMask ( 0xFF );
    pRend->beginWithClear ( 0, 0, 0, 0, 0, 0 );

    //! mark sprite quad into stencil buffer
    glEnable	  ( GL_STENCIL_TEST );
    glStencilFunc ( GL_ALWAYS, 1, 0xFF );
    glStencilOp	  ( GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask	  ( 0, 0, 0, 1 );
    pSprite->visit ( );

    //! move sprite half width and height, and draw only where not marked
    pSprite->setPosition 
	(
		ccpAdd
		(
			pSprite->getPosition ( ), 
			ccpMult 
			(
				ccp 
				(
					pSprite->getContentSize ( ).cx * pSprite->getScale ( ), 
					pSprite->getContentSize ( ).cy * pSprite->getScale ( ) 
				),
				0.5f
			)
		)
	);
    glStencilFunc ( GL_NOTEQUAL, 1, 0xFF );
    glColorMask ( 1, 1, 1, 1 );
    pSprite->visit ( );

    pRend->end ( );

    glDisable ( GL_STENCIL_TEST );

    pRend->setPosition ( ccpMid ( s ) );

    this->addChild ( pRend );	
}

const KDchar* RenderTextureTestDepthStencil::title ( KDvoid )
{
    return "Testing depthStencil attachment";
}

const KDchar* RenderTextureTestDepthStencil::subtitle ( KDvoid )
{
    return "Circle should be missing 1/4 of its region";
}

//------------------------------------------------------------------
//
// RenderTextureTargetNode
//
//------------------------------------------------------------------

KDvoid RenderTextureTargetNode::onEnter ( KDvoid )
{
	TestRenderTexture::onEnter ( );

    //
	//     1    2
	// A: A1   A2
	//
	// B: B1   B2
	//
	//  A1: premulti sprite
	//  A2: premulti render
	//
	//  B1: non-premulti sprite
	//  B2: non-premulti render
	//
	
	const CCSize&  s = this->getContentSize ( );

    //CCLayerColor*  background = CCLayerColor::create(ccc4(40,40,40,255));
    //addChild(background);
    
    // sprite 1
    m_pSprite1 = CCSprite::create ( "Images/fire.png" );
    
    // sprite 2
    m_pSprite2 = CCSprite::create ( "Images/fire_rgba8888.pvr" );
    	    
    // Create the render texture 
    m_pRenderTexture = CCRenderTexture::create ( (KDuint) s.cx, (KDuint) s.cy, kCCTexture2DPixelFormat_RGBA4444 );    
    
    m_pRenderTexture->setPosition ( ccpMid ( s ) );
    
    // add the sprites to the render texture 
    m_pRenderTexture->addChild ( m_pSprite1 );
    m_pRenderTexture->addChild ( m_pSprite2 );
    m_pRenderTexture->setClearColor ( ccc4F ( 0, 0, 0, 0 ) );
    m_pRenderTexture->setClearFlags ( GL_COLOR_BUFFER_BIT );
    
    // add the render texture to the scene 
    this->addChild ( m_pRenderTexture );
    
    m_pRenderTexture->setAutoDraw ( KD_TRUE );
    
    this->scheduleUpdate ( );
    
    // Toggle clear on / off
    CCMenuItemFont*  pItem = CCMenuItemFont::create ( "Clear On/Off", this, menu_selector ( RenderTextureTargetNode::touched ) );
    CCMenu*  pMenu = CCMenu::create( pItem, KD_NULL );
    addChild ( pMenu );

    pMenu->setPosition ( ccpMid ( s ) );
}

KDvoid RenderTextureTargetNode::touched ( CCObject* pSender )
{
    if ( m_pRenderTexture->getClearFlags ( ) == 0 )
    {
        m_pRenderTexture->setClearFlags ( GL_COLOR_BUFFER_BIT );
    }
    else
    {
        m_pRenderTexture->setClearFlags ( 0 );
        m_pRenderTexture->setClearColor ( ccc4f ( CCRANDOM_0_1 ( ), CCRANDOM_0_1 ( ), CCRANDOM_0_1 ( ), 1 ) );
    }
}

KDvoid RenderTextureTargetNode::update ( KDfloat fDelta )
{
    static KDfloat  fTime = 0;
    KDfloat  r = 80;

    m_pSprite1->setPosition ( ccp ( kdCosf ( fTime * 2 ) * r, kdSinf ( fTime * 2 ) * r ) );
    m_pSprite2->setPosition ( ccp ( kdSinf ( fTime * 2 ) * r, kdCosf ( fTime * 2 ) * r ) );
    
    fTime += fDelta;
}

const KDchar* RenderTextureTargetNode::title ( KDvoid )
{
    return "Testing Render Target Node";
}

const KDchar* RenderTextureTargetNode::subtitle ( KDvoid )
{
    return "Sprites should be equal and move with each frame";
}

