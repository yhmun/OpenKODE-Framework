/* --------------------------------------------------------------------------
 *
 *      File            MenuLayer.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-10-25
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

#ifndef __MenuLayer_h__
#define __MenuLayer_h__

#include "TeleScope.h"

class MenuLayer : public CCLayer
{
	public :

		SCENE_FUNC ( MenuLayer );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onEnterTransitionDidFinish ( KDvoid );

		KDvoid				textAnimation ( KDvoid );

		KDvoid				playGame ( CCObject* pSender );

	private :

		CCSprite*			m_aTexts [ 4 ];
		CCSprite*			m_aHoles [ 4 ];

		KDint				m_nIndexText;
		TeleScope*			m_pTelScope;
};

#endif	// __MenuLayer_h__

