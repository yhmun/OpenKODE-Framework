/* --------------------------------------------------------------------------
 *
 *      File            Recipe.h
 *      Created By      Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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

#ifndef __Recipe_h__
#define __Recipe_h__

enum 
{
	TAG_BG			= 1000,
	TAG_GRASS_LEFT  = 1001,
	TAG_GRASS_RIGHT = 1002,
	TAG_RECIPE_BG	= 1003,
	TAG_RECIPE_NAME	= 1004,
	TAG_RECIPE_BACK = 1005,
	TAG_RECIPE_SUB  = 1006,
	TAG_NEXT_BUTTON = 1007,
	TAG_PREV_BUTTON = 1008,
	TAG_RECIPE		= 1009,
	TAG_EFFECT_NODE = 2000,
};

enum 
{
	Z_BG			= -1,
	Z_RECIPE		=  1,
	Z_HUD			= 10,
};

class Recipe : public CCLayer
{
	public :
		
		virtual KDbool				init ( KDvoid );

		virtual KDvoid				drawLayer ( KDvoid );

		virtual KDvoid				initSubMenus ( KDint nSubCount );

		virtual const KDchar*		runSubAction ( KDint nIndex );

		virtual KDvoid				prevSubCallback   ( CCObject* pSender = KD_NULL );
		virtual KDvoid				reloadSubCallback ( CCObject* pSender = KD_NULL );
		virtual KDvoid				nextSubCallback   ( CCObject* pSender = KD_NULL );

		KDvoid						resetMessage ( KDvoid );

		KDvoid						showMessage ( const KDchar* szMsg );

		KDvoid						appendMessage ( const KDchar* szMsg );

	protected :

		CCLabelBMFont*				m_pMessage;
		CCLayerColor*				m_pTitleBG;
		CCLabelTTF*					m_pSubName;
		KDint						m_nSubIndex;
		KDint						m_nSubCount;
};

#endif // __Recipe_h__
