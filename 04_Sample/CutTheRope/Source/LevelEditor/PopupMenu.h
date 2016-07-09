/* --------------------------------------------------------------------------
 *
 *      File            PopupMenu.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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

#ifndef __PopupMenu_h__
#define __PopupMenu_h__

class PopupMenuDelegate
{
	public :

		virtual KDvoid		createPineappleAt	( CCPoint tPosition ) = 0;
		virtual KDvoid		createRopeAt		( CCPoint tPosition ) = 0;
};

class PopupMenu : public CCLayer
{
	public :
				 PopupMenu ( CCNode* pNode );
		virtual ~PopupMenu ( KDvoid );
	    
	public :
	    
		KDvoid		setPopupPositioin	( CCPoint tPosition );

		KDvoid		setMenuEnabled		( KDbool bEnable );
		KDvoid		setRopeItemEnabled	( KDbool bEnable );
	    
		KDbool		isEnabled			( KDvoid );
	    
		KDvoid		createPineapple		( CCObject* pSender );
		KDvoid		createRope			( CCObject* pSender );
	    
		PopupMenuDelegate*		m_pDelegate;

	private :
	    
		CCSprite*				m_pBackground;
		CCMenu*					m_pMenu;
		CCMenuItem*				m_pRopeItem;
		CCPoint					m_tTapPosition;
		KDbool					m_bIsEnabled;
};

#endif	// __PopupMenu_h__