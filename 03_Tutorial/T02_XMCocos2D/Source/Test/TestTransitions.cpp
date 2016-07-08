/* --------------------------------------------------------------------------
 *
 *      File            TestTransitions.cpp
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
#include "TestTransitions.h"

#define TRANSITION_DURATION        ( 0.7f )

class FadeWhiteTransition : public CCTransitionFade 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionFade::create ( t, s, ccWHITE ); 
		}
};

class FlipXLeftOver : public CCTransitionFlipX 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionFlipX::create ( t, s, kCCTransitionOrientationLeftOver );
		}
};

class FlipXRightOver : public CCTransitionFlipX 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionFlipX::create ( t, s, kCCTransitionOrientationRightOver );
		}
};

class FlipYUpOver : public CCTransitionFlipY 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionFlipY::create ( t, s, kCCTransitionOrientationUpOver ); 
		}
};

class FlipYDownOver : public CCTransitionFlipY 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionFlipY::create ( t, s, kCCTransitionOrientationDownOver ); 
		}
};

class FlipAngularLeftOver : public CCTransitionFlipAngular 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionFlipAngular::create ( t, s, kCCTransitionOrientationLeftOver ); 
		}
};

class FlipAngularRightOver : public CCTransitionFlipAngular 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionFlipAngular::create ( t, s, kCCTransitionOrientationRightOver );
		}
};

class ZoomFlipXLeftOver : public CCTransitionZoomFlipX 
{
	public : 

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionZoomFlipX::create ( t, s, kCCTransitionOrientationLeftOver ); 
		}
};

class ZoomFlipXRightOver : public CCTransitionZoomFlipX 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionZoomFlipX::create ( t, s, kCCTransitionOrientationRightOver );
		}
};

class ZoomFlipYUpOver : public CCTransitionZoomFlipY 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionZoomFlipY::create ( t, s, kCCTransitionOrientationUpOver ); 
		}
};

class ZoomFlipYDownOver : public CCTransitionZoomFlipY 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionZoomFlipY::create ( t, s, kCCTransitionOrientationDownOver ); 
		}
};

class ZoomFlipAngularLeftOver : public CCTransitionZoomFlipAngular 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionZoomFlipAngular::create ( t, s, kCCTransitionOrientationLeftOver ); 
		}
};

class ZoomFlipAngularRightOver : public CCTransitionZoomFlipAngular 
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			return CCTransitionZoomFlipAngular::create ( t, s, kCCTransitionOrientationRightOver );
		}
};

class PageTransitionForward : public CCTransitionPageTurn
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			CCDirector::sharedDirector ( )->setDepthTest ( KD_TRUE );

			return CCTransitionPageTurn::create ( t, s, KD_FALSE );
		}
};

class PageTransitionBackward : public CCTransitionPageTurn
{
	public :

		static CCTransitionScene* create ( KDfloat t, CCScene* s )
		{
			CCDirector::sharedDirector ( )->setDepthTest ( KD_TRUE );

			return CCTransitionPageTurn::create ( t, s, KD_TRUE );
		}
};

#define MAX_LAYER     41

static const KDchar*  l_szSubTitleNames [ ] = 
{
    "CCTransitionJumpZoom",

    "CCTransitionProgressRadialCCW",
    "CCTransitionProgressRadialCW",
    "CCTransitionProgressHorizontal",
    "CCTransitionProgressVertical",
    "CCTransitionProgressInOut",
    "CCTransitionProgressOutIn",

    "CCTransitionCrossFade",
    "TransitionPageForward",
    "TransitionPageBackward",
    "CCTransitionFadeTR",
    "CCTransitionFadeBL",
    "CCTransitionFadeUp",
    "CCTransitionFadeDown",
    "CCTransitionTurnOffTiles",
    "CCTransitionSplitRows",
    "CCTransitionSplitCols",

    "CCTransitionFade",
    "FadeWhiteTransition",

    "FlipXLeftOver",
    "FlipXRightOver",
    "FlipYUpOver",
    "FlipYDownOver",
    "FlipAngularLeftOver",
    "FlipAngularRightOver",

    "ZoomFlipXLeftOver",
    "ZoomFlipXRightOver",
    "ZoomFlipYUpOver",
    "ZoomFlipYDownOver",
    "ZoomFlipAngularLeftOver",
    "ZoomFlipAngularRightOver",

    "CCTransitionShrinkGrow",
    "CCTransitionRotoZoom",

    "CCTransitionMoveInL",
    "CCTransitionMoveInR",
    "CCTransitionMoveInT",
    "CCTransitionMoveInB",
    "CCTransitionSlideInL",
    "CCTransitionSlideInR",
    "CCTransitionSlideInT",
    "CCTransitionSlideInB",
};

KDbool TestTransitions::s_bFlip = KD_TRUE;

TestTransitions* TestTransitions::create ( KDvoid )
{
	TestTransitions*  pLayer = new TestTransitions ( );
	pLayer->autorelease ( );
	pLayer->init ( );

	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( pLayer );

	CCScene*  pTrans = KD_NULL;
	KDfloat   fDuration = TRANSITION_DURATION;

	switch ( s_nSubTestIndex )
	{
		case  0 : pTrans = CCTransitionJumpZoom				::create ( fDuration, pScene ); break;

		case  1 : pTrans = CCTransitionProgressRadialCCW	::create ( fDuration, pScene ); break;
		case  2 : pTrans = CCTransitionProgressRadialCW		::create ( fDuration, pScene ); break;
		case  3 : pTrans = CCTransitionProgressHorizontal	::create ( fDuration, pScene ); break;
		case  4 : pTrans = CCTransitionProgressVertical		::create ( fDuration, pScene ); break;
		case  5 : pTrans = CCTransitionProgressInOut		::create ( fDuration, pScene ); break;
		case  6 : pTrans = CCTransitionProgressOutIn		::create ( fDuration, pScene ); break;

		case  7 : pTrans = CCTransitionCrossFade			::create ( fDuration, pScene ); break;

		case  8 : pTrans = PageTransitionForward			::create ( fDuration, pScene ); break;
		case  9 : pTrans = PageTransitionBackward			::create ( fDuration, pScene ); break;
		case 10 : pTrans = CCTransitionFadeTR				::create ( fDuration, pScene ); break;
		case 11 : pTrans = CCTransitionFadeBL				::create ( fDuration, pScene ); break;
		case 12 : pTrans = CCTransitionFadeUp				::create ( fDuration, pScene ); break;
		case 13 : pTrans = CCTransitionFadeDown				::create ( fDuration, pScene ); break;

		case 14 : pTrans = CCTransitionTurnOffTiles			::create ( fDuration, pScene ); break;

		case 15 : pTrans = CCTransitionSplitRows			::create ( fDuration, pScene ); break;
		case 16 : pTrans = CCTransitionSplitCols			::create ( fDuration, pScene ); break;

		case 17 : pTrans = CCTransitionFade					::create ( fDuration, pScene ); break;
		case 18 : pTrans = FadeWhiteTransition				::create ( fDuration, pScene ); break;

		case 19 : pTrans = FlipXLeftOver					::create ( fDuration, pScene ); break;
		case 20 : pTrans = FlipXRightOver					::create ( fDuration, pScene ); break;
		case 21 : pTrans = FlipYUpOver						::create ( fDuration, pScene ); break;
		case 22 : pTrans = FlipYDownOver					::create ( fDuration, pScene ); break;
		case 23 : pTrans = FlipAngularLeftOver				::create ( fDuration, pScene ); break;
		case 24 : pTrans = FlipAngularRightOver				::create ( fDuration, pScene ); break;

		case 25 : pTrans = ZoomFlipXLeftOver				::create ( fDuration, pScene ); break;
		case 26 : pTrans = ZoomFlipXRightOver				::create ( fDuration, pScene ); break;
		case 27 : pTrans = ZoomFlipYUpOver					::create ( fDuration, pScene ); break;
		case 28 : pTrans = ZoomFlipYDownOver				::create ( fDuration, pScene ); break;
		case 29 : pTrans = ZoomFlipAngularLeftOver			::create ( fDuration, pScene ); break;
		case 30 : pTrans = ZoomFlipAngularRightOver			::create ( fDuration, pScene ); break;

		case 31 : pTrans = CCTransitionShrinkGrow			::create ( fDuration, pScene ); break;
		case 32 : pTrans = CCTransitionRotoZoom				::create ( fDuration, pScene ); break;

		case 33 : pTrans = CCTransitionMoveInL				::create ( fDuration, pScene ); break;
		case 34 : pTrans = CCTransitionMoveInR				::create ( fDuration, pScene ); break;
		case 35 : pTrans = CCTransitionMoveInT				::create ( fDuration, pScene ); break;
		case 36 : pTrans = CCTransitionMoveInB				::create ( fDuration, pScene ); break;

		case 37 : pTrans = CCTransitionSlideInL				::create ( fDuration, pScene ); break;
		case 38 : pTrans = CCTransitionSlideInR				::create ( fDuration, pScene ); break;
		case 39 : pTrans = CCTransitionSlideInT				::create ( fDuration, pScene ); break;
		case 40 : pTrans = CCTransitionSlideInB				::create ( fDuration, pScene ); break;
	}

	CCDirector::sharedDirector ( )->replaceScene ( pTrans );

	return KD_NULL;
}

KDbool TestTransitions::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*  pBG = CCSprite::create ( s_bFlip ? "xm_supports/Back1.jpg" : "xm_supports/Back2.png" );
	this->addChild ( pBG );
	pBG->setPositionWithParent ( kCCAlignmentCenter, ccp ( 0, -15 ) );
	
	s_bFlip = !s_bFlip;

	return KD_TRUE;
}

KDuint TestTransitions::count ( KDvoid )
{
	return MAX_LAYER;
}

const KDchar* TestTransitions::title ( KDvoid )
{
	return l_szSubTitleNames [ s_nSubTestIndex ];
}

const KDchar* TestTransitions::subtitle ( KDvoid )
{
	return s_bFlip ? "Background 1" : "Bacground 2";
}