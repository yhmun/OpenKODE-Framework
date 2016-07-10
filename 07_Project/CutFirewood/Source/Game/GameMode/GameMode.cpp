/* -----------------------------------------------------------------------------------
 *
 *      File            GameMode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "GameMode.h"
#include "Object/Animation/Animations.h"
#include "Object/Sprite/Sprite.h"
#include "Object/ColorLayer/ColorLayer.h"
#include "Game/Commander/Commander.h"

cGameMode::cGameMode ( JBaseScene* pScene, cCommander* pCommander )
: m_pCommander						( pCommander	)
, m_pBack							( nullptr		)
, m_pSprGaugeTime					( nullptr		)
, m_pSprGaugeTime2					( nullptr		)
, m_pSprGaugeTime3					( nullptr		)
, m_pSprGaugeBerserker				( nullptr		)
, m_pSprGaugeBerserker2				( nullptr		)
, m_pSprGaugeBerserker3				( nullptr		)
, m_pSprCutin1						( nullptr		)
, m_pSprCutin2						( nullptr		)
, m_pColorLayerIntro				( nullptr		)
, m_tTimeGauge						( Size::ZERO	)
, m_tBerserkerGauge					( Size::ZERO	)
, m_bIsTimeMode						( false			)
, m_bTimeModeGaugeOpacityAdd		( false			)
, m_bIsBerserkerMode				( false			)
, m_bBerserkerModeGaugeOpacityAdd	( false			)
, m_bIsTimeModeIntro				( false			)
, m_nTimeModeIntroStep				( 0				)
, m_dTimeModeIntroTime				( 0				)
, m_nTimeModeIntroCutinType			( 0				)
, m_bIsBerserkerModeIntro			( 0				)
, m_nBerserkerModeIntroStep			( 0				)
, m_dBerserkerModeIntroTime			( 0				)
, m_nBerserkerModeIntroCutinType	( 0				)
, m_fGaugeTime						( 0				)
, m_fGaugeBerserker					( 0				)
, m_fSeenGaugeTime					( 0				)
, m_fSeenGaugeBerserker				( 0				)
, m_fSeenGaugeTimeOpacity			( 0 			)
, m_fSeenGaugeBerserkerOpacity		( 0 			)
{
	// sprite add
	m_pBack				  = new cSprite ( "gauge_bg.png"		 , eLayerGame_Gauge, Point (  0,   0 ) );
	m_pSprGaugeTime2	  = new cSprite ( "gauge_time.png"		 , eLayerGame_Gauge, Point (  0,   3 ) );
	m_pSprGaugeTime		  = new cSprite ( "gauge_time.png"		 , eLayerGame_Gauge, Point (  0,   3 ) );
	m_pSprGaugeTime3	  = new cSprite ( "gauge_time_2.png"	 , eLayerGame_Gauge, Point (  0,   3 ) );
	m_pSprGaugeBerserker2 = new cSprite ( "gauge_berserker.png"	 , eLayerGame_Gauge, Point ( 26,   6 ) );
	m_pSprGaugeBerserker  = new cSprite ( "gauge_berserker.png"	 , eLayerGame_Gauge, Point ( 26,   6 ) );
	m_pSprGaugeBerserker3 = new cSprite ( "gauge_berserker_2.png", eLayerGame_Gauge, Point ( 26,   6 ) );
	m_pSprCutin1		  = new cSprite ( "cutin_1.png"			 , eLayerGame_Cutin, Point (  0,   0 ) );
	m_pSprCutin2		  = new cSprite ( "cutin_2.png"			 , eLayerGame_Cutin, Point (  0,   0 ) );

	m_pSprGaugeTime3	 ->setIsVisible ( false );
	m_pSprGaugeBerserker3->setIsVisible ( false );
	
	m_pSprCutin1->setIsVisible ( false );
	m_pSprCutin2->setIsVisible ( false );
	
	pScene->addDrawObject ( m_pBack					);
	pScene->addDrawObject ( m_pSprGaugeTime			);
	pScene->addDrawObject ( m_pSprGaugeTime2		);
	pScene->addDrawObject ( m_pSprGaugeTime3		);
	pScene->addDrawObject ( m_pSprGaugeBerserker	);
	pScene->addDrawObject ( m_pSprGaugeBerserker2	);
	pScene->addDrawObject ( m_pSprGaugeBerserker3	);
	pScene->addDrawObject ( m_pSprCutin1			);
	pScene->addDrawObject ( m_pSprCutin2			);
	
	// texture size backup
	m_tTimeGauge		= m_pSprGaugeTime	  ->getTextureRect ( ).size;
	m_tBerserkerGauge	= m_pSprGaugeBerserker->getTextureRect ( ).size;
	
	// texture rect
	m_pSprGaugeTime		 ->setTextureRect ( Rect ( 0, m_tTimeGauge.height, m_tTimeGauge.width,						   0 ) );
	m_pSprGaugeTime2	 ->setTextureRect ( Rect ( 0, m_tTimeGauge.height, m_tTimeGauge.width,						   0 ) );
	m_pSprGaugeBerserker ->setTextureRect ( Rect ( 0,					0,					 0, m_tBerserkerGauge.height ) );
	m_pSprGaugeBerserker2->setTextureRect ( Rect ( 0,					0,					 0, m_tBerserkerGauge.height ) );

	// color layer
	Color4B		tColor;
	tColor.r = 255;
	tColor.g = 255;
	tColor.b = 255;
	tColor.a = 255;
	m_pColorLayerIntro = new cColorLayer ( tColor, eLayerGame_gameModeCover, Size ( _SCR_W, _SCR_H ), Point ( 0, 0 ) );
	m_pColorLayerIntro->setIsVisible ( false );
	pScene->addDrawObject ( m_pColorLayerIntro );
}

cGameMode::~cGameMode ( KDvoid )
{
	// sprite
	m_pBack					= nullptr;
	m_pSprGaugeTime			= nullptr;
	m_pSprGaugeTime2		= nullptr;
	m_pSprGaugeTime3		= nullptr;
	m_pSprGaugeBerserker	= nullptr;
	m_pSprGaugeBerserker2	= nullptr;
	m_pSprGaugeBerserker3	= nullptr;
	m_pSprCutin1			= nullptr;
	m_pSprCutin2			= nullptr;
	
	// color layer
	m_pColorLayerIntro		= nullptr;
}

KDvoid cGameMode::setGaugeZero ( KDvoid )
{
	m_bIsTimeMode					= false;
	m_bTimeModeGaugeOpacityAdd		= false;
	m_bIsBerserkerMode				= false;
	m_bBerserkerModeGaugeOpacityAdd	= false;
	m_bIsTimeModeIntro				= false;
	m_nTimeModeIntroStep			= 0;
	m_dTimeModeIntroTime			= 0;
	m_nTimeModeIntroCutinType		= 0;
	m_bIsBerserkerModeIntro			= 0;
	m_nBerserkerModeIntroStep		= 0;
	m_dBerserkerModeIntroTime		= 0;
	m_nBerserkerModeIntroCutinType	= 0;
	m_fGaugeTime					= 0;
	m_fGaugeBerserker				= 0;
	m_fSeenGaugeTime				= 0;
	m_fSeenGaugeBerserker			= 0;
	m_fSeenGaugeTimeOpacity			= 0;
	m_fSeenGaugeBerserkerOpacity	= 0;
}

KDvoid cGameMode::setAddTimeGauge ( KDbool bIsMiss, KDbool bIsComboAdd )
{
	// check
	if ( m_bIsTimeMode == true )
	{
		return;
	}
	
	// add
		 if ( bIsMiss	  == true )	m_fGaugeTime += 0.05f;
	else if ( bIsComboAdd == true )	m_fGaugeTime += 0.01f;
	else							m_fGaugeTime += 0.025f;
	
	// seen gauge opacity
	m_fSeenGaugeTimeOpacity = 0;

	// max check
	if ( m_fGaugeTime > 1 )
	{
		m_fGaugeTime		= 1;
		m_bIsTimeMode		= true;
		m_bIsTimeModeIntro	= true;
		
		// sprite
		m_pSprGaugeTime2->setIsVisible ( false );
		m_pSprGaugeTime3->setIsVisible ( true );
		m_pSprGaugeTime3->setOpacity ( 0 );
		m_bTimeModeGaugeOpacityAdd = true;
		
		// commander send msg
		m_pCommander->msgTimeModeIntro ( true );
		m_pCommander->msgSprBackGray ( true );
		
		// color layer
		m_pColorLayerIntro->setIsVisible ( true );
		m_pColorLayerIntro->setColor ( 255, 255, 255, 255 );
		
		// intro
		m_bIsTimeModeIntro = true;
		m_nTimeModeIntroStep = 0;
		m_dTimeModeIntroTime = 0;
		m_nTimeModeIntroCutinType = kdRand ( ) % 2;
		
		if ( m_nTimeModeIntroCutinType == 0 )	m_pSprCutin1->setIsVisible ( true );
		else									m_pSprCutin2->setIsVisible ( true );

		// sound
		if ( cGlobalData::getObj ( )->m_bIsEffectMusic == true )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "mode_time.wav" );
		}
	}
}

KDvoid cGameMode::setAddBerserkerGauge ( KDbool bIsMiss, KDint nComboCount, KDbool bIsMaxPower )
{
	if ( m_bIsBerserkerMode == true || bIsMiss == true )
	{
		return;
	}

	// add
	if ( bIsMaxPower == true )	m_fGaugeBerserker += 0.08f + ( nComboCount / 1000 );
	else						m_fGaugeBerserker += 0.01f + ( nComboCount / 1000 );
	
	// seen gauge opacity
	m_fSeenGaugeBerserkerOpacity = 0;

	// max check
	if ( m_fGaugeBerserker > 1 )
	{
		m_fGaugeBerserker		= 1;
		m_bIsBerserkerMode		= true;
		m_bIsBerserkerModeIntro	= true;
		
		// sprite
		m_pSprGaugeBerserker2->setIsVisible ( false );
		m_pSprGaugeBerserker3->setIsVisible ( true );
		m_pSprGaugeBerserker3->setOpacity ( 0 );
		m_bBerserkerModeGaugeOpacityAdd = true;
		
		// commander send msg
		m_pCommander->msgBerserkerModeIntro ( true );
		m_pCommander->msgCharAngry		    ( true );

		// color layer
		m_pColorLayerIntro->setIsVisible ( true );
		m_pColorLayerIntro->setColor ( 255, 255, 255, 255 );
		
		// intro
		m_bIsBerserkerModeIntro		   = true;
		m_nBerserkerModeIntroStep	   = 0;
		m_dBerserkerModeIntroTime	   = 0;
		m_nBerserkerModeIntroCutinType = kdRand ( ) % 2;
		
		if ( m_nBerserkerModeIntroCutinType == 0 )	m_pSprCutin1->setIsVisible ( true );
		else										m_pSprCutin2->setIsVisible ( true );
		
		// sound
		if ( cGlobalData::getObj ( )->m_bIsEffectMusic == true )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "mode_berserk.wav" );
		}
	}
}

KDvoid cGameMode::update ( KDdouble dLeftTime )
{
	// time intro mode
	if ( m_bIsTimeModeIntro == true )
	{
		// color layer
		KDfloat		fOpacity = (KDfloat) m_pColorLayerIntro->getOpacity ( );
		fOpacity -= ( 255 * dLeftTime ) * 5;
		if ( fOpacity < 0 )	fOpacity = 0;
		m_pColorLayerIntro->setOpacity ( fOpacity );
		
		// time
		m_dTimeModeIntroTime += dLeftTime;
		
		// step == 0
		if ( m_nTimeModeIntroStep == 0 )
		{
			// power percent
			KDfloat		fPercent = m_dTimeModeIntroTime / 0.2f;
			
			// point
			Point		tPoint = Point ( _SCR_W - 250*fPercent, 0 );
			
			// sprite set
			m_pSprCutin1->setPoint ( tPoint );
			m_pSprCutin2->setPoint ( tPoint );
			
			if ( fPercent > 1 )
			{
				m_pSprCutin1->setPoint ( Point ( _SCR_W - 250, 0 ) );
				m_pSprCutin2->setPoint ( Point ( _SCR_W - 250, 0 ) );
				
				m_dTimeModeIntroTime = 0;
				++m_nTimeModeIntroStep;
			}
		}
		else if ( m_nTimeModeIntroStep == 1 )
		{
			if ( m_dTimeModeIntroTime > 0.7f )
			{
				m_dTimeModeIntroTime = 0;
				++m_nTimeModeIntroStep;
			}
		}
		else if ( m_nTimeModeIntroStep == 2 )
		{
			// power percent
			KDfloat		fPercent = m_dTimeModeIntroTime / 0.2f;
			
			// point
			Point		tPoint = Point ( ( _SCR_W - 250 ) + 250 * fPercent, 0 );
			
			// sprite set
			m_pSprCutin1->setPoint ( tPoint );
			m_pSprCutin2->setPoint ( tPoint );
			
			if ( fPercent > 1 )
			{
				m_pSprCutin1->setIsVisible ( false );
				m_pSprCutin2->setIsVisible ( false );
				
				m_dTimeModeIntroTime = 0;
				++m_nTimeModeIntroStep;
			}
		}
		else if ( m_nTimeModeIntroStep == 3 )
		{
			if ( m_dTimeModeIntroTime > 0.2 )
			{
				m_pColorLayerIntro->setIsVisible ( false );

				m_bIsTimeModeIntro = false;

				m_pCommander->msgTimeModeIntro ( false );
			}
		}
		
		// gauge
		updateGauge ( dLeftTime );
		
		return;
	}
	else if ( m_bIsBerserkerModeIntro == true )
	{
		// color layer
		KDfloat		fOpacity = (KDfloat) m_pColorLayerIntro->getOpacity ( );
		fOpacity -= ( 255 * dLeftTime ) * 5;	
		if ( fOpacity < 0 )	fOpacity = 0;
		m_pColorLayerIntro->setOpacity ( fOpacity );
		
		// time
		m_dBerserkerModeIntroTime += dLeftTime;
		
		// step == 0
		if ( m_nBerserkerModeIntroStep == 0 )
		{
			// power percent
			KDfloat		fPercent = m_dBerserkerModeIntroTime / 0.2f;
			
			//	point
			Point		tPoint = Point ( _SCR_W - 250 * fPercent, 0 );
			
			//sprite set
			m_pSprCutin1->setPoint ( tPoint );
			m_pSprCutin2->setPoint ( tPoint );
			
			if ( fPercent > 1 )
			{
				m_pSprCutin1->setPoint ( Point ( _SCR_W - 250, 0 ) );
				m_pSprCutin2->setPoint ( Point ( _SCR_W - 250, 0 ) );
				
				m_dBerserkerModeIntroTime = 0;
				++m_nBerserkerModeIntroStep;
			}
		}
		else if ( m_nBerserkerModeIntroStep == 1 )
		{
			if ( m_dBerserkerModeIntroTime > 0.7f )
			{
				m_dBerserkerModeIntroTime = 0;
				++m_nBerserkerModeIntroStep;
			}
		}
		else if ( m_nBerserkerModeIntroStep == 2 )
		{
			// power percent
			KDfloat		fPercent = m_dBerserkerModeIntroTime / 0.2f;
			
			// point
			Point		tPoint = Point ( ( _SCR_W - 250 ) + 250 * fPercent, 0 );
			
			//sprite set
			m_pSprCutin1->setPoint ( tPoint );
			m_pSprCutin2->setPoint ( tPoint );
			
			if ( fPercent > 1 )
			{
				m_pSprCutin1->setIsVisible ( false );
				m_pSprCutin2->setIsVisible ( false );
				
				m_dBerserkerModeIntroTime = 0;
				++m_nBerserkerModeIntroStep;
			}
		}
		else if ( m_nBerserkerModeIntroStep == 3 )
		{
			if ( m_dBerserkerModeIntroTime > 0.2 )
			{
				m_pColorLayerIntro->setIsVisible ( false );
				
				m_bIsBerserkerModeIntro = false;
				
				m_pCommander->msgBerserkerModeIntro ( false );
			}
		}
		
		// gauge
		updateGauge ( dLeftTime );

		return;
	}
	
	// berserker gauge
	if ( m_bIsBerserkerMode == true )
	{
		// gauge
		m_fGaugeBerserker -= dLeftTime / 10;
		m_fSeenGaugeBerserker = m_fGaugeBerserker;
		
		// sprite
		KDfloat		fOpacity = m_pSprGaugeBerserker3->getOpacity ( );
		if ( m_bBerserkerModeGaugeOpacityAdd == true )	fOpacity += ( 255 * dLeftTime );
		else											fOpacity -= ( 255 * dLeftTime );
		if ( fOpacity < 155 ) {	m_bBerserkerModeGaugeOpacityAdd = true;	 fOpacity = 155; }
		if ( fOpacity > 255 ) {	m_bBerserkerModeGaugeOpacityAdd = false; fOpacity = 255; }
		m_pSprGaugeBerserker3->setOpacity ( fOpacity );
		
		// check
		if ( m_fGaugeBerserker < 0 )
		{
			// switch
			m_bIsBerserkerMode = false;
			
			// gauge
			m_fGaugeBerserker = 0;
			
			// sprite
			m_pSprGaugeBerserker2->setIsVisible ( true );
			m_pSprGaugeBerserker3->setIsVisible ( false );
			
			// color layer
			m_pColorLayerIntro->setIsVisible ( false );
			
			// commander send msg
			m_pCommander->msgCharAngry ( false );
		}
	}
	
	// time mode
	if ( m_bIsTimeMode == true )
	{
		// gauge
		m_fGaugeTime -= dLeftTime / 10;
		m_fSeenGaugeTime = m_fGaugeTime;

		// sprite
		KDfloat		fOpacity = m_pSprGaugeTime3->getOpacity ( );
		if ( m_bTimeModeGaugeOpacityAdd == true )	fOpacity += ( 255 * dLeftTime );
		else										fOpacity -= ( 255 * dLeftTime );
		if ( fOpacity < 155 ) {	m_bTimeModeGaugeOpacityAdd = true;	fOpacity = 155;	}
		if ( fOpacity > 255 ) {	m_bTimeModeGaugeOpacityAdd = false;	fOpacity = 255;	}
		m_pSprGaugeTime3->setOpacity ( fOpacity );

		// check
		if ( m_fGaugeTime < 0 )
		{
			// switch
			m_bIsTimeMode = false;

			// gauge
			m_fGaugeTime = 0;
			
			// sprite
			m_pSprGaugeTime2->setIsVisible ( true );
			m_pSprGaugeTime3->setIsVisible ( false );
			
			// color layer
			m_pColorLayerIntro->setIsVisible ( false );

			// commander send msg
			m_pCommander->msgSprBackGray ( false );
		}
	}
	
	// gauge
	updateGauge ( dLeftTime );
}

KDvoid cGameMode::updateGauge ( KDdouble dLeftTime )
{
	// berserker gauge
	KDint	nWidth1 = (KDint) ( m_tBerserkerGauge.width * m_fSeenGaugeBerserker );
	KDint	nWidth2 = (KDint) ( m_tBerserkerGauge.width * m_fGaugeBerserker	);
	
	// set texture rect 
	m_pSprGaugeBerserker ->setTextureRect ( Rect ( 0, 0, nWidth1, m_tBerserkerGauge.height ) );
	m_pSprGaugeBerserker2->setTextureRect ( Rect ( 0, 0, nWidth2, m_tBerserkerGauge.height ) );
	m_pSprGaugeBerserker3->setTextureRect ( Rect ( 0, 0, nWidth1, m_tBerserkerGauge.height ) );
	
	// opacity
	m_fSeenGaugeBerserkerOpacity += 255 * ( dLeftTime * 10 );
	if ( m_fSeenGaugeBerserkerOpacity > 255 )	m_pSprGaugeBerserker2->setOpacity ( 255 );
	else										m_pSprGaugeBerserker2->setOpacity ( (KDint) m_fSeenGaugeBerserkerOpacity );
	
	// check
	if ( m_fSeenGaugeBerserkerOpacity > 255 )
	{
		m_fSeenGaugeBerserker = m_fGaugeBerserker;
		m_fSeenGaugeBerserkerOpacity = 255;
	}
	
	// time mode gauge
	KDint	nHeight1 = (KDint) ( m_tTimeGauge.height * m_fSeenGaugeTime );
	KDint	nHeight2 = (KDint) ( m_tTimeGauge.height * m_fGaugeTime	);
	
	// set texture rect
	m_pSprGaugeTime	->setTextureRect ( Rect ( 0, m_tTimeGauge.height - nHeight1, m_tTimeGauge.width, nHeight1 ) );
	m_pSprGaugeTime2->setTextureRect ( Rect ( 0, m_tTimeGauge.height - nHeight2, m_tTimeGauge.width, nHeight2 ) );
	m_pSprGaugeTime3->setTextureRect ( Rect ( 0, m_tTimeGauge.height - nHeight1, m_tTimeGauge.width, nHeight1 ) );
	
	// opacity
	m_fSeenGaugeTimeOpacity += 255 * ( dLeftTime * 10 );
	if ( m_fSeenGaugeTimeOpacity > 255 )	m_pSprGaugeTime2->setOpacity ( 255 );
	else									m_pSprGaugeTime2->setOpacity ( (KDint) m_fSeenGaugeTimeOpacity );
	
	// check
	if ( m_fSeenGaugeTimeOpacity > 255 )
	{
		m_fSeenGaugeTime = m_fGaugeTime;
		m_fSeenGaugeTimeOpacity	= 255;
	}
}
