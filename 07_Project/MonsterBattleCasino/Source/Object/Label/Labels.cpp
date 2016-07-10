/* -----------------------------------------------------------------------------------
 *
 *      File            Labels.cpp
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
#include "Labels.h"

cLabels::cLabels ( Node* pNode )
: m_pNode ( pNode )
{

}

cLabels::~cLabels ( KDvoid )
{
	clear ( );
}

KDvoid cLabels::clear ( KDvoid )
{
	for ( auto pNode : m_aLabels )
	{
		m_pNode->removeChild ( pNode, true );
	}
	m_aLabels.clear ( );
	m_aMoves .clear ( );
}

KDvoid cLabels::changeNode ( Node* pNode )
{
	m_pNode = pNode;
}

LabelTTF* cLabels::add ( const KDchar* pFontName, KDfloat fFontSize, TextHAlignment eAlignment, Rect tRect, const KDchar* pStr, KDint z )
{
	LabelTTF*	pLabel = LabelTTF::create ( pStr, pFontName, fFontSize, tRect.size, eAlignment );

	pLabel->setPosition ( tRect.origin );

	m_pNode->addChild ( pLabel, z );

	m_aLabels.push_back ( pLabel );

	return pLabel;
}

KDvoid cLabels::del ( LabelTTF*	pLabel )
{
	for ( std::vector<LabelTTF*>::iterator pNode = m_aLabels.begin ( ); pNode != m_aLabels.end ( ); pNode++ )
	{
		if ( ( *pNode ) == pLabel )
		{
			m_pNode->removeChild ( *pNode, true );
			m_aLabels.erase ( pNode );
			break;;
		}
	}
}

KDvoid cLabels::setVisible ( LabelTTF*	pLabel, KDbool bIsVisible )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			pNode->setVisible ( bIsVisible );
			break;
		}
	}
}

KDvoid cLabels::setPoint ( LabelTTF* pLabel, Point tPoint )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			pNode->setPosition ( tPoint );
			break;
		}
	}
}

KDvoid cLabels::setAnchorPoint ( LabelTTF* pLabel, Point tPoint )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			pNode->setAnchorPoint ( tPoint );
			break;
		}
	}
}

KDvoid cLabels::setStr ( LabelTTF* pLabel, const KDchar* pStr )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			pNode->setString ( pStr );
			break;
		}
	}
}

KDvoid cLabels::setAllColor ( KDubyte a, KDubyte r, KDubyte g, KDubyte b )
{
	for ( auto pNode : m_aLabels )
	{
		Color3B		tColor;

		tColor.r = r;
		tColor.g = g;
		tColor.b = b;

		pNode->setColor ( tColor );
		pNode->setOpacity ( a );
	}
}

KDvoid cLabels::setColor ( LabelTTF* pLabel, KDubyte r, KDubyte g, KDubyte b )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			Color3B		tColor;

			tColor.r = r;
			tColor.g = g;
			tColor.b = b;

			pNode->setColor ( tColor );
			break;
		}
	}
}

KDvoid cLabels::setAlpha ( LabelTTF* pLabel, KDubyte cAlpha )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			pNode->setOpacity ( cAlpha );
			break;
		}
	}
}

KDvoid cLabels::setPointTime ( LabelTTF* pLabel, Point tPoint, KDdouble dLeftTime )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			sMove		tMove;

			tMove.pLabel		= pNode;
			tMove.tMove			= tPoint - pNode->getPosition ( );
			tMove.tMoveBefore	= pNode->getPosition ( );
			tMove.dTimeMoveFull = dLeftTime;
			tMove.dTimeMoveLeft = dLeftTime;

			m_aMoves.push_back ( tMove );

			break;
		}
	}
}

Point cLabels::getPoint ( LabelTTF* pLabel )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			return pNode->getPosition ( );
		}
	}

	return Point::ZERO;
}

Size cLabels::getSize ( LabelTTF* pLabel )
{
	for ( auto pNode : m_aLabels )
	{
		if ( pNode == pLabel )
		{
			return pNode->getTexture ( )->getContentSize ( );
		}
	}

	return Size::ZERO;
}

KDvoid cLabels::update ( KDdouble dLeftTime )
{
	std::vector<sMove>::iterator	pNode = m_aMoves.begin ( );

	while ( pNode != m_aMoves.end ( ) )
	{
		sMove*		pMove = &( *pNode );
		KDbool		bIsErase = false;

		for ( std::vector<LabelTTF*>::iterator pNode2 = m_aLabels.begin ( ); pNode2 != m_aLabels.end ( ); ++pNode2 )
		{
			LabelTTF*	pLabel = *pNode2;

			if ( pLabel == pMove->pLabel )
			{
				pMove->dTimeMoveLeft -= dLeftTime;

				if ( pMove->dTimeMoveLeft < 0 )
				{
					Point		tPoint = Point ( pMove->tMoveBefore.x + pMove->tMove.x, pMove->tMoveBefore.y + pMove->tMove.y );
					pLabel->setPosition ( tPoint );
					bIsErase = true;
				}
				else
				{
					Point		tPoint = Point ( pMove->tMoveBefore.x + ( pMove->dTimeMoveFull - pMove->dTimeMoveLeft ) * pMove->tMove.x / pMove->dTimeMoveFull,
												 pMove->tMoveBefore.y + ( pMove->dTimeMoveFull - pMove->dTimeMoveLeft ) * pMove->tMove.y / pMove->dTimeMoveFull );

					pLabel->setPosition ( tPoint );
				}

				break;
			}
		}

		if ( bIsErase == true )
		{
			pNode = m_aMoves.erase ( pNode );
		}
		else
		{
			++pNode;
		}
	}
}