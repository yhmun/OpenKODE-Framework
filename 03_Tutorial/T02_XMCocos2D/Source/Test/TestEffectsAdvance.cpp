/* --------------------------------------------------------------------------
 *
 *      File            TestEffectsAdvance.cpp
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
#include "TestEffectsAdvance.h"

enum 
{
	kTagTextLayer  = 1, 

	kTagSprite1    = 1, 
	kTagSprite2    = 2, 

	kTagBackground = 1, 
	kTagLabel      = 2, 
};

TestEffectAdvance* TestEffectAdvance::create ( KDvoid )
{
	TestEffectAdvance*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case 0 : pLayer = new Effect3  ( ); break;
		case 1 : pLayer = new Effect2  ( ); break;
		case 2 : pLayer = new Effect1  ( ); break;
		case 3 : pLayer = new Effect4  ( ); break;
		case 4 : pLayer = new Effect5  ( ); break;
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

KDuint TestEffectAdvance::count ( KDvoid )
{
	return 5;
}

KDbool TestEffectAdvance::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCSprite*  pBG = CCSprite::create ( "Images/background3.png" );
	pBG->setPosition ( ccpMid ( tLyrSize ) );
	pBG->setOpacity ( 128 );
	this->addChild ( pBG, 0, kTagBackground );

	const CCSize&  tSize = pBG->getContentSize ( );

	CCSprite*  pGrossini = CCSprite::create ( "Images/grossinis_sister2.png" );
	pGrossini->setPosition ( ccp ( tSize.cx / 3.0f, tSize.cy / 2 ) );
	pBG->addChild ( pGrossini, 1, kTagSprite1 );
	
	CCActionInterval*  sc = CCScaleBy::create ( 2, 5 );
	pGrossini->runAction ( CCRepeatForever::create ( CCSequence::create ( sc, sc->reverse ( ), KD_NULL ) ) );

	CCSprite*  pTamara = CCSprite::create ( "Images/grossinis_sister1.png" );
	pTamara->setPosition ( ccp ( 2 * tSize.cx / 3.0f, tSize.cy / 2 ) );
	pBG->addChild ( pTamara, 1, kTagSprite2 );

	CCActionInterval*  sc2 = CCScaleBy::create ( 2, 5 );
	pTamara->runAction ( CCRepeatForever::create ( CCSequence::create ( sc2, sc2->reverse ( ), KD_NULL ) ) );
	
	return KD_TRUE;
}

//------------------------------------------------------------------
//
// Effect1
//
//------------------------------------------------------------------
KDvoid Effect1::onEnter ( KDvoid )
{
	TestEffectAdvance::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCNode*  target = getChildByTag ( kTagBackground );

	// To reuse a grid the grid size and the grid type must be the same.
	// in this case:
	//     Lens3D is Grid3D and it's size is ( 15, 10 )
	//     Waves3D is Grid3D and it's size is ( 15, 10 )		
	CCActionInterval*    lens = CCLens3D::create ( 0, ccs ( 15, 10 ), ccp ( s.cx / 2, s.cy / 2 ), 240 );
	CCActionInterval*   waves = CCWaves3D::create ( 10, ccs ( 15, 10 ), 18, 15 );
	CCFiniteTimeAction* reuse = CCReuseGrid::create ( 1 );
	CCActionInterval*   delay = CCDelayTime::create ( 8 );
	CCActionInterval*   orbit = CCOrbitCamera::create ( 5, 1, 2, 0, 180, 0, -90 );

	target->runAction ( CCRepeatForever::create ( CCSequence::create ( orbit, orbit->reverse ( ), KD_NULL ) ) );
	target->runAction ( CCSequence::create ( lens, delay, reuse, waves, KD_NULL ) );

}

const KDchar* Effect1::title ( KDvoid )
{
	return "Lens + Waves3d and OrbitCamera";
}

//------------------------------------------------------------------
//
// Effect2
//
//------------------------------------------------------------------
KDvoid Effect2::onEnter ( KDvoid )
{
	TestEffectAdvance::onEnter ( );

	CCNode*  target = getChildByTag ( kTagBackground );

	// To reuse a grid the grid size and the grid type must be the same.
	// in this case:
	//     ShakyTiles is TiledGrid3D and it's size is ( 15, 10 )
	//     Shuffletiles is TiledGrid3D and it's size is ( 15, 10 )
	//	   TurnOfftiles is TiledGrid3D and it's size is ( 15, 10 )
	CCActionInterval*  shaky   = CCShakyTiles3D::create ( 5, ccs ( 15, 10 ), 4, KD_FALSE );
	CCActionInterval*  shuffle = CCShuffleTiles::create ( 0, ccs ( 15, 10 ), 3 );
	CCActionInterval*  turnoff = CCTurnOffTiles::create ( 0, ccs ( 15, 10 ), 3 );
	
	// reuse 2 times:
	//   1 for shuffle
	//   2 for turn off
	//   turnon tiles will use a new grid
	CCFiniteTimeAction* reuse = CCReuseGrid::create ( 2 );
	CCActionInterval*   delay = CCDelayTime::create ( 1 );
	
	target->runAction ( CCSequence::create ( shaky, delay, reuse, shuffle, delay->copy ( )->autorelease ( ), turnoff, turnoff->reverse ( ), KD_NULL ) );
}

const KDchar* Effect2::title ( KDvoid )
{
	return "ShakyTiles + ShuffleTiles + TurnOffTiles";
}

//------------------------------------------------------------------
//
// Effect3
//
//------------------------------------------------------------------
KDvoid Effect3::onEnter ( KDvoid )
{
	TestEffectAdvance::onEnter ( );

	const CCSize&  s = this->getContentSize ( );

	CCNode*  bg = getChildByTag ( kTagBackground );
	CCNode*  target1 = bg->getChildByTag ( kTagSprite1 );
	CCNode*  target2 = bg->getChildByTag ( kTagSprite2 );	

	target1->setPosition ( ccp ( s.cx / 3, s.cy / 2 ) );
	target2->setPosition ( ccp ( s.cx / 3 * 2, s.cy / 2 ) );

	CCActionInterval*  waves = CCWaves::create ( 5, ccs ( 15, 10 ), 5, 20, KD_TRUE, KD_FALSE );
	target1->runAction ( CCRepeatForever::create ( waves ) );

	CCActionInterval*  shaky = CCShaky3D::create ( 5, ccs ( 15, 10 ), 4, KD_FALSE );	
	target2->runAction ( CCRepeatForever::create ( shaky ) );

	// moving background. Testing issue #244
	CCActionInterval*  move = CCMoveBy::create ( 3, ccp ( 150, 0 ) );
	bg->runAction ( CCRepeatForever::create ( CCSequence::create ( move, move->reverse ( ), KD_NULL ) ) );	

}

const KDchar* Effect3::title ( KDvoid )
{
	return "Effects on 2 sprites";
}

//------------------------------------------------------------------
//
// Effect4
//
//------------------------------------------------------------------
class Lens3DTarget : public CCNode
{
	public :

		static Lens3DTarget*		create ( CCLens3D* pAction )
		{
			Lens3DTarget*  pRet = new Lens3DTarget();
			pRet->m_pLens3D = pAction;
			pRet->autorelease ( );
			return pRet;
		}

		virtual const CCPoint&		getPosition ( KDvoid )
		{
			return m_pLens3D->getPosition ( );
		}

		virtual KDvoid				setPosition ( const CCPoint& tPosition )
		{
			m_pLens3D->setPosition ( tPosition );
		}

	private :

		CCLens3D*					m_pLens3D;
};

KDvoid Effect4::onEnter ( KDvoid )
{
	TestEffectAdvance::onEnter ( );

	CCDirector*  pDirector = CCDirector::sharedDirector ( );

	const CCSize&  tWinSize = pDirector->getWinSize ( );

    CCLens3D*  pLens = CCLens3D::create ( 10, ccs ( 32, 24 ), ccp ( 100, 180 ), 150 );
	CCActionInterval*  pMove = CCJumpBy::create ( 5, ccp ( tWinSize.cx / 3 * 2, tWinSize.cy / 2 ), 100, 4 );
    CCActionInterval*   pSeq = CCSequence::create ( pMove, pMove->reverse ( ), KD_NULL );

    /* In cocos2d-iphone, the type of action's target is 'id', so it supports using the instance of 'CCLens3D' as its target.
        While in cocos2d-x, the target of action only supports CCNode or its subclass,
        so we make an encapsulation for CCLens3D to achieve that.
    */

    CCNode*  pTarget = Lens3DTarget::create ( pLens );
    // Please make sure the target been added to its parent.
    this->addChild ( pTarget );

    pDirector->getActionManager()->addAction ( pSeq, pTarget, KD_FALSE );
    this->runAction ( pLens );
}

const KDchar* Effect4::title ( KDvoid )
{
	return "Jumpy Lens3D";
}

//------------------------------------------------------------------
//
// Effect5
//
//------------------------------------------------------------------
KDvoid Effect5::onEnter ( KDvoid )
{
	TestEffectAdvance::onEnter ( );
	
	CCNode*  pBG = getChildByTag ( kTagBackground );

	CCActionInterval*  pStopEffect = 
	(
		CCSequence::create 
		( 
			CCLiquid   ::create ( 2, ccs ( 32, 24 ), 1, 20 ), 
			CCDelayTime::create ( 2 ), 
			CCStopGrid ::create ( ), 
			KD_NULL 
		)
	);

	pBG->runAction ( pStopEffect );
}

const KDchar* Effect5::title ( KDvoid )
{
	return "Test Stop-Copy-Restar";
}