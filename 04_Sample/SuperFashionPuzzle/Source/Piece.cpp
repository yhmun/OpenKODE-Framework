/* --------------------------------------------------------------------------
 *
 *      File            Piece.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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
#include "Piece.h"

Piece* Piece::create ( CCSpriteFrame* pSpriteFrame, KDuint uPiece, KDuint uGirl )
{
	Piece*	pRet = new Piece ( );

	if ( pRet && pRet->initNormalPieceWithSpriteFrame ( pSpriteFrame, uPiece, uGirl ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Piece* Piece::create ( CCSpriteFrame* pSpriteFrame )
{
	Piece*	pRet = new Piece ( );

	if ( pRet && pRet->initWildcardWithSpriteFrame ( pSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Piece::Piece ( KDvoid )
{
	m_pSpriteFrame = KD_NULL;
}

Piece::~Piece ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSpriteFrame );
}

KDbool Piece::initNormalPieceWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, KDuint uPiece, KDuint uGirl )
{
	CCAssert ( uPiece < NUMBER_OF_PIECES_PER_GIRL, "piece must be less than NUMBER_OF_PIECES_PER_GIRL." );
	CCAssert ( uGirl < NUMBER_OF_GIRLS, "girl must be less than NUMBER_OF_GIRLS." );
	CCAssert ( pSpriteFrame != KD_NULL, "spriteFrame cannot be null." );

	m_uGirl = uGirl;
	m_uPiece = uPiece;
	m_bIsWildcard = KD_FALSE;
	m_pSpriteFrame = pSpriteFrame;
	m_pSpriteFrame->retain ( );

	return KD_TRUE;
}

KDbool Piece::initWildcardWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
	CCAssert ( pSpriteFrame != KD_NULL, "spriteFrame cannot be null." );

	m_uGirl = 0;
	m_uPiece = 0;
	m_bIsWildcard = KD_TRUE;
	m_pSpriteFrame = pSpriteFrame;
	m_pSpriteFrame->retain ( );

	return KD_TRUE;
}

CCSpriteFrame* Piece::getSpriteFrame ( KDvoid )
{
	return m_pSpriteFrame;
}

KDuint Piece::getGirl ( KDvoid )
{
	return m_uGirl;
}

KDuint Piece::getPiece ( KDvoid )
{
	return m_uPiece;
}
