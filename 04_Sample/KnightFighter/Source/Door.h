/* --------------------------------------------------------------------------
 *
 *      File            Door.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 09/05/2011
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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

#ifndef __Door_h__
#define __Door_h__

class Door : public CCNode 
{
	public :

		CREATE_FUNC ( Door );	

	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	getContents ( KDvoid );
		virtual KDvoid			setContents ( const KDchar* szContents );

	public :

		CC_SYNTHESIZE ( CCPoint, m_tTilePos, TilePos );

	private :

		std::string				m_sContents;
};

#endif	// __Door_h__
