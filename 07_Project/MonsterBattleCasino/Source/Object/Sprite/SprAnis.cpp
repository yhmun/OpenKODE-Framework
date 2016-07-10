/* -----------------------------------------------------------------------------------
 *
 *      File            SprAnis.cpp
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
#include "SprAnis.h"
#include "MyData/MyData.h"

#define _ANI_NAME_MAX	18

cSprAnis::cSprAnis ( Node* pNode, std::string sFileName, KDint z, Point tPoint )
: m_pNode				( pNode		)
, m_sFileName			( sFileName	)
, m_nAniSelectionIndex	( 0			)
, m_tPoint				( tPoint	)
{
	KDubyte*	pData = nullptr;
	KDint		nSize = 0;
	KDint		nSeek = 0;

	pData = FileUtils::getInstance ( )->getFileData ( sFileName.c_str ( ), "rb", &nSize );

	std::string		sFileDir = sFileName.substr ( 0, sFileName.rfind ( "/" ) + 1 );

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
		cMyData::readToData ( pData, nSize, nSeek, szImageName	 , sizeof ( KDchar ) * nImageNameSize );
		
		// set ani file names
		std::string	sImageName = sFileDir + szImageName;
		m_sAniFileNames.push_back ( sImageName );

		// cSprAni new
		cSprAni*	pAni = new cSprAni ( m_pNode, sImageName.c_str ( ), z, m_tPoint, false );

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
			pAni->addFrame ( tRectSrc, fDelay, bAtt, bDef, tCenter, tRectAtt, tRectDef );
		}

		pAni->setVisible ( false );
		pAni->setLoop ( true );
		pAni->setFrameInit ( 0 );
		pAni->stop ( );

		m_aAnis.push_back ( pAni );
		m_aAnisAutoPointSet.push_back ( true );
	}

	// 선택된 애니메이션만 보여준다.
	m_aAnis [ m_nAniSelectionIndex  ]->setVisible ( true );
	m_aAnis [ m_nAniSelectionIndex  ]->setLoop ( true );
	m_aAnis [ m_nAniSelectionIndex ]->start ( );

	// release
	CC_SAFE_FREE ( pData );
}

cSprAnis::~cSprAnis ( KDvoid )
{
	for ( auto pAni : m_aAnis )
	{
		CC_SAFE_RELEASE ( pAni );
	}
	m_aAnis.clear ( );
}

KDvoid cSprAnis::changeNode ( Node* pNode, KDint z )
{
	m_pNode = pNode;

	for ( auto pAni : m_aAnis )
	{
		pAni->changeNode ( m_pNode, z );
	}
}

KDvoid cSprAnis::texLoad ( KDint nAniIndex )
{
	if ( nAniIndex < 0 || nAniIndex >= (KDint) m_aAnis.size ( ) )
	{
		return;
	}

	m_aAnis [ nAniIndex ]->texLoad ( );
}

KDvoid cSprAnis::texLoad ( KDint nAniIndex, KDint z )
{
	if ( nAniIndex < 0 || nAniIndex >= (KDint) m_aAnis.size ( ) )
	{
		return;
	}

	m_aAnis [ nAniIndex ]->texLoad ( z );
}

KDvoid cSprAnis::texRelease ( KDint nAniIndex )
{
	if ( nAniIndex < 0 || nAniIndex >= (KDint) m_aAnis.size ( ) )
	{
		return;
	}

	m_aAnis [ nAniIndex ]->texRelease ( );
}

KDbool cSprAnis::update ( KDdouble dLeftTickCount )
{
	for ( auto pAni : m_aAnis )
	{
		pAni->updateMove ( dLeftTickCount );
	}

	return m_aAnis [ m_nAniSelectionIndex ]->update ( dLeftTickCount, false );
}

KDvoid cSprAnis::start ( KDvoid )
{
	m_aAnis [ m_nAniSelectionIndex ]->start ( );
}

KDvoid cSprAnis::stop ( KDvoid )
{
	m_aAnis [ m_nAniSelectionIndex ]->stop ( );
}

KDvoid cSprAnis::changeAni ( KDint nSelectionIndex )
{
	if ( nSelectionIndex < 0 || nSelectionIndex >= (KDint) m_aAnis.size ( ) || m_nAniSelectionIndex == nSelectionIndex )	
	{
		return;
	}

	m_aAnis [ m_nAniSelectionIndex ]->stop ( );

	m_aAnis [ m_nAniSelectionIndex  ]->setVisible ( false );
	m_aAnis [ nSelectionIndex	    ]->setVisible ( true );

	m_nAniSelectionIndex = nSelectionIndex;
}

KDvoid cSprAnis::setVisible ( KDbool bIsVisible )
{
	m_aAnis [ m_nAniSelectionIndex  ]->setVisible ( bIsVisible );
}

KDvoid cSprAnis::setAlpha ( KDubyte cAlpha )
{
	for ( auto pAni : m_aAnis )
	{
		pAni->setAlpha ( cAlpha );
	}
}

KDvoid cSprAnis::setAlpha ( KDint nAniIndex, KDubyte cAlpha )
{
	if ( nAniIndex < 0 || nAniIndex >= (KDint) m_aAnis.size ( ) )
	{
		return;
	}

	m_aAnis [ nAniIndex  ]->setAlpha ( cAlpha );
}

KDvoid cSprAnis::setColor ( KDubyte r, KDubyte g, KDubyte b )
{
	for ( auto pAni : m_aAnis )
	{
		pAni->setColor ( r, g, b );
	}
}

KDvoid cSprAnis::setColor ( KDint nAniIndex, KDubyte r, KDubyte g, KDubyte b )
{
	if ( nAniIndex < 0 || nAniIndex >= (KDint) m_aAnis.size ( ) )
	{
		return;
	}

	m_aAnis [ nAniIndex  ]->setColor ( r, g, b );
}

KDvoid cSprAnis::setPoint ( const Point& tPoint )
{
	for ( KDuint i = 0; i < m_aAnis.size ( ); i++ )
	{
		if ( m_aAnisAutoPointSet [ i ] == true )
		{
			m_aAnis [ i  ]->setPoint ( tPoint );
		}
	}
}

KDvoid cSprAnis::setPointTime ( const Point& tPoint, KDdouble dLeftTime, KDbool bIsForciblyChange )
{
	for ( KDuint i = 0; i < m_aAnis.size ( ); i++ )
	{
		if ( m_aAnisAutoPointSet [ i ] == true )
		{
			m_aAnis [ i  ]->setPointTime ( tPoint, dLeftTime, bIsForciblyChange );
		}
	}
}

KDvoid cSprAnis::setPointAutoSwitch ( KDint nIndex, KDbool bIsAuto )
{
	if ( nIndex < 0 && nIndex >= (KDint) m_aAnisAutoPointSet.size ( ) )
	{
		return;
	}

	m_aAnisAutoPointSet [ nIndex ] = bIsAuto;
}

KDvoid cSprAnis::setLoop ( KDint nAniIndex, KDbool bIsLoop )
{
	if ( nAniIndex < 0 || nAniIndex >= (KDint) m_aAnis.size ( ) )
	{
		return;
	}

	m_aAnis [ nAniIndex  ]->setLoop ( bIsLoop );
}

KDvoid cSprAnis::setScale ( KDfloat fScaleX, KDfloat fScaleY )
{
	for ( auto pAni : m_aAnis )
	{
		pAni->setScale ( fScaleX, fScaleY );
	}
}

KDvoid cSprAnis::setSelectAniNowFrame ( KDint nFrame )
{
	m_aAnis [ m_nAniSelectionIndex  ]->setFrameInit ( nFrame );
}

KDvoid cSprAnis::getAniFileNames ( KDint nIndex, std::string& rAniFileName )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_sAniFileNames.size ( ) )
	{
		return;
	}

	rAniFileName = m_sAniFileNames [ nIndex ];
}

KDint cSprAnis::getSelectAniNowFrame ( KDvoid )
{
	return m_aAnis [ m_nAniSelectionIndex ]->getNowFrame ( );
}

Point cSprAnis::getPoint ( KDvoid )
{
	return m_aAnis [ m_nAniSelectionIndex ]->getPoint ( );
}

cSprAni* cSprAnis::getAni ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aAnis.size ( ) )
	{
		return nullptr;
	}

	return m_aAnis [ nIndex ];
}

KDint cSprAnis::getFrameSize ( KDint nAniIndex )
{
	if ( nAniIndex < 0 || nAniIndex >= (KDint) m_aAnis.size ( ) )
	{
		return 0;
	}

	return m_aAnis [ nAniIndex ]->getFrameSize ( );
}

KDbool cSprAnis::getVisible ( KDvoid )
{
	return m_aAnis [ m_nAniSelectionIndex ]->getSprite ( )->get ( )->isVisible ( );
}

KDubyte cSprAnis::getAlpha ( KDvoid )
{
	return m_aAnis [ m_nAniSelectionIndex ]->getAlpha ( );
}
