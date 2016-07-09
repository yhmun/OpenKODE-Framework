/* --------------------------------------------------------------------------
 *
 *      File            MainMenuScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 16/05/2011
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

#ifndef __MainMenuScene_h__
#define __MainMenuScene_h__

class MainMenuScene : public CCLayer 
{
	public :

		SCENE_FUNC ( MainMenuScene );

	public :

		virtual KDbool		init ( KDvoid );

		KDvoid				newGame ( CCObject* pSender );

		KDvoid				instructions ( CCObject* pSender );

		KDvoid				quit ( CCObject* pSender );

		KDvoid				musicToggle ( CCObject* pSender );

		KDvoid				soundToggle ( CCObject* pSender );

		KDvoid				callStartGame ( KDfloat fDelta );
};

#endif	// __MainMenuScene_h__
