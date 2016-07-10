/* --------------------------------------------------------------------------
 *
 *      File            XMBackground.cpp
 *      Description     XMBackground
 *      Author          J.S Park, P.G Jeon
 *      Contact         pparkppark84@gmail.com
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
#include "XMBackground.h"

XMBackground* XMBackground::create ( KDint nStage )
{
	XMBackground* pRet = new XMBackground ( );

	if ( pRet && pRet->init ( nStage ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool XMBackground::init ( KDint nStage )
{
	if ( !ParallaxScrollNode::init ( ) )
	{
		return KD_FALSE;
	}

	KDfloat		fRatio [ 5 ] = { 0.3f, 0.5f, 0.7f, 1.0f, 1.0f };
	

	// 배경
	Sprite*		pSpriteBg1 = Sprite::create ( "goblin_bg_1.png" );
	Sprite*		pSpriteBg2 = Sprite::create ( "goblin_bg_1.png" );
	Sprite*		pSpriteBg3 = Sprite::create ( "goblin_bg_1.png" );
	Sprite*		pSpriteBg4 = Sprite::create ( "goblin_bg_1.png" );
	Sprite*		pSpriteBg5 = Sprite::create ( "goblin_bg_1.png" );
	Sprite*		pSpriteBg6 = Sprite::create ( "goblin_bg_1.png" );

	this->addInfiniteScrollXWithZ ( 0, Point ( fRatio [ 0 ], 0  ), Point ( 0, 0 ), pSpriteBg1, pSpriteBg2, pSpriteBg3, pSpriteBg4, pSpriteBg5, pSpriteBg6, KD_NULL );

	KDint		nRand = 20;
	// 작은 나무들 생성
	for ( KDint i = 0; i < nRand ; i++ )
	{
		Sprite*		pSpriteSmallTree = Sprite::create ( "goblin_bg_2.png" );
		KDfloat		fSpeedTreeX = CCRANDOM_BETWEEN ( 0.01f, 0.08f );
        this->addChild ( pSpriteSmallTree, 1, Point ( fSpeedTreeX, 0.015f ), getObjectPosX (  i, nRand ), Point ( m_tContentSize.cx * 2, 0 ) );
        pSpriteSmallTree->setScale ( CCRANDOM_BETWEEN ( 0.8f, 1 ) );
	}

	nRand = 7;
	// 중간 나무들 생성 
	for ( KDint i = 0; i < nRand ; i++ )
	{
		Sprite*		pSpritenMiddleTree = Sprite::create ( "goblin_bg_3.png" );
		KDfloat		fSpeedTreeX = CCRANDOM_BETWEEN ( 0.15f, 0.18f );
        this->addChild ( pSpritenMiddleTree, 2, Point ( fSpeedTreeX, 0.015f ), getObjectPosX ( i, nRand ), Point ( m_tContentSize.cx * 2, 0 ) );
        pSpritenMiddleTree->setScale ( CCRANDOM_BETWEEN ( 0.6f, 1 ) );
	}

	nRand = 2;
	// 큰 나무들 생성 
	for ( KDint i = 0; i < nRand ; i++ )
	{
		Sprite*		pSpriteBigTree = Sprite::create ( "goblin_bg_4.png" );
		KDfloat		fSpeedTreeX = CCRANDOM_BETWEEN ( 0.35f, 0.38f );
        this->addChild ( pSpriteBigTree, 3, Point ( fSpeedTreeX, 0.015f ), getObjectPosX ( i, nRand ), Point ( m_tContentSize.cx * 2, 0 ) );
        pSpriteBigTree->setScale ( CCRANDOM_BETWEEN ( 1, 1.2f ) );
	}

	nRand = 4;
	// 깃발 생성 
	for ( KDint i = 0; i < nRand ; i++ )
	{
		Sprite*		pSpriteFlag = Sprite::create ( "goblin_bg_5.png" );
		KDfloat		fSpeedTreeX = CCRANDOM_BETWEEN ( 0.35f, 0.38f );
        this->addChild ( pSpriteFlag, 4, Point ( fSpeedTreeX, 0.015f ), getObjectPosX ( i, nRand ), Point ( m_tContentSize.cx * 2, 0 ) );
        pSpriteFlag->setScale ( CCRANDOM_BETWEEN ( 0.85f, 1 ) );
	}

	this->schedule ( schedule_selector ( XMBackground::moveBackground ) );
	
	return KD_TRUE;
}

KDvoid XMBackground::moveBackground ( KDfloat fDelta )
{
	this->updateWithVelocity ( Point ( -10.0f, 0 ), fDelta );
}

Point XMBackground::getObjectPosX  ( KDint nCnt, KDint nCmp )
{
	Point tPos;
	KDfloat fCnt = nCnt;
	KDfloat fCmp = nCmp;

	KDfloat fPercent = fCnt / fCmp ;
	if ( nCmp % 2 == 0)
	{
		if ( nCmp == 2 )
		{
			// 가까운 오브젝트 크기가 클때
			if ( nCnt == 0 )
			{
				tPos = Point ( 0, 0 );
			}
			else
			{
				tPos = Point ( m_tContentSize.cx , 0 );
			}
		}
		else if ( nCmp >= 4 )
		{
			// 가까운 오브젝트 크기가 작을때
				tPos = Point ( CCRANDOM_BETWEEN ( m_tContentSize.cx * fPercent, ( m_tContentSize.cx * fPercent * 1.2 ) ) , 0 );
		}
	}
	else
	{
		if ( fPercent >= 0.0f  && fPercent < 0.33f )
		{
			tPos = Point ( CCRANDOM_BETWEEN ( 0						      , m_tContentSize.cx * fPercent * 1.5 ), 0 );
		}
		else if ( fPercent >= 0.33f && fPercent < 0.66f )
		{
			tPos = Point ( CCRANDOM_BETWEEN ( m_tContentSize.cx * fPercent, m_tContentSize.cx * fPercent * 1.5 ), 0 );
		}
		else if ( fPercent >= 0.66f && fPercent < 10.0f )
		{
			tPos = Point ( CCRANDOM_BETWEEN ( m_tContentSize.cx * fPercent, m_tContentSize.cx * fPercent * 1.5 ), 0 );
		}
	}
	
	return tPos;
}