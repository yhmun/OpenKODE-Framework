/* --------------------------------------------------------------------------
 *
 *      File            Ch1_GridEffects.cpp
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
#include "Ch1_GridEffects.h"

enum 
{
	TAG_SPRITE_TO_EFFECT	= 0,
	TAG_PREV_EFFECT			= 1,
	TAG_RELOAD_EFFECT		= 2,
	TAG_NEXT_EFFECT			= 3,
	TAG_EFFECT_NAME			= 4,
};

static const KDchar*	l_aEffectMethods [ ] =
{
	"Shaky3D"			,
	"Waves3D"			,
	"FlipX3D"			,
	"FlipY3D"			,
	"Lens3D"			,
	"Ripple3D"			,
	"Liquid"			,
	"Waves"				,
	"Twirl"				,
	"ShakyTiles3D"		,
	"ShatteredTiles3D"	,
	"ShuffleTiles"		,
	"FadeOutTRTiles"	,
	"FadeOutBLTiles"	,
	"FadeOutUpTiles"	,
	"FadeOutDownTiles"	,
	"TurnOffTiles"		,
	"WavesTiles3D"		,
	"JumpTiles3D"		,
	"SplitRows"			,
	"SplitCols"			,
	"PageTurn3D"		,
};

KDbool Ch1_GridEffects::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// TO DO - Replace this texture with one that I've drawn myself. Also, one that better illustrates the effects.

	CCSprite*	pSprite = CCSprite::create ( "colorable_sprite.png" );
	pSprite->setPosition ( ccp ( 240, 150 ) );
	pSprite->setScale ( 2.0f );
	this->addChild ( pSprite, 0, TAG_SPRITE_TO_EFFECT );	

	this->initSubMenus ( sizeof ( l_aEffectMethods ) / sizeof ( l_aEffectMethods [ 0 ] ) );
	
	return KD_TRUE;
}

const KDchar* Ch1_GridEffects::runSubAction ( KDint nIndex )
{
	CCNode*		pNode = this->getChildByTag ( TAG_SPRITE_TO_EFFECT );
	pNode->stopAllActions ( );

	const CCSize&		tLyrSize = this->getContentSize ( );
	CCActionInterval*	pEffect  = KD_NULL;

	switch ( nIndex )
	{
		case 0 :	
			pEffect = CCShaky3D::create ( 10, ccp ( 15, 10 ), 5, KD_TRUE );
			break;

		case 1 :	
			pEffect = CCWaves3D::create ( 10, ccp ( 15, 10 ), 5, 40 );
			break;

		case 2 :	
			pEffect = CCFlipX3D::create ( 1 );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 0 ), pEffect->reverse ( ), KD_NULL );
			break;

		case 3 :	
			pEffect = CCFlipY3D::create ( 1 );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 0 ), pEffect->reverse ( ), KD_NULL );
			break;

		case 4 :	
			pEffect = CCLens3D::create ( 10, ccp ( 15, 10 ), ccpMid ( tLyrSize ), 240 );
			break;

		case 5 :	
			pEffect = CCRipple3D::create ( 10, ccp ( 32, 24 ), ccpMid ( tLyrSize ), 240, 4, 160 );
			break;

		case 6 :	
			pEffect = CCLiquid::create ( 10, ccp ( 16, 12 ), 4, 20 );
			break;

		case 7 :	
			pEffect = CCWaves::create ( 10, ccp ( 16, 12 ), 4, 20, KD_TRUE, KD_TRUE );
			break;

		case 8 :	
			pEffect = CCTwirl::create ( 10, ccp ( 12, 8 ), ccpMid ( tLyrSize ), 1, 2.5f );
			break;

		case 9 :	
			pEffect = CCShakyTiles3D::create ( 10, ccp ( 16, 12 ), 5, KD_TRUE );
			break;

		case 10 :	
			pEffect = CCShaky3D::create ( 10, ccp ( 16, 12 ), 5, KD_TRUE );
			break;

		case 11 :	
			pEffect = CCShuffleTiles::create ( 5, ccp ( 16, 12 ), 25 );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 2 ), pEffect->reverse ( ), KD_NULL );
			break;

		case 12 :	
			pEffect = CCFadeOutTRTiles::create ( 2, ccp ( 16, 12 ) );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 0.5f ), pEffect->reverse ( ), KD_NULL );
			break;

		case 13 :	
			pEffect = CCFadeOutBLTiles::create ( 2, ccp ( 16, 12 ) );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 0.5f ), pEffect->reverse ( ), KD_NULL );
			break;

		case 14 :	
			pEffect = CCFadeOutUpTiles::create ( 2, ccp ( 16, 12 ) );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 0.5f ), pEffect->reverse ( ), KD_NULL );
			break;

		case 15 :	
			pEffect = CCFadeOutDownTiles::create ( 2, ccp ( 16, 12 ) );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 0.5f ), pEffect->reverse ( ), KD_NULL );
			break;

		case 16 :	
			pEffect = CCTurnOffTiles::create ( 2, ccp ( 48, 32 ), 25 );
			pEffect = CCSequence::create ( pEffect, CCDelayTime::create ( 0.5f ), pEffect->reverse ( ), KD_NULL );
			break;

		case 17 :	
			pEffect = CCWavesTiles3D::create ( 10, ccp ( 15, 10 ), 4, 120 );
			break;

		case 18 :	
			pEffect = CCJumpTiles3D::create ( 10, ccp ( 15, 10 ), 2, 30 );
			break;

		case 19 :	
			pEffect = CCSplitRows::create ( 10, 9 );
			break;

		case 20 :	
			pEffect = CCSplitCols::create ( 10, 9 );			
			break;

		case 21 :	
			pEffect = CCPageTurn3D::create ( 2, ccp ( 15, 10 ) );
			break;
	}

	pNode->runAction ( pEffect );

	return l_aEffectMethods [ nIndex ];
}