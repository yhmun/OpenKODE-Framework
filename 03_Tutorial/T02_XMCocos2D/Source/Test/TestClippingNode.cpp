/* --------------------------------------------------------------------------
 *
 *      File            TestClippingNode.cpp
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
#include "TestClippingNode.h"

enum 
{
	kTagTitleLabel		= 1,
	kTagSubtitleLabel	= 2,
	kTagStencilNode		= 100,
	kTagClipperNode		= 101,
	kTagContentNode		= 102,
};

TestClippingNode* TestClippingNode::create ( KDvoid )
{
	TestClippingNode*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new ScrollViewDemo			( ); break;
		case  1 : pLayer = new HoleDemo					( ); break;
		case  2 : pLayer = new ShapeTest				( ); break;
		case  3 : pLayer = new ShapeInvertedTest		( ); break;
		case  4 : pLayer = new SpriteTest				( ); break;
		case  5 : pLayer = new SpriteNoAlphaTest		( ); break;
		case  6 : pLayer = new SpriteInvertedTest		( ); break;
		case  7 : pLayer = new NestedTest				( ); break;
		case  8 : pLayer = new RawStencilBufferTest		( ); break;
		case  9 : pLayer = new RawStencilBufferTest2	( ); break;
		case 10 : pLayer = new RawStencilBufferTest3	( ); break;
		case 11 : pLayer = new RawStencilBufferTest4	( ); break;
		case 12 : pLayer = new RawStencilBufferTest5	( ); break;
		case 13 : pLayer = new RawStencilBufferTest6	( ); break;
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

KDuint TestClippingNode::count ( KDvoid )
{
	return 14;
}

KDbool TestClippingNode::init  ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

    CCSprite*  pBackground = CCSprite::create ( "Images/background3.png" );
    pBackground->setPosition ( ccpMid ( tLyrSize ) );
	pBackground->setOpacity ( 127 );
    this->addChild ( pBackground, -1 );

	this->setup ( );

	return KD_TRUE;
}

KDvoid TestClippingNode::setup ( KDvoid )
{

}

//------------------------------------------------------------------
//
// BasicTest
//
//------------------------------------------------------------------

const KDchar* BasicTest::title ( KDvoid )
{
	return "Basic Test";
}

const KDchar* BasicTest::subtitle ( KDvoid )
{
	return "";
}

KDvoid BasicTest::setup ( KDvoid )
{
    const CCSize&  s = this->getContentSize ( );
    
    CCNode*  pStencil = this->stencil ( );
    pStencil->setTag ( kTagStencilNode );
    pStencil->setPosition ( ccp ( 50, 50 ) );
    
    CCClippingNode*  pClipper = this->clipper ( );
    pClipper->setTag ( kTagClipperNode );
    pClipper->setAnchorPoint (ccp ( 0.5f, 0.5f ) );
    pClipper->setPosition ( ccp ( s.cx / 2 - 50, s.cy / 2 - 50 ) );
    pClipper->setStencil ( pStencil);
    this->addChild ( pClipper );
    
    CCNode*  pContent = this->content ( );
    pContent->setPosition ( ccp ( 50, 50 ) );
    pClipper->addChild ( pContent );
}

CCAction* BasicTest::actionRotate ( KDvoid )
{
    return CCRepeatForever::create ( CCRotateBy::create ( 1.0f, 90.0f ) );
}

CCAction* BasicTest::actionScale ( KDvoid )
{
    CCScaleBy*  pScale = CCScaleBy::create ( 1.33f, 1.5f );
    return CCRepeatForever::create ( (CCActionInterval*) CCSequence::create ( pScale, pScale->reverse ( ), KD_NULL ) );
}

CCDrawNode* BasicTest::shape ( KDvoid )
{
	static CCPoint  aTriangle [ 3 ] =
	{
		ccp ( -100, -100 ),
		ccp (  100, -100 ),
		ccp (    0,  100 )
	};

	static ccColor4F  tGreen = { 0, 1, 0, 1 };

    CCDrawNode*  pShape = CCDrawNode::create ( );    
    pShape->drawPolygon ( aTriangle, 3, tGreen, 0, tGreen );

    return pShape;
}

CCSprite* BasicTest::grossini ( KDvoid )
{
    CCSprite *grossini = CCSprite::create ( "Images/grossini.png" );
    grossini->setScale( 1.5 );
    return grossini;
}

CCNode* BasicTest::stencil ( KDvoid )
{
    return KD_NULL;
}

CCClippingNode* BasicTest::clipper ( KDvoid )
{
    return CCClippingNode::create ( );
}

CCNode* BasicTest::content ( KDvoid )
{
    return KD_NULL;
}


//------------------------------------------------------------------
//
// ShapeTest
//
//------------------------------------------------------------------ 

const KDchar* ShapeTest::title ( KDvoid )
{
	return "Shape Basic Test";
}

const KDchar* ShapeTest::subtitle ( KDvoid )
{
	return "A DrawNode as stencil and Sprite as content";
}

CCNode* ShapeTest::stencil ( KDvoid )
{
    CCNode*  pNode = this->shape ( );
    pNode->runAction ( this->actionRotate ( ) );
    return pNode;
}

CCNode* ShapeTest::content ( KDvoid )
{
    CCNode*  pNode = this->grossini ( );
    pNode->runAction ( this->actionScale ( ) );
    return pNode;
}

//------------------------------------------------------------------
//
// ShapeInvertedTest
//
//------------------------------------------------------------------ 

const KDchar* ShapeInvertedTest::title ( KDvoid )
{
	return "Shape Inverted Basic Test";
}

const KDchar* ShapeInvertedTest::subtitle ( KDvoid )
{
	return "A DrawNode as stencil and Sprite as content, inverted";
}

CCClippingNode* ShapeInvertedTest::clipper ( KDvoid )
{
    CCClippingNode*  pClipper = ShapeTest::clipper ( );
    pClipper->setInverted ( KD_TRUE );
    return pClipper;
}

//------------------------------------------------------------------
//
// SpriteTest
//
//------------------------------------------------------------------

const KDchar* SpriteTest::title ( KDvoid )
{
	return "Sprite Basic Test";
}

const KDchar* SpriteTest::subtitle ( KDvoid )
{
	return "A Sprite as stencil and DrawNode as content";
}

CCNode* SpriteTest::stencil ( KDvoid )
{
    CCNode*  pNode = this->grossini ( );
    pNode->runAction ( this->actionRotate ( ) );
    return pNode;
}

CCClippingNode* SpriteTest::clipper ( KDvoid )
{
    CCClippingNode*  pClipper = BasicTest::clipper ( );
    pClipper->setAlphaThreshold ( 0.05f );
    return pClipper;
}

CCNode* SpriteTest::content ( KDvoid )
{
    CCNode*  pNode = this->shape ( );
    pNode->runAction ( this->actionScale ( ) );
    return pNode;
}

//------------------------------------------------------------------
//
// SpriteNoAlphaTest
//
//------------------------------------------------------------------

const KDchar* SpriteNoAlphaTest::title ( KDvoid )
{
	return "Sprite No Alpha Basic Test";
}

const KDchar* SpriteNoAlphaTest::subtitle ( KDvoid )
{
	return "A Sprite as stencil and DrawNode as content, no alpha";
}

CCClippingNode* SpriteNoAlphaTest::clipper ( KDvoid )
{
    CCClippingNode*  pClipper = SpriteTest::clipper ( );
    pClipper->setAlphaThreshold ( 1 );
    return pClipper;
}

//------------------------------------------------------------------
//
// SpriteInvertedTest
//
//------------------------------------------------------------------

const KDchar* SpriteInvertedTest::title ( KDvoid )
{
	return "Sprite Inverted Basic Test";
}

const KDchar* SpriteInvertedTest::subtitle ( KDvoid )
{
	return "A Sprite as stencil and DrawNode as content, inverted";
}

CCClippingNode* SpriteInvertedTest::clipper ( KDvoid )
{
    CCClippingNode*  pClipper = SpriteTest::clipper ( );
    pClipper->setAlphaThreshold ( 0.05f );
    pClipper->setInverted ( KD_TRUE );
    return pClipper;
}

//------------------------------------------------------------------
//
// NestedTest
//
//------------------------------------------------------------------

const KDchar* NestedTest::title ( KDvoid )
{
	return "Nested Test";
}

const KDchar* NestedTest::subtitle ( KDvoid )
{
	return "Nest 9 Clipping Nodes, max is usually 8";
}

KDvoid NestedTest::setup ( KDvoid )
{
    static KDint  nDepth = 9;
    
    CCNode*  pParent = this;
    
    for ( KDint i = 0; i < nDepth; i++ )
	{                
        KDint  nSize = 225 - i * ( 225 / ( nDepth * 2 ) );

        CCClippingNode*  pClipper = CCClippingNode::create ( );
        pClipper->setContentSize ( ccs ( nSize, nSize ) );
        pClipper->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
        pClipper->setPosition	 ( ccpMid ( pParent->getContentSize ( ) ) );
        pClipper->setAlphaThreshold ( 0.05f );
        pClipper->runAction ( CCRepeatForever::create ( CCRotateBy::create ( i % 3 ? 1.33f : 1.66f, i % 2 ? 90.f : -90.f ) ) );
        pParent->addChild ( pClipper );
        
        CCNode*  pStencil = CCSprite::create ( "Images/grossini.png" );
        pStencil->setScale ( 2.5f - ( i * ( 2.5f / nDepth ) ) );
        pStencil->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
        pStencil->setPosition ( ccpMid ( pClipper->getContentSize ( ) ) );
        pStencil->setVisible ( KD_FALSE );
        pStencil->runAction  ( CCSequence::createWithTwoActions ( CCDelayTime::create ( i * 1.f ), CCShow::create ( ) ) );
        pClipper->setStencil ( pStencil );
        pClipper->addChild ( pStencil );
        
        pParent = pClipper;
    }
}

//------------------------------------------------------------------
//
// HoleDemo
//
//------------------------------------------------------------------

KDvoid HoleDemo::onExit ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pOuterClipper );
    CC_SAFE_RELEASE ( m_pHoles );
    CC_SAFE_RELEASE ( m_pHolesStencil );

	TestClippingNode::onExit ( );
}

const KDchar* HoleDemo::title ( KDvoid )
{
	return "Hole Demo";
}

const KDchar* HoleDemo::subtitle ( KDvoid )
{
	return "Touch/click to poke holes";
}

KDvoid HoleDemo::setup ( KDvoid )
{
    CCSprite*  pTarget = CCSprite::create ( "Images/blocks.png" );
    pTarget->setAnchorPoint ( CCPointZero );
    pTarget->setScale ( 3 );
    
    m_pOuterClipper = CCClippingNode::create ( );
    m_pOuterClipper->retain ( );
    CCAffineTransform  tTranform = CCAffineTransformMakeIdentity ( );
    tTranform = CCAffineTransformScale ( tTranform, pTarget->getScale ( ), pTarget->getScale ( ) );

    m_pOuterClipper->setContentSize ( CCSizeApplyAffineTransform ( pTarget->getContentSize ( ), tTranform ) );
    m_pOuterClipper->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    m_pOuterClipper->setPosition ( ccpMult ( ccpFromSize ( this->getContentSize ( ) ), 0.5f ) );
    m_pOuterClipper->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 1, 45 ) ) );
    
    m_pOuterClipper->setStencil( pTarget );
    
    CCClippingNode*  pHolesClipper = CCClippingNode::create ( );
    pHolesClipper->setInverted ( KD_TRUE );
    pHolesClipper->setAlphaThreshold ( 0.05f );
    
    pHolesClipper->addChild ( pTarget );
    
    m_pHoles = CCNode::create ( );
    m_pHoles->retain ( );
    
    pHolesClipper->addChild ( m_pHoles );
    
    m_pHolesStencil = CCNode::create ( );
    m_pHolesStencil->retain ( );
    
    pHolesClipper->setStencil ( m_pHolesStencil );
    
    m_pOuterClipper->addChild ( pHolesClipper );
    
    this->addChild ( m_pOuterClipper );
        
    this->setTouchEnabled ( KD_TRUE );
}

KDvoid HoleDemo::pokeHoleAtPoint ( CCPoint tPoint )
{
    KDfloat   fScale	= CCRANDOM_0_1 ( ) * 0.2f + 0.9f;
    KDfloat   fRotation = CCRANDOM_0_1 ( ) * 360;
    
    CCSprite*  pHole = CCSprite::create ( "Images/hole_effect.png" );
    pHole->setPosition ( tPoint );
    pHole->setRotation ( fRotation );
    pHole->setScale ( fScale );
    
    m_pHoles->addChild ( pHole );
    
    CCSprite*  pHoleStencil = CCSprite::create("Images/hole_stencil.png");
    pHoleStencil->setPosition ( tPoint );
    pHoleStencil->setRotation ( fRotation );
    pHoleStencil->setScale ( fScale );
    
    m_pHolesStencil->addChild ( pHoleStencil );

    m_pOuterClipper->runAction 
	(
		CCSequence::createWithTwoActions ( CCScaleBy::create ( 0.05f, 0.95f ),CCScaleTo::create ( 0.125f, 1 ) )
	);
}

KDvoid HoleDemo::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint   tPoint = m_pOuterClipper->convertTouchToNodeSpace ( pTouch );
    CCRect    tRect  = ccr ( 0, 0, m_pOuterClipper->getContentSize ( ).cx, m_pOuterClipper->getContentSize ( ).cy );

    if ( !tRect.containsPoint ( tPoint ) )
	{
		return;
	}

    this->pokeHoleAtPoint ( tPoint );
}

//------------------------------------------------------------------
//
// ScrollViewDemo
//
//------------------------------------------------------------------

const KDchar* ScrollViewDemo::title ( KDvoid )
{
	return "Scroll View Demo";
}

const KDchar* ScrollViewDemo::subtitle ( KDvoid )
{
	return "Move/drag to scroll the content";
}

KDvoid ScrollViewDemo::setup ( KDvoid )
{
    CCClippingNode*  pClipper = CCClippingNode::create ( );
    pClipper->setTag ( kTagClipperNode );
    pClipper->setContentSize (  ccs ( 200, 200 ) );
    pClipper->setAnchorPoint (  ccp ( 0.5f, 0.5f ) );
    pClipper->setPosition ( ccpMid ( this->getContentSize ( ) ) );
    pClipper->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 1, 45 ) ) );
    this->addChild ( pClipper );

    CCDrawNode*  pStencil = CCDrawNode::create ( );
    CCPoint		 aRectangle [ 4 ];

    aRectangle [ 0 ] = ccp ( 0, 0 );
    aRectangle [ 1 ] = ccp ( pClipper->getContentSize ( ).cx, 0 );
    aRectangle [ 2 ] = ccp ( pClipper->getContentSize ( ).cx, pClipper->getContentSize ( ).cy );
    aRectangle [ 3 ] = ccp ( 0, pClipper->getContentSize ( ).cy );
    
    ccColor4F    tWhite = { 1, 1, 1, 1 };
    pStencil->drawPolygon ( aRectangle, 4, tWhite, 1, tWhite );
    pClipper->setStencil  ( pStencil );

    CCSprite*  pContent = CCSprite::create ( "Images/background2.png" );
    pContent->setTag ( kTagContentNode );
    pContent->setAnchorPoint (  ccp ( 0.5f, 0.5f ) );
    pContent->setPosition ( ccpMid ( pClipper->getContentSize ( ) ) );
    pClipper->addChild ( pContent);
    
    m_bScrolling = KD_FALSE;

    this->setTouchEnabled ( KD_TRUE );
}

KDvoid ScrollViewDemo::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*  pTouch	= (CCTouch*) pTouches->anyObject ( );
    CCNode*	  pClipper	= this->getChildByTag ( kTagClipperNode );
	CCPoint   tPoint	= pClipper->convertTouchToNodeSpace ( pTouch );
    CCRect    tRect		= ccr ( 0, 0, pClipper->getContentSize ( ).cx, pClipper->getContentSize ( ).cy );
    m_bScrolling = tRect.containsPoint ( tPoint );
    m_tLastPoint = tPoint;
}

KDvoid ScrollViewDemo::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( !m_bScrolling )
	{
		return;
	}

	CCTouch*  pTouch	= (CCTouch*) pTouches->anyObject ( );
    CCNode*	  pClipper	= this->getChildByTag ( kTagClipperNode );
	CCPoint   tPoint	= pClipper->convertTouchToNodeSpace ( pTouch );
	CCPoint	  tDiff		= ccpSub ( tPoint, m_tLastPoint );
    CCNode*   pContent  = pClipper->getChildByTag ( kTagContentNode );
    pContent->setPosition ( ccpAdd ( pContent->getPosition ( ), tDiff ) );
    m_tLastPoint = tPoint;
}

KDvoid ScrollViewDemo::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( !m_bScrolling )
	{
		return;
	}

    m_bScrolling = KD_FALSE;
}

//------------------------------------------------------------------
//
// RawStencilBufferTests
//
//------------------------------------------------------------------

static GLint			s_nStencilBits		= -1;
static const GLfloat	s_fAlphaThreshold	= 0.05f;
static const KDint		s_nPlaneCount		= 8;
static const ccColor4F	s_aPlaneColor[]		=
{
    {    0,    0,    0, 0.65f },
    { 0.7f,    0,    0, 0.60f },
    {    0, 0.7f,    0, 0.55f },
    {    0,    0, 0.7f, 0.50f },
    { 0.7f, 0.7f,    0, 0.45f },
    {    0, 0.7f, 0.7f, 0.40f },
    { 0.7f,    0, 0.7f, 0.35f },
    { 0.7f, 0.7f, 0.7f, 0.30f },
};

KDvoid RawStencilBufferTest::onExit ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pSprite );

	TestClippingNode::onExit ( );
}

const KDchar* RawStencilBufferTest::title ( KDvoid )
{
	return "Raw Stencil Tests";
}

const KDchar* RawStencilBufferTest::subtitle ( KDvoid )
{
	return "1:Default";
}

KDvoid RawStencilBufferTest::setup ( KDvoid )
{
    glGetIntegerv ( GL_STENCIL_BITS, &s_nStencilBits );

    if ( s_nStencilBits < 3 )
	{
        CCLOG ( "Stencil must be enabled for the current CCGLView." );
    }

    m_pSprite = CCSprite::create ( "Images/grossini.png" );
    m_pSprite->retain ( );
    m_pSprite->setAnchorPoint ( ccp ( 0.5f, 0 ) );
    m_pSprite->setScale ( 2.5f );

    CCDirector::sharedDirector ( )->setAlphaBlending ( KD_TRUE );
}

KDvoid RawStencilBufferTest::draw ( KDvoid )
{    
	CCPoint  tLyrPoint  = ccp ( this->getContentSize ( ).cx, 320 );   
    CCPoint  tPlaneSize = ccpMult ( tLyrPoint, 1.0f / s_nPlaneCount );
    
    glEnable ( GL_STENCIL_TEST );
    CHECK_GL_ERROR_DEBUG ( );
        
	kmGLPushMatrix ( );
	kmGLTranslatef ( 0, 80, 0 );

    for ( KDint i = 0; i < s_nPlaneCount; i++ )
	{        
        CCPoint  tStencilPoint = ccpMult ( tPlaneSize, s_nPlaneCount * 1.f - i );
        tStencilPoint.x  = tLyrPoint.x;
        
        CCPoint  tSpritePoint = ccpMult ( tPlaneSize, i * 1.f );
        tSpritePoint.x += tPlaneSize.x / 2 - 60;
		tSpritePoint.y = 0;
        m_pSprite->setPosition ( tSpritePoint );

        this->setupStencilForClippingOnPlane ( i );
        CHECK_GL_ERROR_DEBUG ( );

        ccDrawSolidRect ( CCPointZero, tStencilPoint, ccc4f ( 1, 1, 1, 1 ) );
        
        kmGLPushMatrix	 ( );		
        this->transform  ( );
        m_pSprite->visit ( );
        kmGLPopMatrix	 ( );
        
        this->setupStencilForDrawingOnPlane ( i );
        CHECK_GL_ERROR_DEBUG ( );
        
        ccDrawSolidRect ( CCPointZero, tLyrPoint, s_aPlaneColor [ i ] );
        
        kmGLPushMatrix	 ( );
        this->transform  ( );
        m_pSprite->visit ( );
        kmGLPopMatrix    ( );
    }
    
	kmGLPopMatrix    ( );

    glDisable ( GL_STENCIL_TEST );
    CHECK_GL_ERROR_DEBUG ( );
}

KDvoid RawStencilBufferTest::setupStencilForClippingOnPlane ( GLint nPlane )
{
    GLint  nPlaneMask = 0x1 << nPlane;
    glStencilMask  ( nPlaneMask );
    glClearStencil ( 0x0 );
    glClear ( GL_STENCIL_BUFFER_BIT );
    glFlush ( );
    glStencilFunc ( GL_NEVER, nPlaneMask, nPlaneMask );
    glStencilOp   ( GL_REPLACE, GL_KEEP, GL_KEEP );
}

KDvoid RawStencilBufferTest::setupStencilForDrawingOnPlane ( GLint nPlane )
{
    GLint  nPlaneMask = 0x1 << nPlane;
    GLint  nEqualOrLessPlanesMask = nPlaneMask | ( nPlaneMask - 1 );
    glStencilFunc ( GL_EQUAL, nEqualOrLessPlanesMask, nEqualOrLessPlanesMask );
    glStencilOp   ( GL_KEEP, GL_KEEP, GL_KEEP );
}

//------------------------------------------------------------------
//
// RawStencilBufferTest2
//
//------------------------------------------------------------------

const KDchar* RawStencilBufferTest2::subtitle ( KDvoid )
{
	return "2:DepthMask:FALSE";
}

KDvoid RawStencilBufferTest2::setupStencilForClippingOnPlane ( GLint nPlane )
{
    RawStencilBufferTest::setupStencilForClippingOnPlane ( nPlane );
    glDepthMask ( GL_FALSE );
}

KDvoid RawStencilBufferTest2::setupStencilForDrawingOnPlane(GLint nPlane)
{
    glDepthMask ( GL_TRUE );
    RawStencilBufferTest::setupStencilForDrawingOnPlane ( nPlane );
}

//------------------------------------------------------------------
//
// RawStencilBufferTest3
//
//------------------------------------------------------------------

const KDchar* RawStencilBufferTest3::subtitle ( KDvoid )
{
	return "3:DepthTest:DISABLE,DepthMask:FALSE";
}

KDvoid RawStencilBufferTest3::setupStencilForClippingOnPlane ( GLint nPlane )
{
    RawStencilBufferTest::setupStencilForClippingOnPlane ( nPlane );
    glDisable ( GL_DEPTH_TEST );
    glDepthMask ( GL_FALSE );
}

KDvoid RawStencilBufferTest3::setupStencilForDrawingOnPlane ( GLint nPlane )
{
    glDepthMask ( GL_TRUE );
//	glEnable ( GL_DEPTH_TEST );
    RawStencilBufferTest::setupStencilForDrawingOnPlane ( nPlane );
}

//------------------------------------------------------------------
//
// RawStencilBufferTest4
//
//------------------------------------------------------------------

const KDchar* RawStencilBufferTest4::subtitle ( KDvoid )
{
	return "4:DepthMask:FALSE,AlphaTest:ENABLE";
}

KDvoid RawStencilBufferTest4::setupStencilForClippingOnPlane ( GLint nPlane )
{
    RawStencilBufferTest::setupStencilForClippingOnPlane ( nPlane );
    glDepthMask ( GL_FALSE );

#if defined ( USE_OPEN_GLES2 )
    CCGLProgram*  pProgram = CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColorAlphaTest );
    GLint  nAlphaValueLocation = glGetUniformLocation ( pProgram->getProgram ( ), kCCUniformAlphaTestValue );
    pProgram->setUniformLocationWith1f ( nAlphaValueLocation, s_fAlphaThreshold );
    m_pSprite->setShaderProgram ( pProgram );
#else
	glEnable ( GL_ALPHA_TEST );
    glAlphaFunc ( GL_GREATER, s_fAlphaThreshold );
#endif
}

KDvoid RawStencilBufferTest4::setupStencilForDrawingOnPlane ( GLint nPlane )
{
#if !defined ( USE_OPEN_GLES2 )
	glDisable ( GL_ALPHA_TEST );
#endif

    glDepthMask ( GL_TRUE );
    RawStencilBufferTest::setupStencilForDrawingOnPlane ( nPlane );
}

//------------------------------------------------------------------
//
// RawStencilBufferTest5
//
//------------------------------------------------------------------

const KDchar* RawStencilBufferTest5::subtitle ( KDvoid )
{
	return "5:DepthTest:DISABLE,DepthMask:FALSE,AlphaTest:ENABLE";
}

KDvoid RawStencilBufferTest5::setupStencilForClippingOnPlane ( GLint nPlane )
{
    RawStencilBufferTest::setupStencilForClippingOnPlane ( nPlane );

    glDisable ( GL_DEPTH_TEST );
    glDepthMask ( GL_FALSE );

#if defined ( USE_OPEN_GLES2 )
    CCGLProgram*  pProgram = CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColorAlphaTest );
    GLint  nAlphaValueLocation = glGetUniformLocation(pProgram->getProgram ( ), kCCUniformAlphaTestValue );
    pProgram->setUniformLocationWith1f ( nAlphaValueLocation, s_fAlphaThreshold );
    m_pSprite->setShaderProgram ( pProgram );
#else
	glEnable ( GL_ALPHA_TEST );
    glAlphaFunc ( GL_GREATER, s_fAlphaThreshold );
#endif

}

KDvoid RawStencilBufferTest5::setupStencilForDrawingOnPlane(GLint nPlane)
{
#if !defined ( USE_OPEN_GLES2 )
	glDisable ( GL_ALPHA_TEST );
#endif

    glDepthMask ( GL_TRUE );
//	glEnable ( GL_DEPTH_TEST );
    RawStencilBufferTest::setupStencilForDrawingOnPlane ( nPlane );
}

//------------------------------------------------------------------
//
// RawStencilBufferTest6
//
//------------------------------------------------------------------

const KDchar* RawStencilBufferTest6::subtitle ( KDvoid )
{
	return "6:ManualClear,AlphaTest:ENABLE";
}

KDvoid RawStencilBufferTest6::setup ( KDvoid )
{
#if !defined ( USE_OPEN_GLES2 )
//    KDubyte  cBits = 0;

    glStencilMask ( ~0 );
    glClearStencil ( 0 );
    glClear ( GL_STENCIL_BUFFER_BIT );
    glFlush ( );

//    glReadPixels ( 0, 0, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &cBits );

//    CCLabelTTF*  pClearToZeroLabel = CCLabelTTF::create( ccszf ( "00=%02x", cBits ) , "Arial", 20 );
//    pClearToZeroLabel->setPosition ( ccp ( ( tLyrPoint.x / 3 ) * 1, tLyrPoint.y - 10 ) );
//    this->addChild ( pClearToZeroLabel );

    glStencilMask  ( 0x0F );
    glClearStencil ( 0xAA );
    glClear ( GL_STENCIL_BUFFER_BIT );
    glFlush ( );

//    glReadPixels ( 0, 0, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &cBits );

//    CCLabelTTF*  pClearToMaskLabel = CCLabelTTF::create ( ccszf ( "0a=%02x", cBits ), "Arial", 20 );
//    pClearToMaskLabel->setPosition( ccp ( ( tLyrPoint.x / 3 ) * 2, tLyrPoint.y - 10 ) );
//    this->addChild ( pClearToMaskLabel );

#endif

    glStencilMask ( ~0 );
    RawStencilBufferTest::setup ( );
}

KDvoid RawStencilBufferTest6::setupStencilForClippingOnPlane ( GLint nPlane )
{
    GLint  nPlaneMask = 0x1 << nPlane;
    glStencilMask ( nPlaneMask );
    glStencilFunc ( GL_NEVER, 0, nPlaneMask );
    glStencilOp   ( GL_REPLACE, GL_KEEP, GL_KEEP );
    ccDrawSolidRect ( CCPointZero, ccpFromSize ( this->getContentSize ( ) ), ccc4f ( 1, 1, 1, 1 ) );
    glStencilFunc ( GL_NEVER, nPlaneMask, nPlaneMask );
    glStencilOp ( GL_REPLACE, GL_KEEP, GL_KEEP );
    glDisable   ( GL_DEPTH_TEST );
    glDepthMask ( GL_FALSE );

#if defined ( USE_OPEN_GLES2 )
    CCGLProgram*  pProgram = CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColorAlphaTest );
    GLint  nAlphaValueLocation = glGetUniformLocation(pProgram->getProgram ( ), kCCUniformAlphaTestValue );
    pProgram->setUniformLocationWith1f ( nAlphaValueLocation, s_fAlphaThreshold );
    m_pSprite->setShaderProgram ( pProgram );
#else
	glEnable ( GL_ALPHA_TEST );
    glAlphaFunc ( GL_GREATER, s_fAlphaThreshold );
#endif

    glFlush ( );
}

KDvoid RawStencilBufferTest6::setupStencilForDrawingOnPlane ( GLint nPlane )
{
#if !defined ( USE_OPEN_GLES2 )
	glDisable ( GL_ALPHA_TEST );
#endif
    
	glDepthMask ( GL_TRUE );
//	glEnable ( GL_DEPTH_TEST );

    RawStencilBufferTest::setupStencilForDrawingOnPlane ( nPlane );
    glFlush ( );
}
