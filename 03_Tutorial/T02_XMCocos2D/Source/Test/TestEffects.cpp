/* --------------------------------------------------------------------------
 *
 *      File            TestEffects.cpp
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
#include "TestEffects.h"

enum 
{
    kTagTextLayer  = 1,
    kTagBackground = 1,
    kTagLabel      = 2,
};

static const KDchar*   l_szEffectLists [ ] =
{
	"Shaky3D",
	"Waves3D",
	"FlipX3D",
	"FlipY3D",
	"Lens3D",
	"Ripple3D",
	"Liquid",
	"Waves",
	"Twirl",
	"ShakyTiles3D",
	"ShatteredTiles3D",
	"ShuffleTiles",
	"FadeOutTRTiles",
	"FadeOutBLTiles",
	"FadeOutUpTiles",
	"FadeOutDownTiles",
	"TurnOffTiles",
	"WavesTiles3D",
	"JumpTiles3D",
	"SplitRows",
	"SplitCols",
	"PageTurn3D",
}; 

class Shaky3DDemo : public CCShaky3D 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCShaky3D::create ( fDuration, ccs ( 15, 10 ), 5, KD_FALSE );
		}
};

class Waves3DDemo : public CCWaves3D 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCWaves3D::create ( fDuration, ccs ( 15, 10 ), 5, 40 );
		}
};

class FlipX3DDemo : public CCFlipX3D 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCFlipX3D*	  pFlipX = CCFlipX3D::create ( fDuration );
			CCDelayTime*  pDelay = CCDelayTime::create ( 2 );
			
			return CCSequence::create ( pFlipX, pDelay, pFlipX->reverse ( ), KD_NULL );
		}
};

class FlipY3DDemo : public CCFlipY3D 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCFlipY3D*	  pFlipY = CCFlipY3D::create ( fDuration );
			CCDelayTime*  pDelay = CCDelayTime::create ( 2 );
			
			return CCSequence::create ( pFlipY, pDelay, pFlipY->reverse ( ), KD_NULL );
		}
};

class Lens3DDemo : public CCLens3D 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCLens3D::create ( fDuration, ccs ( 15, 10 ), ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ), 240 ); 
		}
};

class Ripple3DDemo : public CCRipple3D 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCRipple3D::create ( fDuration, ccs ( 32, 24 ), ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ), 240, 4, 160 );
		}
};

class LiquidDemo : public CCLiquid
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCLiquid::create ( fDuration, ccs ( 16, 12 ), 4, 20 ); 
		}
};

class WavesDemo : public CCWaves 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCWaves::create ( fDuration, ccs ( 16, 12 ), 4, 20, KD_TRUE, KD_TRUE );  
		}
};

class TwirlDemo : public CCTwirl 
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCTwirl::create ( fDuration, ccs ( 12, 8 ), ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ), 1, 2.5f ); 
		}
};

class ShakyTiles3DDemo : public CCShakyTiles3D
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCShakyTiles3D::create ( fDuration, ccs ( 16, 12 ), 5, KD_FALSE );
		}
};

class ShatteredTiles3DDemo : public CCShatteredTiles3D
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCShatteredTiles3D::create ( fDuration, ccs ( 16, 12 ), 5, KD_FALSE ); 
		}
};

class ShuffleTilesDemo : public CCShuffleTiles
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCShuffleTiles*  pShuffle = CCShuffleTiles::create ( fDuration, ccs ( 16, 12 ), 25 );
			CCDelayTime*	 pDelay   = CCDelayTime::create ( 2 );

			return CCSequence::create ( pShuffle, pDelay, pShuffle->reverse ( ), KD_NULL );
		}
};

class FadeOutTRTilesDemo : public CCFadeOutTRTiles
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCFadeOutTRTiles*  pFadeout = CCFadeOutTRTiles::create ( fDuration, ccs ( 16, 12 ) );
			CCDelayTime*       pDelay   = CCDelayTime::create ( 0.5f );

			return CCSequence::create ( pFadeout, pDelay, pFadeout->reverse ( ), KD_NULL );
		}
};

class FadeOutBLTilesDemo : public CCFadeOutBLTiles
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCFadeOutBLTiles*  pFadeout = CCFadeOutBLTiles::create ( fDuration, ccs ( 16, 12 ) );
			CCDelayTime*       pDelay   = CCDelayTime::create ( 0.5f );

			return CCSequence::create ( pFadeout, pDelay, pFadeout->reverse ( ), KD_NULL );
		}
};

class FadeOutUpTilesDemo : public CCFadeOutUpTiles
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCFadeOutUpTiles*  pFadeout = CCFadeOutUpTiles::create ( fDuration, ccs ( 16, 12 ) );
			CCDelayTime*       pDelay   = CCDelayTime::create ( 0.5f );

			return CCSequence::create ( pFadeout, pDelay, pFadeout->reverse ( ), KD_NULL );
		}
};

class FadeOutDownTilesDemo : public CCFadeOutDownTiles
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCFadeOutDownTiles*  pFadeout = CCFadeOutDownTiles::create ( fDuration, ccs ( 16, 12 ) );
			CCDelayTime*         pDelay   = CCDelayTime::create ( 0.5f );

			return CCSequence::create ( pFadeout, pDelay, pFadeout->reverse ( ), KD_NULL );
		}
};

class TurnOffTilesDemo : public CCTurnOffTiles
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCTurnOffTiles*   pFadeout = CCTurnOffTiles::create ( fDuration, ccs ( 48, 32 ), 25 );
			CCDelayTime*      pDelay   = CCDelayTime::create ( 0.5f );

			return CCSequence::create ( pFadeout, pDelay, pFadeout->reverse ( ), KD_NULL );
		}
};

class WavesTiles3DDemo : public CCWavesTiles3D
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCWavesTiles3D::create ( fDuration, ccs ( 15, 10 ), 4, 120 ); 
		}
};

class JumpTiles3DDemo : public CCJumpTiles3D
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCJumpTiles3D::create ( fDuration, ccs ( 15, 10 ), 2, 30 ); 
		}
};

class SplitRowsDemo : public CCSplitRows
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCSplitRows::create ( fDuration, 9 ); 
		}
};

class SplitColsDemo : public CCSplitCols
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			return CCSplitCols::create ( fDuration, 9 ); 
		}
};

class PageTurn3DDemo : public CCPageTurn3D
{
	public :

		static CCActionInterval*	create ( KDfloat fDuration )
		{
			CCDirector::sharedDirector ( )->setDepthTest ( KD_TRUE );

			return CCPageTurn3D::create ( fDuration, ccs ( 15, 10 ) ); 
		}
};

//------------------------------------------------------------------
//
// TestEffects
//
//------------------------------------------------------------------
KDuint TestEffects::count ( KDvoid )
{
	return 22;
}

class CCTestNode : public CCNode
{
	public :

	static  CCTestNode*  create ( KDvoid )
	{
		CCTestNode*  pRet = new CCTestNode ( );
		pRet->autorelease ( );
		return pRet;
	}

	virtual KDvoid  visit ( KDvoid )
	{
		CCNode::visit ( );
	}

	virtual KDvoid  draw  ( KDvoid )
	{
		CCNode::draw ( );
	}
};

KDbool TestEffects::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}
	
	const CCSize&  tLyrSize = this->getContentSize ( );
	
	CCNode*  pNode = CCNode::create ( );
	CCActionInterval*  pEffect = createEffect ( 3 );
	pNode->runAction ( pEffect );
	this->addChild ( pNode, 0, kTagBackground );

	CCSprite*  pBG = CCSprite::create ( "Images/background3.png" );
	pBG->setPosition ( ccpMid ( tLyrSize ) );
	pBG->setOpacity  ( 150 );
	pNode->addChild  ( pBG, 0 );

	CCSprite*  pKathia = CCSprite::create ( "Images/grossinis_sister2.png" );
	pKathia->setPosition ( ccp ( tLyrSize.cx / 3, tLyrSize.cy / 2 ) );

	CCActionInterval*  sc = CCScaleBy::create ( 2, 5 );
	CCActionInterval*  sc_back = sc->reverse ( );
	pKathia->runAction ( CCRepeatForever::create ( CCSequence::create ( sc, sc_back, KD_NULL ) ) );
	pNode->addChild ( pKathia, 1 );

	CCSprite*  pTamara = CCSprite::create ( "Images/grossinis_sister1.png" );
	pTamara->setPosition( ccp ( 2 * tLyrSize.cx / 3, tLyrSize.cy / 2 ) );

	CCActionInterval*  sc2 = CCScaleBy::create ( 2, 5 );
	CCActionInterval*  sc2_back = sc2->reverse ( );
	pTamara->runAction ( CCRepeatForever::create ( CCSequence::create ( sc2, sc2_back, KD_NULL ) ) );
	pNode->addChild ( pTamara, 1 );

	schedule ( schedule_selector ( TestEffects::checkAnim ) );

	return KD_TRUE;
}

const KDchar* TestEffects::title ( KDvoid )
{
	return l_szEffectLists [ s_nSubTestIndex ];
}

KDvoid TestEffects::checkAnim ( KDfloat fDelta )
{
	CCNode*  s2 = getChildByTag ( kTagBackground );
	if ( s2 && s2->numberOfRunningActions ( ) == 0 && s2->getGrid ( ) != KD_NULL )
	{
		s2->setGrid ( KD_NULL );
	}
}

CCActionInterval* TestEffects::createEffect ( KDfloat fDuration )
{
	switch ( s_nSubTestIndex )
	{
		case  0 : return Shaky3DDemo          ::create ( fDuration );
		case  1 : return Waves3DDemo          ::create ( fDuration );
		case  2 : return FlipX3DDemo          ::create ( fDuration );
		case  3 : return FlipY3DDemo          ::create ( fDuration );
		case  4 : return Lens3DDemo           ::create ( fDuration );
		case  5 : return Ripple3DDemo         ::create ( fDuration );
		case  6 : return LiquidDemo           ::create ( fDuration );
		case  7 : return WavesDemo            ::create ( fDuration );
		case  8 : return TwirlDemo            ::create ( fDuration );
		case  9 : return ShakyTiles3DDemo     ::create ( fDuration );
		case 10 : return ShatteredTiles3DDemo ::create ( fDuration );
		case 11 : return ShuffleTilesDemo     ::create ( fDuration );
		case 12 : return FadeOutTRTilesDemo   ::create ( fDuration );
		case 13 : return FadeOutBLTilesDemo   ::create ( fDuration );
		case 14 : return FadeOutUpTilesDemo   ::create ( fDuration );
		case 15 : return FadeOutDownTilesDemo ::create ( fDuration );
		case 16 : return TurnOffTilesDemo     ::create ( fDuration );
		case 17 : return WavesTiles3DDemo     ::create ( fDuration );
		case 18 : return JumpTiles3DDemo      ::create ( fDuration );
		case 19 : return SplitRowsDemo        ::create ( fDuration );
		case 20 : return SplitColsDemo        ::create ( fDuration );
		case 21 : return PageTurn3DDemo       ::create ( fDuration );
	}

	return KD_NULL;
}
