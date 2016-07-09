/* --------------------------------------------------------------------------
 *
 *      File            SysMenu.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors    
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __SysMenu_h__
#define __SysMenu_h__

#include "Global.h"

class SysMenu : public CCLayer
{
	public :

		CREATE_FUNC ( SysMenu );

	protected :

		virtual KDbool  init ( KDvoid );

		KDvoid  onFlareEffect ( CCObject* pSender );

		KDvoid  onNewGame ( CCNode*   pSender );
		KDvoid  onSetting ( CCObject* pSender );
		KDvoid  onAbout   ( CCObject* pSender );

		KDvoid  onUpdate  ( KDfloat fDelta );

	private :

		CCSprite*	m_pShip;
};

#endif // __SysMenu_h__