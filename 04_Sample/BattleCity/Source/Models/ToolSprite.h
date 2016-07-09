/* --------------------------------------------------------------------------
 *
 *      File            ToolSprite.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10). All rights reserved.
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

#ifndef __ToolSprite_h__
#define __ToolSprite_h__

typedef enum
{
    eStart		= 1	,
    ePass			,    
    eLife			,    
    eMine			,    
    eSafe			,    
    eWall			,
} ToolSpriteKind;

class ToolSprite : public CCSprite 
{
	public :

		static ToolSprite*		create ( ToolSpriteKind eKind );

	public :

		virtual KDbool			initWithKind ( ToolSpriteKind eKind );

		ToolSpriteKind			getKind ( KDvoid );

	private :

		ToolSpriteKind			m_eKind;
};

#endif	// __ToolSprite_h__
