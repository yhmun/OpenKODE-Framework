/* --------------------------------------------------------------------------
 *
 *      File            Piece.h
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

#ifndef __Piece_h__
#define __Piece_h__

// Represents a piece. Every girl has 5 different pieces and there are 10 girls.
// This object will be used by Board.
class Piece : public CCObject 
{
	public :
				 Piece ( KDvoid );
		virtual ~Piece ( KDvoid );

		static Piece*			create ( CCSpriteFrame* pSpriteFrame, KDuint uPiece, KDuint uGirl );

		static Piece*			create ( CCSpriteFrame* pSpriteFrame );

	public :

		virtual KDbool			initNormalPieceWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, KDuint uPiece, KDuint uGirl );

		virtual KDbool			initWildcardWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );

		CCSpriteFrame*			getSpriteFrame ( KDvoid );

		KDuint					getGirl ( KDvoid );

		KDuint					getPiece ( KDvoid );

	private :

		KDuint					m_uGirl;
		KDuint					m_uPiece;
		KDbool					m_bIsWildcard;
		CCSpriteFrame*			m_pSpriteFrame;
};

#endif	// __Piece_h__
