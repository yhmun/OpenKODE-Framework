/* -----------------------------------------------------------------------------------
 *
 *      File            NumScroll.cpp
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
#include "NumScroll.h"

cNumScroll::cNumScroll ( Node* pNode, const KDchar* pFileName, KDint z, Point tPoint, Size tUnitImgSize, Rect tRectTouch, Rect tRectPut )
: m_pNode				( pNode			)
, m_tPoint				( tPoint		)
, m_tUnitImgSize		( tUnitImgSize	)
, m_tRectTouch			( tRectTouch	)
, m_tRectPut			( tRectPut		)
, m_nNum				( 0				)
, m_nMaxNum				( -1			)
, m_bIsMoving			( 0				)
, m_fMove				( 0				)
, m_fMove2				( 0				)
, m_nTouchID			( -1			)
, m_tTouch				( Point::ZERO	)
, m_dTouchTime			( 0				)
, m_nTouchMovesCount	( 0				)
, m_tTouchMovesSize		( 0				)
{
	Rect	tRectSrc = Rect ( 0, 0, tUnitImgSize.width, tUnitImgSize.height );
	for ( KDint i = 0; i < 10; i++ )
	{
		cSprite*	pSprite = new cSprite ( m_pNode );

		pSprite->initTexture ( pFileName, z, tPoint );
		pSprite->addFrame ( tRectSrc );
		pSprite->setFrame ( 0 );

		m_aSprites.push_back ( pSprite );

		tRectSrc.origin.y += tUnitImgSize.height;

		Color3B		tColor;
		
		tColor.r = 0;
		tColor.g = 0;
		tColor.b = 0;

		m_aSprites [ i ]->get ( )->setColor ( tColor );
	}

	setNum2 ( 0 );
}

cNumScroll::~cNumScroll ( KDvoid )
{
	for ( auto pNode : m_aSprites )
	{
		CC_SAFE_RELEASE ( pNode );
	}
	m_aSprites.clear ( );
}

KDvoid cNumScroll::update ( KDdouble dLeftTime )
{
	if ( m_nMaxNum != -1 && m_nNum > m_nMaxNum )
	{
		m_nNum		= m_nMaxNum;
		m_fMove		= 0;
		m_fMove2	= 0;
		m_bIsMoving	= false;

		setPoint ( m_tPoint, m_fMove );

		setEffect ( );
	}

	if ( m_nTouchID == -1 )
	{
		for ( auto pSprite : m_aSprites )
		{
			Color3B		tColor = pSprite->get ( )->getColor ( );
			if ( tColor.r > 0 || tColor.g > 0 )
			{
				if ( tColor.r - 10 < 0 )	tColor.r  = 0;
				else						tColor.r -= 10;
				if ( tColor.g - 10 < 0 )	tColor.g  = 0;
				else						tColor.g -= 10;

				pSprite->get ( )->setColor ( tColor );
			}
		}
	}

	if ( m_nTouchID == -1 )
	{
		if ( m_bIsMoving == true )
		{
			if ( m_nMaxNum != -1 )
			{
				if ( m_fMove2 < 0 && m_nNum == 0 )
				{
					m_bIsMoving	= false;
					m_fMove		= 0;
					m_fMove2	= 0;

					setNum2 ( m_nNum );

					setEffect ( );
				}
				else if( m_fMove2 > 0 && m_nNum == m_nMaxNum )
				{
					m_bIsMoving	= false;
					m_fMove		= 0;
					m_fMove2	= 0;

					setNum2 ( m_nNum );

					setEffect ( );
				}
			}
		}

		if ( m_bIsMoving == true )
		{
			m_fMove	+= m_fMove2;

			KDfloat		fHalf = m_tUnitImgSize.height / 2;

			while ( m_fMove < -fHalf )
			{
				--m_nNum;
				if ( m_nMaxNum != -1 && m_nNum <= 0 )
				{
					m_nNum		= 0;
					m_bIsMoving	= false;
					m_fMove		= 0;
					m_fMove2	= 0;

					setEffect ( );
					break;
				}
				
				if ( m_nNum < 0 )
				{
					m_nNum = 9;
				}

				m_fMove += m_tUnitImgSize.height;
			}

			while ( m_fMove > fHalf )
			{
				++m_nNum;
				if ( m_nMaxNum != -1 && m_nNum >= m_nMaxNum )
				{
					m_nNum		= m_nMaxNum;
					m_bIsMoving	= false;
					m_fMove		= 0;
					m_fMove2	= 0;

					setEffect ( );
					break;
				}

				if ( m_nNum > 9 )
				{
					m_nNum = 0;
				}

				m_fMove -= m_tUnitImgSize.height;
			}

			setNum2 ( m_nNum );

			if ( m_fMove2 > 0 )
			{
				if ( m_fMove2 > 10 )	m_fMove2 -= 3;
				else					m_fMove2 -= 1.5f;

				if ( m_fMove2 < 2 && m_fMove > 0 )		m_fMove2 = 2;

				if ( m_fMove2 < 0 )
				{
					m_bIsMoving	= false;
					m_fMove2	= 0;
				}
			}
			else if ( m_fMove2 < 0 )
			{
				if ( m_fMove2 < -10 )	m_fMove2 += 3;
				else					m_fMove2 += 1.5f;

				if ( m_fMove2 > -2 && m_fMove < 0 )		m_fMove2 = -2;

				if ( m_fMove2 > 0 )
				{
					m_bIsMoving	= false;
					m_fMove2	= 0;
				}
			}
		}
		else
		{
			if ( m_fMove != 0 )
			{
				if ( m_fMove > 0 )
				{
					m_fMove -= 1;
					if ( m_fMove < 0 )		m_fMove = 0;
				}
				else
				{
					m_fMove += 1;
					if ( m_fMove > 0 )		m_fMove = 0;
				}

				setPoint ( m_tPoint, m_fMove );
			}
		}
	}
}

KDbool cNumScroll::touchBegan ( KDint nTid, Point tPoint )
{
	if ( m_nTouchID != -1 || RectInPoint ( m_tRectTouch, tPoint ) == false )
	{
		return false;
	}

	m_nTouchID			= nTid;
	m_tTouch			= tPoint;
	m_dTouchTime		= kdGetMilliseconds ( );
	m_nTouchMovesCount	= 0;
	m_tTouchMovesSize	= 0;
	m_bIsMoving			= false;
	m_fMove2			= 0;

	return true;
}

KDvoid cNumScroll::touchMoved ( KDint nTid, Point tPoint )
{
	if ( nTid != m_nTouchID )
	{
		return;
	}

	KDfloat		fMoveY = tPoint.y - m_tTouch.y;
	m_fMove	+= fMoveY;
	KDfloat		fHalf = m_tUnitImgSize.height/2;

	if ( m_nMaxNum != -1 )
	{
			 if ( m_fMove < 0 && m_nNum	==		   0 ) { m_fMove = 0; setEffect ( ); }
		else if ( m_fMove > 0 && m_nNum == m_nMaxNum ) { m_fMove = 0; setEffect ( ); }
	}

	if ( m_fMove != 0 )
	{
		Color3B		tColor;

		tColor.r = 0;
		tColor.g = 0;
		tColor.b = 0;

		for ( auto pSprite : m_aSprites )
		{
			pSprite->get ( )->setColor ( tColor );
		}

		while ( m_fMove < -fHalf )
		{
			--m_nNum;
			if ( m_nNum < 0 )
			{
				m_nNum = 9;
			}

			m_fMove += m_tUnitImgSize.height;
		}
		while ( m_fMove > fHalf )
		{
			++m_nNum;
			if ( m_nNum > 9 )
			{
				m_nNum = 0;
			}

			m_fMove -= m_tUnitImgSize.height;
		}

		setNum2 ( m_nNum );
	}		

	m_tTouch	 = tPoint;
	m_dTouchTime = kdGetMilliseconds ( );

	m_aTouchMoves [ m_nTouchMovesCount ].tMove.y	= fMoveY;
	m_aTouchMoves [ m_nTouchMovesCount ].dTimeStamp	= m_dTouchTime - m_dTouchTime;
	++m_nTouchMovesCount;
	++m_tTouchMovesSize;

	if ( m_nTouchMovesCount > 4 ) m_nTouchMovesCount = 0;
	if ( m_tTouchMovesSize	> 5 ) m_tTouchMovesSize  = 5;
}

KDvoid cNumScroll::touchEnded ( KDint nTid, Point tPoint )
{
	if ( nTid != m_nTouchID )
	{
		return;
	}

	KDfloat		fMoveY = tPoint.y -  m_tTouch.y;
	m_fMove	+= fMoveY;
	KDfloat		fHalf = m_tUnitImgSize.height / 2;

	if ( m_nMaxNum != -1 )
	{
			 if ( m_fMove < 0 && m_nNum	==		   0 ) { m_fMove = 0; setEffect ( ); }
		else if ( m_fMove > 0 && m_nNum == m_nMaxNum ) { m_fMove = 0; setEffect ( ); }
	}

	if ( m_fMove != 0 )
	{
		Color3B		tColor;

		tColor.r = 0;
		tColor.g = 0;
		tColor.b = 0;

		for ( auto pSprite : m_aSprites )
		{
			pSprite->get ( )->setColor ( tColor );
		}

		while ( m_fMove < -fHalf )
		{
			--m_nNum;
			if ( m_nNum < 0 )
			{
				m_nNum = 9;
			}

			m_fMove += m_tUnitImgSize.height;
		}

		while ( m_fMove > fHalf )
		{
			++m_nNum;
			if ( m_nNum > 9 )
			{
				m_nNum = 0;
			}

			m_fMove -= m_tUnitImgSize.height;
		}

		setNum2 ( m_nNum );
	}		

	m_tTouch	 = tPoint;
	m_dTouchTime = kdGetMilliseconds ( );

	m_aTouchMoves [ m_nTouchMovesCount ].tMove.y	= fMoveY;
	m_aTouchMoves [ m_nTouchMovesCount ].dTimeStamp	= m_dTouchTime - m_dTouchTime;
	++m_nTouchMovesCount;
	++m_tTouchMovesSize;
	if ( m_nTouchMovesCount > 4 ) m_nTouchMovesCount = 0;
	if ( m_tTouchMovesSize	> 5 ) m_tTouchMovesSize  = 5;

	KDfloat		fMoveSum = 0;
	KDfloat		fTimeSum = 0;
	for ( KDint i = 0; i < m_tTouchMovesSize; i++ )
	{
		fMoveSum += m_aTouchMoves [ i ].tMove.y;
		fTimeSum += m_aTouchMoves [ i ].dTimeStamp;
	}

	if ( fTimeSum > 0 )
	{
		m_fMove2	= ( fMoveSum / fTimeSum ) / 50;
		m_bIsMoving	= true;

		if ( m_fMove2 > -1 && m_fMove2 < 1 )
		{
			m_fMove2	= 0;
			m_bIsMoving	= false;
		}
	}

	m_nTouchID = -1;
}

KDvoid cNumScroll::setVisible ( KDbool bIsVisible )
{
	for ( auto pSprite : m_aSprites )
	{
		pSprite->get ( )->setVisible ( false );
	}

	if ( bIsVisible == true )
	{
		setNum2  ( m_nNum );
	}
}

KDvoid cNumScroll::setNum ( KDint nNum )
{
	m_bIsMoving	= false;
	m_fMove		= 0;
	m_fMove2	= 0;

	setNum2 ( nNum );
}

KDvoid cNumScroll::setMax ( KDint nNum )
{
	if ( nNum < -1 || nNum > 9 )
	{
		return;
	}

	m_nMaxNum = nNum;
}

KDvoid cNumScroll::setEffect ( KDvoid )
{
	Color3B		tColor1;

	tColor1.r = 0;
	tColor1.g = 0;
	tColor1.b = 0;

	Color3B		tColor2;

	tColor2.r = 255;
	tColor2.g = 255;
	tColor2.b = 0;

	for ( KDuint i = 0; i < m_aSprites.size ( ); i++ )
	{
		if ( i == m_nNum )	m_aSprites [ i ]->get ( )->setColor ( tColor2 );
		else				m_aSprites [ i ]->get ( )->setColor ( tColor1 );
	}
}

KDvoid cNumScroll::setNum2 ( KDint nNum )
{
	if ( nNum < 0 || nNum >= 10 )
	{
		return;
	}

	m_nNum = nNum;

	setPoint ( m_tPoint, m_fMove );
}

KDvoid cNumScroll::setPoint ( Point tPoint, KDfloat fMove )
{
	for ( auto pSprite : m_aSprites )
	{
		pSprite->get ( )->setVisible ( false );
	}

	m_tPoint = tPoint;
	m_fMove  = fMove;
	tPoint.y += m_fMove;

	m_aSprites [ m_nNum ]->get ( )->setVisible ( true );
	m_aSprites [ m_nNum ]->get ( )->setPosition ( tPoint );

	for ( KDint i = 0, nLoopNum = m_nNum - 1; i < 3; i++, --nLoopNum )
	{
		tPoint.y += m_tUnitImgSize.height;

		if ( tPoint.y > m_tRectPut.origin.y + m_tRectPut.size.height )
		{
			break;
		}

		if ( nLoopNum < 0 )
		{
			nLoopNum = 9;
		}

		m_aSprites [ nLoopNum ]->get ( )->setVisible ( true );
		m_aSprites [ nLoopNum ]->get ( )->setPosition ( tPoint );
	}

	tPoint	  = m_tPoint;
	tPoint.y += m_fMove;

	for ( KDint i = 0, nLoopNum = m_nNum + 1; i < 3; i++, ++nLoopNum )
	{
		tPoint.y -= m_tUnitImgSize.height;

		if ( tPoint.y + m_tUnitImgSize.height < m_tRectPut.origin.y )
		{
			break;
		}

		if ( nLoopNum > 9 )
		{
			nLoopNum = 0;
		}

		m_aSprites [ nLoopNum ]->get ( )->setVisible ( true );
		m_aSprites [ nLoopNum ]->get ( )->setPosition ( tPoint );
	}
}
