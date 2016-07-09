/* --------------------------------------------------------------------------
 *
 *      File            BoardPiecePlacesPivotNode.cpp
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
#include "BoardPiecePlacesPivotNode.h"
#include "BoardPiecePlace.h"

BoardPiecePlacesPivotNode::BoardPiecePlacesPivotNode ( KDvoid )
{
	m_pBoardPiecePlaces = KD_NULL;
}

BoardPiecePlacesPivotNode::~BoardPiecePlacesPivotNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pBoardPiecePlaces );
}

KDbool BoardPiecePlacesPivotNode::init ( KDvoid )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_pBoardPiecePlaces = new CCArray ( );

	return KD_TRUE;
}

KDvoid BoardPiecePlacesPivotNode::addChild ( CCNode* pChild )
{
	CCAssert ( pChild != KD_NULL, "Argument must be non-nil");

	m_pBoardPiecePlaces->addObject ( pChild );
	
	CCNode::addChild ( pChild, pChild->getZOrder ( ), pChild->getTag ( ) );
}

// Dummy method for CCRGBAProtocol protocol.
KDvoid BoardPiecePlacesPivotNode::setColor ( const ccColor3B& tColor )
{

}

// Dummy method for CCRGBAProtocol protocol
const ccColor3B& BoardPiecePlacesPivotNode::getColor ( KDvoid )
{
	return ccWHITE;
}

const ccColor3B& BoardPiecePlacesPivotNode::getDisplayedColor ( KDvoid )
{
	return ccWHITE;
}

GLubyte BoardPiecePlacesPivotNode::getDisplayedOpacity ( KDvoid )
{
	return 255;
}

// Dummy method for CCRGBAProtocol protocol.
GLubyte BoardPiecePlacesPivotNode::getOpacity ( KDvoid )
{
	return 255;
}

KDvoid BoardPiecePlacesPivotNode::setOpacity ( GLubyte cOpacity )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pBoardPiecePlaces, pObject )
	{
		BoardPiecePlace*	pPiece = (BoardPiecePlace*) pObject;
		pPiece->setOpacity ( cOpacity );
	}
}

KDvoid BoardPiecePlacesPivotNode::setOpacityModifyRGB ( KDbool bValue )
{

}

KDbool BoardPiecePlacesPivotNode::isOpacityModifyRGB ( KDvoid )
{
	return KD_FALSE;
}

KDbool BoardPiecePlacesPivotNode::isCascadeColorEnabled ( KDvoid )
{
	return KD_FALSE;
}

KDvoid BoardPiecePlacesPivotNode::setCascadeColorEnabled ( KDbool bCascadeColorEnabled )
{

}

KDvoid BoardPiecePlacesPivotNode::updateDisplayedColor ( const ccColor3B& tColor )
{

}

KDbool BoardPiecePlacesPivotNode::isCascadeOpacityEnabled ( KDvoid )
{
	return KD_FALSE;
}

KDvoid BoardPiecePlacesPivotNode::setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled )
{

}

KDvoid BoardPiecePlacesPivotNode::updateDisplayedOpacity ( GLubyte cOpacity )
{

}
