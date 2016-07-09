/* --------------------------------------------------------------------------
 *
 *      File            Intro.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Sergey Tikhonov. All rights reserved. 
 *
 *      https://www.github.com/haqu/climbers
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

#ifndef __Intro_h__
#define __Intro_h__

class Intro : public CCLayer
{
	public :	

		SCENE_FUNC ( Intro );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );	

	private :

		CCSprite*			m_pPlayButton;
		CCSprite*			m_pMoreGamesButton;
		CCSprite*			m_pGameSourcesButton;
};

#endif	// __Intro_h__