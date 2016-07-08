/* --------------------------------------------------------------------------
 *
 *      File            TestActions.cpp
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
#include "TestActions.h"

TestActions* TestActions::create ( KDvoid )
{
	TestActions*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0	:	pLayer = new ActionManual						( );  break;
		case  1	:	pLayer = new ActionMove							( );  break;
		case  2	:	pLayer = new ActionRotate						( );  break;
		case  3	:	pLayer = new ActionScale						( );  break;	
		case  4	:	pLayer = new ActionSkew							( );  break;
		case  5 :	pLayer = new ActionRotationalSkew				( );  break;
		case  6 :	pLayer = new ActionRotationalSkewVSStandardSkew	( );  break;
		case  7	:	pLayer = new ActionSkewRotateScale				( );  break;
		case  8	:	pLayer = new ActionJump							( );  break;
		case  9	:	pLayer = new ActionCardinalSpline				( );  break;
		case 10	:	pLayer = new ActionCatmullRom					( );  break;
		case 11	:	pLayer = new ActionBezier						( );  break;
		case 12	:	pLayer = new ActionBlink						( );  break;
		case 13	:	pLayer = new ActionFade							( );  break;
		case 14	:	pLayer = new ActionTint							( );  break;
		case 15	:	pLayer = new ActionAnimate						( );  break;
		case 16	:	pLayer = new ActionSequence						( );  break;
		case 17	:	pLayer = new ActionSequence2					( );  break;
		case 18	:	pLayer = new ActionRemoveSelf					( );  break;
		case 19	:	pLayer = new ActionSpawn						( );  break;
		case 20	:	pLayer = new ActionReverse						( );  break;
		case 21	:	pLayer = new ActionDelayTime					( );  break;
		case 22	:	pLayer = new ActionRepeat						( );  break;
		case 23 :	pLayer = new ActionRepeatForever				( );  break;
		case 24 :	pLayer = new ActionRotateToRepeat				( );  break;
		case 25	:	pLayer = new ActionRotateJerk					( );  break;    
		case 26	:	pLayer = new ActionCallFunc						( );  break;
		case 27	:	pLayer = new ActionCallFuncND					( );  break;
		case 28	:	pLayer = new ActionReverseSequence				( );  break;
		case 29 :	pLayer = new ActionReverseSequence2				( );  break;
		case 30	:	pLayer = new ActionOrbit						( );  break;
		case 31	:	pLayer = new ActionFollow						( );  break;
		case 32	:	pLayer = new ActionTargeted						( );  break;
		case 33	:	pLayer = new ActionMoveStacked					( );  break;
		case 34	:	pLayer = new ActionMoveJumpStacked				( );  break;
		case 35	:	pLayer = new ActionMoveBezierStacked			( );  break;
		case 36	:	pLayer = new ActionCardinalSplineStacked		( );  break;
		case 37	:	pLayer = new ActionCatmullRomStacked			( );  break;
		case 38	:	pLayer = new PauseResumeActions					( );  break;	
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

KDuint TestActions::count ( KDvoid )
{
	return 39;
}

KDbool TestActions::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_pGrossini = CCSprite::create ( "Images/grossini.png" ); 	
	m_pTamara   = CCSprite::create ( "Images/grossinis_sister1.png" ); 	
	m_pKathia   = CCSprite::create ( "Images/grossinis_sister2.png" ); 	

	this->addChild ( m_pGrossini, 0 );
	this->addChild ( m_pTamara  , 1 );
	this->addChild ( m_pKathia  , 2 );

	return KD_TRUE;
}

KDvoid TestActions::centerSprites ( KDuint uNumberOfSprites )
{
	const CCSize&  tLyrSize = this->getContentSize ( );

	switch ( uNumberOfSprites )
	{
		case 0 :
			m_pTamara  ->setVisible  ( KD_FALSE );
			m_pKathia  ->setVisible  ( KD_FALSE );
			m_pGrossini->setVisible  ( KD_FALSE );
			break;

		case 1 :
			m_pTamara  ->setVisible  ( KD_FALSE );
			m_pKathia  ->setVisible  ( KD_FALSE );				
			m_pGrossini->setPosition ( ccpMid ( tLyrSize ) );
			break;

		case 2 :			
			m_pKathia  ->setPosition ( ccp ( tLyrSize.cx / 3    , tLyrSize.cy / 2 ) );
			m_pTamara  ->setPosition ( ccp ( tLyrSize.cx / 3 * 2, tLyrSize.cy / 2 ) );			
			m_pGrossini->setVisible  ( KD_FALSE );
			break;

		case 3 :			
			m_pGrossini->setPosition ( ccpMid ( tLyrSize ) );
			m_pTamara  ->setPosition ( ccp ( tLyrSize.cx / 4    , tLyrSize.cy / 2 ) );
			m_pKathia  ->setPosition ( ccp ( tLyrSize.cx / 4 * 3, tLyrSize.cy / 2 ) );
			break;
	}	
}

KDvoid TestActions::alignSpritesLeft ( KDuint uNumberOfSprites )
{
    const CCSize&  tLyrSize = this->getContentSize ( );

	switch ( uNumberOfSprites )
	{
		case 0 :
			m_pTamara  ->setVisible  ( KD_FALSE );
			m_pKathia  ->setVisible  ( KD_FALSE );
			m_pGrossini->setVisible  ( KD_FALSE );
			break;

		case 1 :			
			m_pTamara  ->setVisible  ( KD_FALSE );
			m_pKathia  ->setVisible  ( KD_FALSE );
			m_pGrossini->setPosition ( ccp ( 60, tLyrSize.cy / 2 ) );
			break;

		case 2 :						
			m_pKathia  ->setPosition ( ccp ( 60,     tLyrSize.cy / 3 ) );
			m_pTamara  ->setPosition ( ccp ( 60, 2 * tLyrSize.cy / 3 ) );		
			m_pGrossini->setVisible  ( KD_FALSE );	
			break;

		case 3 :			
			m_pGrossini->setPosition ( ccp ( 60,     tLyrSize.cy / 2 ) );
			m_pTamara  ->setPosition ( ccp ( 60, 2 * tLyrSize.cy / 3 ) );
			m_pKathia  ->setPosition ( ccp ( 60,     tLyrSize.cy / 3 ) );
			break;
	}
}

//------------------------------------------------------------------
//
// ActionManual
//
//------------------------------------------------------------------
KDbool ActionManual::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );

    m_pGrossini->setRotation ( 120 );
    m_pGrossini->setPosition ( ccpMid ( tLyrSize ) );
    m_pGrossini->setColor    ( ccc3 ( 255, 0, 0 ) );

	m_pTamara->setPosition ( ccp ( 100, 70 ) );
    m_pTamara->setScaleX   ( 2.5f );
    m_pTamara->setScaleY   ( -1.0f );
    m_pTamara->setOpacity  ( 128 );

    m_pKathia->setPosition ( ccp ( tLyrSize.cx - 100, tLyrSize.cy / 2 ) );
    m_pKathia->setColor    ( ccBLUE );

	return KD_TRUE;
}

const KDchar* ActionManual::subtitle ( KDvoid )
{
    return "Manual Transformation";
}

//------------------------------------------------------------------
//
//	ActionMove
//
//------------------------------------------------------------------
KDbool ActionMove::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );

    centerSprites ( 3 );

	CCActionInterval*  actionTo = CCMoveTo::create ( 2, ccp ( tLyrSize.cx - 40, tLyrSize.cy - 40 ) );
	CCActionInterval*  actionBy = CCMoveBy::create ( 2, ccp ( 80, 80 ) );
	CCActionInterval*  actionByBack = actionBy->reverse ( );

	m_pGrossini->runAction ( CCSequence::create ( actionBy, actionByBack, KD_NULL ) );
	m_pTamara->runAction ( CCMoveTo::create ( 1, ccp ( 40, 40 ) ) );
	m_pKathia->runAction ( actionTo );

	return KD_TRUE;
}

const KDchar* ActionMove::subtitle ( KDvoid )
{
    return "MoveTo / MoveBy";
}

//------------------------------------------------------------------
//
// ActionScale
//
//------------------------------------------------------------------
KDbool ActionScale::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 3 );

	CCActionInterval*  actionTo  = CCScaleTo::create ( 2, 0.5f );
	CCActionInterval*  actionBy  = CCScaleBy::create ( 2, 1.0f, 10.0f );
	CCActionInterval*  actionBy2 = CCScaleBy::create ( 2, 5.0f, 1.0f );

	m_pGrossini->runAction ( actionTo );
	m_pTamara->runAction ( CCSequence::create ( actionBy , actionBy ->reverse ( ), KD_NULL ) );
	m_pKathia->runAction ( CCSequence::create ( actionBy2, actionBy2->reverse ( ), KD_NULL ) );

	return KD_TRUE;
    
}

const KDchar* ActionScale::subtitle ( KDvoid )
{
    return "ScaleTo / ScaleBy";
}

//------------------------------------------------------------------
//
//	ActionSkew
//
//------------------------------------------------------------------
KDbool ActionSkew::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	centerSprites ( 3 );

	CCActionInterval*  actionTo		= CCSkewTo::create ( 2, 37.2f, -37.2f );
	CCActionInterval*  actionToBack = CCSkewTo::create ( 2, 0, 0 );
	CCActionInterval*  actionBy		= CCSkewBy::create ( 2, 0.0f, -90.0f );
	CCActionInterval*  actionBy2	= CCSkewBy::create ( 2, 45.0f, 45.0f );

	m_pGrossini->runAction ( CCSequence::create ( actionBy , actionBy->reverse ( ), KD_NULL ) );
	m_pTamara->runAction ( CCSequence::create ( actionTo , actionToBack, KD_NULL ) );		
	m_pKathia->runAction ( CCSequence::create ( actionBy2, actionBy2->reverse ( ), KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionSkew::subtitle ( KDvoid )
{
	return "SkewTo / SkewBy";
}

//------------------------------------------------------------------
//
//	ActionRotationalSkew
//
//------------------------------------------------------------------
KDbool ActionRotationalSkew::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	centerSprites ( 3 );

    CCRotateTo*  actionTo     = CCRotateTo::create ( 2, 37.2f, -37.2f );
    CCRotateTo*  actionToBack = CCRotateTo::create ( 2, 0, 0 );
    CCRotateBy*  actionBy     = CCRotateBy::create ( 2, 0.0f, -90.0f );
    CCRotateBy*  actionBy2    = CCRotateBy::create ( 2, 45.0f, 45.0f );
    CCRotateBy*  actionByBack = (CCRotateBy*) actionBy->reverse ( );

    m_pTamara  ->runAction ( CCSequence::create ( actionTo , actionToBack, KD_NULL ) );
    m_pGrossini->runAction ( CCSequence::create ( actionBy , actionByBack, KD_NULL ) );
    m_pKathia  ->runAction ( CCSequence::create ( actionBy2, actionBy2->reverse ( ), KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionRotationalSkew::subtitle ( KDvoid )
{
	return "RotationalSkewTo / RotationalSkewBy";
}

//------------------------------------------------------------------
//
//	ActionRotationalSkewVSStandardSkew
//
//------------------------------------------------------------------
KDbool ActionRotationalSkewVSStandardSkew::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    m_pTamara  ->removeFromParent ( );
    m_pGrossini->removeFromParent ( );
    m_pKathia  ->removeFromParent ( );

    const CCSize&  tLyrSize = this->getContentSize ( );

    CCSize  tBoxSize = ccs ( 100.0f, 100.0f );

    CCLayerColor*  pBox = CCLayerColor::create ( ccc4 ( 255, 255, 0, 255 ) );
    pBox->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    pBox->setContentSize ( tBoxSize );
    pBox->ignoreAnchorPointForPosition ( KD_FALSE );
    pBox->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy * 2 / 3 - pBox->getContentSize ( ).cy / 2 ) );
    this->addChild ( pBox );

    CCLabelTTF*  pLabel = CCLabelTTF::create ( "Standard cocos2d Skew", "fonts/Marker Felt.ttf", 20 );
    pLabel->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy * 2 / 3 + pLabel->getContentSize ( ).cy ) );
    this->addChild ( pLabel );

    CCSkewBy*  actionTo = CCSkewBy::create ( 2, 360, 0 );
    CCSkewBy*  actionToBack = CCSkewBy::create ( 2, -360, 0 );
    pBox->runAction ( CCSequence::create ( actionTo, actionToBack, KD_NULL ) );

    pBox = CCLayerColor::create ( ccc4 ( 255, 255, 0, 255 ) );
    pBox->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    pBox->setContentSize ( tBoxSize );
    pBox->ignoreAnchorPointForPosition ( KD_FALSE );
    pBox->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 3 - pBox->getContentSize ( ).cy / 2 ) );
    this->addChild ( pBox );

    pLabel = CCLabelTTF::create ( "Rotational Skew", "fonts/Marker Felt.ttf", 20 );
    pLabel->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 3 + pLabel->getContentSize ( ).cy / 2 + 10 ) );
    this->addChild ( pLabel );

    CCRotateBy*  actionTo2     = CCRotateBy::create ( 2,  360, 0 );
    CCRotateBy*  actionToBack2 = CCRotateBy::create ( 2, -360, 0 );
    pBox->runAction ( CCSequence::create(actionTo2, actionToBack2, KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionRotationalSkewVSStandardSkew::subtitle ( KDvoid )
{
	return "Skew Comparison";
}

//------------------------------------------------------------------
//
//	ActionSkewRotateScale
//
//------------------------------------------------------------------
KDbool ActionSkewRotateScale::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	m_pGrossini->removeFromParent ( );
	m_pTamara  ->removeFromParent ( );
	m_pKathia  ->removeFromParent ( );

	CCSize    tSize = ccs ( 100.0f, 100.0f );
	CCLayerColor*  pBox = CCLayerColor::create ( ccc4 ( 255, 255, 0, 255 ) );
	this->addChild ( pBox );
	
	pBox->setContentSize ( tSize );
	pBox->setAnchorPoint ( ccp ( 0, 0 ) );
	pBox->setPosition ( ccp ( tLyrSize.cx / 2 - 50, tLyrSize.cy / 2 - 50 ) );

	KDfloat  fMarkRSide = 10.0f;
	CCLayerColor*  pUL = CCLayerColor::create ( ccc4 ( 255, 0, 0, 255 ) );

	pUL->setContentSize ( ccs ( fMarkRSide, fMarkRSide ) );
	pUL->setPosition ( ccp ( 0.f, tSize.cy - fMarkRSide ) );
	pUL->setAnchorPoint ( ccp ( 0, 0 ) );

	pBox->addChild ( pUL );

	CCLayerColor*  pUR = CCLayerColor::create ( ccc4 ( 0, 0, 255, 255 ) );

	pUR->setContentSize ( ccs ( fMarkRSide, fMarkRSide ) );
	pUR->setPosition ( ccp ( tSize.cx - fMarkRSide, tSize.cy - fMarkRSide ) );
	pUR->setAnchorPoint ( ccp ( 0, 0 ) );

	pBox->addChild ( pUR );
	
	CCActionInterval*       actionTo = CCSkewTo  ::create ( 2, 0.f, 2.f );
	CCActionInterval*       rotateTo = CCRotateTo::create ( 2, 61.0f );
	CCActionInterval*  actionScaleTo = CCScaleTo ::create ( 2, -0.44f, 0.47f );

	CCActionInterval*  actionScaleToBack = CCScaleTo ::create ( 2, 1.0f, 1.0f );
	CCActionInterval*       rotateToBack = CCRotateTo::create ( 2, 0 );
	CCActionInterval*       actionToBack = CCSkewTo  ::create ( 2, 0, 0 );

	pBox->runAction ( CCSequence::create ( actionTo, actionToBack, KD_NULL ) );
	pBox->runAction ( CCSequence::create ( rotateTo, rotateToBack, KD_NULL ) );
	pBox->runAction ( CCSequence::create ( actionScaleTo, actionScaleToBack, KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionSkewRotateScale::subtitle ( KDvoid )
{
	return "Skew + Rotate + Scale";
}

//------------------------------------------------------------------
//
//	ActionRotate
//
//------------------------------------------------------------------
KDbool ActionRotate::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 3 );

    CCActionInterval*  actionTo  = CCRotateTo::create ( 2,  45 );
    CCActionInterval*  actionTo2 = CCRotateTo::create ( 2, -45 );
    CCActionInterval*  actionTo0 = CCRotateTo::create ( 2 ,  0 );
    CCActionInterval*  actionBy  = CCRotateBy::create ( 2,  360 );
    CCActionInterval*  actionByBack = actionBy->reverse ( );

	m_pGrossini->runAction ( CCSequence::create ( actionBy , actionByBack, KD_NULL ) );
    m_pTamara->runAction ( CCSequence::create ( actionTo , actionTo0, KD_NULL ) );    
    m_pKathia->runAction ( CCSequence::create ( actionTo2, actionTo0->copy ( )->autorelease ( ), KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionRotate::subtitle ( KDvoid )
{
    return "RotateTo / RotateBy";
}

//------------------------------------------------------------------
//
// ActionJump
//
//------------------------------------------------------------------
KDbool ActionJump::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

    centerSprites ( 3 );

	CCActionInterval*  actionTo = CCJumpTo::create ( 2, CCPointMake ( s.cx / 5 * 3, s.cy / 3 * 2 ), 50, 4 );
    CCActionInterval*  actionBy = CCJumpBy::create ( 2, CCPointMake ( 300,   0 ), 50, 4 );
    CCActionInterval*  actionUp = CCJumpBy::create ( 2, CCPointMake (   0,   0 ), 80, 4 );
    CCActionInterval*  actionByBack = actionBy->reverse ( );

    m_pTamara->runAction ( actionTo );
    m_pGrossini->runAction ( CCSequence::create ( actionBy, actionByBack, KD_NULL ) );
    m_pKathia->runAction ( CCRepeatForever::create ( actionUp ) );

	return KD_TRUE;
}

const KDchar* ActionJump::subtitle ( KDvoid )
{
    return "JumpTo / JumpBy";
}

//------------------------------------------------------------------
//
// ActionBezier
//
//------------------------------------------------------------------
KDbool ActionBezier::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    const CCSize&  s = this->getContentSize ( );

    //
    // startPosition can be any coordinate, but since the movement
    // is relative to the Bezier curve, make it (0,0)
    //

    centerSprites ( 3 );

    // sprite 1
    ccBezierConfig  tBezierConfig;
    tBezierConfig.tControlPoint1 = ccp (   0,  s.cy / 2 );
    tBezierConfig.tControlPoint2 = ccp ( 300, -s.cy / 2 );
    tBezierConfig.tEndPosition   = ccp ( 300, 100 );

    CCActionInterval*  bezierForward = CCBezierBy::create ( 3, tBezierConfig );
    CCActionInterval*  bezierBack = bezierForward->reverse ( );	
    CCAction*  rep = CCRepeatForever::create ( (CCActionInterval*) CCSequence::create ( bezierForward, bezierBack, KD_NULL ) );

    // sprite 2
	m_pTamara->setPosition ( ccp ( s.cx / 5, s.cy / 2 ) );
    ccBezierConfig  tBezierConfig2;
	tBezierConfig2.tControlPoint1 = ccp ( s.cx / 4,  s.cy / 2 );
    tBezierConfig2.tControlPoint2 = ccp ( s.cx / 3, -s.cy / 2 );
    tBezierConfig2.tEndPosition   = ccp ( s.cx / 2,  s.cy / 2 );

    CCActionInterval*  bezierTo1 = CCBezierTo::create ( 2, tBezierConfig2 );	

    // sprite 3
	m_pKathia->setPosition ( ccp ( s.cx * 3 / 4, s.cy / 2 ) );
    CCActionInterval*  bezierTo2 = CCBezierTo::create ( 2, tBezierConfig2 );

    m_pGrossini->runAction ( rep );
    m_pTamara->runAction ( bezierTo1 );
    m_pKathia->runAction ( bezierTo2 );

	return KD_TRUE;

}

const KDchar* ActionBezier::subtitle ( KDvoid )
{
    return "BezierBy / BezierTo";
}

//------------------------------------------------------------------
//
// ActionBlink
//
//------------------------------------------------------------------
KDbool ActionBlink::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 2 );

    CCActionInterval*  action1 = CCBlink::create ( 2, 10 );
    CCActionInterval*  action2 = CCBlink::create ( 2,  5 );

    m_pTamara->runAction ( action1 );
    m_pKathia->runAction ( action2 );

	return KD_TRUE;
}

const KDchar* ActionBlink::subtitle ( KDvoid )
{
    return "Blink";
}

//------------------------------------------------------------------
//
// ActionFade
//
//------------------------------------------------------------------
KDbool ActionFade::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 2 );

    m_pTamara->setOpacity ( 0 );

    CCActionInterval*  action1 = CCFadeIn::create ( 1.0f );
    CCActionInterval*  action1Back = action1->reverse ( );

    CCActionInterval*  action2 = CCFadeOut::create ( 1.0f );
    CCActionInterval*  action2Back = action2->reverse ( );

    m_pTamara->runAction ( CCSequence::create ( action1, action1Back, KD_NULL ) );
    m_pKathia->runAction ( CCSequence::create ( action2, action2Back, KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionFade::subtitle ( KDvoid )
{
    return "FadeIn / FadeOut";
}

//------------------------------------------------------------------
//
// ActionTint
//
//------------------------------------------------------------------
KDbool ActionTint::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 2 );

    CCActionInterval*  action1 = CCTintTo::create ( 2,  255,    0,  255 );
    CCActionInterval*  action2 = CCTintBy::create ( 2, -127, -255, -127 );
    CCActionInterval*  action2Back = action2->reverse ( );

    m_pTamara->runAction ( action1 );
    m_pKathia->runAction ( CCSequence::create ( action2, action2Back, KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionTint::subtitle ( KDvoid )
{
    return "TintTo / TintBy";
}

//------------------------------------------------------------------
//
// ActionAnimate
//
//------------------------------------------------------------------
KDbool ActionAnimate::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 3 );

    //
    // Manual animation
    //
    CCAnimation*  pAnimation = CCAnimation::create ( );
	for ( KDint i = 1; i < 15; i++ )
    {
        KDchar  szName [100] = { 0 };
        kdSprintf ( szName, "Images/grossini_dance_%02d.png", i );
        pAnimation->addSpriteFrameWithFileName ( szName );
    }

    // should last 2.8 seconds. And there are 14 frames.
    pAnimation->setDelayPerUnit ( 2.8f / 14.0f );
    pAnimation->setRestoreOriginalFrame ( KD_TRUE );

	CCAnimate*  pAction = CCAnimate::create ( pAnimation );
	m_pGrossini->runAction ( CCSequence::create ( pAction, pAction->reverse ( ), KD_NULL ) );

    //
    // File animation
    //
    // With 2 loops and reverse
    CCAnimationCache*  pCache = CCAnimationCache::sharedAnimationCache ( );
    pCache->addAnimationsWithFile ( "animations/animations-2.plist" );
    CCAnimation*  pAnimation2 = pCache->animationByName ( "dance_1" );

    CCAnimate*  pAction2 = CCAnimate::create ( pAnimation2 );
    m_pTamara->runAction ( CCSequence::create ( pAction2, pAction2->reverse ( ), KD_NULL ) );

    //
    // File animation
    //
    // with 4 loops
    CCAnimation*  pAnimation3 = (CCAnimation*) pAnimation2->copy ( )->autorelease ( );
    pAnimation3->setLoops ( 4 );


    CCAnimate*  pAction3 = CCAnimate::create ( pAnimation3 );
    m_pKathia->runAction ( pAction3 );

	return KD_TRUE;
}

const KDchar* ActionAnimate::title ( KDvoid )
{
	return "Animation";
}

const KDchar* ActionAnimate::subtitle ( KDvoid )
{
    return "Center: Manual animation. Border: using file format animation";
}

//------------------------------------------------------------------
//
//	ActionSequence
//
//------------------------------------------------------------------
KDbool ActionSequence::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

    alignSpritesLeft ( 1 );

    CCFiniteTimeAction*  action = CCSequence::create 
	(
        CCMoveBy ::create ( 2, ccp ( s.cx / 2, 0 ) ), CCRotateBy::create ( 2,  540 ), KD_NULL
	);

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

const KDchar* ActionSequence::subtitle ( KDvoid )
{
    return "Sequence: Move + Rotate";
}

//------------------------------------------------------------------
//
//	ActionSequence2
//
//------------------------------------------------------------------
KDbool ActionSequence2::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );

    alignSpritesLeft ( 1 );

    m_pGrossini->setVisible ( KD_FALSE );

    CCFiniteTimeAction*  action = CCSequence::create 
	(
        CCPlace		::create ( ccp ( 200, s.cy / 2 ) ),
        CCShow		::create ( ),
        CCMoveBy	::create ( 1, ccp ( 200, 0 ) ),
        CCCallFunc	::create ( this, callfunc_selector ( ActionSequence2::onCallback1 ) ),
        CCCallFuncN	::create ( this, callfuncN_selector ( ActionSequence2::onCallback2 ) ),
        CCCallFuncND::create ( this, callfuncND_selector ( ActionSequence2::onCallback3 ), (KDvoid*) 0xbebabeba ),
        KD_NULL 
	);

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

KDvoid ActionSequence2::onCallback1 ( KDvoid )
{
    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF* label = CCLabelTTF::create ( "onCallback1 called", "fonts/Marker Felt.ttf", 16 );
	label->setPosition ( ccp ( s.cx / 2, s.cy / 2 + 30 ) );
	this->addChild ( label );
}

KDvoid ActionSequence2::onCallback2 ( CCNode* sender )
{
    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF*  label = CCLabelTTF::create ( "onCallback2 called", "fonts/Marker Felt.ttf", 16 );
	label->setPosition ( ccpMid ( s ) );
	this->addChild ( label );
}

KDvoid ActionSequence2::onCallback3 ( CCNode* sender, KDvoid* data )
{
    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF*  label = CCLabelTTF::create ( "onCallback3 called", "fonts/Marker Felt.ttf", 16 );
	label->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 30 ) );
	this->addChild ( label );
}

const KDchar* ActionSequence2::subtitle ( KDvoid )
{
    return "Sequence of InstantActions";
}

//------------------------------------------------------------------
//
//	ActionCallFunc
//
//------------------------------------------------------------------
KDbool ActionCallFunc::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 3 );

    CCFiniteTimeAction*  action = CCSequence::create 
	(
		CCMoveBy  ::create ( 2, ccp ( 200, 0 ) ),
		CCCallFunc::create ( this, callfunc_selector ( ActionCallFunc::onCallback1 ) ), 
		KD_NULL
	);

    CCFiniteTimeAction*  action2 = CCSequence::create
	(
        CCScaleBy  ::create ( 2 , 2 ),
        CCFadeOut  ::create ( 2 ),
        CCCallFuncN::create ( this, callfuncN_selector ( ActionCallFunc::onCallback2 ) ), 
        KD_NULL
	);

    CCFiniteTimeAction*  action3 = CCSequence::create
	(
        CCRotateBy  ::create ( 3, 360 ),
        CCFadeOut   ::create ( 2 ),
        CCCallFuncND::create ( this, callfuncND_selector ( ActionCallFunc::onCallback3 ), (KDvoid*) 0xbebabeba ), 
        KD_NULL
	);

    m_pGrossini->runAction ( action );
    m_pTamara->runAction ( action2 );
    m_pKathia->runAction ( action3 );

	return KD_TRUE;
}

KDvoid ActionCallFunc::onCallback1 ( KDvoid )
{
    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF* label = CCLabelTTF::create ( "onCallback1 called", "fonts/Marker Felt.ttf", 16 );
    label->setPosition ( CCPointMake ( s.cx / 2, s.cy / 2 + 30 ) );
    addChild ( label );
}

KDvoid ActionCallFunc::onCallback2 ( CCNode* pSender )
{
    const CCSize&  s = this->getContentSize ( );

	CCLabelTTF*  label = CCLabelTTF::create ( "onCallback2 called", "fonts/Marker Felt.ttf", 16 );
	label->setPosition ( ccpMid ( s ) );
	this->addChild ( label );
}

KDvoid ActionCallFunc::onCallback3 ( CCNode* pTarget, KDvoid* data )
{
    const CCSize&  s = this->getContentSize ( );

    CCLabelTTF*  label = CCLabelTTF::create ( "onCallback3 called", "fonts/Marker Felt.ttf", 16 );
	label->setPosition ( ccp ( s.cx / 2, s.cy / 2 - 30 ) );
	this->addChild ( label );
}

const KDchar* ActionCallFunc::subtitle ( KDvoid )
{
    return "Callbacks: CallFunc and friends";
}

//------------------------------------------------------------------
//
// ActionCallFuncND
//
//------------------------------------------------------------------
KDbool ActionCallFuncND::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 1 );

    CCFiniteTimeAction*  action = CCSequence::create
	(
		CCMoveBy    ::create ( 2.0f, ccp ( 200, 0 ) ),
		CCCallFuncND::create ( m_pGrossini, callfuncND_selector ( ActionCallFuncND::onCallback ), (KDvoid*) KD_TRUE ),
		KD_NULL
	);

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

const KDchar* ActionCallFuncND::title ( KDvoid )
{
    return "CallFuncND + auto remove";
}

const KDchar* ActionCallFuncND::subtitle ( KDvoid )
{
    return "Grossini dissapears in 2s";
}

KDvoid ActionCallFuncND::onCallback ( CCNode* pTarget, KDvoid* pData )
{		
	KDbool  bCleanUp = pData != KD_NULL;
	pTarget->removeFromParentAndCleanup ( bCleanUp );
}

//------------------------------------------------------------------
//
// ActionSpawn
//
//------------------------------------------------------------------
KDbool ActionSpawn::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    alignSpritesLeft ( 1 );

    CCAction*  action = CCSpawn::create
	(
        CCJumpBy  ::create ( 2, ccp ( 300, 0 ), 50, 4 ),
        CCRotateBy::create ( 2, 720 ),
		KD_NULL
	);

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

const KDchar* ActionSpawn::subtitle ( KDvoid )
{
    return "Spawn: Jump + Rotate";
}

//------------------------------------------------------------------
//
// ActionRepeatForever
//
//------------------------------------------------------------------
KDbool ActionRepeatForever::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 1 );

    CCFiniteTimeAction*  action = CCSequence::create
	(
        CCDelayTime::create ( 1 ),
        CCCallFuncN::create ( this, callfuncN_selector ( ActionRepeatForever::onRepeatForever ) ), 
        KD_NULL
	);

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

KDvoid ActionRepeatForever::onRepeatForever ( CCNode* pSender )
{
    CCRepeatForever*  repeat = CCRepeatForever::create ( CCRotateBy::create ( 1.0f, 360 ) );

    pSender->runAction ( repeat );
}

const KDchar* ActionRepeatForever::subtitle ( KDvoid )
{
    return "CallFuncN + RepeatForever";
}

//------------------------------------------------------------------
//
// ActionRotateToRepeat
//
//------------------------------------------------------------------
KDbool ActionRotateToRepeat::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 2 );

    CCActionInterval*  act1 = CCRotateTo::create ( 1, 90 );
    CCActionInterval*  act2 = CCRotateTo::create ( 1,  0 );
    CCActionInterval*  seq  = (CCActionInterval*) ( CCSequence::create ( act1, act2, KD_NULL ) );
    CCAction*          rep1 = CCRepeatForever::create ( seq );
    CCActionInterval*  rep2 = CCRepeat::create ( (CCFiniteTimeAction*) ( seq->copy ( )->autorelease ( ) ), 10 );

    m_pTamara->runAction ( rep1 );
    m_pKathia->runAction ( rep2 );

	return KD_TRUE;
}

const KDchar* ActionRotateToRepeat ::subtitle ( KDvoid )
{
    return "Repeat/RepeatForever + RotateTo";
}

//------------------------------------------------------------------
//
// ActionRotateJerk
//
//------------------------------------------------------------------
KDbool ActionRotateJerk::init ( KDvoid )
{
 	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 2 );

    CCFiniteTimeAction*  seq = CCSequence::create 
	(
        CCRotateTo::create ( 0.5f, -20 ),
        CCRotateTo::create ( 0.5f,  20 ),
		KD_NULL
	);

    CCActionInterval*  rep1 = CCRepeat::create ( seq, 10 );
    CCAction*   rep2 = CCRepeatForever::create ( (CCActionInterval*) ( seq->copy ( )->autorelease ( ) ) );

    m_pTamara->runAction ( rep1 );
    m_pKathia->runAction ( rep2 );

	return KD_TRUE;
}

const KDchar* ActionRotateJerk::subtitle ( KDvoid )
{
    return "RepeatForever / Repeat + Rotate";
}

//------------------------------------------------------------------
//
// ActionReverse
//
//------------------------------------------------------------------
KDbool ActionReverse::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    alignSpritesLeft ( 1 );

    CCActionInterval*      jump = CCJumpBy ::create ( 2, ccp ( 300, 0 ), 50, 4 );
    CCFiniteTimeAction*  action = CCSequence::create ( jump, jump->reverse ( ), KD_NULL );

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

const KDchar* ActionReverse::subtitle ( KDvoid )
{
    return "Reverse an action";
}


//------------------------------------------------------------------
//
// ActionDelayTime
//
//------------------------------------------------------------------
KDbool ActionDelayTime::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    alignSpritesLeft ( 1 );

    CCActionInterval*      move = CCMoveBy  ::create ( 1, ccp ( 150, 0 ) );
    CCFiniteTimeAction*  action = CCSequence::create ( move, CCDelayTime::create ( 2 ), move, KD_NULL );

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

const KDchar* ActionDelayTime::subtitle ( KDvoid )
{
    return "DelayTime: move + delay + move";
}

//------------------------------------------------------------------
//
// ActionReverseSequence
//
//------------------------------------------------------------------
KDbool ActionReverseSequence::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    alignSpritesLeft ( 1 );

    CCActionInterval*     move1 = CCMoveBy::create ( 1, ccp ( 250,  0 ) );
    CCActionInterval*     move2 = CCMoveBy::create ( 1, ccp (   0, 50 ) );
    CCFiniteTimeAction*     seq = CCSequence::create ( move1, move2, move1->reverse ( ), KD_NULL );
    CCFiniteTimeAction*  action = CCSequence::create ( seq, seq->reverse ( ), KD_NULL );

    m_pGrossini->runAction ( action );

	return KD_TRUE;
}

const KDchar* ActionReverseSequence::subtitle ( KDvoid )
{
    return "Reverse a sequence";
}

//------------------------------------------------------------------
//
// ActionReverseSequence2
//
//------------------------------------------------------------------
KDbool ActionReverseSequence2::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    alignSpritesLeft ( 2 );

    // Test:
    //   Sequence should work both with IntervalAction and InstantActions
    CCActionInterval*   move1 = CCMoveBy::create ( 1, ccp ( 250,  0 ) );
    CCActionInterval*   move2 = CCMoveBy::create ( 1, ccp (   0, 50 ) );
    CCToggleVisibility*  tog1 = new CCToggleVisibility ( );
    CCToggleVisibility*  tog2 = new CCToggleVisibility ( );
    tog1->autorelease ( );
    tog2->autorelease ( );
    CCFiniteTimeAction*   seq = CCSequence::create ( move1, tog1, move2, tog2, move1->reverse ( ), KD_NULL );
    CCActionInterval*  action = CCRepeat::create ( (CCActionInterval*) ( CCSequence::create ( seq, seq->reverse ( ), KD_NULL ) ), 3 );

    // Test:
    //   Also test that the reverse of Hide is Show, and vice-versa
    m_pKathia->runAction ( action );

    CCActionInterval*  move_tamara  = CCMoveBy::create ( 1, CCPointMake ( 100, 0 ) );
    CCActionInterval*  move_tamara2 = CCMoveBy::create ( 1, CCPointMake (  50, 0 ) );
    CCActionInstant*   hide = new CCHide ( );
    hide->autorelease ( );
    CCFiniteTimeAction*  seq_tamara = CCSequence::create ( move_tamara, hide, move_tamara2, KD_NULL );
    CCFiniteTimeAction*  seq_back = seq_tamara->reverse ( );
    m_pTamara->runAction ( CCSequence::create ( seq_tamara, seq_back, KD_NULL ) );

	return KD_TRUE;
}

const KDchar* ActionReverseSequence2::subtitle ( KDvoid )
{
    return "Reverse sequence 2";
}

//------------------------------------------------------------------
//
// ActionRepeat
//
//------------------------------------------------------------------
KDbool ActionRepeat::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    alignSpritesLeft ( 2 );

    CCActionInterval*  a1 = CCMoveBy::create ( 1, CCPointMake ( 150, 0 ) );

    CCActionInterval*  action1 = CCRepeat::create
	(
		CCSequence::create ( CCPlace::create ( m_pKathia->getPosition ( ) ), a1, KD_NULL ), 3
	);

    CCActionInterval*  action2 = CCRepeatForever::create
	(
        CCSequence::create ( (CCActionInterval*) a1->copy ( )->autorelease ( ), a1->reverse ( ), KD_NULL )
    );

    m_pKathia->runAction ( action1 );
    m_pTamara->runAction ( action2 );

	return KD_TRUE;
}

const KDchar* ActionRepeat::subtitle ( KDvoid )
{
    return "Repeat / RepeatForever actions";
}

//------------------------------------------------------------------
//
// ActionOrbit
//
//------------------------------------------------------------------
KDbool ActionOrbit::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 3 );

    CCActionInterval*     orbit1 = CCOrbitCamera::create ( 2,1, 0, 0, 180, 0, 0 );
    CCFiniteTimeAction*  action1 = CCSequence   ::create ( orbit1, orbit1->reverse ( ), KD_NULL );

    CCActionInterval*     orbit2 = CCOrbitCamera::create ( 2,1, 0, 0, 180, -45, 0 );
    CCFiniteTimeAction*  action2 = CCSequence   ::create ( orbit2, orbit2->reverse ( ), KD_NULL );

    CCActionInterval*     orbit3 = CCOrbitCamera::create ( 2,1, 0, 0, 180, 90, 0 );
    CCFiniteTimeAction*  action3 = CCSequence   ::create ( orbit3, orbit3->reverse ( ), KD_NULL );

    m_pKathia->runAction ( CCRepeatForever::create ( (CCActionInterval*) action1 ) );
    m_pTamara->runAction ( CCRepeatForever::create ( (CCActionInterval*) action2 ) );
    m_pGrossini->runAction ( CCRepeatForever::create ( (CCActionInterval*) action3 ) );

    CCActionInterval*    move = CCMoveBy::create ( 3, CCPointMake ( 100, -100) );
    CCFiniteTimeAction*   seq = CCSequence::create ( move, move->reverse ( ), KD_NULL );
    CCAction*             rfe = CCRepeatForever::create ( (CCActionInterval*) seq );

    m_pKathia->runAction ( rfe );
    m_pTamara->runAction ( (CCAction*) ( rfe->copy ( )->autorelease ( ) ) );
    m_pGrossini->runAction ( (CCAction*) ( rfe->copy ( )->autorelease ( ) ) );

	return KD_TRUE;
}

const KDchar* ActionOrbit::subtitle ( KDvoid )
{
    return "OrbitCamera action";
}

//------------------------------------------------------------------
//
// ActionFollow
//
//------------------------------------------------------------------
KDbool ActionFollow::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 1 );

	CCSize   tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
    CCSize   tLyrSize = this->getContentSize ( );

    m_pGrossini->setPosition ( ccp ( -200, tLyrSize.cy / 2 ) );

    CCActionInterval*	move = CCMoveBy::create ( 2, ccp ( tLyrSize.cx * 3, 0 ) );
    CCSequence*			seq  = CCSequence::create ( move, move->reverse ( ), KD_NULL );
    CCAction*			rep  = CCRepeatForever::create ( seq );

    m_pGrossini->runAction ( rep );

    this->runAction ( CCFollow::create ( m_pGrossini, CCRectMake ( 0, 0, tWinSize.cx * 2 - 100, tWinSize.cy ) ) );

	return KD_TRUE;
}

KDvoid ActionFollow::draw ( KDvoid )
{
	CCSize  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
    
	KDfloat  x = tWinSize.cx * 2 - 100;
	KDfloat  y = tWinSize.cy;
    
	CCPoint  aVertices[] = 
	{
		ccp (     5,     5 ), 
		ccp ( x - 5,     5 ), 
		ccp ( x - 5, y - 5 ),
		ccp (     5, y - 5 ) 
	};
	ccDrawPoly ( aVertices, 4, KD_TRUE );
}

const KDchar* ActionFollow::subtitle ( KDvoid )
{
    return "Follow action";
}


//------------------------------------------------------------------
//
// ActionTargeted
//
//------------------------------------------------------------------
KDbool ActionTargeted::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    centerSprites ( 2 );

    CCJumpBy*   pJump1 = CCJumpBy::create ( 2, CCPointZero, 100, 3 );
    CCJumpBy*   pJump2 = (CCJumpBy*) pJump1->copy()->autorelease ( );
    CCRotateBy*  pRot1 = CCRotateBy::create ( 1, 360 );
    CCRotateBy*  pRot2 = (CCRotateBy*) pRot1->copy ( )->autorelease ( );

    CCTargetedAction*  pT1 = CCTargetedAction::create ( m_pTamara, pJump2 );
    CCTargetedAction*  pT2 = CCTargetedAction::create ( m_pTamara, pRot2 );

    CCSequence*       pSeq = (CCSequence*) CCSequence::create ( pJump1, pT1, pRot1, pT2, KD_NULL );
    CCRepeatForever*  pAlways = CCRepeatForever::create ( pSeq );

    m_pKathia->runAction ( pAlways );

	return KD_TRUE;
}

const KDchar* ActionTargeted::title ( KDvoid )
{
    return "ActionTargeted";
}

const KDchar* ActionTargeted::subtitle ( KDvoid )
{
    return "Action that runs on another target. Useful for sequences";
}


//------------------------------------------------------------------
//
// ActionStacked
//
//------------------------------------------------------------------

KDvoid ActionStacked::onEnter ( KDvoid )
{
    TestActions::onEnter ( );
        
    this->centerSprites ( 0 );
    
    this->setTouchEnabled ( KD_TRUE );
    
	const CCSize&  s = this->getContentSize ( );
    this->addNewSpriteWithCoords ( ccpMid ( s ) );
}

KDvoid ActionStacked::addNewSpriteWithCoords ( CCPoint p )
{
    KDint  idx = (KDint) ( CCRANDOM_0_1 ( ) * 1400 / 100 );
    KDint  x = ( idx % 5 ) * 85;
    KDint  y = ( idx / 5 ) * 121;
    
    
    CCSprite*  pSprite = CCSprite::create ( "Images/grossini_dance_atlas.png", CCRectMake ( x, y, 85, 121 ) );
    
    pSprite->setPosition ( p );
    this->addChild ( pSprite );
    
    this->runActionsInSprite ( pSprite );
}

KDvoid ActionStacked::runActionsInSprite ( CCSprite* pSprite )
{
    // override me
}

KDvoid ActionStacked::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ )
    {
        CCTouch*  pTouch = (CCTouch*) ( *it );

        if ( !pTouch )
		{
            break;
		}

		addNewSpriteWithCoords ( this->convertTouchToNodeSpace ( pTouch ) );
    }
}

const KDchar* ActionStacked::title ( KDvoid )
{
    return "Override me";
}

const KDchar* ActionStacked::subtitle ( KDvoid )
{
    return "Tap screen";
}

//------------------------------------------------------------------
//
// ActionMoveStacked
//
//------------------------------------------------------------------

KDvoid ActionMoveStacked::runActionsInSprite ( CCSprite* pSprite )
{
    pSprite->runAction
	(
        CCRepeatForever::create
		(
			CCSequence::create
			(
                CCMoveBy::create ( 0.05f, ccp (  10,  10 ) ),
                CCMoveBy::create ( 0.05f, ccp ( -10, -10 ) ),
				NULL
			)
		)
	);
    
    CCMoveBy*  action = CCMoveBy::create ( 2, ccp ( 400, 0 ) );
    CCMoveBy*  action_back = (CCMoveBy*) action->reverse ( );
    
    pSprite->runAction
	(
		CCRepeatForever::create 
		(
			CCSequence::create ( action, action_back, KD_NULL )
		)
	);
}


const KDchar* ActionMoveStacked::title ( KDvoid )
{
    return "Stacked CCMoveBy/To actions";
}

//------------------------------------------------------------------
//
// ActionMoveJumpStacked
//
//------------------------------------------------------------------

KDvoid ActionMoveJumpStacked::runActionsInSprite ( CCSprite* pSprite )
{
    pSprite->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCMoveBy::create ( 0.05f, ccp (  10,  2 ) ),
				CCMoveBy::create ( 0.05f, ccp ( -10, -2 ) ),
				KD_NULL
			)
		)
	);
    
    CCJumpBy*  jump = CCJumpBy::create ( 2, ccp ( 400, 0 ), 100, 5 );
    CCJumpBy*  jump_back = (CCJumpBy*) jump->reverse ( );
    
    pSprite->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create ( jump, jump_back, KD_NULL )
		)
	);
}

const KDchar* ActionMoveJumpStacked::title ( KDvoid )
{
    return "tacked Move + Jump actions";
}

//------------------------------------------------------------------
//
// ActionMoveBezierStacked
//
//------------------------------------------------------------------

KDvoid ActionMoveBezierStacked::runActionsInSprite ( CCSprite* pSprite )
{
    const CCSize&  s = this->getContentSize ( );
    
    // sprite 1
    ccBezierConfig   tBezier;
    tBezier.tControlPoint1 = ccp ( 0, s.cx / 2 );
    tBezier.tControlPoint2 = ccp ( 300, -s.cy / 2 );
    tBezier.tEndPosition   = ccp ( 300,100);
    
    CCBezierBy*   bezierForward = CCBezierBy::create ( 3, tBezier );
    CCBezierBy*   bezierBack = (CCBezierBy*) bezierForward->reverse ( );
    CCSequence*   seq = CCSequence::create ( bezierForward, bezierBack, KD_NULL );
    CCRepeatForever*  rep = CCRepeatForever::create ( seq );
    pSprite->runAction ( rep );
    
    pSprite->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCMoveBy::create ( 0.05f, ccp (  10, 0 ) ),
				CCMoveBy::create ( 0.05f, ccp ( -10, 0 ) ),
				KD_NULL
			)
		)
	);
}

const KDchar* ActionMoveBezierStacked::title ( KDvoid )
{
    return "Stacked Move + Bezier actions";
}

//------------------------------------------------------------------
//
// ActionCatmullRomStacked
//
//------------------------------------------------------------------

KDvoid ActionCatmullRomStacked::onEnter ( KDvoid )
{
    TestActions::onEnter ( );
    
    this->centerSprites ( 2 );
    
	const CCSize  s = this->getContentSize ( );
    
    //
    // sprite 1 (By)
    //
    // startPosition can be any coordinate, but since the movement
    // is relative to the Catmull Rom curve, it is better to start with (0,0).
    //
    
    m_pTamara->setPosition ( ccp ( 50, 50 ) );
    
    CCPointArray*  pArray = CCPointArray::create(20);
    
    pArray->addControlPoint ( ccp ( 0, 0 ) );
    pArray->addControlPoint ( ccp ( 80, 80 ) );
    pArray->addControlPoint ( ccp ( s.cx - 80, 80 ) );
    pArray->addControlPoint ( ccp ( s.cx - 80, s.cy - 80 ) );
    pArray->addControlPoint ( ccp ( 80,s.cy - 80 ) );
    pArray->addControlPoint ( ccp ( 80,80 ) );
    pArray->addControlPoint ( ccp ( s.cx / 2, s.cy / 2 ) );
    
    CCCatmullRomBy *action = CCCatmullRomBy::create ( 3, pArray );
    CCCatmullRomBy* reverse = (CCCatmullRomBy*) action->reverse ( );
    
    CCSequence*  seq = CCSequence::create ( action, reverse, KD_NULL );
    
    m_pTamara->runAction ( seq );       
    m_pTamara->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create 
			(
				CCMoveBy::create ( 0.05f, ccp (  10, 0 ) ),
				CCMoveBy::create ( 0.05f, ccp ( -10, 0 ) ),
				KD_NULL
			)
		)
	);
    
    
    //
    // sprite 2 (To)
    //
    // The startPosition is not important here, because it uses a "To" action.
    // The initial position will be the 1st point of the Catmull Rom path
    //
    
    CCPointArray*  pArray2 = CCPointArray::create ( 20 );
    
    pArray2->addControlPoint ( ccp ( s.cx / 2, 30 ) );
    pArray2->addControlPoint ( ccp ( s.cx - 80,30 ) );
    pArray2->addControlPoint ( ccp ( s.cx - 80,s.cy - 80 ) );
    pArray2->addControlPoint ( ccp ( s.cx / 2, s.cy - 80 ) );
    pArray2->addControlPoint ( ccp ( s.cx / 2, 30 ) );
    
    
    CCCatmullRomTo*  action2 = CCCatmullRomTo::create ( 3, pArray2 );
    CCCatmullRomTo*  reverse2 = (CCCatmullRomTo*) action2->reverse ( );
    
    CCSequence*  seq2 = CCSequence::create ( action2, reverse2, KD_NULL );
    
    m_pKathia->runAction ( seq2 );
    
    
    m_pKathia->runAction 
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCMoveBy::create ( 0.05f, ccp (  10, 0 ) ),
				CCMoveBy::create ( 0.05f, ccp ( -10, 0 ) ),
				KD_NULL
			)
		)
	);
    
    
    pArray->retain ( );
    m_pArray1 = pArray;
    pArray2->retain ( );
    m_pArray2 = pArray2;
}

ActionCatmullRomStacked::~ActionCatmullRomStacked ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pArray1 );
    CC_SAFE_RELEASE ( m_pArray2 );
}

KDvoid ActionCatmullRomStacked::draw ( KDvoid )
{
    TestActions::draw ( );
    
    // move to 50,50 since the "by" path will start at 50,50
    kmGLPushMatrix ( );
    kmGLTranslatef ( 50, 50, 0 );
    ccDrawCatmullRom ( m_pArray1, 50 );
    kmGLPopMatrix ( );
    
    ccDrawCatmullRom ( m_pArray2, 50 );
}

const KDchar* ActionCatmullRomStacked::title ( KDvoid )
{
    return "Stacked MoveBy + CatmullRom actions";
}

const KDchar* ActionCatmullRomStacked::subtitle ( KDvoid )
{
    return "MoveBy + CatmullRom at the same time in the same sprite";
}
 

//------------------------------------------------------------------
//
// ActionCardinalSplineStacked
//
//------------------------------------------------------------------

KDvoid ActionCardinalSplineStacked::onEnter ( KDvoid )
{
    TestActions::onEnter ( );
    
    this->centerSprites ( 2 );
    
	const CCSize&  s = this->getContentSize ( );
    
    CCPointArray*  pArray = CCPointArray::create ( 20 );
    
    pArray->addControlPoint ( ccp ( 0, 0 ) );
    pArray->addControlPoint ( ccp ( s.cx / 2-30, 0 ) );
    pArray->addControlPoint ( ccp ( s.cx / 2-30, s.cy - 80 ) );
    pArray->addControlPoint ( ccp ( 0, s.cy - 80 ) );
    pArray->addControlPoint ( ccp ( 0, 0 ) );
    
    
    //
    // sprite 1 (By)
    //
    // Spline with no tension (tension==0)
    //
    
    
    CCCatmullRomBy*  action = (CCCatmullRomBy*) CCCardinalSplineBy::create ( 3, pArray, 0 );
    CCCatmullRomBy*  reverse = (CCCatmullRomBy*) action->reverse ( );
    
    CCSequence*  seq = CCSequence::create ( action, reverse, KD_NULL );
    
    m_pTamara->setPosition ( ccp ( 50, 50 ) );
    m_pTamara->runAction ( seq );
    
    m_pTamara->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCMoveBy::create ( 0.05f, ccp (  10, 0 ) ),
				CCMoveBy::create ( 0.05f, ccp ( -10, 0 ) ),
				KD_NULL
			)
		)
	);
    
    
    //
    // sprite 2 (By)
    //
    // Spline with high tension (tension==1)
    //
    
    CCCatmullRomBy*  pAction2 = (CCCatmullRomBy*) CCCardinalSplineBy::create ( 3, pArray, 1 );
    CCCatmullRomBy*  pReverse2 = (CCCatmullRomBy*) pAction2->reverse ( );
    
    CCSequence*  pSeq2 = CCSequence::create ( pAction2, pReverse2, KD_NULL );
    
    m_pKathia->setPosition ( ccp ( s.cx / 2, 50 ) );
    
    m_pKathia->runAction ( pSeq2 );
    
    m_pKathia->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCMoveBy::create ( 0.05f, ccp (  10, 0 ) ),
				CCMoveBy::create ( 0.05f, ccp ( -10, 0 ) ),
				KD_NULL
			)
		)
	);
    
    
    pArray->retain ( );
    m_pArray = pArray;
}

ActionCardinalSplineStacked::~ActionCardinalSplineStacked ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pArray );
}

KDvoid ActionCardinalSplineStacked::draw ( KDvoid )
{
    TestActions::draw ( );
    
    // move to 50,50 since the "by" path will start at 50,50
    kmGLPushMatrix ( );
    kmGLTranslatef ( 50, 50, 0 );
    ccDrawCardinalSpline ( m_pArray, 0, 100 );
    kmGLPopMatrix ( );
    
	const CCSize&  s = this->getContentSize ( );
    
    kmGLPushMatrix ( );
    kmGLTranslatef ( s.cx / 2, 50, 0 );
    ccDrawCardinalSpline ( m_pArray, 1, 100 );
    kmGLPopMatrix ( );
}

const KDchar* ActionCardinalSplineStacked::title ( KDvoid )
{
    return "Stacked MoveBy + CardinalSpline actions";
}

const KDchar* ActionCardinalSplineStacked::subtitle ( KDvoid )
{
    return "CCMoveBy + CCCardinalSplineBy/To at the same time";
}

//------------------------------------------------------------------
//
// ActionCatmullRom
//
//------------------------------------------------------------------
ActionCatmullRom::~ActionCatmullRom ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pArray1 );
	CC_SAFE_RELEASE ( m_pArray2 );
}

KDbool ActionCatmullRom::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    this->centerSprites ( 2 );
    
    const CCSize&   s = this->getContentSize ( );
    
	//
	// sprite 1 (By)
	//
	// startPosition can be any coordinate, but since the movement
	// is relative to the Catmull Rom curve, it is better to start with (0,0).
	//
	
    m_pTamara->setPosition ( ccp ( 50, 50 ) );
	
    CCPointArray*  pArray = CCPointArray::create ( 20 );
    
    pArray->addControlPoint ( ccp (         0,         0 ) );
    pArray->addControlPoint ( ccp (        80,        80 ) );
    pArray->addControlPoint ( ccp ( s.cx - 80,        80 ) );
    pArray->addControlPoint ( ccp ( s.cx - 80, s.cy - 80 ) );
    pArray->addControlPoint ( ccp (        80, s.cy - 80 ) );
    pArray->addControlPoint ( ccp (        80,        80 ) );
    pArray->addControlPoint ( ccp ( s.cx /  2, s.cy /  2 ) );
    
    CCCatmullRomBy*  pAction = CCCatmullRomBy::create ( 3, pArray );	
    m_pTamara->runAction ( CCSequence::create ( pAction, pAction->reverse ( ), KD_NULL ) );
	
	//
	// sprite 2 (To)
	//
	// The startPosition is not important here, because it uses a "To" action.
	// The initial position will be the 1st point of the Catmull Rom path
	//    
    
    CCPointArray*  pArray2 = CCPointArray::create ( 20 );
    
    pArray2->addControlPoint ( ccp ( s.cx /  2,        30 ) );
    pArray2->addControlPoint ( ccp ( s.cx - 80,        30 ) );
    pArray2->addControlPoint ( ccp ( s.cx - 80, s.cy - 80 ) );
    pArray2->addControlPoint ( ccp ( s.cx /  2, s.cy - 80 ) );
    pArray2->addControlPoint ( ccp ( s.cx /  2,        30 ) );
    
    CCCatmullRomTo*  pAction2 = CCCatmullRomTo::create ( 3, pArray2 );
	m_pKathia->runAction ( CCSequence::create ( pAction2, pAction2->reverse ( ), KD_NULL ) );
    
    m_pArray1 = pArray;
    m_pArray1->retain ( );
    m_pArray2 = pArray2;
    m_pArray2->retain ( );

	return KD_TRUE;
}

const KDchar* ActionCatmullRom::title ( KDvoid )
{
    return "CatmullRomBy / CatmullRomTo";
}

const KDchar* ActionCatmullRom::subtitle ( KDvoid )
{
    return "Catmull Rom spline paths. Testing reverse too";
}

KDvoid ActionCatmullRom::draw ( KDvoid )
{
    TestActions::draw ( );
	
	// move to 50,50 since the "by" path will start at 50,50
	kmGLPushMatrix ( );
	kmGLTranslatef ( 50, 50, 0 );
	ccDrawCatmullRom ( m_pArray1, 50 );
	kmGLPopMatrix  ( );
    
	ccDrawCatmullRom ( m_pArray2, 50 );
}

//------------------------------------------------------------------
//
// ActionCardinalSpline
//
//------------------------------------------------------------------
ActionCardinalSpline::~ActionCardinalSpline ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pArray );
}

KDbool ActionCardinalSpline::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

    this->centerSprites ( 2 );
	
    const CCSize&  s = this->getContentSize ( );
    
    CCPointArray*  pArray = CCPointArray::create ( 20 );
    
    pArray->addControlPoint ( ccp (             0,         0 ) );
    pArray->addControlPoint ( ccp ( s.cx / 2 - 30,         0 ) );
    pArray->addControlPoint ( ccp ( s.cx / 2 - 30, s.cy - 80 ) );
    pArray->addControlPoint ( ccp (             0, s.cy - 80 ) );
    pArray->addControlPoint ( ccp (             0,         0 ) );
    
	//
	// sprite 1 (By)
	//
	// Spline with no tension (tension==0)
	//
    
    CCCardinalSplineBy*  pAction = CCCardinalSplineBy::create ( 3, pArray, 0 );	
	m_pTamara->runAction( CCSequence::create ( pAction, pAction->reverse ( ), KD_NULL ) );
    m_pTamara->setPosition ( ccp ( 50, 50 ) );
    
	
	//
	// sprite 2 (By)
	//
	// Spline with high tension (tension==1)
	//
    
    CCCardinalSplineBy*  pAction2 = CCCardinalSplineBy::create ( 3, pArray, 1 );
	m_pKathia->runAction ( CCSequence::create ( pAction2, pAction2->reverse ( ), KD_NULL ) );
    m_pKathia->setPosition ( ccp ( s.cx / 2, 50 ) );
    
	
    m_pArray = pArray;
    pArray->retain ( );

	return KD_TRUE;
}

const KDchar* ActionCardinalSpline::title ( KDvoid )
{
    return "CardinalSplineBy / CardinalSplineAt";
}

const KDchar* ActionCardinalSpline::subtitle ( KDvoid )
{
    return "Cardinal Spline paths. Testing different tensions for one array";
}

KDvoid ActionCardinalSpline::draw ( KDvoid )
{
    TestActions::draw ( );
	
	// move to 50,50 since the "by" path will start at 50,50
	kmGLPushMatrix ( );
	kmGLTranslatef ( 50, 50, 0 );
	ccDrawCardinalSpline ( m_pArray, 0, 100 );
	kmGLPopMatrix  ( );
    
    const CCSize&  s = this->getContentSize ( );
    
	kmGLPushMatrix ( );
	kmGLTranslatef ( s.cx / 2, 50, 0 );
	ccDrawCardinalSpline ( m_pArray, 1, 100 );
	kmGLPopMatrix  ( );
}

//------------------------------------------------------------------
//
// PauseResumeActions
//
//------------------------------------------------------------------
PauseResumeActions::PauseResumeActions ( KDvoid )
{
	m_pPausedTargets = KD_NULL;
}

PauseResumeActions::~PauseResumeActions ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pPausedTargets );
}

KDbool PauseResumeActions::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	centerSprites ( 3 );

    m_pTamara->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 3,  360 ) ) );
    m_pGrossini->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 3, -360 ) ) );
    m_pKathia->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 3,  360 ) ) );
    
    this->schedule ( schedule_selector ( PauseResumeActions::pause  ), 3, KD_FALSE, 0 );
    this->schedule ( schedule_selector ( PauseResumeActions::resume ), 5, KD_FALSE, 0 );

	return KD_TRUE;
}

const KDchar* PauseResumeActions::title ( KDvoid )
{
    return "PauseResumeActions";
}

const KDchar* PauseResumeActions::subtitle ( KDvoid )
{
    return "All actions pause at 3s and resume at 5s";
}

KDvoid PauseResumeActions::pause  ( KDfloat fDelta )
{
    CCDirector*  pDirector = CCDirector::sharedDirector ( );

    CC_SAFE_RELEASE ( m_pPausedTargets );
    m_pPausedTargets = pDirector->getActionManager ( )->pauseAllRunningActions ( );
    CC_SAFE_RETAIN  ( m_pPausedTargets );
}

KDvoid PauseResumeActions::resume ( KDfloat fDelta )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	pDirector->getActionManager ( )->resumeTargets ( m_pPausedTargets );
}


//------------------------------------------------------------------
//
// ActionRemoveSelf
//
//------------------------------------------------------------------

KDbool ActionRemoveSelf::init ( KDvoid )
{
	if ( !TestActions::init ( ) )
	{
		return KD_FALSE;
	}

	alignSpritesLeft ( 1 );

	CCFiniteTimeAction*  pAction = CCSequence::create
	(
		CCMoveBy::create ( 2, ccp ( 240, 0 ) ),
		CCRotateBy::create ( 2,  540 ),
		CCScaleTo::create ( 1, 0.1f ),
		CCRemoveSelf::create ( ),
		KD_NULL
	);

	m_pGrossini->runAction ( pAction );

	return KD_TRUE;
}

const KDchar* ActionRemoveSelf::subtitle ( KDvoid )
{
	return "Sequence: Move + Rotate + Scale + RemoveSelf";
}
