/* --------------------------------------------------------------------------
 *
 *      File            XMPlayer.h
 *      Description     XMPlayer
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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

#ifndef __XMPlayer_h__
#define __XMPlayer_h__

#include "Node/Character/XMCharacter.h"



class XMPlayer : public XMCharacter
{
	public :

				 XMPlayer ( KDvoid );
		virtual ~XMPlayer ( KDvoid );
		
		virtual KDbool				shoot ( KDvoid );
		virtual KDbool				slash ( KDvoid );

		virtual KDvoid				move ( KDint nDirectrion );
		virtual KDvoid				hitted ( KDvoid );
	protected :
		
		virtual	KDbool				initWithContentsOfFile ( const std::string& filename );

		virtual KDvoid				setFilterInfo ( KDvoid );

		virtual KDvoid				playIdleAnimation ( KDvoid );
		virtual KDvoid				waitSlash ( KDvoid );

		virtual KDvoid				shootBullet ( KDvoid ) = 0;

		virtual KDvoid				recovery ( KDfloat fDelta );


		KDint						m_nDirection;
		KDint						m_nSlashState;
		KDfloat						m_tResetTime;
};

#endif  // __XMPlayer_h__