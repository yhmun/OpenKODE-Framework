/* --------------------------------------------------------------------------
 *
 *      File            BoardPiecePlacesPivotNode.h
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

#ifndef __BoardPiecePlacesPivotNode_h__
#define __BoardPiecePlacesPivotNode_h__

// A container of BoardPiecePlace instances. Actually is used to modify all BoardPiecePlaces opacity at the same time.
class BoardPiecePlacesPivotNode : public CCNode, public CCRGBAProtocol
{
	public :
				 BoardPiecePlacesPivotNode ( KDvoid );
		virtual ~BoardPiecePlacesPivotNode ( KDvoid );

		CREATE_FUNC ( BoardPiecePlacesPivotNode );

	public :

		virtual KDbool				init ( KDvoid );

		virtual KDvoid				addChild ( CCNode* pChild );

		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual const ccColor3B&	getColor ( KDvoid );

		virtual const ccColor3B&	getDisplayedColor ( KDvoid );
		virtual GLubyte				getDisplayedOpacity ( KDvoid );

		virtual GLubyte				getOpacity ( KDvoid );
		virtual KDvoid				setOpacity ( GLubyte cOpacity );

		virtual KDvoid				setOpacityModifyRGB ( KDbool bValue );
		virtual KDbool				 isOpacityModifyRGB ( KDvoid );

		virtual KDbool				 isCascadeColorEnabled ( KDvoid );
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled );
	    
		virtual KDvoid				updateDisplayedColor ( const ccColor3B& tColor );

		virtual KDbool				 isCascadeOpacityEnabled ( KDvoid );
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled );
	    
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cOpacity );

	private :

		CCArray*					m_pBoardPiecePlaces;
};

#endif	// __BoardPiecePlacesPivotNode_h__
