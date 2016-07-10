/* -----------------------------------------------------------------------------------
 *
 *      File            Firewood.cpp
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
#include "Firewood.h"
#include "Object/Sprite/Sprite.h"
#include "Game/Commander/Commander.h"

//-----------------------------------------------------------------------------------------------------//
//------------------------------------------cFirewoodPreview-------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

cFirewoodPreview::cFirewoodPreview ( JBaseScene* pScene, Point tStart, Point tEnd )
: m_nLevel			( -1		)
, m_tStart			( tStart	)
, m_tEnd			( tEnd		)
, m_bIsThrowing		( false		)
, m_dThrowingTime	( 0			)
, m_dThrowTime		( 0.2f		)
{
	for ( KDint i = 0; i < 3; i++ )
	{		
		// new
		cSprite*	pSprFull = new cSprite ( ccszf ( "firewood_%d_1.png", i + 1 ), eLayerGame_Firewood_3, Point ( 0, 0 ) );
		
		// anchor point
		pSprFull->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
		
		// visible
		pSprFull->setIsVisible ( false );
		
		// add draw
		pScene->addDrawObject ( pSprFull );
		
		// add
		m_aSprFulls.push_back ( pSprFull );
	}
}

cFirewoodPreview::~cFirewoodPreview ( KDvoid )
{
	m_aSprFulls.clear ( );
}

KDvoid cFirewoodPreview::throwing ( KDint nLevel )
{
	// visible off
	setOff ( );
	
	// throw
	m_bIsThrowing	= true;
	m_dThrowingTime	= 0;
	
	// level
	m_nLevel = nLevel;
	
	// visible
	m_aSprFulls [ m_nLevel ]->setIsVisible ( true );
	
	// point
	m_aSprFulls [ m_nLevel ]->setPoint ( m_tStart );
	
	// rotation
	m_aSprFulls [ m_nLevel ]->setRotation ( -270 );
}

KDvoid cFirewoodPreview::standby ( KDint nLevel )
{
	// visible off
	setOff ( );
	
	// throw
	m_bIsThrowing	= false;
	m_dThrowingTime	= 0;
	
	// level
	m_nLevel = nLevel;
	
	// visible
	m_aSprFulls [ m_nLevel ]->setIsVisible ( true );

	// tex width half
	KDfloat		fWidthHalf = m_aSprFulls [ m_nLevel ]->getWidth ( ) / 2 + 0.5f;

	// point
	m_aSprFulls [ m_nLevel ]->setPoint ( Point ( m_tEnd.x, m_tEnd.y + fWidthHalf ) );
	
	// rotate
	m_aSprFulls [ m_nLevel ]->setRotation ( 90 );
}

KDvoid cFirewoodPreview::setOff ( KDvoid )
{
	// visible
	for ( auto pSpr : m_aSprFulls )
	{
		pSpr->setIsVisible ( false );

	}
	
	// throw
	m_bIsThrowing = false;
}

KDvoid cFirewoodPreview::update ( KDdouble dLeftTime, KDbool& bIsEnd )
{
	// throw
	if ( m_bIsThrowing == true )
	{
		// time
		m_dThrowingTime += dLeftTime;
		
		// arrive check
		if ( m_dThrowingTime > m_dThrowTime )
		{
			// standby
			standby ( m_nLevel );
			
			// end
			bIsEnd = true;
		}
		else
		{
			// percent
			KDfloat		fPercent = m_dThrowingTime / m_dThrowTime;
			
			// tex width half
			KDfloat		fWidthHalf = m_aSprFulls [ m_nLevel ]->getWidth ( ) / 2 + 0.5f;
			
			// point
			m_aSprFulls [ m_nLevel ]->setPoint ( Point ( m_tStart.x + ( m_tEnd.x - m_tStart.x ) * fPercent, m_tStart.y + ( ( m_tEnd.y + fWidthHalf ) - m_tStart.y ) * fPercent ) );
			
			// rotation
			m_aSprFulls [ m_nLevel ]->setRotation ( -270 + 360 * fPercent );
			
			// end
			bIsEnd = false;
		}
	}
}

//-----------------------------------------------------------------------------------------------------//
//------------------------------------------cFirewoodStandBy-------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

cFirewoodStandBy::cFirewoodStandBy ( JBaseScene* pScene, Point tStart, Point tEnd, KDfloat fThrowHeight )
: m_nLevel			( -1			)
, m_tStart			( tStart		)
, m_tEnd			( tEnd			)
, m_fThrowHeight	( fThrowHeight	)
, m_bIsStandBy		( false			)
, m_bIsThrowing		( false			)
, m_dThrowingTime	( 0				)
, m_dThrowTime		( 0.2f			)
, m_nDamage			( 0				)
{
	for ( KDint i = 0; i < 3; i++ )
	{		
		// new
		cSprite*	pSprFull  = new cSprite ( ccszf ( "firewood_%d_1.png", i + 1 ), eLayerGame_Firewood_3, Point ( 0, 0 ) );
		cSprite*	pSprLeft  = new cSprite ( ccszf ( "firewood_%d_4.png", i + 1 ), eLayerGame_Firewood_2, Point ( 0, 0 ) );
		cSprite*	pSprRight = new cSprite ( ccszf ( "firewood_%d_5.png", i + 1 ), eLayerGame_Firewood_3, Point ( 0, 0 ) );

		// anchor point
		pSprFull->setAnchorPoint ( Point ( 0.5f, 0.5f ) );

		// visible
		pSprFull ->setIsVisible ( false );
		pSprLeft ->setIsVisible ( false );
		pSprRight->setIsVisible ( false );
		
		// add draw
		pScene->addDrawObject ( pSprFull  );
		pScene->addDrawObject ( pSprLeft  );
		pScene->addDrawObject ( pSprRight );
		
		// add
		m_aSprFulls .push_back ( pSprFull  );
		m_aSprLefts .push_back ( pSprLeft  );
		m_aSprRights.push_back ( pSprRight );
	}
}

cFirewoodStandBy::~cFirewoodStandBy ( KDvoid )
{
	m_aSprFulls .clear ( );
	m_aSprLefts .clear ( );
	m_aSprRights.clear ( );
}

KDvoid cFirewoodStandBy::throwing ( KDint nLevel )
{
	// visible off
	setOff ( );
	
	//throw
	m_bIsThrowing	= true;
	m_dThrowingTime	= 0;
	
	// standby
	m_bIsStandBy = false;

	// level
	m_nLevel = nLevel;

	// damage
	switch ( m_nLevel )
	{
		case 0	:	m_nDamage = 1;	break;
		case 1	:	m_nDamage = 3;	break;
		case 2	:	m_nDamage = 4;	break;
	}
	
	// visible
	m_aSprFulls [ m_nLevel ]->setIsVisible ( true );

	// point
	m_aSprFulls [ m_nLevel ]->setPoint ( Point ( m_tStart.x, m_tStart.y ) );
	
	// rotation
	m_aSprFulls [ m_nLevel ]->setRotation ( 90 );
}

KDvoid cFirewoodStandBy::setOff ( KDvoid )
{
	// visible
	for ( auto pSpr : m_aSprFulls  )	pSpr->setIsVisible ( false );
	for ( auto pSpr : m_aSprLefts  )	pSpr->setIsVisible ( false );
	for ( auto pSpr : m_aSprRights )	pSpr->setIsVisible ( false );
	
	// throw
	m_bIsThrowing = false;

	// isStandBy
	m_bIsStandBy = false;
}

KDvoid cFirewoodStandBy::damage ( KDvoid )
{
	m_nDamage -= 1;
}

KDvoid cFirewoodStandBy::update ( KDdouble dLeftTime )
{
	// throw
	if ( m_bIsThrowing == true )
	{
		// time
		m_dThrowingTime += dLeftTime;
		
		// arrive check
		if ( m_dThrowingTime > m_dThrowTime )
		{
			// throw
			m_bIsThrowing	= false;
			m_dThrowingTime	= 0;

			// visible
			m_aSprFulls  [ m_nLevel ]->setIsVisible ( false );
			m_aSprLefts  [ m_nLevel ]->setIsVisible ( true );
			m_aSprRights [ m_nLevel ]->setIsVisible ( true );
			
			// point
			switch ( m_nLevel )
			{
				case 0	:	m_aSprLefts [ 0 ]->setPoint ( Point ( m_tEnd.x - 14.5f, m_tEnd.y ) );	m_aSprRights [ 0 ]->setPoint ( Point ( m_tEnd.x - 14.5f, m_tEnd.y ) );	break;
				case 1	:	m_aSprLefts [ 1 ]->setPoint ( Point ( m_tEnd.x - 19.5f, m_tEnd.y ) );	m_aSprRights [ 1 ]->setPoint ( Point ( m_tEnd.x - 19.5f, m_tEnd.y ) );	break;
				case 2	:	m_aSprLefts [ 2 ]->setPoint ( Point ( m_tEnd.x - 29.5f, m_tEnd.y ) );	m_aSprRights [ 2 ]->setPoint ( Point ( m_tEnd.x - 29.5f, m_tEnd.y ) );	break;
			}
			
			// rotate
			m_aSprLefts  [ m_nLevel ]->setRotation ( 0 );
			m_aSprRights [ m_nLevel ]->setRotation ( 0 );
			
			// stand by switch
			m_bIsStandBy = true;
		}
		else
		{
			// percent
			KDfloat		fPercent = m_dThrowingTime / m_dThrowTime;

			// texture height half
			KDfloat		fTexHeightHalf = m_aSprFulls [ m_nLevel ]->getHeight ( ) / 2;
			
			// height
			KDfloat		fHeight = 0;
			if ( fPercent < 0.5f )	fHeight = m_fThrowHeight * ( fPercent * 2 );
			else					fHeight = m_fThrowHeight - m_fThrowHeight * ( ( fPercent - 0.5f ) * 2 );
			
			// point
			m_aSprFulls [ m_nLevel ]->setPoint ( Point ( m_tStart.x + ( m_tEnd.x - m_tStart.x ) * fPercent, m_tStart.y + ( ( m_tEnd.y + fTexHeightHalf ) - m_tStart.y ) * fPercent + fHeight ) );
			
			// rotation
			m_aSprFulls [ m_nLevel ]->setRotation ( 90 + 270 * fPercent );
		}
	}
}

cFirewoodProduct::cFirewoodProduct ( JBaseScene* pScene, KDint nLevel, Point tStart, Point tEnd )
: m_pScene				( pScene		)
, m_pSprLeft			( nullptr		)
, m_pSprRight			( nullptr		)
, m_nLevel				( nLevel		)
, m_tLeftStart			( Point::ZERO	)
, m_tLeftEnd			( Point::ZERO	)
, m_tRightStart			( Point::ZERO	)
, m_tRightEnd			( Point::ZERO	)
, m_fHeightLeft			( 0				)
, m_fHeightRight		( 0				)
, m_dThrowingTime		( 0				)
, m_dThrowTime			( 0.5f			)
, m_fRotationLeft		( 0				)
, m_fRotationRight		( 0				)
, m_fRotationLeftSpeed	( 0				)
, m_fRotationRightSpeed	( 0				)
, m_bIsChangeZorder		( false			)
{
	// level check
	CCAssert ( m_nLevel >= 0 && m_nLevel <= 2, "" );
	
	// point
	switch ( m_nLevel )
	{
		case 0	:	m_tLeftStart = Point ( tStart.x, tStart.y + 29.5f );	m_tRightStart = Point ( tStart.x +	  4, tStart.y + 27.5f );	break;
		case 1	:	m_tLeftStart = Point ( tStart.x, tStart.y + 32	  );	m_tRightStart = Point ( tStart.x + 6.5f, tStart.y + 31	  );	break;
		case 2	:	m_tLeftStart = Point ( tStart.x, tStart.y + 35	  );	m_tRightStart = Point ( tStart.x + 0.5f, tStart.y + 31.5f );	break;
	}
	
	m_tLeftEnd	= Point ( ( tEnd.x - 60 ) + kdRand ( ) % 121, ( tEnd.y - 10 ) + kdRand ( ) % 21 );
	m_tRightEnd = Point ( ( tEnd.x - 60 ) + kdRand ( ) % 121, ( tEnd.y - 10 ) + kdRand ( ) % 21 );
	
	// sprite	
	m_pSprLeft	= new cSprite ( ccszf ( "firewood_%d_2.png", m_nLevel + 1 ), eLayerGame_Firewood_2, m_tLeftStart  );
	m_pSprRight	= new cSprite ( ccszf ( "firewood_%d_3.png", m_nLevel + 1 ), eLayerGame_Firewood_3, m_tRightStart );
	
	m_pSprLeft ->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	m_pSprRight->setAnchorPoint ( Point ( 0.5f, 0.5f ) );

	// height
	m_fHeightLeft  = 130 + ( kdRand ( ) % 40 );
	m_fHeightRight = 130 + ( kdRand ( ) % 40 );
	
	// rotation
	m_fRotationLeftSpeed  = 700 + ( kdRand ( ) % 500 );
	m_fRotationRightSpeed = 700 + ( kdRand ( ) % 500 );
	
	if ( kdRand ( ) % 2 == 0 )	m_fRotationLeftSpeed  = -m_fRotationLeftSpeed;
	if ( kdRand ( ) % 2 == 0 )	m_fRotationRightSpeed = -m_fRotationRightSpeed;
	
	// add
	pScene->addDrawObject ( m_pSprLeft	 );
	pScene->addDrawObject ( m_pSprRight	 );
}

cFirewoodProduct::~cFirewoodProduct ( KDvoid )
{
	m_pSprLeft	= nullptr;
	m_pSprRight	= nullptr;
}

KDvoid cFirewoodProduct::update ( KDdouble dLeftTime, KDbool& bIsEnd )
{
	// time
	m_dThrowingTime += dLeftTime;
	
	// arrive check
	if ( m_dThrowingTime > m_dThrowTime )
	{
		m_pSprLeft ->setIsVisible ( false );
		m_pSprRight->setIsVisible ( false );
		
		// stand by switch
		bIsEnd = true;
	}
	else
	{
		// percent
		KDfloat		fPercent = m_dThrowingTime / m_dThrowTime;
		
		// height
		KDfloat		fHeightLeft  = 0;
		KDfloat		fHeightRight = 0;
		if ( fPercent < 0.5f )
		{
			fHeightLeft  = m_fHeightLeft  * ( fPercent * 2 );
			fHeightRight = m_fHeightRight * ( fPercent * 2 );
		}
		else
		{
			fHeightLeft  = m_fHeightLeft  - m_fHeightLeft  * ( ( fPercent - 0.5f ) * 2 );
			fHeightRight = m_fHeightRight - m_fHeightRight * ( ( fPercent - 0.5f ) * 2 );
		}
		
		// point
		m_pSprLeft ->setPoint ( Point ( m_tLeftStart .x + ( m_tLeftEnd .x-m_tLeftStart .x ) * fPercent, m_tLeftStart .y + ( m_tLeftEnd .y - m_tLeftStart .y ) * fPercent + fHeightLeft  ) );
		m_pSprRight->setPoint ( Point ( m_tRightStart.x + ( m_tRightEnd.x-m_tRightStart.x ) * fPercent, m_tRightStart.y + ( m_tRightEnd.y - m_tRightStart.y ) * fPercent + fHeightRight ) );
		
		// rotation
		m_fRotationLeft  += m_fRotationLeftSpeed  * dLeftTime;
		m_fRotationRight += m_fRotationRightSpeed * dLeftTime;

		m_pSprLeft ->setRotation ( m_fRotationLeft	);
		m_pSprRight->setRotation ( m_fRotationRight	);
		
		// z order
		if ( m_bIsChangeZorder == false && fPercent > 0.5f )
		{
			m_pScene->setDrawObjZorder ( m_pSprLeft , eLayerGame_Firewood_1 );
			m_pScene->setDrawObjZorder ( m_pSprRight, eLayerGame_Firewood_1 );
		}
	}
}

//-----------------------------------------------------------------------------------------------------//
//---------------------------------------------cFirewoodMgr--------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

cFirewoodMgr::cFirewoodMgr ( JBaseScene* pScene, cCommander* pCommander, KDint nPlayerID, Point tIntro, Point tPreview, Point tStandBy, Point tProduct )
: m_pScene				( pScene		)
, m_pCommander			( pCommander	)
, m_nPlayerID			( nPlayerID		)
, m_pFirewoodPreview	( nullptr		)
, m_pFirewoodStandby	( nullptr		)
, m_tIntro				( tIntro		)
, m_tPreview			( tPreview		)
, m_tStandBy			( tStandBy		)
, m_tProduct			( tProduct		)
{
	m_pFirewoodPreview = new cFirewoodPreview ( pScene, m_tIntro  , m_tPreview	   );
	m_pFirewoodStandby = new cFirewoodStandBy ( pScene, m_tPreview, m_tStandBy, 60 );
}

cFirewoodMgr::~cFirewoodMgr ( KDvoid )
{
	// firewood
	m_pFirewoodPreview = nullptr;
	m_pFirewoodStandby = nullptr;

	m_aFirewoodProducts.clear ( );
}

KDvoid cFirewoodMgr::firstSetting ( KDbool bIsNewPreview )
{
	// standby
	m_pFirewoodStandby->setOff ( );

	// preview
	if ( bIsNewPreview == true )
	{
		KDint	nRand = kdRand ( ) % 8;
		KDint	nLevel = 0;
			 if ( nRand < 5 )	nLevel = 0;
		else if ( nRand < 7 )	nLevel = 1;
		else					nLevel = 2;
		
		// preview standby
		m_pFirewoodPreview->standby ( nLevel );
	}
}

KDvoid cFirewoodMgr::request ( KDvoid )
{
	// standby
	m_pFirewoodStandby->throwing ( m_pFirewoodPreview->getLevel ( ) );

	// level
	KDint	nRand = kdRand ( ) % 8;
	KDint	nLevel = 0;
		 if ( nRand < 5 )	nLevel = 0;
	else if ( nRand < 7 )	nLevel = 1;
	else					nLevel = 2;
	
	// preview
	m_pFirewoodPreview->throwing ( nLevel );
}

KDvoid cFirewoodMgr::splitting ( KDint nPowerLv, KDbool& bIsSuccess, KDint& nFirewoodLv )
{
	// check
	if ( getIsStandBy ( ) == false )
	{
		return;
	}

	// init
	nFirewoodLv = m_pFirewoodStandby->getLevel ( );
	
	// damage
	m_pFirewoodStandby->damage ( );
	
	// succes check
	if ( nFirewoodLv == 0 || ( nFirewoodLv == 1 && nPowerLv >= 1 ) || ( nFirewoodLv == 2 && nPowerLv >= 3 ) )
	{
		bIsSuccess = true;
	}
	
	// damage 
	if ( bIsSuccess == true || m_pFirewoodStandby->getDamage ( ) == 0 )
	{
		// standby firewood off
		m_pFirewoodStandby->setOff ( );
		
		// add product firewood
		m_aFirewoodProducts.push_back ( new cFirewoodProduct ( m_pScene, nFirewoodLv, m_tStandBy, m_tProduct ) );
		
		// success
		bIsSuccess = true;
	}
}

KDbool cFirewoodMgr::getIsStandByThrowing ( KDvoid )
{
	return m_pFirewoodStandby->getIsTorhwing ( );
}

KDbool cFirewoodMgr::getIsStandBy ( KDvoid )
{
	return m_pFirewoodStandby->getIsStandBy ( );
}

KDint cFirewoodMgr::getStandbyLevel ( KDvoid )
{
	return m_pFirewoodStandby->getLevel ( );
}

KDvoid cFirewoodMgr::update ( KDdouble dLeftTime )
{
	// firewood preview
	KDbool		bIsPreviewEnd = false;
	m_pFirewoodPreview->update ( dLeftTime, bIsPreviewEnd );
	
	// send message preview standby
	if ( bIsPreviewEnd == true )
	{
		m_pCommander->msgFirewoodPreviewStandby ( m_nPlayerID );
	}

	// firewood standby
	m_pFirewoodStandby->update ( dLeftTime );
	
	// firewood Product
	std::vector<cFirewoodProduct*>::iterator	pNode = m_aFirewoodProducts.begin ( );
	while ( pNode != m_aFirewoodProducts.end ( ) )
	{
		// init
		KDbool		bIsEnd = false;
		
		// update
		( *pNode )->update ( dLeftTime, bIsEnd );
		
		// end
		if ( bIsEnd == true )
		{
			// send message
			m_pCommander->msgFirewoodProduct ( m_nPlayerID, ( *pNode )->getLevel ( ) );
			m_pCommander->msgCloudAdd ( eLayerGame_Firewood_Cloud, 0, ( *pNode )->getPointLeftEnd  ( ), Point::ZERO, 0.4f, 1, -1, kdRand ( ) % 360, -360 + 120 + kdRand ( ) % 600, false );
			m_pCommander->msgCloudAdd ( eLayerGame_Firewood_Cloud, 0, ( *pNode )->getPointRightEnd ( ), Point::ZERO, 0.4f, 1, -1, kdRand ( ) % 360, -360 + 120 + kdRand ( ) % 600, false );
		}
		
		// next
		if ( bIsEnd == false )	++pNode;
		else					pNode = m_aFirewoodProducts.erase ( pNode );
	}
}
