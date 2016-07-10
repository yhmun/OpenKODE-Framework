/* -----------------------------------------------------------------------------------
 *
 *      File            SprEffects.cpp
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
#include "SprEffects.h"
#include "MyData/MyData.h"

#define _ANI_NAME_MAX		18

cSprEffect::cSprEffect ( Node* pNode, const KDchar* pTexFileName, std::string sName, KDint z, KDint nAniMax )
: m_pNode ( pNode )
, m_sName ( sName )
{
	for ( KDint i = 0; i < nAniMax; i++ )
	{
		cSprAni*	pAni = new cSprAni ( m_pNode, pTexFileName, z, Point ( 0, 0 ) );

		pAni->setVisible ( false );
		pAni->stop ( );

		m_aAnis.push_back ( pAni );
	}
}

cSprEffect::~cSprEffect ( KDvoid )
{
	// anis
	for ( auto pNode : m_aAnis )	
	{
		CC_SAFE_RELEASE ( pNode );
	}
	m_aAnis.clear ( );
}

KDvoid cSprEffect::addFrame ( Rect tRectSrc, KDfloat fDelay, Point tCenter )
{
	for ( auto pNode : m_aAnis )	
	{
		pNode->addFrame ( tRectSrc, fDelay, false, false, tCenter, Rect::ZERO, Rect::ZERO );
	}
}

KDvoid cSprEffect::view ( Point tPoint )
{
	for ( auto pNode : m_aAnis )	
	{		
		if ( pNode->getSprite ( )->get ( )->isVisible ( ) == false)
		{
			pNode->setVisible ( true );
			pNode->setPoint ( tPoint );
			pNode->setFrameInit ( 0 );
			pNode->start ( );
			break;
		}
	}
}

KDvoid cSprEffect::viewReservation ( Point tPoint, KDdouble dLeftTime, std::string sSoundEffect )
{
	sReservationEffect2		tReservation;

	tReservation.tPoint = tPoint;
	tReservation.dLeftTime = dLeftTime;
	tReservation.sSoundEffect = sSoundEffect;

	m_aReservations.push_back ( tReservation );
}

KDbool cSprEffect::isView ( KDvoid )
{
	for ( auto pNode : m_aAnis )	
	{
		if ( pNode->getSprite ( )->get ( )->isVisible ( ) == true )
		{
			return true;
		}
	}

	if ( m_aReservations.size ( ) > 0 )
	{
		return true;
	}

	return false;
}

KDvoid cSprEffect::update ( KDdouble dLeftTickCount )
{
	std::vector<sReservationEffect2>::iterator	pNode = m_aReservations.begin ( );
	while ( pNode != m_aReservations.end ( ) )
	{
		sReservationEffect2&	rReservation = *pNode;

		rReservation.dLeftTime -= dLeftTickCount;

		if ( rReservation.dLeftTime < 0 )
		{
			if ( rReservation.sSoundEffect.size ( ) > 0 )
			{
				if ( cMyData::getObj ( )->m_bIsOptionSound != false )
				{
					SimpleAudioEngine::getInstance ( )->playEffect ( rReservation.sSoundEffect.c_str ( ) );
				}
			}

			view ( rReservation.tPoint );

			pNode = m_aReservations.erase ( pNode );
		}
		else
		{
			++pNode;
		}
	}

	for ( auto pNode : m_aAnis )	
	{
		if ( pNode->update ( dLeftTickCount ) == true )
		{
			pNode->setVisible ( false );
			pNode->stop ( );
		}
	}
}

KDvoid cSprEffect::setScale ( KDfloat fScaleX, KDfloat fScaleY )
{
	for ( auto pNode : m_aAnis )	
	{
		pNode->setScale ( fScaleX, fScaleY );
	}
}

KDvoid cSprEffect::setAnchorPoint ( Point tPoint )
{
	for ( auto pNode : m_aAnis )	
	{
		pNode->anchorPoint ( tPoint );
	}
}

KDvoid cSprEffect::setAlpha ( KDubyte cAlpha )
{
	for ( auto pNode : m_aAnis )	
	{
		pNode->setAlpha ( cAlpha );
	}
}

KDvoid cSprEffect::setColor ( KDint r, KDint g, KDint b )
{
	for ( auto pNode : m_aAnis )	
	{
		pNode->setColor ( r, g, b );
	}
}



cSprEffectFinish::cSprEffectFinish ( Node* pNode, const KDchar* pTexPath, KDint z )
: m_pNode		( pNode			)
, m_pAni		( nullptr		)
, m_nStep		( -1			)
, m_dLeftTime	( 0				)
, m_tPoint			( Point::ZERO	)
{
	m_pAni = new cSprAni ( m_pNode, pTexPath, z, Point ( 0, 0 ) );

	m_pAni->setVisible ( false );
	m_pAni->setLoop ( false );
	m_pAni->stop ( );
}

cSprEffectFinish::~cSprEffectFinish ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pAni );
}

KDvoid cSprEffectFinish::addFrame ( Rect tRectSrc, KDfloat fDelay, Point tCenter )
{
	m_pAni->addFrame ( tRectSrc, fDelay, false, false, tCenter, Rect::ZERO, Rect::ZERO );
}

KDbool cSprEffectFinish::view ( Point tPoint, KDdouble dWaitTime ) 
{
	if ( m_nStep != -1 )	
	{
		return false;
	}

	if ( dWaitTime > 0 )
	{
		m_nStep		= 0;
		m_dLeftTime	= dWaitTime;
		m_tPoint	= tPoint;
	}
	else
	{
		m_pAni->setVisible ( true );
		m_pAni->setPoint ( tPoint );
		m_pAni->anchorPoint ( Point ( 0.5f, 0.5f ) );
		m_pAni->setFrameInit ( 0 );
		m_pAni->setAlpha ( 0 );
		m_pAni->getSprite ( )->get ( )->setRotation ( 0 );
		m_pAni->stop ( );

		m_nStep		= 1;
		m_dLeftTime	= 0.1f;
	}

	return true;
}

KDbool cSprEffectFinish::isView ( KDvoid )
{
	if ( m_nStep == -1 )	return false;
	else					return true;
}

KDvoid cSprEffectFinish::update ( KDdouble dLeftTickCount )
{
	if ( m_nStep == -1 )
	{
		return;
	}

	m_dLeftTime -= dLeftTickCount;

	if ( m_nStep == 0 )
	{
		if ( m_dLeftTime < 0 )
		{
			m_pAni->setVisible ( true );
			m_pAni->setPoint ( m_tPoint );
			m_pAni->anchorPoint ( Point ( 0.5f, 0.5f ) );
			m_pAni->setFrameInit ( 0 );
			m_pAni->setAlpha ( 0 );
			m_pAni->getSprite ( )->get ( )->setRotation ( 0 );
			m_pAni->stop ( );

			++m_nStep;
			m_dLeftTime = 0.1f;
		}
	}
	else if ( m_nStep == 1 )
	{
		if ( m_dLeftTime < 0 )
		{
			m_pAni->setAlpha ( 120 );
			m_dLeftTime = 0.1f;
			++m_nStep;
		}
		else
		{
			m_pAni->setAlpha ( 120 * ( 0.1f - m_dLeftTime ) / 0.1f );
		}
	}
	else if ( m_nStep == 2 )
	{
		if ( m_dLeftTime < 0 )
		{
			m_pAni->getSprite ( )->get ( )->setRotation ( 0 );
			m_dLeftTime = 0.05f;
			m_pAni->setAlpha ( 120 );
			m_pAni->anchorPoint ( Point  ( 0.5f, 0.5f ) );
			m_pAni->setFrameInit ( 1 );
			++m_nStep;
		}
		else
		{
			m_pAni->getSprite ( )->get ( )->setRotation ( 90 * ( 0.1f - m_dLeftTime ) / 0.1f );
			m_pAni->setAlpha ( 120 * ( 0.1f - m_dLeftTime ) / 0.1f );
		}
	}
	else if ( m_nStep == 3 )
	{
		if ( m_dLeftTime < 0 )
		{
			m_dLeftTime = 0.05f;
			m_pAni->setAlpha ( 120 );
			m_pAni->anchorPoint ( Point ( 0.5f, 0.5f ) );
			m_pAni->setFrameInit ( 2 );
			++m_nStep;
		}
	}
	else if ( m_nStep == 4 )
	{
		if ( m_dLeftTime < 0 )
		{
			m_pAni->setVisible ( false );
			m_nStep = -1;
		}
	}
}


cSprEffects::cSprEffects ( Node* pNode, KDint nZ1, KDint nZ2 )
: m_pNode ( pNode )
{
	// path
	KDubyte*	pData = nullptr;
	KDint		nSize = 0;
	KDint		nSeek = 0;

	pData = FileUtils::getInstance ( )->getFileData ( "Effect/effects.spr", "rb", &nSize );

	// ani count
	KDint		nAniCount = 0;
	cMyData::readToData ( pData, nSize, nSeek, &nAniCount, sizeof ( KDint ) );

	for ( KDint i = 0; i < nAniCount; i++ )
	{
		// ani name
		KDint		nAniNameCount = 0;
		KDchar		szAniName [ _ANI_NAME_MAX + 1 ] = { 0, };
		cMyData::readToData ( pData, nSize, nSeek, &nAniNameCount, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szAniName	 , sizeof ( KDchar ) * nAniNameCount );

		// imageName
		KDint		nImageNameSize = 0;
		KDchar		szImageName [ 260 ] = { 0, };
		cMyData::readToData ( pData, nSize, nSeek, &nImageNameSize, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szImageName	  , sizeof ( KDchar ) * nImageNameSize );

		std::string	sImageName = "Effect/" + std::string ( szImageName );

		// cSprAni new
		cSprEffect*		pEffect = nullptr;
		switch ( i )
		{
			case 0	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 40 );	break;
			case 1	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 22 );	break;
			case 2	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 16 );	break;
			case 3	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 10 );	break;
			case 4	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 12 );	break;
			case 5	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 10 );	break;
			case 6	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 10 );	break;
			case 7	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 10 );	break;
			case 8	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 10 );	break;
			case 9	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 10 );	break;
			case 11	:	pEffect = new cSprEffect ( m_pNode, sImageName.c_str ( ), szAniName, nZ2, 10 );	break;
		}

		if ( i == 10 )
		{
			for ( KDint a = 0; a < 2; a++ )
			{
				cSprEffectFinish*	pEffectFinish = new cSprEffectFinish ( m_pNode, sImageName.c_str ( ), nZ1 );

				m_aEffectFinishs.push_back ( pEffectFinish );
			}
		}

		// frame
		KDint		nFrameCount = 0;
		cMyData::readToData ( pData, nSize, nSeek, &nFrameCount, sizeof ( KDint ) );

		for ( KDint a = 0; a < nFrameCount; a++ )
		{
			// src Rect
			Rect		tRectSrc = Rect::ZERO;
			cMyData::readToData ( pData, nSize, nSeek, &tRectSrc.origin.x, sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectSrc.origin.y, sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectSrc.size.cx , sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectSrc.size.cy , sizeof ( KDfloat ) );

			// delay
			KDfloat		fDelay = 0;
			cMyData::readToData ( pData, nSize, nSeek, &fDelay, sizeof ( KDfloat ) );

			// center point
			Point		tCenter = Point::ZERO;
			cMyData::readToData ( pData, nSize, nSeek, &tCenter.x, sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tCenter.y, sizeof ( KDfloat ) );

			// att, def switch
			KDchar		bAtt = 0;
			KDchar		bDef = 0;
			cMyData::readToData ( pData, nSize, nSeek, &bAtt, sizeof ( KDchar ) );
			cMyData::readToData ( pData, nSize, nSeek, &bDef, sizeof ( KDchar ) );

			// att Rect
			Rect		tRectAtt = Rect::ZERO;
			cMyData::readToData ( pData, nSize, nSeek, &tRectAtt.origin.x, sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectAtt.origin.y, sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectAtt.size.cx , sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectAtt.size.cy , sizeof ( KDfloat ) );

			// def Rect
			Rect		tRectDef = Rect::ZERO;
			cMyData::readToData ( pData, nSize, nSeek, &tRectDef.origin.x, sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectDef.origin.y, sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectDef.size.cx , sizeof ( KDfloat ) );
			cMyData::readToData ( pData, nSize, nSeek, &tRectDef.size.cy , sizeof ( KDfloat ) );

			// add
			if ( i == 10 )
			{
				for ( auto pNode : m_aEffectFinishs )
				{
					pNode->addFrame ( tRectSrc, fDelay, tCenter );
				}
			}
			else
			{
				pEffect->addFrame ( tRectSrc, fDelay, tCenter );
			}
		}

		if ( i != 10 )
		{
			m_aEffects.push_back ( pEffect );
		}
	}

	// release
	CC_SAFE_FREE ( pData );
}

cSprEffects::~cSprEffects ( KDvoid )
{
	// effects
	SAFE_RELEASE_ARRAYS ( m_aEffects );

	// effect finishes
	SAFE_RELEASE_ARRAYS ( m_aEffectFinishs );
}

KDvoid cSprEffects::update ( KDdouble dLeftTickCount )
{
	for ( auto pNode : m_aEffects )
	{
		pNode->update ( dLeftTickCount );
	}

	for ( auto pNode : m_aEffectFinishs )
	{
		pNode->update ( dLeftTickCount );
	}
}

KDvoid cSprEffects::addEffect ( std::string sName, Point tPoint)
{
		 if ( sName.compare ( _EFFECT_STR_NAIL	   ) == 0 )	addEffectNail	 ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_CUT	   ) == 0 )	addEffectCut	 ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_BITE	   ) == 0 )	addEffectBite	 ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_HIT	   ) == 0 )	addEffectHit	 ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_NAIL_ALL ) == 0 )	addEffectNailAll ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_CUT_ALL  ) == 0 )	addEffectCutAll	 ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_BITE_ALL ) == 0 )	addEffectBiteAll ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_HIT_ALL  ) == 0 )	addEffectHitAll  ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_POISON   ) == 0 )	addEffectPoison  ( tPoint );
	else if ( sName.compare ( _EFFECT_STR_REGEN	   ) == 0 )	addEffectRegen	 ( tPoint );
	else
	{
		for ( auto pNode : m_aEffects )
		{
			if ( pNode->getName ( ).compare ( sName ) == 0 )
			{
				pNode->view ( tPoint );
				break;
			}
		}
	}
}

KDvoid cSprEffects::addEffectNail ( Point tPoint )
{
	if ( cMyData::getObj ( )->m_bIsOptionSound != false )
	{		
		SimpleAudioEngine::getInstance ( )->playEffect ( "nail.mp3" );
	}

	m_aEffects [ 2 ]->view ( Point ( tPoint.x - 30, tPoint.y + 50 ) );
	m_aEffects [ 2 ]->view ( Point ( tPoint.x	  , tPoint.y + 40 ) );
	m_aEffects [ 2 ]->view ( Point ( tPoint.x + 30, tPoint.y + 55 ) );

	m_aEffects [ 2 ]->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	m_aEffects [ 2 ]->setScale ( 2, 2 );

	addEffectFinish ( tPoint, 0.2f );
}

KDvoid cSprEffects::addEffectCut ( Point tPoint )
{
	if ( cMyData::getObj()->m_bIsOptionSound != 0 )	
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "cut.mp3" );
	}

	m_aEffects [ 0 ]->view ( tPoint );
	m_aEffects [ 0 ]->setScale ( 2, 2 );

	addEffectFinish ( tPoint, 0.2f );
}

KDvoid cSprEffects::addEffectBite ( Point tPoint )
{
	if ( cMyData::getObj ( )->m_bIsOptionSound != 0 )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "bite.mp3" );
	}

	m_aEffects [ 3 ]->view ( Point ( tPoint.x - 30, tPoint.y ) );
	m_aEffects [ 4 ]->view ( Point ( tPoint.x - 15, tPoint.y ) );
	m_aEffects [ 4 ]->view ( Point ( tPoint.x	  , tPoint.y ) );
	m_aEffects [ 4 ]->view ( Point ( tPoint.x + 15, tPoint.y ) );
	m_aEffects [ 3 ]->view ( Point ( tPoint.x + 30, tPoint.y ) );

	m_aEffects [ 5 ]->view ( Point ( tPoint.x - 23, tPoint.y ) );
	m_aEffects [ 6 ]->view ( Point ( tPoint.x -  8, tPoint.y ) );
	m_aEffects [ 6 ]->view ( Point ( tPoint.x +  8, tPoint.y ) );
	m_aEffects [ 5 ]->view ( Point ( tPoint.x + 23, tPoint.y ) );

	m_aEffects [ 3 ]->setScale ( 2, 2 );
	m_aEffects [ 4 ]->setScale ( 2, 2 );
	m_aEffects [ 5 ]->setScale ( 2, 2 );
	m_aEffects [ 6 ]->setScale ( 2, 2 );
	m_aEffects [ 3 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );
	m_aEffects [ 4 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );
	m_aEffects [ 5 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );
	m_aEffects [ 6 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );

	addEffectFinish ( tPoint, 0.3f );
}

KDvoid cSprEffects::addEffectHit ( Point tPoint )
{
	if ( cMyData::getObj()->m_bIsOptionSound != 0 )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "hit.mp3" );
	}

	m_aEffects [ 1 ]->view ( tPoint );

	addEffectFinish ( tPoint, 0.3f );
}

KDvoid cSprEffects::addEffectNailAll ( Point tPoint )
{
	if ( cMyData::getObj()->m_bIsOptionSound != 0 )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "nail.mp3" );
	}

	m_aEffects [ 9 ]->view ( Point (_SCR_W_HARP - 160, _SCR_H_HARP + 60 ) );
	m_aEffects [ 9 ]->view ( Point (_SCR_W_HARP -  80, _SCR_H_HARP + 60 ) );
	m_aEffects [ 9 ]->view ( Point (_SCR_W_HARP		 , _SCR_H_HARP + 60 ) );
	m_aEffects [ 9 ]->view ( Point (_SCR_W_HARP +  80, _SCR_H_HARP + 60 ) );
	m_aEffects [ 9 ]->view ( Point (_SCR_W_HARP + 160, _SCR_H_HARP + 60 ) );

	m_aEffects [ 9 ]->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	m_aEffects [ 9 ]->setScale ( 2, 4 );
}

KDvoid cSprEffects::addEffectCutAll ( Point tPoint )
{
	if ( cMyData::getObj()->m_bIsOptionSound != 0 )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "cut.mp3" );
	}

	std::vector<Point>		aPoints;
	tPoint = Point ( _SCR_W_HARP-160, _SCR_H_HARP+100 );
	for ( KDint y = 0; y < 3; y++ )
	{
		for ( KDint x = 0; x < 6; x++ )
		{
			if ( aPoints.size ( ) > 0 )
			{
				KDint	nRand = kdRand ( ) % aPoints.size ( );

				aPoints.push_back ( aPoints [ nRand ] );
				aPoints [ nRand ] = tPoint;
			}
			else
			{
				aPoints.push_back ( tPoint );
			}

			tPoint.x += 66;
		}

		tPoint.x  = _SCR_W_HARP - 160;
		tPoint.y -= 100;
	}

	KDdouble	dLeftTime = 0;
	KDint		nLeftTemp = 3;
	for ( auto& rPoint : aPoints )
	{
		--nLeftTemp;

		if ( nLeftTemp == 0 )
		{
			m_aEffects [ 0 ]->viewReservation ( rPoint, dLeftTime, "cut.mp3" );
			nLeftTemp = 2;
		}
		else
		{
			m_aEffects [ 0 ]->viewReservation ( rPoint, dLeftTime );
		}

		dLeftTime += 0.02;
	}

	m_aEffects [ 0 ]->setScale ( 2, 2 );
}

KDvoid cSprEffects::addEffectBiteAll ( Point tPoint )
{
	if ( cMyData::getObj()->m_bIsOptionSound != 0 )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "bite.mp3" );
	}

	m_aEffects [ 3 ]->view ( Point ( _SCR_W_HARP - 180, _SCR_H_HARP - 40 ) );
	m_aEffects [ 4 ]->view ( Point ( _SCR_W_HARP -  90, _SCR_H_HARP - 40 ) );
	m_aEffects [ 4 ]->view ( Point ( _SCR_W_HARP	  , _SCR_H_HARP - 40 ) );
	m_aEffects [ 4 ]->view ( Point ( _SCR_W_HARP +  90, _SCR_H_HARP - 40 ) );
	m_aEffects [ 3 ]->view ( Point ( _SCR_W_HARP + 180, _SCR_H_HARP - 40 ) );

	m_aEffects [ 5 ]->view ( Point ( _SCR_W_HARP - 138, _SCR_H_HARP - 40 ) );
	m_aEffects [ 6 ]->view ( Point ( _SCR_W_HARP -  48, _SCR_H_HARP - 40 ) );
	m_aEffects [ 6 ]->view ( Point ( _SCR_W_HARP +  48, _SCR_H_HARP - 40 ) );
	m_aEffects [ 5 ]->view ( Point ( _SCR_W_HARP + 138, _SCR_H_HARP - 40 ) );

	m_aEffects [ 3 ]->setScale ( 3, 2 );
	m_aEffects [ 4 ]->setScale ( 3, 2 );
	m_aEffects [ 5 ]->setScale ( 3, 2 );
	m_aEffects [ 6 ]->setScale ( 3, 2 );
	m_aEffects [ 3 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );
	m_aEffects [ 4 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );
	m_aEffects [ 5 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );
	m_aEffects [ 6 ]->setAnchorPoint ( Point ( 0.5f, 0 ) );
	m_aEffects [ 3 ]->setAlpha ( 200 );
	m_aEffects [ 4 ]->setAlpha ( 200 );
	m_aEffects [ 5 ]->setAlpha ( 200 );
	m_aEffects [ 6 ]->setAlpha ( 200 );
}

KDvoid cSprEffects::addEffectHitAll ( Point tPoint )
{
	if ( cMyData::getObj()->m_bIsOptionSound != 0 )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "hit.mp3" );
	}

	std::vector<Point>		aPoints;
	tPoint = Point (_SCR_W_HARP-200, _SCR_H_HARP);

	for ( KDint x = 0; x < 11; x++ )
	{
		if ( aPoints.size ( ) > 0 )
		{
			Point	tPoint2 = Point ( tPoint.x, tPoint.y - 100 + ( kdRand ( ) % 200 ) );
			KDint	nRand = kdRand ( ) % aPoints.size ( );

			aPoints.push_back ( aPoints [ nRand ] );
			aPoints [ nRand ] = tPoint2;
		}
		else
		{
			aPoints.push_back ( tPoint );
		}

		tPoint.x += 40;
	}

	KDdouble	dLeftTime = 0;
	KDint		i = 0;
	for ( std::vector<Point>::iterator pNode = aPoints.begin ( ); pNode != aPoints.end ( ); ++pNode )
	{
		m_aEffects [ 1 ]->viewReservation ( *pNode, dLeftTime, "hit.mp3" );

			 if ( i == 0 )	dLeftTime += 0.4;
		else if ( i == 1 )	dLeftTime += 0.2;
		else if ( i == 2 )	dLeftTime += 0.1;
		else				dLeftTime += 0.05;

		++i;
	}
}

KDvoid cSprEffects::addEffectPoison ( Point tPoint )
{
	m_aEffects [ 7 ]->view ( tPoint );
	m_aEffects [ 7 ]->setAnchorPoint ( Point ( 0.5f, 0.3f ) );
	m_aEffects [ 7 ]->setScale ( 1.8f, 1.8f );
}

KDvoid cSprEffects::addEffectRegen ( Point tPoint )
{
	m_aEffects [ 8 ]->view ( tPoint );
	m_aEffects [ 8 ]->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	m_aEffects [ 8 ]->setScale ( 2, 2 );
	m_aEffects [ 8 ]->setAlpha ( 130 );
}

KDvoid cSprEffects::addEffectFinish ( Point tPoint, KDfloat fWaitTime )
{
	for ( auto pNode : m_aEffectFinishs )
	{
		if ( pNode->view ( tPoint, fWaitTime ) == true )
		{
			return;
		}
	}
}

KDbool cSprEffects::isEffect ( KDvoid )
{
	for ( auto pNode : m_aEffects )
	{
		if ( pNode->isView ( ) == true )
		{
			return true;
		}
	}

	for ( auto pNode : m_aEffectFinishs )
	{
		if ( pNode->isView ( ) == true )
		{
			return true;
		}
	}

	return false;
}

KDvoid cSprEffects::setColor ( KDint r, KDint g, KDint b )
{
	for ( auto pNode : m_aEffects )
	{
		pNode->setColor ( r, g, b );
	}
}

