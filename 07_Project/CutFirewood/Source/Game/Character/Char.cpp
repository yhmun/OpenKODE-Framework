/* -----------------------------------------------------------------------------------
 *
 *      File            Char.cpp
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
#include "Char.h"
#include "Object/Animation/Animations.h"
#include "Object/Sprite/Sprite.h"
#include "Game/Commander/Commander.h"

cChar::cChar ( JBaseScene* pScene, cCommander* pCommander, eType eType, eLookAtType eLookAt, KDint nPlayeriD, Point tPoint )
: m_pScene					( pScene		)
, m_pCommander				( pCommander	)
, m_nPlayerID				( nPlayeriD		)
, m_pAni					( nullptr		)
, m_nNextAni				( -1			)
, m_pSprMaxPowerGaugeBack	( nullptr		)
, m_pSprMaxPowerGauge		( nullptr		)
, m_pAniMaxPower			( nullptr		)
, m_pSprPerfectEffect		( nullptr		)
, m_tPoint					( tPoint		)
, m_bIsPowerGetting			( false			)
, m_bIsMaxPower				( false			)
, m_nPowerLevel				( 0				)
, m_fPower					( 0				)
, m_fPowerGetSpeed			( 1				)
, m_fPowerLv1CutOff			( 0.3f			)
, m_fPowerLv2CutOff			( 0.3f			)
, m_fPowerLv3CutOff			( 0.3f			)
, m_fMaxPowerGauge			( 0				)
, m_bIsMaxPowerGaugeAdd		( true			)
, m_bIsScaleAdd				( false			)
, m_fScaleY					( 0				)
, m_dScaleAddMaxLv2			( 0.015f		)
, m_dScaleAddMaxLv3			( 0.03f			)
, m_bIsAngry				( false			)
, m_fAngryColor				( 0				)
, m_bIsAngryColorAdd		( false			)
{
	// animations
	switch ( eType )
	{
		case eType_nurungee	:	if ( eLookAt == eLookAtType_left )	m_pAni = new cAnimations ( "char_1_L.spr", eLayerGame_Character, m_tPoint );
								else									m_pAni = new cAnimations ( "char_1_R.spr", eLayerGame_Character, m_tPoint );	break;
	}
	
	m_pAni->selectAnimation ( eAniType_nor );
	m_pAni->start ( );
	
	m_pScene->addDrawObject ( m_pAni );

	// max power gauge
	m_pSprMaxPowerGaugeBack	= new cSprite ( "gauge_power_back.png", eLayerGame_MaxPowerGauge, m_tPoint + Point ( -50, 210 ) );
	m_pSprMaxPowerGauge		= new cSprite ( "gauge_power.png"	  , eLayerGame_MaxPowerGauge, m_tPoint + Point ( -46, 212 ) );

	m_pSprMaxPowerGaugeBack->setIsVisible ( false );
	m_pSprMaxPowerGauge	   ->setIsVisible ( false );
	
	m_pScene->addDrawObject ( m_pSprMaxPowerGaugeBack );
	m_pScene->addDrawObject ( m_pSprMaxPowerGauge	  );

	// max power animations
	switch ( eType )
	{
		case eType_nurungee	:	m_pAniMaxPower = new cAnimations ( "maxpower_effect.spr", eLayerGame_MaxPowerEffect, m_tPoint );	break;
	}

	m_pAniMaxPower->setIsVisible ( false );
	m_pAniMaxPower->stop ( );
	
	m_pScene->addDrawObject ( m_pAniMaxPower );
	
	// effect
	m_pSprPerfectEffect	= new cSprite ( "perfect.png", eLayerGame_PercentEffect, Point::ZERO );
	m_pSprPerfectEffect->setIsVisible ( false );
	
	m_pScene->addDrawObject ( m_pSprPerfectEffect );
}

cChar::~cChar ( KDvoid )
{
	// animations
	m_pAni = nullptr;
	
	// effects
	m_aPerfectEffects.clear ( );

	// max power gauge
	m_pSprMaxPowerGaugeBack	= nullptr;
	m_pSprMaxPowerGauge		= nullptr;
	
	// max power animations
	m_pAniMaxPower = nullptr;
	
	// effect
	m_pSprPerfectEffect = nullptr;
}

KDvoid cChar::setPowerGetting ( KDbool bIsGetting )
{
	// getting switch
	m_bIsPowerGetting = bIsGetting;
	
	// power
	setPowerZero ( );
}

KDvoid cChar::setPowerZero ( KDvoid )
{
	// power
	m_fPower	  = 0;
	m_nPowerLevel = 0;
	m_bIsMaxPower = 0;
	
	// max power
	m_fMaxPowerGauge		= 0;
	m_bIsMaxPowerGaugeAdd	= true;

	// max power gauge sprite
	m_pSprMaxPowerGaugeBack->setIsVisible ( false );
	m_pSprMaxPowerGauge	   ->setIsVisible ( false );
}

KDvoid cChar::setAniNor ( KDvoid )
{
	// power
	setPowerZero ( );
	
	// ani
	m_pAni->setScale ( 1 );
	m_pAni->selectAnimation ( eAniType_nor );
	m_pAni->setIsLoop ( true );
	m_pAni->start ( );
}

KDvoid cChar::setAniFinish ( KDvoid )
{
	// power
	setPowerZero ( );
	
	// ani
	m_pAni->setScale ( 1 );
	m_pAni->selectAnimation ( eAniType_finish );
	m_pAni->setIsLoop ( false );
	m_pAni->start ( );
}

KDvoid cChar::setAniMiss ( KDvoid )
{
	// power
	setPowerZero ( );
	
	// ani
	m_pAni->setScale ( 1 );
	m_pAni->selectAnimation ( eAniType_miss );
	m_pAni->setIsLoop ( false );
	m_pAni->start ( );
}

KDvoid cChar::setNextAniNor ( KDvoid )
{
	m_nNextAni = eAniType_nor;
}

KDvoid cChar::addPerfectEffect ( KDvoid )
{
	// new
	cSprite*	pSprite = new cSprite ( "perfect.png", eLayerGame_PercentEffect, Point ( m_tPoint.x - 47, m_tPoint.y + 200 ) );
	pSprite->setTextureRect ( Rect ( 0, 0, 105, 21 ) );
	
	// add
	m_pScene->addDrawObject ( pSprite );
	m_aPerfectEffects.push_back ( pSprite );
}

KDvoid cChar::addMaxPowerEffect ( KDvoid )
{
	// new
	cSprite*	pSprite = new cSprite ( "perfect.png", eLayerGame_PercentEffect, Point ( m_tPoint.x - 50, m_tPoint.y + 200 ) );
	pSprite->setTextureRect( Rect ( 0, 21, 105, 42 ) );
	
	// add
	m_pScene->addDrawObject ( pSprite );
	m_aPerfectEffects.push_back ( pSprite );
}

KDvoid cChar::setIsAngry ( KDbool bIsAngry )
{
	m_bIsAngry		   = bIsAngry;
	m_fAngryColor	   = 255;
	m_bIsAngryColorAdd = false;
}

KDvoid cChar::update ( KDdouble dLeftTime )
{
	// character
	updateChar ( dLeftTime );
	
	// effect
	updateEffect ( dLeftTime );
}

KDvoid cChar::updateChar ( KDdouble dLeftTime )
{
	// animation update
	m_pAni		  ->update ( dLeftTime );
	m_pAniMaxPower->update ( dLeftTime );

	// next animation
	if ( m_nNextAni != -1 )
	{
		if ( m_pAni->getIsPlay ( ) == false )
		{
			if ( m_nNextAni == eAniType_nor )
			{
				m_pAni->setScale ( 1 );
				m_pAni->selectAnimation ( eAniType_nor );
				m_pAni->setIsLoop ( true );
				m_pAni->start ( );
				m_nNextAni = -1;
			}
		}
	}
	
	if ( m_bIsPowerGetting == true )
	{
		// max power
		m_bIsMaxPower = false;
		
		// power
		if ( m_bIsAngry == true )	m_nPowerLevel = 3;
		else						m_fPower += dLeftTime * m_fPowerGetSpeed;
		
		// level
		if ( m_nPowerLevel == 0 )
		{
			// power check
			if ( m_fPower > m_fPowerLv1CutOff )
			{
				// power
				++m_nPowerLevel;
				m_fPower -= m_fPowerLv1CutOff;
				
				// ani
				m_pAni->selectAnimation ( eAniType_step_2 );
			}
			else
			{
				// ani
				m_pAni->selectAnimation ( eAniType_step_1 );
			}			
			
			// scale
			m_fScaleY = 1;
		}
		else if ( m_nPowerLevel == 1 )
		{
			if ( m_fPower > m_fPowerLv2CutOff )
			{
				// power
				++m_nPowerLevel;
				m_fPower -= m_fPowerLv2CutOff;
				
				// power pencent
				KDfloat		fPercent = m_fPower / m_fPowerLv3CutOff;
				
				// scale
				if ( fPercent < 0.5f )	m_fScaleY = 1 + ( fPercent * 2 ) * m_dScaleAddMaxLv2;
				else					m_fScaleY = ( 1 + m_dScaleAddMaxLv2 ) - ( fPercent - 0.5f ) * 2 * m_dScaleAddMaxLv2;
				
				// ani
				m_pAni->selectAnimation ( eAniType_step_3 );
			}
			else
			{
				// scale
				m_fScaleY = 1;
				
				// ani
				m_pAni->selectAnimation ( eAniType_step_2 );
			}
		}
		else if ( m_nPowerLevel == 2 )
		{
			if ( m_fPower > m_fPowerLv3CutOff )
			{
				// power
				++m_nPowerLevel;
				m_fPower -= m_fPowerLv3CutOff;
				
				// switch
				m_bIsScaleAdd = true;
				
				// scale
				m_fScaleY = 1 + m_dScaleAddMaxLv3;
				
				// ani
				m_pAni->selectAnimation ( eAniType_step_4 );
			}
			else
			{
				// power pencent
				KDfloat		fPercent = m_fPower / m_fPowerLv3CutOff;
				
				// scale
				if ( fPercent < 0.5f )	m_fScaleY = 1 + ( fPercent * 2 ) * m_dScaleAddMaxLv2;
				else					m_fScaleY = ( 1 + m_dScaleAddMaxLv2 ) - ( fPercent - 0.5f ) * 2 * m_dScaleAddMaxLv2;
				
				// ani
				m_pAni->selectAnimation ( eAniType_step_3 );
			}
		}
		else if ( m_nPowerLevel == 3 )
		{
			if ( m_bIsScaleAdd == true )
			{
				// scale
				m_fScaleY += ( dLeftTime * 6 ) * m_dScaleAddMaxLv3;
				
				if ( m_fScaleY > 1 + m_dScaleAddMaxLv3 )
				{
					m_bIsScaleAdd = false;
					m_fScaleY = 1 + m_dScaleAddMaxLv3;
				}
			}
			else
			{
				// scale
				m_fScaleY -= ( dLeftTime * 6 ) * m_dScaleAddMaxLv3;
				
				if ( m_fScaleY < 1 )
				{
					m_bIsScaleAdd = true;
					m_fScaleY = 1;
				}
			}
			
			// max power
			m_pSprMaxPowerGaugeBack->setIsVisible ( true );
			m_pSprMaxPowerGauge	   ->setIsVisible ( true );
			
			if ( m_bIsMaxPowerGaugeAdd == true )	m_fMaxPowerGauge += dLeftTime * 3;
			else									m_fMaxPowerGauge -= dLeftTime * 3;
			
			if ( m_fMaxPowerGauge > 1 )	{ m_fMaxPowerGauge = 1;	m_bIsMaxPowerGaugeAdd = false; }
			if ( m_fMaxPowerGauge < 0 )	{ m_fMaxPowerGauge = 0;	m_bIsMaxPowerGaugeAdd = true;  }
			
			m_pSprMaxPowerGauge->setPoint ( m_tPoint + Point ( -46 + m_fMaxPowerGauge * 88, 212 ) );
			
			if ( m_fMaxPowerGauge > 0.8f )
			{
				m_bIsMaxPower = true;
			}
			
			// ani
			m_pAni->selectAnimation ( eAniType_step_4 );
		}
		
		// scale
		m_pAni->setScaleY ( m_fScaleY );
	}
	
	// angry
	if ( m_bIsAngry == true )
	{
		if ( m_bIsAngryColorAdd == false )	m_fAngryColor -= 255 * ( dLeftTime / 2 );
		else								m_fAngryColor += 255 * ( dLeftTime / 2 );
		
		if ( m_fAngryColor < 150 ) { m_fAngryColor = 150; m_bIsAngryColorAdd = true;  }
		if ( m_fAngryColor > 200 ) { m_fAngryColor = 200; m_bIsAngryColorAdd = false; }
		
		m_pAni->setColor ( 255, m_fAngryColor, m_fAngryColor );
	}
	else
	{
		m_pAni->setColor ( 255, 255, 255 );
	}
}

KDvoid cChar::updateEffect ( KDdouble dLeftTime )
{
	// loop
	std::vector<cSprite*>::iterator		pNode = m_aPerfectEffects.begin ( );
	while ( pNode != m_aPerfectEffects.end ( ) )
	{
		// init
		cSprite*	pSprite = *pNode;
		
		// point
		Point		tPoint = pSprite->getPoint ( );
		tPoint.y += dLeftTime * 20;
		pSprite->setPoint ( tPoint );
		
		// opacity
		KDint		nOpacity = pSprite->getOpacity ( );
		nOpacity -= dLeftTime * 255;
		pSprite->setOpacity ( nOpacity );
		
		// next
		if ( nOpacity < 0 )	
		{
			pNode = m_aPerfectEffects.erase ( pNode );
		}
		else
		{
			++pNode;
		}
	}
}
